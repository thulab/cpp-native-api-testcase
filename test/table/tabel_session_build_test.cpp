// #include "TableSession.h"
// #include "TableSessionBuilder.h"
// #include <iostream>
// #include <ostream>
// #include "gtest/gtest.h"

// using namespace std;

// TableSession *session2;
// bool isErrorSessionTest = false;

// /*
//  * 测试表模型session操作
//  */

// // 用于验证Session有效性
// void validity1(const string& caseName) {
//     try {
//         session2->executeNonQueryStatement("CREATE DATABASE test_session_db1");
//         session2->executeNonQueryStatement("use test_session_db1");
//         session2->executeNonQueryStatement("create table t1 (tag1 string tag, s1 string field)");
//         session2->executeNonQueryStatement("insert into t1 (tag1, s1) values('string', 'text')");
//         unique_ptr<SessionDataSet> dataSet1 = session2->executeQueryStatement("show databases");
//         while (dataSet1->hasNext()) {
//             string databaseName = dataSet1->next()->fields[0].stringV;
//             if (databaseName != "information_schema") {
//                 ASSERT_EQ("test_session_db1", databaseName) << "The database name is incorrect";
//             }
//         }
//         unique_ptr<SessionDataSet> dataSet2 = session2->executeQueryStatement("show tables");
//         while (dataSet2->hasNext()) {
//             ASSERT_EQ("t1", dataSet2->next()->fields[0].stringV) << "The Table name is incorrect";
//         }
//         unique_ptr<SessionDataSet> dataSet3 = session2->executeQueryStatement("select s1 from t1");
//         while (dataSet3->hasNext()) {
//             ASSERT_EQ("text", dataSet3->next()->fields[0].stringV) << "The Data is incorrect";
//         }
//         session2->executeNonQueryStatement("drop table t1");
//         session2->executeNonQueryStatement("drop DATABASE test_session_db1");
//     } catch (IoTDBException &e) {
//         isErrorSessionTest = true;
//         ASSERT_EQ(false, isErrorSessionTest) <<"[Error] " << caseName << " - " << string(e.what());
//     }
// }

// // 用于验证test_database有效性（不指定数据库）
// void validity2(const string& caseName) {
//     try {
//         session2->executeNonQueryStatement("CREATE DATABASE test_session_db2");
//         session2->executeNonQueryStatement("create table t1 (tag1 string tag, s1 string field)");
//         session2->executeNonQueryStatement("insert into t1 (tag1, s1) values('string', 'text')");
//         unique_ptr<SessionDataSet> dataSet1 = session2->executeQueryStatement("show databases");
//         while (dataSet1->hasNext()) {
//             string databaseName = dataSet1->next()->fields[0].stringV;
//             if (databaseName != "information_schema") {
//                 ASSERT_EQ("test_session_db2", databaseName) << "[Error] " << caseName << ": The database name is incorrect";
//             }
//         }
//         unique_ptr<SessionDataSet> dataSet2 = session2->executeQueryStatement("show tables");
//         while (dataSet2->hasNext()) {
//             ASSERT_EQ("t1", dataSet2->next()->fields[0].stringV) << "[Error] " << caseName << ": The Table name is incorrect";
//         }
//         unique_ptr<SessionDataSet> dataSet3 = session2->executeQueryStatement("select s1 from t1");
//         while (dataSet3->hasNext()) {
//             ASSERT_EQ("text", dataSet3->next()->fields[0].stringV) << "[Error] " << caseName << ": The Data is incorrect";
//         }
//         session2->executeNonQueryStatement("drop table t1");
//         session2->executeNonQueryStatement("drop DATABASE test_session_db2");
//     } catch (IoTDBException &e) {
//         isErrorSessionTest = true;
//         ASSERT_EQ(false, isErrorSessionTest) <<"[Error] " << caseName << " - " << string(e.what());
//     }
// }

// // |------------ Normal Test ------------|

