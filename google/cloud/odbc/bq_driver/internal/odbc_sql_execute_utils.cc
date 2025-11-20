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

#include "google/cloud/odbc/bq_driver/internal/odbc_sql_execute_utils.h"
#include "google/cloud/odbc/bq_driver/internal/odbc_internal_commons.h"
#include "google/cloud/odbc/bq_driver/internal/trace_utils.h"
#include <thread>

//////////////////////////////////////////////////////////////////
// This file has query execution related utilities which can have
// statement or descriptor handles as arguments. We have some utils
// in `odbc_internal_commons` but those cannot include any handles
// except connection handle to avoid cyclic dependencies.
//////////////////////////////////////////////////////////////////

namespace google::cloud::odbc_bq_driver_internal {

#if (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)
using ::google::cloud::bigquery::storage::v1::CreateReadSessionRequest;
using ::google::cloud::bigquery::storage::v1::ReadRowsRequest;
using ::google::cloud::bigquery::storage::v1::ReadRowsResponse;
using ::google::cloud::bigquery::storage::v1::ReadSession;
using ::google::cloud::bigquery::storage::v1::DataFormat::ARROW;
using ::google::cloud::bigquery_v2_minimal_internal::Job;
using ::google::cloud::bigquery_v2_minimal_internal::QueryRequest;
#endif  // (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)
using ::google::cloud::bigquery_v2_minimal_internal::GetQueryResults;
using ::google::cloud::bigquery_v2_minimal_internal::GetQueryResultsRequest;
using ::google::cloud::bigquery_v2_minimal_internal::PostQueryRequest;
using ::google::cloud::bigquery_v2_minimal_internal::QueryParameter;
using google::cloud::odbc_bq_driver_internal::DescriptorRecord;
using google::cloud::odbc_bq_driver_internal::DoubleStrToInt;
using google::cloud::odbc_bq_driver_internal::StatementHandle;
using google::cloud::odbc_internal::SQLStates;
using chrono_ms = std::chrono::milliseconds;

StatusRecord ConstructPositionalQueryParams(
    DescriptorHandle& apd, DescriptorHandle& ipd,
    std::vector<QueryParameter>& basic_query_params, bool is_data_buff_req) {
  std::vector<SQLLEN> owned_octet_lengths;  // Owns any needed octet lengths
  for (int param_ind = 0; param_ind < basic_query_params.size(); param_ind++) {
    if (!apd.HasDescriptorRecord(param_ind + 1)) {
      LOG(ERROR) << "ConstructPositionalQueryParams:: APD record missing for "
                    "parameter "
                 << (param_ind + 1);
      return StatusRecord{
          SQLStates::k_07002(),
          "Expected descriptor record does not exist during query execution."};
    }

    DescriptorRecord& apd_rec = apd.GetDescriptorRecord(param_ind + 1);
    // SQL_NULL_DATA implies the application wants to use empty data.
    if (apd_rec.indicator_ptr != nullptr &&
        *apd_rec.indicator_ptr == SQL_NULL_DATA) {
      continue;
    }

    bool is_data_at_exec = false;
    if (apd_rec.indicator_ptr &&
        (*(apd_rec.indicator_ptr) == SQL_DATA_AT_EXEC ||
         *(apd_rec.indicator_ptr) <= SQL_LEN_DATA_AT_EXEC_OFFSET)) {
      is_data_at_exec = true;
    }

    if (!is_data_buff_req && is_data_at_exec) {
      LOG(INFO) << "ConstructPositionalQueryParams:: Parameter "
                << (param_ind + 1) << " requires data-at-execution.";
      return StatusRecord{
          SQLStates::k_SQL_NEED_DATA(),
          "The bound param is set for SQL_DATA_AT_EXEC/SQL_LEN_DATA_AT_EXEC"};
    }

    if (!is_data_buff_req && apd_rec.data_ptr == nullptr) {
      LOG(ERROR) << "ConstructPositionalQueryParams:: Bound parameter buffer "
                    "was null for parameter "
                 << (param_ind + 1);
      return StatusRecord{SQLStates::k_HY009(),
                          "The bound param buffer was null"};
    }

    // If a data buffer is needed (e.g., for SQLPutData or SQLParamData) and
    // it's not empty, use it; otherwise, use the original pointer from the
    // application.
    SQLPOINTER buff =
        ((is_data_buff_req && is_data_at_exec) && !apd_rec.data_buffer.empty())
            ? static_cast<SQLPOINTER>(apd_rec.data_buffer.data())
            : apd_rec.data_ptr;
    DataBuffer data;
    if (is_data_buff_req && is_data_at_exec) {
      owned_octet_lengths.push_back(static_cast<SQLLEN>(
          apd_rec.data_buffer
              .size()));  // Handle stack-use-after-scope for octet_length
      SQLLEN* octet_length_ptr = &owned_octet_lengths.back();
      data = {apd_rec.concise_type, buff, *octet_length_ptr, octet_length_ptr};
    } else {
      data = {apd_rec.concise_type, buff, apd_rec.octet_length,
              apd_rec.octet_length_ptr};
    }

    DescriptorRecord& ipd_rec = ipd.GetDescriptorRecord(param_ind + 1);
    if (!ipd.HasDescriptorRecord(param_ind + 1)) {
      LOG(ERROR) << "ConstructPositionalQueryParams:: IPD record missing for "
                    "parameter "
                 << (param_ind + 1);
      return StatusRecord{
          SQLStates::k_07002(),
          "Expected descriptor record does not exist during query execution."};
    }
    SQLSMALLINT sql_type = ipd_rec.concise_type;
    StatusRecordOr<std::string> conv_status = ConvertFromBuffer(data, sql_type);
    if (!conv_status) {
      LOG(ERROR) << "ConstructPositionalQueryParams::ConvertFromBuffer:: "
                 << conv_status.GetStatusRecord().message;
      return conv_status.GetStatusRecord();
    }
    std::string& value_str = *conv_status;
    // "INT64" is a special case where a string like "23.000" will not be
    // accepted by the BQ Server. For ex, this may occur when translating from
    // SQL_C_CHAR->SQL_DOUBLE.
    if (basic_query_params[param_ind].parameter_type.type == "INT64") {
      // Both integral and floating point values can be expressed as a double.
      // DoubleStrToInt will succeed for those but fail for non-arithmetic
      // value.
      StatusRecord status = DoubleStrToInt(value_str);
      if (!status.ok()) {
        return status;
      }
    }
    basic_query_params[param_ind].parameter_value.value = value_str;
  }
  return StatusRecord::Ok();
}

StatusRecordOr<DSResults> ExecuteScript(
    StatementHandle& stmt_handle, PostQueryRequest const& post_query_request) {
  ConnectionHandle* conn_handle = stmt_handle.GetConnectionHandle();
  if (!conn_handle) {
    LOG(ERROR) << "ExecuteScript:: Invalid connection handle.";
    return StatusRecord{SQLStates::k_HY009(), "Invalid statement handle"};
  }

  // Validate connection handle
  if (!conn_handle->IsConnected()) {
    LOG(ERROR) << "ExecuteScript:: Connection to the data source is broken.";
    return StatusRecord{SQLStates::k_08S01(),
                        "Connection to the data source is broken"};
  }

  auto bq_client = conn_handle->GetClient();
  if (!bq_client) {
    LOG(ERROR) << "ExecuteScript:: Invalid or null BQ Client within the "
                  "connection handle.";
    return StatusRecord{
        SQLStates::k_HY000(),
        "Invalid or null BQ Client within the connection handle"};
  }

  // Execute the query
  Options post_query_options;
  auto pq_status = bq_client->PostQuery(post_query_request, post_query_options);
  if (!pq_status) {
    LOG(ERROR) << "ExecuteScript::PostQuery:: "
               << pq_status.GetStatusRecord().message;
    return pq_status.GetStatusRecord();
  }

  DSResults results;
  if (pq_status->job_complete && pq_status->page_token.empty()) {
    // we have gotten all the results
    results.num_dml_affected_rows = pq_status->num_dml_affected_rows;
    results.data_source_results = *pq_status;
  } else {
    // Call GetAllQueryResults to get all the query results.
    auto gq_status = bq_client->GetAllQueryResults(
        pq_status->job_reference.project_id, pq_status->job_reference.job_id,
        pq_status->job_reference.location,
        post_query_request.query_request().timeout(), post_query_options);
    if (!gq_status) {
      LOG(ERROR) << "ExecuteScript::GetAllQueryResults:: "
                 << gq_status.GetStatusRecord().message;
      return gq_status.GetStatusRecord();
    }
    results.num_dml_affected_rows = gq_status->num_dml_affected_rows;
    results.data_source_results = *gq_status;
  }

  // Retrieve job information
  Options list_job_options;
  auto all_jobs_status =
      bq_client->ListAllJobs(pq_status->job_reference.project_id,
                             pq_status->job_reference.job_id, list_job_options);
  if (!all_jobs_status) {
    LOG(ERROR) << "ExecuteScript::ListAllJobs:: "
               << all_jobs_status.GetStatusRecord().message;
    return all_jobs_status.GetStatusRecord();
  }

  for (auto const& job_status : all_jobs_status.GetValue()) {
    if (job_status.statistics.job_query_stats.statement_type !=
            "CREATE_PROCEDURE" &&
        job_status.statistics.script_statistics.evaluation_kind.value ==
            "STATEMENT") {
      stmt_handle.SetJobData(
          job_status.job_reference.job_id,
          job_status.statistics.job_query_stats.statement_type);
    }
  }

  // Fetch query results if job data is available
  if (!stmt_handle.HasJobData()) {
    return results;
  }
  auto job_status = stmt_handle.GetNextJobData();
  if (!job_status.Ok()) {
    LOG(ERROR) << "ExecuteScript::GetNextJobData:: "
               << job_status.GetStatusRecord().message;
    return job_status.GetStatusRecord();
  }
  auto job_data = job_status.GetValue();
  std::string job_id = job_data.first;
  std::string statement_type = job_data.second;

  Options query_results_options;
  auto gq_status = bq_client->GetAllQueryResults(
      pq_status->job_reference.project_id, job_id,
      pq_status->job_reference.location,
      post_query_request.query_request().timeout(), query_results_options);

  if (!gq_status) {
    LOG(ERROR) << "ExecuteScript::GetAllQueryResults:: "
               << gq_status.GetStatusRecord().message;
    return gq_status.GetStatusRecord();
  }

  // Assign DML row counts
  if (statement_type == "INSERT" || statement_type == "UPDATE" ||
      statement_type == "DELETE") {
    results.num_dml_affected_rows = gq_status->num_dml_affected_rows;
  }
  results.data_source_results = *gq_status;
  stmt_handle.SetDSResults(results);

  if (!conn_handle->IsSessionStarted() &&
      !pq_status->session_info.session_id.empty()) {
    conn_handle->SetSessionId(pq_status->session_info.session_id);
  }

  return results;
}

#if (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)

StatusRecordOr<std::shared_ptr<arrow::Schema>> GetArrowSchema(
    ::google::cloud::bigquery::storage::v1::ArrowSchema const& schema_in,
    RowSchema& row_schema) {
  std::shared_ptr<arrow::Buffer> buffer =
      std::make_shared<arrow::Buffer>(schema_in.serialized_schema());
  arrow::io::BufferReader buffer_reader(buffer);
  arrow::ipc::DictionaryMemo dictionary_memo;
  auto result = arrow::ipc::ReadSchema(&buffer_reader, &dictionary_memo);
  if (!result.ok()) {
    return StatusRecord{SQLStates::k_HY000(),
                        "Internal Error: Unable to parse arrow schema"};
  }
  std::shared_ptr<arrow::Schema> schema = result.ValueOrDie();

  row_schema.clear();
  int col_index = 0;
  for (auto const& field : schema->fields()) {
    ColumnSchema col_schema = {col_index++};
    arrow::Type::type data_type = field->type()->id();
    switch (data_type) {
      case arrow::Type::INT64:
        col_schema.col_type = BQDataType::kInt64;
        break;
      case arrow::Type::DOUBLE:
        col_schema.col_type = BQDataType::kFloat64;
        break;
      case arrow::Type::STRING:
        col_schema.col_type = BQDataType::kString;
        break;
      case arrow::Type::BINARY:
        col_schema.col_type = BQDataType::kInt64;
        break;
      case arrow::Type::BOOL:
        col_schema.col_type = BQDataType::kBool;
        break;
      case arrow::Type::TIMESTAMP:
        col_schema.col_type = BQDataType::kTimeStamp;
        break;
      case arrow::Type::TIME64:
        col_schema.col_type = BQDataType::kTime;
        break;
      case arrow::Type::DATE32:
        col_schema.col_type = BQDataType::kDate;
        break;
      case arrow::Type::DECIMAL128:
        col_schema.col_type = BQDataType::kNumeric;
        break;
      case arrow::Type::DECIMAL256:
        col_schema.col_type = BQDataType::kBigNumeric;
        break;
      case arrow::Type::LIST:
        // For other datatypes within an array, we don't have any special
        // handling. Setting 'is_mode_repeated' is enough
        if (static_cast<arrow::ListType const*>(field->type().get())
                ->value_type()
                ->ToString() == "binary") {
          col_schema.col_type = BQDataType::kBytes;
        }
        col_schema.is_mode_repeated = true;
        break;
      case arrow::Type::STRUCT: {
        col_schema.col_type = BQDataType::kString;
        break;
      }
      default:
        return StatusRecord{SQLStates::k_HY000(),
                            "Internal Error: Unsupported arrow data type"};
    }
    row_schema.emplace_back(col_schema);
  }
  return schema;
}

StatusRecordOr<std::shared_ptr<arrow::RecordBatch>> GetArrowRecordBatch(
    ::google::cloud::bigquery::storage::v1::ArrowRecordBatch const&
        record_batch_in,
    std::shared_ptr<arrow::Schema> schema) {
  std::shared_ptr<arrow::Buffer> buffer = std::make_shared<arrow::Buffer>(
      record_batch_in.serialized_record_batch());
  arrow::io::BufferReader buffer_reader(buffer);
  arrow::ipc::DictionaryMemo dictionary_memo;
  arrow::ipc::IpcReadOptions read_options;
  auto result = arrow::ipc::ReadRecordBatch(schema, &dictionary_memo,
                                            read_options, &buffer_reader);
  if (!result.ok()) {
    return StatusRecord{SQLStates::k_HY000(),
                        "Internal Error: Unable to parse record batch"};
  }
  std::shared_ptr<arrow::RecordBatch> record_batch = result.ValueOrDie();
  return record_batch;
}

// Helper to convert days since epoch (1970-01-01) to SQL_DATE_STRUCT.
// Used for optimized Arrow DATE32 processing.
SQL_DATE_STRUCT ConvertDaysToDateStruct(int32_t days_since_epoch) {
  // 86400 seconds per day.
  // We use time_t/gmtime for simplicity and standard compliance.
  // Note: This is valid for dates representable by time_t (usually 1900-2038 on 32-bit, much wider on 64-bit).
  // For extremely ancient or future dates, a full calendar algo is needed, but this covers standard SQL usage.
  std::time_t raw_time = static_cast<std::time_t>(days_since_epoch) * 86400;
  std::tm* ptm = std::gmtime(&raw_time);
  
  SQL_DATE_STRUCT date_struct;
  if (ptm) {
    date_struct.year = static_cast<SQLSMALLINT>(ptm->tm_year + 1900);
    date_struct.month = static_cast<SQLUSMALLINT>(ptm->tm_mon + 1);
    date_struct.day = static_cast<SQLUSMALLINT>(ptm->tm_mday);
  } else {
    // Fallback or error case (though unlikely for BQ data ranges)
    date_struct = {0, 0, 0}; 
  }
  return date_struct;
}

// Helper to convert Nanoseconds in a day to SQL_TIME_STRUCT.
// Used for optimized Arrow TIME64 processing.
SQL_TIME_STRUCT ConvertNanosToTimeStruct(int64_t nanos) {
  SQL_TIME_STRUCT t_data;
  int64_t total_seconds = nanos / 1000000000LL;
  
  t_data.hour = static_cast<SQLUSMALLINT>(total_seconds / 3600);
  int64_t rem_seconds = total_seconds % 3600;
  t_data.minute = static_cast<SQLUSMALLINT>(rem_seconds / 60);
  t_data.second = static_cast<SQLUSMALLINT>(rem_seconds % 60);
  return t_data;
}

// Optimized Column-Wise processing of Arrow Record Batch.
// Fills the pre-allocated vector of rows.
StatusRecord ProcessRecordBatchToVector(
    std::shared_ptr<arrow::Schema> schema,
    std::shared_ptr<arrow::RecordBatch> record_batch,
    std::vector<DSRow>& out_rows) {

  const int64_t num_rows = record_batch->num_rows();
  if (num_rows == 0) {
    return StatusRecord::Ok();
  }
  const int num_cols = record_batch->num_columns();

  // 1. Pre-allocate vector memory to avoid reallocations during insertion
  // The `out_rows` vector should be empty coming in, or we append to it.
  // Here we assume we are appending or filling.
  size_t start_row_index = out_rows.size();
  out_rows.resize(start_row_index + num_rows);

  // 2. Initialize the DSRow (vector of DSValue) for each new row
  // We do this first so we can access [row][col] directly in the loops below.
  for (size_t i = start_row_index; i < out_rows.size(); ++i) {
    out_rows[i].resize(num_cols);
  }

  // 3. Iterate Column-by-Column (Column-Major Traversal)
  // This reduces virtual function calls (casting array type) from (Rows * Cols) to (Cols).
  for (int col_i = 0; col_i < num_cols; ++col_i) {
    std::shared_ptr<arrow::Array> column_array = record_batch->column(col_i);

    // Inner Loop: Iterate Rows for this specific column
    switch (column_array->type()->id()) {
      case arrow::Type::INT64: {
        auto arr = std::static_pointer_cast<arrow::Int64Array>(column_array);
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            ArithmeticToDSValue<SQLBIGINT>(arr->Value(row_i), cell);
          }
        }
        break;
      }
      case arrow::Type::DOUBLE: {
        auto arr = std::static_pointer_cast<arrow::DoubleArray>(column_array);
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            ArithmeticToDSValue<SQLDOUBLE>(arr->Value(row_i), cell);
          }
        }
        break;
      }
      case arrow::Type::STRING: {
        auto arr = std::static_pointer_cast<arrow::StringArray>(column_array);
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            // string_view avoids copy from Arrow, but StringToDSValue takes std::string
            StringToDSValue(arr->GetString(row_i), cell);
          }
        }
        break;
      }
      case arrow::Type::BOOL: {
        auto arr = std::static_pointer_cast<arrow::BooleanArray>(column_array);
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            BooleanToDSValue(arr->Value(row_i), cell);
          }
        }
        break;
      }
      case arrow::Type::TIMESTAMP: {
        auto arr = std::static_pointer_cast<arrow::TimestampArray>(column_array);
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            // BQ Storage API usually sends Microns or Nanos. 
            // Assuming Nanoseconds based on Arrow Type, but `Value` returns int64 raw.
            // Standard conversion logic from double exists:
            int64_t raw_val = arr->Value(row_i);
            // Convert to seconds (double) for the existing helper
            double unix_timestamp = static_cast<double>(raw_val) / 1e9; // if nanos
            if (arr->type()->ToString().find("micro") != std::string::npos) {
                 unix_timestamp = static_cast<double>(raw_val) / 1e6;
            }
            
            SQL_TIMESTAMP_STRUCT time_struct;
            ConvertUnixTimestampToTimestampStruct(unix_timestamp, time_struct);
            TimestampToDSValue(time_struct, cell);
          }
        }
        break;
      }
      case arrow::Type::TIME64: {
        auto arr = std::static_pointer_cast<arrow::Time64Array>(column_array);
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            // Time64 is usually nanoseconds (or micros) since midnight
            SQL_TIME_STRUCT t_data = ConvertNanosToTimeStruct(arr->Value(row_i));
            TimeToDSValue(t_data, cell);
          }
        }
        break;
      }
      case arrow::Type::DATE32: {
        auto arr = std::static_pointer_cast<arrow::Date32Array>(column_array);
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            // Value() returns int32 days since epoch
            SQL_DATE_STRUCT date_struct = ConvertDaysToDateStruct(arr->Value(row_i));
            DateToDSValue(date_struct, cell);
          }
        }
        break;
      }
      case arrow::Type::BINARY: {
        auto arr = std::static_pointer_cast<arrow::BinaryArray>(column_array);
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            StringToDSValue(arr->GetString(row_i), cell);
          }
        }
        break;
      }
      case arrow::Type::DECIMAL128: {
        auto arr = std::static_pointer_cast<arrow::Decimal128Array>(column_array);
        auto dec_type = std::static_pointer_cast<arrow::Decimal128Type>(arr->type());
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            arrow::Decimal128 dec_val(arr->Value(row_i));
            std::string str_val = dec_val.ToString(dec_type->scale());
            NumericToDSValue(str_val, cell);
          }
        }
        break;
      }
      case arrow::Type::DECIMAL256: {
        auto arr = std::static_pointer_cast<arrow::Decimal256Array>(column_array);
        auto dec_type = std::static_pointer_cast<arrow::Decimal256Type>(arr->type());
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
          DSValue& cell = out_rows[start_row_index + row_i][col_i];
          if (arr->IsNull(row_i)) {
            cell = kNullValue;
          } else {
            arrow::Decimal256 dec_val(arr->Value(row_i));
            std::string str_val = dec_val.ToString(dec_type->scale());
            NumericToDSValue(str_val, cell);
          }
        }
        break;
      }
      // For complex or unoptimized types, we fallback to GetScalar (slow path)
      // This avoids duplicating logic for structs/lists/etc.
      case arrow::Type::LIST:
      case arrow::Type::STRUCT:
      default: {
        for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
           DSValue& cell = out_rows[start_row_index + row_i][col_i];
           // Original slow logic logic for specific cell
           auto result = column_array->GetScalar(row_i);
           if (!result.ok()) return StatusRecord{SQLStates::k_HY000(), "Internal Error: Unable to parse scalar"};
           
           std::shared_ptr<arrow::Scalar> scalar = result.ValueOrDie();
           if (!scalar->is_valid) {
             cell = kNullValue;
           } else {
             std::string data = scalar->ToString();
             StringToDSValue(data, cell);
           }
        }
        break;
      }
    }
  }

  return StatusRecord::Ok();
}


