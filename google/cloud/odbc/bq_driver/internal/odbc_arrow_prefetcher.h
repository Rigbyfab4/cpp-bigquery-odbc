// Copyright 2025 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CPP_BIGQUERY_ODBC_GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_ODBC_ARROW_PREFETCHER_H
#define CPP_BIGQUERY_ODBC_GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_ODBC_ARROW_PREFETCHER_H

#include "google/cloud/odbc/bq_driver/internal/odbc_internal_commons.h"
#include "google/cloud/bigquery/storage/v1/bigquery_read_client.h"
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

namespace google::cloud::odbc_bq_driver_internal {

using google::cloud::odbc_internal::StatusRecord;

// Represents a single processed unit of work.
struct PrefetchedBatch {
  std::vector<DSRow> rows;
  StatusRecord status = StatusRecord::Ok();
  bool is_eos = false; // End of Stream
};

// A thread-safe bounded queue.
class BoundedBlockingQueue {
 public:
  explicit BoundedBlockingQueue(size_t capacity) : capacity_(capacity) {}

  void Push(PrefetchedBatch batch) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_not_full_.wait(lock, [this] { return queue_.size() < capacity_ || shutdown_; });
    if (shutdown_) return;
    queue_.push_back(std::move(batch));
    cv_not_empty_.notify_one();
  }

  PrefetchedBatch Pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_not_empty_.wait(lock, [this] { return !queue_.empty() || shutdown_; });
    
    if (queue_.empty() && shutdown_) {
      // return an error batch indicating cancellation
      return { {}, StatusRecord{google::cloud::odbc_internal::SQLStates::k_HY008(), "Operation canceled."}, true };
    }
    
    PrefetchedBatch batch = std::move(queue_.front());
    queue_.pop_front();
    cv_not_full_.notify_one();
    return batch;
  }

  void Shutdown() {
    std::lock_guard<std::mutex> lock(mutex_);
    shutdown_ = true;
    cv_not_full_.notify_all();
    cv_not_empty_.notify_all();
  }

 private:
  size_t capacity_;
  std::deque<PrefetchedBatch> queue_;
  std::mutex mutex_;
  std::condition_variable cv_not_full_;
  std::condition_variable cv_not_empty_;
  bool shutdown_ = false;
};

// Forward declaration for the processing function refactored in utils.
// It must process into a vector, not a ResultSet.
StatusRecord ProcessRecordBatchToVector(
    std::shared_ptr<arrow::Schema> schema,
    std::shared_ptr<arrow::RecordBatch> record_batch, 
    std::vector<DSRow>& out_rows);

StatusRecordOr<std::shared_ptr<arrow::RecordBatch>> GetArrowRecordBatch(
    ::google::cloud::bigquery::storage::v1::ArrowRecordBatch const& record_batch_in,
    std::shared_ptr<arrow::Schema> schema);

class ArrowPrefetcher {
 public:
  ArrowPrefetcher(
      StreamRange<::google::cloud::bigquery::storage::v1::ReadRowsResponse> stream,
      std::shared_ptr<arrow::Schema> schema)
      : stream_(std::move(stream)),
        arrow_schema_(std::move(schema)),
        queue_(3) {} // Capacity of 3 batches

  ~ArrowPrefetcher() {
    Stop();
  }

  void Start() {
    worker_thread_ = std::thread(&ArrowPrefetcher::WorkerLoop, this);
  }

  void Stop() {
    queue_.Shutdown();
    if (worker_thread_.joinable()) {
      worker_thread_.join();
    }
  }

  PrefetchedBatch GetNextBatch() {
    return queue_.Pop();
  }

 private:
  void WorkerLoop() {
    for (auto const& read_row_status : stream_) {
      // Check cancellation before heavy work
      // Note: We can't easily check queue shutdown state here without locking, 
      // but the Push will handle it.

      if (!read_row_status) {
        queue_.Push({{}, StatusRecord::ConvertFrom(read_row_status.status()), false});
        return;
      }

      auto const& row = *read_row_status;
      if (!row.has_arrow_record_batch()) {
         queue_.Push({{}, StatusRecord{google::cloud::odbc_internal::SQLStates::k_HY000(), "Internal Error: cannot find arrow record batch!"}, false});
         return;
      }

      // Decode Arrow Batch (CPU bound)
      auto record_batch_status = GetArrowRecordBatch(row.arrow_record_batch(), arrow_schema_);
      if (!record_batch_status) {
        queue_.Push({{}, record_batch_status.GetStatusRecord(), false});
        return;
      }

      // Process Rows (CPU bound)
      std::vector<DSRow> processed_rows;
      // Reserve approximate memory if possible, though we don't know exact row count easily without peeking
      // record_batch->num_rows() is available though.
      processed_rows.reserve((*record_batch_status)->num_rows());

      StatusRecord process_status = ProcessRecordBatchToVector(arrow_schema_, *record_batch_status, processed_rows);
      if (!process_status.ok()) {
        queue_.Push({{}, process_status, false});
        return;
      }

      // Push to queue (Blocks if full)
      queue_.Push({std::move(processed_rows), StatusRecord::Ok(), false});
    }

    // End of stream
    queue_.Push({{}, StatusRecord::Ok(), true});
  }

  StreamRange<::google::cloud::bigquery::storage::v1::ReadRowsResponse> stream_;
  std::shared_ptr<arrow::Schema> arrow_schema_;
  BoundedBlockingQueue queue_;
  std::thread worker_thread_;
};

}  // namespace google::cloud::odbc_bq_driver_internal

#endif  // CPP_BIGQUERY_ODBC_GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_ODBC_ARROW_PREFETCHER_H
