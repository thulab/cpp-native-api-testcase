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
 * 测试用例标题：测试表模型查询操作
 * 作者：肖林捷
 * 最新编写日期：2025/6
 */

using namespace std;

TableSession *session_select_test; // 作为全局的session变量
bool isErrorTest_TableSelect = false; // 用于确认是否出错的标识
int expectNum_TableSelect = 0; // 用于记录插入期待的行数
int actualNum_TableSelect = 0; // 用于记录实际查询的行数
static string DATABASE_NAME_TableSelect = "test_select"; // 数据库名
static string TABLE_NAME_TableSelect = "t1"; // 表名
vector<string> columnNames_TableSelect = { // 列名
    "TAG1", "TAG2",
    "ATTR1", "ATTR2",
    "BOOLEAN", "INT32", "INT64", "FLOAT", "DOUBLE", "TEXT", "STRING", "BLOB", "DATE", "TIMESTAMP" // FIELD列
};
vector<TSDataType::TSDataType> dataTypes_TableSelect = { // 数据类型
    TSDataType::STRING, TSDataType::STRING,
    TSDataType::STRING, TSDataType::STRING,
    TSDataType::BOOLEAN, TSDataType::INT32, TSDataType::INT64, TSDataType::FLOAT, TSDataType::DOUBLE, TSDataType::TEXT, TSDataType::STRING, TSDataType::BLOB, TSDataType::DATE, TSDataType::TIMESTAMP // FIELD列
};
vector<ColumnCategory> columnTypes_TableSelect = { // 列类别
    ColumnCategory::TAG,ColumnCategory::TAG,
    ColumnCategory::ATTRIBUTE,ColumnCategory::ATTRIBUTE,
    ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD
};

