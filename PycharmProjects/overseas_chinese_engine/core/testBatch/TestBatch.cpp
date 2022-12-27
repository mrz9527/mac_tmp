/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: 批量测试
* Author: YangNing
* Create: 2021-7-12
*/
#include "TestBatch.h"
#include "OverseasChineseEngineIME.h"

using namespace std;

BEGIN_NAMESPACE_CHINESE_IME
TestBatch::TestBatch()
{
}

TestBatch::~TestBatch()
{
}

void TestBatch::TestBatchUniform(
        string inputPath, string outputPath, string sumPath, string dicPath, string nextWordDicPath, KeyboardType type,
        bool isPredict)
{
    FILE* log = freopen("/sdcard/oce/log.txt", "a+", stdout);
    if (isPredict) {
        SearchNextWord(inputPath, outputPath, sumPath, dicPath, nextWordDicPath);
    } else {
        SearchWithDiffType(inputPath, outputPath, sumPath, dicPath, nextWordDicPath, type);
    }

    if (log != nullptr) {
        fclose(log);
    }
}

void TestBatch::SearchWithDiffType(
        string inputPath, string outputPath, string sumPath, string dicPath, string nextWordDicPath, KeyboardType type)
{
    string line;
    FILE* fp;
    ifstream in(inputPath);
    fp = fopen(outputPath.c_str(), "w");
    FILE* fpsum;
    fpsum = fopen(sumPath.c_str(), "a");
    bool result = InitDict(dicPath.c_str(),
                           nextWordDicPath.c_str(),
                           type);

    int sumline = 0;
    int shouping = 0;
    int shouxuan = 0;
    int xuanzhong = 0;
    unsigned long maxdiff = 0;
    unsigned long mindiff = 100000000;
    unsigned long sumTime = 0;
    unsigned long onceTime = 0;
    while (getline(in, line)) {
        sumline++;
        int count_shouping = 0;
        fprintf(fp, "%s ", line.c_str());
        string str = Split(line, " ")[1];
        string str2 = Split(line, " ")[0];
        const char* inputStr = str.c_str();
        const char* expectStr = str2.c_str();
        std::u16string u16Str = Utils::Utf8ToUtf16(inputStr);
        clock_t start = clock();
        size_t candCount = Search(inputStr, strlen(inputStr));
        clock_t end = clock();
        onceTime = end - start;

        mindiff = min(onceTime, mindiff);
        maxdiff = max(onceTime, maxdiff);
        sumTime = sumTime + onceTime;

        bool findFlag = false;
        bool isShouXuan = false;
        bool isShouPing = false;
        printf("total number:%zu\n", candCount);
        for (int i = 0; i < candCount; ++i) {
            char16 cand[128] = {u'\0'};
            ChineseIme::GetCandidate(i, cand, 128);
            std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
            printf("%d %s\n", i, candUtf8.c_str());
            count_shouping = count_shouping + candUtf8.length() / 3;
            if (candUtf8.compare(expectStr) == 0) {
                xuanzhong++;
                findFlag = true;
                if (i == 0) {
                    shouxuan++;
                    shouping++;
                    isShouXuan = true;
                } else if (count_shouping <= 12) {
                    shouping++;
                    isShouPing = true;
                }
                fprintf(fp, "%s", candUtf8.c_str());
                break;
            }
            count_shouping++;
            if (i == candCount - 1) {
                fprintf(fp, "%s", candUtf8.c_str());
            } else {
                fprintf(fp, "%s%c", candUtf8.c_str(), ',');
            }
        }
        if (isShouXuan) {
            fprintf(fp, " %s", "首选");
        } else if (isShouPing) {
            fprintf(fp, " %s", "首屏");
        } else if (findFlag) {
            fprintf(fp, " %s", "出词");
        } else {
            fprintf(fp, " %s", "未出词");
        }

        char16 zhuyinRes[128] = {u'\0'};
        char16* zhuyinRes2 = ChineseIme::GetDisplayPinyin(zhuyinRes, 128);
        printf("zhuyin result:%s\n",
               (Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(zhuyinRes)))).c_str());
        fprintf(fp, "%c", '\n');
    }
    in.close();

    fprintf(fp, "%s%d\n", "总数：", sumline);
    fprintf(fp, "%s%d%s%f\n", "首选：", shouxuan, "    首选率：", (double) shouxuan / sumline);
    fprintf(fp, "%s%d%s%f\n", "首屏：", shouping, "    首屏率：", (double) shouping / sumline);
    fprintf(fp, "%s%d%s%f\n", "出词：", xuanzhong, "    出词率：", (double) xuanzhong / sumline);
    fprintf(fp, "%s%lu\n", "总耗时（微秒）：", sumTime);
    fprintf(fp, "%s%f\n", "平均耗时（微秒）：", (double) sumTime / sumline);
    fprintf(fp, "%s%lu\n", "最长耗时（微秒）：", maxdiff);
    fprintf(fp, "%s%lu\n", "最短耗时（微秒）：", mindiff);
    fclose(fp);

    fprintf(fpsum, "原始测试文件：%s\n", inputPath.c_str());
    fprintf(fpsum, "%s%d\n", "总数：", sumline);
    fprintf(fpsum, "%s%d%s%f\n", "首选：", shouxuan, "    首选率：", (double) shouxuan / sumline);
    fprintf(fpsum, "%s%d%s%f\n", "首屏：", shouping, "    首屏率：", (double) shouping / sumline);
    fprintf(fpsum, "%s%d%s%f\n", "出词：", xuanzhong, "    出词率：", (double) xuanzhong / sumline);
    fprintf(fpsum, "%s%lu\n", "总耗时（微秒）：", sumTime);
    fprintf(fpsum, "%s%f\n", "平均耗时（微秒）：", (double) sumTime / sumline);
    fprintf(fpsum, "%s%lu\n", "最长耗时（微秒）：", maxdiff);
    fprintf(fpsum, "%s%lu\n", "最短耗时（微秒）：", mindiff);

    ChineseIme::Close();
    fclose(fpsum);
}

