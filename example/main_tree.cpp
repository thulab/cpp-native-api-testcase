// /*
//  * title：集成测试
//  * describe：
//  * creator：肖林捷
//  * date：2024/9/11
//  */

// #include "Session.h"

// #include <cppunit/TestRunner.h> // 引入CppUnit测试框架中的TestRunner类，用于运行测试
// #include <cppunit/TestResult.h> // 引入TestResult类，用于存储测试结果
// #include <cppunit/TestResultCollector.h> // 引入TestResultCollector类，用于收集测试结果
// #include <cppunit/extensions/HelperMacros.h> // 引入辅助宏，用于简化测试用例的编写
// #include <cppunit/BriefTestProgressListener.h> // 引入BriefTestProgressListener类，用于在控制台输出测试进度
// #include <cppunit/extensions/TestFactoryRegistry.h> // 引入TestFactoryRegistry类，用于注册和查找测试用例
// #include <cppunit/TextOutputter.h> // 引入TextOutputter类，用于输出测试结果
// #include <cppunit/TestListener.h>

// using namespace std;
// Session *session;

// // 测试Session链接
// class SessionConnectTest_Normal : public CppUnit::TestFixture {
// public:
//      // 注册到TestCaller中
//      static CppUnit::TestSuite *suite()
//     {
//         CppUnit::TestSuite *testSuite = new CppUnit::TestSuite("SessionConnectTest_Normal");
//         using Caller = CppUnit::TestCaller<SessionConnectTest_Normal>;
//         testSuite->addTest(new Caller("test_Connection_Parameter_Normal", &SessionConnectTest_Normal::test_Connection_Parameter_Normal));
//         testSuite->addTest(new Caller("test_Connection_Paramete_Valuer_Normal", &SessionConnectTest_Normal::test_Connection_Paramete_Valuer_Normal));
//         return testSuite;
//     }

//     // 测试前的准备方法
//     void setUp(void) {}
//     // 测试后的清理方法
//     void tearDown(void) {}
    
//     // 测试不同参数数量连接数据库
// 	void test_Connection_Parameter_Normal()
// 	{
// 		// host和rpcPort参数
// 		session = new Session("127.0.0.1", 6667);
// 		session->open(false);
// 		session->close();
// 		delete session;
		
// 		// host、rpcPort、username和password参数
// 		session = new Session("127.0.0.1", 6667, "root", "root");
// 		session->open(false);
// 		session->close();
// 		delete session;

// 		// host、rpcPort、username、password、zoneId参数
// 		session = new Session("127.0.0.1", 6667, "root", "root", "GMT+8", 10000);
// 		session->open(false);
// 		session->close();
// 		delete session;

// 		// host、rpcPort、username、password、zoneId参数
// 		session = new Session("127.0.0.1", 6667, "root", "root", "", 10000);
// 		session->open(false);
// 		session->close();
// 		delete session;
// 	}

// 	// 测试不同参数值连接数据库
// 	void test_Connection_Paramete_Valuer_Normal()
// 	{
// 		// zoneId参数取不同值
// 		// GMT+8
// 		session = new Session("127.0.0.1", 6667, "root", "root", "GMT+8");
// 		session->open(false);
// 		session->close();
// 		delete session;
// 		// GMT+0
// 		session = new Session("127.0.0.1", 6667, "root", "root", "GMT+0");
// 		session->open(false);
// 		session->close();
// 		delete session;
// 		// GMT+14
// 		session = new Session("127.0.0.1", 6667, "root", "root", "GMT+14");
// 		session->open(false);
// 		session->close();
// 		delete session;
// 		// 为空
// 		session = new Session("127.0.0.1", 6667, "root", "root", "");
// 		session->open(false);
// 		session->close();
// 		delete session;
// 		// 使用默认
// 		session = new Session("127.0.0.1", 6667, "root", "root");
// 		session->open(false);
// 		session->close();
// 		delete session;
// 	}
// };

// // 测试数据库操作
// class DataBaseTest_Normal : public CppUnit::TestFixture {
// public:
//      // 注册组件到TestCaller中
//      static CppUnit::TestSuite *suite()
//     {
//         CppUnit::TestSuite *testSuite = new CppUnit::TestSuite("DataBaseTest_Normal");
//         using Caller = CppUnit::TestCaller<DataBaseTest_Normal>;
//         testSuite->addTest(new Caller("test_SetStorageGroup_Normal", &DataBaseTest_Normal::test_SetStorageGroup_Normal));
//         testSuite->addTest(new Caller("test_DeleteStorageGroups_Normal", &DataBaseTest_Normal::test_DeleteStorageGroups_Normal));
//         return testSuite;
//     }

//     // 测试前的准备方法
//     void setUp(void) 
//     {
//         // 创建session连接
// 		session = new Session("127.0.0.1", 6667);
// 		session->open(false);
//     }
//     // 测试后的清理方法
//     void tearDown(void) 
//     {
//         session->close();
//         delete session;
//     }

// 	// 测试删除数据库
// 	void test_DeleteStorageGroups_Normal() 
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("show databases");
// 		// 用于存储数据库名
// 		vector<string> storageGroups;
// 		// 设置数据记录的数量为1024
// 		dataSet->setFetchSize(1024);
// 		// 判断是否还有有效值行
// 		while (dataSet->hasNext()) {
// 			// 有则获取本行第一列数据库名数据并存入storageGroups中
// 			storageGroups.emplace_back(dataSet->next()->fields[0].stringV);
// 		}
// 		// 无则关闭dataSet，因为是智能指针，出了花括号就会关闭，所以屏蔽
// 		//dataSet->closeOperationHandle();
// 		// 删除数据库
// 		session->deleteStorageGroups(storageGroups);
// 	}

// 	// 测试创建数据库
// 	void test_SetStorageGroup_Normal()
// 	{
// 		// 创建数据库
// 		// 纯字母
// 		session->setStorageGroup("root.a");
// 		session->setStorageGroup("root.A");
// 		session->setStorageGroup("root.qwertyuiopasdfghjklzxcvbnm");
// 		session->setStorageGroup("root.qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm");
// 		// 纯下划线
// 		session->setStorageGroup("root._");
// 		session->setStorageGroup("root.___________________________________________________________");
// 		// 字母、数字和下划线组合命名
// 		session->setStorageGroup("root.a1");
// 		session->setStorageGroup("root.11a");
// 		session->setStorageGroup("root.09d___");
// 		session->setStorageGroup("root.___35ge___");
// 		session->setStorageGroup("root.__123456_35ge_1231231adadawdawda213123adawdad__");
// 		// 带引号的字母、数字、中文组合命名
// 		session->setStorageGroup("root.`1231audh1ua12312312`");
// 		//session->setStorageGroup("root.`没没没`"); // 暂且不支持
// 		session->setStorageGroup("root.`123112312`");
// 		// 多个点
// 		session->setStorageGroup("root.1a21.1b");
// 		session->setStorageGroup("root.`1231daaudadhu12312`.1__b");
// 		// 关键字
// 		session->setStorageGroup("root.database");
// 		session->setStorageGroup("root.session");
// 		session->setStorageGroup("root.AND");
// 	}
// };

// // 测试时间序列操作
// class TimeseriesTest_Normal : public CppUnit::TestCase {

// 	string database = "root.db"; // 数据库名
// 	string path; // 时间序列
// 	string deviceId; // 设备名
// 	string measurement; // 物理量

// public:

// 	 // 注册组件到TestCaller中
//      static CppUnit::TestSuite *suite()
//     {
//         CppUnit::TestSuite *testSuite = new CppUnit::TestSuite("TimeseriesTest_Normal");
//         using Caller = CppUnit::TestCaller<TimeseriesTest_Normal>;
//         testSuite->addTest(new Caller("test_CreateTimeseries_Normal", &TimeseriesTest_Normal::test_CreateTimeseries_Normal));
//         testSuite->addTest(new Caller("test_CreateMultiTimeseries_Normal", &TimeseriesTest_Normal::test_CreateMultiTimeseries_Normal));
// 		testSuite->addTest(new Caller("test_CreateAlignedTimeseries_Normal", &TimeseriesTest_Normal::test_CreateAlignedTimeseries_Normal));
//         return testSuite;
//     }

//     // 测试前的准备方法
//     void setUp(void) 
// 	{
// 		// 创建session连接
// 		session = new Session("127.0.0.1", 6667, "root", "root");
// 		session->open(false);
// 		// 创建数据库
// 		session->setStorageGroup(database);
// 	}
//     // 测试后的清理方法
//     void tearDown(void) 
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("show databases");
// 		// 用于存储数据库名
// 		vector<string> storageGroups;
// 		// 设置数据记录的数量为1024
// 		dataSet->setFetchSize(1024);
// 		// 判断是否还有有效值行
// 		while (dataSet->hasNext()) {
// 			// 有则获取本行第一列数据库名数据并存入storageGroups中
// 			storageGroups.emplace_back(dataSet->next()->fields[0].stringV);
// 		}
// 		// 关闭dataSet
// 		dataSet->closeOperationHandle();
// 		// 删除数据库
// 		session->deleteStorageGroups(storageGroups);
// 		// 关闭session
// 		session->close();
// 		delete session;
// 	}

// 	// 测试创建时间序列
// 	void test_CreateTimeseries_Normal()
// 	{
// 		// 各种命名
// 		// 纯字母
// 		deviceId = database + '.' + "qwertyuIopasdfghjklzxCvbnm";
// 		measurement = "qwertyuIopasdfghjklzxCvbnm";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// 纯下划线
// 		deviceId = database + '.' + "____";
// 		measurement = "____";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// 字母、数字和下划线组合命名
// 		deviceId = database + '.' + "_1234567890_aWa__";
// 		measurement = "_1234567890_aWa__";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// 带引号的字母、数字、中文组合命名
// 		deviceId = database + '.' + "`123`";
// 		measurement = "`123`";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		deviceId = database + '.' + "`123Abc我的___`";
// 		measurement = "`123Abc我的___`";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// 多个点
// 		deviceId = database + '.' + "`qq123ABCc___`" + '.' + "`qq123ABCc___`" + '.' + "`qq123ABCc___`";
// 		measurement = "`qq123ABCc___`";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// 64位长度
// 		deviceId = database + '.' + "12345678901234567890123456789012345678901234567890a";
// 		measurement = "12345678901234567890123456789012345678901234567890a";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// 关键字
// 		deviceId = database + '.' + "ADD";
// 		measurement = "ADD";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		deviceId = database + '.' + "WHERE";
// 		measurement = "WHERE";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		deviceId = database + '.' + "boolean";
// 		measurement = "boolean";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
		
