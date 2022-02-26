# CMakeList.txt : CMake project for visualdb, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("visualdb")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory("bin/glfw")
add_subdirectory("bin/freetype")

# Add source to this project's executable.
add_executable (visualdb "src/visualdb.cpp" "src/glad.c" "src/render.cpp" "include/render.h" "include/shape.h" "src/shape.cpp" "src/update.cpp")
target_include_directories(visualdb PUBLIC include/)
#target_include_directories(visualdb PUBLIC "../AppData/Local/Programs/Python/Python38/include/")
#target_include_directories(visualdb PUBLIC "C:/Python27/include/")
target_link_libraries(visualdb PUBLIC glfw::glfw)
target_link_libraries(visualdb PUBLIC freetype::freetype)
#target_link_directories(visualdb PUBLIC "../AppData/Local/Programs/Python/Python38/libs/")
#find_library(sounddevice PATHS ../AppData/Local/Programs/Python/Python38/libs/ NO_DEFAULT_PATH)
#target_link_directories(visualdb PUBLIC "C:/Python27/libs/")
#find_library(python PATHS C:/Python27/ NO_DEFAULT_PATH)

# TODO: Add tests and install targets if needed.

set_target_properties(visualdb PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")