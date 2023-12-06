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

namespace google {
namespace cloud {
namespace odbc_bq_driver {

int TracePrintInternalStdOut(const std::string& fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int ret = vprintf(fmt.c_str(), args);
  va_end(args);
  return ret;
}

int TracePrintInternalFile(FILE* file, const std::string& fmt, ...)
{
  int ret = -1;
  if (!file) {
    return ret;
  }
  va_list args;
  va_start(args, fmt);
  ret = vfprintf(file, fmt.c_str(), args);
  va_end(args);
  return ret;
}

std::string FormatSqlSmallInt(SQLSMALLINT i)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %hi\n", "SQLSMALLINT", i);
  return buf;
}

std::string FormatSqlUSmallInt(SQLUSMALLINT i)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %hu\n", "SQLUSMALLINT", i);
  return buf;
}

std::string FormatSqlInteger(SQLINTEGER i) 
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %d\n", "SQLINTEGER", i);
  return buf;
}

std::string FormatSqlUInteger(SQLUINTEGER i)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %i\n", "SQLUINTEGER", i);
  return buf;
}

std::string FormatSqlHandleType(SQLSMALLINT type)
{
  char buf[1024];
  switch (type)
  {
    case SQL_HANDLE_ENV:
    {
      sprintf(buf, "\t\t%-s, handle type=%hi\n", "SQL_HANDLE_ENV", type);
      break;
    }
    case SQL_HANDLE_DBC:
    {
      sprintf(buf, "\t\t%-s, handle type=%hi\n", "SQL_HANDLE_DBC", type);
      break;
    }
    case SQL_HANDLE_DESC:
    {
      sprintf(buf, "\t\t%-s, handle type=%hi\n", "SQL_HANDLE_DESC", type);
      break;
    }
    case SQL_HANDLE_STMT:
    {
      sprintf(buf, "\t\t%-s, handle type=%hi\n", "SQL_HANDLE_STMT", type);
      break;
    }
    default:
    {
      sprintf(buf, "\t\t%-s, handle type=%hi\n", "Unknown Handle Type", type);
    }
  }
  return buf;
}

std::string FormatSqlHandle(SQLHANDLE handle)
{
  char buf[1024];
  if (!handle) {
    sprintf(buf, "\t\t%-s, 0x0\n", "SQL_NULL_HANDLE");
  } else {
    sprintf(buf, "\t\t%-s, %p\n", "SQL_HANDLE", handle);
  }
  return buf;
}

std::string FormatSqlPointer(SQLPOINTER p)
{
  char buf[1024];
  if (!p) {
    sprintf(buf, "\t\t%-s, 0x0\n", "SQLPOINTER");
  } else {
    sprintf(buf, "\t\t%-s, %p\n", "SQLPOINTER", p);
  }
  return buf;
}

std::string FormatSqlSmallInt(const SQLSMALLINT* p)
{
  char buf[1024];
   if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLSMALLINT");
  else
    sprintf(buf, "\t\t%-s *, %hi\n", "SQLSMALLINT", *p);
  return buf;
}

std::string FormatSqlUSmallInt(const SQLUSMALLINT* p)
{
  char buf[1024];
   if (!p)
    sprintf(buf, "\t\t%-s *,  0x0\n", "SQLUSMALLINT");
  else
    sprintf(buf, "\t\t%-s *, %hu\n", "SQLUSMALLINT", *p);
  return buf;
}

std::string FormatSqlInteger(const SQLINTEGER* p)
{
  char buf[1024];
   if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLINTEGER");
  else
    sprintf(buf, "\t\t%-s *, %d\n", "SQLINTEGER", *p);
  return buf;
}

std::string FormatSqlUInteger(const SQLUINTEGER* p)
{
  char buf[1024];
  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLUINTEGER");
  else
    sprintf(buf, "\t\t%-s *, %i\n", "SQLUINTEGER", *p);
  return buf;
}

std::string FormatSqlChar(const SQLCHAR* p)
{
  char buf[1024];

  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLCHAR");
  else
    sprintf(buf, "\t\t%-s *, %s\n", "SQLCHAR", p);
  return buf;
}

std::string FormatSqlPointer(const SQLPOINTER* p)
{
  char buf[1024];
  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLPOINTER");
  else
    sprintf(buf, "\t\t%-s *, %p\n", "SQLPOINTER", p);
  return buf;
}

