/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: SearchResultsMng
* Author: Changjie Hu
* Create: 2021-7-12
*/

#include <algorithm>
#include <set>
#include <cctype>
#include "Log.h"
#include "KeyMapping.h"
#include "SearchResultsMng.h"

BEGIN_NAMESPACE_CHINESE_IME

static constexpr uint32 RESULTS_RESERVE_CAPACITY = 1024;
static constexpr uint32 CANDIDATE_PINYIN_CAPACITY = 32;
static constexpr uint32 KEY9_CAPACITY = 10;
static constexpr uint32 KEY9_COLUMN_CAPACITY = 5;
static constexpr uint32 MAX_PINYIN_LENGTH = 8;

using Key9ColumnType = char[KEY9_COLUMN_CAPACITY];

static char pinyinKey2ToKey1[KEY9_CAPACITY][KEY9_COLUMN_CAPACITY] = {
    {'\0', '\0', '\0', '\0', '\0'}, // 0
    {'\0', '\0', '\0', '\0', '\0'}, // 1
    {'a', 'b', 'c', '\0', '\0'}, // 2
    {'d', 'e', 'f', '\0', '\0'}, // 3
    {'g', 'h', 'i', '\0', '\0'}, // 4
    {'j', 'k', 'l', '\0', '\0'}, // 5
    {'m', 'n', 'o', '\0', '\0'}, // 6
    {'p', 'q', 'r', 's', '\0'}, // 7
    {'t', 'u', 'v', '\0', '\0'}, // 8
    {'w', 'x', 'y', 'z', '\0'} // 9
};

static char zhuyinKey2ToKey1[KEY9_CAPACITY][KEY9_COLUMN_CAPACITY] = {
    {'R', 'S', 'T', '\0', '\0'}, // 0
    {'0', '1', '2', '3', '\0'}, // 1
    {'4', '5', '6', '7', '\0'}, // 2
    {'8', '9', ':', '\0', '\0'}, // 3
    {';', '<', '=', '\0', '\0'}, // 4
    {'>', '?', '@', 'A', '\0'}, // 5
    {'B', 'C', 'D', '\0', '\0'}, // 6
    {'E', 'F', 'G', 'H', '\0'}, // 7
    {'I', 'J', 'K', 'L', '\0'}, // 8
    {'M', 'N', 'O', 'P', 'Q'} // 9
};

bool UniqueWordItem::CompareWordItem(const UniqueWordItem &right, size_t index) const
{
    if (index >= sizeof(this->m_wordItem) / sizeof(char16)) {
        return false;
    }

    char16 leftChar16 = this->m_wordItem[index];
    char16 rightChar16 = right.m_wordItem[index];
    if (leftChar16 == u'\0' && rightChar16 == u'\0') {
        return false;
    } else if (leftChar16 == u'\0' && rightChar16 != u'\0') {
        return true;
    } else if (leftChar16 != u'\0' && rightChar16 == u'\0') {
        return false;
    } else if (leftChar16 == rightChar16) {
        return CompareWordItem(right, index + 1);
    } else {
        return leftChar16 < rightChar16;
    }
}

bool UniqueWordItem::operator<(const UniqueWordItem &right) const
{
    return CompareWordItem(right, 0);
}

ChineseIme::SearchResultsMng::SearchResultsMng() { }

ChineseIme::SearchResultsMng::~SearchResultsMng() { }

void ChineseIme::SearchResultsMng::Init(std::shared_ptr<ParseBinDict> parseBinDict)
{
    m_parseBinDict = parseBinDict;
    m_vecMatchItems.reserve(RESULTS_RESERVE_CAPACITY);
    m_vecFilterItems.reserve(RESULTS_RESERVE_CAPACITY);
    m_vecCandPinyin.reserve(CANDIDATE_PINYIN_CAPACITY);
    m_vecUniqueItems.reserve(RESULTS_RESERVE_CAPACITY);
    m_vecFirstKeyMatchItems.reserve(RESULTS_RESERVE_CAPACITY);

    memset_s(m_searchKey, static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH),
             '\0', static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH));
    memset_s(m_prefixKey, static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH),
             '\0', static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH));
    memset_s(m_zhuyinTone, static_cast<unsigned>(ArrayMaxLength::MAX_TONE_LENGTH),
             '\0', static_cast<unsigned>(ArrayMaxLength::MAX_TONE_LENGTH));
}

void ChineseIme::SearchResultsMng::ReleaseMemory()
{
    m_parseBinDict.reset(); // 释放在上层

    ReallocMemory();
}

void ChineseIme::SearchResultsMng::ClearResults()
{
    m_vecMatchItems.clear();
    m_setMatchRows.clear();
    m_vecFirstKeyMatchItems.clear();
    m_setFirstKeyMatchRows.clear();
    m_vecFilterItems.clear();
    m_vecCandPinyin.clear();
    m_vecUniqueItems.clear();

    memset_s(m_searchKey, static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH),
             '\0', static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH));
    memset_s(m_prefixKey, static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH),
             '\0', static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH));
    m_isFiler = false;
    ResetAllZhuyinTone();
}

