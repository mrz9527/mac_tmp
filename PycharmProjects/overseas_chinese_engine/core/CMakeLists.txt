set(CORE_LIB_NAME overseasCore)

set(CORE_SRC_LIST
        ./common/src/Utils.cpp
        ./common/src/Utf16Char.cpp
        ./common/src/utf16writer.cpp
        ./common/src/KeyMapping.cpp
        ./makeDict/src/MakeBinDict.cpp
        ./makeDict/src/FreqConvertScore.cpp
        ./parseDict/src/ParseBinDict.cpp
        ./predict/PredictProcess.cpp
        ./predict/NextWordDicTree.cpp
        KeyboardMng.cpp
        KeyboardTypeBase.cpp
        OverseasChineseEngineIME.cpp
        SearchResultsMng.cpp
        testBatch/TestBatch.cpp)

add_library(${CORE_LIB_NAME} SHARED ${CORE_SRC_LIST})

set(ENABLE_DEBUG "true")
if (ENABLE_DEBUG)
    MESSAGE(WARNING "found enable_debug")
    add_definitions(-DDEBUG)
    find_library(LIBJNILOG log)
    if (NOT LIBJNILOG)
        MESSAGE(WARNING "not found log")
    else ()
        MESSAGE(WARNING "found log")
        add_definitions(-DJNILOG)
        target_link_libraries(${CORE_LIB_NAME} ${LIBJNILOG})
    endif ()
else ()
    MESSAGE(WARNING "not found enable_debug")
endif ()