/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: 解析二进制词典
* Author: ZhouKang
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_PARSEBINDICT_H
#define OVERSEASCHINESEENGINE_PARSEBINDICT_H

#include <vector>
#include "BasicTypeDef.h"
#include "Utf16Char.h"
#include "Log.h"

BEGIN_NAMESPACE_CHINESE_IME

// 词典item
struct DictItem {
    const char* wordItem = nullptr;
    const char* key1Str = nullptr;
    const char* key2Str = nullptr;
    uint8 key1LenEx = 0; // 包含空格
    uint8 key2LenEx = 0;

    DictItem() = default;

    DictItem(const DictItem &) = default;

    DictItem(DictItem &&) = default;

    DictItem &operator=(const DictItem &) = default;

    DictItem &operator=(DictItem &&) = default;

    DictItem(const char* wi, const char* k1s, uint8 k1l, const char* k2s, uint8 k2l)
        :wordItem(wi),
        key1Str(k1s),
        key2Str(k2s),
        key1LenEx(k1l),
        key2LenEx(k2l)
    {
    }

    // 按照key1来排序
    static bool CompareByKey1(const DictItem &left, const DictItem &right)
    {
        uint8 lkey1LenEx = left.key1LenEx;
        uint8 rkey1LenEx = right.key1LenEx;

        size_t pos = 0;
        while (pos < lkey1LenEx && pos < rkey1LenEx) {
            if (left.key1Str[pos] == right.key1Str[pos]) {
                ++pos;
                continue;
            } else if (left.key1Str[pos] == ' ') {
                return true;
            } else if (right.key1Str[pos] == ' ') {
                return false;
            } else {
                return left.key1Str[pos] < right.key1Str[pos];
            }
        }
        return lkey1LenEx < rkey1LenEx;
    }

    // 按照key2来排序
    static bool CompareByKey2(const DictItem &left, const DictItem &right)
    {
        uint8 lkey2LenEx = left.key2LenEx;
        uint8 rkey2LenEx = right.key2LenEx;

        size_t pos = 0;
        while (pos < lkey2LenEx && pos < rkey2LenEx) {
            if (left.key2Str[pos] == right.key2Str[pos]) {
                ++pos;
                continue;
            } else if (left.key2Str[pos] == ' ') {
                return true;
            } else if (right.key2Str[pos] == ' ') {
                return false;
            } else {
                return left.key2Str[pos] < right.key2Str[pos];
            }
        }
        return lkey2LenEx < rkey2LenEx;
    }
};

/* ParseBinDict类用于解析二进制词典
 * ParseBinDict类使用方法：
 * ParseBinDict dict;
 * // 先解析
 * dict.Load(...);
 *  * dict.SetSortType(...);
 * // 再获取查找范围
 * dict.Search(...);
 * // 最后提取数据
 * dict.GetXXX(...);
 * */
class ParseBinDict {
public:
    ParseBinDict() = default;

    ~ParseBinDict();

    bool Load(const char* binDictFile);

    bool SetSortType(DictKey sortKey); // 设置排序类型，方便排序

    // [first, second] 全封闭区间
    bool Search(char ch, std::pair<uint32, uint32> &range) const;

    uint8 GetZiCount(uint32 itemIndex) const;

    uint8 GetKey1Len(uint32 itemIndex) const;

    uint8 GetKey2Len(uint32 itemIndex) const;

    const char* GetKey1Str(uint32 itemIndex) const;

    const char* GetKey2Str(uint32 itemIndex) const;

    uint16 GetWordCount(uint32 itemIndex) const;

    const char* GetWordStr(uint32 itemIndex, uint16 wordIndex) const;

    const char* GetToneStr(uint32 itemIndex, uint16 wordIndex) const;

    uint16 GetFreq(uint32 itemIndex, uint16 wordIndex) const;

    uint32 GetWordItemCount() const
    {
        return m_vecDictItem.size();
    }

private:
    void ReleaseResource();
    bool Read(const char* binDictFile); // 加载词典
    bool Parse(); // 填充m_vecDictItem
    void SortWordItem(); // 对m_vecDictItem按照dictType进行排序
    void CalcDictItemIndex(); // 填充m_DictItemIndex

    void CalcDictItemIndexSub(uint32 index, char preKeyFirstCh, char curKeyFirstCh); // 作为CalcDictItemIndex拆分出来的子函数

    uint8 GetZiCount(const char* wordItem) const;

    uint8 GetKey1Len(const char* wordItem) const;

    uint8 GetKey2Len(const char* wordItem) const;

    const char* GetKey1Str(const char* wordItem) const;

    const char* GetKey2Str(const char* wordItem) const;

    uint16 GetWordCount(const char* wordItem) const;

    const char* GetWordStr(const char* wordItem, uint16 wordIndex) const;

    const char* GetToneStr(const char* wordItem, uint16 wordIndex) const;

    uint16 GetFreq(const char* wordItem, uint16 wordIndex) const;

    const char* NextDictItem(const char* wordItem) const;

    // 重复代码抽取成函数
    bool HasErrorDictMemLayout() const;

    const char* GetWordCountAddress(const char* wordItem) const;

private:
    bool m_hasLoad = false;
    const char* m_dictBuf = nullptr;
    uint32 m_dictBufLen = 0;

    DictKey m_sortKey = DictKey::DICT_KEY_UNKOWN;
    DictMemoryLayout m_dictMemLayout;
    std::vector<DictItem> m_vecDictItem;
    std::vector<std::pair<char, uint32>> m_vecDictItemIndex;
};

END_NAMESPACE_CHINESE_IME

#endif // OVERSEASCHINESEENGINE_PARSEBINDICT_H
