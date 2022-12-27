/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: KeyboardTypeBase
* Author: Changjie Hu
* Create: 2021-7-12
*/
#include <algorithm>
#include "Log.h"
#include "KeyboardTypeBase.h"

BEGIN_NAMESPACE_CHINESE_IME
KeyboardTypeBase::KeyboardTypeBase() { }

KeyboardTypeBase::~KeyboardTypeBase() { }

bool KeyboardTypeBase::Init(const char* sysDictPath, KeyboardType keyboardType)
{
    bool isSuc = LoadDict(sysDictPath, keyboardType);
    if (!isSuc) {
        Close();
        LOGE("Load dict %s failed!", sysDictPath);
        return isSuc;
    } else {
        m_searchResultsMng.Init(m_parseBinDict);
        m_searchResultsMng.SetKeyboardType(keyboardType);
        return true;
    }
}

void KeyboardTypeBase::Close()
{
    m_searchResultsMng.ReleaseMemory();
    // 释放内存
    if (m_parseBinDict) {
        m_parseBinDict.reset();

        m_keyboardType = KeyboardType::KBT_UNKNOWN;
        m_dictType = DictType::DICT_TYPE_UNKOWN;
        m_keyType = DictKey::DICT_KEY_UNKOWN;
    }
}

void KeyboardTypeBase::ModifyKeyboardType(KeyboardType keyboardType, const char* sysDictPath)
{
    m_keyboardType = keyboardType;

    DictKey keyType = SearchResultsMng::ParseDictKeyType(keyboardType);
    m_keyType = keyType;
    DictType dictType = SearchResultsMng::ParseDictType(keyboardType);
    if (m_parseBinDict) {
        if (dictType == m_dictType) {
            // 当修改键盘类型时，如果词典不变，则不重新加载词典，只对词典按keyType排序
            m_parseBinDict->SetSortType(keyType);
        } else {
            m_dictType = dictType;
            bool isSuc = m_parseBinDict->Load(sysDictPath);
            if (!isSuc) {
                Close();
            } else {
                m_parseBinDict->SetSortType(keyType);
            }
        }
    }

    m_searchResultsMng.ReallocMemory();
    m_searchResultsMng.SetKeyboardType(keyboardType);
}

bool KeyboardTypeBase::LoadDict(const char* sysDictPath, KeyboardType keyboardType)
{
    DictType dictType = SearchResultsMng::ParseDictType(keyboardType);
    m_dictType = dictType;
    m_keyboardType = keyboardType;

    if (!m_parseBinDict) {
        m_parseBinDict = std::make_shared<ParseBinDict>(ParseBinDict());
    }

    bool isSuc = false;
    DictKey keyType = SearchResultsMng::ParseDictKeyType(keyboardType);
    m_keyType = keyType;

    if (m_parseBinDict) {
        isSuc = m_parseBinDict->Load(sysDictPath);
        if (isSuc) {
            isSuc = m_parseBinDict->SetSortType(keyType);
        }
    }

    return isSuc;
}

void KeyboardTypeBase::ResetSearch()
{
    m_searchResultsMng.ClearResults();
}

size_t KeyboardTypeBase::WordSearch(const char* spsBuf, size_t spsLen)
{
    m_searchResultsMng.ClearResults();

    if (spsLen == 0 || spsBuf == nullptr) {
        LOGW("The spsBuf is nullptr or spsLen equals 0!");
        return 0;
    }
    // 因为上层传过来的搜索字符串可能没有'\0'结束符，因此需拷贝一份带'\0'的字符串
    char searchBuf[static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)] = {'\0'};
    size_t searchBufLen = sizeof(searchBuf);
    if (searchBufLen > spsLen) {
        if (memcpy_s(searchBuf, searchBufLen, spsBuf, spsLen) != EOK) {
            LOGW("memcpy_s failed!");
            return 0;
        }
        searchBuf[spsLen] = '\0';
    } else {
        if (memcpy_s(searchBuf, searchBufLen, spsBuf, searchBufLen - 1) != EOK) {
            LOGW("memcpy_s failed!");
            return 0;
        }
        searchBuf[searchBufLen - 1] = '\0';
    }
    size_t searchLen = strlen(searchBuf);

    // 目前的判断条件是如果输入的键码串有非法字符，则不查找
    if (!SearchResultsMng::IsValidSearchKeys(searchBuf, m_keyboardType)) {
        LOGW("The spsBuf is invalid!");
        return 0;
    }

    m_searchResultsMng.SetSearchKey(searchBuf, searchLen);

    if (nullptr != m_parseBinDict) {
        std::pair<uint32, uint32> searchRange;
        char firstChar = searchBuf[0];

        if (m_dictType == DictType::DICT_TYPE_BIHUA && firstChar == '6') {
            // 键盘类型是笔画时且首字为通配符时'*',通配符的键码为'6'
            std::pair<uint32, uint32> firstSearchRange;
            std::pair<uint32, uint32> endSearchRange;
            if (m_parseBinDict->Search('1', firstSearchRange) &&
                m_parseBinDict->Search('5', endSearchRange)) {
                searchRange.first = firstSearchRange.first;
                searchRange.second = endSearchRange.second;
                SearchWordInRange(searchRange, searchBuf, searchLen, true);
            }
        } else if (m_parseBinDict->Search(firstChar, searchRange)) {
            SearchWordInRange(searchRange, searchBuf, searchLen, false);
        }
    }

    m_searchResultsMng.UniqueResults(); // 对结果去重

    m_searchResultsMng.SortResults(); // 对结果进行排序

    return m_searchResultsMng.GetMatchResultCount(); // 返回结果数量
}