// 		// 各种数据类型
// 		// Boolean
// 		deviceId = database + '.' + "BOOLEAN";
// 		measurement = "BOOLEAN";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// int32
// 		deviceId = database + '.' + "INT32";
// 		measurement = "INT32";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// int64
// 		deviceId = database + '.' + "INT64";
// 		measurement = "INT64";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT64, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// float
// 		deviceId = database + '.' + "FLOAT";
// 		measurement = "FLOAT";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::FLOAT, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// double
// 		deviceId = database + '.' + "DOUBLE";
// 		measurement = "DOUBLE";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::DOUBLE, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// text
// 		deviceId = database + '.' + "TEXT";
// 		measurement = "TEXT";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::TEXT, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// blob不支持
// 		/*deviceId = database + '.' + "BLOB";
// 		measurement = "BLOB";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BLOB, TSEncoding::PLAIN, CompressionType::SNAPPY);*/
// 		// string不支持
// 		/*deviceId = database + '.' + "STRING";
// 		measurement = "STRING";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::STRING, TSEncoding::PLAIN, CompressionType::SNAPPY);*/
// 		// date不支持
// 		/*deviceId = database + '.' + "DATE";
// 		measurement = "DATE";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::DATE, TSEncoding::PLAIN, CompressionType::SNAPPY);*/
// 		// TIMESTAMP不支持
// 		/*deviceId = database + '.' + "TIMESTAMP";
// 		measurement = "TIMESTAMP";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::TIMESTAMP, TSEncoding::PLAIN, CompressionType::SNAPPY);*/

// 		// 各种编码格式
// 		// PLAIN
// 		deviceId = database + '.' + "PLAIN";
// 		measurement = "PLAIN";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::BOOLEAN, TSEncoding::PLAIN, CompressionType::SNAPPY);
// 		// DICTIONARY
// 		deviceId = database + '.' + "DICTIONARY";
// 		measurement = "DICTIONARY";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::TEXT, TSEncoding::DICTIONARY, CompressionType::SNAPPY);
// 		// RLE
// 		deviceId = database + '.' + "RLE";
// 		measurement = "RLE";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::RLE, CompressionType::SNAPPY);
// 		// TS_2DIFF
// 		deviceId = database + '.' + "TS_2DIFF";
// 		measurement = "TS_2DIFF";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::TS_2DIFF, CompressionType::SNAPPY);
// 		// GORILLA
// 		deviceId = database + '.' + "GORILLA";
// 		measurement = "GORILLA";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::GORILLA, CompressionType::SNAPPY);
// 		// ZIGZAG
// 		deviceId = database + '.' + "ZIGZAG";
// 		measurement = "ZIGZAG";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::ZIGZAG, CompressionType::SNAPPY);
// 		// CHIMP
// 		deviceId = database + '.' + "CHIMP";
// 		measurement = "CHIMP";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::CHIMP, CompressionType::SNAPPY);
// 		// SPRINTZ
// 		deviceId = database + '.' + "SPRINTZ";
// 		measurement = "SPRINTZ";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::SPRINTZ, CompressionType::SNAPPY);
// 		// RLBE
// 		deviceId = database + '.' + "RLBE";
// 		measurement = "RLBE";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::RLBE, CompressionType::SNAPPY);

// 		// 各种压缩类型
// 		// UNCOMPRESSED
// 		deviceId = database + '.' + "UNCOMPRESSED";
// 		measurement = "UNCOMPRESSED";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::RLBE, CompressionType::UNCOMPRESSED);
// 		// SNAPPY 
// 		deviceId = database + '.' + "SNAPPY";
// 		measurement = "SNAPPY";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::RLBE, CompressionType::SNAPPY);
// 		// LZ4 
// 		deviceId = database + '.' + "LZ4";
// 		measurement = "LZ4";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::RLBE, CompressionType::LZ4);
// 		// GZIP 
// 		deviceId = database + '.' + "GZIP";
// 		measurement = "GZIP";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::RLBE, CompressionType::GZIP);
// 		// ZSTD 
// 		deviceId = database + '.' + "ZSTD";
// 		measurement = "ZSTD";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::RLBE, CompressionType::ZSTD);
// 		// LZMA2 
// 		deviceId = database + '.' + "LZMA2";
// 		measurement = "LZMA2";
// 		path = deviceId + '.' + measurement;
// 		session->createTimeseries(path, TSDataType::INT32, TSEncoding::RLBE, CompressionType::LZMA2);
// 	}

// 	// 测试创建多条时间序列
// 	void test_CreateMultiTimeseries_Normal()
// 	{
// 		// 各种命名
// 		vector<string> paths;
// 		deviceId = database + '.' + "_1234wa567890_aWa__";
// 		measurement = "_1234wa567890_aWa__";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		deviceId = database + '.' + "`123Abdadc我的___`";
// 		measurement = "`123Abdadc我的___`";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		deviceId = database + '.' + "`qq123AadaBCc___`" + '.' + "`qq123AadaBCc___`" + '.' + "`qq123AadaBCc___`";
// 		measurement = "`qq123AadaBCc___`";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		deviceId = database + '.' + "1234567890123456789167890245678901234567890a";
// 		measurement = "1234567890123456789167890245678901234567890a";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		deviceId = database + '.' + "______";
// 		measurement = "______";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		deviceId = database + '.' + "void";
// 		measurement = "void";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		deviceId = database + '.' + "`122223`";
// 		measurement = "`122223`";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		deviceId = database + '.' + "LZMa2";
// 		measurement = "LZMa2";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		deviceId = database + '.' + "QWeRtyuIopasdfghjklzxCvbnm";
// 		measurement = "QWeRtyuIopasdfghjklzxCvbnm";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		// 各种数据类型
// 		vector<TSDataType::TSDataType> dataTypes;
// 		dataTypes.emplace_back(TSDataType::INT32);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::FLOAT);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		dataTypes.emplace_back(TSDataType::TEXT);
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		// 各种编码
// 		vector<TSEncoding::TSEncoding> encodings;
// 		encodings.emplace_back(TSEncoding::ZIGZAG);
// 		encodings.emplace_back(TSEncoding::RLBE);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::GORILLA);
// 		encodings.emplace_back(TSEncoding::RLBE);
// 		encodings.emplace_back(TSEncoding::DICTIONARY);
// 		encodings.emplace_back(TSEncoding::RLE);
// 		encodings.emplace_back(TSEncoding::CHIMP);
// 		encodings.emplace_back(TSEncoding::SPRINTZ);
// 		// 各种压缩方式
// 		vector<CompressionType::CompressionType> compressors;
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::UNCOMPRESSED);
// 		compressors.emplace_back(CompressionType::SNAPPY);
// 		compressors.emplace_back(CompressionType::LZ4);
// 		compressors.emplace_back(CompressionType::ZSTD);
// 		compressors.emplace_back(CompressionType::LZMA2);
// 		compressors.emplace_back(CompressionType::LZMA2);
// 		compressors.emplace_back(CompressionType::LZMA2);
// 		compressors.emplace_back(CompressionType::LZMA2);
// 		session->createMultiTimeseries(paths, dataTypes, encodings, compressors,NULL,NULL,NULL,NULL);

// 		test_Show_TimeSeries();
// 	}
    
// 	// 测试创建对齐时间序列
// 	void test_CreateAlignedTimeseries_Normal()
// 	{
// 		deviceId = database + '.' + "qwertyuwawsdfghjklzxCvbnm";
// 		vector<std::string> measurements;
// 		// 各种命名
// 		measurements.emplace_back("_1234wa523467890_aWa__");
// 		measurements.emplace_back("`123Abd234adc我的___`");
// 		measurements.emplace_back("`12334234`");
// 		measurements.emplace_back("1234567890123456789167890245678901234567890a");
// 		measurements.emplace_back("____________");
// 		measurements.emplace_back("while");
// 		measurements.emplace_back("LZma2");
// 		measurements.emplace_back("QWeRtyuIopasdfghjklzxCvbnmQWeRtyuIopasdfghjklzxCvbnm");
// 		measurements.emplace_back("_123wad567890_aWa__");
// 		// 各种数据类型
// 		vector<TSDataType::TSDataType> dataTypes;
// 		dataTypes.emplace_back(TSDataType::INT32);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::FLOAT);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		dataTypes.emplace_back(TSDataType::TEXT);
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		// 各种编码
// 		vector<TSEncoding::TSEncoding> encodings;
// 		encodings.emplace_back(TSEncoding::ZIGZAG);
// 		encodings.emplace_back(TSEncoding::RLBE);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::GORILLA);
// 		encodings.emplace_back(TSEncoding::RLBE);
// 		encodings.emplace_back(TSEncoding::DICTIONARY);
// 		encodings.emplace_back(TSEncoding::RLE);
// 		encodings.emplace_back(TSEncoding::CHIMP);
// 		encodings.emplace_back(TSEncoding::SPRINTZ);
// 		// 各种压缩方式
// 		vector<CompressionType::CompressionType> compressors;
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::UNCOMPRESSED);
// 		compressors.emplace_back(CompressionType::SNAPPY);
// 		compressors.emplace_back(CompressionType::LZ4);
// 		compressors.emplace_back(CompressionType::ZSTD);
// 		compressors.emplace_back(CompressionType::LZMA2);
// 		compressors.emplace_back(CompressionType::LZMA2);
// 		compressors.emplace_back(CompressionType::LZMA2);
// 		compressors.emplace_back(CompressionType::LZMA2);
// 		session->createAlignedTimeseries(deviceId, measurements, dataTypes, encodings, compressors);

// 		test_Show_TimeSeries();
// 	}

// 	// 测试查询时间序列
// 	void test_Show_TimeSeries()
// 	{ 
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("SHOW TIMESERIES");
// 		// 获取每列标题并输出
// 		// for (const string& name : dataSet->getColumnNames()) {
// 		// 	cout << name << "  ";
// 		// }
// 		// cout << endl;
// 		// 设置数据记录的数量为1024
// 		//dataSet->setFetchSize(1024);
// 		// 判断是否还有有效值行，有则输出
// 		// while (dataSet->hasNext()) {
// 		// 	cout << dataSet->next()->toString();
// 		// }
// 		// cout << endl;
// 	}
// };

// // 测试模板操作
// class SchemaTemplateTest_Normal : public CppUnit::TestCase {

// 	string template_name; // 模板名
// 	string database = "root.db"; // 数据库名
// 	string deviceId = database + '.' + "d1"; // 设备名
// 	string prefix_path = deviceId; // 前缀名
// 	string name_; // 物理量名
// 	string measurement; // 新增的物理量名
// 	TSDataType::TSDataType dataType; // 新增的数据类型
// 	TSEncoding::TSEncoding encoding; // 新增的编码格式
// 	CompressionType::CompressionType compressor; // 新增的压缩格式
// 	vector<std::string> measurements;
// 	vector<TSDataType::TSDataType> dataTypes;
// 	vector<TSEncoding::TSEncoding> encodings;
// 	vector<CompressionType::CompressionType> compressors;
// 	string path; // 模板子树路径

// 	TSDataType::TSDataType data_type_; // 数据类型
// 	TSEncoding::TSEncoding encoding_; // 编码格式
// 	CompressionType::CompressionType compression_type_; // 压缩类型

// 	bool is_aligned; // 是否为对齐

// public:

// 	 // 注册组件到TestCaller中
//      static CppUnit::TestSuite *suite()
//     {
//         CppUnit::TestSuite *testSuite = new CppUnit::TestSuite("SchemaTemplateTest_Normal");
//         using Caller = CppUnit::TestCaller<SchemaTemplateTest_Normal>;
//         testSuite->addTest(new Caller("test_SchemaTemplateTest_Normal", &SchemaTemplateTest_Normal::test_SchemaTemplateTest_Normal));
//         return testSuite;
//     }

