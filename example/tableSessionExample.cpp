#include "Common.h"
#include "TableSession.h"
#include "TableSessionBuilder.h"
#include <iostream>
#include <ostream>
#include <boost/optional/optional_io.hpp>

using namespace std;

TableSession *session;

// int main() {
//     try {
//         session = (new TableSessionBuilder())
//             ->host("127.0.0.1")
//             ->rpcPort(6667)
//             ->username("root")
//             ->password("root")
//             ->build();
        
//         // 准备元数据
//         session->executeNonQueryStatement("CREATE DATABASE IF NOT EXISTS db1");
//         session->executeNonQueryStatement("USE db1");
//         session->executeNonQueryStatement("CREATE TABLE table1 ("
//             "\"没问题\" STRING tag,"
//             "\"123\" STRING tag,"
//             "\"!@#\" STRING attribute,"
//             "\" attr2 \" STRING attribute,"
//             "\" 没问题123!@# \" BOOLEAN field,"
//             "field2 INT32 field,"
//             "field3 INT64 field,"
//             "field4 FLOAT field,"
//             "field5 DOUBLE field,"
//             "field6 TEXT field,"
//             "field7 TIMESTAMP field,"
//             "field8 DATE field,"
//             "field9 BLOB field,"
//             "field10 STRING field)"
//         );

//         // 构建Tablet
//         vector<pair<string, TSDataType::TSDataType>> schemaList;
//         schemaList.push_back(make_pair("没问题", TSDataType::STRING));
//         schemaList.push_back(make_pair("123", TSDataType::STRING));
//         schemaList.push_back(make_pair("!@#", TSDataType::STRING));
//         schemaList.push_back(make_pair(" attr2 ", TSDataType::STRING));
//         schemaList.push_back(make_pair(" 没问题123!@# ", TSDataType::BOOLEAN));
//         schemaList.push_back(make_pair("field2", TSDataType::INT32));
//         schemaList.push_back(make_pair("field3", TSDataType::INT64));
//         schemaList.push_back(make_pair("field4", TSDataType::FLOAT));
//         schemaList.push_back(make_pair("field5", TSDataType::DOUBLE));
//         schemaList.push_back(make_pair("field6", TSDataType::TEXT));
//         schemaList.push_back(make_pair("field7", TSDataType::TIMESTAMP));
//         schemaList.push_back(make_pair("field8", TSDataType::DATE));
//         schemaList.push_back(make_pair("field9", TSDataType::BLOB));
//         schemaList.push_back(make_pair("field10", TSDataType::STRING));
//         vector<ColumnCategory> columnTypes;
//         columnTypes.push_back(ColumnCategory::TAG);
//         columnTypes.push_back(ColumnCategory::TAG);
//         columnTypes.push_back(ColumnCategory::ATTRIBUTE);
//         columnTypes.push_back(ColumnCategory::ATTRIBUTE);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         columnTypes.push_back(ColumnCategory::FIELD);
//         int64_t timestamp = 0;
//         int maxRowNumber = 100;
//         Tablet tablet("table1", schemaList, columnTypes, maxRowNumber);

