project(muduo)

file(GLOB PRO_SRC *.cpp Log/*.cpp Log/*.h  Base/*.cpp Base/*.h Net/*.h Net/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(${PROJECT_NAME} SHARED
        ${PRO_SRC})
if(UNIX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()