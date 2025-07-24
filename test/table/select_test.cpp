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
 * 标题：表模型查询相关接口功能测试
 * 作者：肖林捷
 */

using namespace std;

TableSession *session_select_test; // 作为全局的session变量
bool isErrorTest_select_test = false; // 用于确认是否出错的标识
int expectNum_select_test = 0; // 用于记录插入期待的行数
int actualNum_select_test = 0; // 用于记录实际查询的行数
string database_name_select_test = "test_select"; // 数据库名
string table_name_select_test = "t1"; // 表名
vector<string> columnNames_select_test = { // 列名
    "TAG1", "TAG2", // TAG列
    "ATTR1", "ATTR2", // ATTRIBUTE列
    "BOOLEAN", "INT32", "INT64", "FLOAT", "DOUBLE", "TEXT", "STRING", "BLOB", "DATE", "TIMESTAMP" // FIELD列
};
vector<TSDataType::TSDataType> dataTypes_select_test = { // 数据类型
    TSDataType::STRING, TSDataType::STRING,
    TSDataType::STRING, TSDataType::STRING,
    TSDataType::BOOLEAN, TSDataType::INT32, TSDataType::INT64, TSDataType::FLOAT, TSDataType::DOUBLE, TSDataType::TEXT, TSDataType::STRING, TSDataType::BLOB, TSDataType::DATE, TSDataType::TIMESTAMP // FIELD列
};
vector<ColumnCategory> columnTypes_select_test = { // 列类别
    ColumnCategory::TAG,ColumnCategory::TAG,
    ColumnCategory::ATTRIBUTE,ColumnCategory::ATTRIBUTE,
    ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD,ColumnCategory::FIELD
};