//         // 写入数据
//         for (int row = 0; row < maxRowNumber; row++) {
//             int rowIndex = tablet.rowSize++;
//             tablet.timestamps[rowIndex] = timestamp + row;
//             tablet.addValue(0, rowIndex, "tag1_" + to_string(row));
//             tablet.addValue(1, rowIndex, "tag2_" + to_string(row));
//             tablet.addValue(2, rowIndex, "attr1_" + to_string(row));
//             tablet.addValue(3, rowIndex, "attr2_" + to_string(row));
//             tablet.addValue(4, rowIndex, row % 2 == 0);
//             tablet.addValue(5, rowIndex, static_cast<int32_t>(INT32_MAX));
//             // tablet.addValue(5, rowIndex, static_cast<int32_t>(INT32_MIN));
//             tablet.addValue(6, rowIndex, static_cast<int64_t>(INT64_MIN));
//             // tablet.addValue(6, rowIndex, static_cast<int64_t>(INT64_MAX));
//             tablet.addValue(7, rowIndex, static_cast<float>(row * 1.1f));
//             tablet.addValue(8, rowIndex, static_cast<double>(row * 1.1f));
//             tablet.addValue(9, rowIndex, "没问题123!@#text_" + to_string(row));
//             tablet.addValue(10, rowIndex, static_cast<int64_t>(INT64_MIN));
//             // tablet.addValue(10, rowIndex, static_cast<int64_t>(INT64_MAX));
//             tablet.addValue(11, rowIndex, boost::gregorian::date(1400, 5, 15));
//             tablet.addValue(12, rowIndex, "没问题123!@#blob_" + to_string(row));
//             tablet.addValue(13, rowIndex, "没问题123!@#string_" + to_string(row));
//             if (row % 2 == 0) {
//                 tablet.bitMaps[0].mark(row);
//                 tablet.bitMaps[1].mark(row);
//                 tablet.bitMaps[2].mark(row);
//                 tablet.bitMaps[3].mark(row);
//                 tablet.bitMaps[4].mark(row);
//                 tablet.bitMaps[5].mark(row);
//                 tablet.bitMaps[6].mark(row);
//                 tablet.bitMaps[7].mark(row);
//                 tablet.bitMaps[8].mark(row);
//                 tablet.bitMaps[9].mark(row);
//                 tablet.bitMaps[10].mark(row);
//                 tablet.bitMaps[11].mark(row);
//                 tablet.bitMaps[12].mark(row);
//                 tablet.bitMaps[13].mark(row);
//             }
//             // tablet.bitMaps[0].mark(row);
//             // tablet.bitMaps[1].mark(row);
//             // tablet.bitMaps[2].mark(row);
//             // tablet.bitMaps[3].mark(row);
//             // tablet.bitMaps[4].mark(row);
//             // tablet.bitMaps[5].mark(row);
//             // tablet.bitMaps[6].mark(row);
//             // tablet.bitMaps[7].mark(row);
//             // tablet.bitMaps[8].mark(row);
//             // tablet.bitMaps[9].mark(row);
//             // tablet.bitMaps[10].mark(row);
//             // // tablet.bitMaps[11].mark(row);
//             // tablet.bitMaps[12].mark(row);
//             // tablet.bitMaps[13].mark(row);
//             if (tablet.rowSize == tablet.maxRowNumber) {
//                 session->insert(tablet, true);
//                 tablet.reset();
//             }
//         }
//         if (tablet.rowSize != 0) {
//             session->insert(tablet, true);
//             tablet.reset();
//         }
//         session->executeNonQueryStatement("FLUSH");