void SearchResultsMng::InsertMatchResult(uint32 row, uint16 col, KeyMatchType matchType, uint32 score)
{
    if (matchType == KeyMatchType::KMT_FIRST_KEY_MATCH) {
        m_vecFirstKeyMatchItems.emplace_back(MatchItem(row, col, matchType, score));
        m_setFirstKeyMatchRows.insert(row);
    } else {
        m_vecMatchItems.emplace_back(MatchItem(row, col, matchType, score));
        m_setMatchRows.insert(row);
    }
}

char16* ChineseIme::SearchResultsMng::CopyResult(uint16 index, char16* dst, size_t maxLen)
{
    if (m_parseBinDict == nullptr || dst == nullptr) {
        LOGE("The m_parseBinDict is nullptr or dst is nullptr!");
        return nullptr;
    }

    bool isOk = false;
    MatchItem item = GetMatchItem(index, isOk);
    if (isOk == false) {
        return nullptr;
    }

    uint8 ziCount = m_parseBinDict->GetZiCount(item.m_row);
    const char* pWord = m_parseBinDict->GetWordStr(item.m_row, item.m_col);
    char16* pUniWord = reinterpret_cast<char16*>(const_cast<char*>(pWord));
    size_t maxCopyLen = (maxLen - 1) * sizeof(char16);

    if (ziCount < maxLen) {
        size_t copyLen = ziCount * sizeof(char16);
        if (memcpy_s(dst, maxCopyLen, pUniWord, copyLen) == EOK) {
            dst[ziCount] = u'\0'; // 末尾加结束符
        }
    } else {
        if (memcpy_s(dst, maxCopyLen, pUniWord, maxCopyLen) == EOK) {
            dst[maxLen - 1] = u'\0'; // 末尾加结束符
        }
    }

    return dst;
}

size_t SearchResultsMng::GetMatchResultCount()
{
    return m_vecUniqueItems.size();
}

char* SearchResultsMng::CopyKeyStr(uint32 row, DictKey keyType, char* dstBuf, size_t maxLen)
{
    if (nullptr != m_parseBinDict) {
        const char* keyStr = nullptr;
        uint8 keyLength = 0;
        if (keyType == DictKey::DICT_KEY_1) {
            keyStr = m_parseBinDict->GetKey1Str(row);
            keyLength = m_parseBinDict->GetKey1Len(row);
        } else if (keyType == DictKey::DICT_KEY_2) {
            keyStr = m_parseBinDict->GetKey2Str(row);
            keyLength = m_parseBinDict->GetKey2Len(row);
        }

        uint8 ziCount = m_parseBinDict->GetZiCount(row);
        size_t copyCount = keyLength + ziCount; // 末尾的拼音有一个空格符

        if (keyStr != nullptr && copyCount < maxLen) {
            if (strncpy_s(dstBuf, maxLen, keyStr, copyCount) == EOK) {
                dstBuf[copyCount] = '\0';
            }

            return dstBuf;
        }
    }
    return nullptr;
}

void SearchResultsMng::SortResults()
{
    SortMatchItems(m_vecUniqueItems);
}

size_t SearchResultsMng::GetCandidatePinyinCount()
{
    m_vecCandPinyin.clear();

    if (m_parseBinDict != nullptr) {
        if (!m_setMatchRows.empty()) {
            InsertCandidatePinyin(m_setMatchRows);
        } else {
            InsertCandidatePinyin(m_setFirstKeyMatchRows);
        }

        ProcessCandidatePinyin();

        if (m_vecCandPinyin.empty()) {
            return 0;
        } else {
            m_vecLastCandPinyin = m_vecCandPinyin;
            return m_vecCandPinyin.size();
        }
    }

    return 0;
}

char16* SearchResultsMng::GetCandidatePinyin(size_t pinyinId, char16* pinyinBuf, size_t pinyinLen)
{
    size_t pinyinSize = m_vecLastCandPinyin.size();
    if (pinyinId >= pinyinSize) {
        LOGW("The pinyinId is out of range!");
        return nullptr;
    }

    char pinyinTemp[MAX_PINYIN_LENGTH] = {'\0'};
    size_t pinyinTempCap = sizeof(pinyinTemp);
    PinyinItem pinyinItem = m_vecLastCandPinyin.at(pinyinId);
    size_t srcPinyinLen = strlen(pinyinItem.m_pinyin);
    if (pinyinTempCap <= srcPinyinLen) {
        if (memcpy_s(pinyinTemp, pinyinTempCap, pinyinItem.m_pinyin, pinyinTempCap - 1) != EOK) {
            LOGW("memcpy_s failed!");
            return nullptr;
        }
        pinyinTemp[pinyinTempCap - 1] = '\0';
    } else {
        if (memcpy_s(pinyinTemp, pinyinTempCap, pinyinItem.m_pinyin, srcPinyinLen) != EOK) {
            LOGW("memcpy_s failed!");
            return nullptr;
        }
        pinyinTemp[srcPinyinLen] = '\0';
    }

    // 将原始拼音串映射成上屏拼音串，查表转换
    size_t pinyinTempLen = strlen(pinyinTemp);
    ConvertKeyToUtf16Pinyin(pinyinTemp, pinyinTempLen, pinyinBuf, pinyinLen, m_keyboardType);

    return pinyinBuf;
}

