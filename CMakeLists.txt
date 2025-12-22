cmake_minimum_required(VERSION 3.10)
project(DesktopAssistant)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# 自动收集所有 cpp 文件
file(GLOB_RECURSE SOURCES
     "src/*.cpp"
     "src/**/*.cpp"
)

# data 目录在项目根目录下
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/data/pomodoro")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/data/reminder")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/data/account")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/data/todolist")
set(DATA_DIR "${CMAKE_SOURCE_DIR}/data")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES})

# 传给 C++ 作为宏：DATA_DIR="D:/.../data"
target_compile_definitions(${PROJECT_NAME} PRIVATE DATA_DIR=\"${DATA_DIR}\")
