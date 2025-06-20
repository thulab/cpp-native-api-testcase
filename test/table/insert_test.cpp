#include "Session.h"
#include "TableSession.h"
#include "TableSessionBuilder.h"
#include <cstdint>
#include <iostream>
#include <ostream>
#include "gtest/gtest.h"
#include <string>
#include <fstream>

using namespace std;

TableSession *session1; // 作为全局的session变量
bool isErrorInsertTest = false; // 用于确认是否出错的标识
int expectNum = 0; // 用于记录插入期待的行数
int actualNum = 0; // 用于记录实际查询的行数
/*
 * 测试表模型写入操作
 */

class InsertTest : public ::testing::Test {
    protected:
        // 在每个测试用例执行之前调用
        void SetUp() override {
            // 初始化操作
            try {
                session1 = (new TableSessionBuilder())
                    ->host("127.0.0.1")
                    ->rpcPort(6667)
                    ->username("root")
                    ->password("root")
                    ->build();
                    
                session1->executeNonQueryStatement("CREATE DATABASE test_insert_db1");
                session1->executeNonQueryStatement("use test_insert_db1");
                session1->executeNonQueryStatement("CREATE TABLE t1 (tag1 STRING TAG,tag2 STRING TAG,attr1 STRING ATTRIBUTE,attr2 STRING ATTRIBUTE,s1 BOOLEAN FIELD,s2 INT32 FIELD,s3 INT64 FIELD,s4 FLOAT FIELD,s5 DOUBLE FIELD,s6 TEXT FIELD)");
            }  catch (IoTDBConnectionException &e) {
                isErrorInsertTest = true;
                ASSERT_EQ(false, isErrorInsertTest) << "[Error] test_fetchSize - " << string(e.what());
            } catch (IoTDBException &e) {
                isErrorInsertTest = true;
                ASSERT_EQ(false, isErrorInsertTest) << "[Error] test_fetchSize - " << string(e.what());
            }
        }
        // 在每个测试用例执行之后调用
        void TearDown() override {
            // 清理操作
            expectNum = 0;
            try {
                session1->executeNonQueryStatement("drop DATABASE test_insert_db1");
                session1->close();
            }  catch (IoTDBConnectionException &e) {
                isErrorInsertTest = true;
                ASSERT_EQ(false, isErrorInsertTest) << "[Error] test_fetchSize - " << string(e.what());
            } catch (IoTDBException &e) {
                isErrorInsertTest = true;
                ASSERT_EQ(false, isErrorInsertTest) << "[Error] test_fetchSize - " << string(e.what());
            }
        }
};

// 解析CVS文件
std::pair<std::vector<std::vector<std::string>>, std::string> parseCSVFileInsert(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return {{}, "Failed to open file"};
    }

    std::vector<std::vector<std::string>> result; // 用于存储最终结果
    std::string line;

    while (std::getline(file, line)) {
        // 跳过注释行
        if (!line.empty() && line[0] == '#') {
            continue;
        }

        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;

        // 以逗号分隔每行数据
        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // 将分割后的数据添加到结果中
        result.push_back(fields);
    }

    if (file.bad()) {
        return {{}, "Error occurred while reading the file"};
    }

    return {result, ""}; // 返回解析后的二维数组和空错误信息
}

// 验证合法性
void validity1(const string& caseName, const string& tableName, int& expectNum) {
    try {
        unique_ptr<SessionDataSet> dataSet3 = session1->executeQueryStatement("select * from " + tableName);
        while (dataSet3->hasNext()) {
            dataSet3->next();
            actualNum++;
        }
        ASSERT_EQ(expectNum, actualNum)<< "The expected quantity is inconsistent with the actual quantity, expected: "<< expectNum << ", actual: " << actualNum;
        actualNum = 0;
    } catch (IoTDBException &e) {
        isErrorInsertTest = true;
        ASSERT_EQ(false, isErrorInsertTest) <<"[Error] " << caseName << " - " << string(e.what());
    }
}

// 查询
void test_executeQueryStatement(const string& tableName) {
    unique_ptr<SessionDataSet> dataSet = session1->executeQueryStatement("select * from " + tableName + " order by time");
    for (const string &name: dataSet->getColumnNames()) {
        cout << name << "  ";
    }
    cout << endl;
    for (const string &type: dataSet->getColumnTypeList()) {
        cout << type << "  ";
    }
    cout << endl;
    // dataSet->constructOneRow();
    while (dataSet->hasNext()) {
        cout << dataSet->next()->toString();
    }
    cout << endl;

    // while (dataSet->hasNext()) {
    //     RowRecord *next = dataSet->next();
    //     cout << next->timestamp << " " << next->fields[1].dataType << endl;
    // }

    // cout << dataSet->isNull(0, 0) << endl;
    // cout << dataSet->getFetchSize() << endl;
    // dataSet->setFetchSize(1234);
    // cout << dataSet->getFetchSize() << endl;
    
}


