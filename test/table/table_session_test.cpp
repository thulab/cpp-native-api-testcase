#include "TableSession.h"
#include "TableSessionBuilder.h"
#include <iostream>
#include <ostream>
#include "gtest/gtest.h"

using namespace std;

bool isErrorSessionTest = false;

/*
 * 测试表模型session操作
 */

// 用于验证Session有效性
void validity1(const string& caseName, shared_ptr<TableSession> session) {
    try {
        session->executeNonQueryStatement("CREATE DATABASE table_session_test1");
        session->executeNonQueryStatement("use table_session_test1");
        session->executeNonQueryStatement("create table t1 (tag1 string tag, s1 string field)");
        session->executeNonQueryStatement("insert into t1 (tag1, s1) values('string', 'text')");
        unique_ptr<SessionDataSet> dataSet1 = session->executeQueryStatement("show databases");
        while (dataSet1->hasNext()) {
            string databaseName = dataSet1->next()->fields[0].stringV.value_or("null");
            if (databaseName != "information_schema") {
                ASSERT_EQ("table_session_test1", databaseName) << "The database name is incorrect";
            }
        }
        unique_ptr<SessionDataSet> dataSet2 = session->executeQueryStatement("show tables");
        while (dataSet2->hasNext()) {
            ASSERT_EQ("t1", dataSet2->next()->fields[0].stringV.value_or("null")) << "The Table name is incorrect";
        }
        unique_ptr<SessionDataSet> dataSet3 = session->executeQueryStatement("select s1 from t1");
        while (dataSet3->hasNext()) {
            ASSERT_EQ("text", dataSet3->next()->fields[0].stringV.value_or("null")) << "The Data is incorrect";
        }
        session->executeNonQueryStatement("drop table t1");
        session->executeNonQueryStatement("drop DATABASE table_session_test1");
    } catch (IoTDBException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) <<"[Error] " << caseName << " - " << string(e.what());
    }
}

// 用于验证test_database有效性（不指定数据库）
void validity2(const string& caseName, shared_ptr<TableSession> session) {
    try {
        session->executeNonQueryStatement("CREATE DATABASE table_session_test2");
        session->executeNonQueryStatement("create table t1 (tag1 string tag, s1 string field)");
        session->executeNonQueryStatement("insert into t1 (tag1, s1) values('string', 'text')");
        unique_ptr<SessionDataSet> dataSet1 = session->executeQueryStatement("show databases");
        while (dataSet1->hasNext()) {
            string databaseName = dataSet1->next()->fields[0].stringV.value_or("null");
            if (databaseName != "information_schema") {
                ASSERT_EQ("table_session_test2", databaseName) << "[Error] " << caseName << ": The database name is incorrect";
            }
        }
        unique_ptr<SessionDataSet> dataSet2 = session->executeQueryStatement("show tables");
        while (dataSet2->hasNext()) {
            ASSERT_EQ("t1", dataSet2->next()->fields[0].stringV.value_or("null")) << "[Error] " << caseName << ": The Table name is incorrect";
        }
        unique_ptr<SessionDataSet> dataSet3 = session->executeQueryStatement("select s1 from t1");
        while (dataSet3->hasNext()) {
            ASSERT_EQ("text", dataSet3->next()->fields[0].stringV.value_or("null")) << "[Error] " << caseName << ": The Data is incorrect";
        }
        session->executeNonQueryStatement("drop table t1");
        session->executeNonQueryStatement("drop DATABASE table_session_test2");
    } catch (IoTDBException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) <<"[Error] " << caseName << " - " << string(e.what());
    }
}

// |------------ Normal Test ------------|

// 1、参数使用默认值
TEST(TestSessionConnection, TestDefaultValue) {
    try{
        auto builder = std::unique_ptr<TableSessionBuilder>(new TableSessionBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->build()
        );
        
        validity1("TestDefaultValue", session);

        session->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] TestDefaultValue - " << string(e.what());
    }
}

