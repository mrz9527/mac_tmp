/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: bihua_unittest.cpp
 * Author: van
 * Create: 2021/8/18 10:31 上午
 */
#include "EngineBaseTest.h"
#include "Utils.h"
#include "OverseasChineseEngineIME.h"

using namespace ChineseIme;

TEST_F(EngineBaseTest, BiHuaTest)
{
    ChineseIme::InitDict("../../dict/bihua_simp.dat",
                         "../../dict/predict_simp.dat",
                         ChineseIme::KeyboardType::KBT_BIHUA_FANTI);

    const char* inputStr = "444413432"; // 打算查找"济"
    const char* expectStr = "济";

    clock_t start = clock();
    size_t candCount = Search(inputStr, strlen(inputStr));
    clock_t end = clock();
    long diff1 = (end - start);
    printf("%s using time millisecond：%ldms\t%ldus\n", inputStr, diff1 / 1000, diff1);
    EXPECT_TRUE(candCount > 0) << "笔画九键下查找:" << inputStr << "失败";
    printf("result count:%zu\n", candCount);

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        printf("%s\n", candUtf8.c_str());
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;

}

TEST_F(EngineBaseTest, BiHuaTest2)
{
    ChineseIme::InitDict("../../dict/bihua_trad.dat",
                         "../../dict/predict_trad.dat",
                         ChineseIme::KeyboardType::KBT_BIHUA_FANTI);

    const char* inputStr = "411125124345251254311214444"; // 打算查找"济"
    const char* expectStr = "讜";

    clock_t start = clock();
    size_t candCount = Search(inputStr, strlen(inputStr));
    clock_t end = clock();
    long diff1 = (end - start);
    printf("%s using time millisecond：%ldms\t%ldus\n", inputStr, diff1 / 1000, diff1);
    EXPECT_TRUE(candCount > 0) << "笔画九键下查找:" << inputStr << "失败";
    printf("result count:%zu\n", candCount);

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        printf("%s\n", candUtf8.c_str());
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;

}


TEST_F(EngineBaseTest, BiHuaTestwildcard)
{
    ChineseIme::InitDict("../../dict/bihua_trad.dat",
                         "../../dict/predict_trad.dat",
                         ChineseIme::KeyboardType::KBT_BIHUA_FANTI);

    const char* inputStr = "1263"; // 打算查找"济"
    const char* expectStr = "丙";

    clock_t start = clock();
    size_t candCount = Search(inputStr, strlen(inputStr));
    clock_t end = clock();
    long diff1 = (end - start);
    printf("%s using time millisecond：%ldms\t%ldus\n", inputStr, diff1 / 1000, diff1);
    EXPECT_TRUE(candCount > 0) << "笔画九键下查找:" << inputStr << "失败";
    printf("result count:%zu\n", candCount);

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        printf("%s\n", candUtf8.c_str());
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            //break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;

}