// |------------ Normal Test ------------|

// 1、写入各种数据类型包含空值（同一时间分区、设备）
TEST_F(InsertTest, test_all_datatype_null_same_time_and_device_and_device) {
    
    try{
        // 构建Tablet
        vector<pair<string, TSDataType::TSDataType>> schemaList {
            make_pair("Tag1", TSDataType::TEXT),
            make_pair("tag2", TSDataType::TEXT),
            make_pair("Attr1", TSDataType::TEXT),
            make_pair("attr2", TSDataType::TEXT),
            make_pair("S1", TSDataType::BOOLEAN),
            make_pair("s2", TSDataType::INT32),
            make_pair("S3", TSDataType::INT64),
            make_pair("s4", TSDataType::FLOAT),
            make_pair("S5", TSDataType::DOUBLE),
            make_pair("s6", TSDataType::TEXT)
        };
        vector<ColumnCategory> columnTypes = {
            ColumnCategory::TAG,
            ColumnCategory::TAG,
            ColumnCategory::ATTRIBUTE,
            ColumnCategory::ATTRIBUTE,
            ColumnCategory::FIELD,
            ColumnCategory::FIELD,
            ColumnCategory::FIELD,
            ColumnCategory::FIELD,
            ColumnCategory::FIELD,
            ColumnCategory::FIELD
        };
        Tablet tablet("t1", schemaList, columnTypes, 100);

        // 获取数据
        auto [result, err] = parseCSVFileInsert("../../../data/insert_data1.csv");

        // （1）写入各种类型数据（不含空值）
        for (int row = 0; row < result.size(); row++) { // 遍历每行
            int rowIndex = tablet.rowSize++; // 增加并获取有效行
            tablet.timestamps[rowIndex] = stoll(result[row][0]); // 添加时间戳
            for (int column = 0; column < result[row].size() - 1; column++) { // 遍历每列（因为包含一列时间戳需要减少一列）
                switch (schemaList[column].second) {
                    case TSDataType::TEXT:
                        tablet.addValue(column, rowIndex, result[row][column+1]);
                        break;
                    case TSDataType::BOOLEAN:
                        tablet.addValue(column, rowIndex, result[row][column+1].compare("true") == 0 ? true : false);
                        break;
                    case TSDataType::INT32:
                        tablet.addValue(column, rowIndex, stoi(result[row][column+1]));
                        break;
                    case TSDataType::INT64:
                        tablet.addValue(column, rowIndex, int64_t(stoll(result[row][column+1])));
                        break;
                    case TSDataType::FLOAT:
                        tablet.addValue(column, rowIndex, stof(result[row][column+1]));
                        break;
                    case TSDataType::DOUBLE:
                        tablet.addValue(column, rowIndex, stod(result[row][column+1]));
                        break;
                    default:
                        throw UnSupportedDataTypeException(string("Data type ") + to_string(schemaList[column].second) + " is not supported!!!");
                        break;
                }
            }
            // 每写入一条记录期待的数量就加1
            expectNum++;
        }
        tablet.setAligned(false);
        session1->insert(tablet);
        // 将Tablet重置为默认状态-将rowSize设置为0
        tablet.reset();
        // 验证合法性
        validity1("test_all_datatype_null_same_time_and_device", "t1",expectNum);

        // （2）写入各种类型数据（部分含空值）
        int64_t time = 1000;
        for (int row = 0; row < result.size(); row++) { // 遍历每行
            int rowIndex = tablet.rowSize++; // 增加并获取有效行
            tablet.timestamps[rowIndex] = time++; // 添加时间戳
            for (int column = 0; column < result[row].size() - 1; column++) { // 遍历每列（因为包含一列时间戳需要减少一列）
                switch (schemaList[column].second) {
                    case TSDataType::TEXT:
                        tablet.addValue(column, rowIndex, result[row][column+1]);
                        break;
                    case TSDataType::BOOLEAN:
                        tablet.addValue(column, rowIndex, result[row][column+1].compare("true") == 0 ? true : false);
                        break;
                    case TSDataType::INT32:
                        tablet.addValue(column, rowIndex, stoi(result[row][column+1]));
                        break;
                    case TSDataType::INT64:
                        tablet.addValue(column, rowIndex, int64_t(stoll(result[row][column+1])));
                        break;
                    case TSDataType::FLOAT:
                        tablet.addValue(column, rowIndex, stof(result[row][column+1]));
                        break;
                    case TSDataType::DOUBLE:
                        tablet.addValue(column, rowIndex, stod(result[row][column+1]));
                        break;
                    default:
                        throw UnSupportedDataTypeException(string("Data type ") + to_string(schemaList[column].second) + " is not supported!!!");
                        break;
                }
            }
            if (row % 2 == 0) {
                tablet.bitMaps[0].mark(row);
                tablet.bitMaps[1].mark(row);
                tablet.bitMaps[2].mark(row);
                tablet.bitMaps[3].mark(row);
                tablet.bitMaps[4].mark(row);
                tablet.bitMaps[5].mark(row);
                tablet.bitMaps[6].mark(row);
                tablet.bitMaps[7].mark(row);
                tablet.bitMaps[8].mark(row);
                tablet.bitMaps[9].mark(row);
            }
            // 每写入一条记录期待的数量就加1
            expectNum++;
        }
        tablet.setAligned(false);
        session1->insert(tablet);
        // 将Tablet重置为默认状态-将rowSize设置为0
        tablet.reset();

        //（3）写入空值（值为空）
        for (int row = 0; row < result.size(); row++) { // 遍历每行
            int rowIndex = tablet.rowSize++; // 增加并获取有效行
            tablet.timestamps[rowIndex] = time++; // 添加时间戳
            tablet.bitMaps[0].mark(row);
            tablet.bitMaps[1].mark(row);
            tablet.bitMaps[2].mark(row);
            tablet.bitMaps[3].mark(row);
            tablet.bitMaps[4].mark(row);
            tablet.bitMaps[5].mark(row);
            tablet.bitMaps[6].mark(row);
            tablet.bitMaps[7].mark(row);
            tablet.bitMaps[8].mark(row);
            tablet.bitMaps[9].mark(row);
            // 每写入一条记录期待的数量就加1
            expectNum++;
        }
        tablet.setAligned(false);
        session1->insert(tablet);
        // 将Tablet重置为默认状态-将rowSize设置为0
        tablet.reset();
        // 验证合法性
        validity1("test_all_datatype_null_same_time_and_device", "t1",expectNum);

        // （4）写入空值（时间戳为空）
        for (int row = 0; row < result.size(); row++) { // 遍历每行
            int rowIndex = tablet.rowSize++; // 增加并获取有效行
            for (int column = 0; column < result[row].size() - 1; column++) { // 遍历每列（因为包含一列时间戳需要减少一列）
                switch (schemaList[column].second) {
                    case TSDataType::TEXT:
                        tablet.addValue(column, rowIndex, result[row][column+1]);
                        break;
                    case TSDataType::BOOLEAN:
                        tablet.addValue(column, rowIndex, result[row][column+1].compare("true") == 0 ? true : false);
                        break;
                    case TSDataType::INT32:
                        tablet.addValue(column, rowIndex, stoi(result[row][column+1]));
                        break;
                    case TSDataType::INT64:
                        tablet.addValue(column, rowIndex, int64_t(stoll(result[row][column+1])));
                        break;
                    case TSDataType::FLOAT:
                        tablet.addValue(column, rowIndex, stof(result[row][column+1]));
                        break;
                    case TSDataType::DOUBLE:
                        tablet.addValue(column, rowIndex, stod(result[row][column+1]));
                        break;
                    default:
                        throw UnSupportedDataTypeException(string("Data type ") + to_string(schemaList[column].second) + " is not supported!!!");
                        break;
                }
            }
            // 每写入一条记录期待的数量就加1
            expectNum++;
        }
        session1->insert(tablet);
        // 将Tablet重置为默认状态-将rowSize设置为0
        tablet.reset();
        // 验证合法性
        validity1("test_all_datatype_null_same_time_and_device", "t1",expectNum);

        // （5）写入空值（全为空，空tablet）（注意需要在添加值之前执行，因为reset()值清空rowsize不清空之前添加的值）
        session1->insert(tablet);
        // 将Tablet重置为默认状态-将rowSize设置为0
        tablet.reset();
        // 验证合法性
        validity1("test_all_datatype_null_same_time_and_device", "t1",expectNum);

        // （6）写入部分值（其他必须要标记空值，否则会有默认值）
        for (int row = 0; row < result.size(); row++) { // 遍历每行
            int rowIndex = tablet.rowSize++; // 增加并获取有效行
            tablet.timestamps[rowIndex] = time++; // 添加时间戳
            for (int column = 0; column < result[row].size() - 1; column++) { // 遍历每列（因为包含一列时间戳需要减少一列）
                if (column % 2 ==0) { // 只允许写入部分值
                    switch (schemaList[column].second) {
                        case TSDataType::TEXT:
                            tablet.addValue(column, rowIndex, result[row][column+1]);
                            break;
                        case TSDataType::BOOLEAN:
                            tablet.addValue(column, rowIndex, result[row][column+1].compare("true") == 0 ? true : false);
                            break;
                        case TSDataType::INT32:
                            tablet.addValue(column, rowIndex, stoi(result[row][column+1]));
                            break;
                        case TSDataType::INT64:
                            tablet.addValue(column, rowIndex, int64_t(stoll(result[row][column+1])));
                            break;
                        case TSDataType::FLOAT:
                            tablet.addValue(column, rowIndex, stof(result[row][column+1]));
                            break;
                        case TSDataType::DOUBLE:
                            tablet.addValue(column, rowIndex, stod(result[row][column+1]));
                            break;
                        default:
                            throw UnSupportedDataTypeException(string("Data type ") + to_string(schemaList[column].second) + " is not supported!!!");
                            break;
                    }
                }
            }
            // 每写入一条记录期待的数量就加1
            expectNum++;
        }
        session1->insert(tablet);
        // 将Tablet重置为默认状态-将rowSize设置为0
        tablet.reset();
        // 验证合法性
        validity1("test_all_datatype_null_same_time_and_device", "t1",expectNum);
        
        // 查询
        // test_executeQueryStatement("t1");
    
    } catch (IoTDBException &e) {
        isErrorInsertTest = true;
        ASSERT_EQ(false, isErrorInsertTest) << "[Error] test_all_datatype_null_same_time_and_device - " << string(e.what());
    }
}

