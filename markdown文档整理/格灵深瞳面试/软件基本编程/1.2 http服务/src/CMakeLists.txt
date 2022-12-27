cmake_minimum_required(VERSION 3.5)

project(httpserver)
add_definitions(-std=c++11)

include_directories(/home/zhoukang/local/libevent/include)
link_directories(/home/zhoukang/local/libevent/lib)

add_executable(httpserver myhttpServer.cpp)
target_link_libraries(httpserver event)