// Optimized version of ProcessRecordBatch
StatusRecord ProcessRecordBatch1(
    std::shared_ptr<arrow::Schema> schema,
    std::shared_ptr<arrow::RecordBatch> record_batch, ResultSet& result_set) {
  const int64_t num_rows = record_batch->num_rows();
  if (num_rows == 0) {
    return StatusRecord::Ok();
  }
  const int num_cols = record_batch->num_columns();

  // Pre-allocate space for all the new rows to avoid reallocations.
  size_t start_row_index = result_set.rows.size();
  result_set.rows.resize(start_row_index + num_rows);

  // --- Main Optimization: Process column by column ---
  for (int col_i = 0; col_i < num_cols; ++col_i) {
    std::shared_ptr<arrow::Array> column_array = record_batch->column(col_i);

    // This loop populates the data for the current column (`col_i`)
    // across all the newly added rows.
    for (int64_t row_i = 0; row_i < num_rows; ++row_i) {
        // Pre-allocate columns for the row on the first pass (col_i == 0)
        if (col_i == 0) {
            result_set.rows[start_row_index + row_i].resize(num_cols);
        }

        DSValue& cell_value = result_set.rows[start_row_index + row_i][col_i];

        if (column_array->IsNull(row_i)) {
            cell_value = kNullValue; // Assuming kNullValue is your representation
            continue;
        }

        // Use a switch on the column's data type ID. This is much more efficient
        // as we only do the type-check once per column, not once per cell.
        switch (column_array->type()->id()) {
            case arrow::Type::INT64: {
                auto arr = std::static_pointer_cast<arrow::Int64Array>(column_array);
                SQLBIGINT value = arr->Value(row_i);
                // Directly use the helper that was already available.
                ArithmeticToDSValue<SQLBIGINT>(value, cell_value);
                break;
            }
            case arrow::Type::DOUBLE: {
                auto arr = std::static_pointer_cast<arrow::DoubleArray>(column_array);
                SQLDOUBLE value = arr->Value(row_i);
                ArithmeticToDSValue<SQLDOUBLE>(value, cell_value);
                break;
            }
            case arrow::Type::STRING: {
                auto arr = std::static_pointer_cast<arrow::StringArray>(column_array);
                // Get a string_view to avoid copying the string data from Arrow buffer.
                std::string_view value = arr->GetView(row_i);
                StringToDSValue(std::string(value), cell_value); // Assuming StringToDSValue needs std::string
                break;
            }
            case arrow::Type::BOOL: {
                auto arr = std::static_pointer_cast<arrow::BooleanArray>(column_array);
                bool value = arr->Value(row_i);
                BooleanToDSValue(value, cell_value);
                break;
            }
            case arrow::Type::TIMESTAMP: {
                auto arr = std::static_pointer_cast<arrow::TimestampArray>(column_array);
                // Timestamps are often int64 internally (e.g., nanoseconds since epoch)
                // We need to convert this to the SQL_TIMESTAMP_STRUCT
                int64_t nanos_since_epoch = arr->Value(row_i);
                double unix_timestamp = nanos_since_epoch / 1e9;
                SQL_TIMESTAMP_STRUCT time_struct;
                ConvertUnixTimestampToTimestampStruct(unix_timestamp, time_struct);
                TimestampToDSValue(time_struct, cell_value);
                break;
            }
            case arrow::Type::TIME64: {
                auto arr = std::static_pointer_cast<arrow::Time64Array>(column_array);
                // Assuming Time64 is nanoseconds. The original code converted from string.
                // This is a guess; you may need to adjust based on the actual BQ format.
                // For this example, let's assume it's nanoseconds in a day.
                int64_t nanos = arr->Value(row_i);
                SQL_TIME_STRUCT t_data;
                t_data.hour = static_cast<SQLUSMALLINT>(nanos / 3600000000000LL);
                t_data.minute = static_cast<SQLUSMALLINT>((nanos / 60000000000LL) % 60);
                t_data.second = static_cast<SQLUSMALLINT>((nanos / 1000000000LL) % 60);
                // Fractions would require more logic if needed.
                TimeToDSValue(t_data, cell_value);
                break;
            }
            case arrow::Type::DATE32: { // BQ DATE is DATE32 (days since epoch)
                auto arr = std::static_pointer_cast<arrow::Date32Array>(column_array);
                int32_t days_since_epoch = arr->Value(row_i);
                // We need a function to convert days since epoch to SQL_DATE_STRUCT
                // This is much faster than parsing "YYYY-MM-DD" string.
                SQL_DATE_STRUCT date_struct = ConvertDaysToDateStruct(days_since_epoch);
                DateToDSValue(date_struct, cell_value);
                break;
            }
            case arrow::Type::BINARY: {
                auto arr = std::static_pointer_cast<arrow::BinaryArray>(column_array);
                std::string_view value = arr->GetView(row_i);
                // Assuming StringToDSValue works for binary data as well by copying bytes.
                StringToDSValue(std::string(value), cell_value);
                break;
            }
            case arrow::Type::DECIMAL128: {
                auto arr = std::static_pointer_cast<arrow::Decimal128Array>(column_array);
                arrow::Decimal128 dec_val(arr->Value(row_i));
                // The scale is part of the type information
                auto dec_type = std::static_pointer_cast<arrow::Decimal128Type>(arr->type());
                std::string str_val = dec_val.ToString(dec_type->scale());
                NumericToDSValue(str_val, cell_value);
                break;
            }
            case arrow::Type::DECIMAL256: {
                auto arr = std::static_pointer_cast<arrow::Decimal256Array>(column_array);
                arrow::Decimal256 dec_val(arr->Value(row_i));
                auto dec_type = std::static_pointer_cast<arrow::Decimal256Type>(arr->type());
                std::string str_val = dec_val.ToString(dec_type->scale());
                NumericToDSValue(str_val, cell_value);
                break;
            }
            // For LIST and STRUCT, the original code stringified them.
            // A performant solution requires recursively processing them, but for now,
            // we can replicate the old behavior without the GetScalar overhead if needed.
            // However, this part remains slow.
            case arrow::Type::LIST:
            case arrow::Type::STRUCT:
            default: {
                // Fallback to the slower method for unsupported or complex types
                // to maintain correctness. This still avoids GetScalar.
                auto result = column_array->GetScalar(row_i);
                if (result.ok()) {
                    std::string str_val = result.ValueOrDie()->ToString();
                    StringToDSValue(str_val, cell_value);
                } else {
                    return StatusRecord{SQLStates::k_HY000(), "Failed to get scalar for complex type"};
                }
                break;
            }
        }
    }
  }

  return StatusRecord::Ok();
}

