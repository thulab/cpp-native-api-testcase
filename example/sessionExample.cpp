#include "Common.h"
#include "Session.h"
#include <cstdint>

using namespace std;
Session *session;

int main() {
    try {
        session = new Session("127.0.0.1", 6667, "root", "root");
        session->open(false);
        session->createDatabase("root.test_db1");

        // 创建非对齐时间序列
        vector<string> paths;
        paths.emplace_back("root.test_db1.d1.s1");
        paths.emplace_back("root.test_db1.d1.s2");
        paths.emplace_back("root.test_db1.d1.s3");
        paths.emplace_back("root.test_db1.d1.s4");
        paths.emplace_back("root.test_db1.d1.s5");
        paths.emplace_back("root.test_db1.d1.s6");
        paths.emplace_back("root.test_db1.d1.s7");
        paths.emplace_back("root.test_db1.d1.s8");
        paths.emplace_back("root.test_db1.d1.s9");
        paths.emplace_back("root.test_db1.d1.s10");
        vector<TSDataType::TSDataType> tsDataTypes;
        tsDataTypes.push_back(TSDataType::BOOLEAN);
        tsDataTypes.push_back(TSDataType::INT32);
        tsDataTypes.push_back(TSDataType::INT64);
        tsDataTypes.push_back(TSDataType::FLOAT);
        tsDataTypes.push_back(TSDataType::DOUBLE);
        tsDataTypes.push_back(TSDataType::TEXT);
        tsDataTypes.push_back(TSDataType::STRING);
        tsDataTypes.push_back(TSDataType::DATE);
        tsDataTypes.push_back(TSDataType::BLOB);
        tsDataTypes.push_back(TSDataType::TIMESTAMP);
        vector<TSEncoding::TSEncoding> tsEncodings;
        tsEncodings.push_back(TSEncoding::PLAIN);
        tsEncodings.push_back(TSEncoding::PLAIN);
        tsEncodings.push_back(TSEncoding::PLAIN);
        tsEncodings.push_back(TSEncoding::PLAIN);
        tsEncodings.push_back(TSEncoding::PLAIN);
        tsEncodings.push_back(TSEncoding::PLAIN);
        tsEncodings.push_back(TSEncoding::PLAIN);
        tsEncodings.push_back(TSEncoding::PLAIN);
        tsEncodings.push_back(TSEncoding::PLAIN);
        tsEncodings.push_back(TSEncoding::PLAIN);
        vector<CompressionType::CompressionType> compressionTypes;
        compressionTypes.push_back(CompressionType::LZ4);
        compressionTypes.push_back(CompressionType::LZ4);
        compressionTypes.push_back(CompressionType::LZ4);
        compressionTypes.push_back(CompressionType::LZ4);
        compressionTypes.push_back(CompressionType::LZ4);
        compressionTypes.push_back(CompressionType::LZ4);
        compressionTypes.push_back(CompressionType::LZ4);
        compressionTypes.push_back(CompressionType::LZ4);
        compressionTypes.push_back(CompressionType::LZ4);
        compressionTypes.push_back(CompressionType::LZ4);
        session->createMultiTimeseries(paths, tsDataTypes, tsEncodings, compressionTypes, nullptr, nullptr, nullptr, nullptr);

        // 构建tablet
        pair<string, TSDataType::TSDataType> pairA("s1", TSDataType::BOOLEAN);
        pair<string, TSDataType::TSDataType> pairB("s2", TSDataType::INT32);
        pair<string, TSDataType::TSDataType> pairC("s3", TSDataType::INT64);
        pair<string, TSDataType::TSDataType> pairD("s4", TSDataType::FLOAT);
        pair<string, TSDataType::TSDataType> pairE("s5", TSDataType::DOUBLE);
        pair<string, TSDataType::TSDataType> pairF("s6", TSDataType::TEXT);
        pair<string, TSDataType::TSDataType> pairG("s7", TSDataType::STRING);
        pair<string, TSDataType::TSDataType> pairH("s8", TSDataType::DATE);
        pair<string, TSDataType::TSDataType> pairI("s9", TSDataType::BLOB);
        pair<string, TSDataType::TSDataType> pairJ("s10", TSDataType::TIMESTAMP);
        vector<pair<string, TSDataType::TSDataType>> schemas;
        schemas.push_back(pairA);
        schemas.push_back(pairB);
        schemas.push_back(pairC);
        schemas.push_back(pairD);
        schemas.push_back(pairE);
        schemas.push_back(pairF);
        schemas.push_back(pairG);
        schemas.push_back(pairH);
        schemas.push_back(pairI);
        schemas.push_back(pairJ);
        Tablet tablet("root.test_db1.d1", schemas, 100);

        // 写入数据
        int64_t time = 0;
        for (int row = 0; row < 100; row++) {
            int rowIndex = tablet.rowSize++;
            tablet.timestamps[rowIndex] = time++;
            boost::gregorian::date dateValue(2025, 5, 7);
            tablet.addValue("s1", rowIndex, true);
            tablet.addValue("s2", rowIndex, row);
            tablet.addValue("s3", rowIndex, int64_t(row));
            tablet.addValue("s4", rowIndex, 111.1F);
            tablet.addValue("s5", rowIndex, 111.1);
            tablet.addValue("s6", rowIndex, "text");
            tablet.addValue("s7", rowIndex, "string");
            tablet.addValue("s8", rowIndex, dateValue);
            tablet.addValue("s9", rowIndex, "text");
            tablet.addValue("s10", rowIndex, int64_t(row));
            // 标记空值
            if (row % 2 == 0) {
                tablet.bitMaps[0].mark(row);
                tablet.bitMaps[1].mark(row);
                tablet.bitMaps[2].mark(row);
                tablet.bitMaps[3].mark(row);
                tablet.bitMaps[4].mark(row);
            } else {
                tablet.bitMaps[5].mark(row);
                tablet.bitMaps[6].mark(row);
                tablet.bitMaps[7].mark(row);
                tablet.bitMaps[8].mark(row);
                tablet.bitMaps[9].mark(row);
            }
            if (tablet.rowSize == tablet.maxRowNumber) {
                session->insertTablet(tablet);
                tablet.reset();
            }
        }
        if (tablet.rowSize != 0) {
            session->insertTablet(tablet);
            tablet.reset();
        }

        // 查询数据
        unique_ptr<SessionDataSet> sessionDataSet = session->executeQueryStatement("select s1,s2,s3,s4,s5,s6,s7,s8,s9,s10 from root.test_db1.d1");
        for (const string &name: sessionDataSet->getColumnNames()) {
            cout << name << "  ";
        }
        cout << endl;
        for (const string &type: sessionDataSet->getColumnTypeList()) {
            cout << type << "  ";
        }
        cout << endl;
        int count = 0;
        sessionDataSet->setFetchSize(1024);
        tsDataTypes.push_back(TSDataType::BOOLEAN);
        tsDataTypes.push_back(TSDataType::INT32);
        tsDataTypes.push_back(TSDataType::INT64);
        tsDataTypes.push_back(TSDataType::FLOAT);
        tsDataTypes.push_back(TSDataType::DOUBLE);
        tsDataTypes.push_back(TSDataType::TEXT);
        tsDataTypes.push_back(TSDataType::STRING);
        tsDataTypes.push_back(TSDataType::DATE);
        tsDataTypes.push_back(TSDataType::BLOB);
        tsDataTypes.push_back(TSDataType::TIMESTAMP);
        auto dataIter = sessionDataSet->getIterator();
        while (dataIter.next()) {
            cout << dataIter.getLongByIndex(1) << " ";
            cout << dataIter.getBooleanByIndex(2) << " ";
            cout << dataIter.getIntByIndex(3) << " ";
            cout << dataIter.getLongByIndex(4) << " ";
            cout << dataIter.getFloatByIndex(5) << " ";
            cout << dataIter.getDoubleByIndex(6) << " ";
            cout << dataIter.getStringByIndex(7) << " ";
            cout << dataIter.getStringByIndex(8) << " ";
            cout << dataIter.getDateByIndex(9) << " ";
            cout << dataIter.getStringByIndex(10) << " ";
            cout << dataIter.getTimestampByIndex(11) << endl;
            count++;
        }
        cout << "总行数为：" << count << endl; 
        session->deleteDatabase("root.test_db1");
        session->close();
    } catch (IoTDBConnectionException &e) {
        cout << e.what() << endl;
    } catch (IoTDBException &e) {
        cout << e.what() << endl;
    }
}



