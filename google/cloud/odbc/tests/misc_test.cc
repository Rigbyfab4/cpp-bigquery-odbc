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
#include "testing/misc.h"

namespace google {
namespace cloud {
namespace bigquery_odbc {

// Defines the idiomatic ODBC descriptors
// These fields can populated by a call to SQLGetDescRec
struct Descriptor {
  SQLSMALLINT string_len;
  SQLSMALLINT type;
  SQLSMALLINT sub_type;
  SQLLEN length;
  SQLSMALLINT precision;
  SQLSMALLINT scale;
  SQLSMALLINT nullable;
  SQLCHAR name[kBufferLength];
};

Schema kStdSchema = {
  { "Str2", SQL_VARCHAR },
  { "Int2", SQL_INTEGER },
  { "Float2", SQL_FLOAT },
};

void SetGetDescRec(std::shared_ptr<ConnectionHandle> conn, std::string table_name, Schema schema) {
  SQLSMALLINT desc_type;
  SQLHDESC ird_handle; // Implementation row descriptor
  SQLHDESC ipd_handle; // Implementation parameter descriptor
  int num_cols = schema.size();

  auto status = SQLGetStmtAttr(conn->hstmt, SQL_ATTR_IMP_ROW_DESC, &ird_handle, 0, NULL);
  CheckError(status, "SQLGetStmtAttr(SQL_ATTR_IMP_ROW_DESC)", conn);
  status = SQLGetStmtAttr(conn->hstmt, SQL_ATTR_IMP_PARAM_DESC, &ipd_handle, 0, NULL);
  CheckError(status, "SQLGetStmtAttr(SQL_ATTR_IMP_PARAM_DESC)", conn);

  status = SQLExecDirect(conn->hstmt, (SQLCHAR *)("SELECT * FROM "+ table_name).c_str(), SQL_NTS);
  CheckError(status, "SQLExecDirect", conn);

  Descriptor desc, desc_copy;

  for (int i = 0; i < num_cols; i++) {
    // Reads multiple descriptor fields for a column
    status = SQLGetDescRec(ird_handle, i + 1, desc.name, kBufferLength, &desc.string_len, &desc.type,
                &desc.sub_type, &desc.length, &desc.precision, &desc.scale, &desc.nullable);
    CheckError(status, "SQLGetDescRec", conn);
    std::string col_name = (char *)desc.name;
    EXPECT_EQ(col_name, schema[i].name);
    // We are checking if the bigquery data type corresponding to the returned
    //  sql data type correct.
    EXPECT_EQ(ToBqFieldType(desc.type), ToBqFieldType(schema[i].type));

    // Set the same values for another descriptor handle
    status = SQLSetDescRec(ipd_handle, i + 1, desc.type, desc.sub_type, desc.length, desc.precision,
                desc.scale, desc.name, (SQLLEN *)&kBufferLength, NULL);
    CheckError(status, "SQLSetDescRec", conn);
    status = SQLGetDescRec(ird_handle, i + 1, desc_copy.name, kBufferLength, &desc_copy.string_len, &desc_copy.type,
                &desc_copy.sub_type, &desc_copy.length, &desc_copy.precision, &desc_copy.scale, &desc_copy.nullable);
    CheckError(status, "SQLGetDescRec", conn);
    // Check if the values were set correctly by SQLSetDescRec
    EXPECT_EQ(desc_copy.string_len, desc.string_len);
    EXPECT_EQ(desc_copy.type, desc.type);
    EXPECT_EQ(desc_copy.sub_type, desc.sub_type);
    EXPECT_EQ(desc_copy.length, desc.length);
    EXPECT_EQ(desc_copy.precision, desc.precision);
    EXPECT_EQ(desc_copy.scale, desc.scale);
    EXPECT_EQ(desc_copy.nullable, desc.nullable);
  }
}

void CopyDescRec(std::shared_ptr<ConnectionHandle> conn, std::string table_name, Schema schema) {
  SQLSMALLINT desc_type;
  SQLHDESC ird_handle; // Implementation row descriptor
  SQLHDESC ipd_handle; // Implementation parameter descriptor
  int num_cols = schema.size();

  auto status = SQLGetStmtAttr(conn->hstmt, SQL_ATTR_IMP_ROW_DESC, &ird_handle, 0, NULL);
  CheckError(status, "SQLGetStmtAttr(SQL_ATTR_IMP_ROW_DESC)", conn);
  status = SQLGetStmtAttr(conn->hstmt, SQL_ATTR_IMP_PARAM_DESC, &ipd_handle, 0, NULL);
  CheckError(status, "SQLGetStmtAttr(SQL_ATTR_IMP_PARAM_DESC)", conn);

  status = SQLExecDirect(conn->hstmt, (SQLCHAR *)("SELECT * FROM "+ table_name).c_str(), SQL_NTS);
  CheckError(status, "SQLExecDirect", conn);

  Descriptor desc, desc_copy;

  for (int i = 0; i < num_cols; i++) {
    // Reads multiple descriptor fields for a column
    status = SQLGetDescRec(ird_handle, i + 1, desc.name, kBufferLength, &desc.string_len, &desc.type,
                &desc.sub_type, &desc.length, &desc.precision, &desc.scale, &desc.nullable);
    CheckError(status, "SQLGetDescRec", conn);
    std::string col_name = (char *)desc.name;
    EXPECT_EQ(col_name, schema[i].name);
    // We are checking if the bigquery data type corresponding to the returned
    //  sql data type correct.
    EXPECT_EQ(ToBqFieldType(desc.type), ToBqFieldType(schema[i].type));
  }

  status = SQLCopyDesc(ird_handle, ipd_handle);
  CheckError(status, "SQLCopyDesc", conn);

  // We use SQLGetDescField to read the descriptor fields one at a time,
  //  and check if they were copied correctly.
  for (int i = 0; i < num_cols; i++) {
    // Reads a single field from the column descriptor
    status = SQLGetDescField(ipd_handle, i + 1, SQL_DESC_NAME, &desc_copy.name,
                kBufferLength, NULL);
    CheckError(status, "SQLGetDescField(SQL_DESC_NAME)", conn);
    std::string col_name = (char *)desc_copy.name;
    EXPECT_EQ(col_name, schema[i].name);

    status = SQLGetDescField(ipd_handle, i + 1, SQL_DESC_TYPE, &desc_copy.type,
                SQL_IS_SMALLINT, NULL);
    CheckError(status, "SQLGetDescField(SQL_DESC_TYPE)", conn);
    EXPECT_EQ(ToBqFieldType(desc_copy.type), ToBqFieldType(schema[i].type));
  }
}

void CheckDataTypes(std::shared_ptr<ConnectionHandle> conn) {
  auto status = SQLGetTypeInfo(conn->hstmt, SQL_ALL_TYPES);
  CheckError(status, "SQLGetTypeInfo", conn);

  SQLCHAR type_name[kBufferLength];
  SQLSMALLINT sql_data_type;
  SQLINTEGER col_size;
  SQLLEN type_name_len = 0, data_type_len = 0, col_size_len = 0;

  status = SQLBindCol(conn->hstmt, 1, SQL_C_CHAR, (SQLPOINTER)type_name, (SQLLEN)sizeof(type_name), &type_name_len);
  CheckError(status, "SQLBindCol", conn);

  status = SQLBindCol(conn->hstmt, 2, SQL_C_SHORT, (SQLPOINTER)&sql_data_type, (SQLLEN)sizeof(sql_data_type), &data_type_len);
  CheckError(status, "SQLBindCol", conn);

  while (1) {
    status = SQLFetch(conn->hstmt);
    if(status == SQL_NO_DATA) {
      break;
    }
    CheckError(status, "SQLFetch", conn);

    std::string bq_data_type = (char *)type_name;
    EXPECT_EQ(kBqToSqlDataTypes.at(bq_data_type), sql_data_type);
  }
}

TEST(DriverAttributesTest, SQLGetEnvAttr) {
  auto conn = std::make_shared<ConnectionHandle>();
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  EXPECT_EQ(GetEnvInfo(conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(DescriptorFieldsTest, SQLSetDescRec) {
  auto const table_name = kDatasetName + ".ODBC_DESCRIPTORS_TEST";
  auto conn = std::make_shared<ConnectionHandle>();

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  CreateTable(conn, table_name, getSchemaStr(kStdSchema));
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  SetGetDescRec(conn, table_name, kStdSchema);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  DropTable(conn, table_name);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(DescriptorFieldsTest, SQLCopyDesc) {
  auto const table_name = kDatasetName + ".ODBC_DESCRIPTORS_TEST";
  auto conn = std::make_shared<ConnectionHandle>();

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  CreateTable(conn, table_name, getSchemaStr(kStdSchema));
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  CopyDescRec(conn, table_name, kStdSchema);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  DropTable(conn, table_name);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(DescriptorFieldsTest, SQLSetDescField) {
  auto const table_name = kDatasetName + ".ODBC_DESCRIPTORS_TEST";
  auto conn = std::make_shared<ConnectionHandle>();

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  CreateTable(conn, table_name, getSchemaStr(kStdSchema));
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  SQLHDESC ipd_handle; // Implementation param descriptor
  auto status = SQLGetStmtAttr(conn->hstmt, SQL_ATTR_IMP_PARAM_DESC, &ipd_handle, 0, NULL);
  CheckError(status, "SQLGetStmtAttr(SQL_ATTR_IMP_PARAM_DESC)", conn);
  status = SQLSetDescField(ipd_handle, 1, SQL_DESC_PARAMETER_TYPE,
              (SQLPOINTER)SQL_PARAM_INPUT, SQL_IS_INTEGER);
  CheckError(status, "SQLGetStmtAttr(SQL_ATTR_IMP_ROW_DESC)", conn);

  SQLSMALLINT type;
  status = SQLGetDescField(ipd_handle, 1, SQL_DESC_PARAMETER_TYPE, &type,
              SQL_IS_SMALLINT, NULL);
  EXPECT_EQ(type, SQL_PARAM_INPUT);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);

  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  DropTable(conn, table_name);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(DriverPropertiesTest, SQLGetFunctions) {
  auto conn = std::make_shared<ConnectionHandle>();
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  EXPECT_EQ(GetAllFunctions(conn), SQL_SUCCESS);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

TEST(DriverPropertiesTest, SQLGetTypeInfo) {
  auto conn = std::make_shared<ConnectionHandle>();
  EXPECT_EQ(Connect(kDefaultConnectionString, conn), SQL_SUCCESS);
  CheckDataTypes(conn);
  EXPECT_EQ(Disconnect(conn), SQL_SUCCESS);
}

}  // namespace bigquery_odbc
}  // namespace cloud
}  // namespace google