size_t SearchResultsMng::ChooseCandidatePinyin(size_t pinyinId)
{
    if (m_isFiler == false) {
        m_isFiler = true;
    }

    if (pinyinId >= m_vecLastCandPinyin.size()) {
        LOGW("The pinyinId is out of range!");
        return 0;
    }

    m_vecFilterItems.clear();

    // 只针对单字有效
    if (memset_s(m_prefixKey, sizeof(m_prefixKey), '\0', sizeof(m_prefixKey)) != EOK) {
        return 0;
    }

    char filterPinyin[static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)] = {'\0'};
    GetCurFilterPinyin(pinyinId, filterPinyin, sizeof(filterPinyin));

    // 然后更新当前前缀拼音
    if (memcpy_s(m_prefixKey, static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH),
                 filterPinyin, static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)) != EOK) {
        LOGW("memcpy_s failed!");
        return 0;
    }
    size_t dstLen = strlen(m_prefixKey);
    m_prefixKey[dstLen] = ' ';
    m_prefixKey[dstLen + 1] = '\0';

    size_t filterPinLen = strlen(filterPinyin);
    // 按照过滤拼音在结果中过滤并去重
    FilterAndUniqueResults(filterPinyin, filterPinLen);

    // 对过滤后对结果排序
    SortFilterResults();

    return m_vecFilterItems.size();
}

void SearchResultsMng::FilterAndUniqueResults(const char* filterPinyin, size_t filterPinLen)
{
    if (!m_vecMatchItems.empty()) {
        FilterAndUniqueResults(filterPinyin, filterPinLen, m_vecMatchItems);
    } else {
        FilterAndUniqueResults(filterPinyin, filterPinLen, m_vecFirstKeyMatchItems);
    }
}

void SearchResultsMng::SortFilterResults()
{
    SortMatchItems(m_vecFilterItems);
}

void SearchResultsMng::SetSearchKey(const char* key, size_t len)
{
    if (key == nullptr || len >= static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)) {
        LOGW("SetSearchKey failed!");
        return;
    }
    if (memcpy_s(m_searchKey, static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH), key, len) != EOK) {
        LOGW("memcpy_s failed!");
        return;
    }
    m_searchKey[len] = '\0';
}

bool SearchResultsMng::CopyOnePinyin(uint32 row, size_t beginKey, char* dst, size_t dstLen)
{
    const char* key1Str = nullptr;

    key1Str = m_parseBinDict->GetKey1Str(row);
    uint8 key1Len = m_parseBinDict->GetKey1Len(row);
    if (beginKey >= key1Len || key1Str == nullptr) {
        return false;
    }

    size_t searchKeyCount = strlen(m_searchKey);
    size_t validPreKeyCount = GetValidPrefixKeyCount();
    if (validPreKeyCount >= searchKeyCount) {
        return false;
    }

    const char* key2Str = nullptr;
    key2Str = m_parseBinDict->GetKey2Str(row);
    uint8 key2Len = m_parseBinDict->GetKey2Len(row);

    size_t maxFilterCount = searchKeyCount - validPreKeyCount;
    const char* pKey1 = key1Str + beginKey;
    const char* pKey2 = key2Str + beginKey;
    size_t count = 0;
    size_t searchKeyLen = strlen(m_searchKey);
    const char* pSearchKey = m_searchKey + validPreKeyCount;

    while (count < searchKeyLen) {
        if (count >= dstLen - 1 || count >= maxFilterCount || count >= searchKeyLen) {
            return false;
        }

        if (*pKey2 != *pSearchKey) {
            return false;
        }

        dst[count] = *pKey1;
        count++;
        pKey1++;
        pKey2++;
        pSearchKey++;
    }

    dst[count] = '\0';
    return true;
}

bool SearchResultsMng::ContainPinyin(const char* keyPinyin, size_t keyLen, const char* tarPinyin, size_t tarLen)
{
    if (keyLen < tarLen) {
        return false;
    }

    const char* pKeyPin = keyPinyin;
    const char* pTarPin = tarPinyin;
    while (*pTarPin != '\0') {
        if (*pKeyPin != *pTarPin) {
            return false;
        }
        pKeyPin++;
        pTarPin++;
    }

    return true;
}

void SearchResultsMng::SetKeyboardType(KeyboardType keyboardType)
{
    m_keyboardType = keyboardType;
}

char16 SearchResultsMng::ConvertCharToUtf16(char key, KeyboardType keyboardType)
{
    if (keyboardType == KeyboardType::KBT_ZHUYIN_9 || keyboardType == KeyboardType::KBT_ZHUYIN_26) {
        return FindUtf16ByChar(KeyMapping::zhuyinMap, key);
    } else if (keyboardType == KeyboardType::KBT_BIHUA_FANTI || keyboardType == KeyboardType::KBT_BIHUA_JIANTI) {
        return FindUtf16ByChar(KeyMapping::bihuaMap, key);
    } else if (keyboardType == KeyboardType::KBT_CANGJIE || keyboardType == KeyboardType::KBT_CANGJIE_FAST) {
        return FindUtf16ByChar(KeyMapping::cangjieMap, key);
    } else {
        return static_cast<uint16>(key);
    }
}

char16 SearchResultsMng::FindUtf16ByChar(const std::map<char, char16>& mapUtf16, char key)
{
    auto iter = mapUtf16.find(key);
    if (iter != mapUtf16.end()) {
        return iter->second;
    }

    return static_cast<char16>(key);
}

