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

#include "odbc_trace.h"

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace google {
namespace cloud {
namespace odbc_bq_driver {

// Following functionality stull needs to still be implmented for the
// entry functions.
//
// 1) Different levels of logging.
// 2) Ensure no secret data gets removed 
//    from connection string if present. (e.g. secret, tokens etc)
// 3) Implement unicode functions.

void TraceFunctionEntry_SQLAllocHandle(
    SQLSMALLINT handleType, SQLHANDLE inputHandle, SQLHANDLE *outputHandle,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLAllocHandle_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(inputHandle)),
                ToCStr(FormatSqlHandle(outputHandle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLAllocHandle_Entry", 3,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(inputHandle)),
                ToCStr(FormatSqlHandle(outputHandle)));
        }
    }
}

void TraceFunctionExit_SQLAllocHandle(SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLAllocHandle_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLDriverConnect(
    SQLHDBC connectionHandle, SQLHWND windowHandle,
    SQLCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen,
    SQLSMALLINT *outConnectionStringLen, SQLUSMALLINT driverCompletion,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLDriverConnect_Entry", opts.trace_file, 9,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlHandle(windowHandle)),
                ToCStr(FormatSqlChar(inConnectionString)),
                ToCStr(FormatSqlSmallInt(inConnectionStringLen)),
                ToCStr(FormatSqlChar(outConnectionString)),
                ToCStr(FormatSqlSmallInt(outConnectionStringBufferLen)),
                ToCStr(FormatSqlSmallInt(outConnectionStringLen)),
                ToCStr(FormatSqlUSmallInt(driverCompletion)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLDriverConnect_Entry", 9,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlHandle(windowHandle)),
                ToCStr(FormatSqlChar(inConnectionString)),
                ToCStr(FormatSqlSmallInt(inConnectionStringLen)),
                ToCStr(FormatSqlChar(outConnectionString)),
                ToCStr(FormatSqlSmallInt(outConnectionStringBufferLen)),
                ToCStr(FormatSqlSmallInt(outConnectionStringLen)),
                ToCStr(FormatSqlUSmallInt(driverCompletion)));
        }
    }
}

void TraceFunctionSQLDriverConnect_Exit(SQLRETURN retCode, TraceOptions opts) 
{
    ExitInternal("SQLDriverConnect_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLDriverConnectW(
    SQLHDBC connectionHandle, SQLHWND windowHandle,
    SQLWCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLWCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen,
    SQLSMALLINT *outConnectionStringLen,
    SQLUSMALLINT driverCompletion, TraceOptions opts) {}

void TraceFunctionSQLDriverConnect_ExitW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLDriverConnectW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLBrowseConnect(
    SQLHDBC connectionHandle, SQLCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen,
    SQLSMALLINT *outConnectionStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLBrowseConnect_Entry", opts.trace_file, 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlChar(inConnectionString)),
                ToCStr(FormatSqlSmallInt(inConnectionStringLen)),
                ToCStr(FormatSqlChar(outConnectionString)),
                ToCStr(FormatSqlSmallInt(outConnectionStringBufferLen)),
                ToCStr(FormatSqlSmallInt(outConnectionStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLBrowseConnect_Entry", 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlChar(inConnectionString)),
                ToCStr(FormatSqlSmallInt(inConnectionStringLen)),
                ToCStr(FormatSqlChar(outConnectionString)),
                ToCStr(FormatSqlSmallInt(outConnectionStringBufferLen)),
                ToCStr(FormatSqlSmallInt(outConnectionStringLen)));
        }
    }
}

void TraceFunctionExit_SQLBrowseConnect(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLBrowseConnect_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLBrowseConnectW(
    SQLHDBC connectionHandle, SQLWCHAR *inConnectionString,
    SQLSMALLINT inConnectionStringLen, SQLWCHAR *outConnectionString,
    SQLSMALLINT outConnectionStringBufferLen,
    SQLSMALLINT *outConnectionStringLen, TraceOptions opts) {}

void TraceFunctionExit_SQLBrowseConnectW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLBrowseConnectW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLConnect(
    SQLHDBC connectionHandle, SQLCHAR *serverName, SQLSMALLINT serverNameLen,
    SQLCHAR *userName, SQLSMALLINT userNameLen,
    const SQLCHAR *authString, SQLSMALLINT authStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        // Not printing auth string.
        if (opts.trace_file.is_open())
        {
            if (authString && authStringLen > 0)
            {
                CollectAndPrintArgsFile(
                    "SQLConnect_Entry", opts.trace_file, 6,
                    ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                    ToCStr(FormatSqlHandle(connectionHandle)),
                    ToCStr(FormatSqlChar(serverName)),
                    ToCStr(FormatSqlSmallInt(serverNameLen)),
                    ToCStr(FormatSqlChar(userName)),
                    ToCStr(FormatSqlSmallInt(userNameLen)),
                    ToCStr(FormatString("****")),
                    ToCStr(FormatSqlSmallInt(authStringLen)));
            }
            else
            {
                CollectAndPrintArgsFile(
                    "SQLConnect_Entry", opts.trace_file, 6,
                    ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                    ToCStr(FormatSqlHandle(connectionHandle)),
                    ToCStr(FormatSqlChar(serverName)),
                    ToCStr(FormatSqlSmallInt(serverNameLen)),
                    ToCStr(FormatSqlChar(userName)),
                    ToCStr(FormatSqlSmallInt(userNameLen)));
            }
        }
        else
        {
            if (authString && authStringLen > 0)
            {
                CollectAndPrintArgs(
                    "SQLConnect_Entry", 6,
                    ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                    ToCStr(FormatSqlHandle(connectionHandle)),
                    ToCStr(FormatSqlChar(serverName)),
                    ToCStr(FormatSqlSmallInt(serverNameLen)),
                    ToCStr(FormatSqlChar(userName)),
                    ToCStr(FormatSqlSmallInt(userNameLen)),
                    ToCStr(FormatString("****")),
                    ToCStr(FormatSqlSmallInt(authStringLen)));
            }
            else
            {
                CollectAndPrintArgs(
                    "SQLConnect_Entry", 6,
                    ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                    ToCStr(FormatSqlHandle(connectionHandle)),
                    ToCStr(FormatSqlChar(serverName)),
                    ToCStr(FormatSqlSmallInt(serverNameLen)),
                    ToCStr(FormatSqlChar(userName)),
                    ToCStr(FormatSqlSmallInt(userNameLen)));
            }
        }
    }
}