// int main() {
//     try {
//         session = new Session("127.0.0.1", 6667, "root", "root");
//         session->open(false);
//         session->createDatabase("root.test_db1");

//         // 创建非对齐时间序列
//         vector<string> paths;
//         paths.emplace_back("root.test_db1.d1.s1");
//         paths.emplace_back("root.test_db1.d1.s2");
//         paths.emplace_back("root.test_db1.d1.s3");
//         paths.emplace_back("root.test_db1.d1.s4");
//         paths.emplace_back("root.test_db1.d1.s5");
//         paths.emplace_back("root.test_db1.d1.s6");
//         paths.emplace_back("root.test_db1.d1.s7");
//         paths.emplace_back("root.test_db1.d1.s8");
//         paths.emplace_back("root.test_db1.d1.s9");
//         paths.emplace_back("root.test_db1.d1.s10");
//         vector<TSDataType::TSDataType> tsDataTypes;
//         tsDataTypes.push_back(TSDataType::BOOLEAN);
//         tsDataTypes.push_back(TSDataType::INT32);
//         tsDataTypes.push_back(TSDataType::INT64);
//         tsDataTypes.push_back(TSDataType::FLOAT);
//         tsDataTypes.push_back(TSDataType::DOUBLE);
//         tsDataTypes.push_back(TSDataType::TEXT);
//         tsDataTypes.push_back(TSDataType::STRING);
//         tsDataTypes.push_back(TSDataType::DATE);
//         tsDataTypes.push_back(TSDataType::BLOB);
//         tsDataTypes.push_back(TSDataType::TIMESTAMP);
//         vector<TSEncoding::TSEncoding> tsEncodings;
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         tsEncodings.push_back(TSEncoding::PLAIN);
//         vector<CompressionType::CompressionType> compressionTypes;
//         compressionTypes.push_back(CompressionType::LZ4);
//         compressionTypes.push_back(CompressionType::LZ4);
//         compressionTypes.push_back(CompressionType::LZ4);
//         compressionTypes.push_back(CompressionType::LZ4);
//         compressionTypes.push_back(CompressionType::LZ4);
//         compressionTypes.push_back(CompressionType::LZ4);
//         compressionTypes.push_back(CompressionType::LZ4);
//         compressionTypes.push_back(CompressionType::LZ4);
//         compressionTypes.push_back(CompressionType::LZ4);
//         compressionTypes.push_back(CompressionType::LZ4);
//         session->createMultiTimeseries(paths, tsDataTypes, tsEncodings, compressionTypes, nullptr, nullptr, nullptr, nullptr);

