cmake_minimum_required(VERSION 3.5.1)
project(design_mode)

set(CMAKE_CXX_STANDARD 11)

link_directories(../../Common)

add_library(templateMethod_framework SHARED Framework.cpp)
target_link_libraries(templateMethod_framework common)
