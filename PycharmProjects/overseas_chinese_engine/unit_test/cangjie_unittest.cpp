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
TEST_F(EngineBaseTest, CangjieTest)
{
    ChineseIme::InitDict("../../dict/cangjie.dat",
                         "../../dict/predict_trad.dat",
                         ChineseIme::KeyboardType::KBT_CANGJIE);

    //ChineseIme::ModifyKeyboardType(ChineseIme::KBT_CANGJIE,"../../dict/cangji.dat");

    const char* inputStr = "aarf"; // 打算查找 a arf	a af	日照	604.195219728
    const char* expectStr = "日照";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "仓颉下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}

//测试注音+音调
TEST_F(EngineBaseTest, FastCangjieTest)
{

    ChineseIme::InitDict("../../dict/cangjie.dat",
                         "../../dict/predict_trad.dat",
                         ChineseIme::KeyboardType::KBT_CANGJIE_FAST);



    //ChineseIme::ModifyKeyboardType(ChineseIme::KBT_CANGJIE,"../../dict/cangji.dat");

    const char* inputStr = "kg"; // 打算查找 a arf	a af	日照	604.195219728
    const char* expectStr = "在";

    size_t candCount = Search(inputStr, strlen(inputStr));
    EXPECT_TRUE(candCount > 0) << "快速仓颉下查找:" << inputStr << "失败";

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetCandidate(i, cand, 128);
        std::string candUtf8 = Utils::Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;
}