void SearchResultsMng::ConvertKeyToUtf16Pinyin(const char* key, size_t keyLen,
    char16* pinyinBuf, size_t pinyinLen, KeyboardType keyboardType)
{
    const char* pKey = key;
    char16* pPinyin = pinyinBuf;
    if (pinyinLen - 1 <= keyLen) {
        // 最多只能转换pinyinLen - 1个字符
        size_t count = 0;
        while (count < pinyinLen - 1 && *pKey != '\0') {
            uint16 uchar = ConvertCharToUtf16(*pKey, keyboardType);
            *pPinyin = uchar;
            pKey++;
            pPinyin++;
            count++;
        }
        *pPinyin = u'\0';
    } else {
        // 可转换keyLen个字符
        size_t count = 0;
        while (count < keyLen && *pKey != '\0') {
            uint16 uchar = ConvertCharToUtf16(*pKey, keyboardType);
            *pPinyin = uchar;
            pKey++;
            pPinyin++;
            count++;
        }
        *pPinyin = u'\0';
    }
}

char16* SearchResultsMng::GetDisplayPinyin(char16* pinyinBuf, size_t pinyinLen)
{
    // 直接从结果中的第一个拼音转换成Unicode码串上屏
    bool isOk = false;
    MatchItem item = GetMatchItem(0, isOk);
    if (isOk == false) {
        return nullptr;
    }

    DictKey keyType = GetDictKeyType(m_keyboardType);
    char strTemp[static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)] = {'\0'};

    size_t searchKeyCount = strlen(m_searchKey);
    CopyNKeyStr(item.m_row, keyType, strTemp, sizeof(strTemp), searchKeyCount);

    size_t strTempLen = strlen(strTemp);
    ConvertKeyToUtf16Pinyin(strTemp, strTempLen, pinyinBuf, pinyinLen, m_keyboardType);

    return pinyinBuf;
}

DictKey SearchResultsMng::ParseDictKeyType(KeyboardType keyboardType)
{
    DictKey keyType = DictKey::DICT_KEY_UNKOWN;
    switch (keyboardType) {
        case KeyboardType::KBT_PINYIN_9:
        case KeyboardType::KBT_ZHUYIN_9:
        case KeyboardType::KBT_BIHUA_FANTI:
        case KeyboardType::KBT_BIHUA_JIANTI:
        case KeyboardType::KBT_CANGJIE_FAST:
            keyType = DictKey::DICT_KEY_2;
            break;
        case KeyboardType::KBT_PINYIN_26:
        case KeyboardType::KBT_ZHUYIN_26:
        case KeyboardType::KBT_CANGJIE:
            keyType = DictKey::DICT_KEY_1;
            break;
        default:
            break;
    }
    return keyType;
}

DictType SearchResultsMng::ParseDictType(KeyboardType keyboardType)
{
    DictType dictType = DictType::DICT_TYPE_UNKOWN;
    switch (keyboardType) {
        case KeyboardType::KBT_PINYIN_9:
        case KeyboardType::KBT_PINYIN_26:
            dictType = DictType::DICT_TYPE_PINYIN;
            break;
        case KeyboardType::KBT_ZHUYIN_9:
        case KeyboardType::KBT_ZHUYIN_26:
            dictType = DictType::DICT_TYPE_ZHUYIN;
            break;
        case KeyboardType::KBT_BIHUA_FANTI:
        case KeyboardType::KBT_BIHUA_JIANTI:
            dictType = DictType::DICT_TYPE_BIHUA;
            break;
        case KeyboardType::KBT_CANGJIE:
        case KeyboardType::KBT_CANGJIE_FAST:
            dictType = DictType::DICT_TYPE_CANGJIE;
            break;
        default:
            break;
    }
    return dictType;
}

char* SearchResultsMng::CopyNKeyStr(uint32 row, DictKey keyType, char* dstBuf, size_t maxLen, size_t count)
{
    char* retVal = CopyKeyStr(row, keyType, dstBuf, maxLen);

    size_t spsLen = strlen(m_searchKey);

    if (retVal == nullptr) {
        return dstBuf;
    }

    if (m_keyboardType == KeyboardType::KBT_ZHUYIN_9 || m_keyboardType == KeyboardType::KBT_PINYIN_9) {
        const char* key2Str = nullptr;
        key2Str = m_parseBinDict->GetKey2Str(row);

        const char* pSearchKey = m_searchKey;
        size_t matchCount = 0;
        while (*pSearchKey != '\0') {
            if (*pSearchKey != *key2Str) {
                break;
            } else {
                matchCount++;
                pSearchKey++;
                key2Str++;
            }
        }

        if (matchCount < maxLen) {
            dstBuf[matchCount] = '\0';
        } else {
            dstBuf[maxLen - 1] = '\0';
        }

        return dstBuf;
    } else if (m_keyboardType == KeyboardType::KBT_BIHUA_FANTI || m_keyboardType == KeyboardType::KBT_BIHUA_JIANTI) {
        if (spsLen >= maxLen) {
            if (memcpy_s(dstBuf, maxLen - 1, m_searchKey, maxLen - 1) == EOK) {
                dstBuf[maxLen - 1] = '\0';
            }
        } else {
            if (memcpy_s(dstBuf, spsLen, m_searchKey, spsLen) == EOK) {
                dstBuf[spsLen] = '\0';
            }
        }
    } else {
        CompareAndConcatenate(m_searchKey, dstBuf, maxLen);
    }

    return dstBuf;
}