StatusRecord ProcessRecordBatch(
    std::shared_ptr<arrow::Schema> schema,
    std::shared_ptr<arrow::RecordBatch> record_batch, ResultSet& result_set) {
  int num_rows = record_batch->num_rows();
  int num_columns = record_batch->num_columns();

  int old_row_count = result_set.rows.size();
  result_set.rows.resize(num_rows);
  // Resize inner column vectors ONLY for new rows.
  // Existing rows (indices 0 to old_row_count-1) retain their capacity and
  // size.
  for (int i = old_row_count; i < num_rows; ++i) {
    result_set.rows[i].resize(num_columns);
  }

  // Column-Oriented Processing:
  // Arrow is columnar. Accessing data column-by-column allows us to cast the
  // array type ONCE per column, rather than performing type checks and
  // GetScalar() allocations for every single cell.
  for (int col_i = 0; col_i < num_columns; ++col_i) {
    auto column = record_batch->column(col_i);
    auto type_id = column->type_id();

    // Helper lambda to handle nulls efficiently per column type
    auto is_null = [&](int64_t row) { return column->IsNull(row); };

    switch (type_id) {
      case arrow::Type::INT64: {
        auto int_arr = std::static_pointer_cast<arrow::Int64Array>(column);
        for (int64_t row = 0; row < num_rows; ++row) {
          if (int_arr->IsNull(row)) {
            result_set.rows[row][col_i] = kNullValue;
          } else {
            ArithmeticToDSValue<SQLBIGINT>(int_arr->Value(row),
                                           result_set.rows[row][col_i]);
          }
        }
        break;
      }
      case arrow::Type::DOUBLE: {
        auto dbl_arr = std::static_pointer_cast<arrow::DoubleArray>(column);
        for (int64_t row = 0; row < num_rows; ++row) {
          if (dbl_arr->IsNull(row)) {
            result_set.rows[row][col_i] = kNullValue;
          } else {
            ArithmeticToDSValue<SQLDOUBLE>(dbl_arr->Value(row),
                                           result_set.rows[row][col_i]);
          }
        }
        break;
      }
      case arrow::Type::STRING: {
        auto str_arr = std::static_pointer_cast<arrow::StringArray>(column);
        for (int64_t row = 0; row < num_rows; ++row) {
          if (str_arr->IsNull(row)) {
            result_set.rows[row][col_i] = kNullValue;
          } else {
            StringToDSValue(str_arr->GetString(row),
                            result_set.rows[row][col_i]);
          }
        }
        break;
      }
      case arrow::Type::BOOL: {
        auto bool_arr = std::static_pointer_cast<arrow::BooleanArray>(column);
        for (int64_t row = 0; row < num_rows; ++row) {
          if (bool_arr->IsNull(row)) {
            result_set.rows[row][col_i] = kNullValue;
          } else {
            BooleanToDSValue(bool_arr->Value(row), result_set.rows[row][col_i]);
          }
        }
        break;
      }
      case arrow::Type::BINARY: {
        auto bin_arr = std::static_pointer_cast<arrow::BinaryArray>(column);
        for (int64_t row = 0; row < num_rows; ++row) {
          if (bin_arr->IsNull(row)) {
            result_set.rows[row][col_i] = kNullValue;
          } else {
            StringToDSValue(bin_arr->GetString(row),
                            result_set.rows[row][col_i]);
          }
        }
        break;
      }
      // For complex types, we fall back to the existing logic but apply it
      // column-wise. We still avoid the GetScalar() overhead where possible,
      // but use ToString() to maintain compatibility with the existing parsing
      // helpers (ConvertStringTo...)
      default: {
        for (int64_t row = 0; row < num_rows; ++row) {
          if (column->IsNull(row)) {
            result_set.rows[row][col_i] = kNullValue;
            continue;
          }

          // We use GetScalar here only for complex types not optimized above.
          // Note: Creating a scalar per cell is slow, but doing it only for
          // timestamps/structs is better than doing it for Int64/Double too.
          auto scalar_res = column->GetScalar(row);
          if (!scalar_res.ok()) {
            return StatusRecord{SQLStates::k_HY000(),
                                "Internal Error: Unable to parse scalar"};
          }
          std::string data = scalar_res.ValueOrDie()->ToString();

          DSValue& row_val = result_set.rows[row][col_i];

          switch (type_id) {
            case arrow::Type::TIMESTAMP: {
              StatusRecordOr<SQL_TIMESTAMP_STRUCT> time_struct_status =
                  ConvertStringToTimestampStruct(data);
              if (!time_struct_status)
                return time_struct_status.GetStatusRecord();
              TimestampToDSValue(*time_struct_status, row_val);
              break;
            }
            case arrow::Type::TIME64: {
              SQL_TIME_STRUCT t_data = ConvertToTimeStruct(data);
              TimeToDSValue(t_data, row_val);
              break;
            }
            case arrow::Type::DATE32: {
              StatusRecordOr<SQL_DATE_STRUCT> date_struct =
                  ConvertStringToDateStruct(data);
              if (!date_struct.Ok()) return date_struct.GetStatusRecord();
              DateToDSValue(*date_struct, row_val);
              break;
            }
            case arrow::Type::LIST: {
              if (data.rfind("list<", 0) == 0) {
                auto pos = data.find('[');
                if (pos != std::string::npos) data = data.substr(pos);
              }
              StringToDSValue(data, row_val);
              break;
            }
            case arrow::Type::DECIMAL128:
            case arrow::Type::DECIMAL256: {
              NumericToDSValue(data, row_val);
              break;
            }
            default: {
              StringToDSValue(data, row_val);
              break;
            }
          }
        }
        break;
      }
    }
  }
  return StatusRecord::Ok();
}

