# 添加测试文件和源文件文件
add_executable(main 
${CMAKE_SOURCE_DIR}/test/main.cpp 
# 表模型测试用例
# ${CMAKE_SOURCE_DIR}/test/table/table_insert_test.cpp  # 需要参考select_test更新用例
${CMAKE_SOURCE_DIR}/test/table/table_session_test.cpp
${CMAKE_SOURCE_DIR}/test/table/table_select_test.cpp 
# 树模型测试用例
${CMAKE_SOURCE_DIR}/test/tree/tree_select_test.cpp 
# 模型推理 getDouble 用例(V2-917)依赖 AINode/timer_xl 大模型,默认测试环境无 AINode 可用,故默认屏蔽;
# 需手动验证时取消下行注释(并确认被测库已注册 AINode、按 model_inference_getdouble_test.cpp 顶部说明配置连接参数)
# ${CMAKE_SOURCE_DIR}/test/tree/model_inference_getdouble_test.cpp 
# 测试覆盖率的源文件
# ${CMAKE_SOURCE_DIR}/client/include/TableSession.cpp ${CMAKE_SOURCE_DIR}/client/include/SessionDataSet.cpp ${CMAKE_SOURCE_DIR}/client/include/client_types.cpp 
# ${CMAKE_SOURCE_DIR}/client/include/Column.cpp  ${CMAKE_SOURCE_DIR}/client/include/ColumnDecoder.cpp  ${CMAKE_SOURCE_DIR}/client/include/common_types.cpp  
# ${CMAKE_SOURCE_DIR}/client/include/Common.cpp  ${CMAKE_SOURCE_DIR}/client/include/IClientRPCService.cpp  ${CMAKE_SOURCE_DIR}/client/include/IoTDBRpcDataSet.cpp  
# ${CMAKE_SOURCE_DIR}/client/include/NodesSupplier.cpp  ${CMAKE_SOURCE_DIR}/client/include/SessionConnection.cpp  ${CMAKE_SOURCE_DIR}/client/include/ThriftConnection.cpp  
# ${CMAKE_SOURCE_DIR}/client/include/TsBlock.cpp
)

#设置链接库
TARGET_LINK_LIBRARIES(main iotdb_session "${CMAKE_SOURCE_DIR}/client/lib/libgtest.a" dl pthread)
