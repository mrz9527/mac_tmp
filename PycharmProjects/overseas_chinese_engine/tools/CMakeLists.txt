# Created by zhoukang on 2021/9/24

################################################################################################
# cmake3.4.1版本
cmake_minimum_required(VERSION 3.4.1)

# c++11版本
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 编译类型
set(CMAKE_BUILD_TYPE Debug)

# 设置编译选项
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -fPIC -fstack-protector")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s")

add_definitions(-D___BUILD_MODEL___)

# 头文件路径
include_directories(
        ../core/common/include
        ../core/makeDict/include
        ../core/predict
        ../platform/huawei_secure_c
        ../platform/huawei_secure_c/include
        ../platform/huawei_secure_c/src
        )

# 源文件清单
set(COMMEN_AND_MAKEDICT_LIST
        # ../common目录
        ../core/common/src/Utils.cpp
        ../core/common/src/Utf16Char.cpp
        ../core/common/src/KeyMapping.cpp

        # core/makeDict目录
        ../core/makeDict/src/MakeBinDict.cpp
        ../core/makeDict/src/FreqConvertScore.cpp
        ../core/predict/NextWordDicTree.cpp
        MakeDict_Main.cpp
        )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
# 添加可执行文件
add_executable(makeDictTool ${COMMEN_AND_MAKEDICT_LIST})
target_link_libraries(makeDictTool PRIVATE huawei_secure_c)

################################################################################################