// 2、写入各种数据类型包含空值（不同时间分区和相同设备）
// TEST_F(InsertTest, test_all_datatype_nonsame_time_and_same_device) {
    
//     try{
//         // 构建Tablet
//         vector<pair<string, TSDataType::TSDataType>> schemaList {
//             make_pair("Tag1", TSDataType::TEXT),
//             make_pair("tag2", TSDataType::TEXT),
//             make_pair("Attr1", TSDataType::TEXT),
//             make_pair("attr2", TSDataType::TEXT),
//             make_pair("S1", TSDataType::BOOLEAN),
//             make_pair("s2", TSDataType::INT32),
//             make_pair("S3", TSDataType::INT64),
//             make_pair("s4", TSDataType::FLOAT),
//             make_pair("S5", TSDataType::DOUBLE),
//             make_pair("s6", TSDataType::TEXT)
//         };
//         vector<ColumnCategory> columnTypes = {
//             ColumnCategory::TAG,
//             ColumnCategory::TAG,
//             ColumnCategory::ATTRIBUTE,
//             ColumnCategory::ATTRIBUTE,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD
//         };
//         Tablet tablet("t1", schemaList, columnTypes, 100);

//         // 获取数据
//         auto [result, err] = parseCSVFile("../../../data/insert_data2.csv");