void SearchResultsMng::ResetAllZhuyinTone()
{
    ResetAllZhuyinTone(m_zhuyinTone, sizeof(m_zhuyinTone));
}

void SearchResultsMng::SetZhuyinTone(size_t index, char tone)
{
    SetZhuyinTone(m_zhuyinTone, sizeof(m_zhuyinTone), index, tone);
}

char SearchResultsMng::GetZhuyinTone(size_t index)
{
    return GetZhuyinTone(m_zhuyinTone, sizeof(m_zhuyinTone), index);
}

size_t SearchResultsMng::GetToneNum()
{
    return GetToneNum(m_zhuyinTone, sizeof(m_zhuyinTone));
}

void SearchResultsMng::GetCurFilterPinyin(size_t pinyinId, char* dstFilterPinyin, size_t maxLen)
{
    char* leftPinyin = m_vecLastCandPinyin.at(pinyinId).m_pinyin;
    size_t curPreKeyLen = strlen(m_prefixKey);
    size_t leftPinyinLen = strlen(leftPinyin);

    if (curPreKeyLen > 0) {
        if (maxLen >= curPreKeyLen + leftPinyinLen + 1) {
            if (strncpy_s(dstFilterPinyin, maxLen, m_prefixKey, curPreKeyLen) == EOK) {
                dstFilterPinyin[curPreKeyLen] = '\0';
            }
            if (strncat_s(dstFilterPinyin, maxLen, leftPinyin, leftPinyinLen) != EOK) {
                return;
            }
        } else if (maxLen >= curPreKeyLen + 1) {
            if (strncpy_s(dstFilterPinyin, maxLen, m_prefixKey, curPreKeyLen) == EOK) {
                dstFilterPinyin[curPreKeyLen] = '\0';
            }

            if (maxLen > curPreKeyLen + 1 &&
                strncat_s(dstFilterPinyin, maxLen, leftPinyin, maxLen- curPreKeyLen - 1) != EOK) {
                return;
            }
        } else {
            if (strncpy_s(dstFilterPinyin, maxLen, m_prefixKey, maxLen - 1) == EOK) {
                dstFilterPinyin[maxLen - 1] = '\0';
            }
        }
    } else {
        if (maxLen >= leftPinyinLen + 1) {
            if (strncpy_s(dstFilterPinyin, maxLen, leftPinyin, leftPinyinLen) == EOK) {
                dstFilterPinyin[leftPinyinLen] = '\0';
            }
        } else {
            if (strncpy_s(dstFilterPinyin, maxLen, leftPinyin, maxLen - 1) == EOK) {
                dstFilterPinyin[maxLen - 1] = '\0';
            }
        }
    }
}

size_t SearchResultsMng::GetValidPrefixKeyCount()
{
    const char* pPreKey = m_prefixKey;
    size_t count = 0;
    while (*pPreKey != '\0') {
        if (*pPreKey == ' ') {
            pPreKey++;
            continue;
        }
        count++;
        pPreKey++;
    }

    return count;
}

void SearchResultsMng::UniqueResults()
{
    if (m_parseBinDict == nullptr) {
        LOGE("m_parseBinDict is nullptr!");

        return;
    }

    if (!m_vecMatchItems.empty()) {
        std::stable_sort(m_vecMatchItems.begin(), m_vecMatchItems.end(),
                         [](const MatchItem &lhs, const MatchItem &rhs) -> bool {
                             return lhs.m_matchType < rhs.m_matchType;
                         }); // 先按照匹配类型排序，防止去重的时候出bug

        InsertDeDuplicatedMatchItems(m_vecMatchItems);
    } else {
        // 如果结果为空，则用首字符匹配结果
        std::stable_sort(m_vecFirstKeyMatchItems.begin(), m_vecFirstKeyMatchItems.end(),
                         [](const MatchItem &lhs, const MatchItem &rhs) -> bool {
                             return lhs.m_matchType < rhs.m_matchType;
                         }); // 先按照匹配类型排序，防止去重的时候出bug

        InsertDeDuplicatedMatchItems(m_vecFirstKeyMatchItems);
    }
}

void SearchResultsMng::SortMatchItems(std::vector<MatchItem> &vecMatchItems)
{
    if (!vecMatchItems.empty()) {
        std::stable_sort(vecMatchItems.begin(), vecMatchItems.end(),
                         [](const MatchItem &lhs, const MatchItem &rhs) -> bool {
                             return lhs.m_score > rhs.m_score;
                         }); // 先按加权对词频分数由大到小排序，分数大的排前面

        DictKey keyType = ParseDictKeyType(m_keyboardType);
        std::stable_sort(vecMatchItems.begin(), vecMatchItems.end(),
            [keyType, this](const MatchItem &lhs, const MatchItem &rhs) -> bool {
                if (lhs.m_matchType != KeyMatchType::KMT_COMPLETED_MATCH &&
                rhs.m_matchType != KeyMatchType::KMT_COMPLETED_MATCH) {
                    char lhsKeyBuf[static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)] = {'\0'};
                    char rhsKeyBuf[static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)] = {'\0'};
                    this->CopyKeyStr(lhs.m_row, keyType, lhsKeyBuf, sizeof(lhsKeyBuf));
                    this->CopyKeyStr(rhs.m_row, keyType, rhsKeyBuf, sizeof(rhsKeyBuf));

                    size_t validLhsKeyLen = GetMaxMatchLen(this->m_searchKey, lhsKeyBuf);
                    size_t validRhsKeyLen = GetMaxMatchLen(this->m_searchKey, rhsKeyBuf);

                    return validLhsKeyLen > validRhsKeyLen;
                } else {
                    return lhs.m_matchType < rhs.m_matchType;
                }
        }); // 再按匹配类型排序，完全匹配的排前面，其次是部分匹配，再包含匹配，同时按照匹配字符的长度排序，匹配长的排前面
    }
}

