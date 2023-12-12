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

#ifndef GOOGLE_CLOUD_ODBC_BQ_DRIVER_ODBC_TRACE_H
#define GOOGLE_CLOUD_ODBC_BQ_DRIVER_ODBC_TRACE_H

#include "google/cloud/odbc/bq_driver/internal/trace_utils.h"

#include <string>
#include <map>

/////////////////////////////////////////////////////////////
// Defines the functions related to tracing entry and exit
// of all ODBC APIs. Tracing includes tracing of parameters,
// API names and return codes.
/////////////////////////////////////////////////////////////

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace google {
namespace cloud {
namespace odbc_bq_driver {

void TraceFunctionEntry_SQLAllocHandle(
    SQLSMALLINT handleType, SQLHANDLE inputHandle,
    SQLHANDLE *outputHandle, TraceOptions opts = {});
void TraceFunctionExit_SQLAllocHandle(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLDriverConnect(
    SQLHDBC connectionHandle, SQLHWND windowHandle,
    SQLCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen,
    SQLSMALLINT *outConnectionStringLen,
    SQLUSMALLINT driverCompletion, TraceOptions opts = {});
void TraceFunctionExit_SQLDriverConnect(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLDriverConnectW( 
    SQLHDBC connectionHandle, SQLHWND windowHandle,
    SQLWCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLWCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen,
    SQLSMALLINT *outConnectionStringLen,
    SQLUSMALLINT driverCompletion, TraceOptions opts = {});
void TraceFunctionExit_SQLDriverConnectW(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLBrowseConnect(
    SQLHDBC connectionHandle, SQLCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen,
    SQLSMALLINT *outConnectionStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLBrowseConnect(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLBrowseConnectW(
    SQLHDBC connectionHandle, SQLWCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLWCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen,
    SQLSMALLINT *outConnectionStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLBrowseConnectW(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLConnect(
    SQLHDBC connectionHandle, SQLCHAR *serverName,
    SQLSMALLINT serverNameLen,
    SQLCHAR *userName, SQLSMALLINT userNameLen, SQLCHAR *authString,
    SQLSMALLINT authStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLConnect(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLConnectW(
    SQLHDBC connectionHandle, SQLWCHAR *serverName,
    SQLSMALLINT serverNameLen, SQLWCHAR *userName, SQLSMALLINT userNameLen, 
    SQLWCHAR *authString, SQLSMALLINT authStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLConnectW(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetInfo(
    SQLHDBC connectionHandle, SQLUSMALLINT infoType, SQLPOINTER infoValue,
    SQLSMALLINT infoValueBufferLen, SQLSMALLINT *infoValueStringLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetInfo(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetInfoW(
    SQLHDBC connectionHandle, SQLUSMALLINT infoType, SQLPOINTER infoValue,
    SQLSMALLINT infoValueBufferLen, SQLSMALLINT *infoValueStringLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetInfoW(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetFunctions(
    SQLHDBC connectionHandle, SQLUSMALLINT functionId,
    SQLUSMALLINT *supportedFunction, TraceOptions opts = {});
void TraceFunctionExit_SQLGetFunctions(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetTypeInfo(
    SQLHSTMT statementHandle,SQLSMALLINT dataType, TraceOptions opts = {});
void TraceFunctionExit_SQLGetTypeInfo(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetConnectAttr(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value, 
    SQLINTEGER valueStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLSetConnectAttr(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetConnectAttrW(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value, 
    SQLINTEGER valueStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLSetConnectAttrW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetConnectAttr(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetConnectAttr(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetConnectAttrW(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetConnectAttrW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetStmtAttr(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLSetStmtAttr(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetStmtAttrW(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value, 
    SQLINTEGER valueStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLSetStmtAttrW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetStmtAttr(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetStmtAttr(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetStmtAttrW(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetStmtAttrW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetEnvAttr(
    SQLHENV environmentHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLSetEnvAttr(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetEnvAttr(
    SQLHENV environmentHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetEnvAttr(
SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetDescField(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber, SQLSMALLINT fieldId,
    SQLPOINTER outDescValue, SQLINTEGER outDescValueBufferLen,
    SQLINTEGER *outDescValueStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLGetDescField(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetDescFieldW(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber, SQLSMALLINT fieldId,
    SQLPOINTER outDescValue, SQLINTEGER outDescValueBufferLen,
    SQLINTEGER *outDescValueStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLGetDescFieldW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetDescRec(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLCHAR *name, SQLSMALLINT nameBufferLen, SQLSMALLINT *nameStringLen,
    SQLSMALLINT *descType, SQLSMALLINT *descSubType, SQLLEN *descOctetLen,
    SQLSMALLINT *descPrecision, SQLSMALLINT *descScale,
    SQLSMALLINT *nullable, TraceOptions opts = {});
void TraceFunctionExit_SQLGetDescRec(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetDescRecW(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLWCHAR *name, SQLSMALLINT nameBufferLen, SQLSMALLINT *nameStringLen,
    SQLSMALLINT *descType, SQLSMALLINT *descSubType, SQLLEN *descOctetLen,
    SQLSMALLINT *descPrecision, SQLSMALLINT *descScale,
    SQLSMALLINT *nullable, TraceOptions opts = {});
void TraceFunctionExit_SQLGetDescRecW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetDescField(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLSMALLINT fieldIdentifier, SQLPOINTER descValue,
    SQLINTEGER descValueBufferLen, TraceOptions opts = {});
void TraceFunctionExit_SQLSetDescField(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetDescFieldW(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLSMALLINT fieldIdentifier, SQLPOINTER descValue,
    SQLINTEGER descValueBufferLen, TraceOptions opts = {});
void TraceFunctionExit_SQLSetDescFieldW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetDescRec(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLSMALLINT descType, SQLSMALLINT descSubType,
    SQLLEN descOctetLen, SQLSMALLINT descPrecision, SQLSMALLINT descScale,
    SQLPOINTER descData, SQLLEN *descOctetLenPtr, SQLLEN *descIndicator,
    TraceOptions opts = {});
void TraceFunctionExit_SQLSetDescRec(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLCopyDesc(
    SQLHDESC sourceDescHandle, SQLHDESC targetDescHandle,
    TraceOptions opts = {});
void TraceFunctionExit_SQLCopyDesc(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLPrepare(
    SQLHSTMT statementHandle, SQLCHAR *statementText,
    SQLINTEGER statementTextLen, TraceOptions opts = {});
void TraceFunctionExit_SQLPrepare(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLPrepareW(
    SQLHSTMT statementHandle, SQLWCHAR *statementText,
    SQLINTEGER statementTextLen, TraceOptions opts = {});
void TraceFunctionExit_SQLPrepareW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLBindParameter(
    SQLHSTMT statementHandle, SQLUSMALLINT paramNumber,
    SQLSMALLINT paramType, SQLSMALLINT paramCType, SQLSMALLINT paramSqlType,
    SQLULEN paramColSize, SQLSMALLINT paramScale,
    SQLPOINTER paramDataValue, SQLLEN paramDataValueBufferLen,
    SQLLEN *paramDataValueStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLBindParameter(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetCursorName(
    SQLHSTMT statementHandle,
    SQLCHAR *cursorName, SQLSMALLINT cursorNameBufferLen,
    SQLSMALLINT *cursorNameStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLGetCursorName(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetCursorNameW(
    SQLHSTMT statementHandle,
    SQLWCHAR *cursorName, SQLSMALLINT cursorNameBufferLen,
    SQLSMALLINT *cursorNameStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLGetCursorNameW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetCursorName(
    SQLHSTMT statementHandle, SQLCHAR *cursorName, SQLSMALLINT cursorNameLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLSetCursorName(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetCursorNameW(
    SQLHSTMT statementHandle, SQLWCHAR *cursorName, SQLSMALLINT cursorNameLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLSetCursorNameW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLExecute(
    SQLHSTMT statementHandle, TraceOptions opts = {});
void TraceFunctionExit_SQLExecute(SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLExecDirect(
    SQLHSTMT statementHandle, SQLCHAR *statementText,
    SQLINTEGER statementTextLen, TraceOptions opts = {});
void TraceFunctionExit_SQLExecDirect(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLExecDirectW(
    SQLHSTMT statementHandle, SQLWCHAR *statementText,
    SQLINTEGER statementTextLen, TraceOptions opts = {});
void TraceFunctionExit_SQLExecDirectW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLNativeSql(
    SQLHDBC connectionHandle, SQLCHAR *inStatementText,
    SQLINTEGER inStatementTextLen,
    SQLCHAR *outStatementText, SQLINTEGER outStatementTextBufferLen,
    SQLINTEGER *outStatementTextLen, TraceOptions opts = {});
void TraceFunctionExit_SQLNativeSql(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLNativeSqlW(
    SQLHDBC connectionHandle, SQLWCHAR *inStatementText,
    SQLINTEGER inStatementTextLen,
    SQLWCHAR *outStatementText, SQLINTEGER outStatementTextBufferLen,
    SQLINTEGER *outStatementTextLen,TraceOptions opts = {});
void TraceFunctionExit_SQLNativeSqlW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLNumParams(
    SQLHSTMT statementHandle, SQLSMALLINT *paramCount,
    TraceOptions opts = {});
void TraceFunctionExit_SQLNumParams(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLParamData(
    SQLHSTMT statementHandle, SQLPOINTER *paramOrTargetValue,
    TraceOptions opts = {});
void TraceFunctionExit_SQLParamData(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLPutData(
    SQLHSTMT statementHandle, SQLPOINTER paramData, SQLLEN paramDataLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLPutData(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLDescribeParam(
    SQLHSTMT statementHandle, SQLUSMALLINT paramNumber,
    SQLSMALLINT *paramSqlType, 
    SQLULEN *paramSize, SQLSMALLINT *paramScale, SQLSMALLINT *paramNullable,
    TraceOptions opts = {});
void TraceFunctionExit_SQLDescribeParam(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetData(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLSMALLINT targetCType, SQLPOINTER targetValue,
    SQLLEN targetValueBufferLen, SQLLEN *targetValueStringLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetData(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLNumResultCols(
    SQLHSTMT statementHandle, SQLSMALLINT *columnCount,
    TraceOptions opts = {});
void TraceFunctionExit_SQLNumResultCols(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLFetch(
    SQLHSTMT statementHandle, TraceOptions opts = {});
void TraceFunctionExit_SQLFetch(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLExtendedFetch(
    SQLHSTMT statementHandletmt, SQLUSMALLINT fetchOrientation,
    SQLLEN fetchOffset,
    SQLULEN *rowCount, SQLUSMALLINT *rowStatusArray,
    TraceOptions opts = {});
void TraceFunctionExit_SQLExtendedFetch(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLColAttribute(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen, SQLLEN *numericAttribute,
    TraceOptions opts = {});
void TraceFunctionExit_SQLColAttribute(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLColAttributeW(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen, SQLLEN *numericAttribute,
    TraceOptions opts = {});
void TraceFunctionExit_SQLColAttributeW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLColAttributes(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLUSMALLINT fieldIdentifier, SQLPOINTER characterAttribute,
    SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen, SQLLEN *numericAttribute,
    TraceOptions opts = {});
void TraceFunctionExit_SQLColAttributes(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLColAttributesW(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen, SQLLEN *numericAttribute,
    TraceOptions opts = {});
void TraceFunctionExit_SQLColAttributesW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLDescribeCol(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber, SQLCHAR *columnName,
    SQLSMALLINT columnNameBufferLen, SQLSMALLINT *columnNameLen,
    SQLSMALLINT *columnSQLdataType, SQLULEN *columnSize,
    SQLSMALLINT *decimalDigits, SQLSMALLINT *columnNullable,
    TraceOptions opts = {});
void TraceFunctionExit_SQLDescribeCol(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLDescribeColW(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLWCHAR *columnName, SQLSMALLINT columnNameBufferLen,
    SQLSMALLINT *columnNameLen, SQLSMALLINT *columnSQLdataType,
    SQLULEN *columnSize, SQLSMALLINT *decimalDigits,
    SQLSMALLINT *columnNullable, TraceOptions opts = {});
void TraceFunctionExit_SQLDescribeColW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLBindCol(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLSMALLINT targetCType, SQLPOINTER targetValue,
    SQLLEN targetValueBufferLen, SQLLEN *targetValueStrLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLBindCol(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLRowCount(
    SQLHSTMT statementHandle, SQLLEN *rowCount,
    TraceOptions opts = {});
void TraceFunctionExit_SQLRowCount(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLFetchScroll(
    SQLHSTMT statementHandle, SQLSMALLINT fetchOrientation,
    SQLLEN fetchOffset,
    TraceOptions opts = {});
void TraceFunctionExit_SQLFetchScroll(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLMoreResults(
    SQLHSTMT statementHandle, TraceOptions opts = {});
void TraceFunctionExit_SQLMoreResults(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetDiagField(
    SQLSMALLINT handleType, SQLHANDLE handle,
    SQLSMALLINT recNumber, SQLSMALLINT diagIdentifier,
    SQLPOINTER diagInfo, SQLSMALLINT diagInfoBufferLen,
    SQLSMALLINT *diagInfoStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLGetDiagField(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetDiagFieldW(
    SQLSMALLINT handleType, SQLHANDLE handle,
    SQLSMALLINT recNumber, SQLSMALLINT diagIdentifier,
    SQLPOINTER diagInfo, SQLSMALLINT diagInfoBufferLen,
    SQLSMALLINT *diagInfoStringLen, TraceOptions opts = {});
void TraceFunctionExit_SQLGetDiagFieldW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetDiagRec(
    SQLSMALLINT handleType, SQLHANDLE handle, SQLSMALLINT recNumber,
    SQLCHAR *sqlState, SQLINTEGER *nativeError, SQLCHAR *messageText,
    SQLSMALLINT messageTextBufferLen, SQLSMALLINT *messageTextLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetDiagRec(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLGetDiagRecW(
    SQLSMALLINT handleType, SQLHANDLE handle, SQLSMALLINT recNumber,
    SQLWCHAR *sqlState, SQLINTEGER *nativeError, SQLWCHAR *messageText,
    SQLSMALLINT messageTextBufferLen, SQLSMALLINT *messageTextLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLGetDiagRecW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLColumns(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName, 
    SQLSMALLINT tableNameLen, SQLCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLColumns(SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLColumnsW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLWCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLColumnsW(SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLTables(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLCHAR *tableType,
    SQLSMALLINT tableTypeLen, TraceOptions opts = {});
void TraceFunctionExit_SQLTables(SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLTablesW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLWCHAR *tableType, SQLSMALLINT tableTypeLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLTablesW(SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLPrimaryKeys(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLPrimaryKeys(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLPrimaryKeysW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLPrimaryKeysW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLProcedureColumns(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *procName,
    SQLSMALLINT procNameLen, SQLCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLProcedureColumns(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLProcedureColumnsW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *procName,
    SQLSMALLINT procNameLen, SQLWCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLProcedureColumnsW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLProcedures(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *procName,
    SQLSMALLINT procNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLProcedures(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLProceduresW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *procName, SQLSMALLINT procNameLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLProceduresW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSpecialColumns(
    SQLHSTMT statementHandle, SQLUSMALLINT identifierType,
    SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName,
    SQLSMALLINT schemaNameLen, SQLCHAR *tableName, SQLSMALLINT tableNameLen,
    SQLUSMALLINT minRowIdScope, SQLUSMALLINT colNullable,
    TraceOptions opts = {});
void TraceFunctionExit_SQLSpecialColumns(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSpecialColumnsW(
    SQLHSTMT statementHandle, SQLUSMALLINT identifierType,
    SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen, SQLWCHAR *schemaName,
    SQLSMALLINT schemaNameLen, SQLWCHAR *tableName, SQLSMALLINT tableNameLen,
    SQLUSMALLINT minRowIdScope, SQLUSMALLINT colNullable,
    TraceOptions opts = {});
void TraceFunctionExit_SQLSpecialColumnsW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLStatistics(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLUSMALLINT indexType, SQLUSMALLINT reserved,
    TraceOptions opts = {});
void TraceFunctionExit_SQLStatistics(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLStatisticsW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLUSMALLINT indexType, SQLUSMALLINT reserved,
    TraceOptions opts = {});
void TraceFunctionExit_SQLStatisticsW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLTablePrivileges(
    SQLHSTMT statementHandle, SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLTablePrivileges(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLTablePrivilegesW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLTablePrivilegesW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLForeignKeys(
    SQLHSTMT statementHandle, SQLCHAR *pkCatalogName,
    SQLSMALLINT pkCatalogNameLen,
    SQLCHAR *pkSchemaName, SQLSMALLINT pkSchemaNameLen,
    SQLCHAR *pkTableName,
    SQLSMALLINT pkTableNameLen, SQLCHAR *fkCatalogName,
    SQLSMALLINT fkCatalogNameLen,
    SQLCHAR *fkSchemaName, SQLSMALLINT fkSchemaNameLen,
    SQLCHAR *fkTableName, SQLSMALLINT fkTableNameLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLForeignKeys(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLForeignKeysW(
    SQLHSTMT statementHandle, SQLWCHAR *pkCatalogName,
    SQLSMALLINT pkCatalogNameLen,
    SQLWCHAR *pkSchemaName, SQLSMALLINT pkSchemaNameLen,
    SQLWCHAR *pkTableName,
    SQLSMALLINT pkTableNameLen, SQLWCHAR *fkCatalogName,
    SQLSMALLINT fkCatalogNameLen,
    SQLWCHAR *fkSchemaName, SQLSMALLINT fkSchemaNameLen,
    SQLWCHAR *fkTableName, SQLSMALLINT fkTableNameLen,
    TraceOptions opts = {});
void TraceFunctionExit_SQLForeignKeysW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLColumnPrivileges(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLColumnPrivileges(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLColumnPrivilegesW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLWCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts = {});
void TraceFunctionExit_SQLColumnPrivilegesW(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLFreeStmt(
    SQLHSTMT statementHandle, SQLUSMALLINT option, TraceOptions opts = {});
void TraceFunctionExit_SQLFreeStmt(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLEndTran(
    SQLSMALLINT handleType, SQLHANDLE handle, SQLSMALLINT completionType,
    TraceOptions opts = {});
void TraceFunctionExit_SQLEndTran(SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLCancel(
    SQLHSTMT statementHandle, TraceOptions opts = {});
void TraceFunctionExit_SQLCancel(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLCloseCursor(
    SQLHSTMT statementHandle, TraceOptions opts = {});
void TraceFunctionExit_SQLCloseCursor(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLDisconnect(
    SQLHDBC connectionHandle, TraceOptions opts = {});
void TraceFunctionExit_SQLDisconnect(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLFreeHandle(
    SQLSMALLINT handleType, SQLHANDLE handle, TraceOptions opts = {});
void TraceFunctionExit_SQLFreeHandle(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLCancelHandle(
    SQLSMALLINT handleType, SQLHANDLE handle, TraceOptions opts = {});
void TraceFunctionExit_SQLCancelHandle(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLSetPos(
    SQLHSTMT statementHandle, SQLSETPOSIROW rowNumber,
    SQLUSMALLINT operation, SQLUSMALLINT lockType,
    TraceOptions opts = {});
void TraceFunctionExit_SQLSetPos(
    SQLRETURN retCode, TraceOptions opts = {});

void TraceFunctionEntry_SQLBulkOperations(
    SQLHSTMT statementHandle, SQLSMALLINT operation, TraceOptions opts = {});
void TraceFunctionExit_SQLBulkOperations(
    SQLRETURN retCode, TraceOptions opts = {});

}  // namespace odbc_bq_driver
}  // namespace cloud
}  // namespace google
// NOLINTEND(modernize-concat-nested-namespaces)

#endif  // GOOGLE_CLOUD_ODBC_BQ_DRIVER_ODBC_TRACE_H