//     // 测试前的准备方法
//     void setUp(void) 
// 	{
// 		// 创建session连接
// 		session = new Session("127.0.0.1", 6667, "root", "root");
// 		session->open(false);
// 		// 创建数据库
// 		session->setStorageGroup(database);
// 	}
//     // 测试后的清理方法
//     void tearDown(void) 
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("show databases");
// 		// 用于存储数据库名
// 		vector<string> storageGroups;
// 		// 设置数据记录的数量为1024
// 		dataSet->setFetchSize(1024);
// 		// 判断是否还有有效值行
// 		while (dataSet->hasNext()) {
// 			// 有则获取本行第一列数据库名数据并存入storageGroups中
// 			storageGroups.emplace_back(dataSet->next()->fields[0].stringV);
// 		}
// 		// 关闭dataSet
// 		dataSet->closeOperationHandle();
// 		// 删除数据库
// 		session->deleteStorageGroups(storageGroups);
// 		// 关闭session
// 		session->close();
// 		delete session;
// 	}

// 	// 查询设备模板
// 	void test_showSchemaTemplate(string sql)
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		// unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement(sql);
// 		// // 获取每列标题并输出
// 		// for (const string& name : dataSet->getColumnNames()) {
// 		// 	cout << name << "  ";
// 		// }
// 		// cout << endl;
// 		// // 设置数据记录的数量为1024
// 		// dataSet->setFetchSize(1024);
// 		// // 判断是否还有有效值行，有则输出
// 		// while (dataSet->hasNext()) {
// 		// 	cout << dataSet->next()->toString();
// 		// }
// 		// cout << endl;
// 		// 无则关闭dataSet，因为是智能指针，出了花括号就会关闭，所以屏蔽
// 		//dataSet->closeOperationHandle();
// 	}

// 	// 创建模板对象
// 	Template templ()
// 	{
// 		is_aligned = true;
// 		// 创建模板对象
// 		Template templ(template_name, is_aligned);
// 		// 各种命名
// 		data_type_ = TSDataType::INT32;
// 		encoding_ = TSEncoding::PLAIN;
// 		compression_type_ = CompressionType::GZIP;
// 		name_ = "qwertyuiopasdfghjklzxcvbnm";
// 		MeasurementNode child1(name_, data_type_, encoding_, compression_type_);
// 		name_ = "__________";
// 		MeasurementNode child2(name_, data_type_, encoding_, compression_type_);
// 		name_ = "12345";
// 		MeasurementNode child3(name_, data_type_, encoding_, compression_type_);
// 		name_ = "!@#￥￥￥";
// 		MeasurementNode child4(name_, data_type_, encoding_, compression_type_); // 可以创建，不符合预期，应该不可以创建
// 		name_ = "没没没";
// 		MeasurementNode child5(name_, data_type_, encoding_, compression_type_); // 可以创建，不符合预期，应该不可以创建
// 		name_ = "";
// 		MeasurementNode child6(name_, data_type_, encoding_, compression_type_); // 可以创建，不符合预期，应该不可以创建
// 		name_ = "root";
// 		MeasurementNode child7(name_, data_type_, encoding_, compression_type_); // 可以创建，不符合预期，应该不可以创建
// 		name_ = "`没没没没没没`";
// 		MeasurementNode child8(name_, data_type_, encoding_, compression_type_); // 可以创建，但是结果不符合预期，结果应该不带引号
// 		templ.addToTemplate(child2);
// 		templ.addToTemplate(child1);
// 		templ.addToTemplate(child3);
// 		templ.addToTemplate(child4);
// 		templ.addToTemplate(child5);
// 		templ.addToTemplate(child6);
// 		templ.addToTemplate(child7);
// 		templ.addToTemplate(child8);

// 		// 各种类型
// 		name_ = "t1";
// 		encoding_ = TSEncoding::PLAIN;
// 		compression_type_ = CompressionType::GZIP;
// 		data_type_ = TSDataType::INT32;
// 		MeasurementNode child11(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t2";
// 		data_type_ = TSDataType::BOOLEAN;
// 		MeasurementNode child12(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t3";
// 		data_type_ = TSDataType::INT64;
// 		MeasurementNode child13(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t4";
// 		data_type_ = TSDataType::FLOAT;
// 		MeasurementNode child14(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t5";
// 		data_type_ = TSDataType::DOUBLE;
// 		MeasurementNode child15(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t6";
// 		data_type_ = TSDataType::TEXT;
// 		MeasurementNode child16(name_, data_type_, encoding_, compression_type_);
// 		templ.addToTemplate(child12);
// 		templ.addToTemplate(child11);
// 		templ.addToTemplate(child13);
// 		templ.addToTemplate(child14);
// 		templ.addToTemplate(child15);
// 		templ.addToTemplate(child16);

// 		// 各种编码
// 		name_ = "t21";
// 		encoding_ = TSEncoding::PLAIN;
// 		compression_type_ = CompressionType::GZIP;
// 		data_type_ = TSDataType::INT32;
// 		MeasurementNode child31(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t22";
// 		data_type_ = TSDataType::BOOLEAN;
// 		encoding_ = TSEncoding::RLE;
// 		MeasurementNode child32(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t23";
// 		data_type_ = TSDataType::INT64;
// 		encoding_ = TSEncoding::TS_2DIFF;
// 		MeasurementNode child33(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t24";
// 		data_type_ = TSDataType::FLOAT;
// 		encoding_ = TSEncoding::GORILLA;
// 		MeasurementNode child34(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t25";
// 		data_type_ = TSDataType::DOUBLE;
// 		encoding_ = TSEncoding::CHIMP;
// 		MeasurementNode child35(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t26";
// 		data_type_ = TSDataType::TEXT;
// 		encoding_ = TSEncoding::DICTIONARY;
// 		MeasurementNode child36(name_, data_type_, encoding_, compression_type_);
// 		templ.addToTemplate(child32);
// 		templ.addToTemplate(child31);
// 		templ.addToTemplate(child33);
// 		templ.addToTemplate(child34);
// 		templ.addToTemplate(child35);
// 		templ.addToTemplate(child36);

// 		// 各种压缩
// 		name_ = "t11";
// 		encoding_ = TSEncoding::PLAIN;
// 		compression_type_ = CompressionType::GZIP;
// 		data_type_ = TSDataType::INT32;
// 		MeasurementNode child21(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t12";
// 		data_type_ = TSDataType::BOOLEAN;
// 		compression_type_ = CompressionType::UNCOMPRESSED;
// 		MeasurementNode child22(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t13";
// 		data_type_ = TSDataType::INT64;
// 		compression_type_ = CompressionType::SNAPPY;
// 		MeasurementNode child23(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t14";
// 		data_type_ = TSDataType::FLOAT;
// 		compression_type_ = CompressionType::LZ4;
// 		MeasurementNode child24(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t15";
// 		data_type_ = TSDataType::DOUBLE;
// 		compression_type_ = CompressionType::LZMA2;
// 		MeasurementNode child25(name_, data_type_, encoding_, compression_type_);
// 		name_ = "t16";
// 		data_type_ = TSDataType::TEXT;
// 		compression_type_ = CompressionType::ZSTD;
// 		MeasurementNode child26(name_, data_type_, encoding_, compression_type_);
// 		templ.addToTemplate(child22);
// 		templ.addToTemplate(child21);
// 		templ.addToTemplate(child23);
// 		templ.addToTemplate(child24);
// 		templ.addToTemplate(child25);
// 		templ.addToTemplate(child26);
		
// 		return templ;
// 	}

// 	// 测试元数据模板操作
// 	void test_SchemaTemplate()
// 	{
// 		// 创建模板
// 		session->createSchemaTemplate(templ());
// 		// 查看设备模板
// 		test_showSchemaTemplate("show device templates");
// 		// 查看某个设备模板下的物理量
// 		test_showSchemaTemplate("show nodes in device template " + template_name);
// 		// 添加物理量
// 		// 新增一组对齐的物理量	修改模板方法不支持报错
// 		/*measurement = "newM2 ";
// 		dataType = TSDataType::BOOLEAN;
// 		encoding = TSEncoding::RLE;
// 		compressor = CompressionType::UNCOMPRESSED;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM3 ";
// 		dataType = TSDataType::FLOAT;
// 		encoding = TSEncoding::SPRINTZ;
// 		compressor = CompressionType::SNAPPY;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM4 ";
// 		dataType = TSDataType::INT32;
// 		encoding = TSEncoding::TS_2DIFF;
// 		compressor = CompressionType::LZ4;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM5 ";
// 		dataType = TSDataType::INT64;
// 		encoding = TSEncoding::ZIGZAG;
// 		compressor = CompressionType::GZIP;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM6 ";
// 		dataType = TSDataType::TEXT;
// 		encoding = TSEncoding::DICTIONARY;
// 		compressor = CompressionType::ZSTD;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM7 ";
// 		dataType = TSDataType::DOUBLE;
// 		encoding = TSEncoding::GORILLA;
// 		compressor = CompressionType::LZMA2;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		session->addAlignedMeasurementsInTemplate(template_name, measurements, dataTypes, encodings, compressors);*/
// 		// 新增一个对齐物理量		修改模板方法不支持报错
// 		/*measurement = "newM1 ";
// 		dataType = TSDataType::BOOLEAN;
// 		encoding = TSEncoding::PLAIN;
// 		compressor = CompressionType::GZIP;
// 		session->addAlignedMeasurementsInTemplate(template_name, measurement, dataType, encoding, compressor);*/
// 		// 新增一个不对齐物理量	修改模板方法不支持报错
// 		/*measurement = "newM1 ";
// 		dataType = TSDataType::BOOLEAN;
// 		encoding = TSEncoding::PLAIN;
// 		compressor = CompressionType::GZIP;
// 		session->addUnalignedMeasurementsInTemplate(template_name, measurement, dataType, encoding, compressor);*/
// 		// 新增一组不对齐的物理量	修改模板方法不支持报错
// 		/*measurement = "newM2 ";
// 		dataType = TSDataType::BOOLEAN;
// 		encoding = TSEncoding::RLE;
// 		compressor = CompressionType::UNCOMPRESSED;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM3 ";
// 		dataType = TSDataType::FLOAT;
// 		encoding = TSEncoding::SPRINTZ;
// 		compressor = CompressionType::SNAPPY;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM4 ";
// 		dataType = TSDataType::INT32;
// 		encoding = TSEncoding::TS_2DIFF;
// 		compressor = CompressionType::LZ4;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM5 ";
// 		dataType = TSDataType::INT64;
// 		encoding = TSEncoding::ZIGZAG;
// 		compressor = CompressionType::GZIP;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM6 ";
// 		dataType = TSDataType::TEXT;
// 		encoding = TSEncoding::DICTIONARY;
// 		compressor = CompressionType::ZSTD;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		measurement = "newM7 ";
// 		dataType = TSDataType::DOUBLE;
// 		encoding = TSEncoding::GORILLA;
// 		compressor = CompressionType::LZMA2;
// 		measurements.push_back(measurement);
// 		dataTypes.push_back(dataType);
// 		encodings.push_back(encoding);
// 		compressors.push_back(compressor);
// 		session->addUnalignedMeasurementsInTemplatetemplate_name, measurements, dataTypes, encodings, compressors);*/
// 		// 删除一个节点及其子树	修改模板方法不支持报错
// 		/*path = deviceId + '.' + name_;
// 		cout << path << endl;
// 		session->deleteNodeInTemplate(template_name, path);*/
// 		// 挂载模板
// 		session->setSchemaTemplate(template_name, prefix_path);
// 		// 查看挂载了某个设备模板的路径
// 		test_showSchemaTemplate("show paths set device template " + template_name);
// 		// 卸载模板
// 		session->unsetSchemaTemplate(prefix_path, template_name);
// 		// 删除模板
// 		session->executeNonQueryStatement("drop device template " + template_name);
// 	}

