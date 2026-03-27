cmake_minimum_required(VERSION 3.10)
project(ChamEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ========== EasyX 配置 ==========
# EasyX 文件夹在项目根目录
set(EASY_X_DIR ${CMAKE_SOURCE_DIR}/easyx)

# 添加 EasyX 头文件路径
include_directories(${EASY_X_DIR}/include)

# 添加 EasyX 库文件路径（32位版本）
link_directories(${EASY_X_DIR}/lib64)

# ========== 项目配置 ==========
# 包含项目自己的头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# 创建你的库
add_library(ChamMath src/chammath.cpp)
add_library(ChamGame src/chamgame.cpp)

# 主程序
add_executable(Main Main.cpp)

# 链接你的库（顺序：被依赖的放后面）
target_link_libraries(Main ChamGame ChamMath)

target_link_libraries(Main libeasyx.a)

# EasyX 依赖的 Windows 系统库
target_link_libraries(Main gdi32 ole32)