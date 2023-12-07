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

#include "google/cloud/odbc/bq_driver/internal/odbc_includes.h"

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
    SQLSMALLINT handleType, SQLHANDLE inputHandle, SQLHANDLE *outputHandle);
void TraceFunctionExit_SQLAllocHandle(SQLRETURN retCode);

void TraceFunctionEntry_SQLDriverConnect(
    SQLHDBC connectionHandle, SQLHWND windowHandle, SQLCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen, SQLSMALLINT *outConnectionStringLen,
    SQLUSMALLINT driverCompletiion);
void TraceFunctionExit_SQLDriverConnect(SQLRETURN retCode);

void TraceFunctionEntry_SQLDriverConnectW(
    SQLHDBC connectionHandle, SQLHWND windowHandle, SQLWCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLWCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen, SQLSMALLINT *outConnectionStringLen,
    SQLUSMALLINT driverCompletion);
void TraceFunctionExit_SQLDriverConnectW(SQLRETURN retCode);

void TraceFunctionEntry_SQLBrowseConnect(
    SQLHDBC connectionHandle, SQLCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen, SQLSMALLINT *outConnectionStringLen);
void TraceFunctionExit_SQLBrowseConnect(SQLRETURN retCode);

void TraceFunctionEntry_SQLBrowseConnectW(
    SQLHDBC connectionHandle, SQLWCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLWCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen, SQLSMALLINT *outConnectionStringLen);
void TraceFunctionExit_SQLBrowseConnectW(SQLRETURN retCode);

void TraceFunctionEntry_SQLConnect(
    SQLHDBC connectionHandle, SQLCHAR *serverName, SQLSMALLINT serverNameLen,
    SQLCHAR *userName, SQLSMALLINT userNameLen, SQLCHAR *authString, SQLSMALLINT authStringLen);
void TraceFunctionExit_SQLConnect(SQLRETURN retCode);

void TraceFunctionEntry_SQLConnectW(
    SQLHDBC connectionHandle, SQLWCHAR *serverName,
    SQLSMALLINT serverNameLen, SQLWCHAR *userName, SQLSMALLINT userNameLen, 
    SQLWCHAR *authString, SQLSMALLINT authStringLen);
