# Cpp-Session-Test说明

-----

## 一、介绍

本程序是基于开源版IoTDB C++客户端API进行测试的程序

### 1、目录

```txt
├── assert                      // 静态资源目录
├── build                       // 编译和生成可执行文件目录
├── client                      // 头文件和库文件目录
├── data                        // 测试数据目录
|   ├── table                   // 表模型测试数据目录
|   ├── tree                    // 树模型测试数据目录
├── test                        // 测试代码目录
|   ├── table                   // 表模型测试用例目录
|   ├── tree                    // 树模型测试用例目录
|   ├── CMakeLists.txt          // 测试代码目录的配置文件
|   ├── main.cpp                // 测试程序入口文件
├── CMakeLists.txt              // 主配置文件
├── CMakePresets.json           // CMake项目构建配置文件
├── compile.sh                  // 编译脚本
├── README.md                   // 程序说明文档
├── run.sh                      // 执行脚本
```

### 2、编译

拉取iotdb源码并编译生成c++头文件和库文件

```bash
# 拉取
git clone https://github.com/apache/iotdb.git
cd iotdb
./mvnw clean package -pl example/client-cpp-example -am -DskipTests -P with-cpp
# 存放头文件和库文件
头文件位于：iotdb-client/client-cpp/target/client-cpp-*-SNAPSHOT-cpp-linux-x86_64/include下，存放到测试程序根目录的client/include下
库文件位于：iotdb-client/client-cpp/target/client-cpp-*-SNAPSHOT-cpp-linux-x86_64/lib下，存放到测试程序根目录的client/lib下
```

第一次需要手动编译gtest测试工具生成gtest头文件和库文件，也需要存放到同样地方下

---

## 二、使用

在根目录下执行命令

```bash
# 编译
./compile.sh
# 执行
./run.sh
```

成功结果如下图所示

![image-20240912111738245](./assert/run_test1.png)

出现错误结果如下图所示

![image-20240912111707611](./assert/run_test2.png)