void SearchResultsMng::ResetAllZhuyinTone(char* toneBuf, size_t maxLen)
{
    if (toneBuf == nullptr) {
        return;
    }

    memset_s(toneBuf, maxLen, '\0', maxLen);
}

void SearchResultsMng::SetZhuyinTone(char* toneBuf, size_t maxLen, size_t index, char tone)
{
    if (toneBuf == nullptr || index >= maxLen) {
        LOGW("index out of range or toneBuf is nullptr!");
        return;
    }

    toneBuf[index] = tone;
}

char SearchResultsMng::GetZhuyinTone(char* toneBuf, size_t maxLen, size_t index)
{
    if (toneBuf == nullptr || index >= maxLen) {
        LOGW("index out of range or toneBuf is nullptr!");
        return '\0';
    }

    return toneBuf[index];
}

size_t SearchResultsMng::GetToneNum(char* toneBuf, size_t maxLen)
{
    if (toneBuf == nullptr) {
        return 0;
    }

    return strlen(toneBuf);
}

bool SearchResultsMng::IsZhuyinTone(char tone)
{
    if (tone == 'U' || tone == 'W' || tone == 'V' || tone == 'X' || tone == 'Y') {
        return true;
    }

    return false;
}

bool SearchResultsMng::IsValidSearchKeys(char* spsBuf, KeyboardType keyboardType)
{
    std::map<char, char16> mapUtf16;
    bool isKeyType9 = false; // 键盘类型是否是9键
    if (keyboardType == KeyboardType::KBT_ZHUYIN_26) {
        mapUtf16 = KeyMapping::zhuyinMap;
    } else if (keyboardType == KeyboardType::KBT_BIHUA_FANTI || keyboardType == KeyboardType::KBT_BIHUA_JIANTI) {
        mapUtf16 = KeyMapping::bihuaMap;
    } else if (keyboardType == KeyboardType::KBT_CANGJIE || keyboardType == KeyboardType::KBT_CANGJIE_FAST ||
        keyboardType == KeyboardType::KBT_PINYIN_26) {
        mapUtf16 = KeyMapping::cangjieMap;
    } else if (keyboardType == KeyboardType::KBT_ZHUYIN_9 || keyboardType == KeyboardType::KBT_PINYIN_9) {
        isKeyType9 = true;
    }

    const char* pSps = spsBuf;
    while (*pSps != '\0') {
        if (isKeyType9) {
            if (!std::isdigit(*pSps)) {
                return false;
            }
        } else {
            auto iter = mapUtf16.find(*pSps);
            if (iter == mapUtf16.end()) {
                return false;
            }
        }
        pSps++;
    }

    return true;
}

void SearchResultsMng::ReallocMemory()
{
    std::vector<MatchItem>().swap(m_vecMatchItems);
    std::vector<MatchItem>().swap(m_vecFirstKeyMatchItems);
    std::vector<MatchItem>().swap(m_vecFilterItems);
    std::vector<MatchItem>().swap(m_vecUniqueItems);
    std::vector<PinyinItem>().swap(m_vecCandPinyin);
    std::set<uint32>().swap(m_setMatchRows);
    std::set<uint32>().swap(m_setFirstKeyMatchRows);

    m_vecMatchItems.reserve(RESULTS_RESERVE_CAPACITY);
    m_vecFilterItems.reserve(RESULTS_RESERVE_CAPACITY);
    m_vecCandPinyin.reserve(CANDIDATE_PINYIN_CAPACITY);
    m_vecUniqueItems.reserve(RESULTS_RESERVE_CAPACITY);
    m_vecFirstKeyMatchItems.reserve(RESULTS_RESERVE_CAPACITY);

    memset_s(m_searchKey, static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH),
             '\0', static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH));
    memset_s(m_prefixKey, static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH),
             '\0', static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH));
    m_isFiler = false;
    ResetAllZhuyinTone();
}

