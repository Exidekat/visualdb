project(freetype)

add_library(freetype INTERFACE)
target_include_directories(freetype INTERFACE include/)
target_link_libraries(freetype INTERFACE freetype.lib)
target_link_directories(freetype INTERFACE libs/)

add_library(freetype::freetype ALIAS freetype)