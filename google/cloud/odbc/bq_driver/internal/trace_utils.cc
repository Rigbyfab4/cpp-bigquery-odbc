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

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace google {
namespace cloud {
namespace odbc_bq_driver {

constexpr int kCharBufSize1 = 1024;
constexpr int kCharBufSize2 = 256;

int TracePrintInternalStdOut(std::string& s)
{
  if (s.empty()) {
    return -1;
  }
  std::cout << s << std::endl;
  return 0;
}

int TracePrintInternalFile(std::ofstream& file, std::string& s)
{
  if (!file.is_open())
  {
    return -1;
  }
  if (s.empty())
  {
    return -1;
  }
  file << s << std::endl;
  return 0;
}

std::string CollectArgs(va_list src_args, int num_args)
{
  std::string trace_str;
  va_list dest_args;
  va_copy(dest_args, src_args);
  for (int i = 0; i < num_args; i++)
  {
    std::string s = va_arg(dest_args, const char *);
    trace_str.append(s);
  }
  va_end(dest_args); // src_args needs to be ended by the caller.
  return trace_str;
}

std::string CollectAndPrintArgs(const std::string& func_name, int num_args, ...)
{
  std::string trace_str;
  trace_str.append(func_name);

  if (num_args > 0)
  {
    va_list args_list;
    va_start(args_list, num_args);
    trace_str.append(CollectArgs(args_list, num_args));
    va_end(args_list);

    int ret = TracePrintInternalStdOut(trace_str);
    if (ret < 0)
    {
      return "";
    }
  }
  return trace_str;
}

std::string CollectAndPrintArgsFile(
  const std::string& func_name, std::ofstream& file, int num_args, ...)
{
  std::string trace_str;
  trace_str.append(func_name);

  if (num_args > 0)
  {
    va_list args_list;
    va_start(args_list, num_args);
    trace_str.append(CollectArgs(args_list, num_args));
    va_end(args_list);

    int ret = TracePrintInternalFile(file, trace_str);
    if (ret < 0)
    {
      return "";
    }
  }
  return trace_str;
}

std::string FormatSqlSmallInt(SQLSMALLINT i)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %hi\n", "SQLSMALLINT", i);
  return buf;
}

std::string FormatSqlUSmallInt(SQLUSMALLINT i)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %hu\n", "SQLUSMALLINT", i);
  return buf;
}

std::string FormatSqlInteger(SQLINTEGER i) 
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %d\n", "SQLINTEGER", i);
  return buf;
}

std::string FormatSqlUInteger(SQLUINTEGER i)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %i\n", "SQLUINTEGER", i);
  return buf;
}

std::string FormatSqlHandleType(SQLSMALLINT type)
{
  char buf[kCharBufSize1];
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
  char buf[kCharBufSize1];
  if (!handle) {
    sprintf(buf, "\t\t%-s, 0x0\n", "SQL_NULL_HANDLE");
  } else {
    sprintf(buf, "\t\t%-s, %p\n", "SQL_HANDLE", handle);
  }
  return buf;
}

std::string FormatSqlPointer(SQLPOINTER p)
{
  char buf[kCharBufSize1];
  if (!p) {
    sprintf(buf, "\t\t%-s, 0x0\n", "SQLPOINTER");
  } else {
    sprintf(buf, "\t\t%-s, %p\n", "SQLPOINTER", p);
  }
  return buf;
}

std::string FormatSqlSmallInt(const SQLSMALLINT* p)
{
  char buf[kCharBufSize1];
  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLSMALLINT");
  else
    sprintf(buf, "\t\t%-s *, %hi\n", "SQLSMALLINT", *p);
  return buf;
}

std::string FormatSqlUSmallInt(const SQLUSMALLINT* p)
{
  char buf[kCharBufSize1];
  if (!p)
    sprintf(buf, "\t\t%-s *,  0x0\n", "SQLUSMALLINT");
  else
    sprintf(buf, "\t\t%-s *, %hu\n", "SQLUSMALLINT", *p);
  return buf;
}