// 2、指定参数database为大小写数据库
TEST(TestSessionConnection, TestDatabaseParameter) {
    try{
        auto builder = std::unique_ptr<TableSessionBuilder>(new TableSessionBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->database("table_session_test2")
            ->build()
        );
        
        validity2("TestDatabaseParameter", session);

        session->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] TestDatabaseParameter - " << string(e.what());
    }
}

// 3、设置查询结果的提取大小
TEST(TestSessionConnection, TestFetchSizeParameter) {
    // 3.1、设置为1
    try{
        auto builder = std::unique_ptr<TableSessionBuilder>(new TableSessionBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->fetchSize(1)
            ->build()
        );
        
        validity1("TestFetchSizeParameter", session);

        session->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] TestFetchSizeParameter - " << string(e.what());
    }
    // 3.2、设置为10000
    try{
        auto builder = std::unique_ptr<TableSessionBuilder>(new TableSessionBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->fetchSize(10000)
            ->build()
        );
        
        validity1("TestFetchSizeParameter", session);

        session->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] TestFetchSizeParameter - " << string(e.what());
    }
}

// 4、指定参数zoneId
TEST(TestSessionConnection, TestZoneIdParameter) {
    try{
        auto builder = std::unique_ptr<TableSessionBuilder>(new TableSessionBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->zoneId("America/New_York")
            ->build()
        );
        
        validity1("TestZoneIdParameter", session);

        session->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] TestZoneIdParameter - " << string(e.what());
    }
}

// 5、使用全部参数
TEST(TestSessionConnection, TestAllParameter) {
    try{
        auto builder = std::unique_ptr<TableSessionBuilder>(new TableSessionBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("root")
            ->password("root")
            ->database("table_session_test2")
            ->fetchSize(10000)
            ->build()
        );
        
        validity2("TestAllParameter", session);

        session->close();
    } catch (IoTDBConnectionException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) << "[Error] TestAllParameter - " << string(e.what());
    }
}

// |------------ Error Test ------------|

// 1、错误的用户名或密码
TEST(TestSessionConnectionError, TestErrorUsernameAndPasswordParameter) {
    try{
        auto builder = std::unique_ptr<TableSessionBuilder>(new TableSessionBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host("127.0.0.1")
            ->rpcPort(6667)
            ->username("error")
            ->password("error")
            ->build()
        );
        FAIL() << "Expecting an error, but actually running normally";
    } catch (exception &e) {
        ASSERT_EQ("804: The user error does not exist.", string(e.what())) << "[Error] Error message does not match expectations, expect: 801: Authentication failed., actual: " << string(e.what());
    }
}

// 2、错误的ip地址
// TEST(TestSessionConnectionError, TestErrorHostParameter) {
//     try{
//         auto builder = std::unique_ptr<TableSessionBuilder>(new TableSessionBuilder());
//         auto session =std::shared_ptr<TableSession>(
//             builder
//             ->host("192.0.0.1")
//             ->rpcPort(6667)
//             ->username("root")
//             ->password("root")
//             ->build()
//         );
//         FAIL() << "Expecting an error, but actually running normally";
//     } catch (exception &e) {
//         ASSERT_EQ("timed out", string(e.what())) << "[Error] Error message does not match expectations, expect: timed out, actual: " << string(e.what());
//     }
// }

// 3、错误的端口号
TEST(TestSessionConnectionError, TestErrorRpcPortParameter) {
    try{
        auto builder = std::unique_ptr<TableSessionBuilder>(new TableSessionBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host("127.0.0.1")
            ->rpcPort(0000)
            ->username("root")
            ->password("root")
            ->build()
        );
        FAIL() << "Expecting an error, but actually running normally";
    } catch (exception &e) {
        ASSERT_EQ("No valid endpoints available", string(e.what())) << "[Error] Error message does not match expectations, expect: No valid endpoints available, actual: " << string(e.what());
    }
}

// 3、未开启session连接就使用session（未实现）
// TEST(TestSessionConnectionError, TestErrorSessionClose) {
// }

// 4、设置查询结果的提取大小为负值：目前不会报错
// TEST(TestSessionConnectionError, TestErrorFetchSizeParameter) {
// }

