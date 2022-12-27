cmake_minimum_required(VERSION 3.5.1)
project(design_mode)

set(CMAKE_CXX_STANDARD 11)

include_directories(./Common)

add_subdirectory(Common)
add_subdirectory(组件协作)
add_subdirectory(对象创建模式)
add_subdirectory(单一职责)

add_executable(demo 回文数.cpp)