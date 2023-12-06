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

#ifndef GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_TRACE_UTILS_H
#define GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_TRACE_UTILS_H

#include "odbc_includes.h"

#include <algorithm>
#include <fstream>
#include <memory>
#include <map>
#include <string>
// NOLINTBEGIN(modernize-deprecated-headers)
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
// NOLINTEND(modernize-deprecated-headers)

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace google {
namespace cloud {
namespace odbc_bq_driver {

// Emit methods for actually printing the trace lines to stdout or a trace file.

// Prints to stdout.
int TracePrintInternalStdOut(const std::string& fmt, ...);
// Prints to a trace file.
int TracePrintInternalFile(FILE* file, const std::string& fmt, ...);

// Returns formatted string for different parameter types.

// Basic types.
std::string FormatSqlSmallInt(SQLSMALLINT i);
std::string FormatSqlUSmallInt(SQLUSMALLINT i);
std::string FormatSqlInteger(SQLINTEGER i);
std::string FormatSqlUInteger(SQLUINTEGER i);

// Handles.
std::string FormatSqlHandleType(SQLSMALLINT type);
std::string FormatSqlHandle(SQLHANDLE handle);

// Pointers.
std::string FormatSqlPointer(SQLPOINTER p);
std::string FormatSqlSmallInt(const SQLSMALLINT* p);
std::string FormatSqlUSmallInt(const SQLUSMALLINT* p);
std::string FormatSqlInteger(const SQLINTEGER* p);
std::string FormatSqlUInteger(const SQLUINTEGER* p);
std::string FormatSqlChar(const SQLCHAR* p);
std::string FormatSqlPointer(const SQLPOINTER* p);
std::string FormatSqlHandle(const SQLHANDLE* p);

// length.
std::string FormatSqlLen(SQLLEN l);
std::string FormatSqlULen(SQLULEN l);
std::string FormatSqlSetPosiRow(SQLSETPOSIROW rp);

// Return codes.
std::string FormatSqlReturnCode(RETCODE ret);
std::string FormatSqlReturn(SQLRETURN ret);

#if (ODBCVER >= 0x0300)
std::string FormatSqlDate(const SQLDATE* d);
std::string FormatSqlDecimal(SQLDECIMAL d);
std::string FormatSqlDecimal(const SQLDECIMAL* d);
std::string FormatSqlNumeric(SQLNUMERIC n);
std::string FormatSqlNumeric(const SQLNUMERIC* n);
std::string FormatSqlDouble(SQLDOUBLE d);
std::string FormatSqlDouble(const SQLDOUBLE* d);
std::string FormatSqlFloat(SQLFLOAT f);
std::string FormatSqlFloat(const SQLFLOAT* f);
std::string FormatSqlReal(SQLREAL r);
std::string FormatSqlReal(const SQLREAL* r);
std::string FormatSqlTime(const SQLTIME* t);
std::string FormatSqlTimestamp(const SQLTIMESTAMP* tp);
std::string FormatSqlVarchar(const SQLVARCHAR* s);
#endif  /* ODBCVER >= 0x0300 */

// To Be Implemented:
// 1) Unicode types.
// 2) Window specific types.
// 3) Trace Options.

}  // namespace odbc_bq_driver
}  // namespace cloud
}  // namespace google
// NOLINTEND(modernize-concat-nested-namespaces)

#endif  // GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_TRACE_UTILS_H
