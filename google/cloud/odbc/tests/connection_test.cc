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

#include "testing/connection.h"
#include "testing/functions.h"
#include "testing/statement.h"

namespace google {
namespace cloud {
namespace bigquery_odbc {

StdRows kSampleData{
  { "Test String 1", 1, 1.1 },
  { .int_field = 237, .float_field = 2.22 },
  { "Test String 3", NULL, 3.333 },
  { "Test String 4", 49 }
};

TEST(ConnectionTest, SQLDriverConnect) {
  shared_ptr<ConnectionHandle> conn(new ConnectionHandle());
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}


TEST(DriverInfoTest, SQLGetInfo) {
  shared_ptr<ConnectionHandle> conn(new ConnectionHandle());
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  EXPECT_EQ(GetDriverInfo(conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(DriverAttributesTest, SQLGetEnvAttr) {
  shared_ptr<ConnectionHandle> conn(new ConnectionHandle());
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  EXPECT_EQ(GetEnvInfo(conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(DescriptorFieldsTest, SQLGetDescRec) {
  shared_ptr<ConnectionHandle> conn(new ConnectionHandle());
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  EXPECT_EQ(GetDescRec(conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(FunctionsTest, SQLGetFunctions) {
  shared_ptr<ConnectionHandle> conn(new ConnectionHandle());
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  EXPECT_EQ(GetAllFunctions(conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(StatementTest, SQLExecDirect) {
  shared_ptr<ConnectionHandle> conn(new ConnectionHandle());
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  EXPECT_EQ(InsertDirectStatement(conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(StatementTest, SQLExecute) {
  shared_ptr<ConnectionHandle> conn(new ConnectionHandle());
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  EXPECT_EQ(InsertStatement(conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(StatementTest, SQLDescribeCol) {
  const string table_name = kDatasetName + ".ODBC_COLUMN_DESCRIPTION_TEST";

  Schema schema {
    { "StringField", SQL_VARCHAR},
    { "IntegerField", SQL_BIGINT},
    { "FloatField", SQL_DOUBLE}
  };

  //Create Table
  shared_ptr<ConnectionHandle> conn(new ConnectionHandle());
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  CreateTable(conn, table_name, "(StringField STRING, IntegerField INTEGER, FloatField FLOAT64)");
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  //Insert data to read
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  InsertIntoTable(conn, table_name, kSampleData);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  CheckColumnData(conn, table_name, schema);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  DropTable(conn, table_name);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(StatementTest, SQLFetch) {
  const string table_name = kDatasetName + ".ODBC_CHECK_RESULTS_TEST";

  //TODO(#14): Add integer and floating point fields too
  //Schema returned by the query
  Schema schema {
    { "StringField", SQL_VARCHAR}
  };

  //Create Table
  shared_ptr<ConnectionHandle> conn(new ConnectionHandle());
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  CreateTable(conn, table_name, "(StringField STRING, IntegerField INTEGER, FloatField FLOAT64)");
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  //Insert data to read
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  InsertIntoTable(conn, table_name, kSampleData);
  SQLLEN rows_count = 0;
  SQLRETURN status = SQLRowCount(conn->hstmt, &rows_count);
  CheckError(status, "SQLRowCount", conn);
  EXPECT_EQ(rows_count, kSampleData.size());
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);


  //Execute a read query and check whether the results returned are as expected
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  //TODO(#14): Add integer and floating point fields too
  string query = "SELECT StringField FROM " + table_name;
  shared_ptr<Results> results_ptr = FetchResults(conn, query, kSampleData);
  Results results = *results_ptr;

  vector<string> ret_col_values = results["StringField"];
  //We have to sort inserted and returned values because we haven't specified the ordering
  sort(ret_col_values.begin(), ret_col_values.end(), str_comparison);

  vector<string> input_col_values;
  for(auto data: kSampleData) {
    input_col_values.push_back(data.str_field);
  }
  sort(input_col_values.begin(), input_col_values.end(), str_comparison);

  //Check if the sorted inserted and returned vectors have same values
  EXPECT_EQ(ret_col_values.size(), input_col_values.size());
  for(int i = 0; i < ret_col_values.size(); i++) {
    EXPECT_EQ(ret_col_values[i], input_col_values[i]);
  }

  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  //Delete table
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  DropTable(conn, table_name);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

}  // namespace bigquery_odbc
}  // namespace cloud
}  // namespace google
