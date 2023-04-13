
// Copyright 2023 Google LLC
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

#include "odbc/statement.h"

namespace google {
namespace cloud {
namespace bigquery_odbc {

// Tests direct execution of statements using SQLExecDirect
SQLRETURN InsertDirectStatement(ConnectionHandle *conn) {
  SQLRETURN status;
  const char * create_table_stmt = "CREATE OR REPLACE TABLE ODBCTESTDATASET.ODBCTRANSACTIONTEST (string_field STRING)";
  const char * drop_table_stmt = "DROP TABLE ODBCTESTDATASET.ODBCTRANSACTIONTEST";

  string string_field = "Test String 1";
  string insert_stmt = (string)"INSERT INTO ODBCTESTDATASET.ODBCTRANSACTIONTEST VALUES ('" + string_field + (string)"')";

  status = SQLExecDirect(conn->hstmt, (SQLCHAR *)create_table_stmt, SQL_NTS);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLExecDirect", conn);
    return status;
  }

  status = SQLExecDirect(conn->hstmt, (SQLCHAR *)Cstr(insert_stmt), SQL_NTS);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLExecDirect", conn);
    return status;
  }

  status = SQLExecDirect(conn->hstmt, (SQLCHAR *)drop_table_stmt, SQL_NTS);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLExecDirect", conn);
    return status;
  }
  return status;
}

}  // namespace bigquery_odbc
}  // namespace cloud
}  // namespace google
