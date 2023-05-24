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

namespace google {
namespace cloud {
namespace bigquery_odbc {

SQLSMALLINT kMaxDsnLen = 1024; // Maximum number of characters in a data source name

SQLRETURN Connect(std::string conn_str, std::shared_ptr<ConnectionHandle> conn) {
  SQLSMALLINT buflen;
  SQLCHAR data_source[kMaxDsnLen];
  SQLSMALLINT out_len;
  SQLRETURN status;

  status = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &conn->henv);
  CheckError(status, "SQLAllocHandle", conn);

  status = SQLSetEnvAttr(conn->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3,
                SQL_IS_UINTEGER);
  CheckError(status, "SQLSetEnvAttr", conn);
  
  status = SQLAllocHandle(SQL_HANDLE_DBC, conn->henv, &conn->hdbc);     
  CheckError(status, "SQLAllocHandle", conn);

  // Set the application name
  status = SQLSetConnectAttr(conn->hdbc, SQL_APPLICATION_NAME, (SQLPOINTER)("odbctest"), SQL_NTS);
  CheckError(status, "SQLSetConnectAttr", conn);

  StrToChar((char *)data_source, conn_str);

  status = SQLDriverConnect(conn->hdbc, 0, (SQLCHAR *)data_source, SQL_NTS,
                            (SQLCHAR *)conn->outdsn, NumSqlChar(conn->outdsn), &buflen,
                            SQL_DRIVER_COMPLETE);
  CheckError(status, "SQLDriverConnect", conn);
  conn->connected = true;
  
  PrintDriverVerName(conn);
  
