project(glfw)

add_library(glfw INTERFACE)
target_include_directories(glfw INTERFACE include/)
target_link_libraries(glfw INTERFACE glfw3.lib)
target_link_directories(glfw INTERFACE libs/)

add_library(glfw::glfw ALIAS glfw)