// 	// 模板测试
// 	void test_SchemaTemplateTest_Normal()
// 	{
// 		// 各种命名
// 		template_name = "qwertyuiopasdfghjklzxcvb";
// 		test_SchemaTemplate();
// 		template_name = "__________";
// 		test_SchemaTemplate();
// 		template_name = "abcd1";
// 		test_SchemaTemplate();
// 		template_name = "___abcd1__";
// 		test_SchemaTemplate();
// 		template_name = "___231__";
// 		test_SchemaTemplate();
// 		template_name = "`123`";
// 		test_SchemaTemplate();
// 		template_name = "`没没没`";
// 		test_SchemaTemplate();
// 		template_name = "for";
// 		test_SchemaTemplate();
// 		/*template_name = "TIME"; // 可以创建模板，不符合预期
// 		test_SchemaTemplate();*/
// 		/*template_name = "TIMESTAMP"; // 可以创建模板，不符合预期
// 		test_SchemaTemplate();*/
// 		/*template_name = "time"; // 可以创建模板，不符合预期
// 		test_SchemaTemplate();*/
// 		//template_name = "timestamp"; // 可以创建模板，不符合预期
// 		//test_SchemaTemplate();
// 		//template_name = "ROOT"; // 可以创建模板，不符合预期
// 		//test_SchemaTemplate();
// 		//template_name = "root"; // 可以创建模板，不符合预期
// 		//test_SchemaTemplate();
// 	}
	
// };

// // 测试数据写入-Tablet
// class InsertTabletTest_Normal : public CppUnit::TestCase {

//     string database = "root.db"; // 数据库名
// 	string deviceId; // 设备名
// 	string measurement; // 物理量
// 	string path; // 时间序列

// 	size_t rowIndex; // 需要写入数据值所在的列索引
// 	bool booleanValue; // 用于存储boolean类型的值
// 	int int32Value; // 用于存储int32类型的值
// 	int64_t int64Value; // 用于存储int32类型的值
// 	float floatValue; // 用于存储float类型的值
// 	double doubleValue; // 用于存储double类型的值
// 	string textValue; // 用于存储text类型的值

// 	int num = 1; // 编号，用于每次调用创建时间序列函数创建的deviceId_Num不一样

// public:

// 	// 注册组件到TestCaller中
//      static CppUnit::TestSuite *suite()
//     {
//         CppUnit::TestSuite *testSuite = new CppUnit::TestSuite("InsertTabletTest_Normal");
//         using Caller = CppUnit::TestCaller<InsertTabletTest_Normal>;
//         testSuite->addTest(new Caller("test_InsertTablet_Normal", &InsertTabletTest_Normal::test_InsertTablet_Normal));
// 		testSuite->addTest(new Caller("test_InsertAlignedTablet_Normal", &InsertTabletTest_Normal::test_InsertAlignedTablet_Normal));
// 		testSuite->addTest(new Caller("test_InsertAlignedTablets_Normal", &InsertTabletTest_Normal::test_InsertAlignedTablets_Normal));
// 		testSuite->addTest(new Caller("test_InsertTabletNoTS_Normal", &InsertTabletTest_Normal::test_InsertTabletNoTS_Normal));
// 		testSuite->addTest(new Caller("test_InsertAlignedTabletNoATS_Normal", &InsertTabletTest_Normal::test_InsertAlignedTabletNoATS_Normal));
// 		testSuite->addTest(new Caller("test_InsertTabletToATS_Normal", &InsertTabletTest_Normal::test_InsertTabletToATS_Normal));
// 		testSuite->addTest(new Caller("test_InsertAlignedTabletToTS_Normal", &InsertTabletTest_Normal::test_InsertAlignedTabletToTS_Normal));
//         return testSuite;
//     }

//     // 测试前的准备方法
//     void setUp(void) 
// 	{
// 		// 创建session连接
// 		session = new Session("127.0.0.1", 6667, "root", "root");
// 		session->open(false);
// 		// 创建数据库
// 		session->setStorageGroup(database);
// 	}
//     // 测试后的清理方法
//     void tearDown(void) 
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("show databases");
// 		// 用于存储数据库名
// 		vector<string> storageGroups;
// 		// 设置数据记录的数量为1024
// 		dataSet->setFetchSize(1024);
// 		// 判断是否还有有效值行
// 		while (dataSet->hasNext()) {
// 			// 有则获取本行第一列数据库名数据并存入storageGroups中
// 			storageGroups.emplace_back(dataSet->next()->fields[0].stringV);
// 		}
// 		// 关闭dataSet
// 		dataSet->closeOperationHandle();
// 		// 删除数据库
// 		session->deleteStorageGroups(storageGroups);
// 		// 关闭session
// 		session->close();
// 		delete session;
// 	}

// 	// 创建时间序列
// 	void createMultiTimeseries()
// 	{
// 		// 设置设备编号
// 		int deviceId_Num = num++;
// 		// 设备名
// 		deviceId = database + '.' + "d" + to_string(deviceId_Num);
// 		// 时间序列路径
// 		vector<string> paths;
// 		measurement = "BOOLEAN";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "INT32";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "INT64";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "FLOAT";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "DOUBLE";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "TEXT";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		// 数据类型
// 		vector<TSDataType::TSDataType> dataTypes;
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::INT32);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::FLOAT);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		dataTypes.emplace_back(TSDataType::TEXT);
// 		// 编码类型
// 		vector<TSEncoding::TSEncoding> encodings;
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		// 压缩方式
// 		vector<CompressionType::CompressionType> compressors;
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		session->createMultiTimeseries(paths, dataTypes, encodings, compressors, NULL, NULL, NULL, NULL);
// 	}

// 	// 创建对齐时间序列
// 	void createAlignedTimeseries()
// 	{
// 		// 设置设备编号
// 		int deviceId_Num = num++;
// 		// 设备名
// 		deviceId = database + '.' + "d" + to_string(deviceId_Num);
// 		// 物理量名
// 		vector<std::string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		// 数据类型
// 		vector<TSDataType::TSDataType> dataTypes;
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::INT32);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::FLOAT);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		dataTypes.emplace_back(TSDataType::TEXT);
// 		// 编码类型
// 		vector<TSEncoding::TSEncoding> encodings;
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		// 压缩方式
// 		vector<CompressionType::CompressionType> compressors;
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		// 创建对齐时间序列
// 		session->createAlignedTimeseries(deviceId, measurements, dataTypes, encodings, compressors);
// 	}

// 	// 显示数据
// 	void showDate()
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("select * from root.**");
// 		// // 获取每列标题并输出
// 		// for (const string& name : dataSet->getColumnNames()) {
// 		// 	cout << name << "  ";
// 		// }
// 		// cout << endl;
// 		// // 设置数据记录的数量为1024
// 		// dataSet->setFetchSize(1024);
// 		// // 判断是否还有有效值行，有则输出
// 		// while (dataSet->hasNext()) {
// 		// 	cout << dataSet->next()->toString();
// 		// }
// 		// cout << endl;
// 	}

// 	// 测试insertTablet函数
// 	void test_InsertTablet_Normal()
// 	{
// 		// 创建非对齐时间序列
// 		createMultiTimeseries();
// 		// 准备创建tablet的数据类型
// 		pair<string, TSDataType::TSDataType> pair1("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> pair2("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> pair3("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> pair4("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> pair5("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> pair6("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(pair1);
// 		schemas.push_back(pair2);
// 		schemas.push_back(pair3);
// 		schemas.push_back(pair4);
// 		schemas.push_back(pair5);
// 		schemas.push_back(pair6);
// 		// 创建tablet
// 		Tablet tablet(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex , &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		// 插入数据
// 		session->insertTablet(tablet, true);
// 	}

// 	// 测试insertTablets函数
// 	void insertTablets()
// 	{
// 		// 创建tablet1的非对齐时间序列
// 		createMultiTimeseries();
// 		// 创建tablets
// 		unordered_map<string, Tablet*> tablets;
// 		// 准备创建tablet需要的
// 		pair<string, TSDataType::TSDataType> BOOLEAN("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> INT32("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> INT64("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> FLOAT("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> DOUBLE("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> text("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(BOOLEAN);
// 		schemas.push_back(INT32);
// 		schemas.push_back(INT64);
// 		schemas.push_back(FLOAT);
// 		schemas.push_back(DOUBLE);
// 		schemas.push_back(text);
// 		// 创建tablet1
// 		Tablet tablet1(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet1.addValue(5, rowIndex, &textValue);
// 		// 添加tablet1到tablets中
// 		tablets[deviceId] = &tablet1;

// 		// 创建tablet2的非对齐时间序列
// 		createMultiTimeseries();
// 		// 创建tablet2
// 		Tablet tablet2(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet2.addValue(5, rowIndex, &textValue);
// 		// 添加tablet2到tablets中
// 		tablets[deviceId] = &tablet2;

// 		// 插入数据
// 		session->insertTablets(tablets, true);
// 	}

// 	// 测试insertAlignedTablet函数
// 	void test_InsertAlignedTablet_Normal()
// 	{
// 		// 创建对齐时间序列
// 		createAlignedTimeseries();
// 		// 准备创建tablet的数据类型
// 		pair<string, TSDataType::TSDataType> pair1("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> pair2("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> pair3("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> pair4("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> pair5("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> pair6("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(pair1);
// 		schemas.push_back(pair2);
// 		schemas.push_back(pair3);
// 		schemas.push_back(pair4);
// 		schemas.push_back(pair5);
// 		schemas.push_back(pair6);
// 		// 创建tablet
// 		Tablet tablet(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);
// 		// 写入数据
// 		session->insertAlignedTablet(tablet);
// 	}

// 	// 测试insertAlignedTablets函数
// 	void test_InsertAlignedTablets_Normal()
// 	{
// 		// 创建tablet1的对齐数据序列
// 		createAlignedTimeseries();
// 		// 创建tablets
// 		unordered_map<string, Tablet*> tablets;
// 		// 准备创建tablet需要的
// 		pair<string, TSDataType::TSDataType> BOOLEAN("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> INT32("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> INT64("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> FLOAT("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> DOUBLE("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> text("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(BOOLEAN);
// 		schemas.push_back(INT32);
// 		schemas.push_back(INT64);
// 		schemas.push_back(FLOAT);
// 		schemas.push_back(DOUBLE);
// 		schemas.push_back(text);
// 		// 创建tablet1
// 		Tablet tablet1(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet1.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet1.rowSize++;
// 		tablet1.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet1.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet1.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet1.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet1.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet1.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet1.addValue(5, rowIndex, &textValue);
// 		// 添加tablet1到tablets中
// 		tablets[deviceId] = &tablet1;

