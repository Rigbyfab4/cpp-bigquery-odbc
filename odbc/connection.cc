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

namespace google {
namespace cloud {
namespace bigquery_odbc {

const char *kDefaultConnectionString = "DSN=SampleDSN";

SQLSMALLINT kBufferLength = 255;
SQLSMALLINT kMaxDsnLen = 1024; //Maximum number of characters in a data source name

SQLRETURN Connect(char *conn_str, ConnectionHandle *conn) {
  SQLSMALLINT buflen;
  SQLCHAR data_source[kMaxDsnLen];
  SQLSMALLINT out_len;
  SQLRETURN status;

  status = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &conn->henv);
  if (!SQL_SUCCEEDED(status)){
    return status;
  }
  SQLSetEnvAttr(conn->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3,
                SQL_IS_UINTEGER);
  
  status = SQLAllocHandle(SQL_HANDLE_DBC, conn->henv, &conn->hdbc);     
  if (!SQL_SUCCEEDED(status)) {
    return status;
  }

  //Set the application name
  SQLSetConnectOption(conn->hdbc, SQL_APPLICATION_NAME, (SQLULEN)("odbctest"));

  strcpy((char *)data_source, conn_str);

  status = SQLDriverConnect(conn->hdbc, 0, (SQLCHAR *)data_source, SQL_NTS,
                            (SQLCHAR *)conn->outdsn, NumSqlChar(conn->outdsn), &buflen,
                            SQL_DRIVER_COMPLETE);
  if (!SQL_SUCCEEDED(status)) {
    return status;
  }
  conn->connected = true;
  
  status = PrintDriverVerName(conn);
  if (!SQL_SUCCEEDED(status)){
    return status;
  }
  
  //Allocate statement handle
  status = SQLAllocHandle(SQL_HANDLE_STMT, conn->hdbc, &conn->hstmt);
  return status;
}

//Disconnect from the database
SQLRETURN Disconnect(ConnectionHandle *conn) {
  if (conn->hstmt) {
    SQLCloseCursor(conn->hstmt);
    SQLFreeHandle(SQL_HANDLE_STMT, conn->hstmt);
  }
  if (conn->connected) {
    SQLDisconnect(conn->hdbc);
  }
  if (conn->hdbc) {
    SQLFreeHandle(SQL_HANDLE_DBC, conn->hdbc);
  }
  if (conn->henv) {
    SQLFreeHandle(SQL_HANDLE_ENV, conn->henv);
  }
  return 0;
}

//Gets Info about the driver.
SQLRETURN GetDriverInfo(ConnectionHandle *conn) {
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


// Prints if the environment is ODBC3
SQLRETURN GetEnvInfo(ConnectionHandle *conn) {
  SQLUINTEGER out;
  SQLRETURN status = SQLGetEnvAttr(conn->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)&out,
                          SQL_IS_UINTEGER, NULL);
  if (SQL_SUCCEEDED(status) && out == SQL_OV_ODBC3) {
    printf("****************************************\n");
    printf("Environment is ODBC3\n");
    printf("****************************************\n\n");
    return status;
  }
  return status;
}

SQLRETURN GetDescRec(ConnectionHandle *conn) {
  SQLRETURN status;
  SQLSMALLINT desc_type;
  SQLHDESC desc_handle;

  status = SQLGetStmtAttr(conn->hstmt, SQL_ATTR_APP_ROW_DESC, &desc_handle, 0, NULL);
  if (!SQL_SUCCEEDED(status)) {
    return status;
  }
  // Set the type
  status = SQLSetDescField(desc_handle, 1, SQL_DESC_TYPE,
                            (SQLPOINTER)SQL_C_DEFAULT, SQL_IS_SMALLINT);
  if (!SQL_SUCCEEDED(status)) {
    return status;
  }
  printf("SQLSetDescField Succeeded!\n\n");
  
  // get number of fields in the descriptor
  status = SQLGetDescField(desc_handle, 1, SQL_DESC_TYPE, &desc_type,
                            SQL_IS_SMALLINT, NULL);
  if (!SQL_SUCCEEDED(status)) {
    return status;
  }
  printf("SQLGetDescField Succeeded: desc_type[%d] \n\n", desc_type);
  

  printf("Trying SQLGetDescRec:\n");
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


// Print the version and the name of the connected driver
SQLRETURN PrintDriverVerName(ConnectionHandle *conn) {
  SQLCHAR driver_info[kBufferLength];
  SQLSMALLINT out_len;
  SQLRETURN status;
  status = SQLGetInfo(conn->hdbc, SQL_DRIVER_VER, driver_info, NumSqlChar(driver_info), &out_len);
  if (SQL_SUCCEEDED(status)) {
    printf("Driver: %s", driver_info);
    status = SQLGetInfo(conn->hdbc, SQL_DRIVER_NAME, driver_info, NumSqlChar(driver_info),
                        &out_len);
    if (SQL_SUCCEEDED(status)) {
      printf(" (%s) \n\n", driver_info);
    }
  }
  return status;
}

}  // namespace bigquery_odbc
}  // namespace cloud
}  // namespace google
