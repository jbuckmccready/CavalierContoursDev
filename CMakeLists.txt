cmake_minimum_required(VERSION 3.5)
project(CavalierContoursDev)

# add CavalierContours header only library
add_library(CavalierContours INTERFACE)
target_include_directories(CavalierContours
  INTERFACE CavalierContours/include)

# add subdir projects
add_subdirectory(interactiveui)
add_subdirectory(benchmarks)
