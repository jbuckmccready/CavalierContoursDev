cmake_minimum_required(VERSION 3.5)

project(InteractiveUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

FILE(GLOB HeaderFiles "./*.h")
add_custom_target(headers SOURCES ${HeaderFiles})

include_directories(CavalierContours/include)
add_executable(InteractiveUI
  main.cpp
  plineoffsetalgorithmview.cpp
  graphicshelpers.cpp
  polylinenode.cpp
  spatialindexboundingboxesnode.cpp
  flatcolorgeometrynode.cpp
  rawoffsetsegmentsnode.cpp
  plinesegmentnode.cpp
  qml.qrc)

target_compile_definitions(InteractiveUI
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(InteractiveUI
  PRIVATE Qt5::Core Qt5::Quick)