//         // （1）写入各种类型数据（不含空值）
//         for (int row = 0; row < result.size(); row++) { // 遍历每行
//             int rowIndex = tablet.rowSize++; // 增加并获取有效行
//             tablet.timestamps[rowIndex] = stoll(result[row][0]); // 添加时间戳
//             for (int column = 0; column < result[row].size() - 1; column++) { // 遍历每列（因为包含一列时间戳需要减少一列）
//                 switch (schemaList[column].second) {
//                     case TSDataType::TEXT:
//                         tablet.addValue(column, rowIndex, result[row][column+1]);
//                         break;
//                     case TSDataType::BOOLEAN:
//                         tablet.addValue(column, rowIndex, result[row][column+1].compare("true") == 0 ? true : false);
//                         break;
//                     case TSDataType::INT32:
//                         tablet.addValue(column, rowIndex, stoi(result[row][column+1]));
//                         break;
//                     case TSDataType::INT64:
//                         tablet.addValue(column, rowIndex, int64_t(stoll(result[row][column+1])));
//                         break;
//                     case TSDataType::FLOAT:
//                         tablet.addValue(column, rowIndex, stof(result[row][column+1]));
//                         break;
//                     case TSDataType::DOUBLE:
//                         tablet.addValue(column, rowIndex, stod(result[row][column+1]));
//                         break;
//                     default:
//                         throw UnSupportedDataTypeException(string("Data type ") + to_string(schemaList[column].second) + " is not supported!!!");
//                         break;
//                 }
//             }
//             // 每写入一条记录期待的数量就加1
//             expectNum++;
//         }
//         tablet.setAligned(false);
//         session1->insert(tablet);
//         // 将Tablet重置为默认状态-将rowSize设置为0
//         tablet.reset();
//         // 验证合法性
//         validity1("test_all_datatype_nonsame_time_and_same_device", "t1",expectNum);
//         // 查询
//         test_executeQueryStatement("t1");
    