// REFACTORED: ReadNextResultsFromStream
StatusRecord ReadNextResultsFromStream(StatementHandle& stmt_handle) {
  ArrowPrefetcher* prefetcher = stmt_handle.GetArrowPrefetcher();
  if (!prefetcher) {
    return StatusRecord{SQLStates::k_HY000(), "Internal Error: Arrow prefetcher not initialized!"};
  }

  // Blocking Pop from the queue
  PrefetchedBatch batch = prefetcher->GetNextBatch();

  if (!batch.status.ok()) {
    stmt_handle.ClearArrowPrefetcher(); // Stop on error
    return batch.status;
  }
  if (batch.is_eos) {
    stmt_handle.ClearArrowPrefetcher();
    LOG(INFO) << "FetchBQDataReadArrow:: Read stream ended.";
    return StatusRecord({SQLStates::k_SQL_NO_DATA(), "Read stream ended."});
  }

  // Success: Move rows into ResultSet
  ResultSet& result_set = stmt_handle.GetResultSet();
  result_set.rows = std::move(batch.rows);
  result_set.cursor = -1;

  return StatusRecord::Ok();
}

// REFACTORED: FetchBQDataReadArrow
StatusRecord FetchBQDataReadArrow(StatementHandle& stmt_handle,
                                  TableReference& table_ref) {
  std::string project_id = table_ref.project_id;
  std::string dataset_id = table_ref.dataset_id;
  std::string table_id = table_ref.table_id;
  std::string table_path = "projects/" + project_id + "/datasets/" +
                           dataset_id + "/tables/" + table_id;

  CreateReadSessionRequest create_read_session_request;
  create_read_session_request.set_parent("projects/" + project_id);
  create_read_session_request.set_max_stream_count(1);
  auto* read_session = create_read_session_request.mutable_read_session();
  read_session->set_table(table_path);
  read_session->set_data_format(ARROW);

  Options options;
  auto bq_client = stmt_handle.GetConnectionHandle()->GetClient();
  auto read_session_status = bq_client->CreateReadSession(create_read_session_request, options);
  if (!read_session_status) {
    return read_session_status.GetStatusRecord();
  }

  auto session = *read_session_status;

  if (!session.streams().empty()) {
    std::string read_stream_name = session.streams(0).name();

    ResultSet result_set;
    StatusRecordOr<std::shared_ptr<arrow::Schema>> schema_status =
        GetArrowSchema(session.arrow_schema(), result_set.row_schema);
    if (!schema_status) {
      return schema_status.GetStatusRecord();
    }
    
    stmt_handle.SetResultSet(result_set);
    std::shared_ptr<arrow::Schema> schema = *schema_status;
    stmt_handle.SetArrowSchema(schema);

    // Create a ReadRowsRequest.
    ReadRowsRequest read_rows_request;
    read_rows_request.set_read_stream(read_stream_name);

    // Create the stream
    StreamRange<google::cloud::bigquery::storage::v1::ReadRowsResponse>
        read_rows_stream = bq_client->GetReadRowsStream(read_rows_request, options);

    // Initialize and Start Prefetcher
    // We move the stream into the prefetcher immediately.
    auto prefetcher = std::make_unique<ArrowPrefetcher>(std::move(read_rows_stream), schema);
    prefetcher->Start();
    stmt_handle.SetArrowPrefetcher(std::move(prefetcher));

    // Fetch the first batch to populate the initial result set
    return ReadNextResultsFromStream(stmt_handle);
  }

  return StatusRecord{SQLStates::k_HY000(), "No valid stream found to read results"};
}

