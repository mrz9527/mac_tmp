/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: KeyboardTypeBase
* Author: Changjie Hu
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_KEYBOARDTYPEBASE_H
#define OVERSEASCHINESEENGINE_KEYBOARDTYPEBASE_H

#include <memory>
#include "common/include/BasicTypeDef.h"
#include "parseDict/include/ParseBinDict.h"
#include "SearchResultsMng.h"
#include "securec.h"
#include "securectype.h"

BEGIN_NAMESPACE_CHINESE_IME
class KeyboardTypeBase {
public:
    KeyboardTypeBase();

    virtual ~KeyboardTypeBase();

    virtual bool Init(const char* sysDictPath, KeyboardType keyboardType);

    virtual void Close();

    virtual void ModifyKeyboardType(KeyboardType keyboardType, const char* sysDictPath);

    virtual void ResetSearch();

    virtual size_t WordSearch(const char* spsBuf, size_t spsLen);

    virtual char16* GetDisplayPinyin(char16* pinyinBuf, size_t pinyinLen);

    virtual size_t GetCandidatePinyinCount();

    virtual char16* GetCandidatePinyin(size_t pinyinId, char16* pinyinBuf, size_t pinyinLen);

    virtual size_t ChooseCandidatePinyin(size_t pinyinId);

    virtual char16* GetCandidates(size_t candId, char16* candBuf, size_t maxLen);

private:
    bool LoadDict(const char* sysDictPath, KeyboardType keyboardType);

    KeyMatchType GetKeyMatch(const char* spsBuf, size_t spsLen, const char* keyBuf, size_t keyLen, uint8 ziCount);

    uint8 GetKeyLength(uint32 row, DictKey keyType) const;

    void InsertMatchItem(uint32 row, KeyMatchType keyMatchType);

    uint32 ComputeSortScore(KeyMatchType keyMatchType, uint32 freq) const;

    KeyMatchType JudgeKeyCompletedMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount) const;

    KeyMatchType JudgeKeyPartMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount) const;

    KeyMatchType JudgeKeyContainMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount) const;

    // 获取注音26键键码串匹配类型，单独处理，因为注音26键需处理音调
    KeyMatchType GetZhuyinKeyMatch(const char* spsBuf, size_t spsLen, const char* keyBuf, size_t keyLen, uint8 ziCount);

    KeyMatchType JudgeZhuyinKeyCompletedMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount);

    KeyMatchType JudgeZhuyinPartMatch(const char* spsBuf, size_t spsLen, const char* keyBuf, uint8 ziCount);

    KeyMatchType JudgeZhuyinContainMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount);

    uint8 GetZhuyinSpsLength(const char* spsBuf) const; // 获取注音去除声调后的字符个数

    void SearchWordInRange(const std::pair<uint32, uint32> &searchRange, const char* searchBuf,
        size_t searchLen, bool isComMatch); // isComMatch是否是完全匹配类型，因为笔画以通配符开头只支持完全匹配

    size_t GetMatchToneCount(size_t minToneCount, const char* pKeyTone);

private:
    std::shared_ptr<ParseBinDict> m_parseBinDict; // 加载词典类，保存二进制词典缓存及提供搜索词典的方法

    KeyboardType m_keyboardType = KeyboardType::KBT_UNKNOWN; // 当前的键盘类型
    DictType m_dictType = DictType::DICT_TYPE_UNKOWN; // 当前的词典类型
    DictKey m_keyType = DictKey::DICT_KEY_UNKOWN; // 当前的key类型

    SearchResultsMng m_searchResultsMng;
};
END_NAMESPACE_CHINESE_IME // end namespace
#endif // OVERSEASCHINESEENGINE_KEYBOARDTYPEBASE_H
