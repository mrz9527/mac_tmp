cmake_minimum_required(VERSION 3.5.1)
project(design_mode)

set(CMAKE_CXX_STANDARD 11)

add_library(decorator_framework SHARED FileStream.cpp MemoryStream.cpp NetworkStream.cpp Decorator.cpp)
target_link_libraries(decorator_framework common)