  // Allocate statement handle
  status = SQLAllocHandle(SQL_HANDLE_STMT, conn->hdbc, &conn->hstmt);
  CheckError(status, "SQLAllocHandle", conn);
  return status;
}

// Disconnect from the database
SQLRETURN Disconnect(std::shared_ptr<ConnectionHandle> conn) {
  SQLRETURN status;
  if (conn->hstmt) {
    // Using SQLFreeStmt rather than SQLCloseCursor(conn->hstmt) is better since it doesn't
    //  fail if no cursor was open.
    status = SQLFreeStmt(conn->hstmt, SQL_CLOSE);
    CheckError(status, "SQLFreeStmt", conn);
    status = SQLFreeHandle(SQL_HANDLE_STMT, conn->hstmt);
    CheckError(status, "SQLFreeHandle", conn);
  }
  if (conn->connected) {
    status = SQLDisconnect(conn->hdbc);
    CheckError(status, "SQLDisconnect", conn);
  }
  if (conn->hdbc) {
    status = SQLFreeHandle(SQL_HANDLE_DBC, conn->hdbc);
    CheckError(status, "SQLFreeHandle", conn);
  }
  if (conn->henv) {
    status = SQLFreeHandle(SQL_HANDLE_ENV, conn->henv);
    CheckError(status, "SQLFreeHandle", conn);
  }
  return 0;
}

// TODO(#10): Remove printf and support logging
// Gets Info about the driver.
SQLRETURN GetDriverInfo(std::shared_ptr<ConnectionHandle> conn) {
  SQLCHAR out[kMaxDsnLen];
  SQLSMALLINT out_len;
  SQLRETURN status;

  printf("\n\n****************************************\n");
  printf("Driver Info \n");
  printf("****************************************\n");
  status = SQLGetInfo(conn->hdbc, SQL_DATA_SOURCE_NAME, out, sizeof(out),
                   &out_len);
  if (SQL_SUCCEEDED(status)) {
    if (status == SQL_SUCCESS_WITH_INFO) {
      printf("[Truncated]");
    }
    printf("SQL_DATA_SOURCE_NAME: %s\n", out);
  } else {
    printf("Error calling SqlGetInfo for SQL_DATA_SOURCE_NAME: %d\n", status);
    return status;
  }
  status = SQLGetInfo(conn->hdbc, SQL_ODBC_VER, out, sizeof(out),
                   &out_len);
  if (SQL_SUCCEEDED(status)) {
    if (status == SQL_SUCCESS_WITH_INFO) {
      printf("[Truncated]");
    }
    printf("SQL_ODBC_VER: %s\n", out);
  } else {
    printf("Error calling SqlGetInfo for SQL_ODBC_VER: %d\n", status);
    return status;
  }
  status = SQLGetInfo(conn->hdbc, SQL_DATABASE_NAME, out, sizeof(out),
                   &out_len);
  if (SQL_SUCCEEDED(status)) {
    if (status == SQL_SUCCESS_WITH_INFO) {
      printf("[Truncated]");
    }
    printf("SQL_DATABASE_NAME: %s\n", out);
  } else {
    printf("Error calling SqlGetInfo for SQL_DATABASE_NAME: %d\n", status);
    return status;
  }
  status = SQLGetInfo(conn->hdbc, SQL_DRIVER_NAME, out, sizeof(out),
                   &out_len);
  if (SQL_SUCCEEDED(status)) {
    if (status == SQL_SUCCESS_WITH_INFO) {
      printf("[Truncated]");
    }
    printf("SQL_DRIVER_NAME: %s\n", out);
  } else {
    printf("Error calling SqlGetInfo for SQL_DRIVER_NAME: %d\n", status);
    return status;
  }
  status = SQLGetInfo(conn->hdbc, SQL_DRIVER_ODBC_VER, out, sizeof(out),
                   &out_len);
  if (SQL_SUCCEEDED(status)) {
    if (status == SQL_SUCCESS_WITH_INFO) {
      printf("[Truncated]");
    }
    printf("SQL_DRIVER_ODBC_VER: %s\n", out);
  } else {
    printf("Error calling SqlGetInfo for SQL_DRIVER_ODBC_VER: %d\n", status);
    return status;
  }
  status = SQLGetInfo(conn->hdbc, SQL_DRIVER_VER, out, sizeof(out),
                   &out_len);
  if (SQL_SUCCEEDED(status))
  {
    if (status == SQL_SUCCESS_WITH_INFO) {
      printf("[Truncated]");
    }
    printf("SQL_DRIVER_VER: %s\n\n", out);
  } else {
    printf("Error calling SqlGetInfo for SQL_DRIVER_VER: %d\n", status);
    return status;
  }
  return status;
}

// TODO(#10): Remove printf and support logging
// Prints if the environment is ODBC3
SQLRETURN GetEnvInfo(std::shared_ptr<ConnectionHandle> conn) {
  SQLUINTEGER out;
  auto status = SQLGetEnvAttr(conn->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)&out,
                          SQL_IS_UINTEGER, NULL);
  if (SQL_SUCCEEDED(status) && out == SQL_OV_ODBC3) {
    printf("****************************************\n");
    printf("Environment is ODBC3\n");
    printf("****************************************\n\n");
    return status;
  }
  return status;
}

// TODO(#10): Remove printf and support logging
SQLRETURN GetDescRec(std::shared_ptr<ConnectionHandle> conn) {
  SQLRETURN status;
  SQLSMALLINT desc_type;
  SQLHDESC desc_handle;

  status = SQLGetStmtAttr(conn->hstmt, SQL_ATTR_APP_ROW_DESC, &desc_handle, 0, NULL);
  CheckError(status, "SQLGetStmtAttr", conn);
  // Set the type
  status = SQLSetDescField(desc_handle, 1, SQL_DESC_TYPE,
                            (SQLPOINTER)SQL_C_DEFAULT, SQL_IS_SMALLINT);
  CheckError(status, "SQLSetDescField", conn);
  
  // get number of fields in the descriptor
  status = SQLGetDescField(desc_handle, 1, SQL_DESC_TYPE, &desc_type,
                            SQL_IS_SMALLINT, NULL);
  CheckError(status, "SQLGetDescField", conn);
  printf("SQLGetDescField Succeeded: desc_type[%d] \n\n", desc_type);
  
  SQLSMALLINT stringLength;
  SQLSMALLINT type;
  SQLSMALLINT subType;
  SQLLEN length;
  SQLSMALLINT precision;
  SQLSMALLINT scale;
  SQLSMALLINT nullable;
  SQLCHAR name[kBufferLength];
  status =
      SQLGetDescRec(desc_handle, 1, name, kBufferLength, &stringLength, &type,
                    &subType, &length, &precision, &scale, &nullable);
  if (!SQL_SUCCEEDED(status)) {
    if (status == SQL_NO_DATA) {
      printf("\nNo records for SQLGetDescRec !\n");
    } else {
      return status;
    }
  } else {
    printf("\nName %s, ", name);
    printf("Type %i, ", (int)type);
    printf("SubType %i, ", (int)subType);
    printf("Length %i, ", (int)length);
    printf("Precision %i, ", (int)precision);
    printf(" Scale %i, ", (int)scale);
    printf("Nullable %i\n\n", (int)nullable);
  }
  return status;
}


// TODO(#10): Remove printf and support logging
// Print the version and the name of the connected driver
SQLRETURN PrintDriverVerName(std::shared_ptr<ConnectionHandle> conn) {
  SQLCHAR driver_info[kBufferLength];
  SQLSMALLINT out_len;
  SQLRETURN status;
  status = SQLGetInfo(conn->hdbc, SQL_DRIVER_VER, driver_info, NumSqlChar(driver_info), &out_len);
  CheckError(status, "SQLGetInfo", conn);
  printf("Driver: %s", driver_info);
  status = SQLGetInfo(conn->hdbc, SQL_DRIVER_NAME, driver_info, NumSqlChar(driver_info),
                      &out_len);
  CheckError(status, "SQLGetInfo", conn);
  printf(" (%s) \n\n", driver_info);
  return status;
}

}  // namespace bigquery_odbc
}  // namespace cloud
}  // namespace google