//         // 查询数据
//         unique_ptr<SessionDataSet> sessionDataSet = session->executeQueryStatement("SELECT * FROM table1 order by time");
//         auto dataIter = sessionDataSet->getIterator();
//         int rowNum = 0;
//         timestamp = 0;
//         for (const string &name: dataIter.getColumnNames()) {
//         cout << name << "  ";
//         }
//         cout << endl;
//         for (const string &type: dataIter.getColumnTypeList()) {
//             cout << type << "  ";
//         }
//         cout << endl;
//         while (dataIter.next()) {
//             cout << dataIter.getLongByIndex(1) << " ";
//             cout << dataIter.getStringByIndex(2) << " ";
//             cout << dataIter.getStringByIndex(3) << " ";
//             cout << dataIter.getStringByIndex(4) << " ";
//             cout << dataIter.getStringByIndex(5) << " ";
//             cout << dataIter.getBooleanByIndex(6) << " ";
//             cout << dataIter.getIntByIndex(7) << " ";
//             cout << dataIter.getLongByIndex(8) << " ";
//             cout << dataIter.getFloatByIndex(9) << " ";
//             cout << dataIter.getDoubleByIndex(10) << " ";
//             cout << dataIter.getStringByIndex(11) << " ";;
//             cout << dataIter.getTimestampByIndex(12) << " ";
//             cout << dataIter.getDateByIndex(13) << " ";
//             cout << dataIter.getStringByIndex(14) << " ";
//             cout << dataIter.getStringByIndex(15) << endl;
//             cout << dataIter.getLong("time") << " ";
//             cout << dataIter.getString("没问题") << " ";
//             cout << dataIter.getString("123") << " ";
//             cout << dataIter.getString("!@#") << " ";
//             cout << dataIter.getString(" attr2 ") << " ";
//             cout << dataIter.getBoolean(" 没问题123!@# ") << " ";
//             cout << dataIter.getInt("field2") << " ";
//             cout << dataIter.getLong("field3") << " ";
//             cout << dataIter.getFloat("field4") << " ";
//             cout << dataIter.getDouble("field5") << " ";
//             cout << dataIter.getString("field6") << " ";;
//             cout << dataIter.getTimestamp("field7") << " ";
//             cout << dataIter.getDate("field8") << " ";
//             cout << dataIter.getString("field9") << " ";
//             cout << dataIter.getString("field10") << endl;
//             cout << dataIter.isNullByIndex(1) << " ";
//             cout << dataIter.isNullByIndex(2) << " ";
//             cout << dataIter.isNullByIndex(3) << " ";
//             cout << dataIter.isNullByIndex(4) << " ";
//             cout << dataIter.isNullByIndex(5) << " ";
//             cout << dataIter.isNullByIndex(6) << " ";
//             cout << dataIter.isNullByIndex(7) << " ";
//             cout << dataIter.isNullByIndex(8) << " ";
//             cout << dataIter.isNullByIndex(9) << " ";
//             cout << dataIter.isNullByIndex(10) << " ";
//             cout << dataIter.isNullByIndex(11) << " ";;
//             cout << dataIter.isNullByIndex(12) << " ";
//             cout << dataIter.isNullByIndex(13) << " ";
//             cout << dataIter.isNullByIndex(14) << " ";
//             cout << dataIter.isNullByIndex(15) << endl;
//             cout << dataIter.isNull("time") << " ";
//             cout << dataIter.isNull("没问题") << " ";
//             cout << dataIter.isNull("123") << " ";
//             cout << dataIter.isNull("!@#") << " ";
//             cout << dataIter.isNull(" attr2 ") << " ";
//             cout << dataIter.isNull(" 没问题123!@# ") << " ";
//             cout << dataIter.isNull("field2") << " ";
//             cout << dataIter.isNull("field3") << " ";
//             cout << dataIter.isNull("field4") << " ";
//             cout << dataIter.isNull("field5") << " ";
//             cout << dataIter.isNull("field6") << " ";;
//             cout << dataIter.isNull("field7") << " ";
//             cout << dataIter.isNull("field8") << " ";
//             cout << dataIter.isNull("field9") << " ";
//             cout << dataIter.isNull("field10") << endl;
//             rowNum++;
//         }
//         cout << "查询总行数：" << rowNum << endl;
//         // session->executeNonQueryStatement("DROP DATABASE db1");
//     } catch (IoTDBConnectionException &e) {
//         cout << e.what() << endl;
//     } catch (IoTDBException &e) {
//         cout << e.what() << endl;
//     } catch (exception &e) {
//         cout << e.what() << endl;
//     }
// }


// int main() {
//     try {
//         session = (new TableSessionBuilder())
//             ->host("127.0.0.1")
//             ->rpcPort(6667)
//             ->username("root")
//             ->password("root")
//             ->build();
//         session->executeNonQueryStatement("CREATE DATABASE IF NOT EXISTS db1");
//         session->executeNonQueryStatement("USE db1");
//         session->executeNonQueryStatement("DROP TABLE IF EXISTS table1");
//         session->executeNonQueryStatement("CREATE TABLE table1 ("
//             "tag1 string tag,"
//             "attr1 string attribute,"
//             "m1 DATE field)");
//         vector<pair<string, TSDataType::TSDataType>> schemaList;
//         schemaList.push_back(make_pair("tag1", TSDataType::TEXT));
//         schemaList.push_back(make_pair("attr1", TSDataType::TEXT));
//         schemaList.push_back(make_pair("m1", TSDataType::DATE));
//         vector<ColumnCategory> columnTypes = {ColumnCategory::TAG, ColumnCategory::ATTRIBUTE, ColumnCategory::FIELD};

//         int64_t timestamp = 0;
//         Tablet tablet("table1", schemaList, columnTypes, 15);