//         // 构建tablet
//         pair<string, TSDataType::TSDataType> pairA("s1", TSDataType::BOOLEAN);
//         pair<string, TSDataType::TSDataType> pairB("s2", TSDataType::INT32);
//         pair<string, TSDataType::TSDataType> pairC("s3", TSDataType::INT64);
//         pair<string, TSDataType::TSDataType> pairD("s4", TSDataType::FLOAT);
//         pair<string, TSDataType::TSDataType> pairE("s5", TSDataType::DOUBLE);
//         pair<string, TSDataType::TSDataType> pairF("s6", TSDataType::TEXT);
//         pair<string, TSDataType::TSDataType> pairG("s7", TSDataType::STRING);
//         pair<string, TSDataType::TSDataType> pairH("s8", TSDataType::DATE);
//         pair<string, TSDataType::TSDataType> pairI("s9", TSDataType::BLOB);
//         pair<string, TSDataType::TSDataType> pairJ("s10", TSDataType::TIMESTAMP);
//         vector<pair<string, TSDataType::TSDataType>> schemas;
//         schemas.push_back(pairA);
//         schemas.push_back(pairB);
//         schemas.push_back(pairC);
//         schemas.push_back(pairD);
//         schemas.push_back(pairE);
//         schemas.push_back(pairF);
//         schemas.push_back(pairG);
//         schemas.push_back(pairH);
//         schemas.push_back(pairI);
//         schemas.push_back(pairJ);
//         Tablet tablet("root.test_db1.d1", schemas, 100);