char16* KeyboardTypeBase::GetDisplayPinyin(char16* pinyinBuf, size_t pinyinLen)
{
    return m_searchResultsMng.GetDisplayPinyin(pinyinBuf, pinyinLen);
}

size_t KeyboardTypeBase::GetCandidatePinyinCount()
{
    return m_searchResultsMng.GetCandidatePinyinCount();
}

char16* KeyboardTypeBase::GetCandidatePinyin(size_t pinyinId, char16* pinyinBuf, size_t pinyinLen)
{
    return m_searchResultsMng.GetCandidatePinyin(pinyinId, pinyinBuf, pinyinLen);
}

size_t KeyboardTypeBase::ChooseCandidatePinyin(size_t pinyinId)
{
    return m_searchResultsMng.ChooseCandidatePinyin(pinyinId);
}

char16* KeyboardTypeBase::GetCandidates(size_t candId, char16* candBuf, size_t maxLen)
{
    if (m_searchResultsMng.CopyResult(candId, candBuf, maxLen) != nullptr) {
        return candBuf;
    } else {
        return nullptr;
    }
}

KeyMatchType KeyboardTypeBase::GetKeyMatch(
    const char* spsBuf, size_t spsLen, const char* keyBuf,
    size_t keyLen, uint8 ziCount)
{
    // 注音26键需单独处理，因为要处理音调
    if (m_keyboardType == KeyboardType::KBT_ZHUYIN_26) {
        return GetZhuyinKeyMatch(spsBuf, spsLen, keyBuf, keyLen, ziCount);
    } else {
        // 其它键盘类型统一处理
        if (spsLen == keyLen) {
            // 先判断是否完全匹配
            return JudgeKeyCompletedMatch(spsBuf, keyBuf, ziCount);
        } else if (spsLen < keyLen) {
            // 再判断是否部分匹配
            return JudgeKeyPartMatch(spsBuf, keyBuf, ziCount);
        } else {
            // 再判断是否包含匹配
            return JudgeKeyContainMatch(spsBuf, keyBuf, ziCount);
        }
    }
}

uint8 KeyboardTypeBase::GetKeyLength(uint32 row, DictKey keyType) const
{
    if (m_parseBinDict != nullptr) {
        if (keyType == DictKey::DICT_KEY_1) {
            return m_parseBinDict->GetKey1Len(row);
        } else if (keyType == DictKey::DICT_KEY_2) {
            return m_parseBinDict->GetKey2Len(row);
        }
    }
    return 0;
}

void KeyboardTypeBase::InsertMatchItem(uint32 row, KeyMatchType keyMatchType)
{
    if (m_parseBinDict == nullptr) {
        return;
    }

    uint16 wordCount = m_parseBinDict->GetWordCount(row);
    uint16 freq = 0;
    uint32 score = 0;
    size_t toneCount = m_searchResultsMng.GetToneNum();
    size_t ziCount = m_parseBinDict->GetZiCount(row);
    const char* pKeyTone = nullptr;

    for (uint16 col = 0; col < wordCount; ++col) {
        freq = m_parseBinDict->GetFreq(row, col);
        score = ComputeSortScore(keyMatchType, freq);

        // 注音26键特殊处理，需要针对音调过滤
        if (m_keyboardType == KeyboardType::KBT_ZHUYIN_26 && toneCount > 0) {
            pKeyTone = m_parseBinDict->GetToneStr(row, col);
            size_t minToneCount = std::min(toneCount, ziCount);
            size_t matchToneCount = GetMatchToneCount(minToneCount, pKeyTone);
            if (matchToneCount == minToneCount) {
                m_searchResultsMng.InsertMatchResult(row, col, keyMatchType, score);
            }
        } else {
            m_searchResultsMng.InsertMatchResult(row, col, keyMatchType, score);
        }
    }
}

