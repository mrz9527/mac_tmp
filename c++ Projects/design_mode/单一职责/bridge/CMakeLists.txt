cmake_minimum_required(VERSION 3.5.1)
project(design_mode)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SRC_DIR_LIST "Application/*.cpp" "Application/*.c")

add_executable(bridge_app ${SRC_DIR_LIST} main.cpp)
target_link_libraries(bridge_app common)