// 解析CVS文件，获取数据
pair<vector<vector<string>>, string> parseCSVFile_TableSelect(const string& filename) {
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
void insertDate_TableSelect() {
    try{
        vector<pair<string, TSDataType::TSDataType>> schemaList;
        for (int i =0; i < columnNames_TableSelect.size(); i++) {
            schemaList.push_back(make_pair(columnNames_TableSelect[i],dataTypes_TableSelect[i]));
        }
        Tablet tablet(TABLE_NAME_TableSelect, schemaList, columnTypes_TableSelect, 10000);
        auto [result, err] = parseCSVFile_TableSelect("../../data/table/select_data1.csv");
        for (int row = 0; row < result.size(); row++) {
            int rowIndex = tablet.rowSize++;
            tablet.timestamps[rowIndex] = stoll(result[row][0]);
            for (int column = 0; column < result[row].size() - 1; column++) {
                if (result[row][column+1] == "null") {
                    switch (dataTypes_TableSelect[column]) {
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
                    switch (dataTypes_TableSelect[column]) {
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
            expectNum_TableSelect++;
        }
        tablet.setAligned(false);
        session_select_test->insert(tablet);
    }  catch (IoTDBConnectionException &e) {
        isErrorTest_TableSelect = true;
        ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
    } catch (IoTDBException &e) {
        isErrorTest_TableSelect = true;
        ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBException - " << string(e.what());
    } catch (exception &e) {
        isErrorTest_TableSelect = true;
        ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] exception - " << string(e.what());
    }
}


// 测试组件：用于初始化环境和清理环境
class TableSelectTest : public ::testing::Test {
    protected:
        // 在每个测试用例执行之前调用
        void SetUp() override {
            // 初始化操作
            try {
                session_select_test = (new TableSessionBuilder())
                    ->host("127.0.0.1")
                    ->rpcPort(6667)
                    ->username("root")
                    ->password("root")
                    ->build();
                session_select_test->executeNonQueryStatement("CREATE DATABASE " + DATABASE_NAME_TableSelect);
                session_select_test->executeNonQueryStatement("use " + DATABASE_NAME_TableSelect);
                session_select_test->executeNonQueryStatement("CREATE TABLE " + TABLE_NAME_TableSelect + " (tag1 STRING TAG, tag2 STRING TAG, attr1 STRING ATTRIBUTE, attr2 STRING ATTRIBUTE, BOOLEAN BOOLEAN FIELD, INT32 INT32 FIELD, INT64 INT64 FIELD, FLOAT FLOAT FIELD, DOUBLE DOUBLE FIELD, TEXT TEXT FIELD, STRING STRING FIELD, BLOB BLOB FIELD, DATE DATE FIELD, TIMESTAMP TIMESTAMP FIELD)");
                insertDate_TableSelect();
            }  catch (IoTDBConnectionException &e) {
                isErrorTest_TableSelect = true;
                ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
            } catch (IoTDBException &e) {
                isErrorTest_TableSelect = true;
                ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBException - " << string(e.what());
            } catch (exception &e) {
                isErrorTest_TableSelect = true;
                ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] exception - " << string(e.what());
            }
        }
        // 在每个测试用例执行之后调用
        void TearDown() override {
            // 清理操作
            expectNum_TableSelect = 0;
            try {
                session_select_test->executeNonQueryStatement("drop DATABASE " + DATABASE_NAME_TableSelect);
                session_select_test->close();
            }  catch (IoTDBConnectionException &e) {
                isErrorTest_TableSelect = true;
                ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
            } catch (IoTDBException &e) {
                isErrorTest_TableSelect = true;
                ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBException - " << string(e.what());
            }  catch (exception &e) {
                isErrorTest_TableSelect = true;
                ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] exception - " << string(e.what());
            }
        }
};


// |------------ Normal Test ------------|

// 一、验证executeQueryStatement和RowRecord的Field查询：包含SessionDataSet类查询返回的列名、数据类型和行数正确性和其他方法可用性，RowRecord的Field查询值的正确性（注意：目前表模型使用Field查询存在问题）
TEST_F(TableSelectTest, TestSelect1) {
    try {
        unique_ptr<SessionDataSet> dataSet = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + TABLE_NAME_TableSelect + " order by time");
        // 1、验证列名正确性
        auto getColumnNames = dataSet->getColumnNames();
        for (int i = 0; i < columnNames_TableSelect.size(); i++) {
            std::transform(columnNames_TableSelect[i].begin(), columnNames_TableSelect[i].end(), columnNames_TableSelect[i].begin(),[](unsigned char c) { return std::tolower(c); });
            ASSERT_EQ(columnNames_TableSelect[i], getColumnNames[i + 1]) << "[TestSelect1 FAIL] Expected column name and actual column name are inconsistent," << " Expected: " << columnNames_TableSelect[i] << ", Actual: " << getColumnNames[i] << std::endl;
        }
        // 2、验证数据类型正确性
        auto getColumnTypeList = dataSet->getColumnTypeList();
        for (int i = 0; i < dataTypes_TableSelect.size(); i++) {
            switch (dataTypes_TableSelect[i]) {
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
                    isErrorTest_TableSelect = true;
                    ASSERT_EQ(false, isErrorTest_TableSelect) << "[TestSelect1 FAIL] Data type " << dataTypes_TableSelect[i] << " is not supported!!!";
                    break;
            }
        }
        // 3、验证行数正确性（表模型无法正确获取，树模型可以）
        while (dataSet->hasNext()) {
            dataSet->next()->toString();
            // dataSet->next()->toString();
            actualNum_TableSelect++;
        }
        ASSERT_EQ(expectNum_TableSelect, actualNum_TableSelect) << "[TestSelect1 FAIL] Expected num and actual num are inconsistent"
        << " Expected: " << expectNum_TableSelect << ", Actual: " << actualNum_TableSelect << std::endl;
        // 4、验证值正确性（表模型无法正确获取，树模型可以）
        // unique_ptr<SessionDataSet> dataSet2 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + TABLE_NAME_TableSelect + " order by time");
        // while (dataSet2->hasNext()) {
            // auto rowRecord = dataSet->next();
            // cout << rowRecord->timestamp << " ";  
            // cout << rowRecord->fields[0].stringV << " ";
            // cout << rowRecord->fields[1].stringV << " ";
            // cout << rowRecord->fields[2].stringV << " ";
            // cout << rowRecord->fields[3].stringV << " ";
            // cout << rowRecord->fields[4].boolV << " ";
            // cout << rowRecord->fields[5].intV << " ";
            // cout << rowRecord->fields[6].longV << " ";
            // cout << rowRecord->fields[7].floatV << " ";
            // cout << rowRecord->fields[8].doubleV << " ";
            // cout << rowRecord->fields[9].stringV << " ";
            // cout << rowRecord->fields[10].stringV << " ";
            // cout << rowRecord->fields[11].stringV << " ";
            // cout << rowRecord->fields[12].dateV << " ";
            // cout << rowRecord->fields[13].longV << endl;
        // }
        // 5、验证值其他方法可用性
        dataSet->setFetchSize(1024);
        ASSERT_EQ(1024, dataSet->getFetchSize()) << "[TestSelect1 FAIL] Expected fetch size and actual fetch size are inconsistent"
        << " Expected: " << expectNum_TableSelect << ", Actual: " << actualNum_TableSelect << std::endl;
        dataSet->closeOperationHandle();
    }  catch (IoTDBConnectionException &e) {
        isErrorTest_TableSelect = true;
        ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
    } catch (IoTDBException &e) {
        isErrorTest_TableSelect = true;
        ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBException - " << string(e.what());
    }  catch (exception &e) {
        isErrorTest_TableSelect = true;
        ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] exception - " << string(e.what());
    }
}

