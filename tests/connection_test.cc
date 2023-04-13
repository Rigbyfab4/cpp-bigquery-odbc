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

#include "odbc/connection.h"
#include "odbc/functions.h"
#include "odbc/statement.h"
#include <gtest/gtest.h>

namespace google {
namespace cloud {
namespace bigquery_odbc {

TEST(ConnectionTest, SQLDriverConnect) {
  ConnectionHandle conn;
  EXPECT_EQ(Connect((char *)kDefaultConnectionString, &conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(&conn), SQL_SUCCESS);
}

TEST(DriverInfoTest, SQLGetInfo) {
  ConnectionHandle conn;
  EXPECT_EQ(Connect((char *)kDefaultConnectionString, &conn), SQL_SUCCESS);
  EXPECT_EQ(GetDriverInfo(&conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(&conn), SQL_SUCCESS);
}


TEST(DriverAttributesTest, SQLGetEnvAttr) {
  ConnectionHandle conn;
  EXPECT_EQ(Connect((char *)kDefaultConnectionString, &conn), SQL_SUCCESS);
  EXPECT_EQ(GetEnvInfo(&conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(&conn), SQL_SUCCESS);
}

TEST(DescriptorFieldsTest, SQLGetDescRec) {
  ConnectionHandle conn;
  EXPECT_EQ(Connect((char *)kDefaultConnectionString, &conn), SQL_SUCCESS);
  EXPECT_EQ(GetDescRec(&conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(&conn), SQL_SUCCESS);
}

TEST(FunctionsTest, SQLGetFunctions) {
  ConnectionHandle conn;
  EXPECT_EQ(Connect((char *)kDefaultConnectionString, &conn), SQL_SUCCESS);
  EXPECT_EQ(PrintSupportedFunctions(&conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(&conn), SQL_SUCCESS);
}

TEST(StatementTest, SQLExecDirect) {
  ConnectionHandle conn;
  EXPECT_EQ(Connect((char *)kDefaultConnectionString, &conn), SQL_SUCCESS);
  EXPECT_EQ(InsertDirectStatement(&conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(&conn), SQL_SUCCESS);
}

TEST(StatementTest, SQLExecute) {
  ConnectionHandle conn;
  EXPECT_EQ(Connect((char *)kDefaultConnectionString, &conn), SQL_SUCCESS);
  EXPECT_EQ(InsertStatement(&conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(&conn), SQL_SUCCESS);
}

}  // namespace bigquery_odbc
}  // namespace cloud
}  // namespace google