// 		// 创建tablet2的对齐时间序列
// 		createAlignedTimeseries();
// 		// 创建tablet2
// 		Tablet tablet2(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet2.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet2.rowSize++;
// 		tablet2.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet2.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet2.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet2.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet2.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet2.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet2.addValue(5, rowIndex, &textValue);
// 		// 添加tablet2到tablets中
// 		tablets[deviceId] = &tablet2;

// 		// 写入数据
// 		session->insertAlignedTablets(tablets);
// 	}

// 	// 测试不创建timeSeries，直接插入数据（非对齐）
// 	void test_InsertTabletNoTS_Normal()
// 	{
// 		deviceId = database + '.' + "test_InsertTabletNoTS_Normal";
// 		// 准备创建tablet的数据类型
// 		pair<string, TSDataType::TSDataType> pair1("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> pair2("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> pair3("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> pair4("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> pair5("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> pair6("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(pair1);
// 		schemas.push_back(pair2);
// 		schemas.push_back(pair3);
// 		schemas.push_back(pair4);
// 		schemas.push_back(pair5);
// 		schemas.push_back(pair6);
// 		// 创建tablet
// 		Tablet tablet(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		// 插入数据
// 		session->insertTablet(tablet, true);
// 	}

// 	// 测试不创建timeSeries，直接插入数据（对齐）
// 	void test_InsertAlignedTabletNoATS_Normal()
// 	{
// 		deviceId = database + '.' + "test_InsertAlignedTabletNoATS_Normal";
// 		// 准备创建tablet的数据类型
// 		pair<string, TSDataType::TSDataType> pair1("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> pair2("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> pair3("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> pair4("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> pair5("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> pair6("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(pair1);
// 		schemas.push_back(pair2);
// 		schemas.push_back(pair3);
// 		schemas.push_back(pair4);
// 		schemas.push_back(pair5);
// 		schemas.push_back(pair6);
// 		// 创建tablet
// 		Tablet tablet(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);
// 		// 写入数据
// 		session->insertAlignedTablet(tablet);
// 	}

// 	// 测试插入数据到对齐时间序列中
// 	void test_InsertTabletToATS_Normal()
// 	{
// 		// 创建对齐时间序列
// 		createAlignedTimeseries();
// 		// 准备创建tablet的数据类型
// 		pair<string, TSDataType::TSDataType> pair1("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> pair2("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> pair3("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> pair4("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> pair5("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> pair6("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(pair1);
// 		schemas.push_back(pair2);
// 		schemas.push_back(pair3);
// 		schemas.push_back(pair4);
// 		schemas.push_back(pair5);
// 		schemas.push_back(pair6);
// 		// 创建tablet
// 		Tablet tablet(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		// 插入数据
// 		session->insertTablet(tablet, true);
// 	}

// 	// 测试插入数据到非对齐时间序列中
// 	void test_InsertAlignedTabletToTS_Normal()
// 	{
// 		// 创建非对齐时间序列
// 		createMultiTimeseries();
// 		// 准备创建tablet的数据类型
// 		pair<string, TSDataType::TSDataType> pair1("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> pair2("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> pair3("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> pair4("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> pair5("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> pair6("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(pair1);
// 		schemas.push_back(pair2);
// 		schemas.push_back(pair3);
// 		schemas.push_back(pair4);
// 		schemas.push_back(pair5);
// 		schemas.push_back(pair6);
// 		// 创建tablet
// 		Tablet tablet(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = -822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 822337203685477580;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);
// 		// 写入数据
// 		session->insertAlignedTablet(tablet);
// 	}

// };

// // 测试写入数据-Record
// class InsertRecordTest_Normal : public CppUnit::TestCase {

// 	string database = "root.db"; // 数据库名
// 	string deviceId; // 设备名
// 	string measurement; // 物理量
// 	string path; // 时间序列

// 	bool booleanValue; // 用于存储boolean类型的值
// 	int int32Value; // 用于存储int32类型的值
// 	int64_t int64Value; // 用于存储int32类型的值
// 	float floatValue; // 用于存储float类型的值
// 	double doubleValue; // 用于存储double类型的值
// 	string textValue; // 用于存储text类型的值

// 	int num = 1; // 编号，用于每次调用创建时间序列创建的deviceId_Num不一样

// public:

// 	// 注册组件到TestCaller中
//      static CppUnit::TestSuite *suite()
//     {
//         CppUnit::TestSuite *testSuite = new CppUnit::TestSuite("InsertRecordTest_Normal");
//         using Caller = CppUnit::TestCaller<InsertRecordTest_Normal>;
//         testSuite->addTest(new Caller("test_InsertRecord_Normal", &InsertRecordTest_Normal::test_InsertRecord_Normal));
// 		testSuite->addTest(new Caller("test_InsertRecords_Normal", &InsertRecordTest_Normal::test_InsertRecords_Normal));
// 		testSuite->addTest(new Caller("test_InsertRecordsOfOneDevice_Normal", &InsertRecordTest_Normal::test_InsertRecordsOfOneDevice_Normal));
// 		testSuite->addTest(new Caller("test_InsertAlignedRecord_Normal", &InsertRecordTest_Normal::test_InsertAlignedRecord_Normal));
// 		testSuite->addTest(new Caller("test_InsertAlignedRecords_Normal", &InsertRecordTest_Normal::test_InsertAlignedRecords_Normal));
// 		testSuite->addTest(new Caller("test_InsertAlignedRecordsOfOneDevice_Normal", &InsertRecordTest_Normal::test_InsertAlignedRecordsOfOneDevice_Normal));
// 		testSuite->addTest(new Caller("test_InsertRecord_Inference_Normal", &InsertRecordTest_Normal::test_InsertRecord_Inference_Normal));
// 		testSuite->addTest(new Caller("test_InsertRecords_Inference_Normal", &InsertRecordTest_Normal::test_InsertRecords_Inference_Normal));
// 		//testSuite->addTest(new Caller("test_InsertRecordsOfOneDevice_Inference_Normal", &InsertRecordTest_Normal::test_InsertRecordsOfOneDevice_Inference_Normal)); // 未实现
// 		testSuite->addTest(new Caller("test_InsertAlignedRecord_Inference_Normal", &InsertRecordTest_Normal::test_InsertAlignedRecord_Inference_Normal));
// 		testSuite->addTest(new Caller("test_InsertAlignedRecords_Inference_Normal", &InsertRecordTest_Normal::test_InsertAlignedRecords_Inference_Normal));
// 		//testSuite->addTest(new Caller("test_InsertAlignedRecordsOfOneDevice_Inference_Normal", &InsertRecordTest_Normal::test_InsertAlignedRecordsOfOneDevice_Inference_Normal)); //未实现
//         return testSuite;
//     }

//     // 测试前的准备方法
//     void setUp(void) 
// 	{
// 		// 创建session连接
// 		session = new Session("127.0.0.1", 6667, "root", "root");
// 		session->open(false);
// 		// 创建数据库
// 		session->setStorageGroup(database);
// 	}
//     // 测试后的清理方法
//     void tearDown(void) 
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("show databases");
// 		// 用于存储数据库名
// 		vector<string> storageGroups;
// 		// 设置数据记录的数量为1024
// 		dataSet->setFetchSize(1024);
// 		// 判断是否还有有效值行
// 		while (dataSet->hasNext()) {
// 			// 有则获取本行第一列数据库名数据并存入storageGroups中
// 			storageGroups.emplace_back(dataSet->next()->fields[0].stringV);
// 		}
// 		// 关闭dataSet
// 		dataSet->closeOperationHandle();
// 		// 删除数据库
// 		session->deleteStorageGroups(storageGroups);
// 		// 关闭session
// 		session->close();
// 		delete session;
// 	}

// 	// 创建时间序列
// 	void createMultiTimeseries()
// 	{
// 		// 设置设备编号
// 		int deviceId_Num = num++;
// 		// 设备名
// 		deviceId = database + '.' + "d" + to_string(deviceId_Num);
// 		// 时间序列路径
// 		vector<string> paths;
// 		measurement = "BOOLEAN";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "INT32";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "INT64";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "FLOAT";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "DOUBLE";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "TEXT";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		// 数据类型
// 		vector<TSDataType::TSDataType> dataTypes;
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::INT32);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::FLOAT);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		dataTypes.emplace_back(TSDataType::TEXT);
// 		// 编码类型
// 		vector<TSEncoding::TSEncoding> encodings;
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		// 压缩方式
// 		vector<CompressionType::CompressionType> compressors;
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		session->createMultiTimeseries(paths, dataTypes, encodings, compressors, NULL, NULL, NULL, NULL);
// 	}

// 	// 创建对齐时间序列
// 	void createAlignedTimeseries()
// 	{
// 		// 设置设备编号
// 		int deviceId_Num = num++;
// 		// 设备名
// 		deviceId = database + '.' + "d" + to_string(deviceId_Num);
// 		// 物理量名
// 		vector<std::string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		// 数据类型
// 		vector<TSDataType::TSDataType> dataTypes;
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::INT32);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::FLOAT);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		dataTypes.emplace_back(TSDataType::TEXT);
// 		// 编码类型
// 		vector<TSEncoding::TSEncoding> encodings;
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		// 压缩方式
// 		vector<CompressionType::CompressionType> compressors;
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		// 创建对齐时间序列
// 		session->createAlignedTimeseries(deviceId, measurements, dataTypes, encodings, compressors);
// 	}

// 	// 测试InsertRecord函数
// 	void test_InsertRecord_Normal()
// 	{
// 		// 创建时间序列
// 		createMultiTimeseries();
// 		// 物理量
// 		vector<string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		vector<TSDataType::TSDataType> types;
// 		types.emplace_back(TSDataType::BOOLEAN);
// 		types.emplace_back(TSDataType::INT32);
// 		types.emplace_back(TSDataType::INT64);
// 		types.emplace_back(TSDataType::FLOAT);
// 		types.emplace_back(TSDataType::DOUBLE);
// 		types.emplace_back(TSDataType::TEXT);
// 		// time
// 		int64_t time = 1L;
// 		// 值
// 		vector<char*> values;
// 		booleanValue = false;
// 		values.emplace_back((char*) &booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*) &int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*) &int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*) &floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*) &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*) &"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 2L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 3L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 4L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 5L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 10L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 6L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 8L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 7L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 9L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, types, values);
// 	}

// 	// 测试InsertRecords函数
// 	void test_InsertRecords_Normal()
// 	{
// 		vector<std::string> deviceIds;
// 		vector<int64_t> times;
// 		vector<std::vector<std::string>> measurementsList;
// 		vector<std::vector<TSDataType::TSDataType>> typesList;
// 		vector<std::vector<char*>> valuesList;