std::string FormatSqlInteger(const SQLINTEGER* p)
{
  char buf[kCharBufSize1];
  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLINTEGER");
  else
    sprintf(buf, "\t\t%-s *, %d\n", "SQLINTEGER", *p);
  return buf;
}

std::string FormatSqlUInteger(const SQLUINTEGER* p)
{
  char buf[kCharBufSize1];
  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLUINTEGER");
  else
    sprintf(buf, "\t\t%-s *, %i\n", "SQLUINTEGER", *p);
  return buf;
}

std::string FormatSqlChar(const SQLCHAR* p)
{
  char buf[kCharBufSize1];

  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLCHAR");
  else
    sprintf(buf, "\t\t%-s *, %s\n", "SQLCHAR", p);
  return buf;
}

std::string FormatSqlPointer(const SQLPOINTER* p)
{
  char buf[kCharBufSize1];
  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLPOINTER");
  else
    sprintf(buf, "\t\t%-s *, %p\n", "SQLPOINTER", p);
  return buf;
}

std::string FormatSqlHandle(const SQLHANDLE* p)
{
  char buf[kCharBufSize1];
  if (!p)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLHANDLE");
  else
    sprintf(buf, "\t\t%-s *, %p\n", "SQLHANDLE", p);
  return buf;
}

#if (ODBCVER >= 0x0300)
std::string FormatSqlDate(const SQLDATE* d)
{
  char buf[kCharBufSize1];
  if (!d)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLDATE");
  else 
    sprintf(buf, "\t\t%-s *, %s\n", "SQLDATE", d);

  return buf;
}

std::string FormatSqlDecimal(SQLDECIMAL d)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %d\n", "SQLDECIMAL", d);
  return buf;
}

std::string FormatSqlDecimal(const SQLDECIMAL* d)
{
  char buf[kCharBufSize1];
  if (!d)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLDECIMAL");
  else
    sprintf(buf, "\t\t%-s *, %d\n", "SQLDECIMAL", *d);
  return buf;
}

std::string FormatSqlNumeric(SQLNUMERIC n) 
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %d\n", "SQLNUMERIC", n);
  return buf;
}

std::string FormatSqlNumeric(const SQLNUMERIC* n) 
{
  char buf[kCharBufSize1];
  if (!n)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLNUMERIC");
  else
    sprintf(buf, "\t\t%-s *, %d\n", "SQLNUMERIC", *n);
  return buf;
}

std::string FormatSqlDouble(SQLDOUBLE d)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %.4lf\n", "SQLDOUBLE", d);
  return buf;
}

std::string FormatSqlDouble(const SQLDOUBLE* d)
{
  char buf[kCharBufSize1];
  if (!d)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLDOUBLE");
  else
    sprintf(buf, "\t\t%-s *, %.4lf\n", "SQLDOUBLE", *d);
  return buf;
}

std::string FormatSqlFloat(SQLFLOAT f)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %.4lf\n", "SQLFLOAT", f);
  return buf;
}

std::string FormatSqlFloat(const SQLFLOAT* f)
{
  char buf[kCharBufSize1];
  if (!f)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLFLOAT");
  else
    sprintf(buf, "\t\t%-s *, %.4lf\n", "SQLFLOAT", *f);
  return buf;
}

std::string FormatSqlReal(SQLREAL r)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %.2f\n", "SQLREAL", r);
  return buf;
}

std::string FormatSqlReal(const SQLREAL* r)
{
  char buf[kCharBufSize1];
  if (!r)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLREAL");
  else
    sprintf(buf, "\t\t%-s *, %.2f\n", "SQLREAL", *r);
  return buf;
}

std::string FormatSqlTime(const SQLTIME* t)
{
  char buf[kCharBufSize1];
  if (!t)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLTIME");
  else 
    sprintf(buf, "\t\t%-s *, %s\n", "SQLTIME", t);

  return buf;
}

std::string FormatSqlTimestamp(const SQLTIMESTAMP* tp)
{
  char buf[kCharBufSize1];
  if (!tp)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLTIMESTAMP");
  else 
    sprintf(buf, "\t\t%-s *, %s\n", "SQLTIMESTAMP", tp);

  return buf;
}

