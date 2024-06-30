cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(ISTL VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(include)

# 查找 src 文件夹中的所有源文件
file(GLOB SOURCES "src/*.cpp")

# 添加可执行文件
add_executable(test test.cpp ${SOURCES})

# 设置编译选项
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