// 		// 创建record1的非对齐时间序列
// 		createMultiTimeseries();
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		// 物理量
// 		vector<string> measurements1;
// 		measurements1.emplace_back("BOOLEAN");
// 		measurements1.emplace_back("INT32");
// 		measurements1.emplace_back("INT64");
// 		measurements1.emplace_back("FLOAT");
// 		measurements1.emplace_back("DOUBLE");
// 		measurements1.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		// 数据类型
// 		vector<TSDataType::TSDataType> types1;
// 		types1.emplace_back(TSDataType::BOOLEAN);
// 		types1.emplace_back(TSDataType::INT32);
// 		types1.emplace_back(TSDataType::INT64);
// 		types1.emplace_back(TSDataType::FLOAT);
// 		types1.emplace_back(TSDataType::DOUBLE);
// 		types1.emplace_back(TSDataType::TEXT);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		// time
// 		int64_t time1 = 1L;
// 		times.emplace_back(time1);
// 		time1 = 2L;
// 		times.emplace_back(time1);
// 		time1 = 3L;
// 		times.emplace_back(time1);
// 		time1 = 4L;
// 		times.emplace_back(time1);
// 		time1 = 5L;
// 		times.emplace_back(time1);
// 		time1 = 10L;
// 		times.emplace_back(time1);
// 		time1 = 8L;
// 		times.emplace_back(time1);
// 		time1 = 6L;
// 		times.emplace_back(time1);
// 		time1 = 9L;
// 		times.emplace_back(time1);
// 		time1 = 7L;
// 		times.emplace_back(time1);
// 		// 值
// 		vector<char*> values1;
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();

// 		// 创建record1的非对齐时间序列
// 		createMultiTimeseries();
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		// 物理量
// 		vector<string> measurements2;
// 		measurements2.emplace_back("BOOLEAN");
// 		measurements2.emplace_back("INT32");
// 		measurements2.emplace_back("INT64");
// 		measurements2.emplace_back("FLOAT");
// 		measurements2.emplace_back("DOUBLE");
// 		measurements2.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		// time
// 		int64_t time2 = 1L;
// 		times.emplace_back(time2);
// 		time2 = 2L;
// 		times.emplace_back(time2);
// 		time2 = 3L;
// 		times.emplace_back(time2);
// 		time2 = 4L;
// 		times.emplace_back(time2);
// 		time2 = 5L;
// 		times.emplace_back(time2);
// 		time2 = 10L;
// 		times.emplace_back(time2);
// 		time2 = 8L;
// 		times.emplace_back(time2);
// 		time2 = 6L;
// 		times.emplace_back(time2);
// 		time2 = 9L;
// 		times.emplace_back(time2);
// 		time2 = 7L;
// 		times.emplace_back(time2);
// 		// 数据类型
// 		vector<TSDataType::TSDataType> types2;
// 		types2.emplace_back(TSDataType::BOOLEAN);
// 		types2.emplace_back(TSDataType::INT32);
// 		types2.emplace_back(TSDataType::INT64);
// 		types2.emplace_back(TSDataType::FLOAT);
// 		types2.emplace_back(TSDataType::DOUBLE);
// 		types2.emplace_back(TSDataType::TEXT);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		// 值
// 		vector<char*> values2;
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();

// 		// 写入数据
// 		session->insertRecords(deviceIds, times, measurementsList, typesList, valuesList);

// 	}

// 	// 测试insertRecordsOfOneDevice函数
// 	void test_InsertRecordsOfOneDevice_Normal()
// 	{
// 		// 创建时间序列
// 		createMultiTimeseries();
// 		// time
// 		vector<int64_t> times;
// 		times.push_back(1L);
// 		times.push_back(2L);
// 		times.push_back(3L);
// 		times.push_back(4L);
// 		times.push_back(5L);
// 		times.push_back(10L);
// 		times.push_back(8L);
// 		times.push_back(6L);
// 		times.push_back(9L);
// 		times.push_back(7L);
// 		// 物理量
// 		vector<vector<string>> measurementsList;
// 		vector<string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		// 数据类型
// 		vector<vector<TSDataType::TSDataType>> typesList;
// 		vector<TSDataType::TSDataType> types;
// 		types.emplace_back(TSDataType::BOOLEAN);
// 		types.emplace_back(TSDataType::INT32);
// 		types.emplace_back(TSDataType::INT64);
// 		types.emplace_back(TSDataType::FLOAT);
// 		types.emplace_back(TSDataType::DOUBLE);
// 		types.emplace_back(TSDataType::TEXT);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		// 值
// 		vector<vector<char*>> valuesList;
// 		vector<char*> values;
// 		booleanValue = false;
// 		values.emplace_back((char*) (&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 写入数据
// 		session->insertRecordsOfOneDevice(deviceId, times, measurementsList, typesList, valuesList, true);
// 	}

// 	// 测试InsertAlignedRecord函数
// 	void test_InsertAlignedRecord_Normal()
// 	{
// 		// 创建时间序列
// 		createAlignedTimeseries();
// 		// 物理量
// 		vector<string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		vector<TSDataType::TSDataType> types;
// 		types.emplace_back(TSDataType::BOOLEAN);
// 		types.emplace_back(TSDataType::INT32);
// 		types.emplace_back(TSDataType::INT64);
// 		types.emplace_back(TSDataType::FLOAT);
// 		types.emplace_back(TSDataType::DOUBLE);
// 		types.emplace_back(TSDataType::TEXT);
// 		// time
// 		int64_t time = 1L;
// 		// 值
// 		vector<char*> values;
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 2L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 3L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 4L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 5L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 10L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 6L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 8L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 7L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 		values.clear();
// 		// time
// 		time = 9L;
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, types, values);
// 	}

// 	// 测试insertAlignedRecords函数
// 	void test_InsertAlignedRecords_Normal()
// 	{
// 		vector<std::string> deviceIds;
// 		vector<int64_t> times;
// 		vector<std::vector<std::string>> measurementsList;
// 		vector<std::vector<TSDataType::TSDataType>> typesList;
// 		vector<std::vector<char*>> valuesList;

// 		// 创建record1的对齐时间序列
// 		createAlignedTimeseries();
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		// 物理量
// 		vector<string> measurements1;
// 		measurements1.emplace_back("BOOLEAN");
// 		measurements1.emplace_back("INT32");
// 		measurements1.emplace_back("INT64");
// 		measurements1.emplace_back("FLOAT");
// 		measurements1.emplace_back("DOUBLE");
// 		measurements1.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		// 数据类型
// 		vector<TSDataType::TSDataType> types1;
// 		types1.emplace_back(TSDataType::BOOLEAN);
// 		types1.emplace_back(TSDataType::INT32);
// 		types1.emplace_back(TSDataType::INT64);
// 		types1.emplace_back(TSDataType::FLOAT);
// 		types1.emplace_back(TSDataType::DOUBLE);
// 		types1.emplace_back(TSDataType::TEXT);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		typesList.emplace_back(types1);
// 		// time
// 		int64_t time1 = 1L;
// 		times.emplace_back(time1);
// 		time1 = 2L;
// 		times.emplace_back(time1);
// 		time1 = 3L;
// 		times.emplace_back(time1);
// 		time1 = 4L;
// 		times.emplace_back(time1);
// 		time1 = 5L;
// 		times.emplace_back(time1);
// 		time1 = 10L;
// 		times.emplace_back(time1);
// 		time1 = 8L;
// 		times.emplace_back(time1);
// 		time1 = 6L;
// 		times.emplace_back(time1);
// 		time1 = 9L;
// 		times.emplace_back(time1);
// 		time1 = 7L;
// 		times.emplace_back(time1);
// 		// 值
// 		vector<char*> values1;
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		booleanValue = false;
// 		values1.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values1.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values1.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values1.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values1.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values1.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values1.emplace_back((char*) &textValue);
// 		values1.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();

// 		// 创建record1的对齐时间序列
// 		createAlignedTimeseries();
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		// 物理量
// 		vector<string> measurements2;
// 		measurements2.emplace_back("BOOLEAN");
// 		measurements2.emplace_back("INT32");
// 		measurements2.emplace_back("INT64");
// 		measurements2.emplace_back("FLOAT");
// 		measurements2.emplace_back("DOUBLE");
// 		measurements2.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		// time
// 		int64_t time2 = 1L;
// 		times.emplace_back(time2);
// 		time2 = 2L;
// 		times.emplace_back(time2);
// 		time2 = 3L;
// 		times.emplace_back(time2);
// 		time2 = 4L;
// 		times.emplace_back(time2);
// 		time2 = 5L;
// 		times.emplace_back(time2);
// 		time2 = 10L;
// 		times.emplace_back(time2);
// 		time2 = 8L;
// 		times.emplace_back(time2);
// 		time2 = 6L;
// 		times.emplace_back(time2);
// 		time2 = 9L;
// 		times.emplace_back(time2);
// 		time2 = 7L;
// 		times.emplace_back(time2);
// 		// 数据类型
// 		vector<TSDataType::TSDataType> types2;
// 		types2.emplace_back(TSDataType::BOOLEAN);
// 		types2.emplace_back(TSDataType::INT32);
// 		types2.emplace_back(TSDataType::INT64);
// 		types2.emplace_back(TSDataType::FLOAT);
// 		types2.emplace_back(TSDataType::DOUBLE);
// 		types2.emplace_back(TSDataType::TEXT);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		typesList.emplace_back(types2);
// 		// 值
// 		vector<char*> values2;
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		booleanValue = false;
// 		values2.emplace_back((char*)&booleanValue);
// 		int32Value = 0;
// 		values2.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values2.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values2.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values2.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values2.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();

// 		// 写入数据
// 		session->insertAlignedRecords(deviceIds, times, measurementsList, typesList, valuesList);
// 	}

// 	// 测试insertAlignedRecordsOfOneDevice函数
// 	void test_InsertAlignedRecordsOfOneDevice_Normal()
// 	{
// 		// 创建时间序列
// 		createAlignedTimeseries();
// 		// time
// 		vector<int64_t> times;
// 		times.push_back(1L);
// 		times.push_back(2L);
// 		times.push_back(3L);
// 		times.push_back(4L);
// 		times.push_back(5L);
// 		times.push_back(10L);
// 		times.push_back(8L);
// 		times.push_back(6L);
// 		times.push_back(9L);
// 		times.push_back(7L);
// 		// 物理量
// 		vector<vector<string>> measurementsList;
// 		vector<string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		measurementsList.emplace_back(measurements);
// 		// 数据类型
// 		vector<vector<TSDataType::TSDataType>> typesList;
// 		vector<TSDataType::TSDataType> types;
// 		types.emplace_back(TSDataType::BOOLEAN);
// 		types.emplace_back(TSDataType::INT32);
// 		types.emplace_back(TSDataType::INT64);
// 		types.emplace_back(TSDataType::FLOAT);
// 		types.emplace_back(TSDataType::DOUBLE);
// 		types.emplace_back(TSDataType::TEXT);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		typesList.emplace_back(types);
// 		// 值
// 		vector<vector<char*>> valuesList;
// 		vector<char*> values;
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 值
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		values.clear();
// 		// 写入数据
// 		session->insertRecordsOfOneDevice(deviceId, times, measurementsList, typesList, valuesList, true);
// 	}

// 	// 测试带推断的insertRecord函数
// 	void test_InsertRecord_Inference_Normal()
// 	{
// 		// 设备名
// 		deviceId = database + '.' + "d" + "_Inference";
// 		// 物理量
// 		vector<string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		int64_t time = 1L;
// 		// 值
// 		vector<string> values;
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 2L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 3L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 4L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 5L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 10L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 9L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 8L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 7L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 6L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertRecord(deviceId, time, measurements, values);
// 	}

// 	// 测试带推断的insertRecords函数
// 	void test_InsertRecords_Inference_Normal()
// 	{
// 		vector<std::string> deviceIds;
// 		vector<int64_t> times;
// 		vector<std::vector<std::string>> measurementsList;
// 		vector<std::vector<string>> valuesList;