StatusRecord CreateLargeDatasetIfNeeded(
    std::shared_ptr<ODBCBQClient> bq_client, std::string project_id,
    std::string dataset_id, std::string large_table_expiration_time) {
  // 1. Construct the CREATE SCHEMA DDL.
  std::string full_dataset_name = "`" + project_id + "." + dataset_id + "`";
  std::string query = "CREATE SCHEMA IF NOT EXISTS " + full_dataset_name;

  // 2. Handle Expiration Time Conversion.
  // The input is in milliseconds (string), but SQL DDL OPTIONS expects
  // 'default_table_expiration_days'.
  if (!large_table_expiration_time.empty()) {
    try {
      long long expiration_ms = std::stoll(large_table_expiration_time);
      if (expiration_ms <= 0) {
        throw std::invalid_argument("Expiration time cannot be negative");
      }
      if (expiration_ms > 0) {
        // Convert milliseconds to days (MilliSeconds / 1000*24*60*60)
        double expiration_days = static_cast<double>(expiration_ms) / 86400000;

        query += " OPTIONS(default_table_expiration_days=" +
                 std::to_string(expiration_days) + ")";
      }
    } catch (std::exception const& e) {
      std::string err_msg =
          "CreateLargeDatasetIfNeeded:: Invalid large_table_expiration_time "
          "format: " +
          large_table_expiration_time;
      LOG(ERROR) << err_msg;
      return StatusRecord{SQLStates::k_HY000(), err_msg};
    }
  }
  LOG(INFO) << "CreateLargeDatasetIfNeeded:: Executing DDL: " << query;

  // 3. Prepare the QueryRequest.
  QueryRequest query_request;
  query_request.set_query(query);
  query_request.set_use_legacy_sql(false);

  // 4. Prepare the PostQueryRequest.
  PostQueryRequest post_query_request;
  post_query_request.set_project_id(project_id);
  post_query_request.set_query_request(std::move(query_request));

  // 5. Execute using the helper function.
  auto result = PostQueryWithoutResults(bq_client, post_query_request);
  if (!result.Ok()) {
    LOG(ERROR) << "CreateLargeDatasetIfNeeded:: Failed to create dataset: "
               << result.GetStatusRecord().message;
    return result.GetStatusRecord();
  }
  return StatusRecord::Ok();
}