// 解析CVS文件，获取数据
pair<vector<vector<string>>, string> parseCSVFile_select_test(const string& filename) {
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
void insertDate_select_test() {
    try{
        vector<pair<string, TSDataType::TSDataType>> schemaList;
        for (int i =0; i < columnNames_select_test.size(); i++) {
            schemaList.push_back(make_pair(columnNames_select_test[i],dataTypes_select_test[i]));
        }
        Tablet tablet(table_name_select_test, schemaList, columnTypes_select_test, 10000);
        auto [result, err] = parseCSVFile_select_test("../../data/table/select_data1.csv");
        for (int row = 0; row < result.size(); row++) {
            int rowIndex = tablet.rowSize++;
            tablet.timestamps[rowIndex] = stoll(result[row][0]);
            for (int column = 0; column < result[row].size() - 1; column++) {
                if (result[row][column+1] == "null") {
                    switch (dataTypes_select_test[column]) {
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
                    switch (dataTypes_select_test[column]) {
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
            expectNum_select_test++;
        }
        tablet.setAligned(false);
        session_select_test->insert(tablet);
    }  catch (IoTDBConnectionException &e) {
        isErrorTest_select_test = true;
        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBConnectionException - " << string(e.what());
    } catch (IoTDBException &e) {
        isErrorTest_select_test = true;
        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBException - " << string(e.what());
    } catch (exception &e) {
        isErrorTest_select_test = true;
        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] exception - " << string(e.what());
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
                session_select_test->executeNonQueryStatement("drop database if exists " + database_name_select_test);
                session_select_test->executeNonQueryStatement("create database " + database_name_select_test);
                session_select_test->executeNonQueryStatement("use " + database_name_select_test);
                session_select_test->executeNonQueryStatement("CREATE TABLE " + table_name_select_test + " (tag1 STRING TAG, tag2 STRING TAG, attr1 STRING ATTRIBUTE, attr2 STRING ATTRIBUTE, BOOLEAN BOOLEAN FIELD, INT32 INT32 FIELD, INT64 INT64 FIELD, FLOAT FLOAT FIELD, DOUBLE DOUBLE FIELD, TEXT TEXT FIELD, STRING STRING FIELD, BLOB BLOB FIELD, DATE DATE FIELD, TIMESTAMP TIMESTAMP FIELD)");
                insertDate_select_test();
            }  catch (IoTDBConnectionException &e) {
                isErrorTest_select_test = true;
                ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBConnectionException - " << string(e.what());
            } catch (IoTDBException &e) {
                isErrorTest_select_test = true;
                ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBException - " << string(e.what());
            } catch (exception &e) {
                isErrorTest_select_test = true;
                ASSERT_EQ(false, isErrorTest_select_test) << "[Error] exception - " << string(e.what());
            }
        }
        // 在每个测试用例执行之后调用
        void TearDown() override {
            // 清理操作
            expectNum_select_test = 0;
            try {
                session_select_test->executeNonQueryStatement("drop DATABASE " + database_name_select_test);
                session_select_test->close();
            }  catch (IoTDBConnectionException &e) {
                isErrorTest_select_test = true;
                ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBConnectionException - " << string(e.what());
            } catch (IoTDBException &e) {
                isErrorTest_select_test = true;
                ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBException - " << string(e.what());
            }  catch (exception &e) {
                isErrorTest_select_test = true;
                ASSERT_EQ(false, isErrorTest_select_test) << "[Error] exception - " << string(e.what());
            }
        }
};


// |------------ Normal Test ------------|

// 一、验证executeQueryStatement和RowRecord的Field查询：包含SessionDataSet类查询返回的列名、数据类型和行数正确性和其他方法可用性，RowRecord的Field查询值的正确性（注意：目前表模型使用Field查询存在问题）
TEST_F(TableSelectTest, TestSelect1) {
    try {
        unique_ptr<SessionDataSet> dataSet = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + table_name_select_test + " order by time");
        // 1、验证列名正确性
        auto getColumnNames = dataSet->getColumnNames();
        for (int i = 0; i < columnNames_select_test.size(); i++) {
            std::transform(columnNames_select_test[i].begin(), columnNames_select_test[i].end(), columnNames_select_test[i].begin(),[](unsigned char c) { return std::tolower(c); });
            ASSERT_EQ(columnNames_select_test[i], getColumnNames[i + 1]) << "[FAIL] Expected column name and actual column name are inconsistent," << " Expected: " << columnNames_select_test[i] << ", Actual: " << getColumnNames[i] << std::endl;
        }
        // 2、验证数据类型正确性
        auto getColumnTypeList = dataSet->getColumnTypeList();
        for (int i = 0; i < dataTypes_select_test.size(); i++) {
            switch (dataTypes_select_test[i]) {
                case 0:
                    ASSERT_EQ("BOOLEAN", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: BOOLEAN" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 1:
                    ASSERT_EQ("INT32", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: INT32" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 2:
                    ASSERT_EQ("INT64", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: INT64" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 3:
                    ASSERT_EQ("FLOAT", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: FLOAT" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 4:
                    ASSERT_EQ("DOUBLE", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: DOUBLE" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 5:
                    ASSERT_EQ("TEXT", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: TEXT" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 8:
                    ASSERT_EQ("TIMESTAMP", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: TIMESTAMP" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 9:
                    ASSERT_EQ("DATE", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: DATE" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 10:
                    ASSERT_EQ("BLOB", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: BOBLOBOLEAN" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 11:
                    ASSERT_EQ("STRING", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: STRING" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                default:
                    isErrorTest_select_test = true;
                    ASSERT_EQ(false, isErrorTest_select_test) << "[FAIL] Data type " << dataTypes_select_test[i] << " is not supported!!!";
                    break;
            }
        }
        // 3、验证行数正确性（表模型无法正确获取，树模型可以）
        while (dataSet->hasNext()) {
            dataSet->next()->toString();
            // cout << dataSet->next()->toString() << " ";  
            actualNum_select_test++;
        }
        ASSERT_EQ(expectNum_select_test, actualNum_select_test) << "[FAIL] Expected num and actual num are inconsistent"
        << " Expected: " << expectNum_select_test << ", Actual: " << actualNum_select_test << std::endl;
        // 4、验证值正确性
        auto [result, err] = parseCSVFile_select_test("../../data/table/select_data1.csv");
        unique_ptr<SessionDataSet> dataSet2 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + table_name_select_test + " order by time");
        for (int row = 0; row < result.size(); row++) {
            auto rowRecord = dataSet2->next();
            ASSERT_EQ(rowRecord->fields[0].longV.value(), int64_t(stoll(result[row][0]))) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: "<< rowRecord->fields[0].longV.value() <<", Actual: " << int64_t(stoll(result[row][0])) << std::endl;
            for (int column = 0; column < result[row].size() - 1; column++) {
                if (result[row][column+1] != "null") {
                    switch (dataTypes_select_test[column]) {
                    case TSDataType::TEXT:
                    case TSDataType::STRING:
                    case TSDataType::BLOB:
                        ASSERT_EQ(rowRecord->fields[column+1].stringV.value(), result[row][column+1]) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].stringV.value() << ", Actual: " << result[row][column+1] << std::endl;
                        break;
                    case TSDataType::BOOLEAN:
                        ASSERT_EQ(rowRecord->fields[column+1].boolV.value(), ((result[row][column+1].compare("true") == 0) ? true : false)) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].boolV.value() << ", Actual: " << ((result[row][column+1].compare("true") == 0) ? true : false) << std::endl;
                        break;
                    case TSDataType::INT32:
                        ASSERT_EQ(rowRecord->fields[column+1].intV.value(), stoi(result[row][column+1])) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].intV.value() << ", Actual: " << stoi(result[row][column+1]) << std::endl;
                        break;
                    case TSDataType::TIMESTAMP:
                    case TSDataType::INT64:
                        ASSERT_EQ(rowRecord->fields[column+1].longV.value(), int64_t(stoll(result[row][column+1]))) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].longV.value() << ", Actual: " << int64_t(stoll(result[row][column+1])) << std::endl;
                        break;
                    case TSDataType::FLOAT:
                        ASSERT_EQ(rowRecord->fields[column+1].floatV.value(), stof(result[row][column+1])) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].floatV.value() << ", Actual: " << stof(result[row][column+1]) << std::endl;
                        break;
                    case TSDataType::DOUBLE:
                        ASSERT_EQ(rowRecord->fields[column+1].doubleV.value(), stod(result[row][column+1])) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].doubleV.value() << ", Actual: " << stod(result[row][column+1]) << std::endl;
                        break;
                    case TSDataType::DATE:
                        ASSERT_EQ(rowRecord->fields[column+1].dateV.value() , boost::gregorian::from_string(result[row][column+1])) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[13].dateV.value()  << ", Actual: " << boost::gregorian::from_string(result[row][column+1]) << std::endl;
                        break;
                    default:
                        isErrorTest_select_test = true;
                        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBException - Data type "<< dataTypes_select_test[column] << " is not supported!!!";
                        break;
                    }
                } else {
                    switch (dataTypes_select_test[column]) {
                        case TSDataType::TEXT:
                        case TSDataType::STRING:
                        case TSDataType::BLOB:
                            ASSERT_EQ(rowRecord->fields[column+1].stringV.value_or("null"), "null") << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].stringV.value_or("null") << ", Actual: null" << std::endl;
                            break;
                        case TSDataType::BOOLEAN:
                            ASSERT_EQ(rowRecord->fields[column+1].boolV.value_or(false), false) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].boolV.value_or(false) << ", Actual: false" << std::endl;
                            break;
                        case TSDataType::INT32:
                            ASSERT_EQ(rowRecord->fields[column+1].intV.value_or(0), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].intV.value_or(0) << ", Actual: 0" << std::endl;
                            break;
                        case TSDataType::INT64:
                        case TSDataType::TIMESTAMP:
                            ASSERT_EQ(rowRecord->fields[column+1].longV.value_or(0L), 0L) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].longV.value_or(0L) << ", Actual: 0" << std::endl;
                            break;
                        case TSDataType::FLOAT:
                            ASSERT_EQ(rowRecord->fields[column+1].floatV.value_or(0.0F), 0.0F) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].floatV.value_or(0.0F) << ", Actual: 0.0F" << std::endl;
                            break;
                        case TSDataType::DOUBLE:
                            ASSERT_EQ(rowRecord->fields[column+1].doubleV.value_or(0.0), 0.0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].doubleV.value_or(0.0) << ", Actual: 0.0" << std::endl;
                            break;
                        case TSDataType::DATE:
                            ASSERT_EQ(rowRecord->fields[column+1].dateV.value_or(boost::gregorian::date(1970, 1, 2)), boost::gregorian::date(1970, 1, 2)) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << rowRecord->fields[column+1].dateV.value_or(boost::gregorian::date(1970, 1, 2)) << ", Actual: " << boost::gregorian::date(1970, 1, 2) << std::endl;
                            break;
                        default:
                            isErrorTest_select_test = true;
                            ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBException - Data type " << dataTypes_select_test[column] << " is not supported!!!";
                            break;
                    }
                }
            } 
        }
        // 5、验证值其他方法可用性
        dataSet->setFetchSize(1024);
        ASSERT_EQ(1024, dataSet->getFetchSize()) << "[FAIL] Expected fetch size and actual fetch size are inconsistent"
        << " Expected: " << expectNum_select_test << ", Actual: " << actualNum_select_test << std::endl;
        dataSet->closeOperationHandle();
    }  catch (IoTDBConnectionException &e) {
        isErrorTest_select_test = true;
        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBConnectionException - " << string(e.what());
    } catch (IoTDBException &e) {
        isErrorTest_select_test = true;
        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBException - " << string(e.what());
    } catch (exception &e) {
        isErrorTest_select_test = true;
        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] exception - " << string(e.what());
    }
}