// 		// 设备名
// 		deviceId = database + '.' + "d" + "_Inference1";
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		// 物理量
// 		vector<string> measurements1;
// 		measurements1.emplace_back("BOOLEAN");
// 		measurements1.emplace_back("INT32");
// 		measurements1.emplace_back("INT64");
// 		measurements1.emplace_back("FLOAT");
// 		measurements1.emplace_back("DOUBLE");
// 		measurements1.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		// time
// 		int64_t time1 = 1L;
// 		times.emplace_back(time1);
// 		time1 = 2L;
// 		times.emplace_back(time1);
// 		time1 = 3L;
// 		times.emplace_back(time1);
// 		time1 = 4L;
// 		times.emplace_back(time1);
// 		time1 = 5L;
// 		times.emplace_back(time1);
// 		time1 = 10L;
// 		times.emplace_back(time1);
// 		time1 = 9L;
// 		times.emplace_back(time1);
// 		time1 = 8L;
// 		times.emplace_back(time1);
// 		time1 = 7L;
// 		times.emplace_back(time1);
// 		time1 = 6L;
// 		times.emplace_back(time1);
// 		// 值
// 		vector<string> values1;
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);

// 		// 设备名
// 		deviceId = database + '.' + "d" + "_Inference2";
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		// 物理量
// 		vector<string> measurements2;
// 		measurements2.emplace_back("BOOLEAN");
// 		measurements2.emplace_back("INT32");
// 		measurements2.emplace_back("INT64");
// 		measurements2.emplace_back("FLOAT");
// 		measurements2.emplace_back("DOUBLE");
// 		measurements2.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		// time
// 		int64_t time2 = 1L;
// 		times.emplace_back(time2);
// 		time2 = 2L;
// 		times.emplace_back(time2);
// 		time2 = 3L;
// 		times.emplace_back(time2);
// 		time2 = 4L;
// 		times.emplace_back(time2);
// 		time2 = 5L;
// 		times.emplace_back(time2);
// 		time2 = 10L;
// 		times.emplace_back(time2);
// 		time2 = 9L;
// 		times.emplace_back(time2);
// 		time2 = 8L;
// 		times.emplace_back(time2);
// 		time2 = 7L;
// 		times.emplace_back(time2);
// 		time2 = 6L;
// 		times.emplace_back(time2);
// 		// 值
// 		vector<string> values2;
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 写入数据
// 		session->insertRecords(deviceIds, times, measurementsList,  valuesList);
// 	}

// 	// 【未实现】测试insertStringRecordsOfOneDevice函数
// 	void test_InsertRecordsOfOneDevice_Inference_Normal()
// 	{
// 		// time
// 		vector<int64_t> times;
// 		times.push_back(1L);
// 		// 物理量
// 		vector<vector<string>> measurementsList;
// 		vector<string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements);
// 		// 数据类型
// 		vector<vector<TSDataType::TSDataType>> typesList;
// 		vector<TSDataType::TSDataType> types;
// 		types.emplace_back(TSDataType::BOOLEAN);
// 		types.emplace_back(TSDataType::INT32);
// 		types.emplace_back(TSDataType::INT64);
// 		types.emplace_back(TSDataType::FLOAT);
// 		types.emplace_back(TSDataType::DOUBLE);
// 		types.emplace_back(TSDataType::TEXT);
// 		typesList.emplace_back(types);
// 		// 值
// 		vector<vector<char*>> valuesList;
// 		vector<char*> values;
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		int64_t time = 0;
// 		// 写入数据
// 		//session.insertRecordsOfOneDevice(deviceId, times, measurementsList, valuesList, true);
// 	}

// 	// 测试带推断的insertAlignedRecord函数
// 	void test_InsertAlignedRecord_Inference_Normal()
// 	{
// 		// 设备名
// 		deviceId = database + '.' + "d" + "_Inference";
// 		// 物理量
// 		vector<string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		int64_t time = 1L;
// 		// 值
// 		vector<string> values;
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 2L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 3L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 4L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 5L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 10L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 9L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 8L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 7L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 		values.clear();
// 		time = 6L;
// 		// 值
// 		values.emplace_back("true");
// 		values.emplace_back("0");
// 		values.emplace_back("0");
// 		values.emplace_back("0.0f");
// 		values.emplace_back("0.0");
// 		values.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		// 写入数据
// 		session->insertAlignedRecord(deviceId, time, measurements, values);
// 	}

// 	// 测试带推断类型insertAlignedRecords_Inference()函数
// 	void test_InsertAlignedRecords_Inference_Normal()
// 	{
// 		vector<std::string> deviceIds;
// 		vector<int64_t> times;
// 		vector<std::vector<std::string>> measurementsList;
// 		vector<std::vector<string>> valuesList;

// 		// 设备名
// 		deviceId = database + '.' + "d" + "_Inference1";
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		// 物理量
// 		vector<string> measurements1;
// 		measurements1.emplace_back("BOOLEAN");
// 		measurements1.emplace_back("INT32");
// 		measurements1.emplace_back("INT64");
// 		measurements1.emplace_back("FLOAT");
// 		measurements1.emplace_back("DOUBLE");
// 		measurements1.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		measurementsList.emplace_back(measurements1);
// 		// time
// 		int64_t time1 = 1L;
// 		times.emplace_back(time1);
// 		time1 = 2L;
// 		times.emplace_back(time1);
// 		time1 = 3L;
// 		times.emplace_back(time1);
// 		time1 = 4L;
// 		times.emplace_back(time1);
// 		time1 = 5L;
// 		times.emplace_back(time1);
// 		time1 = 10L;
// 		times.emplace_back(time1);
// 		time1 = 9L;
// 		times.emplace_back(time1);
// 		time1 = 8L;
// 		times.emplace_back(time1);
// 		time1 = 7L;
// 		times.emplace_back(time1);
// 		time1 = 6L;
// 		times.emplace_back(time1);
// 		// 值
// 		vector<string> values1;
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);
// 		values1.clear();
// 		// 值
// 		values1.emplace_back("true");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0");
// 		values1.emplace_back("0.0f");
// 		values1.emplace_back("0.0");
// 		values1.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values1);

// 		// 设备名
// 		deviceId = database + '.' + "d" + "_Inference2";
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		deviceIds.emplace_back(deviceId);
// 		// 物理量
// 		vector<string> measurements2;
// 		measurements2.emplace_back("BOOLEAN");
// 		measurements2.emplace_back("INT32");
// 		measurements2.emplace_back("INT64");
// 		measurements2.emplace_back("FLOAT");
// 		measurements2.emplace_back("DOUBLE");
// 		measurements2.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		measurementsList.emplace_back(measurements2);
// 		// time
// 		int64_t time2 = 1L;
// 		times.emplace_back(time2);
// 		time2 = 2L;
// 		times.emplace_back(time2);
// 		time2 = 3L;
// 		times.emplace_back(time2);
// 		time2 = 4L;
// 		times.emplace_back(time2);
// 		time2 = 5L;
// 		times.emplace_back(time2);
// 		time2 = 10L;
// 		times.emplace_back(time2);
// 		time2 = 9L;
// 		times.emplace_back(time2);
// 		time2 = 8L;
// 		times.emplace_back(time2);
// 		time2 = 7L;
// 		times.emplace_back(time2);
// 		time2 = 6L;
// 		times.emplace_back(time2);
// 		// 值
// 		vector<string> values2;
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 值
// 		values2.emplace_back("true");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0");
// 		values2.emplace_back("0.0f");
// 		values2.emplace_back("0.0");
// 		values2.emplace_back("qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values2);
// 		values2.clear();
// 		// 写入数据
// 		session->insertAlignedRecords(deviceIds, times, measurementsList, valuesList);
// 	}

// 	// 【未实现】测试带推断类型的insertAlignedRecordsOfOneDevice()函数
// 	void test_InsertAlignedRecordsOfOneDevice_Inference_Normal()
// 	{
// 		// time
// 		vector<int64_t> times;
// 		times.push_back(1L);
// 		// 物理量
// 		vector<vector<string>> measurementsList;
// 		vector<string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		measurementsList.emplace_back(measurements);
// 		// 值
// 		vector<vector<char*>> valuesList;
// 		vector<char*> values;
// 		booleanValue = false;
// 		values.emplace_back((char*)(&booleanValue));
// 		int32Value = 0;
// 		values.emplace_back((char*)&int32Value);
// 		int64Value = 0;
// 		values.emplace_back((char*)&int64Value);
// 		floatValue = 0.0f;
// 		values.emplace_back((char*)&floatValue);
// 		doubleValue = 0.0;
// 		values.emplace_back((char*)&doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		//values.push_back("qwertyuiopasdfghjklzxcvbnm");
// 		//values.emplace_back((char*) &textValue);
// 		values.emplace_back((char*)&"qwertyuiopasdfghjklzxcvbnm");
// 		valuesList.emplace_back(values);
// 		int64_t time = 0;
// 		// 写入数据
// 		//session.insertAlignedRecordsOfOneDevice(deviceId, times, measurementsList, true);
// 	}

// 	// 显示数据
// 	void showDate()
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("select * from root.**");
// 		// // 获取每列标题并输出
// 		// for (const string& name : dataSet->getColumnNames()) {
// 		// 	cout << name << "  ";
// 		// }
// 		// cout << endl;
// 		// // 设置数据记录的数量为1024
// 		// dataSet->setFetchSize(1024);
// 		// // 判断是否还有有效值行，有则输出
// 		// while (dataSet->hasNext()) {
// 		// 	cout << dataSet->next()->toString();
// 		// }
// 		// cout << endl;
// 	}
	
// };

// // 测试数据删除
// class DeleteData_Normal : public CppUnit::TestCase {

// 	string database = "root.db"; // 数据库名
// 	string deviceId = database + '.' + "d"; // 设备名
// 	string AlignedDeviceId = database + '.' + "ad"; // 设备名
// 	string measurement; // 物理量
// 	string path; // 时间序列

// 	size_t rowIndex; // 需要写入数据值所在的列索引
// 	bool booleanValue; // 用于存储boolean类型的值
// 	int int32Value; // 用于存储int32类型的值
// 	int64_t int64Value; // 用于存储int32类型的值
// 	float floatValue; // 用于存储float类型的值
// 	double doubleValue; // 用于存储double类型的值
// 	string textValue; // 用于存储text类型的值

// public:

// 	// 注册组件到TestCaller中
//      static CppUnit::TestSuite *suite()
//     {
//         CppUnit::TestSuite *testSuite = new CppUnit::TestSuite("DeleteData_Normal");
//         using Caller = CppUnit::TestCaller<DeleteData_Normal>;
//         testSuite->addTest(new Caller("test_DeleteData_Normal", &DeleteData_Normal::test_DeleteData_Normal));
// 		testSuite->addTest(new Caller("test_DeleteDatas_Normal", &DeleteData_Normal::test_DeleteDatas_Normal));
// 		return testSuite;
//     }

