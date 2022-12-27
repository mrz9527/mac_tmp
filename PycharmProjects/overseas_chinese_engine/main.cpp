//
// Created by xm210508 on 2021/7/12.
//

#include <iostream>
//#include "DictBuild.h"
#include "OverseasChineseEngineIME.h"
#include <string.h>
#include <string>
#include <vector>
#include "Utf16Char.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
    // test search
    using namespace ChineseIme;
    std::string strSysDictPath = "/Users/xm210408/program2/overseas_chinese_engine/dict/pinyin_9key.dat";
    std::string strPredictPath = "/Users/xm210408/program2/overseas_chinese_engine/dict/predict_simp.dat";

    KeyboardType keyboardType = KeyboardType::KBT_PINYIN_9;
    bool isOk = InitDict(strSysDictPath.c_str(), strPredictPath.c_str(), keyboardType);
    if (false == isOk) {
        std::cout << "Dict init failed!" << std::endl;
        return -1;
    }

    char16 predictWords[static_cast<unsigned>(ArrayMaxLength::MAX_PREDICT_SIZE)] = {u'\0'};
    const char* searchStr = "322";
    size_t resultsCount = Search(searchStr, strlen(searchStr));
    std::vector<std::u16string> vecPinyin;// 左侧拼音列表容器
    // 获取左侧拼音列表，9键
    if (keyboardType == KeyboardType::KBT_PINYIN_9 || keyboardType == KeyboardType::KBT_ZHUYIN_9) {
        size_t pinyinCount = GetCandidatePinyinCount();
        char16 pinyinBuf[8] = {u'\0'};
        for (int i = 0; i < pinyinCount; ++i) {
            GetCandidatePinyin(i, pinyinBuf, sizeof(pinyinBuf));
            size_t pinyinLen = Utf16Strlen(pinyinBuf);
            std::u16string strPinyin(reinterpret_cast<const char16_t*>(pinyinBuf), pinyinLen);
            vecPinyin.push_back(strPinyin);
        }

        std::cout << "The left pinyin is: " << std::endl;
        for (auto &pinyin : vecPinyin) {
            std::cout << Utils::Utf16ToUtf8(pinyin) << std::endl;
        }
    }

    char16 resultBuf[128] = {u'\0'};
    std::vector<std::u16string> vecResults;
    for (int i = 0; i < resultsCount; ++i) {
        GetCandidate(i, resultBuf, sizeof(resultBuf));
        size_t ziCount = Utf16Strlen(resultBuf);
        std::u16string word(reinterpret_cast<const char16_t*>(resultBuf), ziCount);
        vecResults.push_back(word);
    }

    std::cout << "The result count is " << vecResults.size() << std::endl;
    for (auto x : vecResults) {
        std::cout << Utils::Utf16ToUtf8(x) << std::endl;
    }
    std::cout << std::endl;

    // 测试点击左侧拼音
    if (keyboardType == KeyboardType::KBT_PINYIN_9 || keyboardType == KeyboardType::KBT_ZHUYIN_9) {
        size_t pinyinSize = vecPinyin.size();
        if (pinyinSize > 0) {
            srand((unsigned int) time(NULL));
            size_t randomPinyinId = rand() % pinyinSize;
            size_t filterPinyinCount = ChooseCandidatePinyin(randomPinyinId);
            vecResults.clear();
            for (int i = 0; i < filterPinyinCount; ++i) {
                GetCandidate(i, resultBuf, sizeof(resultBuf));
                size_t ziCount = Utf16Strlen(resultBuf);
                std::u16string result(reinterpret_cast<const char16_t*>(resultBuf), ziCount);
                vecResults.push_back(result);
            }

            std::cout << "The filter results is: " << std::endl;
            for (auto &item : vecResults) {
                std::cout << Utils::Utf16ToUtf8(item) << std::endl;
            }

            size_t pinyinCount = GetCandidatePinyinCount();
            vecPinyin.clear();
            char16 pinyinBuf[8] = {u'\0'};
            for (int i = 0; i < pinyinCount; ++i) {
                GetCandidatePinyin(i, pinyinBuf, sizeof(pinyinBuf));
                size_t pinyinLen = Utf16Strlen(pinyinBuf);
                std::u16string strPinyin(reinterpret_cast<const char16_t*>(pinyinBuf), pinyinLen);
                vecPinyin.push_back(strPinyin);
            }

            std::cout << "The second left pinyin is: " << std::endl;
            for (auto &pinyin : vecPinyin) {
                std::cout << Utils::Utf16ToUtf8(pinyin) << std::endl;
            }

            pinyinSize = vecPinyin.size();
            if (pinyinSize > 0) {
                srand((unsigned int) time(NULL));
                size_t randomPinyinId = rand() % pinyinSize;
                size_t filterPinyinCount = ChooseCandidatePinyin(randomPinyinId);
                vecResults.clear();
                for (int i = 0; i < filterPinyinCount; ++i) {
                    GetCandidate(i, resultBuf, sizeof(resultBuf));
                    size_t ziCount = Utf16Strlen(resultBuf);
                    std::u16string result(reinterpret_cast<const char16_t*>(resultBuf), ziCount);
                    vecResults.push_back(result);
                }

                if (!vecResults.empty()) {
                    GetCandidate(0, predictWords, sizeof(predictWords));
                }
                std::cout << "The second filter results is: " << std::endl;
                for (auto &item : vecResults) {
                    std::cout << Utils::Utf16ToUtf8(item) << std::endl;
                }
            }
        }
    }

    GetCandidate(0, predictWords, sizeof(predictWords));
    char16 displayPinyin[static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)] = {u'\0'};
    GetDisplayPinyin(displayPinyin, sizeof(displayPinyin));
    size_t displayPinyinLen = Utf16Strlen(displayPinyin);
    std::u16string displayPinyinStr(reinterpret_cast<const char16_t*>(displayPinyin), displayPinyinLen);
    std::cout << "The display pinyin is: " << Utils::Utf16ToUtf8(displayPinyinStr) << std::endl;

    // 测试下一词联想
    std::cout << "下一词联想：" << std::endl;
    std::vector<std::u16string> vecPredictsResults;
    char16 predictBuf[static_cast<unsigned>(ArrayMaxLength::MAX_PREDICT_SIZE)] = {u'\0'};
    if (!vecResults.empty()) {
        size_t predictWordsLen = Utf16Strlen(predictWords);
        std::u16string preStr(reinterpret_cast<const char16_t*>(predictWords), predictWordsLen);
        std::cout << "历史词：" << Utils::Utf16ToUtf8(preStr) << std::endl;
        size_t preCount = GetPredictsCount(predictWords, predictWordsLen);
        std::cout << "推荐词的个数：" << preCount << std::endl;
        std::cout << "推荐词为：" << std::endl;
        for (size_t i = 0; i < preCount; ++i) {
            GetPredicts(i, predictBuf, sizeof(predictBuf));
            size_t predictLen = Utf16Strlen(predictBuf);
            std::u16string predictStr(reinterpret_cast<const char16_t*>(predictBuf), predictLen);
            std::cout << Utils::Utf16ToUtf8(predictStr) << std::endl;
            vecPredictsResults.push_back(predictStr);
        }
    }

    Close();
    //strSysDictPath = "../../dict/zhuyin.dat";
    keyboardType = KeyboardType::KBT_PINYIN_26;
    isOk = InitDict(strSysDictPath.c_str(), strPredictPath.c_str(), keyboardType);
    if (false == isOk) {
        std::cout << "Dict init failed!" << std::endl;
        return -1;
    }

    searchStr = "da";
    resultsCount = Search(searchStr, strlen(searchStr));

    vecResults.clear();
    for (int i = 0; i < resultsCount; ++i) {
        GetCandidate(i, resultBuf, sizeof(resultBuf));
        size_t ziCount = Utf16Strlen(resultBuf);
        std::u16string word(reinterpret_cast<const char16_t*>(resultBuf), ziCount);
        vecResults.push_back(word);
    }

    std::cout << "The result count is " << vecResults.size() << std::endl;
    for (auto x : vecResults) {
        std::cout << Utils::Utf16ToUtf8(x) << std::endl;
    }
    std::cout << std::endl;

    Close();

    return 0;
}