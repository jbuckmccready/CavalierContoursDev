cmake_minimum_required(VERSION 3.16)
project(CavalierContoursDev)

SET(CAVC_HEADER_ONLY ON CACHE BOOL "using headers only")
# add subdir projects
add_subdirectory(CavalierContours)
add_subdirectory(interactiveui)
