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
add_executable (visualdb "src/visualdb.cpp" "src/glad.c" "src/render.cpp" "include/render.h" "include/shape.h" "src/shape.cpp")
target_include_directories(visualdb PUBLIC include/)
target_link_libraries(visualdb PUBLIC glfw::glfw)
# find_package(freetype REQUIRED)
target_link_libraries(visualdb PUBLIC freetype::freetype)

# TODO: Add tests and install targets if needed.

set_target_properties(visualdb PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")