#include "Common.h"
#include "Session.h"
#include "TableSession.h"
#include "TableSessionBuilder.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include "gtest/gtest.h"
#include <string>
#include <fstream>
#include <boost/optional/optional_io.hpp>

/*
 * 测试用例标题：测试树模型查询操作
 * 作者：肖林捷
 * 最新编写日期：2025/6
 */

using namespace std;

Session *treeSession_TreeSelect; // 作为全局的session变量
bool isErrorTest_TreeSelect = false; // 用于确认是否出错的标识
int expectNum_TreeSelect = 0; // 用于记录插入期待的行数
int actualNum_TreeSelect = 0; // 用于记录实际查询的行数
static string DATABASE_NAME_TreeSelect = "root.test_select"; // 数据库名
static string DEVICE_ID_TreeSelect = DATABASE_NAME_TreeSelect + ".d1"; // 设备名 root.test_select.d1
vector<string> columnNames_TreeSelect = { // 测点名
    "boolean", "int32", "int64", "float", "double", "text", "string", "blob", "date", "ts" // FIELD列
};
vector<std::string> paths_TreeSelect = { // 测点路径
    DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[0], DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[1], DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[2], DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[3], DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[4], DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[5], DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[6], DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[7], DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[8], DEVICE_ID_TreeSelect + "." + columnNames_TreeSelect[9]
};
vector<TSDataType::TSDataType> dataTypes_TreeSelect = { // 测点数据类型
    TSDataType::BOOLEAN, TSDataType::INT32, TSDataType::INT64, TSDataType::FLOAT, TSDataType::DOUBLE, TSDataType::TEXT, TSDataType::STRING, TSDataType::BLOB, TSDataType::DATE, TSDataType::TIMESTAMP
}; 
vector<TSEncoding::TSEncoding> encodings_TreeSelect = { // 测点编码方式
    TSEncoding::PLAIN, TSEncoding::PLAIN, TSEncoding::PLAIN, TSEncoding::PLAIN, TSEncoding::PLAIN, TSEncoding::PLAIN, TSEncoding::PLAIN, TSEncoding::PLAIN, TSEncoding::PLAIN, TSEncoding::PLAIN
}; 
vector<CompressionType::CompressionType> compressors_TreeSelect = { // 测点压缩方式
    CompressionType::LZ4, CompressionType::LZ4, CompressionType::LZ4, CompressionType::LZ4, CompressionType::LZ4, CompressionType::LZ4, CompressionType::LZ4, CompressionType::LZ4, CompressionType::LZ4, CompressionType::LZ4
}; 

