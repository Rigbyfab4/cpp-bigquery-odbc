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

#include <odbc/commons.h>

namespace google {
namespace cloud {
namespace bigquery_odbc {

const SQLSMALLINT kBufferLength = 512;

int GetErrorDetails(const char *api, ConnectionHandle* conn) {
  SQLCHAR buf[kBufferLength];
  SQLCHAR sqlstate[15];
  SQLINTEGER native_error = 0;
  SQLRETURN status;
  int rec_num;

  //Get statement errors
  rec_num = 0;
  while (conn->hstmt && rec_num < 5) {
    status = SQLGetDiagRec(SQL_HANDLE_STMT, conn->hstmt, ++rec_num, sqlstate, &native_error,
                        buf, kBufferLength, NULL);
    if (!SQL_SUCCEEDED(status)) {
      break;
    }
    printf("ERROR:: %d: %s = %s (%ld) SQLSTATE=%s\n", rec_num, api, buf,
            (long)native_error, sqlstate);
  }

  //Get connection errors
  rec_num = 0;
  while (conn->hdbc && rec_num < 5) {
    status = SQLGetDiagRec(SQL_HANDLE_DBC, conn->hdbc, ++rec_num, sqlstate, &native_error, buf,
                        kBufferLength, NULL);
    if (!SQL_SUCCEEDED(status)) {
      break;
    }
    printf("ERROR:: %d: %s = %s (%ld) SQLSTATE=%s\n", rec_num, api, buf,
        (long)native_error, sqlstate);
  }

  //Get environment errors
  rec_num = 0;
  while (conn->henv && rec_num < 5) {
    status = SQLGetDiagRec(SQL_HANDLE_ENV, conn->henv, ++rec_num, sqlstate, &native_error, buf,
                        kBufferLength, NULL);
    if (!SQL_SUCCEEDED(status)) {
      break;
    }
    printf("ERROR:: %d: %s = %s (%ld) SQLSTATE=%s\n", rec_num, api, buf,
        (long)native_error, sqlstate);
  }

  return status;
}

}  // namespace bigquery_odbc
}  // namespace cloud
}  // namespace google
