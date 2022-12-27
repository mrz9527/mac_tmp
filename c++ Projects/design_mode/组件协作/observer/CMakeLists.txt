cmake_minimum_required(VERSION 3.5.1)
project(design_mode)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Framework)

add_executable(observer_app Application/ConsoleLogObserver.cpp Application/FileLogObserver.cpp Application/ConcreteSubject.cpp main.cpp)
target_link_libraries(observer_app observer_framework)