#include "TableSession.h"
#include "TableSessionBuilder.h"
#include <iostream>
#include <ostream>
#include "gtest/gtest.h"

using namespace std;

TableSession *session_session_test;
bool isErrorSessionTest = false;

/*
 * 测试表模型session操作
 */

// 用于验证Session有效性
void validity1(const string& caseName) {
    try {
        session_session_test->executeNonQueryStatement("CREATE DATABASE test_session_db1");
        session_session_test->executeNonQueryStatement("use test_session_db1");
        session_session_test->executeNonQueryStatement("create table t1 (tag1 string tag, s1 string field)");
        session_session_test->executeNonQueryStatement("insert into t1 (tag1, s1) values('string', 'text')");
        unique_ptr<SessionDataSet> dataSet1 = session_session_test->executeQueryStatement("show databases");
        while (dataSet1->hasNext()) {
            string databaseName = dataSet1->next()->fields[0].stringV.value_or("null");
            if (databaseName != "information_schema") {
                ASSERT_EQ("test_session_db1", databaseName) << "The database name is incorrect";
            }
        }
        unique_ptr<SessionDataSet> dataSet2 = session_session_test->executeQueryStatement("show tables");
        while (dataSet2->hasNext()) {
            ASSERT_EQ("t1", dataSet2->next()->fields[0].stringV.value_or("null")) << "The Table name is incorrect";
        }
        unique_ptr<SessionDataSet> dataSet3 = session_session_test->executeQueryStatement("select s1 from t1");
        while (dataSet3->hasNext()) {
            ASSERT_EQ("text", dataSet3->next()->fields[0].stringV.value_or("null")) << "The Data is incorrect";
        }
        session_session_test->executeNonQueryStatement("drop table t1");
        session_session_test->executeNonQueryStatement("drop DATABASE test_session_db1");
    } catch (IoTDBException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) <<"[Error] " << caseName << " - " << string(e.what());
    }
}

// 用于验证test_database有效性（不指定数据库）
void validity2(const string& caseName) {
    try {
        session_session_test->executeNonQueryStatement("CREATE DATABASE test_session_db2");
        session_session_test->executeNonQueryStatement("create table t1 (tag1 string tag, s1 string field)");
        session_session_test->executeNonQueryStatement("insert into t1 (tag1, s1) values('string', 'text')");
        unique_ptr<SessionDataSet> dataSet1 = session_session_test->executeQueryStatement("show databases");
        while (dataSet1->hasNext()) {
            string databaseName = dataSet1->next()->fields[0].stringV.value_or("null");
            if (databaseName != "information_schema") {
                ASSERT_EQ("test_session_db2", databaseName) << "[Error] " << caseName << ": The database name is incorrect";
            }
        }
        unique_ptr<SessionDataSet> dataSet2 = session_session_test->executeQueryStatement("show tables");
        while (dataSet2->hasNext()) {
            ASSERT_EQ("t1", dataSet2->next()->fields[0].stringV.value_or("null")) << "[Error] " << caseName << ": The Table name is incorrect";
        }
        unique_ptr<SessionDataSet> dataSet3 = session_session_test->executeQueryStatement("select s1 from t1");
        while (dataSet3->hasNext()) {
            ASSERT_EQ("text", dataSet3->next()->fields[0].stringV.value_or("null")) << "[Error] " << caseName << ": The Data is incorrect";
        }
        session_session_test->executeNonQueryStatement("drop table t1");
        session_session_test->executeNonQueryStatement("drop DATABASE test_session_db2");
    } catch (IoTDBException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) <<"[Error] " << caseName << " - " << string(e.what());
    }
}

// |------------ Normal Test ------------|

// 1、参数使用默认值
TEST(test_session_connection, test_default_value) {
    try{
        session_session_test = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->build();
        
        validity1("test_default_value");

        session_session_test->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_default_value - " << string(e.what());
    }
}

// 2、指定参数database为大小写数据库
TEST(test_session_connection, test_database) {
    try{
        session_session_test = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->database("Test_session_DB2")
            ->build();
        
        validity2("test_database");

        session_session_test->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_database - " << string(e.what());
    }
}

// 3、设置查询结果的提取大小
TEST(test_session_connection, test_fetchSize) {
    // 3.1、设置为1
    try{
        session_session_test = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->fetchSize(1)
            ->build();
        
        validity1("test_fetchSize");

        session_session_test->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_fetchSize - " << string(e.what());
    }
    // 3.2、设置为10000
    try{
        session_session_test = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->fetchSize(10000)
            ->build();
        
        validity1("test_fetchSize");

        session_session_test->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_fetchSize - " << string(e.what());
    }
}

// 4、指定参数zoneId
TEST(test_session_connection, test_zoneId) {
    try{
        session_session_test = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->zoneId("America/New_York")
            ->build();
        
        validity1("test_zoneId");

        session_session_test->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_zoneId - " << string(e.what());
    }
}

// 5、使用全部参数
TEST(test_session_connection, test_all) {
    try{
        session_session_test = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->database("Test_session_DB2")
            ->fetchSize(10000)
            ->build();
        
        validity2("test_all");

        session_session_test->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_all - " << string(e.what());
    }
}

// |------------ Error Test ------------|

// 1、错误的用户名或密码
TEST(test_session_connection_error, test_error_username_and_password) {
    try{
        session_session_test = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("1234")
            ->password("1234")
            ->build();

        session_session_test->close();
    } catch (exception &e) {
        ASSERT_EQ("801: Authentication failed.", string(e.what())) << "[Error] test_error_username_and_password - " << string(e.what());
    }
}

// 2、关闭session连接后使用session
TEST(test_session_connection_error, test_error_session_close) {
    try{
        session_session_test->executeNonQueryStatement("show databases");
        session_session_test = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->build();
        session_session_test->close();
        session_session_test->executeNonQueryStatement("show databases");
    } catch (IoTDBConnectionException &e) {
        ASSERT_EQ("Called write on non-open socket", string(e.what())) << "[Error] test_default_value - " << string(e.what());
    }
    
}

// 3、设置查询结果的提取大小为负值：目前不会报错
TEST(test_session_connection, test_error_fetchSize) {
    try{
        session_session_test = (new TableSessionBuilder())
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->fetchSize(-10)
            ->build();
        
        validity1("test_fetchSize");

        session_session_test->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_fetchSize - " << string(e.what());
    }
}

