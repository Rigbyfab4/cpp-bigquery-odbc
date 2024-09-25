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

#include "google/cloud/odbc/bq_driver/internal/data_translation.h"

namespace google::cloud::odbc_bq_driver_internal {

using google::cloud::odbc_internal::SQLStates;
using google::cloud::odbc_internal::StatusRecord;
using google::cloud::odbc_internal::StatusRecordOr;

StatusRecord ConvertFromJsonDSValue(DSValue const& src_dsval,
                                    DataBuffer& dest_data) {
  std::string src_str;
  DSValueToString(src_dsval, src_str);
  SQLSMALLINT dest_type = dest_data.type;
  SQLPOINTER dest_buf = dest_data.buf;
  SQLLEN buffer_length = dest_data.buflen;
  SQLLEN* res_len = dest_data.result_len;

  switch (dest_type) {
    case SQL_C_CHAR: {
      StatusRecord status_record =
          StringValueToOutputBufferResponse(src_str.c_str(), dest_data);
      return status_record;
    }
    case SQL_C_WCHAR: {
      StatusRecordOr<std::wstring> wide_string = Utf8ToUtf16(src_str);
      if (!wide_string.Ok()) {
        StatusRecord status_record =
            StatusRecord{SQLStates::k_HY000(), "Conversion Failed"};
        break;
      }
      return WStrToOutputBufferResponse(
          wide_string.GetValue(), dest_buf, buffer_length, src_str.length(),
          src_str.length(), reinterpret_cast<SQLLEN*>(dest_data.result_len));
      break;
    }
    // TODO(b\367841053): SQL_C_BINARY to be done later
    default: {
      return StatusRecord{SQLStates::k_HY000(), "Conversion is unsupported"};
    }
  }
  return StatusRecord::Ok();
}

}  // namespace google::cloud::odbc_bq_driver_internal
