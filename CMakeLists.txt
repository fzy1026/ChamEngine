cmake_minimum_required(VERSION 4.2.1)
project(ChamEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# 包含目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 创建你的库
add_library(ChamMath src/chammath.cpp)
add_library(ChamTool src/chamtool.cpp)
add_library(ChamGame src/chamgame.cpp)

# 主程序
add_executable(Main Main.cpp)

# 链接你的库（调整顺序：被依赖的放后面）
target_link_libraries(Main ChamGame ChamMath ChamTool)

# 链接 EasyX（最重要的一步）
target_link_libraries(Main easyx)