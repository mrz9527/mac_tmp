cmake_minimum_required(VERSION 3.5.1)
project(design_mode)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Framework)

add_executable(templateMethod_app main.cpp Application/Application.cpp)
target_link_libraries(templateMethod_app templateMethod_framework)