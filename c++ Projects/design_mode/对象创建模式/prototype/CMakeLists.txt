cmake_minimum_required(VERSION 3.5.1)
project(design_mode)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SRC_DIR_LIST "Application/*.cpp" "Application/*.c")

include_directories(Framework Application)
link_directories(Application ../../Common)
add_executable(prototype_app main.cpp SplitProcess.cpp ${SRC_DIR_LIST})
target_link_libraries(prototype_app common)

set(PROJECT_BINARY_DIR)
set(PROJECT_SOURCE_DIR )