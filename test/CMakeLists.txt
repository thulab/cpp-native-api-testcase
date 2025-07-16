# 添加测试文件和源文件文件
add_executable(main 
${CMAKE_SOURCE_DIR}/test/main.cpp 
# 表模型测试用例
# ${CMAKE_SOURCE_DIR}/test/table/insert_test.cpp  # 需要参考select_test更新用例
# ${CMAKE_SOURCE_DIR}/test/table/table_session_build_test.cpp  需要参考select_test更新用例
${CMAKE_SOURCE_DIR}/test/table/select_test.cpp 
# 树模型测试用例
${CMAKE_SOURCE_DIR}/test/tree/select_test.cpp 
# 测试覆盖率的源文件
# ${CMAKE_SOURCE_DIR}/client/include/TableSession.cpp ${CMAKE_SOURCE_DIR}/client/include/SessionDataSet.cpp ${CMAKE_SOURCE_DIR}/client/include/client_types.cpp 
# ${CMAKE_SOURCE_DIR}/client/include/Column.cpp  ${CMAKE_SOURCE_DIR}/client/include/ColumnDecoder.cpp  ${CMAKE_SOURCE_DIR}/client/include/common_types.cpp  
# ${CMAKE_SOURCE_DIR}/client/include/Common.cpp  ${CMAKE_SOURCE_DIR}/client/include/IClientRPCService.cpp  ${CMAKE_SOURCE_DIR}/client/include/IoTDBRpcDataSet.cpp  
# ${CMAKE_SOURCE_DIR}/client/include/NodesSupplier.cpp  ${CMAKE_SOURCE_DIR}/client/include/SessionConnection.cpp  ${CMAKE_SOURCE_DIR}/client/include/ThriftConnection.cpp  
# ${CMAKE_SOURCE_DIR}/client/include/TsBlock.cpp
)

#设置链接库
TARGET_LINK_LIBRARIES(main iotdb_session "${CMAKE_SOURCE_DIR}/client/lib/libthrift.a" "${CMAKE_SOURCE_DIR}/client/lib/libgtest.a" dl pthread)
