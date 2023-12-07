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

#include "google/cloud/odbc/bq_driver/internal/trace_utils.h"
#include <gtest/gtest.h>

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace google {
namespace cloud {
namespace odbc_bq_driver {

TEST(TraceLogging, BasicTypes)
{
  auto fmt1 = FormatSqlSmallInt(1);
  auto fmt2 = FormatSqlUSmallInt(2);
  auto fmt3 = FormatSqlInteger(3);
  auto fmt4 = FormatSqlUInteger(4);

  EXPECT_EQ("TestBasicTypes\t\tSQLSMALLINT, 1\n\t\tSQLUSMALLINT, 2\n\t\tSQLINTEGER, 3\n\t\tSQLUINTEGER, 4\n",
            CollectAndPrintArgs("TestBasicTypes",
                                4, fmt1.c_str(), fmt2.c_str(), fmt3.c_str(), fmt4.c_str()));
}

TEST(TraceLogging, Handle)
{
  std::string expected;
  expected.append("TestHandle\t\tSQL_NULL_HANDLE, 0x0\n\t\t")
      .append("SQL_HANDLE_DBC, handle type=2\n\t\t")
      .append("SQL_HANDLE_DESC, handle type=4\n\t\t")
      .append("SQL_HANDLE_ENV, handle type=1\n\t\t")
      .append("SQL_HANDLE_STMT, handle type=3\n\t\t")
      .append("Unknown Handle Type, handle type=1234\n");

  SQLHANDLE handle = nullptr;

  auto fmt1 = FormatSqlHandle(handle);
  auto fmt2 = FormatSqlHandleType(SQL_HANDLE_DBC);
  auto fmt3 = FormatSqlHandleType(SQL_HANDLE_DESC);
  auto fmt4 = FormatSqlHandleType(SQL_HANDLE_ENV);
  auto fmt5 = FormatSqlHandleType(SQL_HANDLE_STMT);
  auto fmt6 = FormatSqlHandleType(1234);

  EXPECT_EQ(expected, CollectAndPrintArgs("TestHandle",
                                          6, fmt1.c_str(), fmt2.c_str(), fmt3.c_str(),
                                          fmt4.c_str(), fmt5.c_str(), fmt6.c_str()));
}

TEST(TraceLogging, Pointers)
{
  SQLPOINTER p = nullptr;
  SQLPOINTER *pp = nullptr;
  SQLHANDLE *hp = nullptr;
  SQLSMALLINT i1 = 1;
  SQLUSMALLINT i2 = 2;
  SQLINTEGER i3 = 3;
  SQLUINTEGER i4 = 4;
  SQLCHAR *str = (SQLCHAR *)"Hello World";

  auto fmt1 = FormatSqlPointer(p);
  auto fmt2 = FormatSqlSmallInt(&i1);
  auto fmt3 = FormatSqlUSmallInt(&i2);
  auto fmt4 = FormatSqlInteger(&i3);
  auto fmt5 = FormatSqlUInteger(&i4);
  auto fmt6 = FormatSqlChar(str);
  auto fmt7 = FormatSqlPointer(pp);
  auto fmt8 = FormatSqlHandle(hp);

  std::string expected;
  expected.append("TestPointers\t\tSQLPOINTER, 0x0\n\t\t")
      .append("SQLSMALLINT *, 1\n\t\tSQLUSMALLINT *, 2\n\t\t")
      .append("SQLINTEGER *, 3\n\t\tSQLUINTEGER *, 4\n\t\t")
      .append("SQLCHAR *, Hello World\n\t\tSQLPOINTER *, 0x0\n\t\tSQLHANDLE *, 0x0\n");

  EXPECT_EQ(expected, CollectAndPrintArgs("TestPointers",
                                          8, fmt1.c_str(), fmt2.c_str(), fmt3.c_str(), fmt4.c_str(),
                                          fmt5.c_str(), fmt6.c_str(), fmt7.c_str(), fmt8.c_str()));
}

TEST(TraceLogging, Length)
{
  auto fmt1 = FormatSqlLen(10);
  auto fmt2 = FormatSqlULen(11);
  auto fmt3 = FormatSqlSetPosiRow(50);

  EXPECT_EQ("TestLength\t\tSQLLEN, 10\n\t\tSQLULEN, 11\n\t\tSQLSETPOSIROW, 50\n",
            CollectAndPrintArgs("TestLength", 3, fmt1.c_str(), fmt2.c_str(), fmt3.c_str()));
}

TEST(TraceLogging, ReturnCodes)
{
  auto fmt1 = FormatSqlReturnCode(1);
  auto fmt2 = FormatSqlReturn(2);

  EXPECT_EQ("TestRetCodes\t\tRETCODE, 1\n\t\tSQLRETURN, 2\n",
            CollectAndPrintArgs("TestRetCodes", 2, fmt1.c_str(), fmt2.c_str()));
}

TEST(TraceLogging, AdditionalSqlTypes)
{
  SQLDATE *d = (SQLDATE *)"1901-01-01";
  SQLTIME *t = (SQLTIME *)"10:30:00";
  SQLTIMESTAMP *tp = (SQLTIMESTAMP *)"1901-01-01 10:30:00";
  SQLVARCHAR *str = (SQLVARCHAR *)"Hello";

  SQLDECIMAL dec = 10;
  SQLNUMERIC n = 11;
  SQLDOUBLE dbl = 1.1;
  SQLFLOAT fl = 2.2;
  SQLREAL r = 3.3f;

  auto fmt1 = FormatSqlDate(d);
  auto fmt2 = FormatSqlDecimal(dec);
  auto fmt3 = FormatSqlNumeric(n);
  auto fmt4 = FormatSqlDouble(dbl);
  auto fmt5 = FormatSqlFloat(fl);
  auto fmt6 = FormatSqlReal(r);
  auto fmt7 = FormatSqlTime(t);
  auto fmt8 = FormatSqlTimestamp(tp);
  auto fmt9 = FormatSqlVarchar(str);
  auto fmt10 = FormatSqlDecimal(&dec);
  auto fmt11 = FormatSqlNumeric(&n);
  auto fmt12 = FormatSqlDouble(&dbl);
  auto fmt13 = FormatSqlFloat(&fl);
  auto fmt14 = FormatSqlReal(&r);

  std::string expected;
  expected.append("TestAdditionalSqlTypes\t\tSQLDATE *, 1901-01-01\n\t\tSQLDECIMAL, 10\n\t\t")
      .append("SQLNUMERIC, 11\n\t\tSQLDOUBLE, 1.1000\n\t\tSQLFLOAT, 2.2000\n\t\t")
      .append("SQLREAL, 3.30\n\t\tSQLTIME *, 10:30:00\n\t\t")
      .append("SQLTIMESTAMP *, 1901-01-01 10:30:00\n\t\tSQLVARCHAR *, Hello\n\t\t")
      .append("SQLDECIMAL *, 10\n\t\tSQLNUMERIC *, 11\n\t\tSQLDOUBLE *, 1.1000\n\t\t")
      .append("SQLFLOAT *, 2.2000\n\t\tSQLREAL *, 3.30\n");

  EXPECT_EQ(expected,
            CollectAndPrintArgs("TestAdditionalSqlTypes", 14,
                                fmt1.c_str(), fmt2.c_str(), fmt3.c_str(), fmt4.c_str(),
                                fmt5.c_str(), fmt6.c_str(), fmt7.c_str(), fmt8.c_str(),
                                fmt9.c_str(), fmt10.c_str(), fmt11.c_str(), fmt12.c_str(),
                                fmt13.c_str(), fmt14.c_str()));
}

}  // namespace odbc_bq_driver
}  // namespace cloud
}  // namespace google
// NOLINTEND(modernize-concat-nested-namespaces)