// 二、验证DataIterator查询：包含返回列名、数据类型和值正确性、同种数据类型通过索引和通过列名返回值一致性、是否为空值判断的正确性和其他方法可用性
TEST_F(TableSelectTest, TestSelect2) {
    try {
        unique_ptr<SessionDataSet> dataSet1 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + table_name_select_test + " order by time");
        auto dataIterator1 = dataSet1->getIterator();
        // 1、验证列名正确性
        auto getColumnNames = dataIterator1.getColumnNames();
        for (int i = 0; i < columnNames_select_test.size(); i++) {
            std::transform(columnNames_select_test[i].begin(), columnNames_select_test[i].end(), columnNames_select_test[i].begin(),[](unsigned char c) { return std::tolower(c); });
            ASSERT_EQ(columnNames_select_test[i], getColumnNames[i + 1]) << "[FAIL] Expected column name and actual column name are inconsistent," << " Expected: " << columnNames_select_test[i] << ", Actual: " << getColumnNames[i] << std::endl;
        }
        // 2、验证数据类型正确性
        auto getColumnTypeList = dataIterator1.getColumnTypeList();
        for (int i = 0; i < dataTypes_select_test.size(); i++) {
            switch (dataTypes_select_test[i]) {
                case 0:
                    ASSERT_EQ("BOOLEAN", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: BOOLEAN" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 1:
                    ASSERT_EQ("INT32", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: INT32" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 2:
                    ASSERT_EQ("INT64", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: INT64" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 3:
                    ASSERT_EQ("FLOAT", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: FLOAT" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 4:
                    ASSERT_EQ("DOUBLE", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: DOUBLE" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 5:
                    ASSERT_EQ("TEXT", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: TEXT" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 8:
                    ASSERT_EQ("TIMESTAMP", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: TIMESTAMP" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 9:
                    ASSERT_EQ("DATE", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: DATE" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 10:
                    ASSERT_EQ("BLOB", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: BOBLOBOLEAN" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                case 11:
                    ASSERT_EQ("STRING", getColumnTypeList[i + 1]) << "[FAIL] Expected data type and actual data type are inconsistent," 
                    << " Expected: STRING" << ", Actual: " << getColumnTypeList[i] << std::endl;
                    break;
                default:
                    isErrorTest_select_test = true;
                    ASSERT_EQ(false, isErrorTest_select_test) << "[FAIL] Data type " << dataTypes_select_test[i] << " is not supported!!!";
                    break;
            }
        }
        // 3、验证返回值正确性
        auto [result, err] = parseCSVFile_select_test("../../data/table/select_data1.csv");
        for (int row = 0; row < result.size(); row++) {
            dataIterator1.next();
            ASSERT_EQ(dataIterator1.getLongByIndex(1), int64_t(stoll(result[row][0]))) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: "<< dataIterator1.getLongByIndex(1) <<", Actual: " << int64_t(stoll(result[row][0])) << std::endl;
            for (int column = 0; column < result[row].size() - 1; column++) {
                if (result[row][column+1] != "null") {
                    switch (dataTypes_select_test[column]) {
                        case TSDataType::TEXT:
                        case TSDataType::STRING:
                        case TSDataType::BLOB:
                            ASSERT_EQ(dataIterator1.getStringByIndex(column+2), result[row][column+1]) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getStringByIndex(column+2) << ", Actual: " << result[row][column+1] << std::endl;
                            break;
                        case TSDataType::BOOLEAN:
                            ASSERT_EQ(dataIterator1.getBooleanByIndex(column+2), ((result[row][column+1].compare("true") == 0) ? true : false)) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getBooleanByIndex(column+2) << ", Actual: " << ((result[row][column+1].compare("true") == 0) ? true : false) << std::endl;
                            break;
                        case TSDataType::INT32:
                            ASSERT_EQ(dataIterator1.getIntByIndex(column+2), stoi(result[row][column+1])) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getIntByIndex(column+2) << ", Actual: " << stoi(result[row][column+1]) << std::endl;
                            break;
                        case TSDataType::INT64:
                            ASSERT_EQ(dataIterator1.getLongByIndex(column+2), int64_t(stoll(result[row][column+1]))) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getLongByIndex(column+2) << ", Actual: " << int64_t(stoll(result[row][column+1])) << std::endl;
                            break;
                        case TSDataType::TIMESTAMP:
                            ASSERT_EQ(dataIterator1.getTimestampByIndex(column+2), int64_t(stoll(result[row][column+1]))) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getTimestampByIndex(column+2) << ", Actual: " << int64_t(stoll(result[row][column+1])) << std::endl;
                            break;
                        case TSDataType::FLOAT:
                            ASSERT_EQ(dataIterator1.getFloatByIndex(column+2), stof(result[row][column+1])) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getFloatByIndex(column+2) << ", Actual: " << stof(result[row][column+1]) << std::endl;
                            break;
                        case TSDataType::DOUBLE:
                            ASSERT_EQ(dataIterator1.getDoubleByIndex(column+2), stod(result[row][column+1])) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getDoubleByIndex(column+2) << ", Actual: " << stod(result[row][column+1]) << std::endl;
                            break;
                        case TSDataType::DATE:
                            ASSERT_EQ(dataIterator1.getDateByIndex(column+2), boost::gregorian::from_string(result[row][column+1])) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getDateByIndex(column+2) << ", Actual: " << boost::gregorian::from_string(result[row][column+1]) << std::endl;
                            break;
                        default:
                            isErrorTest_select_test = true;
                            ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBException - Data type "<< dataTypes_select_test[column] << " is not supported!!!";
                            break;
                    }
                } else {
                    switch (dataTypes_select_test[column]) {
                            case TSDataType::TEXT:
                            case TSDataType::STRING:
                            case TSDataType::BLOB:
                                ASSERT_EQ(dataIterator1.getStringByIndex(column+2).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getStringByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::BOOLEAN:
                                ASSERT_EQ(dataIterator1.getBooleanByIndex(column+2).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getBooleanByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::INT32:
                                ASSERT_EQ(dataIterator1.getIntByIndex(column+2).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getIntByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::INT64:
                                ASSERT_EQ(dataIterator1.getLongByIndex(column+2).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getLongByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::TIMESTAMP:
                                // ASSERT_EQ(dataIterator1.getTimestampByIndex(column+2).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getTimestampByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                            break;
                            case TSDataType::FLOAT:
                                ASSERT_EQ(dataIterator1.getFloatByIndex(column+2).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getFloatByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                           case TSDataType::DOUBLE:
                                ASSERT_EQ(dataIterator1.getDoubleByIndex(column+2).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getDoubleByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            case TSDataType::DATE:
                                ASSERT_EQ(dataIterator1.getDateByIndex(column+2).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator1.getDateByIndex(column+2).is_initialized() << ", Actual: " << 0 << std::endl;
                                break;
                            default:
                                isErrorTest_select_test = true;
                                ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBException - Data type "<< dataTypes_select_test[column] << " is not supported!!!";
                                break;
                    }
                }
            }
        }
        // 4、验证同种数据类型通过索引和通过列名返回值一致性
        unique_ptr<SessionDataSet> dataSet2 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + table_name_select_test + " order by time");
        auto dataIterator2 = dataSet2->getIterator();
        while (dataIterator2.next()) {
            if (dataIterator2.isNullByIndex(1) == 1) {
                ASSERT_EQ(dataIterator2.getLongByIndex(1).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(1) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getLongByIndex(1).value(), dataIterator2.getLong("time")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(1) << ", Actual: " << dataIterator2.getLong("time") << std::endl;
            }
            if (dataIterator2.isNullByIndex(2) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(2).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(2) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(2).value(), dataIterator2.getString("tag1")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(2) << ", Actual: " << dataIterator2.getString("tag1") << std::endl;
            }
            if (dataIterator2.isNullByIndex(3) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(3).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(3) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(3).value(), dataIterator2.getString("tag2")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(3) << ", Actual: " << dataIterator2.getString("tag2") << std::endl;
            }
            if (dataIterator2.isNullByIndex(4) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(4).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(4) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(4).value(), dataIterator2.getString("attr1")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(4) << ", Actual: " << dataIterator2.getString("attr1") << std::endl;
            }
            if (dataIterator2.isNullByIndex(5) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(5).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(5) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(5).value(), dataIterator2.getString("attr2")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(5) << ", Actual: " << dataIterator2.getString("attr2") << std::endl;
            }
            if (dataIterator2.isNullByIndex(6) == 1) {
                ASSERT_EQ(dataIterator2.getBooleanByIndex(6).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getBooleanByIndex(6) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getBooleanByIndex(6).value(), dataIterator2.getBoolean("boolean")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getBooleanByIndex(6) << ", Actual: " << dataIterator2.getBoolean("boolean") << std::endl;
            }
            if (dataIterator2.isNullByIndex(7) == 1) {
                ASSERT_EQ(dataIterator2.getIntByIndex(7).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getIntByIndex(7) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getIntByIndex(7).value(), dataIterator2.getInt("int32")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getIntByIndex(7) << ", Actual: " << dataIterator2.getInt("int32") << std::endl;
            }
            if (dataIterator2.isNullByIndex(8) == 1) {
                ASSERT_EQ(dataIterator2.getLongByIndex(8).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(8) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getLongByIndex(8).value(), dataIterator2.getLong("int64")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getLongByIndex(8) << ", Actual: " << dataIterator2.getLong("int64") << std::endl;
            }
            if (dataIterator2.isNullByIndex(9) == 1) {
                ASSERT_EQ(dataIterator2.getFloatByIndex(9).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getFloatByIndex(9) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getFloatByIndex(9).value(), dataIterator2.getFloat("float")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getFloatByIndex(9) << ", Actual: " << dataIterator2.getFloat("float") << std::endl;
            }
            if (dataIterator2.isNullByIndex(10) == 1) {
                ASSERT_EQ(dataIterator2.getDoubleByIndex(10).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDoubleByIndex(10) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getDoubleByIndex(10).value(), dataIterator2.getDouble("double")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDoubleByIndex(10) << ", Actual: " << dataIterator2.getDouble("double") << std::endl;
            }
            if (dataIterator2.isNullByIndex(11) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(11).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(11) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(11).value(), dataIterator2.getString("text")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(11) << ", Actual: " << dataIterator2.getString("text") << std::endl;
            }
            if (dataIterator2.isNullByIndex(12) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(12).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(12) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(12).value(), dataIterator2.getString("string")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(12) << ", Actual: " << dataIterator2.getString("string") << std::endl;
            }
            if (dataIterator2.isNullByIndex(13) == 1) {
                ASSERT_EQ(dataIterator2.getStringByIndex(13).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(13) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getStringByIndex(13).value(), dataIterator2.getString("blob")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getStringByIndex(13) << ", Actual: " << dataIterator2.getString("blob") << std::endl;
            }
            if (dataIterator2.isNullByIndex(14) == 1) {
                ASSERT_EQ(dataIterator2.getDateByIndex(14).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDateByIndex(14) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getDateByIndex(14).value(), dataIterator2.getDate("date")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getDateByIndex(14) << ", Actual: " << dataIterator2.getDate("date") << std::endl;
            }
            if (dataIterator2.isNullByIndex(15) == 1) {
                // ASSERT_EQ(dataIterator2.getTimestampByIndex(15).is_initialized(), 0) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getTimestampByIndex(15) << ", Actual: " << 0 << std::endl;
            } else {
                ASSERT_EQ(dataIterator2.getTimestampByIndex(15).value(), dataIterator2.getTimestamp("timestamp")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator2.getTimestampByIndex(15) << ", Actual: " << dataIterator2.getTimestamp("timestamp") << std::endl;
            }
        }
        // 5、验证是否为空值判断的正确性
        unique_ptr<SessionDataSet> dataSet3 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + table_name_select_test + " order by time");
        auto dataIterator3 = dataSet3->getIterator();
        while (dataIterator3.next()) {
            ASSERT_EQ(dataIterator3.isNullByIndex(1), dataIterator3.isNull("time")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(1) << ", Actual: " << dataIterator3.isNull("time") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(2), dataIterator3.isNull("tag1")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(2) << ", Actual: " << dataIterator3.isNull("tag1") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(3), dataIterator3.isNull("tag2")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(3) << ", Actual: " << dataIterator3.isNull("tag2") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(4), dataIterator3.isNull("attr1")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(4) << ", Actual: " << dataIterator3.isNull("attr1") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(5), dataIterator3.isNull("attr2")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(5) << ", Actual: " << dataIterator3.isNull("attr2") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(6), dataIterator3.isNull("boolean")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(6) << ", Actual: " << dataIterator3.isNull("boolean") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(7), dataIterator3.isNull("int32")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(7) << ", Actual: " << dataIterator3.isNull("int32") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(8), dataIterator3.isNull("int64")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(8) << ", Actual: " << dataIterator3.isNull("int64") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(9), dataIterator3.isNull("float")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(9) << ", Actual: " << dataIterator3.isNull("float") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(10), dataIterator3.isNull("double")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(10) << ", Actual: " << dataIterator3.isNull("double") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(11), dataIterator3.isNull("text")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(11) << ", Actual: " << dataIterator3.isNull("text") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(12), dataIterator3.isNull("string")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(12) << ", Actual: " << dataIterator3.isNull("string") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(13), dataIterator3.isNull("blob")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(13) << ", Actual: " << dataIterator3.isNull("blob") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(14), dataIterator3.isNull("date")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(14) << ", Actual: " << dataIterator3.isNull("date") << std::endl;
            ASSERT_EQ(dataIterator3.isNullByIndex(15), dataIterator3.isNull("timestamp")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: " << dataIterator3.isNullByIndex(15) << ", Actual: " << dataIterator3.isNull("timestamp") << std::endl;
        }
        // 6、验证其他方法可用性：findColumn
        unique_ptr<SessionDataSet> dataSet4 = session_select_test->executeQueryStatement("select time,tag1,tag2,attr1,attr2,boolean,int32,int64,float,double,text,string,blob,date,timestamp from " + table_name_select_test + " order by time");
        auto dataIterator4 = dataSet4->getIterator();
        ASSERT_EQ(1, dataIterator3.findColumn("time")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: 1, Actual: " << dataIterator3.findColumn("time") << std::endl;
        ASSERT_EQ(2, dataIterator3.findColumn("tag1")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: 2, Actual: " << dataIterator3.findColumn("tag1") << std::endl;
        ASSERT_EQ(4, dataIterator3.findColumn("attr1")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: 4, Actual: " << dataIterator3.findColumn("attr1") << std::endl;
        ASSERT_EQ(6, dataIterator3.findColumn("boolean")) << "[FAIL] Expected value and actual value are inconsistent," << " Expected: 6, Actual: " << dataIterator3.findColumn("boolean") << std::endl;
    }  catch (IoTDBConnectionException &e) {
        isErrorTest_select_test = true;
        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBConnectionException - " << string(e.what());
    } catch (IoTDBException &e) {
        isErrorTest_select_test = true;
        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] IoTDBException - " << string(e.what());
    }  catch (exception &e) {
        isErrorTest_select_test = true;
        ASSERT_EQ(false, isErrorTest_select_test) << "[Error] exception - " << string(e.what());
    }
}