void TraceFunctionExit_SQLConnect(SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLConnect_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLConnectW(
    SQLHDBC connectionHandle, SQLWCHAR *serverName,
    SQLSMALLINT serverNameLen, SQLWCHAR *userName, SQLSMALLINT userNameLen,
    const SQLWCHAR *authString, SQLSMALLINT authStringLen, TraceOptions opts) {}

void TraceFunctionExit_SQLConnectW(SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLConnectW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetInfo(
    SQLHDBC connectionHandle, SQLUSMALLINT infoType, SQLPOINTER infoValue,
    SQLSMALLINT infoValueBufferLen,
    SQLSMALLINT *infoValueStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetInfo_Entry", opts.trace_file, 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlUSmallInt(infoType)),
                ToCStr(FormatSqlPointer(infoValue)),
                ToCStr(FormatSqlSmallInt(infoValueBufferLen)),
                ToCStr(FormatSqlSmallInt(infoValueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetInfo_Entry", 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlUSmallInt(infoType)),
                ToCStr(FormatSqlPointer(infoValue)),
                ToCStr(FormatSqlSmallInt(infoValueBufferLen)),
                ToCStr(FormatSqlSmallInt(infoValueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLGetInfo(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetInfo_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetInfoW(
    SQLHDBC connectionHandle, SQLUSMALLINT infoType, SQLPOINTER infoValue,
    SQLSMALLINT infoValueBufferLen,
    SQLSMALLINT *infoValueStringLen, TraceOptions opts) {}

void TraceFunctionExit_SQLGetInfoW(SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetInfoW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetFunctions(
    SQLHDBC connectionHandle, SQLUSMALLINT functionId,
    SQLUSMALLINT *supportedFunction, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetFunctions_Entry", opts.trace_file, 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlUSmallInt(functionId)),
                ToCStr(FormatSqlUSmallInt(supportedFunction)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetFunctions_Entry", 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlUSmallInt(functionId)),
                ToCStr(FormatSqlUSmallInt(supportedFunction)));
        }
    }
}

void TraceFunctionExit_SQLGetFunctions(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetFunctions_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetTypeInfo(
    SQLHSTMT statementHandle,
    SQLSMALLINT dataType, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetTypeInfo_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSmallInt(dataType)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetTypeInfo_Entry", 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSmallInt(dataType)));
        }
    }
}

void TraceFunctionExit_SQLGetTypeInfo(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetTypeInfo_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetConnectAttr(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value, 
    SQLINTEGER valueStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLSetConnectAttr_Entry", opts.trace_file, 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLSetConnectAttr_Entry", 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLSetConnectAttr(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetConnectAttr_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetConnectAttrW(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value, 
    SQLINTEGER valueStringLen, TraceOptions opts) {}

void TraceFunctionExit_SQLSetConnectAttrW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetConnectAttrW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetConnectAttr(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen,
    SQLINTEGER *valueStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetConnectAttr_Entry", opts.trace_file, 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueBufferLen)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetConnectAttr_Entry", 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueBufferLen)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLGetConnectAttr(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetConnectAttr_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetConnectAttrW(
    SQLHDBC connectionHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen,
    TraceOptions opts)  {}

void TraceFunctionExit_SQLGetConnectAttrW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetConnectAttrW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetStmtAttr(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLSetStmtAttr_Entry", opts.trace_file, 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLSetStmtAttr_Entry", 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLSetStmtAttr(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetStmtAttr_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetStmtAttrW(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value, 
    SQLINTEGER valueStringLen, TraceOptions opts) {}

void TraceFunctionExit_SQLSetStmtAttrW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetStmtAttrW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetStmtAttr(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen,
    SQLINTEGER *valueStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetStmtAttr_Entry", opts.trace_file, 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueBufferLen)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetStmtAttr_Entry", 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueBufferLen)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLGetStmtAttr(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetStmtAttr_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetStmtAttrW(
    SQLHSTMT statementHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen,
    SQLINTEGER *valueStringLen, TraceOptions opts) {}

void TraceFunctionExit_SQLGetStmtAttrW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetStmtAttrW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetEnvAttr(
    SQLHENV environmentHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLSetEnvAttr_Entry", opts.trace_file, 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_ENV)),
                ToCStr(FormatSqlHandle(environmentHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLSetEnvAttr_Entry", 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_ENV)),
                ToCStr(FormatSqlHandle(environmentHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLSetEnvAttr(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetEnvAttr_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetEnvAttr(
    SQLHENV environmentHandle, SQLINTEGER attribute, SQLPOINTER value,
    SQLINTEGER valueBufferLen, SQLINTEGER *valueStringLen,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetEnvAttr_Entry", opts.trace_file, 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_ENV)),
                ToCStr(FormatSqlHandle(environmentHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueBufferLen)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetEnvAttr_Entry", 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_ENV)),
                ToCStr(FormatSqlHandle(environmentHandle)),
                ToCStr(FormatSqlInteger(attribute)),
                ToCStr(FormatSqlPointer(value)),
                ToCStr(FormatSqlInteger(valueBufferLen)),
                ToCStr(FormatSqlInteger(valueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLGetEnvAttr(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetEnvAttr_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetDescField(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber, SQLSMALLINT fieldId,
    SQLPOINTER outDescValue, SQLINTEGER outDescValueBufferLen,
    SQLINTEGER *outDescValueStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetDescField_Entry", opts.trace_file, 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(descriptorHandle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlSmallInt(fieldId)),
                ToCStr(FormatSqlPointer(outDescValue)),
                ToCStr(FormatSqlInteger(outDescValueBufferLen)),
                ToCStr(FormatSqlInteger(outDescValueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetDescField_Entry", 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(descriptorHandle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlSmallInt(fieldId)),
                ToCStr(FormatSqlPointer(outDescValue)),
                ToCStr(FormatSqlInteger(outDescValueBufferLen)),
                ToCStr(FormatSqlInteger(outDescValueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLGetDescField(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetDescField_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetDescFieldW(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber, SQLSMALLINT fieldId,
    SQLPOINTER outDescValue, SQLINTEGER outDescValueBufferLen,
    SQLINTEGER *outDescValueStringLen, TraceOptions opts) {}

void TraceFunctionExit_SQLGetDescFieldW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetDescFieldW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetDescRec(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLCHAR *name, SQLSMALLINT nameBufferLen, SQLSMALLINT *nameStringLen,
    SQLSMALLINT *descType, SQLSMALLINT *descSubType, SQLLEN *descOctetLen,
    SQLSMALLINT *descPrecision, SQLSMALLINT *descScale,
    SQLSMALLINT *nullable, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetDescRec_Entry", opts.trace_file, 12,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(descriptorHandle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlChar(name)),
                ToCStr(FormatSqlSmallInt(nameBufferLen)),
                ToCStr(FormatSqlSmallInt(nameStringLen)),
                ToCStr(FormatSqlSmallInt(descType)),
                ToCStr(FormatSqlSmallInt(descSubType)),
                ToCStr(FormatSqlLen(descOctetLen)),
                ToCStr(FormatSqlSmallInt(descPrecision)),
                ToCStr(FormatSqlSmallInt(descScale)),
                ToCStr(FormatSqlSmallInt(nullable)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetDescRec_Entry", 12,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(descriptorHandle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlChar(name)),
                ToCStr(FormatSqlSmallInt(nameBufferLen)),
                ToCStr(FormatSqlSmallInt(nameStringLen)),
                ToCStr(FormatSqlSmallInt(descType)),
                ToCStr(FormatSqlSmallInt(descSubType)),
                ToCStr(FormatSqlLen(descOctetLen)),
                ToCStr(FormatSqlSmallInt(descPrecision)),
                ToCStr(FormatSqlSmallInt(descScale)),
                ToCStr(FormatSqlSmallInt(nullable)));
        }
    }
}

void TraceFunctionExit_SQLGetDescRec(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetDescRec_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetDescRecW(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLWCHAR *name, SQLSMALLINT nameBufferLen, SQLSMALLINT *nameStringLen,
    SQLSMALLINT *descType, SQLSMALLINT *descSubType, SQLLEN *descOctetLen,
    SQLSMALLINT *descPrecision, SQLSMALLINT *descScale,
    SQLSMALLINT *nullable, TraceOptions opts) {}

void TraceFunctionExit_SQLGetDescRecW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetDescRecW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetDescField(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLSMALLINT fieldIdentifier,
    SQLPOINTER descValue, SQLINTEGER descValueBufferLen,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLSetDescField_Entry", opts.trace_file, 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(descriptorHandle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlSmallInt(fieldIdentifier)),
                ToCStr(FormatSqlPointer(descValue)),
                ToCStr(FormatSqlInteger(descValueBufferLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLSetDescField_Entry", 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(descriptorHandle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlSmallInt(fieldIdentifier)),
                ToCStr(FormatSqlPointer(descValue)),
                ToCStr(FormatSqlInteger(descValueBufferLen)));
        }
    }
}

void TraceFunctionExit_SQLSetDescField(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetDescField_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetDescFieldW(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLSMALLINT fieldIdentifier, SQLPOINTER descValue,
    SQLINTEGER descValueBufferLen, TraceOptions opts) {}

void TraceFunctionExit_SQLSetDescFieldW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetDescFieldW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetDescRec(
    SQLHDESC descriptorHandle, SQLSMALLINT recNumber,
    SQLSMALLINT descType, SQLSMALLINT descSubType,
    SQLLEN descOctetLen, SQLSMALLINT descPrecision, SQLSMALLINT descScale,
    SQLPOINTER descData, SQLLEN *descOctetLenPtr,
    SQLLEN *descIndicator, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLSetDescRec_Entry", opts.trace_file, 11,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(descriptorHandle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlSmallInt(descType)),
                ToCStr(FormatSqlSmallInt(descSubType)),
                ToCStr(FormatSqlLen(descOctetLen)),
                ToCStr(FormatSqlSmallInt(descPrecision)),
                ToCStr(FormatSqlSmallInt(descScale)),
                ToCStr(FormatSqlPointer(descData)),
                ToCStr(FormatSqlLen(descOctetLenPtr)),
                ToCStr(FormatSqlLen(descIndicator)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLSetDescRec_Entry", 11,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(descriptorHandle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlSmallInt(descType)),
                ToCStr(FormatSqlSmallInt(descSubType)),
                ToCStr(FormatSqlLen(descOctetLen)),
                ToCStr(FormatSqlSmallInt(descPrecision)),
                ToCStr(FormatSqlSmallInt(descScale)),
                ToCStr(FormatSqlPointer(descData)),
                ToCStr(FormatSqlLen(descOctetLenPtr)),
                ToCStr(FormatSqlLen(descIndicator)));
        }
    }
}

void TraceFunctionExit_SQLSetDescRec(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetDescRec_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLCopyDesc(
    SQLHDESC sourceDescHandle, SQLHDESC targetDescHandle,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLCopyDesc_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(sourceDescHandle)),
                ToCStr(FormatSqlHandle(targetDescHandle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLCopyDesc_Entry", 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DESC)),
                ToCStr(FormatSqlHandle(sourceDescHandle)),
                ToCStr(FormatSqlHandle(targetDescHandle)));
        }
    }
}

void TraceFunctionExit_SQLCopyDesc(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLCopyDesc_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLPrepare(
    SQLHSTMT statementHandle, SQLCHAR *statementText,
    SQLINTEGER statementTextLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLPrepare_Entry", opts.trace_file, 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(statementText)),
                ToCStr(FormatSqlInteger(statementTextLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLPrepare_Entry", 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(statementText)),
                ToCStr(FormatSqlInteger(statementTextLen)));
        }
    }
}

void TraceFunctionExit_SQLPrepare(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLPrepare_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLPrepareW(
    SQLHSTMT statementHandle, SQLWCHAR *statementText,
    SQLINTEGER statementTextLen, TraceOptions opts) {}

void TraceFunctionExit_SQLPrepareW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLPrepareW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLBindParameter(
    SQLHSTMT statementHandle, SQLUSMALLINT paramNumber,
    SQLSMALLINT paramType, SQLSMALLINT paramCType,
    SQLSMALLINT paramSqlType,
    SQLULEN paramColSize, SQLSMALLINT paramScale,
    SQLPOINTER paramDataValue, SQLLEN paramDataValueBufferLen,
    SQLLEN *paramDataValueStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLBindParameter_Entry", opts.trace_file, 11,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(paramNumber)),
                ToCStr(FormatSqlSmallInt(paramType)),
                ToCStr(FormatSqlSmallInt(paramCType)),
                ToCStr(FormatSqlSmallInt(paramSqlType)),
                ToCStr(FormatSqlULen(paramColSize)),
                ToCStr(FormatSqlSmallInt(paramScale)),
                ToCStr(FormatSqlPointer(paramDataValue)),
                ToCStr(FormatSqlLen(paramDataValueBufferLen)),
                ToCStr(FormatSqlLen(paramDataValueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLBindParameter_Entry", 11,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(paramNumber)),
                ToCStr(FormatSqlSmallInt(paramType)),
                ToCStr(FormatSqlSmallInt(paramCType)),
                ToCStr(FormatSqlSmallInt(paramSqlType)),
                ToCStr(FormatSqlULen(paramColSize)),
                ToCStr(FormatSqlSmallInt(paramScale)),
                ToCStr(FormatSqlPointer(paramDataValue)),
                ToCStr(FormatSqlLen(paramDataValueBufferLen)),
                ToCStr(FormatSqlLen(paramDataValueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLBindParameter(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLBindParameter_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetCursorName(
    SQLHSTMT statementHandle,
    SQLCHAR *cursorName, SQLSMALLINT cursorNameBufferLen,
    SQLSMALLINT *cursorNameStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetCursorName_Entry", opts.trace_file, 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(cursorName)),
                ToCStr(FormatSqlSmallInt(cursorNameBufferLen)),
                ToCStr(FormatSqlSmallInt(cursorNameStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetCursorName_Entry", 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(cursorName)),
                ToCStr(FormatSqlSmallInt(cursorNameBufferLen)),
                ToCStr(FormatSqlSmallInt(cursorNameStringLen)));
        }
    }
}

void TraceFunctionExit_SQLGetCursorName(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetCursorName_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetCursorNameW(
    SQLHSTMT statementHandle,
    SQLWCHAR *cursorName, SQLSMALLINT cursorNameBufferLen,
    SQLSMALLINT *cursorNameStringLen, TraceOptions opts) {}

void TraceFunctionExit_SQLGetCursorNameW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetCursorNameW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetCursorName(
    SQLHSTMT statementHandle, SQLCHAR *cursorName,
    SQLSMALLINT cursorNameLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLSetCursorName_Entry", opts.trace_file, 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(cursorName)),
                ToCStr(FormatSqlSmallInt(cursorNameLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLSetCursorName_Entry", 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(cursorName)),
                ToCStr(FormatSqlSmallInt(cursorNameLen)));
        }
    }
}

void TraceFunctionExit_SQLSetCursorName(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetCursorName_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetCursorNameW(
    SQLHSTMT statementHandle, SQLWCHAR *cursorName,
    SQLSMALLINT cursorNameLen, TraceOptions opts) {}

void TraceFunctionExit_SQLSetCursorNameW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetCursorNameW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLExecute(
    SQLHSTMT statementHandle, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLExecute_Entry", opts.trace_file, 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLExecute_Entry", 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
    }
}

void TraceFunctionExit_SQLExecute(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLExecute_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLExecDirect(
    SQLHSTMT statementHandle, SQLCHAR *statementText,
    SQLINTEGER statementTextLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLExecDirect_Entry", opts.trace_file, 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(statementText)),
                ToCStr(FormatSqlInteger(statementTextLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLExecDirect_Entry", 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(statementText)),
                ToCStr(FormatSqlInteger(statementTextLen)));
        }
    }
}

void TraceFunctionExit_SQLExecDirect(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLExecDirect_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLExecDirectW(
    SQLHSTMT statementHandle, SQLWCHAR *statementText,
    SQLINTEGER statementTextLen, TraceOptions opts) {}

void TraceFunctionExit_SQLExecDirectW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLExecDirectW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLNativeSql(
    SQLHDBC connectionHandle, SQLCHAR *inStatementText,
    SQLINTEGER inStatementTextLen,
    SQLCHAR *outStatementText, SQLINTEGER outStatementTextBufferLen,
    SQLINTEGER *outStatementTextLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLNativeSql_Entry", opts.trace_file, 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlChar(inStatementText)),
                ToCStr(FormatSqlInteger(inStatementTextLen)),
                ToCStr(FormatSqlChar(outStatementText)),
                ToCStr(FormatSqlInteger(outStatementTextBufferLen)),
                ToCStr(FormatSqlInteger(outStatementTextLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLNativeSql_Entry", 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)),
                ToCStr(FormatSqlChar(inStatementText)),
                ToCStr(FormatSqlInteger(inStatementTextLen)),
                ToCStr(FormatSqlChar(outStatementText)),
                ToCStr(FormatSqlInteger(outStatementTextBufferLen)),
                ToCStr(FormatSqlInteger(outStatementTextLen)));
        }
    }
}

void TraceFunctionExit_SQLNativeSql(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLNativeSql_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLNativeSqlW(
    SQLHDBC connectionHandle, SQLWCHAR *inStatementText,
    SQLINTEGER inStatementTextLen,
    SQLWCHAR *outStatementText, SQLINTEGER outStatementTextBufferLen,
    SQLINTEGER *outStatementTextLen, TraceOptions opts) {}

void TraceFunctionExit_SQLNativeSqlW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLNativeSqlW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLNumParams(
    SQLHSTMT statementHandle, SQLSMALLINT *paramCount,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLNumParams_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSmallInt(paramCount)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLNumParams_Entry", 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSmallInt(paramCount)));
        }
    }
}

void TraceFunctionExit_SQLNumParams(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLNumParams_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLParamData(
    SQLHSTMT statementHandle, SQLPOINTER *paramOrTargetValue,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLParamData_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlPointer(paramOrTargetValue)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLParamData_Entry", 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlPointer(paramOrTargetValue)));
        }
    }
}

void TraceFunctionExit_SQLParamData(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLParamData_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLPutData(
    SQLHSTMT statementHandle, SQLPOINTER paramData,
    SQLLEN paramDataLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLPutData_Entry", opts.trace_file, 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlPointer(paramData)),
                ToCStr(FormatSqlLen(paramDataLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLPutData_Entry", 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlPointer(paramData)),
                ToCStr(FormatSqlLen(paramDataLen)));
        }
    }
}

void TraceFunctionExit_SQLPutData(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLPutData_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLDescribeParam(
    SQLHSTMT statementHandle, SQLUSMALLINT paramNumber,
    SQLSMALLINT *paramSqlType,
    SQLULEN *paramSize, SQLSMALLINT *paramScale,
    SQLSMALLINT *paramNullable, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLDescribeParam_Entry", opts.trace_file, 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(paramNumber)),
                ToCStr(FormatSqlSmallInt(paramSqlType)),
                ToCStr(FormatSqlULen(paramSize)),
                ToCStr(FormatSqlSmallInt(paramScale)),
                ToCStr(FormatSqlSmallInt(paramNullable)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLDescribeParam_Entry", 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(paramNumber)),
                ToCStr(FormatSqlSmallInt(paramSqlType)),
                ToCStr(FormatSqlULen(paramSize)),
                ToCStr(FormatSqlSmallInt(paramScale)),
                ToCStr(FormatSqlSmallInt(paramNullable)));
        }
    }
}

void TraceFunctionExit_SQLDescribeParam(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLDescribeParam_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetData(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLSMALLINT targetCType, SQLPOINTER targetValue,
    SQLLEN targetValueBufferLen,
    SQLLEN *targetValueStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetData_Entry", opts.trace_file, 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlSmallInt(targetCType)),
                ToCStr(FormatSqlPointer(targetValue)),
                ToCStr(FormatSqlLen(targetValueBufferLen)),
                ToCStr(FormatSqlLen(targetValueStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetData_Entry", 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlSmallInt(targetCType)),
                ToCStr(FormatSqlPointer(targetValue)),
                ToCStr(FormatSqlLen(targetValueBufferLen)),
                ToCStr(FormatSqlLen(targetValueStringLen)));
        }
    }
}

void TraceFunctionExit_SQLGetData(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetData_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLNumResultCols(
    SQLHSTMT statementHandle, SQLSMALLINT *columnCount,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLNumResultCols_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSmallInt(columnCount)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLNumResultCols_Entry", 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSmallInt(columnCount)));
        }
    }
}

void TraceFunctionExit_SQLNumResultCols(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLNumResultCols_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLFetch(
    SQLHSTMT statementHandle, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLFetch_Entry", opts.trace_file, 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLFetch_Entry", 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
    }
}

void TraceFunctionExit_SQLFetch(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLFetch_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLExtendedFetch(
    SQLHSTMT statementHandle,
    SQLUSMALLINT fetchOrientation, SQLLEN fetchOffset,
    SQLULEN *rowCount, SQLUSMALLINT *rowStatusArray,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLExtendedFetch_Entry", opts.trace_file, 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(fetchOrientation)),
                ToCStr(FormatSqlLen(fetchOffset)),
                ToCStr(FormatSqlULen(rowCount)),
                ToCStr(FormatSqlUSmallInt(rowStatusArray)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLExtendedFetch_Entry", 6,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(fetchOrientation)),
                ToCStr(FormatSqlLen(fetchOffset)),
                ToCStr(FormatSqlULen(rowCount)),
                ToCStr(FormatSqlUSmallInt(rowStatusArray)));
        }
    }
}

void TraceFunctionExit_SQLExtendedFetch(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLExtendedFetch_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLColAttribute(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen,
    SQLLEN *numericAttribute, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLColAttribute_Entry", opts.trace_file, 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlUSmallInt(fieldIdentifier)),
                ToCStr(FormatSqlPointer(characterAttribute)),
                ToCStr(FormatSqlSmallInt(characterAttributeBufferLen)),
                ToCStr(FormatSqlSmallInt(characterAttributeStringLen)),
                ToCStr(FormatSqlLen(numericAttribute)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLColAttribute_Entry", 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlUSmallInt(fieldIdentifier)),
                ToCStr(FormatSqlPointer(characterAttribute)),
                ToCStr(FormatSqlSmallInt(characterAttributeBufferLen)),
                ToCStr(FormatSqlSmallInt(characterAttributeStringLen)),
                ToCStr(FormatSqlLen(numericAttribute)));
        }
    }
}

void TraceFunctionExit_SQLColAttribute(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLColAttribute_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLColAttributeW(
    SQLHSTMT statementHandle,
    SQLUSMALLINT columnNumber, SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen,
    SQLLEN *numericAttribute, TraceOptions opts) {}

void TraceFunctionExit_SQLColAttributeW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLColAttributeW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLColAttributes(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen,
    SQLLEN *numericAttribute, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLColAttributes_Entry", opts.trace_file, 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlUSmallInt(fieldIdentifier)),
                ToCStr(FormatSqlPointer(characterAttribute)),
                ToCStr(FormatSqlSmallInt(characterAttributeBufferLen)),
                ToCStr(FormatSqlSmallInt(characterAttributeStringLen)),
                ToCStr(FormatSqlLen(numericAttribute)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLColAttributes_Entry", 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlUSmallInt(fieldIdentifier)),
                ToCStr(FormatSqlPointer(characterAttribute)),
                ToCStr(FormatSqlSmallInt(characterAttributeBufferLen)),
                ToCStr(FormatSqlSmallInt(characterAttributeStringLen)),
                ToCStr(FormatSqlLen(numericAttribute)));
        }
    }
}

void TraceFunctionExit_SQLColAttributes(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLColAttributes_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLColAttributesW(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLUSMALLINT fieldIdentifier,
    SQLPOINTER characterAttribute, SQLSMALLINT characterAttributeBufferLen,
    SQLSMALLINT *characterAttributeStringLen, 
    SQLLEN *numericAttribute, TraceOptions opts) {}

void TraceFunctionExit_SQLColAttributesW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLColAttributesW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLDescribeCol(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber, SQLCHAR *columnName,
    SQLSMALLINT columnNameBufferLen, SQLSMALLINT *columnNameLen,
    SQLSMALLINT *columnSQLdataType, SQLULEN *columnSize,
    SQLSMALLINT *decimalDigits,
    SQLSMALLINT *columnNullable, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLDescribeCol_Entry", opts.trace_file, 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlChar(columnName)),
                ToCStr(FormatSqlSmallInt(columnNameBufferLen)),
                ToCStr(FormatSqlSmallInt(columnNameLen)),
                ToCStr(FormatSqlSmallInt(columnSQLdataType)),
                ToCStr(FormatSqlULen(columnSize)),
                ToCStr(FormatSqlSmallInt(decimalDigits)),
                ToCStr(FormatSqlSmallInt(columnNullable)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLDescribeCol_Entry", 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlChar(columnName)),
                ToCStr(FormatSqlSmallInt(columnNameBufferLen)),
                ToCStr(FormatSqlSmallInt(columnNameLen)),
                ToCStr(FormatSqlSmallInt(columnSQLdataType)),
                ToCStr(FormatSqlULen(columnSize)),
                ToCStr(FormatSqlSmallInt(decimalDigits)),
                ToCStr(FormatSqlSmallInt(columnNullable)));
        }
    }
}

void TraceFunctionExit_SQLDescribeCol(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLDescribeCol_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLDescribeColW(
    SQLHSTMT statementHandle,
    SQLUSMALLINT columnNumber, SQLWCHAR *columnName,
    SQLSMALLINT columnNameBufferLen, SQLSMALLINT *columnNameLen,
    SQLSMALLINT *columnSQLdataType, SQLULEN *columnSize,
    SQLSMALLINT *decimalDigits, SQLSMALLINT *columnNullable,
    TraceOptions opts) {}

void TraceFunctionExit_SQLDescribeColW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLDescribeColW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLBindCol(
    SQLHSTMT statementHandle, SQLUSMALLINT columnNumber,
    SQLSMALLINT targetCType,
    SQLPOINTER targetValue, SQLLEN targetValueBufferLen,
    SQLLEN *targetValueStrLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLBindCol_Entry", opts.trace_file, 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlSmallInt(targetCType)),
                ToCStr(FormatSqlPointer(targetValue)),
                ToCStr(FormatSqlLen(targetValueBufferLen)),
                ToCStr(FormatSqlLen(targetValueStrLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLBindCol_Entry", 7,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(columnNumber)),
                ToCStr(FormatSqlSmallInt(targetCType)),
                ToCStr(FormatSqlPointer(targetValue)),
                ToCStr(FormatSqlLen(targetValueBufferLen)),
                ToCStr(FormatSqlLen(targetValueStrLen)));
        }
    }
}

void TraceFunctionExit_SQLBindCol(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLBindCol_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLRowCount(
    SQLHSTMT statementHandle, SQLLEN *rowCount, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLRowCount_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlLen(rowCount)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLRowCount_Entry", 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlLen(rowCount)));
        }
    }
}

void TraceFunctionExit_SQLRowCount(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLRowCount_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLFetchScroll(
    SQLHSTMT statementHandle, SQLSMALLINT fetchOrientation,
    SQLLEN fetchOffset, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLFetchScroll_Entry", opts.trace_file, 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSmallInt(fetchOrientation)),
                ToCStr(FormatSqlLen(fetchOffset)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLFetchScroll_Entry", 4,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSmallInt(fetchOrientation)),
                ToCStr(FormatSqlLen(fetchOffset)));
        }
    }
}

void TraceFunctionExit_SQLFetchScroll(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLFetchScroll_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLMoreResults(
    SQLHSTMT statementHandle, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLMoreResults_Entry", opts.trace_file, 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLMoreResults_Entry", 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
    }
}

void TraceFunctionExit_SQLMoreResults(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLMoreResults_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetDiagField(
    SQLSMALLINT handleType, SQLHANDLE handle,
    SQLSMALLINT recNumber, SQLSMALLINT diagIdentifier,
    SQLPOINTER diagInfo, SQLSMALLINT diagInfoBufferLen,
    SQLSMALLINT *diagInfoStringLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetDiagField_Entry", opts.trace_file, 7,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlSmallInt(diagIdentifier)),
                ToCStr(FormatSqlPointer(diagInfo)),
                ToCStr(FormatSqlSmallInt(diagInfoBufferLen)),
                ToCStr(FormatSqlSmallInt(diagInfoStringLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetDiagField_Entry", 7,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlSmallInt(diagIdentifier)),
                ToCStr(FormatSqlPointer(diagInfo)),
                ToCStr(FormatSqlSmallInt(diagInfoBufferLen)),
                ToCStr(FormatSqlSmallInt(diagInfoStringLen)));
        }
    }
}

void TraceFunctionExit_SQLGetDiagField(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetDiagField_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetDiagFieldW(
    SQLSMALLINT handleType, SQLHANDLE handle,
    SQLSMALLINT recNumber, SQLSMALLINT diagIdentifier,
    SQLPOINTER diagInfo, SQLSMALLINT diagInfoBufferLen,
    SQLSMALLINT *diagInfoStringLen, TraceOptions opts) {}

void TraceFunctionExit_SQLGetDiagFieldW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetDiagFieldW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetDiagRec(
    SQLSMALLINT handleType, SQLHANDLE handle,
    SQLSMALLINT recNumber,
    SQLCHAR *sqlState, SQLINTEGER *nativeError,
    SQLCHAR *messageText, SQLSMALLINT messageTextBufferLen,
    SQLSMALLINT *messageTextLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLGetDiagRec_Entry", opts.trace_file, 8,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlChar(sqlState)),
                ToCStr(FormatSqlInteger(nativeError)),
                ToCStr(FormatSqlChar(messageText)),
                ToCStr(FormatSqlSmallInt(messageTextBufferLen)),
                ToCStr(FormatSqlSmallInt(messageTextLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLGetDiagRec_Entry", 8,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)),
                ToCStr(FormatSqlSmallInt(recNumber)),
                ToCStr(FormatSqlChar(sqlState)),
                ToCStr(FormatSqlInteger(nativeError)),
                ToCStr(FormatSqlChar(messageText)),
                ToCStr(FormatSqlSmallInt(messageTextBufferLen)),
                ToCStr(FormatSqlSmallInt(messageTextLen)));
        }
    }
}

void TraceFunctionExit_SQLGetDiagRec(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetDiagRec_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLGetDiagRecW(
    SQLSMALLINT handleType, SQLHANDLE handle,
    SQLSMALLINT recNumber,
    SQLWCHAR *sqlState, SQLINTEGER *nativeError,
    SQLWCHAR *messageText, SQLSMALLINT messageTextBufferLen,
    SQLSMALLINT *messageTextLen, TraceOptions opts) {}

void TraceFunctionExit_SQLGetDiagRecW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLGetDiagRecW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLColumns(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLColumns_Entry", opts.trace_file, 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlChar(columnName)),
                ToCStr(FormatSqlSmallInt(columnNameLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLColumns_Entry", 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlChar(columnName)),
                ToCStr(FormatSqlSmallInt(columnNameLen)));
        }
    }
}

void TraceFunctionExit_SQLColumns(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLColumns_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLColumnsW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLWCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts) {}

void TraceFunctionExit_SQLColumnsW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLColumnsW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLTables(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLCHAR *tableType,
    SQLSMALLINT tableTypeLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLTables_Entry", opts.trace_file, 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlChar(tableType)),
                ToCStr(FormatSqlSmallInt(tableTypeLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLTables_Entry", 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlChar(tableType)),
                ToCStr(FormatSqlSmallInt(tableTypeLen)));
        }
    }
}

void TraceFunctionExit_SQLTables(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLTables_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLTablesW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLWCHAR *tableType,
    SQLSMALLINT tableTypeLen, TraceOptions opts) {}

void TraceFunctionExit_SQLTablesW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLTablesW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLPrimaryKeys(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLPrimaryKeys_Entry", opts.trace_file, 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLPrimaryKeys_Entry", 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)));
        }
    }
}

void TraceFunctionExit_SQLPrimaryKeys(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLPrimaryKeys_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLPrimaryKeysW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen, SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, TraceOptions opts) {}

void TraceFunctionExit_SQLPrimaryKeysW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLPrimaryKeysW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLProcedureColumns(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *procName,
    SQLSMALLINT procNameLen, SQLCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLProcedureColumns_Entry", opts.trace_file, 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(procName)),
                ToCStr(FormatSqlSmallInt(procNameLen)),
                ToCStr(FormatSqlChar(columnName)),
                ToCStr(FormatSqlSmallInt(columnNameLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLProcedureColumns_Entry", 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(procName)),
                ToCStr(FormatSqlSmallInt(procNameLen)),
                ToCStr(FormatSqlChar(columnName)),
                ToCStr(FormatSqlSmallInt(columnNameLen)));
        }
    }
}

void TraceFunctionExit_SQLProcedureColumns(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLProcedureColumns_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLProcedureColumnsW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *procName,
    SQLSMALLINT procNameLen, SQLWCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts) {}

void TraceFunctionExit_SQLProcedureColumnsW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLProcedureColumnsW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLProcedures(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *procName,
    SQLSMALLINT procNameLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLProcedures_Entry", opts.trace_file, 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(procName)),
                ToCStr(FormatSqlSmallInt(procNameLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLProcedures_Entry", 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(procName)),
                ToCStr(FormatSqlSmallInt(procNameLen)));
        }
    }
}

void TraceFunctionExit_SQLProcedures(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLProcedures_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLProceduresW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *procName,
    SQLSMALLINT procNameLen, TraceOptions opts) {}

void TraceFunctionExit_SQLProceduresW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLProceduresW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSpecialColumns(
    SQLHSTMT statementHandle, SQLUSMALLINT identifierType,
    SQLCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName,
    SQLSMALLINT schemaNameLen, SQLCHAR *tableName,
    SQLSMALLINT tableNameLen,
    SQLUSMALLINT minRowIdScope, SQLUSMALLINT colNullable,
    TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLSpecialColumns_Entry", opts.trace_file, 11,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(identifierType)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlUSmallInt(minRowIdScope)),
                ToCStr(FormatSqlUSmallInt(colNullable)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLSpecialColumns_Entry", 11,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(identifierType)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlUSmallInt(minRowIdScope)),
                ToCStr(FormatSqlUSmallInt(colNullable)));
        }
    }
}

void TraceFunctionExit_SQLSpecialColumns(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSpecialColumns_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSpecialColumnsW(
    SQLHSTMT statementHandle, SQLUSMALLINT identifierType,
    SQLWCHAR *catalogName, SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName,
    SQLSMALLINT schemaNameLen, SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen,
    SQLUSMALLINT minRowIdScope, SQLUSMALLINT colNullable,
    TraceOptions opts) {}

void TraceFunctionExit_SQLSpecialColumnsW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSpecialColumnsW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLStatistics(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLUSMALLINT indexType,
    SQLUSMALLINT reserved, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLStatistics_Entry", opts.trace_file, 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlUSmallInt(indexType)),
                ToCStr(FormatSqlUSmallInt(reserved)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLStatistics_Entry", 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlUSmallInt(indexType)),
                ToCStr(FormatSqlUSmallInt(reserved)));
        }
    }
}

void TraceFunctionExit_SQLStatistics(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLStatistics_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLStatisticsW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLUSMALLINT indexType,
    SQLUSMALLINT reserved, TraceOptions opts) {}

void TraceFunctionExit_SQLStatisticsW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLStatisticsW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLTablePrivileges(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLTablePrivileges_Entry", opts.trace_file, 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLTablePrivileges_Entry", 8,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)));
        }
    }
}

void TraceFunctionExit_SQLTablePrivileges(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLTablePrivileges_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLTablePrivilegesW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, TraceOptions opts) {}

void TraceFunctionExit_SQLTablePrivilegesW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLTablePrivilegesW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLForeignKeys(
    SQLHSTMT statementHandle, SQLCHAR *pkCatalogName,
    SQLSMALLINT pkCatalogNameLen,
    SQLCHAR *pkSchemaName, SQLSMALLINT pkSchemaNameLen,
    SQLCHAR *pkTableName,
    SQLSMALLINT pkTableNameLen, SQLCHAR *fkCatalogName,
    SQLSMALLINT fkCatalogNameLen,
    SQLCHAR *fkSchemaName, SQLSMALLINT fkSchemaNameLen,
    SQLCHAR *fkTableName,
    SQLSMALLINT fkTableNameLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLForeignKeys_Entry", opts.trace_file, 14,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(pkCatalogName)),
                ToCStr(FormatSqlSmallInt(pkCatalogNameLen)),
                ToCStr(FormatSqlChar(pkSchemaName)),
                ToCStr(FormatSqlSmallInt(pkSchemaNameLen)),
                ToCStr(FormatSqlChar(pkTableName)),
                ToCStr(FormatSqlSmallInt(pkTableNameLen)),
                ToCStr(FormatSqlChar(fkCatalogName)),
                ToCStr(FormatSqlSmallInt(fkCatalogNameLen)),
                ToCStr(FormatSqlChar(fkSchemaName)),
                ToCStr(FormatSqlSmallInt(fkSchemaNameLen)),
                ToCStr(FormatSqlChar(fkTableName)),
                ToCStr(FormatSqlSmallInt(fkTableNameLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLForeignKeys_Entry", 14,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(pkCatalogName)),
                ToCStr(FormatSqlSmallInt(pkCatalogNameLen)),
                ToCStr(FormatSqlChar(pkSchemaName)),
                ToCStr(FormatSqlSmallInt(pkSchemaNameLen)),
                ToCStr(FormatSqlChar(pkTableName)),
                ToCStr(FormatSqlSmallInt(pkTableNameLen)),
                ToCStr(FormatSqlChar(fkCatalogName)),
                ToCStr(FormatSqlSmallInt(fkCatalogNameLen)),
                ToCStr(FormatSqlChar(fkSchemaName)),
                ToCStr(FormatSqlSmallInt(fkSchemaNameLen)),
                ToCStr(FormatSqlChar(fkTableName)),
                ToCStr(FormatSqlSmallInt(fkTableNameLen)));
        }
    }
}

void TraceFunctionExit_SQLForeignKeys(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLForeignKeys_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLForeignKeysW(
    SQLHSTMT statementHandle, SQLWCHAR *pkCatalogName,
    SQLSMALLINT pkCatalogNameLen,
    SQLWCHAR *pkSchemaName, SQLSMALLINT pkSchemaNameLen,
    SQLWCHAR *pkTableName,
    SQLSMALLINT pkTableNameLen, SQLWCHAR *fkCatalogName,
    SQLSMALLINT fkCatalogNameLen,
    SQLWCHAR *fkSchemaName, SQLSMALLINT fkSchemaNameLen,
    SQLWCHAR *fkTableName,
    SQLSMALLINT fkTableNameLen, TraceOptions opts) {}

void TraceFunctionExit_SQLForeignKeysW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLForeignKeysW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLColumnPrivileges(
    SQLHSTMT statementHandle, SQLCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLColumnPrivileges_Entry", opts.trace_file, 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlChar(columnName)),
                ToCStr(FormatSqlSmallInt(columnNameLen)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLColumnPrivileges_Entry", 10,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlChar(catalogName)),
                ToCStr(FormatSqlSmallInt(catalogNameLen)),
                ToCStr(FormatSqlChar(schemaName)),
                ToCStr(FormatSqlSmallInt(schemaNameLen)),
                ToCStr(FormatSqlChar(tableName)),
                ToCStr(FormatSqlSmallInt(tableNameLen)),
                ToCStr(FormatSqlChar(columnName)),
                ToCStr(FormatSqlSmallInt(columnNameLen)));
        }
    }
}

void TraceFunctionExit_SQLColumnPrivileges(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLColumnPrivileges_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLColumnPrivilegesW(
    SQLHSTMT statementHandle, SQLWCHAR *catalogName,
    SQLSMALLINT catalogNameLen,
    SQLWCHAR *schemaName, SQLSMALLINT schemaNameLen,
    SQLWCHAR *tableName,
    SQLSMALLINT tableNameLen, SQLWCHAR *columnName,
    SQLSMALLINT columnNameLen, TraceOptions opts) {}

void TraceFunctionExit_SQLColumnPrivilegesW(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLColumnPrivilegesW_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLFreeStmt(
    SQLHSTMT statementHandle, SQLUSMALLINT option, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLFreeStmt_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(option)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLFreeStmt_Entry", 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(option)));
        }
    }
}

void TraceFunctionExit_SQLFreeStmt(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLFreeStmt_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLEndTran(
    SQLSMALLINT handleType, SQLHANDLE handle,
    SQLSMALLINT completionType, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLEndTran_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)),
                ToCStr(FormatSqlSmallInt(completionType)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLEndTran_Entry", 3,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)),
                ToCStr(FormatSqlSmallInt(completionType)));
        }
    }
}

void TraceFunctionExit_SQLEndTran(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLEndTran_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLCancel(
    SQLHSTMT statementHandle, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLCancel_Entry", opts.trace_file, 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLCancel_Entry", 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
    }
}

void TraceFunctionExit_SQLCancel(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLCancel_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLCloseCursor(
    SQLHSTMT statementHandle, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLCloseCursor_Entry", opts.trace_file, 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLCloseCursor_Entry", 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)));
        }
    }
}