// 解析CVS文件，获取数据
pair<vector<vector<string>>, string> parseCSVFile_TreeSelect(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return {{}, "Failed to open file"};
    }

    vector<vector<string>> result; // 用于存储最终结果
    string line;

    while (getline(file, line)) {
        // 跳过注释行
        if (!line.empty() && line[0] == '#') {
            continue;
        }

        vector<string> fields;
        stringstream ss(line);
        string field;

        // 以逗号分隔每行数据
        while (getline(ss, field, ',')) {
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

// 插入各种类型数据（包含空值、最大值和最小值，同时间分区，10000行）
void insertDate_TreeSelect() {
    try{
        vector<pair<string, TSDataType::TSDataType>> schemaList;
        for (int i =0; i < columnNames_TreeSelect.size(); i++) {
            schemaList.push_back(make_pair(columnNames_TreeSelect[i],dataTypes_TreeSelect[i]));
        }
        Tablet tablet(DEVICE_ID_TreeSelect, schemaList, 10000);
        auto [result, err] = parseCSVFile_TreeSelect("../../../data/tree/select_data1.csv");
        for (int row = 0; row < result.size(); row++) {
            int rowIndex = tablet.rowSize++;
            tablet.timestamps[rowIndex] = stoll(result[row][0]);
            for (int column = 0; column < result[row].size() - 1; column++) {
                if (result[row][column+1] == "null") {
                    switch (dataTypes_TreeSelect[column]) {
                        case TSDataType::TEXT:
                        case TSDataType::STRING:
                        case TSDataType::BLOB:
                            tablet.addValue(column, rowIndex, "NULL");
                            break;
                        case TSDataType::BOOLEAN:
                            tablet.addValue(column, rowIndex, false);
                            break;
                        case TSDataType::INT32:
                            tablet.addValue(column, rowIndex, 0);
                            break;
                        case TSDataType::INT64:
                        case TSDataType::TIMESTAMP:
                            tablet.addValue(column, rowIndex, int64_t(0));
                            break;
                        case TSDataType::FLOAT:
                            tablet.addValue(column, rowIndex, static_cast<float>(0.0));
                            break;
                        case TSDataType::DOUBLE:
                            tablet.addValue(column, rowIndex, 0.0);
                           break;
                        case TSDataType::DATE:
                            tablet.addValue(column, rowIndex, boost::gregorian::date(2025, 5, 15));
                            break;
                        default:
                            throw UnSupportedDataTypeException(string("Data type ") + to_string(schemaList[column].second) + " is not supported!!!");
                            break;
                    }
                    tablet.bitMaps[column].mark(row);
                } else {
                    switch (dataTypes_TreeSelect[column]) {
                        case TSDataType::TEXT:
                        case TSDataType::STRING:
                        case TSDataType::BLOB:
                            tablet.addValue(column, rowIndex, result[row][column+1]);
                            break;
                        case TSDataType::BOOLEAN:
                            tablet.addValue(column, rowIndex, result[row][column+1].compare("true") == 0 ? true : false);
                            break;
                        case TSDataType::INT32:
                            tablet.addValue(column, rowIndex, stoi(result[row][column+1]));
                            break;
                        case TSDataType::INT64:
                        case TSDataType::TIMESTAMP:
                            tablet.addValue(column, rowIndex, int64_t(stoll(result[row][column+1])));
                            break;
                        case TSDataType::FLOAT:
                            tablet.addValue(column, rowIndex, stof(result[row][column+1]));
                            break;
                        case TSDataType::DOUBLE:
                            tablet.addValue(column, rowIndex, stod(result[row][column+1]));
                            break;
                        case TSDataType::DATE:
                            tablet.addValue(column, rowIndex, boost::gregorian::from_string(result[row][column+1]));
                            break;
                        default:
                            throw UnSupportedDataTypeException(string("Data type ") + to_string(schemaList[column].second) + " is not supported!!!");
                            break;
                    }
                }
            }
            // 每写入一条记录期待的数量就加1
            expectNum_TreeSelect++;
        }
        tablet.setAligned(false);
        treeSession_TreeSelect->insertTablet(tablet);
    }  catch (IoTDBConnectionException &e) {
        isErrorTest_TreeSelect = true;
        ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
    } catch (IoTDBException &e) {
        isErrorTest_TreeSelect = true;
        ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBException - " << string(e.what());
    } catch (exception &e) {
        isErrorTest_TreeSelect = true;
        ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] exception - " << string(e.what());
    }
}


// 测试组件：用于初始化环境和清理环境
class TreeSelectTest : public ::testing::Test {
    protected:
        // 在每个测试用例执行之前调用
        void SetUp() override {
            // 初始化操作
            try {
                treeSession_TreeSelect = new Session("127.0.0.1", 6667, "root", "root");
                treeSession_TreeSelect->open(false);
                treeSession_TreeSelect->executeNonQueryStatement("CREATE DATABASE " + DATABASE_NAME_TreeSelect);
                treeSession_TreeSelect->createMultiTimeseries(paths_TreeSelect, dataTypes_TreeSelect, encodings_TreeSelect, compressors_TreeSelect, NULL, NULL, NULL, NULL);
                insertDate_TreeSelect();
            }  catch (IoTDBConnectionException &e) {
                isErrorTest_TreeSelect = true;
                ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
            } catch (IoTDBException &e) {
                isErrorTest_TreeSelect = true;
                ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBException - " << string(e.what());
            } catch (exception &e) {
                isErrorTest_TreeSelect = true;
                ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] exception - " << string(e.what());
            }
        }
        // 在每个测试用例执行之后调用
        void TearDown() override {
            // 清理操作
            expectNum_TreeSelect = 0;
            try {
                treeSession_TreeSelect->executeNonQueryStatement("delete database root.**");
                treeSession_TreeSelect->close();
            }  catch (IoTDBConnectionException &e) {
                isErrorTest_TreeSelect = true;
                ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
            } catch (IoTDBException &e) {
                isErrorTest_TreeSelect = true;
                ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBException - " << string(e.what());
            }  catch (exception &e) {
                isErrorTest_TreeSelect = true;
                ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] exception - " << string(e.what());
            }
        }
};


