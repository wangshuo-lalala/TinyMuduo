cmake_minimum_required(VERSION 3.14)
project(TinyMuduo)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/muduo)
add_subdirectory(muduo)
add_subdirectory(examples)
