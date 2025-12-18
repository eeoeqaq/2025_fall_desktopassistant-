cmake_minimum_required(VERSION 3.10)
project(ReminderApp)

set(CMAKE_CXX_STANDARD 17)

# 自动收集所有 cpp 文件
file(GLOB SRC "*.cpp")

add_executable(reminder ${SRC})