void TraceFunctionExit_SQLConnectW(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetInfo(
    SQLHDBC connectionHandle, SQLUSMALLINT infoType, SQLPOINTER infoValue,
    SQLSMALLINT infoValueBufferLen, SQLSMALLINT *infoValueStringLen);
void TraceFunctionExit_SQLGetInfo(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetInfoW(
    SQLHDBC connectionHandle, SQLUSMALLINT infoType, SQLPOINTER infoValue,
    SQLSMALLINT infoValueBufferLen, SQLSMALLINT *infoValueStringLen);
void TraceFunctionExit_SQLGetInfoW(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetFunctions(
    SQLHDBC connectionHandle, SQLUSMALLINT functionId, SQLUSMALLINT *supportedFunction);
void TraceFunctionExit_SQLGetFunctions(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetTypeInfo(SQLHSTMT statementHandle, SQLSMALLINT dataType);
void TraceFunctionExit_SQLGetTypeInfo(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetConnectAttr(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value, 
    SQLINTEGER valueStringLen);
void TraceFunctionExit_SQLSetConnectAttr(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetConnectAttrW(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value, 
    SQLINTEGER valueStringLen);
void TraceFunctionExit_SQLSetConnectAttrW(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetConnectAttr(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen);
void TraceFunctionExit_SQLGetConnectAttr(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetConnectAttrW(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen);
void TraceFunctionExit_SQLGetConnectAttrW(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetStmtAttr(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueStringLen);
void TraceFunctionExit_SQLSetStmtAttr(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetStmtAttrW(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value, 
    SQLINTEGER valueStringLen);
void TraceFunctionExit_SQLSetStmtAttrW(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetStmtAttr(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen);
void TraceFunctionExit_SQLGetStmtAttr(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetStmtAttrW(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen);
void TraceFunctionExit_SQLGetStmtAttrW(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetEnvAttr(
    SQLHENV environmentHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueStringLen);
void TraceFunctionExit_SQLSetEnvAttr(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetEnvAttr(
    SQLHENV environmentHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen);
void TraceFunctionExit_SQLGetEnvAttr(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetDescField(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber, SQLSMALLINT fieldId,
    SQLPOINTER outDescValue, SQLINTEGER outDescValueBufferLen, SQLINTEGER *outDescValueStringLen);
void TraceFunctionExit_SQLGetDescField(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetDescFieldW(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber, SQLSMALLINT fieldId,
    SQLPOINTER outDescValue, SQLINTEGER outDescValueBufferLen, SQLINTEGER *outDescValueStringLen);
void TraceFunctionExit_SQLGetDescFieldW(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetDescRec(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLCHAR *name, SQLSMALLINT nameBufferLen, SQLSMALLINT *nameStringLen,
    SQLSMALLINT *descType, SQLSMALLINT *descSubType, SQLLEN *descOctetLen,
    SQLSMALLINT *descPrecision, SQLSMALLINT *descScale, SQLSMALLINT *nullable);
void TraceFunctionExit_SQLGetDescRec(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetDescRecW(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLWCHAR *name, SQLSMALLINT nameBufferLen, SQLSMALLINT *nameStringLen,
    SQLSMALLINT *descType, SQLSMALLINT *descSubType, SQLLEN *descOctetLen,
    SQLSMALLINT *descPrecision, SQLSMALLINT *descScale, SQLSMALLINT *nullable);
void TraceFunctionExit_SQLGetDescRecW(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetDescField(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber, SQLSMALLINT fieldIdentifier,
    SQLPOINTER descValue, SQLINTEGER descValueBufferLen);
void TraceFunctionExit_SQLSetDescField(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetDescFieldW(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLSMALLINT fieldIdentifier, SQLPOINTER descValue,
    SQLINTEGER descValueBufferLen);
void TraceFunctionExit_SQLSetDescFieldW(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetDescRec(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLSMALLINT descType, SQLSMALLINT descSubType,
    SQLLEN descOctetLen, SQLSMALLINT descPrecision, SQLSMALLINT descScale,
    SQLPOINTER descData, SQLLEN *descOctetLenPtr, SQLLEN *descIndicator);
void TraceFunctionExit_SQLSetDescRec(SQLRETURN retCode);

void TraceFunctionEntry_SQLCopyDesc(SQLHDESC sourceDescHandle, SQLHDESC targetDescHandle);
void TraceFunctionExit_SQLCopyDesc(SQLRETURN retCode);

void TraceFunctionEntry_SQLPrepare(
    SQLHSTMT statementHandle, SQLCHAR *statementText, SQLINTEGER statementTextLen);
void TraceFunctionExit_SQLPrepare(SQLRETURN retCode);

void TraceFunctionEntry_SQLPrepareW(
    SQLHSTMT statementHandle, SQLWCHAR *statementText, SQLINTEGER statementTextLen);
void TraceFunctionExit_SQLPrepareW(SQLRETURN retCode);

void TraceFunctionEntry_SQLBindParameter(
    SQLHSTMT statementHandle, SQLUSMALLINT paramNumber,
    SQLSMALLINT paramType, SQLSMALLINT paramCType, SQLSMALLINT paramSqlType,
    SQLULEN paramColSize, SQLSMALLINT paramScale,
    SQLPOINTER paramDataValue, SQLLEN paramDataValueBufferLen, SQLLEN *paramDataValueStringLen);
void TraceFunctionExit_SQLBindParameter(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetCursorName(
    SQLHSTMT statementHandle,
    SQLCHAR *cursorName, SQLSMALLINT cursorNameBufferLen, SQLSMALLINT *cursorNameStringLen);
void TraceFunctionExit_SQLGetCursorName(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetCursorNameW(
    SQLHSTMT statementHandle,
    SQLWCHAR *cursorName, SQLSMALLINT cursorNameBufferLen, SQLSMALLINT *cursorNameStringLen);
void TraceFunctionExit_SQLGetCursorNameW(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetCursorName(
    SQLHSTMT statementHandle, SQLCHAR *cursorName, SQLSMALLINT cursorNameLen);
void TraceFunctionExit_SQLSetCursorName(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetCursorNameW(
    SQLHSTMT statementHandle, SQLWCHAR *cursorName, SQLSMALLINT cursorNameLen);
void TraceFunctionExit_SQLSetCursorNameW(SQLRETURN retCode);

void TraceFunctionEntry_SQLExecute(SQLHSTMT statementHandle);
void TraceFunctionExit_SQLExecute(SQLRETURN retCode);

void TraceFunctionEntry_SQLExecDirect(
    SQLHSTMT statementHandle, SQLCHAR *statementText, SQLINTEGER statementTextLen);
void TraceFunctionExit_SQLExecDirect(SQLRETURN retCode);

void TraceFunctionEntry_SQLExecDirectW(
    SQLHSTMT statementHandle, SQLWCHAR *statementText, SQLINTEGER statementTextLen);
void TraceFunctionExit_SQLExecDirectW(SQLRETURN retCode);

void TraceFunctionEntry_SQLNativeSql(
    SQLHDBC connectionHandle, SQLCHAR *inStatementText, SQLINTEGER inStatementTextLen,
    SQLCHAR *outStatementText, SQLINTEGER outStatementTextBufferLen,
    SQLINTEGER *outStatementTextLen);
void TraceFunctionExit_SQLNativeSql(SQLRETURN retCode);

void TraceFunctionEntry_SQLNativeSqlW(
    SQLHDBC connectionHandle, SQLWCHAR *inStatementText, SQLINTEGER inStatementTextLen,
    SQLWCHAR *outStatementText, SQLINTEGER outStatementTextBufferLen,
    SQLINTEGER *outStatementTextLen);
void TraceFunctionExit_SQLNativeSqlW(SQLRETURN retCode);

void TraceFunctionEntry_SQLNumParams(SQLHSTMT statementHandle, SQLSMALLINT *paramCount);
void TraceFunctionExit_SQLNumParams(SQLRETURN retCode);

void TraceFunctionEntry_SQLParamData(SQLHSTMT statementHandle, SQLPOINTER *paramOrTargetValue);
void TraceFunctionExit_SQLParamData(SQLRETURN retCode);

void TraceFunctionEntry_SQLPutData(
    SQLHSTMT statementHandle, SQLPOINTER paramData, SQLLEN paramDataLen);
void TraceFunctionExit_SQLPutData(SQLRETURN retCode);

void TraceFunctionEntry_SQLDescribeParam(
    SQLHSTMT statementHandle, SQLUSMALLINT paramNumber, SQLSMALLINT *paramSqlType, 
    SQLULEN *paramSize, SQLSMALLINT *paramScale, SQLSMALLINT *paramNullable);
void TraceFunctionExit_SQLDescribeParam(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetData(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLSMALLINT targetCType, SQLPOINTER targetValue, SQLLEN targetValueBufferLen,
    SQLLEN *targetValueStringLen);
void TraceFunctionExit_SQLGetData(SQLRETURN retCode);

void TraceFunctionEntry_SQLNumResultCols(SQLHSTMT statementHandle, SQLSMALLINT *columnCount);
void TraceFunctionExit_SQLNumResultCols(SQLRETURN retCode);

void TraceFunctionEntry_SQLFetch(SQLHSTMT statementHandle);
void TraceFunctionExit_SQLFetch(SQLRETURN retCode);

void TraceFunctionEntry_SQLExtendedFetch(
    SQLHSTMT statementHandletmt, SQLUSMALLINT fetchOrientation, SQLLEN fetchOffset,
    SQLULEN *rowCount, SQLUSMALLINT *rowStatusArray);
void TraceFunctionExit_SQLExtendedFetch(SQLRETURN retCode);

void TraceFunctionEntry_SQLColAttribute(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber, SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen, SQLLEN *numericAttribute);
void TraceFunctionExit_SQLColAttribute(SQLRETURN retCode);

void TraceFunctionEntry_SQLColAttributeW(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber, SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen, SQLLEN *numericAttribute);
void TraceFunctionExit_SQLColAttributeW(SQLRETURN retCode);

void TraceFunctionEntry_SQLColAttributes(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber, SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen, SQLLEN *numericAttribute);
void TraceFunctionExit_SQLColAttributes(SQLRETURN retCode);

void TraceFunctionEntry_SQLColAttributesW(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber, SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen, SQLLEN *numericAttribute);
void TraceFunctionExit_SQLColAttributesW(SQLRETURN retCode);

void TraceFunctionEntry_SQLDescribeCol(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber, SQLCHAR *columnName,
    SQLSMALLINT columnNameBufferLen, SQLSMALLINT *columnNameLe,
    SQLSMALLINT *columnSQLdataType, SQLULEN *columnSize,
    SQLSMALLINT *decimalDigits, SQLSMALLINT *columnNullable);
void TraceFunctionExit_SQLDescribeCol(SQLRETURN retCode);

void TraceFunctionEntry_SQLDescribeColW(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber, SQLWCHAR *columnName,
    SQLSMALLINT columnNameBufferLen, SQLSMALLINT *columnNameLen,
    SQLSMALLINT *columnSQLdataType, SQLULEN *columnSize,
    SQLSMALLINT *decimalDigits, SQLSMALLINT *columnNullable);
void TraceFunctionExit_SQLDescribeColW(SQLRETURN retCode);

void TraceFunctionEntry_SQLBindCol(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber, SQLSMALLINT targetCType,
    SQLPOINTER targetValue, SQLLEN targetValueBufferLen, SQLLEN *targetValueStrLen);
void TraceFunctionExit_SQLBindCol(SQLRETURN retCode);

void TraceFunctionEntry_SQLRowCount(SQLHSTMT statementHandle, SQLLEN *rowCount);
void TraceFunctionExit_SQLRowCount(SQLRETURN retCode);

void TraceFunctionEntry_SQLFetchScroll(
    SQLHSTMT statementHandle, SQLSMALLINT fetchOrientation, SQLLEN fetchOffset);
void TraceFunctionExit_SQLFetchScroll(SQLRETURN retCode);

void TraceFunctionEntry_SQLMoreResults(SQLHSTMT statementHandle);
void TraceFunctionExit_SQLMoreResults(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetDiagField(
    SQLSMALLINT handleType, SQLHANDLE handle, SQLSMALLINT recNumber, SQLSMALLINT diagIdentifier,
    SQLPOINTER diagInfo, SQLSMALLINT diagInfoBufferLen, SQLSMALLINT *diagInfoStringLen);
void TraceFunctionExit_SQLGetDiagField(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetDiagFieldW(
    SQLSMALLINT handleType, SQLHANDLE handle, SQLSMALLINT recNumber, SQLSMALLINT diagIdentifier,
    SQLPOINTER diagInfo, SQLSMALLINT diagInfoBufferLen, SQLSMALLINT *diagInfoStringLen);
void TraceFunctionExit_SQLGetDiagFieldW(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetDiagRec(
    SQLSMALLINT handleType, SQLHANDLE handle, SQLSMALLINT recNumber,
    SQLCHAR *sqlState, SQLINTEGER *nativeError, SQLCHAR *messageText,
    SQLSMALLINT messageTextBufferLen, SQLSMALLINT *messageTextLen);
void TraceFunctionExit_SQLGetDiagRec(SQLRETURN retCode);

void TraceFunctionEntry_SQLGetDiagRecW(
    SQLSMALLINT handleType, SQLHANDLE handle, SQLSMALLINT recNumber,
    SQLWCHAR *sqlState, SQLINTEGER *nativeError, SQLWCHAR *messageText,
    SQLSMALLINT messageTextBufferLen, SQLSMALLINT *messageTextLen);
void TraceFunctionExit_SQLGetDiagRecW(SQLRETURN retCode);

void TraceFunctionEntry_SQLColumns(
    SQLHSTMT statementHandle, SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *tableName, 
    SQLSMALLINT tableNameLen, SQLCHAR *columnName, SQLSMALLINT columnNameLen);
void TraceFunctionExit_SQLColumns(SQLRETURN retCode);

void TraceFunctionEntry_SQLColumnsW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLWCHAR *columnName, SQLSMALLINT columnNameLen);
void TraceFunctionExit_SQLColumnsW(SQLRETURN retCode);

void TraceFunctionEntry_SQLTables(
    SQLHSTMT statementHandle, SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLCHAR *tableType, SQLSMALLINT tableTypeLen);
void TraceFunctionExit_SQLTables(SQLRETURN retCode);

void TraceFunctionEntry_SQLTablesW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLWCHAR *tableType, SQLSMALLINT tableTypeLen);
void TraceFunctionExit_SQLTablesW(SQLRETURN retCode);

void TraceFunctionEntry_SQLPrimaryKeys(
    SQLHSTMT statementHandle, SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *tableName,
    SQLSMALLINT tableNameLen);
void TraceFunctionExit_SQLPrimaryKeys(SQLRETURN retCode);

void TraceFunctionEntry_SQLPrimaryKeysW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen);
void TraceFunctionExit_SQLPrimaryKeysW(SQLRETURN retCode);

void TraceFunctionEntry_SQLProcedureColumns(
    SQLHSTMT statementHandle, SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *procName,
    SQLSMALLINT procNameLen, SQLCHAR *columnName, SQLSMALLINT columnNameLen);
void TraceFunctionExit_SQLProcedureColumns(SQLRETURN retCode);

void TraceFunctionEntry_SQLProcedureColumnsW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *procName,
    SQLSMALLINT procNameLen, SQLWCHAR *columnName, SQLSMALLINT columnNameLen);
void TraceFunctionExit_SQLProcedureColumnsW(SQLRETURN retCode);

void TraceFunctionEntry_SQLProcedures(
    SQLHSTMT statementHandle, SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *procName,
    SQLSMALLINT procNameLen);
void TraceFunctionExit_SQLProcedures(SQLRETURN retCode);

void TraceFunctionEntry_SQLProceduresW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *procName,
    SQLSMALLINT procNameLen);
void TraceFunctionExit_SQLProceduresW(SQLRETURN retCode);

void TraceFunctionEntry_SQLSpecialColumns(
    SQLHSTMT statementHandle, SQLUSMALLINT identifierType,
    SQLCHAR *catalogName, SQLSMALLINT catalogNameLen, SQLCHAR *schemaName,
    SQLSMALLINT schemaNameLen, SQLCHAR *tableName, SQLSMALLINT tableNameLen,
    SQLUSMALLINT minRowIdScope, SQLUSMALLINT colNullable);
void TraceFunctionExit_SQLSpecialColumns(SQLRETURN retCode);

void TraceFunctionEntry_SQLSpecialColumnsW(
    SQLHSTMT statementHandle, SQLUSMALLINT identifierType,
    SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen, SQLWCHAR *schemaName,
    SQLSMALLINT schemaNameLen, SQLWCHAR *tableName, SQLSMALLINT tableNameLen,
    SQLUSMALLINT minRowIdScope, SQLUSMALLINT colNullable);
void TraceFunctionExit_SQLSpecialColumnsW(SQLRETURN retCode);

void TraceFunctionEntry_SQLStatistics(
    SQLHSTMT statementHandle, SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLUSMALLINT indexType, SQLUSMALLINT reserved);
void TraceFunctionExit_SQLStatistics(SQLRETURN retCode);

void TraceFunctionEntry_SQLStatisticsW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLUSMALLINT indexType, SQLUSMALLINT reserved);
void TraceFunctionExit_SQLStatisticsW(SQLRETURN retCode);

void TraceFunctionEntry_SQLTablePrivileges(
    SQLHSTMT statementHandle, SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *tableName,
    SQLSMALLINT tableNameLen);
void TraceFunctionExit_SQLTablePrivileges(SQLRETURN retCode);

void TraceFunctionEntry_SQLTablePrivilegesW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen);
void TraceFunctionExit_SQLTablePrivilegesW(SQLRETURN retCode);

void TraceFunctionEntry_SQLForeignKeys(
    SQLHSTMT statementHandle, SQLCHAR *pkCatalogName, SQLSMALLINT pkCatalogNameLen,
    SQLCHAR *pkSchemaName, SQLSMALLINT pkSchemaNameLen, SQLCHAR *pkTableName,
    SQLSMALLINT pkTableNameLen, SQLCHAR *fkCatalogName, SQLSMALLINT fkCatalogNameLen,
    SQLCHAR *fkSchemaName, SQLSMALLINT fkSchemaNameLen, SQLCHAR *fkTableName,
    SQLSMALLINT fkTableNameLen);
void TraceFunctionExit_SQLForeignKeys(SQLRETURN retCode);

void TraceFunctionEntry_SQLForeignKeysW(
    SQLHSTMT statementHandle, SQLWCHAR *pkCatalogName, SQLSMALLINT pkCatalogNameLen,
    SQLWCHAR *pkSchemaName, SQLSMALLINT pkSchemaNameLen, SQLWCHAR *pkTableName,
    SQLSMALLINT pkTableNameLen, SQLWCHAR *fkCatalogName, SQLSMALLINT fkCatalogNameLen,
    SQLWCHAR *fkSchemaName, SQLSMALLINT fkSchemaNameLen, SQLWCHAR *fkTableName,
    SQLSMALLINT fkTableNameLen);
void TraceFunctionExit_SQLForeignKeysW(SQLRETURN retCode);

void TraceFunctionEntry_SQLColumnPrivileges(
    SQLHSTMT statementHandle, SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLCHAR *columnName, SQLSMALLINT columnNameLen);
void TraceFunctionExit_SQLColumnPrivileges(SQLRETURN retCode);

void TraceFunctionEntry_SQLColumnPrivilegesW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLWCHAR *columnName, SQLSMALLINT columnNameLen);
void TraceFunctionExit_SQLColumnPrivilegesW(SQLRETURN retCode);

void TraceFunctionEntry_SQLFreeStmt(SQLHSTMT statementHandle, SQLUSMALLINT option);
void TraceFunctionExit_SQLFreeStmt(SQLRETURN retCode);

void TraceFunctionEntry_SQLEndTran(SQLSMALLINT handleType, SQLHANDLE handle, SQLSMALLINT completionType);
void TraceFunctionExit_SQLEndTran(SQLRETURN retCode);

void TraceFunctionEntry_SQLCancel(SQLHSTMT statementHandle);
void TraceFunctionExit_SQLCancel(SQLRETURN retCode);

void TraceFunctionEntry_SQLCloseCursor(SQLHSTMT statementHandle);
void TraceFunctionExit_SQLCloseCursor(SQLRETURN retCode);

void TraceFunctionEntry_SQLDisconnect(SQLHDBC connectionHandle);
void TraceFunctionExit_SQLDisconnect(SQLRETURN retCode);

void TraceFunctionEntry_SQLFreeHandle(SQLSMALLINT handleType, SQLHANDLE handle);
void TraceFunctionExit_SQLFreeHandle(SQLRETURN retCode);

void TraceFunctionEntry_SQLCancelHandle(SQLSMALLINT handleType, SQLHANDLE handle);
void TraceFunctionExit_SQLCancelHandle(SQLRETURN retCode);

void TraceFunctionEntry_SQLSetPos(
    SQLHSTMT statementHandle, SQLSETPOSIROW rowNumber, SQLUSMALLINT operation,SQLUSMALLINT lockType);
void TraceFunctionExit_SQLSetPos(SQLRETURN retCode);

void TraceFunctionEntry_SQLBulkOperations(SQLHSTMT statementHandle, SQLSMALLINT operation);
void TraceFunctionExit_SQLBulkOperations(SQLRETURN retCode);

}  // namespace odbc_bq_driver
}  // namespace cloud
}  // namespace google
// NOLINTEND(modernize-concat-nested-namespaces)

#endif  // GOOGLE_CLOUD_ODBC_BQ_DRIVER_ODBC_TRACE_H