void TraceFunctionExit_SQLCloseCursor(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLCloseCursor_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLDisconnect(
    SQLHDBC connectionHandle, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLDisconnect_Entry", opts.trace_file, 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLDisconnect_Entry", 2,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_DBC)),
                ToCStr(FormatSqlHandle(connectionHandle)));
        }
    }
}

void TraceFunctionExit_SQLDisconnect(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLDisconnect_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLFreeHandle(
    SQLSMALLINT handleType, SQLHANDLE handle, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLFreeHandle_Entry", opts.trace_file, 2,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLFreeHandle_Entry", 2,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)));
        }
    }
}

void TraceFunctionExit_SQLFreeHandle(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLFreeHandle_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLCancelHandle(
    SQLSMALLINT handleType, SQLHANDLE handle, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLCancelHandle_Entry", opts.trace_file, 2,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLCancelHandle_Entry", 2,
                ToCStr(FormatSqlHandleType(handleType)),
                ToCStr(FormatSqlHandle(handle)));
        }
    }
}

void TraceFunctionExit_SQLCancelHandle(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLCancelHandle_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLSetPos(
    SQLHSTMT statementHandle, SQLSETPOSIROW rowNumber,
    SQLUSMALLINT operation, SQLUSMALLINT lockType, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLSetPos_Entry", opts.trace_file, 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSetPosiRow(rowNumber)),
                ToCStr(FormatSqlUSmallInt(operation)),
                ToCStr(FormatSqlUSmallInt(lockType)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLSetPos_Entry", 5,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlSetPosiRow(rowNumber)),
                ToCStr(FormatSqlUSmallInt(operation)),
                ToCStr(FormatSqlUSmallInt(lockType)));
        }
    }
}

void TraceFunctionExit_SQLSetPos(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLSetPos_Exit", retCode, opts);
}

void TraceFunctionEntry_SQLBulkOperations(
    SQLHSTMT statementHandle, SQLSMALLINT operation, TraceOptions opts)
{
    if (opts.logging_enabled)
    {
        if (opts.trace_file.is_open())
        {
            CollectAndPrintArgsFile(
                "SQLBulkOperations_Entry", opts.trace_file, 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(operation)));
        }
        else
        {
            CollectAndPrintArgs(
                "SQLBulkOperations_Entry", 3,
                ToCStr(FormatSqlHandleType(SQL_HANDLE_STMT)),
                ToCStr(FormatSqlHandle(statementHandle)),
                ToCStr(FormatSqlUSmallInt(operation)));
        }
    }
}

void TraceFunctionExit_SQLBulkOperations(
    SQLRETURN retCode, TraceOptions opts)
{
    ExitInternal("SQLBulkOperations_Exit", retCode, opts);
}

}  // namespace odbc_bq_driver
}  // namespace cloud
}  // namespace google
// NOLINTEND(modernize-concat-nested-namespaces)
