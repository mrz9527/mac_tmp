/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: zhuyin_unittest.cpp
 * Author: van
 * Create: 2021/8/9 2:43 下午
 */
#include "EngineBaseTest.h"
#include "Utils.h"
#include "OverseasChineseEngineIME.h"

using namespace ChineseIme;

//测试注音+音调
TEST_F(EngineBaseTest, Zhuyin26Test)
{
    ChineseIme::InitDict("../../dict/zhuyin.dat",
                         "../../dict/predict_trad.dat",
                         ChineseIme::KeyboardType::KBT_ZHUYIN_26);

    const char* inputStr = "0F3S"; // 打算查找 0F 3S	17 10	伯父VX	209.485777119	撥付UX	234.348853194
    const char* expectStr = "伯父";

    size_t candCount = ChineseIme::Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "注音26键下查找:" << inputStr << "失败";

    bool findFlag = false;
    printf("total number:%zu\n", candCount);
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        ChineseIme::GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        printf("%d %s\n", i, candUtf8.c_str());
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    char16 zhuyinRes[128] = {u'\0'};
    char16* zhuyinRes2 = ChineseIme::GetDisplayPinyin(zhuyinRes, 128);
    printf("zhuyin result:%s\n", (Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(zhuyinRes)))).c_str());

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
    ChineseIme::Close();

    bool initFlag = ChineseIme::InitDict("../../dict/zhuyin.dat",
                                         "../../dict/predict_nextword_trad.dat",
                                         ChineseIme::KeyboardType::KBT_ZHUYIN_26);

    findFlag = false;
    candCount = ChineseIme::Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "注音26键下查找:" << inputStr << "失败";

    printf("total number2:%zu\n", candCount);
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        ChineseIme::GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        //printf("%d %s\n", i, candUtf8.c_str());
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;

    const char* inputStr2 = "9GR"; // 打算查找 0F 3S	17 10	伯父VX	209.485777119	撥付UX	234.348853194
    const char* expectStr2 = "可以";

    candCount = ChineseIme::Search(inputStr2, strlen(inputStr2));
    EXPECT_TRUE(candCount > 0) << "注音26键下查找:" << inputStr2 << "失败";

    findFlag = false;
    printf("total number:%zu\n", candCount);
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        ChineseIme::GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        //printf("%d %s\n", i, candUtf8.c_str());
        if (candUtf8.compare(expectStr2) == 0) {
            findFlag = true;
            break;
        }
    }

    char16 zhuyinRes3[128] = {u'\0'};
    char16* zhuyinRes4 = ChineseIme::GetDisplayPinyin(zhuyinRes3, 128);
    printf("zhuyin result:%s\n", (Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(zhuyinRes3)))).c_str());

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}
