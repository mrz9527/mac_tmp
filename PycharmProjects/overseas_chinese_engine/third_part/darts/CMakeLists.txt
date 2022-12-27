cmake_minimum_required(VERSION 3.0)

set(DARTS_LIB_NAME darts)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/third_part/darts)

set(SRC_LIST
        darts.cpp
        mkdarts.cpp)

add_library(${DARTS_LIB_NAME} SHARED ${SRC_LIST})