/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: system dic info
* Author: ZhouKang
* Create: 2021-7-12
*/
#ifndef MAKE_DICT_DICT_INFO_H
#define MAKE_DICT_DICT_INFO_H

#include <cstddef>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include "BasicTypeDef.h"
#include "securecutil.h"

//
BEGIN_NAMESPACE_CHINESE_IME

/*
 * 词典前256个byte，存放版本号，文件大小，简介等信息，256—512byte存放不同首字母开头的词语（
 * 例如：ㄉ开始的注音，从什么位置开始，ㄐ开始的注音从什么位置开始），接下来存第一个拼音：*/
typedef struct SDictHeader {
    char version[static_cast<int>(DictHeadSize::DICT_HEADER_VERSION_SIZE)] = {0};
    uint32 fileSize = 0; //256 - 44 = 212 106
    char16 brief[static_cast<int>(DictHeadSize::DICT_HEADER_BRIEF_SIZE) / 2] = {0};

    uint32 wordItemCount = 0; // 词典的行数
    DictMemoryLayout memLayout; // 词典的内存布局

    void Init()
    {
        memset_s(version, sizeof(version), 0, sizeof(version));
        memset_s(brief, sizeof(brief), 0, sizeof(brief));
        memLayout.Init();

        fileSize = 0;
        wordItemCount = 0;
    }
} DictHeader; //512

struct DictWordItem {
public:
    // 例如：mei wan	634 926	没完	23374	每晚	22763	没玩	19040
    //uint8 pyCount = 0;   // 拼音个数 = 2
    //uint8 pyLen = 0;     // 拼音总长度 = 6
    std::vector<std::string> vecPy; // key1
    std::vector<std::string> vecNumber; // key2
    //uint16 wordCount = 0; // 词的个数
    std::vector<std::pair<std::u16string, uint16>> vecWordAndFreq;

private:
    static bool Compare_Freq(
            const std::pair<std::u16string, uint16> &left,
            const std::pair<std::u16string, uint16> &right)
    { // 内部词频采用降序
        return left.second > right.second;
    }

public:
    /* 生成dict时，采用默认参数
     * 从dict解析时，指定参数
     * */
    DictWordItem(int pyCount = 1, int wordCount = 1)
    {
        vecWordAndFreq.reserve(wordCount);
        for (auto &elem:vecWordAndFreq) {
            elem.first.reserve(4);
        }
        vecPy.reserve(pyCount);
        for (auto &elem:vecPy) {
            elem.reserve(6);
        }
        vecNumber.reserve(pyCount);
        for (auto &elem:vecNumber) {
            elem.reserve(6);
        }
    }

    void AdjustResource()
    {
        vecWordAndFreq.shrink_to_fit();
        vecPy.shrink_to_fit();
        vecNumber.shrink_to_fit();

        for (auto &elem:vecWordAndFreq) {
            elem.first.shrink_to_fit();
        }
        for (auto &elem:vecPy) {
            elem.shrink_to_fit();
        }
        for (auto &elem:vecNumber) {
            elem.shrink_to_fit();
        }
    }

    ~DictWordItem()
    {
        for (auto &elem:vecWordAndFreq) {
            std::u16string().swap(elem.first);
        }
        for (auto &elem:vecPy) {
            std::string().swap(elem);
        }
        for (auto &elem:vecNumber) {
            std::string().swap(elem);
        }

        std::vector<std::string>().swap(vecPy);
        std::vector<std::string>().swap(vecNumber);
        std::vector<std::pair<std::u16string, uint16>>().swap(vecWordAndFreq);
    }

    // DictWordItem外部拼音排序
    static bool Compare_Py(const DictWordItem &left, const DictWordItem &right)
    {
        size_t leftPySize = left.vecPy.size();
        size_t rightPySize = right.vecPy.size();

        size_t pos = 0;
        while (pos < leftPySize && pos < rightPySize) {
            int res = left.vecPy[pos].compare(right.vecPy[pos]);
            if (res != 0)
                return res < 0;
            ++pos;
        }

        if (leftPySize == rightPySize) // 两个拼音相同，也需要返回false，这是std::sort规定，不能出现<=或者>=，只要left<right，才为真，否者为假
            //return true;
            return false;
        else                            // 其中一个拼音是另一个拼音子串
            return leftPySize < rightPySize;
    }

    // DictWordItem外部排序：数字键码排序
    static bool Compare_Number(const DictWordItem &left, const DictWordItem &right)
    {
        size_t leftNumberSize = left.vecNumber.size();
        size_t rightNumberSize = right.vecNumber.size();

        size_t pos = 0;
        while (pos < leftNumberSize && pos < rightNumberSize) {
            int res = left.vecNumber[pos].compare(right.vecNumber[pos]);
            if (res != 0)
                return res < 0;
            ++pos;
        }

        if (leftNumberSize == rightNumberSize) //两个数字码相同，也需要返回false，这是std::sort规定，不能出现<=或者>=，只要left<right，才为真，否者为假
            return false;
        else                            // 其中一个数字码是另一个数字码的字串
            return leftNumberSize < rightNumberSize;
    }

    // DictWordItem内部排序：词频排序
    void SortByFreq()
    {
        if (vecWordAndFreq.size() <= 0) {
            return;
        }

        std::sort(vecWordAndFreq.begin(), vecWordAndFreq.end(), Compare_Freq);
    }
};
END_NAMESPACE_CHINESE_IME

#endif // MAKE_DICT_DICT_INFO_H