//         for (int row = 0; row < 15; row++) {
//             int rowIndex = tablet.rowSize++;
//             tablet.timestamps[rowIndex] = timestamp + row;
//             tablet.addValue(0, rowIndex, "tag1_" + to_string(row));
//             tablet.addValue(1, rowIndex, "attr1_" + to_string(row));
//             tablet.addValue(2, rowIndex, boost::gregorian::date(2025, 5, 15));
//             if (row % 2 == 0) {
//                 tablet.bitMaps[0].mark(row);
//                 tablet.bitMaps[1].mark(row);
//                 tablet.bitMaps[2].mark(row);
//             }
//             if (tablet.rowSize == tablet.maxRowNumber) {
//                 session->insert(tablet, true);
//                 tablet.reset();
//             }
//         }
//         if (tablet.rowSize != 0) {
//             session->insert(tablet, true);
//             tablet.reset();
//         }
//         session->executeNonQueryStatement("FLUSH");
//         int cnt = 0;
//         unique_ptr<SessionDataSet> sessionDataSet = session->executeQueryStatement("SELECT * FROM table1 order by time");
//         while (sessionDataSet->hasNext()) {
//             auto rowRecord = sessionDataSet->next();
//             cout << record->timestamp << " ";
//             cout << rowRecord->fields[0].longV << " ";
//             cout << rowRecord->fields[1].stringV << " ";
//             cout << rowRecord->fields[2].stringV << " ";
//             cout << rowRecord->fields[3].dateV << endl;
//             cnt++;
//         }
//     } catch (IoTDBConnectionException &e) {
//         cout << e.what() << endl;
//     } catch (IoTDBException &e) {
//         cout << e.what() << endl;
//     }
// }

