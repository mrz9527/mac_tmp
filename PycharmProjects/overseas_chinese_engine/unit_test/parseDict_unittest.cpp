/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: ParseDictTest
* Author: ZhouKang
* Create: 2021-7-12
*/
#include "gtest/gtest.h"
#include "ParseBinDict.h"
#include "utf16writer.h"

using namespace ChineseIme;

class ParseDictTest : public ::testing::Test {
public:
    ParseBinDict dict;
};

void PrintDict(const char* logfile, const ParseBinDict &dict)
{
    Utf16Writer writer;
    bool flag = writer.OpenNewFile(logfile);
    EXPECT_TRUE(flag) << "OpenNewFile failed";

    uint32 wordItemCount = dict.GetWordItemCount();
    EXPECT_TRUE(wordItemCount > 0) << "wordItemCount == 0";

    char buf[8096] = {0};
    for (uint32 i = 0; i < wordItemCount; ++i) {
        uint8 ziCount = dict.GetZiCount(i);
        uint8 key1Len = dict.GetKey1Len(i);
        uint8 key2Len = dict.GetKey2Len(i);
        const char* key1Str = dict.GetKey1Str(i);
        const char* key2Str = dict.GetKey2Str(i);

        memset(buf, 0, 8096);
        int pos = 0;
        int j = 0;
        int k = 0;

        while (key1Str != nullptr && j < key1Len +
                                         ziCount) { // 61 00----> 00 61  //记住一条：小端，数据的低字节保存到低地址，高字节保存到高地址 'a'：0x61，用2个字节表示0x0061，小端，低地址保存0x61，高地址保存0x00
            buf[pos++] = *(key1Str + j);              //低地址buf[pos] 高地址buf[pos+1]
            buf[pos++] = 0; //
            ++j;
        }

        j = 0;
        while (key2Str != nullptr && j < key2Len + ziCount) {
            buf[pos++] = *(key2Str + j);
            buf[pos++] = 0;

            ++j;
        }

        uint16 wordCount = dict.GetWordCount(i);
        j = 0;
        while (j < wordCount) {
            const char* wordStr = dict.GetWordStr(i, j);
            k = 0;
            while (k < ziCount) {
                buf[pos++] = *(wordStr + k * 2);
                buf[pos++] = *(wordStr + k * 2 + 1);

                ++k;
            }

            const char* toneStr = dict.GetToneStr(i, j);
            k = 0;
            while (toneStr != nullptr && k < ziCount) {
                buf[pos++] = *(toneStr + k);
                buf[pos++] = 0;
                ++k;
            }
            buf[pos++] = ' ';
            buf[pos++] = 0;

            uint16 freq = dict.GetFreq(i, j);
            char freqStr[256] = {0};
            sprintf(freqStr, "%d", freq);
            int len = strlen(freqStr);
            k = 0;
            while (k < len) {
                buf[pos++] = freqStr[k];
                buf[pos++] = 0;
                ++k;
            }

            buf[pos++] = ' ';
            buf[pos++] = 0;

            ++j;
        }


        buf[pos++] = 0xA;
        buf[pos++] = 0;

        writer.Write(buf, pos);
    }

    writer.Close();

}

TEST_F(ParseDictTest, ParseZhuYinTest)
{
    bool flag = dict.Load("../../dict/zhuyin.dat");
    EXPECT_TRUE(flag) << "注音词典加载失败";

    flag = dict.SetSortType(DictKey::DICT_KEY_1);
    EXPECT_TRUE(flag) << "注音key1排序失败";
    //PrintDict("../../build/midoutput/zhuyin_key1.txt", dict);

    flag = dict.SetSortType(DictKey::DICT_KEY_2);
    EXPECT_TRUE(flag) << "注音key2排序失败";
    //PrintDict("../../build/midoutput/zhuyin_key2.txt", dict);
}

TEST_F(ParseDictTest, ParsePinYinTest)
{

    bool flag = dict.Load("../../dict/pinyin_9key.dat");
    EXPECT_TRUE(flag) << "拼音词典加载失败";
    flag = dict.SetSortType(DictKey::DICT_KEY_1);
    EXPECT_TRUE(flag) << "拼音key1排序失败";
    //PrintDict("../../build/midoutput/pinyin_key1.txt", dict);

    flag = dict.SetSortType(DictKey::DICT_KEY_2);
    EXPECT_TRUE(flag) << "拼音key2排序失败";
    //PrintDict("../../build/midoutput/pinyin_key2.txt", dict);
}

TEST_F(ParseDictTest, ParseCangJieTest)
{
    bool flag = dict.Load("../../dict/cangjie.dat");
    EXPECT_TRUE(flag) << "仓颉词典加载失败";
    flag = dict.SetSortType(DictKey::DICT_KEY_1);
    EXPECT_TRUE(flag) << "仓颉key1排序失败";
    //PrintDict("../../build/midoutput/cangjie_key1.txt", dict);

    flag = dict.SetSortType(DictKey::DICT_KEY_2);
    EXPECT_TRUE(flag) << "仓颉key2排序失败";
    //PrintDict("../../build/midoutput/cangjie_key2.txt", dict);
}

TEST_F(ParseDictTest, ParseBiHuaTest)
{
    bool flag = dict.Load("../../dict/bihua_simp.dat");
    EXPECT_TRUE(flag) << "笔画词典加载失败";

    flag = dict.SetSortType(DictKey::DICT_KEY_2);
    EXPECT_TRUE(flag) << "笔画key2排序失败";
    //PrintDict("../../build/midoutput/bihua_key2.txt", dict);
}