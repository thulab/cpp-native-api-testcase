/*
 * ============================================================================
 * 【默认屏蔽 / 需手动解除】
 * 本用例依赖被测库已部署并注册 AINode(内置时序大模型 timer_xl),默认自动化测试
 * 环境无 AINode 可用,故已在 test/CMakeLists.txt 中注释本文件、不参与默认构建。
 * 手动验证步骤:
 *   1) 准备一套已注册 AINode/timer_xl 的被测库;
 *   2) 按下方 kHost/kPort/kUser/kPass 配置连接参数(默认经 SSH 隧道 127.0.0.1:16667);
 *   3) 在 test/CMakeLists.txt 中取消对本文件的注释后重新编译运行。
 * ============================================================================
 */
#include "Common.h"
#include "Session.h"
#include "SessionDataSet.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <cmath>

/*
 * 测试用例标题：模型推理结果读取（覆盖 V2-917）
 * 作者：肖林捷
 * 最新编写日期：2026/6
 *
 * 背景（缺陷 V2-917 【企业-001898】湃方-C++API接口使用模型能力报错 Unsupported operation: getDouble）：
 *   客户通过 C++ 原生 API 调用模型推理（CALL INFERENCE）后，按 SessionDataSet -> next() -> toString()
 *   迭代结果时抛出 `IoTDBException: Unsupported operation: getDouble`；同样的推理在 CLI 中可正常输出。
 *   根因：模型推理结果列声明为 DOUBLE，底层实际为 FLOAT 列（FloatColumn），旧版 FloatColumn 未实现
 *   getDouble，迭代 RowRecord 取值时落到基类抛出 "Unsupported operation: getDouble"。
 *   修复（client-cpp Column.cpp/Column.h，#17759）：为 FloatColumn / IntColumn / LongColumn 补充 getDouble，
 *   使 DOUBLE 声明列可从 FLOAT 等数值列安全取值。
 *
 * 本用例只验证“修复版本”的预期行为：模型推理结果可被 toString() 与 getDouble 正常读取，不再抛
 * Unsupported operation: getDouble。
 */

using namespace std;

// ==== 连接参数（按环境修改） ====
// 注：本验证环境中 C++ 客户端经 SSH 隧道连接被测 IoTDB（127.0.0.1:16667 -> 被测库 6667），
// 被测库需已部署并注册 AINode（含内置时序大模型，如 timer_xl）。
static const char* kHost = "127.0.0.1";
static const int   kPort = 16667;
static const char* kUser = "root";
static const char* kPass = "TimechoDB@2021";

// 测试数据库与序列
static const string DB_917 = "root.model_infer_917";
static const string DEVICE_917 = DB_917 + ".d1";
static const string PATH_917 = DEVICE_917 + ".s1";
// 时序大模型 timer_xl 要求输入序列长度 >= 96
static const int POINT_NUM_917 = 120;
// 推理使用的内置模型与预测步长
static const string MODEL_917 = "timer_xl";
static const int PREDICT_LEN_917 = 10;

shared_ptr<Session> session_917;
bool isErrorTest_917 = false;

static string inferenceSql_917() {
    return "CALL INFERENCE(" + MODEL_917 + ", \"select s1 from " + DEVICE_917 +
           "\", predict_length=" + to_string(PREDICT_LEN_917) + ")";
}

