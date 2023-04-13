
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

// Tests insertion with params using SQLPrepare, SQLBindParameter and SQLExecute
SQLRETURN InsertStatement(ConnectionHandle *conn) {
  SQLRETURN status;
  const char * create_table_stmt = "create or replace table ODBCTESTDATASET.ODBC_INSERT_PARAMS_TEST (StringField STRING, IntegerField INTEGER)";
  const char * drop_table_stmt = "DROP TABLE ODBCTESTDATASET.ODBC_INSERT_PARAMS_TEST";

  const char * str_field = "Test String 1";
  int int_field = 42;
  const char * insert_stmt = "INSERT INTO ODBCTESTDATASET.ODBC_INSERT_PARAMS_TEST VALUES (?, ?)";

  //Create Table
  status = SQLExecDirect(conn->hstmt, (SQLCHAR *)create_table_stmt, SQL_NTS);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLExecDirect", conn);
    return status;
  }

  //Prepare statement with insert query string
  status = SQLPrepare(conn->hstmt, (SQLCHAR *)insert_stmt, SQL_NTS);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLPrepare", conn);
    return status;
  }

  //Add param 1 to insert query string
  SQLLEN len_string_field = strlen(str_field);
  status = SQLBindParameter(conn->hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
                            SQL_CHAR, len_string_field, 0, (SQLCHAR * )str_field,
                            len_string_field, NULL);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLBindParameter", conn);
    return status;
  }

  //Add param 2 to insert query string
  status = SQLBindParameter(conn->hstmt, 2, SQL_PARAM_INPUT, SQL_C_SSHORT,
                            SQL_INTEGER, 0, 0, &int_field,
                            0, NULL);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLBindParameter", conn);
    return status;
  }

  //Execute insertion
  status = SQLExecute (conn->hstmt);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLExecute", conn);
    return status;
  }

  //Drop Table
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
