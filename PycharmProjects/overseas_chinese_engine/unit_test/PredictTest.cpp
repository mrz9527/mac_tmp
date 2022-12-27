/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: PredictTest
* Author: ZhouKang
* Create: 2021-7-12
*/
#include "gtest/gtest.h"
#include "Utils.h"
#include "NextWordDicTree.h"
#include <fstream>

using namespace ChineseIme;
using namespace std;

vector<string> split(const string &str, const string &delim)
{
    vector<string> res;
    if ("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char* strs = new char[str.length() + 1]; //不要忘了
    strcpy(strs, str.c_str());

    char* d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char* p = strtok(strs, d);
    while (p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }

    return res;
}


//TEST(PredictTest, PredictPinyinTest){
//    PredictProcess predictProcess;
//    if(predictProcess.Load("../../dict/pinyin_predict.dat")){
//        std::string u8Str = "天下无";
//        std::u16string u16Str = Utf8ToUtf16(u8Str);
//        size_t count = predictProcess.GetPredictWordCount(u16Str.c_str(), u16Str.size());
//        EXPECT_TRUE(count>0) << "GetPredict failed";
//
//        printf("%s 的下一词数量为:%d\n", u8Str.c_str(), count);
//
//        if (count>0){
//            printf("我的下一词个数为:%d，具体为：\n\t", count);
//            for (size_t index = 0; index < count; ++index) {
//                char16* word = predictProcess.GetPredictWord(index);
//                printf("%s \t", Utf16ToUtf8(word).c_str());
//            }
//        }
//    }
//}

/*
TEST(PredictTest, Predict3PinyinTest) {
    ifstream in("../../data/predict_2w.txt");
    bool flag = in.is_open();
    EXPECT_TRUE(flag) << "in.is_open faild";

    PredictProcess predictProcess;
    flag = predictProcess.Load("../../dict/pinyin_predict.dat");
    EXPECT_TRUE(flag) << "Load false";

    unsigned long maxdiff = 0;
    unsigned long mindiff = 100000000;
    string line;

    printf("---------------------------PredictProcess test start---------------------------\n");
    clock_t start = clock();
    while (getline(in, line)) {
        std::string u8Str = line;
        std::u16string u16Str = Utf8ToUtf16(u8Str);
        clock_t s = clock();
        size_t count = predictProcess.GetPredictWordCount(u16Str.c_str(), u16Str.size());
        clock_t e = clock();
        mindiff = std::min((unsigned long)(e-s), mindiff);
        maxdiff = std::max((unsigned long)(e-s), maxdiff);
    }
    clock_t end = clock();
    long diff1 = (end - start);
    printf("\ttotal cost time:%lfs,\t%lfms,\tcpu计数次数%ld\n", (double) diff1/CLOCKS_PER_SEC, (double) diff1/1000, diff1);
    printf("\t one min cost time:%lfs,\t%lfms,\tcpu计数次数%ld\n", (double)mindiff/CLOCKS_PER_SEC, (double) mindiff/1000, mindiff);
    printf("\t one max cost time:%lfs,\t%lfms,\tcpu计数次数%ld\n", (double)maxdiff/CLOCKS_PER_SEC, (double) maxdiff/1000, maxdiff);
    printf("---------------------------PredictProcess test end---------------------------\n");

    in.close();
}


TEST(PredictTest, Predict4PinyinTest) {
    ifstream in("../../data/predict_2w.txt");
    bool flag = in.is_open();
    EXPECT_TRUE(flag) << "in.is_open faild";

    NextWordDicTree nextWordDicTree;
    nextWordDicTree.LoadDic("../../dict/predict_nextword.dat");

    unsigned long maxdiff = 0;
    unsigned long mindiff = 100000000;
    string line;

    printf("---------------------------NextWordDicTree test start---------------------------\n");
    clock_t start = clock();
    while (getline(in, line)) {
        std::string u8Str = line;
        std::u16string u16Str = Utf8ToUtf16(u8Str);
        clock_t s = clock();
        size_t count = nextWordDicTree.GetPredictWordCount(u16Str.c_str(), u16Str.size());
        clock_t e = clock();
        mindiff = std::min((unsigned long)(e-s), mindiff);
        maxdiff = std::max((unsigned long)(e-s), maxdiff);
    }
    clock_t end = clock();
    long diff1 = (end - start);
    printf("\ttotal cost time:%lfs,\t%lfms,\tcpu计数次数%ld\n", (double) diff1/CLOCKS_PER_SEC, (double) diff1/1000, diff1);
    printf("\t one min cost time:%lfs,\t%lfms,\tcpu计数次数%ld\n", (double)mindiff/CLOCKS_PER_SEC, (double) mindiff/1000, mindiff);
    printf("\t one max cost time:%lfs,\t%lfms,\tcpu计数次数%ld\n", (double)maxdiff/CLOCKS_PER_SEC, (double) maxdiff/1000, maxdiff);
    printf("---------------------------NextWordDicTree test end---------------------------\n");

    in.close();
}
*/
