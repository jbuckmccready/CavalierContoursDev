cmake_minimum_required(VERSION 3.5)

project(Benchmarks LANGUAGES CXX)


find_package(Qt5Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

add_executable(Benchmarks benchmarks.cpp)
add_test(Benchmarks COMMAND Benchmarks)

target_link_libraries(Benchmarks PRIVATE Qt5::Test)