class ModelInferenceGetDoubleTest : public ::testing::Test {
protected:
    void SetUp() override {
        try {
            session_917 = make_shared<Session>(kHost, kPort, kUser, kPass);
            session_917->open(false);
            // 干净准备：删除可能残留的库，重建并写入 INT64 序列（对应客户测点类型 int64）
            try {
                session_917->executeNonQueryStatement("delete database " + DB_917);
            } catch (exception&) { /* 库不存在忽略 */ }
            session_917->executeNonQueryStatement("CREATE DATABASE " + DB_917);
            session_917->executeNonQueryStatement(
                "CREATE TIMESERIES " + PATH_917 + " WITH DATATYPE=INT64,ENCODING=PLAIN");
            // 写入 >=96 个点，满足时序大模型最小输入长度
            string sql = "INSERT INTO " + DEVICE_917 + "(time,s1) VALUES";
            for (int t = 1; t <= POINT_NUM_917; ++t) {
                long v = (t * 7) % 50 + 10;
                sql += "(" + to_string(t) + "," + to_string(v) + ")";
                if (t != POINT_NUM_917) sql += ",";
            }
            session_917->executeNonQueryStatement(sql);
        } catch (IoTDBConnectionException& e) {
            isErrorTest_917 = true;
            ASSERT_EQ(false, isErrorTest_917) << "[Error] IoTDBConnectionException - " << string(e.what());
        } catch (IoTDBException& e) {
            isErrorTest_917 = true;
            ASSERT_EQ(false, isErrorTest_917) << "[Error] IoTDBException - " << string(e.what());
        } catch (exception& e) {
            isErrorTest_917 = true;
            ASSERT_EQ(false, isErrorTest_917) << "[Error] exception - " << string(e.what());
        }
    }

    void TearDown() override {
        try {
            session_917->executeNonQueryStatement("delete database " + DB_917);
            session_917->close();
        } catch (exception& e) {
            // 清理失败不阻断
            cerr << "[TearDown] " << string(e.what()) << endl;
        }
    }
};

// 一、复刻客户路径：CALL INFERENCE 后按 next()->toString() 迭代整张结果集，必须不抛
// "Unsupported operation: getDouble"（修复前此处崩溃）。
TEST_F(ModelInferenceGetDoubleTest, TestInferenceToStringNotThrow) {
    int rowCount = 0;
    try {
        unique_ptr<SessionDataSet> dataSet = session_917->executeQueryStatement(inferenceSql_917());
        ASSERT_NE(dataSet, nullptr) << "[FAIL] 推理结果 dataSet 为空";
        while (dataSet->hasNext()) {
            auto record = dataSet->next();        // 客户的崩溃点：next() 内部构造 RowRecord
            string line = record->toString();     // 客户的崩溃点：toString() 内部对 DOUBLE 列取值
            ASSERT_FALSE(line.empty()) << "[FAIL] 推理结果行 toString 为空";
            ++rowCount;
        }
    } catch (IoTDBException& e) {
        // 修复前会在此抛出 "Unsupported operation: getDouble"
        FAIL() << "[V2-917 FAIL] 迭代模型推理结果抛 IoTDBException: " << string(e.what());
    } catch (exception& e) {
        FAIL() << "[V2-917 FAIL] 迭代模型推理结果抛异常: " << string(e.what());
    }
    EXPECT_GT(rowCount, 0) << "[FAIL] 模型推理未返回结果行";
}

// 二、显式按 DOUBLE 读取推理结果 output 列：修复后 FloatColumn::getDouble 可用，
// getDoubleByIndex 不再抛 "Unsupported operation: getDouble"，且能取到有限数值。
TEST_F(ModelInferenceGetDoubleTest, TestInferenceGetDoubleReadable) {
    int rowCount = 0;
    try {
        unique_ptr<SessionDataSet> dataSet = session_917->executeQueryStatement(inferenceSql_917());
        ASSERT_NE(dataSet, nullptr);
        auto it = dataSet->getIterator();
        // 结果集仅含一列 output（模型预测值，声明为 DOUBLE，底层为 FLOAT）
        while (it.next()) {
            Optional<double> v = it.getDoubleByIndex(1);   // 修复前此处抛 Unsupported operation: getDouble
            ASSERT_TRUE(v.has_value()) << "[FAIL] 第 " << (rowCount + 1) << " 行 output 取值为空";
            double d = v.value();
            ASSERT_FALSE(std::isnan(d)) << "[FAIL] output 取到 NaN";
            ++rowCount;
        }
    } catch (IoTDBException& e) {
        FAIL() << "[V2-917 FAIL] getDouble 读取推理结果抛 IoTDBException: " << string(e.what());
    } catch (exception& e) {
        FAIL() << "[V2-917 FAIL] getDouble 读取推理结果抛异常: " << string(e.what());
    }
    EXPECT_GT(rowCount, 0) << "[FAIL] 模型推理未返回结果行";
}
