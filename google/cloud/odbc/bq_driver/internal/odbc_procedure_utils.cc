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

#include "google/cloud/odbc/bq_driver/internal/odbc_procedure_utils.h"
#include "google/cloud/odbc/internal/status_record_or.h"

namespace google::cloud::odbc_bq_driver_internal {
using ::google::cloud::odbc_internal::SQLStates;
using ::google::cloud::odbc_internal::StatusRecord;

/**
 * Validates the parameters for retrieving procedure column metadata.
 *
 * @param metadata_id - Indicates whether to use standard metadata retrieval.
 * @return StatusRecord indicating validation SUCCESS or FAILURE.
 */

StatusRecord ValidateProcedureColumnParameters(const SQLCHAR* catalog_name,
                                               SQLSMALLINT catalog_name_len,
                                               const SQLCHAR* schema_name,
                                               SQLSMALLINT schema_name_len,
                                               const SQLCHAR* procedure_name,
                                               SQLSMALLINT procedure_name_len,
                                               SQLULEN metadata_id) {
  if (catalog_name_len < 0 && catalog_name_len != SQL_NTS) {
    return StatusRecord{SQLStates::k_HY090(),
                        "Invalid buffer length - catalog length is invalid"};
  }
  if (schema_name_len < 0 && schema_name_len != SQL_NTS) {
    return StatusRecord{SQLStates::k_HY090(),
                        "Invalid buffer length - schema length is invalid"};
  }
  if (procedure_name_len < 0 && procedure_name_len != SQL_NTS) {
    return StatusRecord{
        SQLStates::k_HY090(),
        "Invalid buffer length - procedure name length is invalid"};
  }

  if (metadata_id == SQL_TRUE) {
    if (!catalog_name) {
      return StatusRecord{SQLStates::k_HY009(),
                          "Invalid use of NULL pointer for catalog name"};
    }
    if (!schema_name) {
      return StatusRecord{SQLStates::k_HY009(),
                          "Invalid use of NULL pointer for schema name"};
    }
    if (!procedure_name) {
      return StatusRecord{SQLStates::k_HY009(),
                          "Invalid use of NULL pointer for procedure name"};
    }
  }

  if (IsSearchPatternArgument(reinterpret_cast<char const*>(catalog_name))) {
    return StatusRecord{SQLStates::k_HY090(),
                        "Catalog name cannot be a search pattern"};
  }

  return StatusRecord::Ok();
}

}  // namespace google::cloud::odbc_bq_driver_internal