int main(){
    try {
                session = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->build();
    session->executeNonQueryStatement("CREATE DATABASE IF NOT EXISTS db1");
    session->executeNonQueryStatement("USE db1");
    session->executeNonQueryStatement("DROP TABLE IF EXISTS table1");
    session->executeNonQueryStatement("CREATE TABLE table1 ("
        "field1 BOOLEAN field,"
        "field2 INT32 field,"
        "field3 INT64 field,"
        "field4 FLOAT field,"
        "field5 DOUBLE field,"
        "field6 TEXT field,"
        "field7 TIMESTAMP field,"
        "field8 DATE field,"
        "field9 BLOB field,"
        "field10 STRING field)");

    vector<pair<string, TSDataType::TSDataType>> schemaList;
    schemaList.push_back(make_pair("field1", TSDataType::BOOLEAN));
    schemaList.push_back(make_pair("field2", TSDataType::INT32));
    schemaList.push_back(make_pair("field3", TSDataType::INT64));
    schemaList.push_back(make_pair("field4", TSDataType::FLOAT));
    schemaList.push_back(make_pair("field5", TSDataType::DOUBLE));
    schemaList.push_back(make_pair("field6", TSDataType::TEXT));
    schemaList.push_back(make_pair("field7", TSDataType::TIMESTAMP));
    schemaList.push_back(make_pair("field8", TSDataType::DATE));
    schemaList.push_back(make_pair("field9", TSDataType::BLOB));
    schemaList.push_back(make_pair("field10", TSDataType::STRING));

    vector<ColumnCategory> columnTypes(10, ColumnCategory::FIELD);

    int64_t timestamp = 0;
    int maxRowNumber = 50000;
    Tablet tablet("table1", schemaList, columnTypes, maxRowNumber);

    for (int row = 0; row < maxRowNumber; row++) {
        int rowIndex = tablet.rowSize++;
        tablet.timestamps[rowIndex] = timestamp + row;
        tablet.addValue(0, rowIndex, row % 2 == 0);
        tablet.addValue(1, rowIndex, static_cast<int32_t>(row));
        tablet.addValue(2, rowIndex, static_cast<int64_t>(timestamp));
        tablet.addValue(3, rowIndex, static_cast<float>(row * 1.1f));
        tablet.addValue(4, rowIndex, static_cast<double>(row * 1.1f));
        tablet.addValue(5, rowIndex, "text_" + to_string(row));
        tablet.addValue(6, rowIndex, static_cast<int64_t>(timestamp));
        tablet.addValue(7, rowIndex, boost::gregorian::date(2025, 5, 15));
        tablet.addValue(8, rowIndex, "blob_" + to_string(row));
        tablet.addValue(9, rowIndex, "string_" + to_string(row));

        if (row % 2 == 0) {
            for (int col = 0; col <= 9; col++) {
                tablet.bitMaps[col].mark(row);
            }
        }

        if (tablet.rowSize == tablet.maxRowNumber) {
            session->insert(tablet, true);
            tablet.reset();
        }
    }

    if (tablet.rowSize != 0) {
        session->insert(tablet, true);
        tablet.reset();
    }
    session->executeNonQueryStatement("FLUSH");

    unique_ptr<SessionDataSet> sessionDataSet = session->executeQueryStatement("SELECT * FROM table1 order by time");
    auto dataIter = sessionDataSet->getIterator();
    int rowNum = 0;
    timestamp = 0;
    while (dataIter.next()) {
        if (rowNum % 2 == 0) {
            cout << dataIter.getBooleanByIndex(2).is_initialized() << " ";
            cout <<dataIter.getIntByIndex(3).is_initialized()<< " ";
            cout <<dataIter.getLongByIndex(4).is_initialized()<< " ";
            cout <<dataIter.getFloatByIndex(5).is_initialized()<< " ";
            cout <<dataIter.getDoubleByIndex(6).is_initialized()<< " ";
            cout <<dataIter.getStringByIndex(7).is_initialized()<< " ";
            cout <<dataIter.getLongByIndex(8).is_initialized()<< " ";
            cout <<dataIter.getDateByIndex(9).is_initialized()<< " ";
            cout <<dataIter.getStringByIndex(10).is_initialized()<< " ";
            cout <<dataIter.getStringByIndex(11).is_initialized()<< endl;
        } else {
            cout <<dataIter.getLongByIndex(1).value()<< " ";
            cout <<dataIter.getBooleanByIndex(2).value()<< " ";
            cout <<dataIter.getIntByIndex(3).value()<< " ";
            cout <<dataIter.getLongByIndex(4).value()<< " ";
            cout <<dataIter.getFloatByIndex(5).value()<< " ";
            cout <<dataIter.getDoubleByIndex(6).value()<< " ";
            cout <<dataIter.getStringByIndex(7).value()<< " ";
            cout <<dataIter.getLongByIndex(8).value()<< " ";
            cout <<dataIter.getDateByIndex(9).value()<< " ";
            cout <<dataIter.getStringByIndex(10).value()<< " ";
            cout <<dataIter.getStringByIndex(11).value()<< endl;
        }
        rowNum++;
    }

    // sessionDataSet = session->executeQueryStatement("SELECT * FROM table1 order by time");
    // rowNum = 0;
    // timestamp = 0;
    // while (sessionDataSet->hasNext()) {
    //     auto record = sessionDataSet->next();
    //     if (rowNum % 2 == 0) {
    //         record->fields[1].boolV.is_initialized();
    //         record->fields[2].intV.is_initialized();
    //         record->fields[3].longV.is_initialized();
    //         record->fields[4].floatV.is_initialized();
    //         record->fields[5].doubleV.is_initialized();
    //         record->fields[6].stringV.is_initialized();
    //         record->fields[7].longV.is_initialized();
    //         record->fields[8].dateV.is_initialized();
    //         record->fields[9].stringV.is_initialized();
    //         record->fields[10].stringV.is_initialized();
    //     } else {
    //         record->fields[1].boolV.value();
    //         record->fields[2].intV.value();
    //         record->fields[3].longV.value();
    //         record->fields[4].floatV.value();
    //         record->fields[5].doubleV.value();
    //         record->fields[6].stringV.value();
    //         record->fields[7].longV.value();
    //         record->fields[8].dateV.value();
    //         record->fields[9].stringV.value();
    //         record->fields[10].stringV.value();
    //     }
    //     rowNum++;
    // }
    session->executeNonQueryStatement("DROP DATABASE db1");
    } catch (IoTDBConnectionException &e) {
        cout << e.what() << endl;
    } catch (IoTDBException &e) {
        cout << e.what() << endl;
    }
}
