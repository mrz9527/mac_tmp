cmake_minimum_required(VERSION 3.5)

project(cg_huake_wanlin_demo1)

set(CMAKE_CXX_STANDARD 11)

# glad 头文件
include_directories(./third-part/glad/include)
# glfw 头文件
include_directories(./third-part/glfw-3.3.8/include)
## soil 头文件 soil安装到了系统默认路径，会自动寻找路径
#include_directories(./third-part/soil-1.07/include)
include_directories(third-part/glm-0.9.9.8)

# glfw 库
link_directories(./third-part/glfw-3.3.8/lib)
## soil库，soil安装到了系统默认路径，会自动寻找路径
#link_directories(./third-part/soil-1.07/lib)

add_executable(demo1_chapter3_triangle main.cpp Shader.cpp VertexData.cpp ./third-part/glad/src/glad.c)

target_link_libraries(demo1_chapter3_triangle glfw SOIL)