std::string FormatSqlVarchar(const SQLVARCHAR* s)
{
  char buf[kCharBufSize1];
  if (!s)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLVARCHAR");
  else 
    sprintf(buf, "\t\t%-s *, %s\n", "SQLVARCHAR", s);

  return buf;
}
#endif  /* ODBCVER >= 0x0300 */

std::string FormatSqlLen(SQLLEN l)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %ld\n", "SQLLEN", l);
  return buf;
}

std::string FormatSqlULen(SQLULEN l)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %lu\n", "SQLULEN", l);
  return buf;
}

std::string FormatSqlSetPosiRow(SQLSETPOSIROW rp)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %hu\n", "SQLSETPOSIROW", rp);
  return buf;
}

std::string FormatSqlReturnCode(RETCODE ret)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %hi\n", "RETCODE", ret);
  return buf;
}

std::string FormatSqlReturn(SQLRETURN ret)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%-s, %hi\n", "SQLRETURN", ret);
  return buf;
}

std::string FormatSqlLen(SQLLEN *l)
{
  char buf[kCharBufSize1];
  if (!l)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLLEN");
  else
    sprintf(buf, "\t\t%-s *, %ld\n", "SQLLEN", *l);
  return buf;
}

std::string FormatSqlULen(SQLULEN *l)
{
  char buf[kCharBufSize1];
  if (!l)
    sprintf(buf, "\t\t%-s *, 0x0\n", "SQLULEN");
  else
    sprintf(buf, "\t\t%-s *, %lu\n", "SQLULEN", *l);
  return buf;
}

std::string FormatString(const std::string& str)
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%s\n", str.c_str());
  return buf;
}

std::string FormatCharString(const char* str)
{
  char buf[kCharBufSize1];
  if (!str)
    sprintf(buf, "\t\t 0x0 null string\n");
  else
    sprintf(buf, "\t\t%s\n", str);
  return buf;
}

std::string FormatCharArray(const char str[])
{
  char buf[kCharBufSize1];
  sprintf(buf, "\t\t%s\n", str);
  return buf;
}

std::string FormatChar(char c)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%c\n", c);
  return buf;
}

std::string FormatCharU(unsigned char c)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%c\n", c);
  return buf;
}

std::string FormatInt(int d)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%d\n", d);
  return buf;
}

std::string FormatIntU(unsigned int d)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%i\n", d);
  return buf;
}

std::string FormatLong(std::int64_t d)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%ld\n", d);
  return buf;
}

std::string FormatLongU(std::uint64_t d)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%li\n", d);
  return buf;
}

std::string FormatShort(std::int16_t d)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%hi\n", d);
  return buf;
}

std::string FormatShortU(std::uint16_t d)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%hu\n", d);
  return buf;
}

std::string FormatDouble(double d)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%.4lf\n", d);
  return buf;
}

std::string FormatFloat(float d)
{
  char buf[kCharBufSize2];
  sprintf(buf, "\t\t%.2f\n", d);
  return buf;
}

std::string FormatPointer(void* p)
{
  char buf[kCharBufSize2];
  if (!p)
    sprintf(buf, "\t\t0x0 null pointer\n");
  else
    sprintf(buf, "\t\t%p\n", p);
  return buf;
}

std::string FormatBool(bool b)
{
  char buf[kCharBufSize2];
  if (b)
    sprintf(buf, "\t\t%s\n", "TRUE");
  else
    sprintf(buf, "\t\t%s\n", "FALSE");
  return buf;
}

const char *ToCStr(const std::string &str)
{
  return str.c_str();
}

void ExitInternal(
    const std::string &func_name, RETCODE retCode, TraceOptions &opts)
{
  if (opts.logging_enabled)
  {
    if (opts.trace_file.is_open())
    {
      CollectAndPrintArgsFile(
          func_name, opts.trace_file, 1,
          ToCStr(FormatSqlReturnCode(retCode)));
    }
    else
    {
      CollectAndPrintArgs(
          func_name, 1,
          ToCStr(FormatSqlReturnCode(retCode)));
    }
  }
}

}  // namespace odbc_bq_driver
}  // namespace cloud
}  // namespace google
// NOLINTEND(modernize-concat-nested-namespaces)