// |------------ Normal Test ------------|
// 一、验证executeQueryStatement和RowRecord的Field查询：包含SessionDataSet类查询返回的列名、数据类型和行数正确性和其他方法可用性，RowRecord的Field查询值的正确性（注意：目前表模型使用Field查询存在问题）
TEST_F(TreeSelectTest, TestSelect1) {
    try {
        unique_ptr<SessionDataSet> dataSet = treeSession_TreeSelect->executeQueryStatement("select boolean,int32,int64,float,double,text,string,blob,date,ts from " + DEVICE_ID_TreeSelect + " order by time");
        // 1、验证列名正确性
        auto getColumnNames = dataSet->getColumnNames();
        for (int i = 0; i < paths_TreeSelect.size(); i++) {
            ASSERT_EQ(paths_TreeSelect[i], getColumnNames[i + 1]) << "[TestSelect1 FAIL] Expected column name and actual column name are inconsistent," << " Expected: " << paths_TreeSelect[i] << ", Actual: " << getColumnNames[i] << std::endl;
        }
        // 2、验证数据类型正确性
        auto getColumnTypeList = dataSet->getColumnTypeList();
        for (int i = 0; i < dataTypes_TreeSelect.size(); i++) {
            switch (dataTypes_TreeSelect[i]) {
                case 0:
                    ASSERT_EQ("BOOLEAN", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: BOOLEAN" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 1:
                    ASSERT_EQ("INT32", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: INT32" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 2:
                    ASSERT_EQ("INT64", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: INT64" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 3:
                    ASSERT_EQ("FLOAT", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: FLOAT" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 4:
                    ASSERT_EQ("DOUBLE", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: DOUBLE" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 5:
                    ASSERT_EQ("TEXT", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: TEXT" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 8:
                    ASSERT_EQ("TIMESTAMP", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: TIMESTAMP" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 9:
                    ASSERT_EQ("DATE", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: DATE" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 10:
                    ASSERT_EQ("BLOB", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: BOBLOBOLEAN" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 11:
                    ASSERT_EQ("STRING", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: STRING" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                default:
                    isErrorTest_TreeSelect = true;
                    ASSERT_EQ(false, isErrorTest_TreeSelect) << "[TestSelect1 FAIL] Data type " << dataTypes_TreeSelect[i] << " is not supported!!!";
                    break;
            }
        }
        // 3、验证行数正确性（表模型无法正确获取，树模型可以）
        while (dataSet->hasNext()) {
            // cout << dataSet->next()->toString() << "";
            dataSet->next()->toString();
            actualNum_TreeSelect++;
        }
        ASSERT_EQ(expectNum_TreeSelect, actualNum_TreeSelect) << "[TestSelect1 FAIL] Expected num and actual num are inconsistent"
        << " Expected: " << expectNum_TreeSelect << ", Actual: " << actualNum_TreeSelect << std::endl;
        // 4、验证值正确性（表模型无法正确获取，树模型可以）
        unique_ptr<SessionDataSet> dataSet2 = treeSession_TreeSelect->executeQueryStatement("select boolean,int32,int64,float,double,text,string,blob,date,ts from " + DEVICE_ID_TreeSelect + " order by time");
        while (dataSet2->hasNext()) {
            auto rowRecord = dataSet2->next();
            cout << rowRecord->timestamp << " ";  
            cout << rowRecord->fields[0].boolV.value() << " ";
            cout << rowRecord->fields[1].intV.value() << " ";
            cout << rowRecord->fields[2].longV.value() << " ";
            cout << rowRecord->fields[3].floatV.value() << " ";
            cout << rowRecord->fields[4].doubleV.value() << " ";
            cout << rowRecord->fields[5].stringV.value() << " ";
            cout << rowRecord->fields[6].stringV.value() << " ";
            cout << rowRecord->fields[7].stringV.value() << " ";
            cout << rowRecord->fields[8].dateV.value() << " ";
            cout << rowRecord->fields[9].longV.value() << endl;
        }
        // 5、验证值其他方法可用性
        dataSet->setFetchSize(1024);
        ASSERT_EQ(1024, dataSet->getFetchSize()) << "[TestSelect1 FAIL] Expected fetch size and actual fetch size are inconsistent"
        << " Expected: " << expectNum_TreeSelect << ", Actual: " << actualNum_TreeSelect << std::endl;
        dataSet->closeOperationHandle();
    }  catch (IoTDBConnectionException &e) {
        isErrorTest_TreeSelect = true;
        ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
    } catch (IoTDBException &e) {
        isErrorTest_TreeSelect = true;
        ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBException - " << string(e.what());
    }  catch (exception &e) {
        isErrorTest_TreeSelect = true;
        ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] exception - " << string(e.what());
    }
}

// 二、验证DataIterator查询：包含返回列名、数据类型和值正确性、同种数据类型通过索引和通过列名返回值一致性、是否为空值判断的正确性和其他方法可用性
TEST_F(TreeSelectTest, TestSelect2) {
    try {
        unique_ptr<SessionDataSet> dataSet1 = treeSession_TreeSelect->executeQueryStatement("select boolean,int32,int64,float,double,text,string,blob,date,ts from " + DEVICE_ID_TreeSelect + " order by time");
        auto dataIterator1 = dataSet1->getIterator();
        // 1、验证列名正确性
        auto getColumnNames = dataIterator1.getColumnNames();
        for (int i = 0; i < paths_TreeSelect.size(); i++) {
            ASSERT_EQ(paths_TreeSelect[i], getColumnNames[i + 1]) << "[TestSelect1 FAIL] Expected column name and actual column name are inconsistent," << " Expected: " << paths_TreeSelect[i] << ", Actual: " << getColumnNames[i] << std::endl;
        }
        // 2、验证数据类型正确性
        auto getColumnTypeList = dataIterator1.getColumnTypeList();
        for (int i = 0; i < dataTypes_TreeSelect.size(); i++) {
            switch (dataTypes_TreeSelect[i]) {
                case 0:
                    ASSERT_EQ("BOOLEAN", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: BOOLEAN" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 1:
                    ASSERT_EQ("INT32", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: INT32" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 2:
                    ASSERT_EQ("INT64", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: INT64" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 3:
                    ASSERT_EQ("FLOAT", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: FLOAT" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 4:
                    ASSERT_EQ("DOUBLE", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: DOUBLE" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 5:
                    ASSERT_EQ("TEXT", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: TEXT" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 8:
                    ASSERT_EQ("TIMESTAMP", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: TIMESTAMP" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 9:
                    ASSERT_EQ("DATE", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: DATE" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 10:
                    ASSERT_EQ("BLOB", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: BOBLOBOLEAN" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 11:
                    ASSERT_EQ("STRING", getColumnTypeList[i + 1]) << "[TestSelect1 FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: STRING" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                default:
                    isErrorTest_TreeSelect = true;
                    ASSERT_EQ(false, isErrorTest_TreeSelect) << "[TestSelect1 FAIL] Data type " << dataTypes_TreeSelect[i] << " is not supported!!!";
                    break;
            }
        }
        // 3、验证返回值正确性
        auto [result, err] = parseCSVFile_TreeSelect("../../../data/tree/select_data1.csv");
        for (int row = 0; row < result.size(); row++) {
            dataIterator1.next();
            ASSERT_EQ(dataIterator1.getLongByIndex(1), int64_t(stoll(result[row][0]))) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: "<< dataIterator1.getLongByIndex(1) <<", Actual: " << int64_t(stoll(result[row][0])) << std::endl;
            for (int column = 0; column < result[row].size() - 1; column++) {
                if (result[row][column+1] != "null") {
                    switch (dataTypes_TreeSelect[column]) {
                        case TSDataType::TEXT:
                        case TSDataType::STRING:
                        case TSDataType::BLOB:
                            ASSERT_EQ(dataIterator1.getStringByIndex(column+2), result[row][column+1]) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getStringByIndex(column+2) << ", Actual: " << result[row][column+1] << std::endl;
                            break;
                        case TSDataType::BOOLEAN:
                            ASSERT_EQ(dataIterator1.getBooleanByIndex(column+2), ((result[row][column+1].compare("true") == 0) ? true : false)) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getBooleanByIndex(column+2) << ", Actual: " << ((result[row][column+1].compare("true") == 0) ? true : false) << std::endl;
                            break;
                        case TSDataType::INT32:
                            ASSERT_EQ(dataIterator1.getIntByIndex(column+2), stoi(result[row][column+1])) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getIntByIndex(column+2) << ", Actual: " << stoi(result[row][column+1]) << std::endl;
                            break;
                        case TSDataType::INT64:
                            ASSERT_EQ(dataIterator1.getLongByIndex(column+2), int64_t(stoll(result[row][column+1]))) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getLongByIndex(column+2) << ", Actual: " << int64_t(stoll(result[row][column+1])) << std::endl;
                            break;
                        case TSDataType::TIMESTAMP:
                            ASSERT_EQ(dataIterator1.getTimestampByIndex(column+2), int64_t(stoll(result[row][column+1]))) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getTimestampByIndex(column+2) << ", Actual: " << int64_t(stoll(result[row][column+1])) << std::endl;
                            break;
                        case TSDataType::FLOAT:
                            ASSERT_EQ(dataIterator1.getFloatByIndex(column+2), stof(result[row][column+1])) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getFloatByIndex(column+2) << ", Actual: " << stof(result[row][column+1]) << std::endl;
                            break;
                        case TSDataType::DOUBLE:
                            ASSERT_EQ(dataIterator1.getDoubleByIndex(column+2), stod(result[row][column+1])) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getDoubleByIndex(column+2) << ", Actual: " << stod(result[row][column+1]) << std::endl;
                            break;
                        case TSDataType::DATE:
                            ASSERT_EQ(dataIterator1.getDateByIndex(column+2), boost::gregorian::from_string(result[row][column+1])) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getDateByIndex(column+2) << ", Actual: " << boost::gregorian::from_string(result[row][column+1]) << std::endl;
                            break;
                        default:
                            isErrorTest_TreeSelect = true;
                            ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBException - Data type "<< dataTypes_TreeSelect[column] << " is not supported!!!";
                            break;
                    }
                }
            }
        }
        // 4、验证同种数据类型通过索引和通过列名返回值一致性
        unique_ptr<SessionDataSet> dataSet2 = treeSession_TreeSelect->executeQueryStatement("select boolean,int32,int64,float,double,text,string,blob,date,ts from " + DEVICE_ID_TreeSelect + " order by time");
        auto dataIterator2 = dataSet2->getIterator();
        while (dataIterator2.next()) {
            ASSERT_EQ(dataIterator2.getLongByIndex(1), dataIterator2.getLong("Time")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(1) << ", Actual: " << dataIterator2.getLong("Time") << std::endl;
            ASSERT_EQ(dataIterator2.getBooleanByIndex(2), dataIterator2.getBoolean("root.test_select.d1.boolean")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getBooleanByIndex(2) << ", Actual: " << dataIterator2.getBoolean("root.test_select.d1.boolean") << std::endl;
            ASSERT_EQ(dataIterator2.getIntByIndex(3), dataIterator2.getInt("root.test_select.d1.int32")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getIntByIndex(3) << ", Actual: " << dataIterator2.getInt("root.test_select.d1.int32") << std::endl;
            ASSERT_EQ(dataIterator2.getLongByIndex(4), dataIterator2.getLong("root.test_select.d1.int64")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(4) << ", Actual: " << dataIterator2.getLong("root.test_select.d1.int64") << std::endl;
            ASSERT_EQ(dataIterator2.getFloatByIndex(5), dataIterator2.getFloat("root.test_select.d1.float")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getFloatByIndex(5) << ", Actual: " << dataIterator2.getFloat("root.test_select.d1.float") << std::endl;
            ASSERT_EQ(dataIterator2.getDoubleByIndex(6), dataIterator2.getDouble("root.test_select.d1.double")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDoubleByIndex(6) << ", Actual: " << dataIterator2.getDouble("root.test_select.d1.double") << std::endl;
            ASSERT_EQ(dataIterator2.getStringByIndex(7), dataIterator2.getString("root.test_select.d1.text")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(7) << ", Actual: " << dataIterator2.getString("root.test_select.d1.text") << std::endl;
            ASSERT_EQ(dataIterator2.getStringByIndex(8), dataIterator2.getString("root.test_select.d1.string")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(8) << ", Actual: " << dataIterator2.getString("root.test_select.d1.string") << std::endl;
            ASSERT_EQ(dataIterator2.getStringByIndex(9), dataIterator2.getString("root.test_select.d1.blob")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(9) << ", Actual: " << dataIterator2.getString("root.test_select.d1.blob") << std::endl;
            ASSERT_EQ(dataIterator2.getDateByIndex(10), dataIterator2.getDate("root.test_select.d1.date")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDateByIndex(10) << ", Actual: " << dataIterator2.getDate("root.test_select.d1.date") << std::endl;
            ASSERT_EQ(dataIterator2.getTimestampByIndex(11), dataIterator2.getTimestamp("root.test_select.d1.ts")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getTimestampByIndex(11) << ", Actual: " << dataIterator2.getTimestamp("root.test_select.d1.ts") << std::endl;
        }
        // 5、验证是否为空值判断的正确性
        unique_ptr<SessionDataSet> dataSet3 = treeSession_TreeSelect->executeQueryStatement("select boolean,int32,int64,float,double,text,string,blob,date,ts from " + DEVICE_ID_TreeSelect + " order by time");
        auto dataIterator3 = dataSet3->getIterator();
        while (dataIterator3.next()) {
            ASSERT_EQ(dataIterator3.isNullByIndex(1), dataIterator3.isNull("Time")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(1) << ", Actual: " << dataIterator3.isNull("Time") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(2), dataIterator3.isNull("root.test_select.d1.boolean")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(2) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.boolean") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(3), dataIterator3.isNull("root.test_select.d1.int32")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(3) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.int32") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(4), dataIterator3.isNull("root.test_select.d1.int64")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(4) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.int64") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(5), dataIterator3.isNull("root.test_select.d1.float")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(5) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.float") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(6), dataIterator3.isNull("root.test_select.d1.double")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(6) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.double") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(7), dataIterator3.isNull("root.test_select.d1.text")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(7) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.text") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(8), dataIterator3.isNull("root.test_select.d1.string")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(8) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.string") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(9), dataIterator3.isNull("root.test_select.d1.blob")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(9) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.blob") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(10), dataIterator3.isNull("root.test_select.d1.date")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(10) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.date") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(11), dataIterator3.isNull("root.test_select.d1.ts")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(11) << ", Actual: " << dataIterator3.isNull("root.test_select.d1.ts") << std::endl;
        }
        // 6、验证其他方法可用性：findColumn
        unique_ptr<SessionDataSet> dataSet4 = treeSession_TreeSelect->executeQueryStatement("select boolean,int32,int64,float,double,text,string,blob,date,ts from " + DEVICE_ID_TreeSelect + " order by time");
        auto dataIterator4 = dataSet4->getIterator();
        ASSERT_EQ(1, dataIterator3.findColumn("Time")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: 1, Actual: " << dataIterator3.findColumn("Time") << std::endl;
        ASSERT_EQ(2, dataIterator3.findColumn("root.test_select.d1.boolean")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: 2, Actual: " << dataIterator3.findColumn("root.test_select.d1.boolean") << std::endl;
    }  catch (IoTDBConnectionException &e) {
        isErrorTest_TreeSelect = true;
        ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
    } catch (IoTDBException &e) {
        isErrorTest_TreeSelect = true;
        ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] IoTDBException - " << string(e.what());
    }  catch (exception &e) {
        isErrorTest_TreeSelect = true;
        ASSERT_EQ(false, isErrorTest_TreeSelect) << "[Error] exception - " << string(e.what());
    }
}