//     } catch (IoTDBException &e) {
//         isErrorInsertTest = true;
//         ASSERT_EQ(false, isErrorInsertTest) << "[Error] test_all_datatype_nonsame_time_and_same_device - " << string(e.what());
//     }
// }

// // 5、自动创建（因为tag和ATTRIBUTE列只有text类型，所以自动创建会报 DataType of TAG Column should only be STRING, current is TEXT）
// TEST_F(InsertTest, test_auto_insert) {
//     try{
//         // 构建Tablet
//         vector<pair<string, TSDataType::TSDataType>> schemaList {
//             make_pair("Tag1", TSDataType::TEXT),
//             make_pair("tag2", TSDataType::TEXT),
//             make_pair("Attr1", TSDataType::TEXT),
//             make_pair("attr2", TSDataType::TEXT),
//             make_pair("S1", TSDataType::BOOLEAN),
//             make_pair("s2", TSDataType::INT32),
//             make_pair("S3", TSDataType::INT64),
//             make_pair("s4", TSDataType::FLOAT),
//             make_pair("S5", TSDataType::DOUBLE),
//             make_pair("s6", TSDataType::TEXT)
//         };
//         vector<ColumnCategory> columnTypes = {
//             ColumnCategory::TAG,
//             ColumnCategory::TAG,
//             ColumnCategory::ATTRIBUTE,
//             ColumnCategory::ATTRIBUTE,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD,
//             ColumnCategory::FIELD
//         };
//         Tablet tablet(nullptr, schemaList, columnTypes, 100);

//         // 获取数据
//         auto [result, err] = parseCSVFile("../../../data/insert_data1.csv");

//         // （1）写入各种类型数据（不含空值）
//         for (int row = 0; row < result.size(); row++) { // 遍历每行
//             int rowIndex = tablet.rowSize++; // 增加并获取有效行
//             tablet.timestamps[rowIndex] = stoll(result[row][0]); // 添加时间戳
//             for (int column = 0; column < result[row].size() - 1; column++) { // 遍历每列（因为包含一列时间戳需要减少一列）
//                 switch (schemaList[column].second) {
//                     case TSDataType::TEXT:
//                         tablet.addValue(column, rowIndex, result[row][column+1]);
//                         break;
//                     case TSDataType::BOOLEAN:
//                         tablet.addValue(column, rowIndex, result[row][column+1].compare("true") == 0 ? true : false);
//                         break;
//                     case TSDataType::INT32:
//                         tablet.addValue(column, rowIndex, stoi(result[row][column+1]));
//                         break;
//                     case TSDataType::INT64:
//                         tablet.addValue(column, rowIndex, int64_t(stoll(result[row][column+1])));
//                         break;
//                     case TSDataType::FLOAT:
//                         tablet.addValue(column, rowIndex, stof(result[row][column+1]));
//                         break;
//                     case TSDataType::DOUBLE:
//                         tablet.addValue(column, rowIndex, stod(result[row][column+1]));
//                         break;
//                     default:
//                         throw UnSupportedDataTypeException(string("Data type ") + to_string(schemaList[column].second) + " is not supported!!!");
//                         break;
//                 }
//             }
//             // 每写入一条记录期待的数量就加1
//             expectNum++;
//         }
//         tablet.setAligned(false);
//         session1->insert(tablet);
//         // 将Tablet重置为默认状态-将rowSize设置为0
//         tablet.reset();
//         // 验证合法性
//         validity1("test_all_datatype_null_same_time_and_device", "t2",expectNum);
//     } catch (IoTDBException &e) {
//         isErrorInsertTest = true;
//         ASSERT_EQ(false, isErrorInsertTest) << "[Error] test_all_datatype_null_same_time_and_device - " << string(e.what());
//     }
// }