uint32 KeyboardTypeBase::ComputeSortScore(KeyMatchType keyMatchType, uint32 freq) const
{
    if (keyMatchType != KeyMatchType::KMT_UNMATCH) {
        return freq;
    }
    return 0;
}

KeyMatchType KeyboardTypeBase::JudgeKeyCompletedMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount) const
{
    const char* pSps = spsBuf;
    const char* pKey = keyBuf;
    uint8 matchZiCount = 1;
    bool mayCommonMatch = false;
    int matchKeyCount = 0;

    while (*pSps != '\0' && *pKey != '\0') {
        if (m_dictType == DictType::DICT_TYPE_BIHUA && *pSps == '6') {
            // 笔画有通配符*，单独处理，*对应搜索键码6
            pKey++;
            pSps++;
            continue;
        }
        if (*pKey == ' ') {
            pKey++;
            matchZiCount++;
            continue;
        }
        if (*pSps != *pKey) {
            mayCommonMatch = true;
            break;
        }
        pKey++;
        pSps++;
        matchKeyCount++;
    }

    if (mayCommonMatch) {
        // 特殊处理，只要是首字符匹配，直接返回首字符匹配
        if (matchKeyCount == 1) {
            return KeyMatchType::KMT_FIRST_KEY_MATCH;
        } else if (matchKeyCount > 1 && matchZiCount == ziCount && *(pKey - 1) != ' ') {
            return KeyMatchType::KMT_COMMON_PREFIX_MATCH;
        } else {
            return KeyMatchType::KMT_UNMATCH;
        }
    } else {
        return KeyMatchType::KMT_COMPLETED_MATCH;
    }
}

KeyMatchType KeyboardTypeBase::JudgeKeyPartMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount) const
{
    const char* pSps = spsBuf;
    const char* pKey = keyBuf;
    uint8 matchZiCnt = 1;
    bool mayCommonMatch = false;
    int matchKeyCnt = 0;

    while (*pSps != '\0') {
        if (m_dictType == DictType::DICT_TYPE_BIHUA && *pSps == '6') {
            // 笔画有通配符*，单独处理，*对应搜索键码6
            pKey++;
            pSps++;
            continue;
        }
        if (*pKey == ' ') {
            matchZiCnt++;
            pKey++;
            continue;
        }
        if (*pSps != *pKey) {
            mayCommonMatch = true;
            break;
        }
        pKey++;
        pSps++;
        matchKeyCnt++;
    }

    if (mayCommonMatch) {
        // 特殊处理，只要是首字符匹配，直接返回首字符匹配
        if (matchKeyCnt == 1) {
            return KeyMatchType::KMT_FIRST_KEY_MATCH;
        } else if (matchKeyCnt > 1 && matchZiCnt == ziCount && *(pKey - 1) != ' ') {
            return KeyMatchType::KMT_COMMON_PREFIX_MATCH;
        } else {
            return KeyMatchType::KMT_UNMATCH;
        }
    } else {
        if (*pKey != '\0' && *pKey != ' ' && matchZiCnt == ziCount) {
            return KeyMatchType::KMT_PART_MATCH;
        } else {
            return KeyMatchType::KMT_UNMATCH;
        }
    }
}

KeyMatchType KeyboardTypeBase::JudgeKeyContainMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount) const
{
    const char* pSpellings = spsBuf;
    const char* pKey = keyBuf;
    uint8 matZiCount = 1;
    bool mayCommonMatch = false;
    int matKeyCount = 0;

    while (*pKey != '\0') {
        if (m_dictType == DictType::DICT_TYPE_BIHUA && *pSpellings == '6') {
            // 笔画有通配符*，单独处理，*对应搜索键码6
            pKey++;
            pSpellings++;
            continue;
        }
        if (*pKey == ' ') {
            pKey++;
            matZiCount++;
            continue;
        }
        if (*pSpellings != *pKey) {
            mayCommonMatch = true;
            break;
        }
        pKey++;
        pSpellings++;
        matKeyCount++;
    }

    if (mayCommonMatch) {
        // 特殊处理，只要是首字符匹配，直接返回首字符匹配
        if (matKeyCount == 1) {
            return KeyMatchType::KMT_FIRST_KEY_MATCH;
        } else if (matKeyCount > 1 && matZiCount == ziCount && *(pKey - 1) != ' ') {
            return KeyMatchType::KMT_COMMON_PREFIX_MATCH;
        } else {
            return KeyMatchType::KMT_UNMATCH;
        }
    } else {
        return KeyMatchType::KMT_CONTAIN_MATCH;
    }
}

