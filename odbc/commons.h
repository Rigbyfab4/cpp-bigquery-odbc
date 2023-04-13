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

#ifndef CPP_BIGQUERY_ODBC_ODBC_COMMONS_H
#define CPP_BIGQUERY_ODBC_ODBC_COMMONS_H

#include <iodbcext.h>
#include <locale.h>
#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <map>

using std::string;
using std::map;

namespace google {
namespace cloud {
namespace bigquery_odbc {

extern const SQLSMALLINT kBufferLength;
struct ConnectionHandle {
  HENV henv;
  HDBC hdbc;
  HSTMT hstmt;
  bool connected;
  SQLCHAR outdsn[4096];
};

inline SQLSMALLINT NumSqlChar(SQLCHAR * x) {
  return (sizeof(x) / sizeof(SQLCHAR));
}

inline const char * Cstr(string x) {
  return x.c_str();
}

SQLRETURN GetErrorDetails(const char *api, ConnectionHandle* conn);

}  // namespace bigquery_odbc
}  // namespace cloud
}  // namespace google

#endif  //CPP_BIGQUERY_ODBC_ODBC_COMMONS_H
