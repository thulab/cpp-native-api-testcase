# 设置项目cmake版本最低为3.7
cmake_minimum_required (VERSION 3.7)

# 设置项目名
project ("cpp-session-test")

# 设置 C++ 标准库版本为17
SET(CMAKE_CXX_STANDARD 17)
# 设置使用指定的 C++ 标准
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置启用位置无关代码
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
# coverage option
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")


# 添加头文件路径
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/client/include)
# 查找boost
FIND_PACKAGE(Boost REQUIRED)
IF (DEFINED BOOST_INCLUDEDIR)
    include_directories("${Boost_INCLUDE_DIR}")
ENDIF()

# 指定库文件的目录
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/client/lib)

# 包含子项目。
add_subdirectory ("test")
# add_subdirectory ("example")