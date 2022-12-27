# Created by zhoukang on 2021/9/6

################################################################################################
# cmake3.4.1版本
cmake_minimum_required(VERSION 3.4.1)

# project name
project(overseasChineseEngine)

# c++11版本
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 设置编译选项
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -ffunction-sections -fdata-sections -fPIC")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -fPIC -fstack-protector")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s")
# -z 编译选项，在clion中不识别，导致clion中编译失败，这里临时关闭
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now")

# 根据编译类型，设定宏开关，在debug模式下，开启宏OCE_DEBUG，同时设定变量ENABLE_DEBUG为true
if (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE EQUAL "None" OR NOT CMAKE_BUILD_TYPE)
    add_definitions(-DOCE_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(ENABLE_DEBUG true)
endif ()

# 头文件路径
include_directories(
        ./third_part/mini_boost
        ./core/predict
        ./core/common/include
        ./core/parseDict/include
        ./core/makeDict/include
        ./core)

# 华为安全编码规范函数头文件路径
set(HUAWEI_SECURE_C_DIR ./platform/huawei_secure_c)
include_directories(
        ${HUAWEI_SECURE_C_DIR}
        ${HUAWEI_SECURE_C_DIR}/include
        ${HUAWEI_SECURE_C_DIR}/src
        )

# TODO 临时设置，方便clion上编译，后续去掉该设置，通过cmake命令行参数给定
set(OCE_PC true)

# 根据运行环境，设置头文件路径
if (OCE_PC)   # 运行环境是PC模式
    include_directories(
            ./third_part/googletest/include)
else ()       # 安卓模式
    include_directories(
            ./android) #jni目录
endif ()

if (OCE_DEBUG)
    include_directories(
            ./core/testBatch})
endif ()

# 根据运行环境，设置库路径
if (OCE_PC)
    link_directories(
            ./third_part/googletest/lib)
endif ()

# 源文件清单
set(OCE_SRC_LIST
        # core/common目录
        ./core/common/src/Utils.cpp
        ./core/common/src/Utf16Char.cpp
        ./core/common/src/KeyMapping.cpp

        # core/makeDict目录
        ./core/makeDict/src/MakeBinDict.cpp
        ./core/makeDict/src/FreqConvertScore.cpp

        # core/parseDict目录
        ./core/parseDict/src/ParseBinDict.cpp

        # core/predict目录
        ./core/predict/NextWordDicTree.cpp

        # core目录
        ./core/KeyboardMng.cpp
        ./core/KeyboardTypeBase.cpp
        ./core/OverseasChineseEngineIME.cpp
        ./core/SearchResultsMng.cpp
        ./core/testBatch/TestBatch.cpp)

# 根据运行环境，设置源文件清单
if (NOT OCE_PC)
    set(OCE_SRC_LIST ${OCE_SRC_LIST} ./android/OverseasChineseEngineJni.cpp)
else ()
    set(OCE_SRC_LIST ${OCE_SRC_LIST} ./core/common/src/utf16writer.cpp)
endif ()

# 设置动态链接库名称
if (OCE_PC)
    set(OCE_LIB_NAME overseasCore)
else ()
    set(OCE_LIB_NAME oceJni)
endif ()

# 添加动态库
add_library(${OCE_LIB_NAME} SHARED ${OCE_SRC_LIST})

add_subdirectory(platform/huawei_secure_c)

target_link_libraries(${OCE_LIB_NAME} PRIVATE huawei_secure_c)

# 链接库文件
if (ENABLE_DEBUG)
    message(STATUS "-------------enable_debug")
    find_library(LIBJNILOG log)
    if (LIBJNILOG)
        add_definitions(-DOCE_JNILOG)
        target_link_libraries(${OCE_LIB_NAME} PRIVATE ${LIBJNILOG})
    endif ()
endif ()

# 根据运行环境，添加子目录
if (OCE_PC) # 在非安卓模式下
    add_subdirectory(unit_test)
    add_executable(DictBuild main.cpp)
    target_link_libraries(DictBuild PRIVATE ${OCE_LIB_NAME})
endif ()

add_subdirectory(tools)

################################################################################################