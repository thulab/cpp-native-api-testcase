# Cpp 原生接口测试用例审查（PLM 导入前）

- 测试程序：`cpp-native-api-testcase`
- 仓库：`https://github.com/thulab/cpp-native-api-testcase.git`
- 分支：`feat/plm-onboarding`
- PLM 自动化类型：`session-cpp`
- 权威用例来源：`./main --gtest_list_tests`（76 执行机实测 11 条；111 报告同为 11 条 / 0 失败）
- 说明：本文件用于导入前人工审查；有分歧的条目在“待审查”列标注。

## 一、导入用例（11 条）

| # | gtest 目标 | PLM 用例名 | 功能点 | 代码行为说明 | 待审查 |
|---|---|---|---|---|---|
| 1 | `TableSelectTest.TestSelect1` | C++ 原生｜表模型-查询：SessionDataSet 与 RowRecord 字段校验 | 接口模块/C++ 原生接口/表模型-查询 | 表模型全类型查询：验证 SessionDataSet 列名、列类型、行数与 CSV 期望值；覆盖 TAG/ATTRIBUTE/FIELD 各类列。 | 无 |
| 2 | `TableSelectTest.TestSelect2` | C++ 原生｜表模型-查询：DataIterator 列名/类型/值一致性 | 接口模块/C++ 原生接口/表模型-查询 | 表模型全类型查询：通过 DataIterator 验证列名、列类型与返回值一致性。 | 无 |
| 3 | `TestSessionConnection.TestDefaultValue` | C++ 原生｜表模型-会话：默认参数连接 | 接口模块/C++ 原生接口/表模型-会话 | 使用全部默认参数建立表模型 Session，随后执行建库/建表/写入/查询/清理的闭环校验（validity1）。 | 无 |
| 4 | `TestSessionConnection.TestDatabaseParameter` | C++ 原生｜表模型-会话：指定 database 参数 | 接口模块/C++ 原生接口/表模型-会话 | 在 builder 指定 database=table_session_test2，验证不依赖 use 语句也能在该库内建表、写入、查询（validity2）。 | 无 |
| 5 | `TestSessionConnection.TestFetchSizeParameter` | C++ 原生｜表模型-会话：fetchSize 参数 | 接口模块/C++ 原生接口/表模型-会话 | 分别以 fetchSize=1 和 10000 建立 Session，验证两种提取大小下查询结果一致（validity1）。 | 无 |
| 6 | `TestSessionConnection.TestZoneIdParameter` | C++ 原生｜表模型-会话：zoneId 参数 | 接口模块/C++ 原生接口/表模型-会话 | 指定 zoneId=America/New_York，验证时区参数可被服务端接受且读写闭环正常（validity1）。 | 无 |
| 7 | `TestSessionConnection.TestAllParameter` | C++ 原生｜表模型-会话：全部参数组合 | 接口模块/C++ 原生接口/表模型-会话 | 同时指定 database、fetchSize 等全部参数，验证组合配置下建表/写入/查询闭环（validity2）。 | 无 |
| 8 | `TestSessionConnectionError.TestErrorUsernameAndPasswordParameter` | C++ 原生｜表模型-会话：错误用户名或密码 | 接口模块/C++ 原生接口/表模型-会话 | 使用错误用户名/口令建立表模型 Session，期望抛出异常且错误信息为 “804: The user error does not exist.”。 | 无 |
| 9 | `TestSessionConnectionError.TestErrorRpcPortParameter` | C++ 原生｜表模型-会话：错误端口 | 接口模块/C++ 原生接口/表模型-会话 | 使用错误 rpc 端口 0 建立 Session，期望抛出 “No valid endpoints available”。 | 无 |
| 10 | `TreeSelectTest.TestSelect1` | C++ 原生｜树模型-查询：SessionDataSet 与 RowRecord 字段校验 | 接口模块/C++ 原生接口/树模型-查询 | 树模型全类型查询：验证 SessionDataSet 列名、列类型、行数与 CSV 期望值。 | 无 |
| 11 | `TreeSelectTest.TestSelect2` | C++ 原生｜树模型-查询：DataIterator 列名/类型/值一致性 | 接口模块/C++ 原生接口/树模型-查询 | 树模型全类型查询：通过 DataIterator 验证列名、列类型与返回值一致性。 | 无 |

## 二、未导入的 3 条（构建层已排除）

| gtest 目标 | 未导入原因 |
|---|---|
| `InsertTest.test_all_datatype_null_same_time_and_device_and_device` | `test/CMakeLists.txt` 注释了 `table_insert_test.cpp`（注释写明“需要参考 select_test 更新用例”），二进制中不存在该测试。 |
| `ModelInferenceGetDoubleTest.TestInferenceToStringNotThrow` | `test/CMakeLists.txt` 默认注释 `model_inference_getdouble_test.cpp`，依赖 AINode/timer_xl，默认测试环境无 AINode。 |
| `ModelInferenceGetDoubleTest.TestInferenceGetDoubleReadable` | 同上。 |

## 三、审查结论

- 11 条用例名称与功能点路径均与测试代码行为一致，无命名分歧。
- 2 条错误路径用例（错误用户名/口令、错误端口）断言了具体错误文案；这是程序自身断言，不影响 PLM 用例命名，但若服务端文案变化需同步修改测试。
- 表模型/树模型查询用例依赖 `data/table/select_data1.csv`、`data/tree/select_data1.csv`，执行机上仓库需包含 data 目录。

## 四、PLM 真实验证（2026-11-05）

- 执行机：qa-fqx-76；K8s 实例由产品 Native 模板创建；PLM run **808 SUCCESS，11/11 passed**。
- 首轮 run 804/805/806/807 暴露并修复了两个 K8s 适配问题：
  1. **内部 DNS**：`Session` 默认 `enableAutoFetch=true` 会拉取节点列表并拿到 `*.svc.cluster.local`，执行机无法解析。已在 `plmTableBuilder` / `plmTreeSession` 中同时关闭 `enableAutoFetch` 与 `enableRedirections`。
  2. **DDL 在途 304**：连续执行同一批会话用例时，`drop database` 返回成功后服务端 DDL 仍在途，紧随的 `use` / `create table` 会返回 304。已把 `drop/create/use/create table/drop table` 统一收敛到 `executeWithRetry`（仅对 304 重试）。
- 说明：这两项是 K8s 共享实例下的适配，不影响 111 上本地单实例回归的执行语义。