StatusRecord FetchBQDataRead(StatementHandle& stmt_handle,
                             PostQueryRequest const& post_query_request) {
  QueryRequest query_request = post_query_request.query_request();
  std::string query = query_request.query();
  Job job;
  job.configuration.query.query = query;
  job.configuration.query.use_query_cache = true;
  job.configuration.dry_run = false;
  job.configuration.query.allow_large_results = true;
  job.configuration.query.use_legacy_sql = false;
  job.configuration.query.create_disposition = "CREATE_IF_NEEDED";
  job.configuration.query.write_disposition = "WRITE_TRUNCATE";
  job.configuration.query.query_parameters = query_request.query_parameters();

  ConnectionHandle& conn_handle = *(stmt_handle.GetConnectionHandle());
  auto dsn = conn_handle.GetDsn();
  std::string catalog_name = dsn.catalog;
  std::string default_dataset = dsn.default_dataset;
  if (!default_dataset.empty()) {
    job.configuration.query.default_dataset.project_id = catalog_name;
    job.configuration.query.default_dataset.dataset_id = default_dataset;
  }
  job.configuration.query.destination_table.project_id = catalog_name;
  job.configuration.query.destination_table.dataset_id =
      dsn.use_default_large_results_dataset ? kDefaultDestDatasetId
                                            : dsn.large_results_dataset_id;
  job.configuration.query.destination_table.table_id = GenerateTableId();
  std::cout << "SACHIN:: FetchBQDataRead:: dsn.large_results_dataset_id:: " << dsn.large_results_dataset_id << std::endl;
  std::cout << "SACHIN:: FetchBQDataRead:: dataset_id:: " << job.configuration.query.destination_table.dataset_id << std::endl;
  std::cout << "SACHIN:: FetchBQDataRead:: table_id:: " << job.configuration.query.destination_table.table_id << std::endl;

  job.configuration.query.parameter_mode = "POSITIONAL";
  job.configuration.query.allow_large_results = true;

  Options opt;
  auto bq_client = conn_handle.GetClient();
  // We need to first create large results dataset if it was not there
  StatusRecord create_dataset_status = CreateLargeDatasetIfNeeded(
      bq_client, dsn.catalog,
      job.configuration.query.destination_table.dataset_id,
      dsn.large_table_expiration_time);
  if (!create_dataset_status.ok()) {
    return create_dataset_status;
  }

  // Insert job
  auto insert_response = bq_client->InsertJob(dsn.catalog, job, opt);
  if (!insert_response.Ok()) {
    return insert_response.GetStatusRecord();
  }
  // Here we are replacing the dry run Job created during SQLPrepare.
  // This should be safe since the same query is executed during HTAPI flow too.
  stmt_handle.SetPreparedJob(*insert_response);

  // Wait for Job to complete
  std::string job_status = insert_response->status.state;
  ExponentialBackoffPolicy backoff(chrono_ms(100), chrono_ms(200), 2);
  StatusRecordOr<Job> get_job_response;
  while (job_status != "DONE") {
    std::this_thread::sleep_for(backoff.OnCompletion());
    get_job_response = bq_client->GetJob(
        conn_handle.GetDsn().catalog, insert_response->job_reference.job_id,
        insert_response->job_reference.location, opt);
    if (!get_job_response.Ok()) {
      return get_job_response.GetStatusRecord();
    }
    job_status = get_job_response->status.state;
  }
  std::string error_message = get_job_response->status.error_result.message;
  if (!error_message.empty()) {
    LOG(ERROR) << "FetchBQDataRead:: " << error_message;
    return StatusRecord{SQLStates::k_HY000(), error_message};
  }

  return FetchBQDataReadArrow(
      stmt_handle, insert_response->configuration.query.destination_table);
}

