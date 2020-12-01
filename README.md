# CavalierContoursDev
This is a Qt project for development work on the [CavalierContours project](https://github.com/jbuckmccready/CavalierContours). It has an interactive UI app for exploring and testing the algorithm as well as any benchmarks/tests. The project uses a submodule for tracking the main CavalierContours project.

# Requirements to Build and Run
- CMake (tested with CMake 3.15.3)
- Qt 5.13 or later (tested with Qt 13.1 and Qt 14.0)
- C++14 compatible compiler (tested with GCC 7.3.0 64 bit and MSVC 2019)

# Basic Getting Started on Windows
1. Install the latest CMake (can be found [here](https://cmake.org/download/))
2. Install Qt and the MinGW 7.3.0 tools via the Qt Online Installer (can be found [here](https://www.qt.io/download), select the "Downloads for open source users")
3. Open Qt Creator, go to Tools->Options menu
4. Click on "Kits" on the left, then click on the "CMake" tab
5. If CMake was not auto detected (shown under "Auto-detected") then click "Add" on the right and set the "Path" parameter to `C:\Program Files (x86)\CMake\bin\cmake.exe` or wherever you installed CMake and save the settings
6. Clone this repository including submodules `git clone --recursive https://github.com/jbuckmccready/CavalierContoursDev.git`
7. Ensure git is in your `PATH` environment variable (required when running CMake to clone and download clipper and google test for benchmarks and tests)
8. Open the root CMakeLists.txt in Qt Creator and select the MinGW 7.3.0 kit
9. Click the Kits button in the lower left of Qt Creator (has a computer icon) and select MinGW->Debug->InteractiveUI
10. Click the Run button in the lower left, the interactive ui application should run

# Changing the Test Polyline
The vertex positions can be edited by dragging them around but currently any other editing is done by changing the initial polyline that is hardcoded in the constructor of PlineOffsetAlgorithmView (see [plineoffsetalgorithmview.cpp](interactiveui/plineoffsetalgorithmview.cpp)).