// 二、验证DataIterator查询：包含返回列名、数据类型和值正确性、同种数据类型通过索引和通过列名返回值一致性、是否为空值判断的正确性和其他方法可用性
TEST_F(TableSelectTest, TestSelect2) {
    try {
        unique_ptr<SessionDataSet> dataSet1 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + TABLE_NAME_TableSelect + " order by time");
        auto dataIterator1 = dataSet1->getIterator();
        // 1、验证列名正确性
        auto getColumnNames = dataIterator1.getColumnNames();
        for (int i = 0; i < columnNames_TableSelect.size(); i++) {
            std::transform(columnNames_TableSelect[i].begin(), columnNames_TableSelect[i].end(), columnNames_TableSelect[i].begin(),[](unsigned char c) { return std::tolower(c); });
            ASSERT_EQ(columnNames_TableSelect[i], getColumnNames[i + 1]) << "[TestSelect1 FAIL] Expected column name and actual column name are inconsistent," << " Expected: " << columnNames_TableSelect[i] << ", Actual: " << getColumnNames[i] << std::endl;
        }
        // 2、验证数据类型正确性
        auto getColumnTypeList = dataIterator1.getColumnTypeList();
        for (int i = 0; i < dataTypes_TableSelect.size(); i++) {
            switch (dataTypes_TableSelect[i]) {
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
                    isErrorTest_TableSelect = true;
                    ASSERT_EQ(false, isErrorTest_TableSelect) << "[TestSelect1 FAIL] Data type " << dataTypes_TableSelect[i] << " is not supported!!!";
                    break;
            }
        }
        // 3、验证返回值正确性
        auto [result, err] = parseCSVFile_TableSelect("../../data/table/select_data1.csv");
        for (int row = 0; row < result.size(); row++) {
            dataIterator1.next();
            ASSERT_EQ(dataIterator1.getLongByIndex(1), int64_t(stoll(result[row][0]))) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: "<< dataIterator1.getLongByIndex(1) <<", Actual: " << int64_t(stoll(result[row][0])) << std::endl;
            for (int column = 0; column < result[row].size() - 1; column++) {
                if (result[row][column+1] != "null") {
                    switch (dataTypes_TableSelect[column]) {
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
                            isErrorTest_TableSelect = true;
                            ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBException - Data type "<< dataTypes_TableSelect[column] << " is not supported!!!";
                            break;
                    }
                } else {
                    switch (dataTypes_TableSelect[column]) {
                            case TSDataType::TEXT:
                            case TSDataType::STRING:
                            case TSDataType::BLOB:
                                ASSERT_EQ(dataIterator1.getStringByIndex(column+2).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getStringByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::BOOLEAN:
                                ASSERT_EQ(dataIterator1.getBooleanByIndex(column+2).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getBooleanByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::INT32:
                                ASSERT_EQ(dataIterator1.getIntByIndex(column+2).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getIntByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::INT64:
                                ASSERT_EQ(dataIterator1.getLongByIndex(column+2).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getLongByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::TIMESTAMP:
                                // ASSERT_EQ(dataIterator1.getTimestampByIndex(column+2).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getTimestampByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                            break;
                            case TSDataType::FLOAT:
                                ASSERT_EQ(dataIterator1.getFloatByIndex(column+2).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getFloatByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                           case TSDataType::DOUBLE:
                                ASSERT_EQ(dataIterator1.getDoubleByIndex(column+2).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getDoubleByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::DATE:
                                ASSERT_EQ(dataIterator1.getDateByIndex(column+2).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getDateByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            default:
                                isErrorTest_TableSelect = true;
                                ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBException - Data type "<< dataTypes_TableSelect[column] << " is not supported!!!";
                                break;
                    }
                }
            }
        }
        // 4、验证同种数据类型通过索引和通过列名返回值一致性
        unique_ptr<SessionDataSet> dataSet2 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + TABLE_NAME_TableSelect + " order by time");
        auto dataIterator2 = dataSet2->getIterator();
        while (dataIterator2.next()) {
            if (dataIterator2.isNullByIndex(1) == 1) {
                ASSERT_EQ(dataIterator2.getLongByIndex(1).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(1) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getLongByIndex(1).value(), dataIterator2.getLong("time")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(1) << ", Actual: " << dataIterator2.getLong("time") << std::endl;
            }
            if (dataIterator2.isNullByIndex(2) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(2).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(2) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(2).value(), dataIterator2.getString("tag1")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(2) << ", Actual: " << dataIterator2.getString("tag1") << std::endl;
            }
            if (dataIterator2.isNullByIndex(3) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(3).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(3) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(3).value(), dataIterator2.getString("tag2")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(3) << ", Actual: " << dataIterator2.getString("tag2") << std::endl;
            }
            if (dataIterator2.isNullByIndex(4) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(4).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(4) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(4).value(), dataIterator2.getString("attr1")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(4) << ", Actual: " << dataIterator2.getString("attr1") << std::endl;
            }
            if (dataIterator2.isNullByIndex(5) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(5).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(5) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(5).value(), dataIterator2.getString("attr2")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(5) << ", Actual: " << dataIterator2.getString("attr2") << std::endl;
            }
            if (dataIterator2.isNullByIndex(6) == 1) {
                ASSERT_EQ(dataIterator2.getBooleanByIndex(6).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getBooleanByIndex(6) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getBooleanByIndex(6).value(), dataIterator2.getBoolean("boolean")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getBooleanByIndex(6) << ", Actual: " << dataIterator2.getBoolean("boolean") << std::endl;
            }
            if (dataIterator2.isNullByIndex(7) == 1) {
                ASSERT_EQ(dataIterator2.getIntByIndex(7).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getIntByIndex(7) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getIntByIndex(7).value(), dataIterator2.getInt("int32")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getIntByIndex(7) << ", Actual: " << dataIterator2.getInt("int32") << std::endl;
            }
            if (dataIterator2.isNullByIndex(8) == 1) {
                ASSERT_EQ(dataIterator2.getLongByIndex(8).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(8) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getLongByIndex(8).value(), dataIterator2.getLong("int64")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(8) << ", Actual: " << dataIterator2.getLong("int64") << std::endl;
            }
            if (dataIterator2.isNullByIndex(9) == 1) {
                ASSERT_EQ(dataIterator2.getFloatByIndex(9).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getFloatByIndex(9) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getFloatByIndex(9).value(), dataIterator2.getFloat("float")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getFloatByIndex(9) << ", Actual: " << dataIterator2.getFloat("float") << std::endl;
            }
            if (dataIterator2.isNullByIndex(10) == 1) {
                ASSERT_EQ(dataIterator2.getDoubleByIndex(10).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDoubleByIndex(10) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getDoubleByIndex(10).value(), dataIterator2.getDouble("double")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDoubleByIndex(10) << ", Actual: " << dataIterator2.getDouble("double") << std::endl;
            }
            if (dataIterator2.isNullByIndex(11) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(11).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(11) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(11).value(), dataIterator2.getString("text")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(11) << ", Actual: " << dataIterator2.getString("text") << std::endl;
            }
            if (dataIterator2.isNullByIndex(12) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(12).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(12) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(12).value(), dataIterator2.getString("string")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(12) << ", Actual: " << dataIterator2.getString("string") << std::endl;
            }
            if (dataIterator2.isNullByIndex(13) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(13).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(13) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(13).value(), dataIterator2.getString("blob")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(13) << ", Actual: " << dataIterator2.getString("blob") << std::endl;
            }
            if (dataIterator2.isNullByIndex(14) == 1) {
                ASSERT_EQ(dataIterator2.getDateByIndex(14).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDateByIndex(14) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getDateByIndex(14).value(), dataIterator2.getDate("date")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDateByIndex(14) << ", Actual: " << dataIterator2.getDate("date") << std::endl;
            }
            if (dataIterator2.isNullByIndex(15) == 1) {
                // ASSERT_EQ(dataIterator2.getTimestampByIndex(15).is_initialized(), 0) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getTimestampByIndex(15) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getTimestampByIndex(15).value(), dataIterator2.getTimestamp("timestamp")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getTimestampByIndex(15) << ", Actual: " << dataIterator2.getTimestamp("timestamp") << std::endl;
            }
        }
        // 5、验证是否为空值判断的正确性
        unique_ptr<SessionDataSet> dataSet3 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + TABLE_NAME_TableSelect + " order by time");
        auto dataIterator3 = dataSet3->getIterator();
        while (dataIterator3.next()) {
            ASSERT_EQ(dataIterator3.isNullByIndex(1), dataIterator3.isNull("time")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(1) << ", Actual: " << dataIterator3.isNull("time") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(2), dataIterator3.isNull("tag1")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(2) << ", Actual: " << dataIterator3.isNull("tag1") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(3), dataIterator3.isNull("tag2")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(3) << ", Actual: " << dataIterator3.isNull("tag2") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(4), dataIterator3.isNull("attr1")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(4) << ", Actual: " << dataIterator3.isNull("attr1") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(5), dataIterator3.isNull("attr2")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(5) << ", Actual: " << dataIterator3.isNull("attr2") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(6), dataIterator3.isNull("boolean")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(6) << ", Actual: " << dataIterator3.isNull("boolean") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(7), dataIterator3.isNull("int32")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(7) << ", Actual: " << dataIterator3.isNull("int32") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(8), dataIterator3.isNull("int64")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(8) << ", Actual: " << dataIterator3.isNull("int64") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(9), dataIterator3.isNull("float")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(9) << ", Actual: " << dataIterator3.isNull("float") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(10), dataIterator3.isNull("double")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(10) << ", Actual: " << dataIterator3.isNull("double") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(11), dataIterator3.isNull("text")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(11) << ", Actual: " << dataIterator3.isNull("text") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(12), dataIterator3.isNull("string")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(12) << ", Actual: " << dataIterator3.isNull("string") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(13), dataIterator3.isNull("blob")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(13) << ", Actual: " << dataIterator3.isNull("blob") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(14), dataIterator3.isNull("date")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(14) << ", Actual: " << dataIterator3.isNull("date") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(15), dataIterator3.isNull("timestamp")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(15) << ", Actual: " << dataIterator3.isNull("timestamp") << std::endl;
        }
        // 6、验证其他方法可用性：findColumn
        unique_ptr<SessionDataSet> dataSet4 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + TABLE_NAME_TableSelect + " order by time");
        auto dataIterator4 = dataSet4->getIterator();
        ASSERT_EQ(1, dataIterator3.findColumn("time")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: 1, Actual: " << dataIterator3.findColumn("time") << std::endl;
        ASSERT_EQ(2, dataIterator3.findColumn("tag1")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: 2, Actual: " << dataIterator3.findColumn("tag1") << std::endl;
        ASSERT_EQ(4, dataIterator3.findColumn("attr1")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: 4, Actual: " << dataIterator3.findColumn("attr1") << std::endl;
        ASSERT_EQ(6, dataIterator3.findColumn("boolean")) << "[TestSelect1 FAIL] Expected value and actual value are inconsistent," << " Expected: 6, Actual: " << dataIterator3.findColumn("boolean") << std::endl;
    }  catch (IoTDBConnectionException &e) {
        isErrorTest_TableSelect = true;
        ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBConnectionException - " << string(e.what());
    } catch (IoTDBException &e) {
        isErrorTest_TableSelect = true;
        ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] IoTDBException - " << string(e.what());
    }  catch (exception &e) {
        isErrorTest_TableSelect = true;
        ASSERT_EQ(false, isErrorTest_TableSelect) << "[Error] exception - " << string(e.what());
    }
}