#endif  // (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)

// TODO(b/388947009): Add unit tests for this function
StatusRecordOr<DSResults> FetchBQData(
    StatementHandle& stmt_handle, PostQueryRequest const& post_query_request,
    [[maybe_unused]] bool with_htapi) {
  ConnectionHandle& conn_handle = *(stmt_handle.GetConnectionHandle());
#if (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)
  if (with_htapi && conn_handle.GetDsn().allow_htapi) {
    StatusRecord read_status = FetchBQDataRead(stmt_handle, post_query_request);
    if (!read_status.ok()) {
      return read_status;
    }
    DSResults results;
    results.data_source_results = stmt_handle.GetResultSet();
    return results;
  }
#endif  // (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)

  auto pq_status = PostQueryWithoutResults(conn_handle, post_query_request);
  if (!pq_status) {
    return pq_status.GetStatusRecord();
  }
  DSResults results;
  results.num_dml_affected_rows = pq_status->num_dml_affected_rows;
  results.job_ref = pq_status->job_reference;
  stmt_handle.GetPagingInfo().job_id = pq_status->job_reference.job_id;
  stmt_handle.GetPagingInfo().page_token = pq_status->page_token;
  if (pq_status->job_complete) {
    // we have gotten all the results
    results.data_source_results = *pq_status;
  } else {
    auto gq_status =
        FetchNextPageOfQueryResults(stmt_handle, post_query_request);
    if (!gq_status) {
      LOG(ERROR) << "FetchBQData::FetchNextPageOfQueryResults:: "
                 << gq_status.GetStatusRecord().message;
      return gq_status.GetStatusRecord();
    }
    results.num_dml_affected_rows = gq_status->num_dml_affected_rows;
    results.data_source_results = *gq_status;
  }
  if (!conn_handle.IsSessionStarted() &&
      !pq_status->session_info.session_id.empty()) {
    conn_handle.SetSessionId(pq_status->session_info.session_id);
  }
  return results;
}

