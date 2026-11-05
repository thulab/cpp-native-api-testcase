#include "TableSession.h"
#include "TableSessionBuilder.h"
#include <iostream>
#include <ostream>
#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#include "plm_test_config.h"

using namespace std;

// K8s 实例可能复用存储，上一轮失败会留下同名数据库；建库前先清理。
// 304 表示服务端仍有 DDL 在途，稍等重试即可。
static void executeWithRetry(const shared_ptr<TableSession>& session, const string& sql) {
    for (int attempt = 0; attempt < 20; ++attempt) {
        try {
            session->executeNonQueryStatement(sql);
            return;
        } catch (const exception& e) {
            if (string(e.what()).find("304") == string::npos) {
                throw;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    throw runtime_error("execute failed after retries: " + sql);
}

// 适配新 SDK：Optional<T> 无 value_or，提供等价 helper（单次求值，避免 next() 重复推进游标）
static inline std::string optStr(const Optional<std::string>& o, const std::string& d) {
    return o.has_value() ? o.value() : d;
}


bool isErrorSessionTest = false;

/*
 * 测试表模型session操作
 */

// 用于验证Session有效性
void validity1(const string& caseName, shared_ptr<TableSession> session) {
    try {
        executeWithRetry(session, "drop database if exists table_session_test1");
        executeWithRetry(session, "CREATE DATABASE table_session_test1");
        executeWithRetry(session, "use table_session_test1");
        executeWithRetry(session, "create table t1 (tag1 string tag, s1 string field)");
        executeWithRetry(session, "insert into t1 (tag1, s1) values('string', 'text')");
        unique_ptr<SessionDataSet> dataSet1 = session->executeQueryStatement("show databases");
        bool foundDatabase = false;
        while (dataSet1->hasNext()) {
            string databaseName = optStr(dataSet1->next()->fields[0].stringV, "null");
            if (databaseName == "table_session_test1") {
                foundDatabase = true;
            }
        }
        ASSERT_TRUE(foundDatabase) << "The database name is incorrect";
        unique_ptr<SessionDataSet> dataSet2 = session->executeQueryStatement("show tables");
        while (dataSet2->hasNext()) {
            ASSERT_EQ("t1", optStr(dataSet2->next()->fields[0].stringV, "null")) << "The Table name is incorrect";
        }
        unique_ptr<SessionDataSet> dataSet3 = session->executeQueryStatement("select s1 from t1");
        while (dataSet3->hasNext()) {
            ASSERT_EQ("text", optStr(dataSet3->next()->fields[0].stringV, "null")) << "The Data is incorrect";
        }
        executeWithRetry(session, "drop table t1");
        executeWithRetry(session, "drop DATABASE table_session_test1");
    } catch (IoTDBException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) <<"[Error] " << caseName << " - " << string(e.what());
    }
}

// 先在不绑定 database 的会话里准备好数据库，再让绑定 database 的会话打开；
// 否则目标库不存在时后续语句会报 701: database is not specified。
static void prepareDatabase(const string& database) {
    auto builder = unique_ptr<TableSessionBuilder>(plmTableBuilder());
    auto session = shared_ptr<TableSession>(
        builder->host(plmHost())
            ->rpcPort(plmPort())
            ->username(plmUser())
            ->password(plmPassword())
            ->build());
    executeWithRetry(session, "drop database if exists " + database);
    executeWithRetry(session, "CREATE DATABASE " + database);
    session->close();
}

// 用于验证test_database有效性（会话已绑定 database）
void validity2(const string& caseName, shared_ptr<TableSession> session) {
    try {
        executeWithRetry(session, "create table t1 (tag1 string tag, s1 string field)");
        executeWithRetry(session, "insert into t1 (tag1, s1) values('string', 'text')");
        unique_ptr<SessionDataSet> dataSet1 = session->executeQueryStatement("show databases");
        bool foundDatabase = false;
        while (dataSet1->hasNext()) {
            string databaseName = optStr(dataSet1->next()->fields[0].stringV, "null");
            if (databaseName == "table_session_test2") {
                foundDatabase = true;
            }
        }
        ASSERT_TRUE(foundDatabase) << "[Error] " << caseName << ": The database name is incorrect";
        unique_ptr<SessionDataSet> dataSet2 = session->executeQueryStatement("show tables");
        while (dataSet2->hasNext()) {
            ASSERT_EQ("t1", optStr(dataSet2->next()->fields[0].stringV, "null")) << "[Error] " << caseName << ": The Table name is incorrect";
        }
        unique_ptr<SessionDataSet> dataSet3 = session->executeQueryStatement("select s1 from t1");
        while (dataSet3->hasNext()) {
            ASSERT_EQ("text", optStr(dataSet3->next()->fields[0].stringV, "null")) << "[Error] " << caseName << ": The Data is incorrect";
        }
        executeWithRetry(session, "drop table t1");
        executeWithRetry(session, "drop DATABASE table_session_test2");
    } catch (IoTDBException &e) {
        isErrorSessionTest = true;
        ASSERT_EQ(false, isErrorSessionTest) <<"[Error] " << caseName << " - " << string(e.what());
    }
}

// |------------ Normal Test ------------|

// 1、参数使用默认值
TEST(TestSessionConnection, TestDefaultValue) {
    try{
        auto builder = std::unique_ptr<TableSessionBuilder>(plmTableBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host(plmHost())
            ->rpcPort(plmPort())
            ->username(plmUser())
            ->password(plmPassword())
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
        prepareDatabase("table_session_test2");
        auto builder = std::unique_ptr<TableSessionBuilder>(plmTableBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host(plmHost())
            ->rpcPort(plmPort())
            ->username(plmUser())
            ->password(plmPassword())
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
        auto builder = std::unique_ptr<TableSessionBuilder>(plmTableBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host(plmHost())
            ->rpcPort(plmPort())
            ->username(plmUser())
            ->password(plmPassword())
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
        auto builder = std::unique_ptr<TableSessionBuilder>(plmTableBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host(plmHost())
            ->rpcPort(plmPort())
            ->username(plmUser())
            ->password(plmPassword())
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
        auto builder = std::unique_ptr<TableSessionBuilder>(plmTableBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host(plmHost())
            ->rpcPort(plmPort())
            ->username(plmUser())
            ->password(plmPassword())
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
        prepareDatabase("table_session_test2");
        auto builder = std::unique_ptr<TableSessionBuilder>(plmTableBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host(plmHost())
            ->rpcPort(plmPort())
            ->username(plmUser())
            ->password(plmPassword())
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
        auto builder = std::unique_ptr<TableSessionBuilder>(plmTableBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host(plmHost())
            ->rpcPort(plmPort())
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
//         auto builder = std::unique_ptr<TableSessionBuilder>(plmTableBuilder());
//         auto session =std::shared_ptr<TableSession>(
//             builder
//             ->host("192.0.0.1")
//             ->rpcPort(plmPort())
//             ->username(plmUser())
//             ->password(plmPassword())
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
        auto builder = std::unique_ptr<TableSessionBuilder>(plmTableBuilder());
        auto session =std::shared_ptr<TableSession>(
            builder
            ->host(plmHost())
            ->rpcPort(0000)
            ->username(plmUser())
            ->password(plmPassword())
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

