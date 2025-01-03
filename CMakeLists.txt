cmake_minimum_required(VERSION 3.1)
project(BackTest)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# 获取当前项目的路径
# get_filename_component(PROJECT_ROOT ${CMAKE_SOURCE_DIR} ABSOLUTE)/

# 添加全局编译选项，替换文件路径为相对路径
find_package(Boost 1.87.0 REQUIRED COMPONENTS system)# 查找 Boost 库
find_package(OpenSSL REQUIRED)# 找到 OpenSSL

include_directories(
    "${PROJECT_SOURCE_DIR}/include/"
    "/usr/local/include/"
)

link_directories(
    "${PROJECT_SOURCE_DIR}/lib/"
    "/usr/local/lib/"
)

# 添加库
add_library(foundation src/foundation.cpp)
add_library(binary src/binary.cpp)
add_library(config src/config.cpp)
# 添加可执行文件
add_executable(backtest src/main.cpp)

# 链接库
target_link_libraries(backtest foundation binary  Boost::system log4cxx -lz)
