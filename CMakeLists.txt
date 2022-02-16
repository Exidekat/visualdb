# CMakeList.txt : CMake project for visualdb, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("visualdb")

add_subdirectory("bin/glfw")

# Add source to this project's executable.
add_executable (visualdb "src/visualdb.cpp" "src/glad.c")
target_include_directories(visualdb PUBLIC include/)
target_link_libraries(visualdb PUBLIC glfw::glfw)


# TODO: Add tests and install targets if needed.