void SearchResultsMng::CompareAndConcatenate(const char* searchKey, char* keyBuf, size_t keyBufLen)
{
    const char* pSearchKey = searchKey;
    char* pKey = keyBuf;
    size_t keyIndex = 0;

    while (*pKey != '\0' && *pSearchKey != '\0') {
        if (*pKey == ' ') {
            if (IsZhuyinTone(*pSearchKey)) {
                *pKey = *pSearchKey; // 空格处如果是音调则用音调替换
                pSearchKey++;
            } else {
                *pKey = '\'';
            }

            pKey++;
            keyIndex++;
            continue;
        }

        if (m_keyboardType == KeyboardType::KBT_PINYIN_9 || m_keyboardType == KeyboardType::KBT_ZHUYIN_9) {
            if (!IsKey2MapToKey1(*pSearchKey, *pKey, m_keyboardType)) {
                break;
            }
        } else {
            if (*pKey != *pSearchKey) {
                break;
            }
        }

        pSearchKey++;
        pKey++;
        keyIndex++;
    }

    // 如果searchKey还没结束，则拼接到pKey后
    while (*pSearchKey != '\0') {
        if (keyIndex == keyBufLen - 1) {
            *pKey = '\0';
            keyIndex++;
            break;
        }

        if (m_keyboardType == KeyboardType::KBT_PINYIN_9 || m_keyboardType == KeyboardType::KBT_ZHUYIN_9) {
            *pKey = GetDefaultKey2ToKey1(*pSearchKey, m_keyboardType);
        } else {
            *pKey = *pSearchKey;
        }

        pKey++;
        pSearchKey++;
        keyIndex++;
    }

    *pKey = '\0';
}

bool SearchResultsMng::IsKey2MapToKey1(char key2, char key1, KeyboardType keyboardType)
{
    if (key2 >= '0' && key2 <= '9') {
        size_t key2Index = key2 - '0';
        Key9ColumnType* pKey9Column = nullptr;
        if (keyboardType == KeyboardType::KBT_PINYIN_9) {
            pKey9Column = pinyinKey2ToKey1 + key2Index;
        } else if (keyboardType == KeyboardType::KBT_ZHUYIN_9) {
            pKey9Column = zhuyinKey2ToKey1 + key2Index;
        }

        for (char* pKey = *pKey9Column; pKey != *pKey9Column + KEY9_COLUMN_CAPACITY; ++pKey) {
            if (*pKey == key1) {
                return true;
            }
        }
        return false;
    } else {
        return false;
    }

    return false;
}

char SearchResultsMng::GetDefaultKey2ToKey1(char key2, KeyboardType keyboardType)
{
    if (key2 >= '0' && key2 <= '9') {
        size_t key2Index = key2 - '0';
        Key9ColumnType* pKey9Column = nullptr;
        if (keyboardType == KeyboardType::KBT_PINYIN_9) {
            pKey9Column = pinyinKey2ToKey1 + key2Index;
        } else if (keyboardType == KeyboardType::KBT_ZHUYIN_9) {
            pKey9Column = zhuyinKey2ToKey1 + key2Index;
        }

        char* pKey = *pKey9Column; // 返回每列的首个字符
        return *pKey;
    } else {
        return '\0';
    }

    return '\0';
}

void SearchResultsMng::InsertDeDuplicatedMatchItems(const std::vector<MatchItem> &vecMatchItems)
{
    m_vecUniqueItems.clear();

    std::set<UniqueWordItem> setUniqueWordItems;
    UniqueWordItem uniqueWordItem;
    const char* pWord = nullptr;
    char16* pUniWord = nullptr;
    uint8 maxLen = sizeof(uniqueWordItem.m_wordItem);
    uint32 char16Size = sizeof(char16);

    for (auto &matchItem : vecMatchItems) {
        uint8 ziCount = m_parseBinDict->GetZiCount(matchItem.m_row);
        pWord = m_parseBinDict->GetWordStr(matchItem.m_row, matchItem.m_col);
        pUniWord = reinterpret_cast<char16*>(const_cast<char*>(pWord));

        if (ziCount < maxLen) {
            if (memcpy_s(uniqueWordItem.m_wordItem, maxLen, pUniWord, ziCount * char16Size) == EOK) {
                uniqueWordItem.m_wordItem[ziCount] = u'\0'; // 末尾加结束符
            }
        } else {
            if (memcpy_s(uniqueWordItem.m_wordItem, maxLen, pUniWord, (maxLen - 1) * char16Size) == EOK) {
                uniqueWordItem.m_wordItem[maxLen - 1] = u'\0'; // 末尾加结束符
            }
        }

        // 去重，如果没有重复的词，则加入到最终到结果中去
        if (setUniqueWordItems.find(uniqueWordItem) == setUniqueWordItems.end()) {
            setUniqueWordItems.insert(uniqueWordItem);
            m_vecUniqueItems.push_back(matchItem);
        }
    }
}

void SearchResultsMng::InsertCandidatePinyin(const std::set<uint32> &setMatchRows)
{
    size_t beginIndex = strlen(m_prefixKey);
    for (auto &row : setMatchRows) {
        PinyinItem pinyinItem;

        bool isOk = CopyOnePinyin(row, beginIndex, pinyinItem.m_pinyin, sizeof(pinyinItem.m_pinyin));
        if (isOk) {
            // 9键情况单独处理
            if (m_keyboardType == KeyboardType::KBT_PINYIN_9 || m_keyboardType == KeyboardType::KBT_ZHUYIN_9) {
                m_vecCandPinyin.push_back(pinyinItem);
            }
        }
    }
}

