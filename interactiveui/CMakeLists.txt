cmake_minimum_required(VERSION 3.11)

project(InteractiveUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(InteractiveUI
  main.cpp
  geometrycanvasitem.cpp
  plineoffsetalgorithmview.cpp
  plinecombinealgorithmview.cpp
  plineoffsetislandsalgorithmview.cpp
  demofuncs.cpp
  graphicshelpers.cpp
  polylinenode.cpp
  spatialindexboundingboxesnode.cpp
  flatcolorgeometrynode.cpp
  rawoffsetsegmentsnode.cpp
  plinesegmentnode.cpp
  qml.qrc)

target_link_libraries(InteractiveUI
  PRIVATE CavalierContours)

target_compile_definitions(InteractiveUI
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(InteractiveUI
  PRIVATE Qt5::Core Qt5::Quick)
