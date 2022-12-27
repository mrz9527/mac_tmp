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

TEST_F(EngineBaseTest, PredictTest)
{
    ChineseIme::InitDict("../../dict/bihua_simp.dat",
                         "../../dict/predict_simp.dat",
                         ChineseIme::KeyboardType::KBT_BIHUA_FANTI);

    const char* inputStr = "天下无"; // 打算查找"济"
    const char* expectStr = "贼";

    clock_t start = clock();
    size_t candCount = GetPredictsCount((Utils::Utf8ToUtf16(inputStr)).c_str(), strlen(inputStr));
    clock_t end = clock();
    long diff1 = (end - start);
    printf("%s using time millisecond：%ldms\t%ldus\n", inputStr, diff1 / 1000, diff1);
    EXPECT_TRUE(candCount > 0) << "下一词联想:" << inputStr << "失败";
    printf("result count:%zu\n", candCount);

    bool findFlag = false;
    for (int i = 0; i < candCount; ++i) {
        char16 cand[128] = {u'\0'};
        GetPredicts(i, cand, 128);
        std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
        printf("%s\n", candUtf8.c_str());
        if (candUtf8.compare(expectStr) == 0) {
            findFlag = true;
            //break;
        }
    }

    EXPECT_TRUE(findFlag) << "查找结果没有:" << expectStr;

}