// // 1、参数使用默认值
// TEST(test_session_connection, test_default_value) {
//     try{
//         session2 = (new TableSessionBuilder())
//             ->host("127.0.0.1")
//             ->rpcPort(6667)
//             ->username("root")
//             ->password("root")
//             ->build();
        
//         validity1("test_default_value");

//         session2->close();
//     } catch (IoTDBConnectionException &e) {
//         isErrorSessionTest = true;
//         ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_default_value - " << string(e.what());
//     }
// }

// // 2、指定参数database为大小写数据库
// TEST(test_session_connection, test_database) {
//     try{
//         session2 = (new TableSessionBuilder())
//             ->host("127.0.0.1")
//             ->rpcPort(6667)
//             ->username("root")
//             ->password("root")
//             ->database("Test_session_DB2")
//             ->build();
        
//         validity2("test_database");

//         session2->close();
//     } catch (IoTDBConnectionException &e) {
//         isErrorSessionTest = true;
//         ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_database - " << string(e.what());
//     }
// }

// // 3、设置查询结果的提取大小
// TEST(test_session_connection, test_fetchSize) {
//     try{
//         session2 = (new TableSessionBuilder())
//             ->host("127.0.0.1")
//             ->rpcPort(6667)
//             ->username("root")
//             ->password("root")
//             ->fetchSize(10)
//             ->build();
        
//         validity1("test_fetchSize");

//         session2->close();
//     } catch (IoTDBConnectionException &e) {
//         isErrorSessionTest = true;
//         ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_fetchSize - " << string(e.what());
//     }
// }

// // 4、指定参数zoneId
// TEST(test_session_connection, test_zoneId) {
//     try{
//         session2 = (new TableSessionBuilder())
//             ->host("127.0.0.1")
//             ->rpcPort(6667)
//             ->username("root")
//             ->password("root")
//             ->zoneId("America/New_York")
//             ->build();
        
//         validity1("test_zoneId");

//         session2->close();
//     } catch (IoTDBConnectionException &e) {
//         isErrorSessionTest = true;
//         ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_zoneId - " << string(e.what());
//     }
// }

// // 5、使用全部参数
// TEST(test_session_connection, test_all) {
//     try{
//         session2 = (new TableSessionBuilder())
//             ->host("127.0.0.1")
//             ->rpcPort(6667)
//             ->username("root")
//             ->password("root")
//             ->database("Test_session_DB2")
//             ->fetchSize(10)
//             ->build();
        
//         validity2("test_all");

//         session2->close();
//     } catch (IoTDBConnectionException &e) {
//         isErrorSessionTest = true;
//         ASSERT_EQ(false, isErrorSessionTest) << "[Error] test_all - " << string(e.what());
//     }
// }

// // |------------ Error Test ------------|

// // // 6、错误的用户名或密码
// // TEST(test_session_connection_error, test_error_username_and_password) {
// //     try{
// //         session2 = (new TableSessionBuilder())
// //             ->host("127.0.0.1")
//             // ->rpcPort(6667)
// //             ->username("1234")
// //             ->password("1234")
// //             ->build();

// //         session2->close();
// //     } catch (exception &e) {
// //         ASSERT_EQ("801: Authentication failed.", string(e.what())) << "[Error] test_error_username_and_password - " << string(e.what());
// //     }
// // }

// // // 7、关闭session连接后使用session
// // TEST(test_session_connection_error, test_error_session_close) {
// //     try{
// //         session2 = (new TableSessionBuilder())
// //             ->host("127.0.0.1")
//             // ->rpcPort(6667)
//             // ->username("root")
//             // ->password("root")
// //             ->build();
        
// //     } catch (IoTDBConnectionException &e) {
// //         ASSERT_EQ("Called write on non-open socket", string(e.what())) << "[Error] test_default_value - " << string(e.what());
// //     }
// //     session2->executeNonQueryStatement("show database");
// // }

