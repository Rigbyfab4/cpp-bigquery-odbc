// Copyright 2024 Google LLC
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

#ifndef CPP_BIGQUERY_ODBC_GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_ODBC_SQL_FETCH_H
#define CPP_BIGQUERY_ODBC_GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_ODBC_SQL_FETCH_H

#include "google/cloud/odbc/bq_driver/internal/odbc_desc_handle.h"
#include "google/cloud/odbc/bq_driver/internal/odbc_internal_commons.h"
#include "google/cloud/odbc/internal/diagnostic_records.h"

namespace google::cloud::odbc_bq_driver_internal {

// #if (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)
// google::cloud::odbc_internal::StatusRecord WriteArrowBatchRowset(StatementHandle& stmt_handle, 
//                                    int const rowset_size,
//                                    DescriptorHandle& ard, 
//                                    DescriptorHandle& ird);
// #endif  // (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)

/*

#if (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)

google::cloud::odbc_internal::StatusRecord WriteArrowBatchRowset(StatementHandle& stmt_handle, 
                                   int const rowset_size,
                                   DescriptorHandle& ard, 
                                   DescriptorHandle& ird) {
  // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP0:: ";
  auto columns = stmt_handle.GetArrowColumns();
  int64_t total_rows = stmt_handle.GetArrowBatchNumRows();
  int64_t batch_cursor = stmt_handle.GetArrowBatchCursor();
  // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP1:: ";
  
  if (columns.empty() || batch_cursor >= total_rows) {
      return google::cloud::odbc_internal::StatusRecord({SQLStates::k_SQL_NO_DATA(), "No data in current batch."});
  }

  // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP2:: ";

  int64_t rows_to_write = std::min(static_cast<int64_t>(rowset_size), total_rows - batch_cursor);

  // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP3:: ";
  
  RowSchema const& schema = stmt_handle.GetResultSet().row_schema;
  
  SQLLEN* bind_offset_ptr = ard.GetHeaderRecord().bind_offset_ptr;
  SQLLEN bind_offset = (bind_offset_ptr) ? *bind_offset_ptr : 0;
  SQLUSMALLINT* row_status_ptr = ird.GetHeaderRecord().array_status_ptr;

  for (int i = 0; i < rows_to_write; ++i) {
    // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP4:: ";
    int64_t arrow_row_idx = batch_cursor + i;
    
    // Calculate Application Buffer Row Offset
    // We do this calculation inside the loop but outside the column loop 
    // if stride is uniform, but ARD allows per-column binding types.
    // So we calculate per column.
    
    for (ColumnSchema const& col_schema : schema) {
      int col_index = col_schema.col_index;
      
      if (!ard.HasDescriptorRecord(col_index + 1)) {
        continue;
      }
      
      DescriptorRecord& col_desc = ard.GetDescriptorRecord(col_index + 1);
      auto& column = columns[col_index];

      // 1. Setup Target Pointers
      SQLINTEGER bind_type = ard.GetHeaderRecord().bind_type;
      SQLLEN elem_size, elem_size_ind;
      
      if (bind_type == SQL_BIND_BY_COLUMN) {
        elem_size = GetElemSize(col_desc);
        elem_size_ind = sizeof(SQLLEN);
      } else {
        elem_size = bind_type;
        elem_size_ind = bind_type;
      }

      SQLLEN row_offset = i * elem_size;
      SQLLEN row_offset_ind = i * elem_size_ind;

      SQLPOINTER app_buffer = static_cast<char*>(col_desc.data_ptr) + bind_offset + row_offset;
      SQLLEN* indicator_ptr = nullptr;
      SQLLEN* octet_length_ptr = nullptr;

      if (col_desc.indicator_ptr) {
        indicator_ptr = reinterpret_cast<SQLLEN*>(
            reinterpret_cast<char*>(col_desc.indicator_ptr) + bind_offset + row_offset_ind);
      }
      if (col_desc.octet_length_ptr) {
        octet_length_ptr = reinterpret_cast<SQLLEN*>(
            reinterpret_cast<char*>(col_desc.octet_length_ptr) + bind_offset + row_offset_ind);
      }

      // 2. Handle Nulls
      if (column->IsNull(arrow_row_idx)) {
        if (indicator_ptr == nullptr) {
          return {SQLStates::k_22002(), "Indicator variable required but not supplied for NULL data"};
        }
        *indicator_ptr = SQL_NULL_DATA;
        continue;
      }

      // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP5:: " << col_index;
      
      // Prepare DataBuffer wrapper for the utility functions
      DataBuffer data = {col_desc.concise_type, app_buffer, col_desc.octet_length, octet_length_ptr};
      
      // 3. Extract Arrow Value & Write Directly (Merged Switch)
      google::cloud::odbc_internal::StatusRecord status;
      auto type_id = column->type_id();

      // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP6:: ";

      switch (type_id) {
        case arrow::Type::INT64: {
          auto int_arr = std::static_pointer_cast<arrow::Int64Array>(column);
          // ConvertFromArithmeticDSValue expects a string representation
          DSValue val = std::to_string(int_arr->Value(arrow_row_idx));
          status = ConvertFromArithmeticDSValue<SQLBIGINT>(val, data);
          break;
        }
        case arrow::Type::DOUBLE: {
          auto dbl_arr = std::static_pointer_cast<arrow::DoubleArray>(column);
          DSValue val = std::to_string(dbl_arr->Value(arrow_row_idx));
          status = ConvertFromArithmeticDSValue<SQLDOUBLE>(val, data);
          break;
        }
        case arrow::Type::STRING: {
          // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP7:: ";
          auto str_arr = std::static_pointer_cast<arrow::StringArray>(column);
          auto view = str_arr->GetView(arrow_row_idx);
          // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP8:: ";
          DSValue val(view.data(), view.size());
          // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP9:: ";
          status = ConvertFromStringDSValue(val, data);
          // LOG(INFO) << "SACHIN:: WriteArrowBatchRowset CP10:: ";
          break;
        }
        case arrow::Type::BOOL: {
          auto bool_arr = std::static_pointer_cast<arrow::BooleanArray>(column);
          // ConvertFromBooleanDSValue typically expects "true"/"false" or "1"/"0" string
          DSValue val = bool_arr->Value(arrow_row_idx) ? "true" : "false";
          status = ConvertFromBooleanDSValue(val, data);
          break;
        }
        case arrow::Type::BINARY: {
          auto bin_arr = std::static_pointer_cast<arrow::BinaryArray>(column);
          auto view = bin_arr->GetView(arrow_row_idx);
          DSValue val(view.data(), view.size());
          status = ConvertFromBytesDSValue(val, data);
          break;
        }
        // For complex types or those needing formatting (Timestamp/Date/Time),
        // we fallback to Arrow's ToString() which generates the format
        // expected by our ConvertFrom* utilities (e.g. "YYYY-MM-DD...").
        default: {
          auto scalar_res = column->GetScalar(arrow_row_idx);
          if (!scalar_res.ok()) {
            return google::cloud::odbc_internal::StatusRecord{SQLStates::k_HY000(), "Internal Error: Arrow GetScalar failed"};
          }
          std::string str_data = scalar_res.ValueOrDie()->ToString();
          
          switch (type_id) {
            case arrow::Type::TIMESTAMP:
              status = ConvertFromTimestampDSValue(str_data, data);
              break;
            case arrow::Type::DATE32:
              status = ConvertFromDateDSValue(str_data, data);
              break;
            case arrow::Type::TIME64:
              status = ConvertFromTimeDSValue(str_data, data);
              break;
            case arrow::Type::DECIMAL128:
            case arrow::Type::DECIMAL256:
              status = ConvertFromNumericDSValue(str_data, data);
              break;
            case arrow::Type::LIST:
              // Remove arrow debug formatting if present (e.g. "list<item: string>[...]")
              if (str_data.rfind("list<", 0) == 0) {
                auto pos = str_data.find('[');
                if (pos != std::string::npos) str_data = str_data.substr(pos);
              }
              status = ConvertFromArrayDSValue(str_data, data);
              break;
            case arrow::Type::STRUCT:
               status = ConvertFromStructDSValue(str_data, data);
               break;
            default:
              // Fallback for unknown types -> Treat as String
              status = ConvertFromStringDSValue(str_data, data);
              break;
          }
          break;
        }
      }

      if (!status.ok()) {
        LOG(ERROR) << "WriteArrowBatchRowset::ConversionError:: " << status.message;
        return status;
      }

      // If success and indicator exists, set length (handled inside ConvertFrom... usually, 
      // but we ensure the indicator is set to non-null if the util didn't)
      if (indicator_ptr && *indicator_ptr == SQL_NULL_DATA) {
         // This case should ideally not be reached if ConvertFrom* works correctly,
         // as they calculate octet length.
         // We leave it to the utility functions to populate *indicator_ptr (octet_length).
      }
    }

    if (row_status_ptr) {
      row_status_ptr[i] = SQL_ROW_SUCCESS;
    }
  }

  if (row_status_ptr) {
    for (int i = rows_to_write; i < rowset_size; i++) {
      row_status_ptr[i] = SQL_ROW_NOROW;
    }
  }

  if (auto* processed = ird.GetHeaderRecord().rows_processed_ptr) {
    *processed = rows_to_write;
  }

  stmt_handle.SetArrowBatchCursor(batch_cursor + rows_to_write);

  return google::cloud::odbc_internal::StatusRecord::Ok();
}

#endif  // (!defined(_WIN32) || defined(_WIN64)) && !defined(NO_ARROW)

*/

// Writes rowset_size number of rows to the columns bound by the application
google::cloud::odbc_internal::StatusRecord WriteRowset(
    ResultSet const& result_set, int rowset_size, DescriptorHandle& ard,
    DescriptorHandle& ird);

// Fetches the next batch of ResultSet rows
google::cloud::odbc_internal::StatusRecord FetchNextResultSet1(
    StatementHandle& stmt_handle);

}  // namespace google::cloud::odbc_bq_driver_internal

#endif  // CPP_BIGQUERY_ODBC_GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_ODBC_SQL_FETCH_H
