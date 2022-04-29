cmake_minimum_required(VERSION 3.14)
project(TinyMuduo)

file(GLOB PRO_SRC *.cpp Log/*.cpp Log/*.h  Base/*.cpp Base/*.h Net/*.h Net/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(TinyMuduo ${PRO_SRC})
if(UNIX)
    target_link_libraries(TinyMuduo pthread)
endif()