KeyMatchType KeyboardTypeBase::GetZhuyinKeyMatch(
    const char* spsBuf, size_t spsLen,
    const char* keyBuf, size_t keyLen, uint8 ziCount)
{
    size_t validSpsLen = GetZhuyinSpsLength(spsBuf);
    if (validSpsLen == keyLen) {
        return JudgeZhuyinKeyCompletedMatch(spsBuf, keyBuf, ziCount);
    } else if (validSpsLen < keyLen) {
        return JudgeZhuyinPartMatch(spsBuf, spsLen, keyBuf, ziCount);
    } else {
        return JudgeZhuyinContainMatch(spsBuf, keyBuf, ziCount);
    }
}

KeyMatchType KeyboardTypeBase::JudgeZhuyinKeyCompletedMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount)
{
    const char* pSps = spsBuf;
    const char* keyPtr = keyBuf;
    uint8 matZiCnt = 1;
    bool mayCommonMatch = false;
    int matKeyCnt = 0;
    m_searchResultsMng.ResetAllZhuyinTone(); // 每次判断是否匹配前先将注音音调置空

    while (*pSps != '\0' && *keyPtr != '\0') {
        if (*keyPtr == ' ') {
            if (SearchResultsMng::IsZhuyinTone(*pSps)) {
                m_searchResultsMng.SetZhuyinTone(matZiCnt - 1, *pSps);
                pSps++;
            } else {
                m_searchResultsMng.SetZhuyinTone(matZiCnt - 1, '*');
            }
            keyPtr++;
            matZiCnt++;
            continue;
        }

        if (*pSps != *keyPtr) {
            mayCommonMatch = true;
            break;
        }
        keyPtr++;
        pSps++;
        matKeyCnt++;
    }

    if (mayCommonMatch) {
        // 特殊处理，只要是首字符匹配，直接返回首字符匹配
        if (matKeyCnt == 1) {
            return KeyMatchType::KMT_FIRST_KEY_MATCH;
        } else if (matKeyCnt > 1 && matZiCnt == ziCount && *(keyPtr - 1) != ' ') {
            return KeyMatchType::KMT_COMMON_PREFIX_MATCH;
        } else {
            return KeyMatchType::KMT_UNMATCH;
        }
    } else {
        return KeyMatchType::KMT_COMPLETED_MATCH;
    }
}

KeyMatchType KeyboardTypeBase::JudgeZhuyinPartMatch(
    const char* spsBuf, size_t spsLen,
    const char* keyBuf, uint8 ziCount)
{
    const char* pSps = spsBuf;
    const char* pKey = keyBuf;
    uint8 matchZiNum = 1;
    bool mayCommonMatch = false;
    int matchKeyNum = 0;
    m_searchResultsMng.ResetAllZhuyinTone(); // 每次判断是否匹配前先将注音音调置空

    while (*pSps != '\0') {
        // 当匹配到最后一位是注音音调时，例如9GWRW，则要进行部分匹配，特殊处理
        if ((pSps - spsBuf == spsLen - 1) && SearchResultsMng::IsZhuyinTone(*pSps)) {
            m_searchResultsMng.SetZhuyinTone(matchZiNum - 1, *pSps);
            pSps++;
            continue;
        }
        if (*pKey == ' ') {
            if (SearchResultsMng::IsZhuyinTone(*pSps)) {
                m_searchResultsMng.SetZhuyinTone(matchZiNum - 1, *pSps);
                pSps++;
            } else {
                m_searchResultsMng.SetZhuyinTone(matchZiNum - 1, '*');
            }

            matchZiNum++;
            pKey++;
            continue;
        }

        if (*pSps != *pKey) {
            mayCommonMatch = true;
            break;
        }
        pKey++;
        pSps++;
        matchKeyNum++;
    }

    if (mayCommonMatch) {
        // 特殊处理，只要是首字符匹配，直接返回首字符匹配
        if (matchKeyNum == 1) {
            return KeyMatchType::KMT_FIRST_KEY_MATCH;
        } else if (matchKeyNum > 1 && matchZiNum == ziCount && *(pKey - 1) != ' ') {
            return KeyMatchType::KMT_COMMON_PREFIX_MATCH;
        } else {
            return KeyMatchType::KMT_UNMATCH;
        }
    } else {
        if (*pKey != '\0' && *pKey != ' ' && matchZiNum == ziCount) {
            return KeyMatchType::KMT_PART_MATCH;
        } else {
            return KeyMatchType::KMT_UNMATCH;
        }
    }
}

