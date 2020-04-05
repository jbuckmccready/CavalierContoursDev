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
	if (MSVC)
        target_link_options(${name} PRIVATE $<$<CONFIG:RELWITHDEBINFO>:/PROFILE>)
    endif()
endmacro()

add_benchmark(offsetbenchmarks)
add_benchmark(spatialindexbenchmarks)
add_benchmark(extentsbenchmarks)
add_benchmark(areabenchmarks)
add_benchmark(pathlengthbenchmarks)
add_benchmark(windingnumberbenchmarks)
add_benchmark(combinebenchmarks)

add_benchmark(clipperbenchmarks)
target_link_libraries(clipperbenchmarks
  PRIVATE clipper_static)

add_test(Benchmarks COMMAND Benchmarks)
