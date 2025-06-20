#include "TableSession.h"
#include "TableSessionBuilder.h"
#include <iostream>
#include <ostream>

using namespace std;

TableSession *session;

int main() {
    try {
        session = (new TableSessionBuilder())->build();
        try {
            session->executeNonQueryStatement("CREATE DATABASE test_insert_db1");
            session->executeNonQueryStatement("use test_insert_db1");
            session->executeNonQueryStatement("CREATE TABLE t1 (tag1 STRING TAG,attr1 STRING ATTRIBUTE,s1 BOOLEAN FIELD,s2 INT32 FIELD,s3 INT64 FIELD,s4 FLOAT FIELD,s5 DOUBLE FIELD,s6 TEXT FIELD)");
            // 构建Tablet
            vector<pair<string, TSDataType::TSDataType>> schemaList {
                make_pair("tag1", TSDataType::TEXT),
                make_pair("attr1", TSDataType::TEXT),
                make_pair("s1", TSDataType::BOOLEAN),
                make_pair("s2", TSDataType::INT32),
                make_pair("s3", TSDataType::INT64),
                make_pair("s4", TSDataType::FLOAT),
                make_pair("s5", TSDataType::DOUBLE),
                make_pair("s6", TSDataType::TEXT)
            };
            vector<ColumnCategory> columnTypes = {
                ColumnCategory::TAG,
                ColumnCategory::ATTRIBUTE,
                ColumnCategory::FIELD,
                ColumnCategory::FIELD,
                ColumnCategory::FIELD,
                ColumnCategory::FIELD,
                ColumnCategory::FIELD,
                ColumnCategory::FIELD
            };
            Tablet tablet("t1", schemaList, columnTypes, 100);

            // 写入包含时间戳的空值
            int64_t time = 0;
            for (int row = 0; row < 10; row++) {
                int rowIndex = tablet.rowSize++;
                tablet.timestamps[rowIndex] = time++;
                tablet.addValue("tag1", rowIndex, "tag1");
                tablet.addValue("attr1", rowIndex, "attr1");
                tablet.addValue("s1", rowIndex, true);
                tablet.addValue("s2", rowIndex, row);
                tablet.addValue("s3", rowIndex, int64_t(row));
                tablet.addValue("s4", rowIndex, 111.1F);
                tablet.addValue("s5", rowIndex, 111.1);
                tablet.addValue("s6", rowIndex, "test");
            }
            session->insert(tablet);
        } catch (IoTDBException &e) {
            cout << e.what() << endl;
        }

        session->close();
    } catch (IoTDBConnectionException &e) {
        cout << e.what() << endl;
    }
    return 0;
}