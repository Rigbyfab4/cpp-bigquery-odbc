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
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <cstdarg>
#include <cstdint>

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace google {
namespace cloud {
namespace odbc_bq_driver {

///////////////////////////
// Trace Options
///////////////////////////
struct TraceOptions {
    bool logging_enabled = false;
    int  log_level = 0;
    std::ofstream trace_file;
};
// Emit methods for actually printing the trace lines to stdout or a trace file.

// Clients of this utility should use the two methods below to emit
// a trace of all parameters to an stdout or a trace file.
std::string CollectAndPrintArgs(const std::string& func_name, int num_args, ...);
std::string CollectAndPrintArgsFile(
    const std::string& func_name, std::ofstream& file, int num_args, ...);

// Below are Helper methods for the above.

// Prints the trace string to stdout.
int TracePrintInternalStdOut(std::string& s);
// Prints the trace string to a trace file. 
// It is the responsibility of the caller to open and close the time
int TracePrintInternalFile(std::ofstream& file, std::string& s);
// Collects all the passed in arguments and returns a 
// formatted string to be traced for all the args.
std::string CollectArgs(va_list src_args, int num_args);

///////////////////////////////////////////
// Convenience Helper Methods.
////////////////////////////////////////////
const char *ToCStr(const std::string &str);
void ExitInternal(
    const std::string &func_name, RETCODE retCode, TraceOptions &opts);

////////////////////////////////////////////////////////////////////
// Additional Helper methods for validating and formatting strings
// based on parameter types.
////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////
// ODBC SQL TYPES
/////////////////////////////////////////////
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
std::string FormatSqlLen(const SQLLEN *l);
std::string FormatSqlULen(const SQLULEN *l);
// Return codes.
std::string FormatSqlReturnCode(RETCODE ret);
std::string FormatSqlReturn(SQLRETURN ret);
// Additional types specific to 3.x
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

/////////////////////////////////////////////
// Basic C Types
/////////////////////////////////////////////
std::string FormatString(const std::string& str);
std::string FormatCharString(const char* str);
std::string FormatCharArray(const char str[]);
std::string FormatChar(char c);
std::string FormatCharU(unsigned char c);
std::string FormatInt(int d);
std::string FormatIntU(unsigned int d);
std::string FormatLong(std::int64_t d);
std::string FormatLongU(std::uint64_t d);
std::string FormatShort(std::int16_t d);
std::string FormatShortU(std::uint16_t d);
std::string FormatDouble(double d);
std::string FormatFloat(float d);
std::string FormatPointer(void* p);
std::string FormatBool(bool b);
// Additional basic types (e.g. array or pointer versions of the above)
// may be added as needed.

///////////////////////////////////////////////
// To Be Implemented:
// 1) Unicode types.
// 2) Window specific types.

/////////////////////////////////////////////
// Unicode Types
/////////////////////////////////////////////


/////////////////////////////////////////////
// Window specific types.
/////////////////////////////////////////////

}  // namespace odbc_bq_driver
}  // namespace cloud
}  // namespace google
// NOLINTEND(modernize-concat-nested-namespaces)

#endif  // GOOGLE_CLOUD_ODBC_BQ_DRIVER_INTERNAL_TRACE_UTILS_H