KeyMatchType KeyboardTypeBase::JudgeZhuyinContainMatch(const char* spsBuf, const char* keyBuf, uint8 ziCount)
{
    const char* spellingPtr = spsBuf;
    const char* keysPtr = keyBuf;
    uint8 matZiNum = 1;
    bool mayCommonMatch = false;
    int matKeyNum = 0;
    m_searchResultsMng.ResetAllZhuyinTone(); // 每次判断是否匹配前先将注音音调置空

    while (*keysPtr != '\0') {
        if (*keysPtr == ' ') {
            if (SearchResultsMng::IsZhuyinTone(*spellingPtr)) {
                m_searchResultsMng.SetZhuyinTone(matZiNum - 1, *spellingPtr);
                spellingPtr++;
            } else {
                m_searchResultsMng.SetZhuyinTone(matZiNum - 1, '*');
            }

            matZiNum++;
            keysPtr++;
            continue;
        }

        if (*spellingPtr != *keysPtr) {
            mayCommonMatch = true;
            break;
        }
        keysPtr++;
        spellingPtr++;
        matKeyNum++;
    }

    if (mayCommonMatch) {
        // 特殊处理，只要是首字符匹配，直接返回首字符匹配
        if (matKeyNum == 1) {
            return KeyMatchType::KMT_FIRST_KEY_MATCH;
        } else if (matKeyNum > 1 && matZiNum == ziCount && *(keysPtr - 1) != ' ') {
            return KeyMatchType::KMT_COMMON_PREFIX_MATCH;
        } else {
            return KeyMatchType::KMT_UNMATCH;
        }
    } else {
        return KeyMatchType::KMT_CONTAIN_MATCH;
    }
}

uint8 KeyboardTypeBase::GetZhuyinSpsLength(const char* spsBuf) const
{
    uint8 length = 0;
    const char* pSps = spsBuf;
    while (*pSps != '\0') {
        if (SearchResultsMng::IsZhuyinTone(*pSps)) {
            pSps++;
            continue;
        }
        length++;
        pSps++;
    }

    return length;
}

void KeyboardTypeBase::SearchWordInRange(const std::pair<uint32, uint32> &searchRange,
    const char* searchBuf, size_t searchLen, bool isComMatch)
{
    char keyBuf[static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)] = {'\0'};
    uint32 beginIndex = searchRange.first;
    uint32 endIndex = searchRange.second;
    uint8 keyLen = 0;
    KeyMatchType keyMatchType = KeyMatchType::KMT_UNMATCH;
    // 开始在范围[beginIndex, endIndex]内搜索
    for (uint32 i = beginIndex; i <= endIndex; ++i) {
        m_searchResultsMng.CopyKeyStr(i, m_keyType, keyBuf, sizeof(keyBuf));
        keyLen = GetKeyLength(i, m_keyType);
        uint8 ziCount = m_parseBinDict->GetZiCount(i);

        keyMatchType = GetKeyMatch(searchBuf, searchLen, keyBuf, keyLen, ziCount);
        if (keyMatchType == KeyMatchType::KMT_UNMATCH) {
            continue;
        } else if (isComMatch == true && keyMatchType != KeyMatchType::KMT_COMPLETED_MATCH) {
            continue;
        } else {
            InsertMatchItem(i, keyMatchType);
        }
    }
}

size_t KeyboardTypeBase::GetMatchToneCount(size_t minToneCount, const char* pKeyTone)
{
    size_t matchToneCount = 0; // 匹配的音调个数
    for (size_t i = 0; i < minToneCount; ++i) {
        char keyToneChar = *pKeyTone;
        char spsToneChar = m_searchResultsMng.GetZhuyinTone(i);
        if (spsToneChar == '*' || spsToneChar == keyToneChar) {
            matchToneCount++;
            pKeyTone++;
        } else {
            break;
        }
    }
    return matchToneCount;
}
END_NAMESPACE_CHINESE_IME