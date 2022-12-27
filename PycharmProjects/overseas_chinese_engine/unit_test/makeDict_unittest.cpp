///*
//* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
//* Description: makeDictTest
//* Author: ZhouKang
//* Create: 2021-7-12
//*/
//#include "gtest/gtest.h"
//#include "MakeBinDict.h"
//
//using namespace ChineseIme;
//
////TEST(MakeDictTest, makeDictTest){
////    MakeBinDict dict;
////    char* version = "2021.7.28_pinyin";
////    std::u16string brief = u"kika first version";
////    dict.SetDictVersionAndBrief(version, strlen(version), brief.c_str(), brief.length());
////    const char* txtDictPath = "../../data";
////    const char* binDictPath = "../../dict";
////    bool flag = dict.MakeProcess(txtDictPath, binDictPath);
////    EXPECT_TRUE(flag);
////}
//
//
//TEST(MakeDictTest, makeDictTest1)
//{
//    MakeBinDict dict;
//    const char* version = "2021.7.28_pinyin";
//    std::u16string brief = u"kika first version";
//    dict.SetDictVersionAndBrief(version, strlen(version), brief.c_str(), brief.length());
//    const char* txtDictPath = "../../data/rawdict_utf16_pinyin.txt";
//    const char* binDictPath = "../../dict";
//    bool flag = dict.MakeProcess(txtDictPath, binDictPath);
//    EXPECT_TRUE(flag);
//}
//
//TEST(MakeDictTest, makeDictTest2)
//{
//    MakeBinDict dict;
//    const char* version = "2021.7.28_pinyin";
//    std::u16string brief = u"kika first version";
//    dict.SetDictVersionAndBrief(version, strlen(version), brief.c_str(), brief.length());
//    const char* txtDictPath = "../../data/rawdict_utf16_pinyin_0903.txt";
//    const char* binDictPath = "../../dict";
//    bool flag = dict.MakeProcess(txtDictPath, binDictPath);
//    EXPECT_TRUE(flag);
//}
//
//TEST(MakeDictTest, makeDictTest3)
//{
//    MakeBinDict dict;
//    const char* version = "2021.7.28_pinyin";
//    std::u16string brief = u"kika first version";
//    dict.SetDictVersionAndBrief(version, strlen(version), brief.c_str(), brief.length());
//    const char* txtDictPath = "../../data/rawdict_utf16_zhuyin.txt";
//    const char* binDictPath = "../../dict";
//    bool flag = dict.MakeProcess(txtDictPath, binDictPath);
//    EXPECT_TRUE(flag);
//}
//
//TEST(MakeDictTest, makeDictTest4)
//{
//    MakeBinDict dict;
//    const char* version = "2021.7.28_pinyin";
//    std::u16string brief = u"kika first version";
//    dict.SetDictVersionAndBrief(version, strlen(version), brief.c_str(), brief.length());
//    const char* txtDictPath = "../../data/rawdict_utf16_zhuyin_0903.txt";
//    const char* binDictPath = "../../dict";
//    bool flag = dict.MakeProcess(txtDictPath, binDictPath);
//    EXPECT_TRUE(flag);
//}