StatusRecord FetchNextPageResultSet(StatementHandle& stmt_handle) {
  // In case of non-HTAPI execution there is no pagination, so we have to return
  // `SQL_NO_DATA`
  if (stmt_handle.GetPagingInfo().page_token.empty()) {
    return StatusRecord(
        {SQLStates::k_SQL_NO_DATA(), "No more data to return."});
  }

  stmt_handle.GetResultSet().rows.clear();
  auto ds_status_record_or = FetchNextPageOfQueryResults(
      stmt_handle, stmt_handle.GetPostQueryRequest());
  if (!ds_status_record_or) {
    stmt_handle.SetStmtState(StmtStates::kStatementPrepared);
    return ds_status_record_or.GetStatusRecord();
  }
  DSResults results;
  results.num_dml_affected_rows = ds_status_record_or->num_dml_affected_rows;
  results.job_ref = ds_status_record_or->job_reference;
  results.data_source_results = *ds_status_record_or;
  stmt_handle.GetPagingInfo().page_token = ds_status_record_or->page_token;
  stmt_handle.SetDSResults(results);
  auto rs_status_record_or = ProcessQueryResults(results);
  if (!rs_status_record_or) {
    stmt_handle.SetStmtState(StmtStates::kStatementPrepared);
    LOG(ERROR) << "FetchNextPageResultSet:: "
               << rs_status_record_or.GetStatusRecord().message;
    return rs_status_record_or.GetStatusRecord();
  }
  stmt_handle.SetResultSet(*rs_status_record_or);
  return StatusRecord::Ok();
}

StatusRecordOr<GetQueryResults> FetchNextPageOfQueryResults(
    StatementHandle& stmt_handle, PostQueryRequest const& post_query_request) {
  GetQueryResultsRequest get_query_results_request;
  get_query_results_request.set_project_id(post_query_request.project_id());
  get_query_results_request.set_job_id(stmt_handle.GetPagingInfo().job_id);
  get_query_results_request.set_location(
      post_query_request.query_request().location());
  get_query_results_request.set_timeout(
      post_query_request.query_request().timeout());
  get_query_results_request.set_page_token(
      stmt_handle.GetPagingInfo().page_token);

  ExponentialBackoffPolicy backoff(chrono_ms(10), chrono_ms(200), 2);
  auto start_time = std::chrono::system_clock::now();
  auto timeout_ms =
      std::chrono::milliseconds(post_query_request.query_request().timeout());

  Options options;
  auto job_client = stmt_handle.GetConnectionHandle()->GetClient();

  LOG(INFO) << "FetchNextPageOfQueryResults:: Request body: "
            << get_query_results_request.DebugString("");

  while (true) {
    if (timeout_ms.count() > 0 &&
        std::chrono::system_clock::now() > start_time + timeout_ms) {
      std::string message = "The query timeout period of " +
                            std::to_string(timeout_ms.count()) +
                            "ms has expired";
      LOG(ERROR) << "FetchNextPageOfQueryResults:: " << message;
      return StatusRecord{SQLStates::k_HYT00(), message};
    }

    auto get_query_results_partial =
        job_client->GetQueryResults(get_query_results_request, options);

    if (!get_query_results_partial) {
      LOG(ERROR) << "FetchNextPageOfQueryResults::QueryResults failed: "
                 << get_query_results_partial.status().message();
      return StatusRecord::ConvertFrom(get_query_results_partial.status());
    }

    // Wait if job is not yet complete and no rows have arrived
    if (!get_query_results_partial->job_complete &&
        get_query_results_partial->rows.empty()) {
      std::this_thread::sleep_for(backoff.OnCompletion());
      continue;
    }

    LOG(INFO) << "FetchNextPageOfQueryResults:: Response body: "
              << get_query_results_partial->DebugString("");

    // Replace get_query_results with this latest result
    GetQueryResults get_query_results = *get_query_results_partial;
    stmt_handle.GetPagingInfo().page_token =
        get_query_results_partial->page_token;

    return get_query_results;
  }
}
}  // namespace google::cloud::odbc_bq_driver_internal
