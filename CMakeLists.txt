cmake_minimum_required(VERSION 3.14)
project(tinyMuduo)

file(GLOB PRO_SRC *.cpp Log/*.cpp Log/*.h  Base/*.cpp Base/*.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(tinyMuduo ${PRO_SRC})
if(UNIX)
    target_link_libraries(tinyMuduo pthread)
endif()