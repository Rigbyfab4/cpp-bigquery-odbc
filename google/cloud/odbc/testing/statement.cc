
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

#include "testing/statement.h"

namespace google {
namespace cloud {
namespace bigquery_odbc {

const string kDatasetName = "ODBCTESTDATASET";

// Tests direct execution of statements using SQLExecDirect
SQLRETURN InsertDirectStatement(shared_ptr<ConnectionHandle> conn) {
  SQLRETURN status;
  const string table_name = kDatasetName + ".ODBC_INSERT_DIRECT_TEST";

  const string string_field = "Test String 1";
  char insert_stmt[kBufferLength];
  sprintf(insert_stmt, "INSERT INTO %s VALUES ('%s')", table_name.c_str(), string_field.c_str());

  //Create Table
  CreateTable(conn, table_name, "(string_field STRING)");

  //Execute insertion
  ExecuteStatement(conn, insert_stmt);

  //Drop Table
  DropTable(conn, table_name);

  return status;
}

// Tests insertion with params using SQLPrepare, SQLBindParameter and SQLExecute
SQLRETURN InsertStatement(shared_ptr<ConnectionHandle> conn) {
  SQLRETURN status;
  const string table_name = kDatasetName + ".ODBC_INSERT_PARAMS_TEST";
  char insert_stmt[kBufferLength];
  StrToChar(insert_stmt, "INSERT INTO " +(string)table_name + " VALUES (?, ?)");

  //Create Table
  CreateTable(conn, table_name, "(StringField STRING, IntegerField INTEGER)");

  //Prepare statement with insert query string
  status = SQLPrepare(conn->hstmt, (SQLCHAR *)insert_stmt, SQL_NTS);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLPrepare", conn);
    return status;
  }

  //Testing SQLNumParams API
  SQLSMALLINT num_params;
  status = SQLNumParams(conn->hstmt, &num_params);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLNumParams", conn);
    return status;
  }
  EXPECT_EQ(num_params, 2);

  //Add param 1(string) to insert query string
  constexpr char * str_field = "Test String 1";
  SQLLEN len_string_field = strlen(str_field);
  status = SQLBindParameter(conn->hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
                            SQL_CHAR, len_string_field, 0, (SQLCHAR * )str_field,
                            len_string_field, NULL);
  if (!SQL_SUCCEEDED(status)) {
    GetErrorDetails("SQLBindParameter", conn);
    return status;
  }

  //Add param 2 to insert query string
  int int_field = 42;
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
  DropTable(conn, table_name);

  return status;
}

}  // namespace bigquery_odbc
}  // namespace cloud
}  // namespace google
