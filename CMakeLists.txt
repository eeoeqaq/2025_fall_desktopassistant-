cmake_minimum_required(VERSION 3.10)
project(DesktopAssistant)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# 自动收集所有 cpp 文件
file(GLOB_RECURSE SOURCES
     "src/*.cpp"
     "src/**/*.cpp"
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES})
