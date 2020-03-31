cmake_minimum_required(VERSION 3.11)

include(googlebenchmark.cmake)
include(clipper.cmake)

project(Benchmarks LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

macro(add_benchmark name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name}
    PRIVATE CavalierContours)
  target_link_libraries(${name}
    PRIVATE benchmark::benchmark)
endmacro()

add_benchmark(offsetbenchmarks)
add_benchmark(clipperbenchmarks)
target_link_libraries(clipperbenchmarks
  PRIVATE clipper_static)
add_benchmark(spatialindexbenchmarks)

#add_executable(Benchmarks benchmarks.cpp)
#add_executable(ClipperBenchmarks clipperbenchmarks.cpp)

add_test(Benchmarks COMMAND Benchmarks)

#target_link_libraries(Benchmarks
#  PRIVATE CavalierContours)

#target_link_libraries(Benchmarks
#  PRIVATE benchmark::benchmark)

#target_link_libraries(ClipperBenchmarks
#  PRIVATE CavalierContours)

#target_link_libraries(ClipperBenchmarks
#  PRIVATE clipper_static)

#target_link_libraries(ClipperBenchmarks
#  PRIVATE benchmark::benchmark)
