cmake_minimum_required(VERSION 3.5.1)
project(design_mode)

set(CMAKE_CXX_STANDARD 11)

add_library(builder_framework SHARED HouseBuilder.cpp House.cpp)
target_link_libraries(builder_framework common)