void SearchResultsMng::ProcessCandidatePinyin()
{
    // 去重
    std::sort(m_vecCandPinyin.begin(), m_vecCandPinyin.end(),
        [](const PinyinItem &lhs, const PinyinItem &rhs) -> bool {
        if (strcmp(lhs.m_pinyin, rhs.m_pinyin) < 0) {
            return true;
        }
        return false;
    });

    auto iter = std::unique(m_vecCandPinyin.begin(), m_vecCandPinyin.end(),
        [](const PinyinItem &lhs, const PinyinItem &rhs) -> bool {
        if (strcmp(lhs.m_pinyin, rhs.m_pinyin) == 0) {
            return true;
        }
        return false;
    });
    if (iter != m_vecCandPinyin.end()) {
        m_vecCandPinyin.erase(iter, m_vecCandPinyin.end());
    }

    size_t spsLen = strlen(m_searchKey);
    size_t maxValidSpsLen = 0;
    for (size_t i = 0; i < m_vecCandPinyin.size(); ++i) {
        size_t pinyinLen = strlen(m_vecCandPinyin.at(i).m_pinyin);
        if (pinyinLen > maxValidSpsLen && pinyinLen <= spsLen) {
            maxValidSpsLen = pinyinLen;
        }
    }

    for (auto iter = m_vecCandPinyin.begin(); iter != m_vecCandPinyin.end();) {
        if (strlen((*iter).m_pinyin) != maxValidSpsLen) {
            iter = m_vecCandPinyin.erase(iter);
        } else {
            ++iter;
        }
    }
}

void SearchResultsMng::FilterAndUniqueResults(const char *filterPinyin, size_t filterPinLen,
    const std::vector<MatchItem> &vecMatchItems)
{
    std::set<UniqueWordItem> uniqueWordItemsSet;
    UniqueWordItem uniqueWordItem;
    const char* wordPtr = nullptr;
    char16* uniWordPtr = nullptr;
    uint8 maxLen = sizeof(uniqueWordItem.m_wordItem);

    const char* key1Str = nullptr;
    uint32 char16Size = sizeof(char16);

    for (auto &matchItem : vecMatchItems) {
        key1Str = m_parseBinDict->GetKey1Str(matchItem.m_row);
        uint8 key1Len = m_parseBinDict->GetKey1Len(matchItem.m_row);

        bool isContain = ContainPinyin(key1Str, key1Len, filterPinyin, filterPinLen);
        if (isContain == false) {
            continue;
        }

        uint8 ziCount = m_parseBinDict->GetZiCount(matchItem.m_row);
        wordPtr = m_parseBinDict->GetWordStr(matchItem.m_row, matchItem.m_col);
        uniWordPtr = reinterpret_cast<char16*>(const_cast<char*>(wordPtr));

        if (ziCount < maxLen) {
            if (memcpy_s(uniqueWordItem.m_wordItem, maxLen, uniWordPtr, ziCount * char16Size) == EOK) {
                uniqueWordItem.m_wordItem[ziCount] = u'\0'; // 末尾加结束符
            }
        } else {
            if (memcpy_s(uniqueWordItem.m_wordItem, maxLen, uniWordPtr, (maxLen - 1) * char16Size) == EOK) {
                uniqueWordItem.m_wordItem[maxLen - 1] = u'\0'; // 末尾加结束符
            }
        }

        // 去重，如果没有重复的词，则加入到最终到结果中去
        if (uniqueWordItemsSet.find(uniqueWordItem) == uniqueWordItemsSet.end()) {
            uniqueWordItemsSet.insert(uniqueWordItem);
            m_vecFilterItems.push_back(matchItem);
        }
    }
}

size_t SearchResultsMng::GetMaxMatchLen(const char *searchKey, const char *keyBuf)
{
    size_t maxLen = 0;
    while (*searchKey != '\0' && *keyBuf != '\0') {
        if (*keyBuf == ' ') {
            keyBuf++;
            continue;
        }
        // 注音的音调需单独处理
        if (IsZhuyinTone(*searchKey)) {
            searchKey++;
            continue;
        }
        // 笔画的通配符也需单独处理
        if (m_keyboardType == KeyboardType::KBT_BIHUA_JIANTI || m_keyboardType == KeyboardType::KBT_BIHUA_FANTI) {
            if (*searchKey == '6') {
                searchKey++;
                continue;
            }
        }
        if (*searchKey != *keyBuf) {
            break;
        }

        maxLen++;
        searchKey++;
        keyBuf++;
    }
    return maxLen;
}

MatchItem SearchResultsMng::GetMatchItem(size_t index, bool& isOk)
{
    if (m_isFiler) {
        if (index >= m_vecFilterItems.size()) {
            isOk = false;
            LOGE("The index is out of range!");

            return MatchItem(0, 0, KeyMatchType::KMT_FIRST_KEY_MATCH, 0);
        }
        isOk = true;

        return m_vecFilterItems.at(index);
    } else {
        if (index >= m_vecUniqueItems.size()) {
            isOk = false;
            LOGE("The index is out of range!");

            return MatchItem(0, 0, KeyMatchType::KMT_FIRST_KEY_MATCH, 0);
        }
        isOk = true;

        return m_vecUniqueItems.at(index);
    }
}

DictKey SearchResultsMng::GetDictKeyType(KeyboardType keyboardType)
{
    if (keyboardType == KeyboardType::KBT_BIHUA_JIANTI || keyboardType == KeyboardType::KBT_BIHUA_FANTI ||
        keyboardType == KeyboardType::KBT_CANGJIE_FAST) {
        return DictKey::DICT_KEY_2;
    } else {
        return DictKey::DICT_KEY_1;
    }
}
END_NAMESPACE_CHINESE_IME