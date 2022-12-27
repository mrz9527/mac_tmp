/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: 批量测试
* Author: YangNing
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_TESTBATCH_H
#define OVERSEASCHINESEENGINE_TESTBATCH_H

#include <iostream>
#include <string>
#include <vector>
#include "Utf16Char.h"
#include "Utils.h"
#include "common/include/BasicTypeDef.h"
#include <fstream>

BEGIN_NAMESPACE_CHINESE_IME
class TestBatch {
public:
    TestBatch();

    ~TestBatch();

    void TestBatchUniform(
            std::string inputPath, std::string outputPath, std::string sumPath,
            std::string dicPath, std::string nextWordDicPath, KeyboardType type, bool isPredict);

private:
    void SearchWithDiffType(
            std::string inputPath, std::string outputPath, std::string sumPath,
            std::string dicPath, std::string nextWordDicPath, KeyboardType type);

    void SearchNextWord(
            std::string inputPath, std::string outputPath, std::string sumPath,
            std::string dicPath, std::string nextWordDicPath);

    std::vector<std::string> Split(const std::string &str, const std::string &delim);
};
END_NAMESPACE_CHINESE_IME
#endif //OVERSEASCHINESEENGINE_TESTBATCH_H