//         // 写入数据
//         int64_t time = 0;
//         for (int row = 0; row < 100; row++) {
//             int rowIndex = tablet.rowSize++;
//             tablet.timestamps[rowIndex] = time++;
//             boost::gregorian::date dateValue(2025, 5, 7);
//             tablet.addValue("s1", rowIndex, true);
//             tablet.addValue("s2", rowIndex, row);
//             tablet.addValue("s3", rowIndex, int64_t(row));
//             tablet.addValue("s4", rowIndex, 111.1F);
//             tablet.addValue("s5", rowIndex, 111.1);
//             tablet.addValue("s6", rowIndex, "text");
//             tablet.addValue("s7", rowIndex, "string");
//             tablet.addValue("s8", rowIndex, dateValue);
//             tablet.addValue("s9", rowIndex, "text");
//             tablet.addValue("s10", rowIndex, int64_t(row));
//             // 标记空值
//             // if (row % 2 == 0) {
//             //     tablet.bitMaps[0].mark(row);
//             //     tablet.bitMaps[1].mark(row);
//             //     tablet.bitMaps[2].mark(row);
//             //     tablet.bitMaps[3].mark(row);
//             //     tablet.bitMaps[4].mark(row);
//             // } else {
//             //     tablet.bitMaps[5].mark(row);
//             //     tablet.bitMaps[6].mark(row);
//             //     tablet.bitMaps[7].mark(row);
//             //     tablet.bitMaps[8].mark(row);
//             //     tablet.bitMaps[9].mark(row);
//             // }
//             if (tablet.rowSize == tablet.maxRowNumber) {
//                 session->insertTablet(tablet);
//                 tablet.reset();
//             }
//         }
//         if (tablet.rowSize != 0) {
//             session->insertTablet(tablet);
//             tablet.reset();
//         }

//         // 查询数据
//         unique_ptr<SessionDataSet> sessionDataSet = session->executeQueryStatement("select s1,s2,s3,s4,s5,s6,s7,s8,s9,s10 from root.test_db1.d1");
//         for (const string &name: sessionDataSet->getColumnNames()) {
//             cout << name << "  ";
//         }
//         cout << endl;
//         for (const string &type: sessionDataSet->getColumnTypeList()) {
//             cout << type << "  ";
//         }
//         cout << endl;
//         int count = 0;
//         sessionDataSet->setFetchSize(1024);
//         while (sessionDataSet->hasNext()) {
//             auto record = sessionDataSet->next();
//             cout << record->timestamp << " ";
//             cout << record->fields[0].boolV << " ";
//             cout << record->fields[1].intV << " ";
//             cout << record->fields[2].longV << " ";
//             cout << record->fields[3].floatV << " ";
//             cout << record->fields[4].doubleV << " ";
//             cout << record->fields[5].stringV << " ";
//             cout << record->fields[6].stringV << " ";
//             cout << record->fields[7].dateV << " ";
//             cout << record->fields[8].stringV << " ";
//             cout << record->fields[9].longV << endl;
//             count++;
//         }
//         cout << "总行数为：" << count << endl; 
//         session->deleteDatabase("root.test_db1");
//         session->close();
//     } catch (IoTDBConnectionException &e) {
//         cout << e.what() << endl;
//     } catch (IoTDBException &e) {
//         cout << e.what() << endl;
//     }

// }