//     // 测试前的准备方法
//     void setUp(void) 
// 	{
// 		// 创建session连接
// 		session = new Session("127.0.0.1", 6667, "root", "root");
// 		session->open(false);
// 		// 创建数据库
// 		session->setStorageGroup(database);
// 		// 创建时间序列
// 		createAlignedTimeseries();
// 		createMultiTimeseries();
// 		// 写入数据
// 		insertTablet();
// 		insertAlignedTablet();
// 		// 查看数据
// 		showDate();
// 	}
//     // 测试后的清理方法
//     void tearDown(void) 
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("show databases");
// 		// 用于存储数据库名
// 		vector<string> storageGroups;
// 		// 设置数据记录的数量为1024
// 		dataSet->setFetchSize(1024);
// 		// 判断是否还有有效值行
// 		while (dataSet->hasNext()) {
// 			// 有则获取本行第一列数据库名数据并存入storageGroups中
// 			storageGroups.emplace_back(dataSet->next()->fields[0].stringV);
// 		}
// 		// 关闭dataSet
// 		dataSet->closeOperationHandle();
// 		// 删除数据库
// 		session->deleteStorageGroups(storageGroups);
// 		// 关闭session
// 		session->close();
// 		delete session;
// 	}

// 	// 创建时间序列
// 	void createMultiTimeseries()
// 	{
// 		// 时间序列路径
// 		vector<string> paths;
// 		measurement = "BOOLEAN";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "INT32";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "INT64";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "FLOAT";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "DOUBLE";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		measurement = "TEXT";
// 		path = deviceId + '.' + measurement;
// 		paths.emplace_back(path);
// 		// 数据类型
// 		vector<TSDataType::TSDataType> dataTypes;
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::INT32);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::FLOAT);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		dataTypes.emplace_back(TSDataType::TEXT);
// 		// 编码类型
// 		vector<TSEncoding::TSEncoding> encodings;
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		// 压缩方式
// 		vector<CompressionType::CompressionType> compressors;
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		session->createMultiTimeseries(paths, dataTypes, encodings, compressors, NULL, NULL, NULL, NULL);
// 	}

// 	// 创建对齐时间序列
// 	void createAlignedTimeseries()
// 	{
// 		// 物理量名
// 		vector<std::string> measurements;
// 		measurements.emplace_back("BOOLEAN");
// 		measurements.emplace_back("INT32");
// 		measurements.emplace_back("INT64");
// 		measurements.emplace_back("FLOAT");
// 		measurements.emplace_back("DOUBLE");
// 		measurements.emplace_back("TEXT");
// 		// 数据类型
// 		vector<TSDataType::TSDataType> dataTypes;
// 		dataTypes.emplace_back(TSDataType::BOOLEAN);
// 		dataTypes.emplace_back(TSDataType::INT32);
// 		dataTypes.emplace_back(TSDataType::INT64);
// 		dataTypes.emplace_back(TSDataType::FLOAT);
// 		dataTypes.emplace_back(TSDataType::DOUBLE);
// 		dataTypes.emplace_back(TSDataType::TEXT);
// 		// 编码类型
// 		vector<TSEncoding::TSEncoding> encodings;
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		encodings.emplace_back(TSEncoding::PLAIN);
// 		// 压缩方式
// 		vector<CompressionType::CompressionType> compressors;
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		compressors.emplace_back(CompressionType::GZIP);
// 		// 创建对齐时间序列
// 		session->createAlignedTimeseries(AlignedDeviceId, measurements, dataTypes, encodings, compressors);
// 	}
	
// 	// 写入非对齐数据
// 	void insertTablet()
// 	{
// 		// 准备创建tablet的数据类型
// 		pair<string, TSDataType::TSDataType> pair1("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> pair2("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> pair3("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> pair4("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> pair5("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> pair6("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(pair1);
// 		schemas.push_back(pair2);
// 		schemas.push_back(pair3);
// 		schemas.push_back(pair4);
// 		schemas.push_back(pair5);
// 		schemas.push_back(pair6);
// 		// 创建tablet
// 		Tablet tablet(deviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = -8223372036854775808;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 8223372036854775807;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		// 插入数据
// 		session->insertTablet(tablet, true);
// 	}
	
// 	// 写入对齐数据
// 	void insertAlignedTablet()
// 	{
// 		// 准备创建tablet的数据类型
// 		pair<string, TSDataType::TSDataType> pair1("BOOLEAN", TSDataType::BOOLEAN);
// 		pair<string, TSDataType::TSDataType> pair2("INT32", TSDataType::INT32);
// 		pair<string, TSDataType::TSDataType> pair3("INT64", TSDataType::INT64);
// 		pair<string, TSDataType::TSDataType> pair4("FLOAT", TSDataType::FLOAT);
// 		pair<string, TSDataType::TSDataType> pair5("DOUBLE", TSDataType::DOUBLE);
// 		pair<string, TSDataType::TSDataType> pair6("TEXT", TSDataType::TEXT);
// 		vector<pair<string, TSDataType::TSDataType>> schemas;
// 		schemas.push_back(pair1);
// 		schemas.push_back(pair2);
// 		schemas.push_back(pair3);
// 		schemas.push_back(pair4);
// 		schemas.push_back(pair5);
// 		schemas.push_back(pair6);
// 		// 创建tablet
// 		Tablet tablet(AlignedDeviceId, schemas, 10);
// 		// 补充tablet时间戳、值
// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 1L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "qwertyuiopasdfghjklzxcvbnm";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 2L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 2147483647;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = -8223372036854775808;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -0.12345678f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 3L;
// 		booleanValue = true;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = -2147483648;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 8223372036854775807;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.123456789;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.12345678901234567;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "()";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 4L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -4.1234567;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -4.1234567890123456;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 5L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 4.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 4.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "          ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 10L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = -11.123456;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = -11.123456789012345;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "没没";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 8L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 11.12345;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 11.12345678901234;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "123abc没问题！￥（）";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 6L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = " 123abc没问题！￥（） ";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 9L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);

// 		rowIndex = tablet.rowSize++;
// 		tablet.timestamps[rowIndex] = 7L;
// 		booleanValue = false;
// 		tablet.addValue(0, rowIndex, &booleanValue);
// 		int32Value = 0;
// 		tablet.addValue(1, rowIndex, &int32Value);
// 		int64Value = 0;
// 		tablet.addValue(2, rowIndex, &int64Value);
// 		floatValue = 0.0f;
// 		tablet.addValue(3, rowIndex, &floatValue);
// 		doubleValue = 0.0;
// 		tablet.addValue(4, rowIndex, &doubleValue);
// 		textValue = "1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg1234567890123456789012345678901234567890abcdefg";
// 		tablet.addValue(5, rowIndex, &textValue);
// 		// 写入数据
// 		session->insertAlignedTablet(tablet);
// 	}

// 	// 测试删除删除一个数据
// 	void test_DeleteData_Normal()
// 	{
// 		path = deviceId + '.' + "BOOLEAN";
// 		session->deleteData(path, 1);
// 		path = deviceId + '.' + "INT32";
// 		session->deleteData(path, 2);
// 		path = deviceId + '.' + "INT64";
// 		session->deleteData(path, 3);
// 		path = deviceId + '.' + "FLOAT";
// 		session->deleteData(path, 4);
// 		path = deviceId + '.' + "DOUBLE";
// 		session->deleteData(path, 5);
// 		path = deviceId + '.' + "TEXT";
// 		session->deleteData(path, 6);

// 		path = AlignedDeviceId + '.' + "BOOLEAN";
// 		session->deleteData(path, 1);
// 		path = AlignedDeviceId + '.' + "INT32";
// 		session->deleteData(path, 2);
// 		path = AlignedDeviceId + '.' + "INT64";
// 		session->deleteData(path, 3);
// 		path = AlignedDeviceId + '.' + "FLOAT";
// 		session->deleteData(path, 4);
// 		path = AlignedDeviceId + '.' + "DOUBLE";
// 		session->deleteData(path, 5);
// 		path = AlignedDeviceId + '.' + "TEXT";
// 		session->deleteData(path, 6);

// 		showDate();
// 	}

// 	// 测试删除删除多个数据
// 	void test_DeleteDatas_Normal()
// 	{
// 		vector<string> paths1;
// 		paths1.emplace_back(deviceId + '.' + "BOOLEAN");
// 		paths1.emplace_back(deviceId + '.' + "INT32");
// 		paths1.emplace_back(deviceId + '.' + "INT64");
// 		paths1.emplace_back(deviceId + '.' + "FLOAT");
// 		paths1.emplace_back(deviceId + '.' + "DOUBLE");
// 		paths1.emplace_back(deviceId + '.' + "TEXT");
// 		session->deleteData(paths1, 7);
// 		session->deleteData(paths1, 8);
// 		session->deleteData(paths1, 9);
// 		session->deleteData(paths1, 10);

// 		vector<string> paths2;
// 		paths2.emplace_back(AlignedDeviceId + '.' + "BOOLEAN");
// 		paths2.emplace_back(AlignedDeviceId + '.' + "INT32");
// 		paths2.emplace_back(AlignedDeviceId + '.' + "INT64");
// 		paths2.emplace_back(AlignedDeviceId + '.' + "FLOAT");
// 		paths2.emplace_back(AlignedDeviceId + '.' + "DOUBLE");
// 		paths2.emplace_back(AlignedDeviceId + '.' + "TEXT");
// 		session->deleteData(paths2, 7);
// 		session->deleteData(paths2, 8);
// 		session->deleteData(paths2, 9);
// 		session->deleteData(paths2, 10);

// 		showDate();
// 	}

// 	// 显示数据
// 	void showDate()
// 	{
// 		// 获取查询到的数据并生成一个SessionDataSet 类型的智能指针对象
// 		unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement("select * from root.**");
// 		// // 获取每列标题并输出
// 		// for (const string& name : dataSet->getColumnNames()) {
// 		// 	cout << name << "  ";
// 		// }
// 		// cout << endl;
// 		// // 设置数据记录的数量为1024
// 		// dataSet->setFetchSize(1024);
// 		// // 判断是否还有有效值行，有则输出
// 		// while (dataSet->hasNext()) {
// 		// 	cout << dataSet->next()->toString();
// 		// }
// 		// cout << endl;
// 	}
// };

// // 主函数，程序的入口点
// int main()
// {
//     // 测试结果控制器
//     CppUnit::TestResult result;
    
//     // 测试结果收集器
//     CppUnit::TestResultCollector resultCollector;
//     // 收集测试结果
//     result.addListener( &resultCollector ); 
    
//     // 测试进度监听器
//     CppUnit::BriefTestProgressListener progress;
//     // 收集测试进度
//     result.addListener( &progress ); 
    
//     // 测试运行器
//     CppUnit::TestRunner runner;
//     // 将测试用例添加到测试运行器中
//     runner.addTest( SessionConnectTest_Normal::suite());
//     runner.addTest( DataBaseTest_Normal::suite());
// 	runner.addTest( TimeseriesTest_Normal::suite());
// 	runner.addTest( SchemaTemplateTest_Normal::suite());
// 	runner.addTest( InsertTabletTest_Normal::suite());
// 	runner.addTest( InsertRecordTest_Normal::suite());
// 	runner.addTest( DeleteData_Normal::suite());
//     // 运行测试，并传递控制器
//     runner.run( result );

//     // 测试结果
//     CppUnit::TextOutputter outputter(&resultCollector, std::cout);
//     // 打印测试结果
//     outputter.write(); 
    
//     // 返回测试结果
//     return resultCollector.wasSuccessful() ? 0 : 1;
// }
