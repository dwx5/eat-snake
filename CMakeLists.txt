cmake_minimum_required(VERSION 3.10)

# 项目名称
project(eat-snake)

# C++标准配置
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 头文件目录（所有.h文件都放在include下）
include_directories(include)

# 源文件收集
# 匹配src目录下的所有.cpp文件和根目录的main.cpp
file(GLOB SOURCES 
    "src/*.cpp"    # 包含src下的Pos.cpp、Game.cpp、Food.cpp、Snake.cpp等
    "main.cpp"     # 主程序入口
)

# 生成可执行文件
add_executable(eat-snake ${SOURCES})