void
TestBatch::SearchNextWord(string inputPath, string outputPath, string sumPath, string dicPath, string nextWordDicPath)
{
    string line;
    FILE* fp;
    ifstream in(inputPath);
    fp = fopen(outputPath.c_str(), "w");
    FILE* fpsum;
    fpsum = fopen(sumPath.c_str(), "a");
    bool result = InitDict(dicPath.c_str(),
                           nextWordDicPath.c_str(),
                           ChineseIme::KeyboardType::KBT_PINYIN_26);

    int sumline = 0;
    int shouping = 0;
    int shouxuan = 0;
    int xuanzhong = 0;
    unsigned long maxdiff = 0;
    unsigned long mindiff = 100000000;
    unsigned long sumTime = 0;
    unsigned long onceTime = 0;
    while (getline(in, line)) {
        sumline++;
        int count_shouping = 0;
        fprintf(fp, "%s ", line.c_str());
        string str = Split(line, " ")[0];
        string str2 = Split(line, " ")[1];
        const char* inputStr = str.c_str();
        const char* expectStr = str2.c_str();
        std::u16string u16Str = Utils::Utf8ToUtf16(inputStr);
        clock_t start = clock();
        size_t candCount = ChineseIme::GetPredictsCount(u16Str.c_str(), u16Str.size());
        clock_t end = clock();
        onceTime = end - start;

        mindiff = min(onceTime, mindiff);
        maxdiff = max(onceTime, maxdiff);
        sumTime = sumTime + onceTime;
        bool findFlag = false;
        bool isShouXuan = false;
        bool isShouPing = false;
        printf("total number:%zu\n", candCount);
        for (int i = 0; i < candCount; ++i) {
            char16 cand[128] = {u'\0'};
            GetPredicts(i, cand, 128);
            std::string candUtf8 = Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(cand)));
            printf("%d %s\n", i, candUtf8.c_str());
            count_shouping = count_shouping + candUtf8.length() / 3;
            if (candUtf8.compare(expectStr) == 0) {
                xuanzhong++;
                findFlag = true;
                if (i == 0) {
                    shouxuan++;
                    shouping++;
                    isShouXuan = true;
                } else if (count_shouping <= 12) {
                    shouping++;
                    isShouPing = true;
                }
                fprintf(fp, "%s", candUtf8.c_str());
                break;
            }
            count_shouping++;
            if (i == candCount - 1) {
                fprintf(fp, "%s", candUtf8.c_str());
            } else {
                fprintf(fp, "%s%c", candUtf8.c_str(), ',');
            }
        }
        if (isShouXuan) {
            fprintf(fp, " %s", "首选");
        } else if (isShouPing) {
            fprintf(fp, " %s", "首屏");
        } else if (findFlag) {
            fprintf(fp, " %s", "出词");
        } else {
            fprintf(fp, " %s", "未出词");
        }

        char16 zhuyinRes[128] = {u'\0'};
        char16* zhuyinRes2 = ChineseIme::GetDisplayPinyin(zhuyinRes, 128);
        printf("zhuyin result:%s\n",
               (Utils::Utf16ToUtf8(std::u16string(reinterpret_cast<const char16_t*>(zhuyinRes)))).c_str());
        fprintf(fp, "%c", '\n');
    }
    in.close();

    fprintf(fp, "%s%d\n", "总数：", sumline);
    fprintf(fp, "%s%d%s%f\n", "首选：", shouxuan, "    首选率：", (double) shouxuan / sumline);
    fprintf(fp, "%s%d%s%f\n", "首屏：", shouping, "    首屏率：", (double) shouping / sumline);
    fprintf(fp, "%s%d%s%f\n", "出词：", xuanzhong, "    出词率：", (double) xuanzhong / sumline);
    fprintf(fp, "%s%lu\n", "总耗时（微秒）：", sumTime);
    fprintf(fp, "%s%f\n", "平均耗时（微秒）：", (double) sumTime / sumline);
    fprintf(fp, "%s%lu\n", "最长耗时（微秒）：", maxdiff);
    fprintf(fp, "%s%lu\n", "最短耗时（微秒）：", mindiff);
    fclose(fp);

    fprintf(fpsum, "原始测试文件：%s\n", inputPath.c_str());
    fprintf(fpsum, "%s%d\n", "总数：", sumline);
    fprintf(fpsum, "%s%d%s%f\n", "首选：", shouxuan, "    首选率：", (double) shouxuan / sumline);
    fprintf(fpsum, "%s%d%s%f\n", "首屏：", shouping, "    首屏率：", (double) shouping / sumline);
    fprintf(fpsum, "%s%d%s%f\n", "出词：", xuanzhong, "    出词率：", (double) xuanzhong / sumline);
    fprintf(fpsum, "%s%lu\n", "总耗时（微秒）：", sumTime);
    fprintf(fpsum, "%s%f\n", "平均耗时（微秒）：", (double) sumTime / sumline);
    fprintf(fpsum, "%s%lu\n", "最长耗时（微秒）：", maxdiff);
    fprintf(fpsum, "%s%lu\n", "最短耗时（微秒）：", mindiff);

    ChineseIme::Close();
    fclose(fpsum);
}

vector<string> TestBatch::Split(const string &str, const string &delim)
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

    delete[]strs;
    strs = nullptr;
    delete[]d;
    d = nullptr;

    return res;
}
END_NAMESPACE_CHINESE_IME