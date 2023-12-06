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

namespace google {
namespace cloud {
namespace odbc_bq_driver {

// For unit test purposes we just log to stderr.
FILE* err_file = stderr;

TEST(TraceLogging, BasicTypes)
{
  auto fmt1 = FormatSqlSmallInt(1);
  auto fmt2 = FormatSqlUSmallInt(2);
  auto fmt3 = FormatSqlInteger(3);
  auto fmt4 = FormatSqlUInteger(4);

  EXPECT_EQ("\t\tSQLSMALLINT, 1\n", fmt1);
  EXPECT_EQ("\t\tSQLUSMALLINT, 2\n", fmt2);
  EXPECT_EQ("\t\tSQLINTEGER, 3\n", fmt3);
  EXPECT_EQ("\t\tSQLUINTEGER, 4\n", fmt4);

  EXPECT_TRUE(TracePrintInternalStdOut(fmt1, fmt2, fmt3, fmt4) > 0);
  EXPECT_TRUE(TracePrintInternalFile(err_file, fmt1, fmt2, fmt3, fmt4) > 0);
}

TEST(TraceLogging, Handle)
{
  SQLHANDLE handle = nullptr;

  auto fmt_handle = FormatSqlHandle(handle);
  auto fmt_type1 = FormatSqlHandleType(SQL_HANDLE_DBC);
  auto fmt_type2 = FormatSqlHandleType(SQL_HANDLE_DESC);
  auto fmt_type3 = FormatSqlHandleType(SQL_HANDLE_ENV);
  auto fmt_type4 = FormatSqlHandleType(SQL_HANDLE_STMT);
  auto fmt_type5 = FormatSqlHandleType(1234);

  EXPECT_EQ("\t\tSQL_NULL_HANDLE, 0x0\n", fmt_handle);
  EXPECT_EQ("\t\tSQL_HANDLE_DBC, handle type=2\n", fmt_type1);
  EXPECT_EQ("\t\tSQL_HANDLE_DESC, handle type=4\n", fmt_type2);
  EXPECT_EQ("\t\tSQL_HANDLE_ENV, handle type=1\n", fmt_type3);
  EXPECT_EQ("\t\tSQL_HANDLE_STMT, handle type=3\n", fmt_type4);
  EXPECT_EQ("\t\tUnknown Handle Type, handle type=1234\n", fmt_type5);

  EXPECT_TRUE(TracePrintInternalStdOut(fmt_type1, fmt_handle) > 0);
  EXPECT_TRUE(TracePrintInternalStdOut(fmt_type2, fmt_handle) > 0);
  EXPECT_TRUE(TracePrintInternalStdOut(fmt_type3, fmt_handle) > 0);
  EXPECT_TRUE(TracePrintInternalStdOut(fmt_type4, fmt_handle) > 0);
  EXPECT_TRUE(TracePrintInternalStdOut(fmt_type5, fmt_handle) > 0);

  EXPECT_TRUE(TracePrintInternalFile(err_file, fmt_type1, fmt_handle) > 0);
  EXPECT_TRUE(TracePrintInternalFile(err_file, fmt_type2, fmt_handle) > 0);
  EXPECT_TRUE(TracePrintInternalFile(err_file, fmt_type3, fmt_handle) > 0);
  EXPECT_TRUE(TracePrintInternalFile(err_file, fmt_type4, fmt_handle) > 0);
  EXPECT_TRUE(TracePrintInternalFile(err_file, fmt_type5, fmt_handle) > 0);
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

  EXPECT_EQ("\t\tSQLPOINTER, 0x0\n", fmt1);
  EXPECT_EQ("\t\tSQLSMALLINT *, 1\n", fmt2);
  EXPECT_EQ("\t\tSQLUSMALLINT *, 2\n", fmt3);
  EXPECT_EQ("\t\tSQLINTEGER *, 3\n", fmt4);
  EXPECT_EQ("\t\tSQLUINTEGER *, 4\n", fmt5);
  EXPECT_EQ("\t\tSQLCHAR *, Hello World\n", fmt6);
  EXPECT_EQ("\t\tSQLPOINTER *, 0x0\n", fmt7);
  EXPECT_EQ("\t\tSQLHANDLE *, 0x0\n", fmt8);

  EXPECT_TRUE(
      TracePrintInternalStdOut(fmt1, fmt2, fmt3, fmt4, fmt5, fmt6, fmt7, fmt8) > 0);
  EXPECT_TRUE(
      TracePrintInternalFile(err_file, fmt1, fmt2, fmt3, fmt4, fmt5, fmt6, fmt7, fmt8) > 0);
}

TEST(TraceLogging, Length)
{
  auto fmt1 = FormatSqlLen(10);
  auto fmt2 = FormatSqlULen(11);
  auto fmt3 = FormatSqlSetPosiRow(50);

  EXPECT_EQ("\t\tSQLLEN, 10\n", fmt1);
  EXPECT_EQ("\t\tSQLULEN, 11\n", fmt2);
  EXPECT_EQ("\t\tSQLSETPOSIROW, 50\n", fmt3);

  EXPECT_TRUE(TracePrintInternalStdOut(fmt1, fmt2, fmt3) > 0);
  EXPECT_TRUE(TracePrintInternalFile(err_file, fmt1, fmt2, fmt3) > 0);
}

TEST(TraceLogging, ReturnCodes)
{
  auto fmt1 = FormatSqlReturnCode(1);
  auto fmt2 = FormatSqlReturn(2);

  EXPECT_EQ("\t\tRETCODE, 1\n", fmt1);
  EXPECT_EQ("\t\tSQLRETURN, 2\n", fmt2);

  EXPECT_TRUE(TracePrintInternalStdOut(fmt1, fmt2) > 0);
  EXPECT_TRUE(TracePrintInternalFile(err_file, fmt1, fmt2) > 0);
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

  EXPECT_EQ("\t\tSQLDATE *, 1901-01-01\n", fmt1);
  EXPECT_EQ("\t\tSQLDECIMAL, 10\n", fmt2);
  EXPECT_EQ("\t\tSQLNUMERIC, 11\n", fmt3);
  EXPECT_EQ("\t\tSQLDOUBLE, 1.1000\n", fmt4);
  EXPECT_EQ("\t\tSQLFLOAT, 2.2000\n", fmt5);
  EXPECT_EQ("\t\tSQLREAL, 3.30\n", fmt6);
  EXPECT_EQ("\t\tSQLTIME *, 10:30:00\n", fmt7);
  EXPECT_EQ("\t\tSQLTIMESTAMP *, 1901-01-01 10:30:00\n", fmt8);
  EXPECT_EQ("\t\tSQLVARCHAR *, Hello\n", fmt9);
  EXPECT_EQ("\t\tSQLDECIMAL *, 10\n", fmt10);
  EXPECT_EQ("\t\tSQLNUMERIC *, 11\n", fmt11);
  EXPECT_EQ("\t\tSQLDOUBLE *, 1.1000\n", fmt12);
  EXPECT_EQ("\t\tSQLFLOAT *, 2.2000\n", fmt13);
  EXPECT_EQ("\t\tSQLREAL *, 3.30\n", fmt14);

  EXPECT_TRUE(
      TracePrintInternalStdOut(
          fmt1, fmt2, fmt3, fmt4, fmt5, fmt6, fmt7, fmt8,
          fmt9, fmt10, fmt11, fmt12, fmt13, fmt14) > 0);
  EXPECT_TRUE(
      TracePrintInternalFile(
          err_file, fmt1, fmt2, fmt3, fmt4, fmt5, fmt6,
          fmt7, fmt8, fmt9, fmt10, fmt11, fmt12, fmt13, fmt14) > 0);
}

}  // namespace odbc_bq_driver
}  // namespace cloud
}  // namespace google