std::string FormatSqlHandle(const SQLHANDLE* p)
{
  char buf[1024];
  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLHANDLE");
  else
    sprintf(buf, "\t\t%-s *, %p\n", "SQLHANDLE", p);
  return buf;
}

#if (ODBCVER >= 0x0300)
std::string FormatSqlDate(const SQLDATE* d)
{
  char buf[1024];
  if (!d)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLDATE");
  else 
    sprintf(buf, "\t\t%-s *, %s\n", "SQLDATE", d);

  return buf;
}

std::string FormatSqlDecimal(SQLDECIMAL d)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %d\n", "SQLDECIMAL", d);
  return buf;
}

std::string FormatSqlDecimal(const SQLDECIMAL* d)
{
  char buf[1024];
  if (!d)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLDECIMAL");
  else
    sprintf(buf, "\t\t%-s *, %d\n", "SQLDECIMAL", *d);
  return buf;
}

std::string FormatSqlNumeric(SQLNUMERIC n) 
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %d\n", "SQLNUMERIC", n);
  return buf;
}

std::string FormatSqlNumeric(const SQLNUMERIC* n) 
{
  char buf[1024];
  if (!n)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLNUMERIC");
  else
    sprintf(buf, "\t\t%-s *, %d\n", "SQLNUMERIC", *n);
  return buf;
}

std::string FormatSqlDouble(SQLDOUBLE d)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %.4lf\n", "SQLDOUBLE", d);
  return buf;
}

std::string FormatSqlDouble(const SQLDOUBLE* d)
{
  char buf[1024];
  if (!d)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLDOUBLE");
  else
    sprintf(buf, "\t\t%-s *, %.4lf\n", "SQLDOUBLE", *d);
  return buf;
}

std::string FormatSqlFloat(SQLFLOAT f)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %.4lf\n", "SQLFLOAT", f);
  return buf;
}

std::string FormatSqlFloat(const SQLFLOAT* f)
{
  char buf[1024];
  if (!f)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLFLOAT");
  else
    sprintf(buf, "\t\t%-s *, %.4lf\n", "SQLFLOAT", *f);
  return buf;
}

std::string FormatSqlReal(SQLREAL r)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %.2f\n", "SQLREAL", r);
  return buf;
}

std::string FormatSqlReal(const SQLREAL* r)
{
  char buf[1024];
  if (!r)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLREAL");
  else
    sprintf(buf, "\t\t%-s *, %.2f\n", "SQLREAL", *r);
  return buf;
}

std::string FormatSqlTime(const SQLTIME* t)
{
  char buf[1024];
  if (!t)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLTIME");
  else 
    sprintf(buf, "\t\t%-s *, %s\n", "SQLTIME", t);

  return buf;
}

std::string FormatSqlTimestamp(const SQLTIMESTAMP* tp)
{
  char buf[1024];
  if (!tp)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLTIMESTAMP");
  else 
    sprintf(buf, "\t\t%-s *, %s\n", "SQLTIMESTAMP", tp);

  return buf;
}

std::string FormatSqlVarchar(const SQLVARCHAR* s)
{
  char buf[1024];
  if (!s)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLVARCHAR");
  else 
    sprintf(buf, "\t\t%-s *, %s\n", "SQLVARCHAR", s);

  return buf;
}
#endif  /* ODBCVER >= 0x0300 */

std::string FormatSqlLen(SQLLEN l)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %ld\n", "SQLLEN", l);
  return buf;
}

std::string FormatSqlULen(SQLULEN l)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %lu\n", "SQLULEN", l);
  return buf;
}

std::string FormatSqlSetPosiRow(SQLSETPOSIROW rp)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %hu\n", "SQLSETPOSIROW", rp);
  return buf;
}

std::string FormatSqlReturnCode(RETCODE ret)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %hi\n", "RETCODE", ret);
  return buf;
}

std::string FormatSqlReturn(SQLRETURN ret)
{
  char buf[1024];
  sprintf(buf, "\t\t%-s, %hi\n", "SQLRETURN", ret);
  return buf;
}

}  // namespace odbc_bq_driver
}  // namespace cloud
}  // namespace google
