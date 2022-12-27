/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: core_unittest
* Author: ZhouKang
* Create: 2021-7-12
*/
#include "gtest/gtest.h"
#include "OverseasChineseEngineIME.h"
#include "Utils.h"

using namespace ChineseIme;

// 拼音9键查找测试
TEST(CoreTest, Pinyin9Test)
{
    InitDict("../../dict/pinyin_9key.dat",
             "../../dict/predict_simp.dat",
             KeyboardType::KBT_PINYIN_9);

    const char* inputStr = "23744"; // 打算查找"测试"
    //const char* expectStr = "测试";
    const char* expectStr = "测";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "拼音九键下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}

TEST(CoreTest, Pinyin9FilterTest)
{
    //InitDict("../../dict/pinyin.dat",
    //         KBT_PINYIN_9);
    //
    InitDict("../../dict/pinyin_9key.dat",
             "../../dict/predict_simp.dat",
             KeyboardType::KBT_PINYIN_9);

    const char* inputStr = "23744"; // 打算查找"测试"
    //const char* expectStr = "测试";
    const char* expectStr = "测";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "拼音九键下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}

TEST(CoreTest, Pinyin26Test)
{
    InitDict("../../dict/pinyin_9key.dat",
             "../../dict/predict_simp.dat",
             KeyboardType::KBT_PINYIN_26);
    const char* inputStr = "cesh"; // 打算查找"测试" ce shi 测 shi
    //const char* expectStr = "测试";
    const char* expectStr = "测";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "拼音26键下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
        }
        std::cout << candUtf8 << " ";
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}

TEST(CoreTest, Bihua9Test)
{
    InitDict("../../dict/bihua_trad.dat",
             "../../dict/predict_trad.dat",
             KeyboardType::KBT_BIHUA_JIANTI);

    const char* inputStr = "444413432"; // 打算查找"济"
    const char* expectStr = "济";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "笔画九键下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}

TEST(CoreTest, Zhuyin9Test)
{
    InitDict("../../dict/zhuyin_9key.dat",
             "../../dict/predict_trad.dat",
             KeyboardType::KBT_ZHUYIN_9);

    const char* inputStr = "17508"; // 打算查找"把水WW"
    const char* expectStr = "把";
    //const char* expectStr = "把水";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "注音九键下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}

TEST(CoreTest, Zhuyin26Test)
{
    InitDict("../../dict/zhuyin.dat",
             "../../dict/predict_trad.dat",
             KeyboardType::KBT_ZHUYIN_26);

    const char* inputStr = "0F3S"; // 打算查找 0F 3S	17 10	伯父VX	209.485777119	撥付UX	234.348853194
    const char* expectStr = "伯父";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "注音26键下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}

TEST(CoreTest, CangjieTest)
{
    InitDict("../../dict/cangjie.dat",
             "../../dict/predict_trad.dat",
             KeyboardType::KBT_CANGJIE);

    const char* inputStr = "aarf"; // 打算查找 a arf	a af	日照	604.195219728
    const char* expectStr = "日照";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "仓颉下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}

TEST(CoreTest, CangjieFastTest)
{
    InitDict("../../dict/cangjie.dat",
             "../../dict/predict_trad.dat",
             KeyboardType::KBT_CANGJIE_FAST);

    const char* inputStr = "aaf"; // 打算查找 a arf	a af	日照	604.195219728
    const char* expectStr = "日照";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "快速仓颉下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}

TEST(CoreTest, Switch_Bihua9_To_Pinyin9Test)
{
    InitDict("../../dict/bihua_trad.dat",
             "../../dict/predict_trad.dat",
             KeyboardType::KBT_BIHUA_JIANTI);

    const char* inputStr = "444413432"; // 打算查找"济"
    const char* expectStr = "济";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "笔画九键下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;

    //ModifyKeyboardType(KBT_PINYIN_9);
    {
        InitDict("../../dict/pinyin_9key.dat",
                 "../../dict/predict_simp.dat",
                 KeyboardType::KBT_PINYIN_9);
        const char* inputStr = "23744"; // 打算查找"测试"
        //const char* expectStr = "测试";
        const char* expectStr = "测";

        size_t candCount = Search(inputStr, strlen(inputStr));
        EXPECT_TRUE(candCount > 0) << "拼音九键下查找:" << inputStr << "失败";

        bool findFlag = false;
        for (int i = 0; i < candCount; ++i) {
            char16 cand[128] = {u'\0'};
            GetCandidate(i, cand, 128);
            std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
            if (candUtf8.compare(expectStr) == 0) {
                findFlag = true;
                break;
            }
        }

        EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
    }
}



