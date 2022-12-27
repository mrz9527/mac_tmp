/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: 解析二进制词典
* Author: ZhouKang
* Create: 2021-7-12
*/
#include <algorithm>
#include "Log.h"
#include "ParseBinDict.h"

BEGIN_NAMESPACE_CHINESE_IME

ParseBinDict::~ParseBinDict()
{
    ReleaseResource();
}

bool ParseBinDict::Load(const char* binDictFile)
{
    ReleaseResource();
    m_hasLoad = false;

    if (!Read(binDictFile)) {
        LOGW("Read failed\n");
        return false;
    }

    if (!Parse()) {
        LOGW("Parse failed\n");
        return false;
    }

    m_hasLoad = true;

    return true;
}

bool ParseBinDict::Read(const char* binDictFile)
{
    FILE* fp = fopen(binDictFile, "rb");
    if (fp == nullptr) {
        LOGW("dictfile = %s", binDictFile);
        return false;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (fileSize <= static_cast<long>(DictHeadSize::DICT_HEADER_SIZE)) {
        fclose(fp);
        LOGW("fileSize <= DICT_HEADER_SIZE");
        return false;
    }

    char* dictBuf = (char*) malloc(fileSize);
    if (dictBuf == nullptr) {
        fclose(fp);
        LOGW("dictBuf == nullptr");
        return false;
    }

    fread(dictBuf, fileSize, 1, fp);
    fclose(fp);

    m_dictBufLen = fileSize;
    m_dictBuf = dictBuf;

    return true;
}

bool ParseBinDict::SetSortType(DictKey sortKey)
{
    if (!m_hasLoad) {
        return false;
    }

    if (sortKey == DictKey::DICT_KEY_UNKOWN) {
        return false;
    }

    m_sortKey = sortKey;

    SortWordItem();
    CalcDictItemIndex();

    return true;
}

bool ParseBinDict::Parse()
{
    const char* dictBuf = m_dictBuf;

    // 校验文件大小
    uint32 pos = static_cast<uint32>(DictHeadSize::DICT_HEADER_VERSION_SIZE);
    uint32 fileSize = *(uint32*) (dictBuf + pos);
    if (fileSize != m_dictBufLen) {
        LOGW("fileSize != m_dictBufLen");
        return false;
    }

    // wordItemCount数量
    pos = static_cast<uint32>(DictHeadSize::DICT_HEADER_VERSION_SIZE) +
        static_cast<uint32>(DictHeadSize::DICT_HEADER_FILESIZE_SIZE) +
        static_cast<uint32>(DictHeadSize::DICT_HEADER_BRIEF_SIZE);
    uint32 wordItemCount = *(uint32*) (dictBuf + pos);
    if (wordItemCount == 0) {
        return false;
    }
    m_vecDictItem.reserve(wordItemCount);

    // dictMemLayout内存布局
    pos += static_cast<uint32>(DictHeadSize::DICT_HEADER_WORDITEMCOUNT_SIZE);
    m_dictMemLayout = *(DictMemoryLayout*) (dictBuf + pos);
    if (HasErrorDictMemLayout()) {
        return false;
    }

    pos = static_cast<uint32>(DictHeadSize::DICT_HEADER_SIZE);
    dictBuf += pos;
    while (dictBuf < m_dictBuf + m_dictBufLen) {
        uint8 ziCount = GetZiCount(dictBuf);
        uint8 key1LenEx = m_dictMemLayout.key1StrOcc > 0 ? (GetKey1Len(dictBuf) + ziCount) : 0;
        uint8 key2LenEx = m_dictMemLayout.key2StrOcc > 0 ? (GetKey2Len(dictBuf) + ziCount) : 0;
        m_vecDictItem.emplace_back(dictBuf, GetKey1Str(dictBuf), key1LenEx,
                                        GetKey2Str(dictBuf), key2LenEx);
        dictBuf = NextDictItem(dictBuf);
    }

    return true;
}

bool ParseBinDict::Search(char ch, std::pair<uint32, uint32> &range) const
{
    bool findFlag = false;
    for (uint32 i = 0; i < m_vecDictItemIndex.size(); ++i) {
        if (ch == m_vecDictItemIndex[i].first) {
            range.first = m_vecDictItemIndex[i].second;
            if (i != m_vecDictItemIndex.size() - 1) {
                range.second = m_vecDictItemIndex[i + 1].second;
            } else {
                range.second = m_vecDictItem.size() - 1;
            }
            findFlag = true;
            break;
        }
    }
    return findFlag;
}

uint8 ParseBinDict::GetZiCount(const char* wordItem) const
{
    if (wordItem == nullptr) {
        LOGW("wordItem == nullptr");
        return 0;
    }

    return *(uint8*) wordItem;
}

uint8 ParseBinDict::GetKey1Len(const char* wordItem) const
{
    if (wordItem == nullptr) {
        LOGW("wordItem is nullptr");
        return 0;
    }

    const char* p = wordItem + m_dictMemLayout.ziCountOcc;
    return *(uint8*) p;
}

uint8 ParseBinDict::GetKey2Len(const char* wordItem) const
{
    if (wordItem == nullptr) {
        LOGW("wordItem is nullptr");
        return 0;
    }

    const char* p = wordItem + m_dictMemLayout.ziCountOcc;
    if (m_dictMemLayout.key2LenOcc > 0) {
        p += m_dictMemLayout.key1LenOcc;
    }

    return *(uint8*) p;
}

const char* ParseBinDict::GetKey1Str(const char* wordItem) const
{
    if (wordItem == nullptr) {
        LOGW("wordItem == nullptr");
        return nullptr;
    }

    if (m_dictMemLayout.key1StrOcc == 0)
        return nullptr;

    return wordItem + m_dictMemLayout.ziCountOcc + m_dictMemLayout.key1LenOcc +
           m_dictMemLayout.key2LenOcc;
}

const char* ParseBinDict::GetKey2Str(const char* wordItem) const
{
    if (wordItem == nullptr) {
        LOGW("wordItem == nullptr");
        return nullptr;
    }

    if (m_dictMemLayout.key2StrOcc == 0)
        return nullptr;

    const char* p = wordItem + m_dictMemLayout.ziCountOcc + m_dictMemLayout.key1LenOcc +
                    m_dictMemLayout.key2LenOcc;
    if (m_dictMemLayout.key1StrOcc > 0) {
        p += GetKey1Len(wordItem) + GetZiCount(wordItem);
    }
    return p;
}

uint16 ParseBinDict::GetWordCount(const char* wordItem) const
{
    const char* p = GetWordCountAddress(wordItem);
    if (p == nullptr) {
        return 0;
    }

    return *(uint16*) p;
}

const char* ParseBinDict::GetWordCountAddress(const char* wordItem) const
{
    if (wordItem == nullptr) {
        LOGW("wordItem == nullptr");
        return nullptr;
    }

    const char* p = wordItem + m_dictMemLayout.ziCountOcc + m_dictMemLayout.key1LenOcc +
                    m_dictMemLayout.key2LenOcc;
    uint8 ziCount = GetZiCount(wordItem);

    if (m_dictMemLayout.key1StrOcc > 0) {
        p += GetKey1Len(wordItem) + ziCount;
    }
    if (m_dictMemLayout.key2StrOcc > 0) {
        p += GetKey2Len(wordItem) + ziCount;
    }
    return p;
}

const char* ParseBinDict::GetWordStr(const char* wordItem, uint16 wordIndex) const
{
    if (wordItem == nullptr) {
        LOGW("wordItem == nullptr");
        return nullptr;
    }

    if (wordIndex >= GetWordCount(wordItem)) {
        LOGW("wordIndex>=GetWordCount(wordItem)");
        return nullptr;
    }

    const char* p = GetWordCountAddress(wordItem);
    if (p == nullptr) {
        return nullptr;
    }

    uint8 ziCount = GetZiCount(wordItem);

    p += m_dictMemLayout.wordCountOcc +
        wordIndex * (ziCount * (m_dictMemLayout.wordStrOcc + m_dictMemLayout.toneStrOcc) + m_dictMemLayout.freqOcc);

    return p;
}

const char* ParseBinDict::GetToneStr(const char* wordItem, uint16 wordIndex) const
{
    if (wordItem == nullptr) {
        LOGW("wordItem == nullptr");
        return nullptr;
    }

    if (m_dictMemLayout.toneStrOcc <= 0) {
        LOGW("m_dictMemLayout.toneStrOcc <= 0");
        return nullptr;
    }

    if (wordIndex >= GetWordCount(wordItem)) {
        LOGW("wordIndex < GetWordCount(wordItem)");
        return nullptr;
    }

    const char* p = GetWordCountAddress(wordItem);
    if (p == nullptr) {
        return nullptr;
    }
    uint8 ziCount = GetZiCount(wordItem);
    p += m_dictMemLayout.wordCountOcc + ziCount * m_dictMemLayout.wordStrOcc +
        wordIndex * (ziCount * (m_dictMemLayout.wordStrOcc + m_dictMemLayout.toneStrOcc) + m_dictMemLayout.freqOcc);

    return p;
}

uint16 ParseBinDict::GetFreq(const char* wordItem, uint16 wordIndex) const
{
    if (wordItem == nullptr) {
        LOGW("wordItem == nullptr");
        return 0;
    }

    if (wordIndex >= GetWordCount(wordItem)) {
        LOGW("wordIndex < GetWordCount(wordItem)");
        return 0;
    }

    const char* p = GetWordCountAddress(wordItem);
    if (p == nullptr) {
        return 0;
    }

    uint8 ziCount = GetZiCount(wordItem);
    p += m_dictMemLayout.wordCountOcc - m_dictMemLayout.freqOcc + (wordIndex + 1) *
        (ziCount * (m_dictMemLayout.wordStrOcc + m_dictMemLayout.toneStrOcc) + m_dictMemLayout.freqOcc);

    return *(uint16*) p;
}

const char* ParseBinDict::NextDictItem(const char* wordItem) const
{
    uint32 ziCount = GetZiCount(wordItem);
    uint32 key1Len = GetKey1Len(wordItem);
    uint32 key2Len = GetKey2Len(wordItem);
    uint32 wordCount = GetWordCount(wordItem);

    const char* p = wordItem;
    p += m_dictMemLayout.ziCountOcc + m_dictMemLayout.key1LenOcc + m_dictMemLayout.key2LenOcc +
         m_dictMemLayout.key1StrOcc * (key1Len + ziCount) +
         m_dictMemLayout.key2StrOcc * (key2Len + ziCount) +
         m_dictMemLayout.wordCountOcc + wordCount *
        ((m_dictMemLayout.wordStrOcc + m_dictMemLayout.toneStrOcc) * ziCount + m_dictMemLayout.freqOcc);

    return p;
}


uint8 ParseBinDict::GetZiCount(uint32 itemIndex) const
{
    if (itemIndex >= m_vecDictItem.size()) {
        LOGW("itemIndex>=m_vecDictItem.size()");
        return 0;
    }

    return GetZiCount(m_vecDictItem[itemIndex].wordItem);
}

uint8 ParseBinDict::GetKey1Len(uint32 itemIndex) const
{
    if (itemIndex >= m_vecDictItem.size()) {
        LOGW("itemIndex>=m_vecDictItem.size()");
        return 0;
    }

    return GetKey1Len(m_vecDictItem[itemIndex].wordItem);
}

uint8 ParseBinDict::GetKey2Len(uint32 itemIndex) const
{
    if (itemIndex >= m_vecDictItem.size()) {
        LOGW("itemIndex>=m_vecDictItem.size()");
        return 0;
    }

    return GetKey2Len(m_vecDictItem[itemIndex].wordItem);
}

const char* ParseBinDict::GetKey1Str(uint32 itemIndex) const
{
    if (itemIndex >= m_vecDictItem.size()) {
        LOGW("itemIndex>=m_vecDictItem.size()");
        return nullptr;
    }

    return GetKey1Str(m_vecDictItem[itemIndex].wordItem);
}

const char* ParseBinDict::GetKey2Str(uint32 itemIndex) const
{
    if (itemIndex >= m_vecDictItem.size()) {
        LOGW("itemIndex>=m_vecDictItem.size()");
        return 0;
    }

    return GetKey2Str(m_vecDictItem[itemIndex].wordItem);
}

uint16 ParseBinDict::GetWordCount(uint32 itemIndex) const
{
    if (itemIndex >= m_vecDictItem.size()) {
        LOGW("itemIndex>=m_vecDictItem.size()");
        return 0;
    }

    return GetWordCount(m_vecDictItem[itemIndex].wordItem);
}

const char* ParseBinDict::GetWordStr(uint32 itemIndex, uint16 wordIndex) const
{
    if (itemIndex >= m_vecDictItem.size()) {
        LOGW("itemIndex>=m_vecDictItem.size()");
        return 0;
    }

    return GetWordStr(m_vecDictItem[itemIndex].wordItem, wordIndex);
}

const char* ParseBinDict::GetToneStr(uint32 itemIndex, uint16 wordIndex) const
{
    if (itemIndex >= m_vecDictItem.size()) {
        LOGW("itemIndex>=m_vecDictItem.size()");
        return 0;
    }

    return GetToneStr(m_vecDictItem[itemIndex].wordItem, wordIndex);
}

uint16 ParseBinDict::GetFreq(uint32 itemIndex, uint16 wordIndex) const
{
    if (itemIndex >= m_vecDictItem.size()) {
        LOGW("itemIndex>=m_vecDictItem.size()");
        return 0;
    }

    return GetFreq(m_vecDictItem[itemIndex].wordItem, wordIndex);
}

void ParseBinDict::SortWordItem()
{
    if (m_sortKey == DictKey::DICT_KEY_1)
        std::sort(m_vecDictItem.begin(), m_vecDictItem.end(),
                  DictItem::CompareByKey1);
    else if (m_sortKey == DictKey::DICT_KEY_2)
        std::sort(m_vecDictItem.begin(), m_vecDictItem.end(),
                  DictItem::CompareByKey2);
}

void ParseBinDict::CalcDictItemIndex()
{
    std::vector<std::pair<char, uint32>>().swap(m_vecDictItemIndex);
    char preKeyFirstCh;
    char curKeyFirstCh;

    uint32 index = 0;
    if (m_sortKey == DictKey::DICT_KEY_1) {
        for (const auto &elem:m_vecDictItem) {
            curKeyFirstCh = elem.key1Str[0];
            CalcDictItemIndexSub(index, preKeyFirstCh, curKeyFirstCh);
            preKeyFirstCh = curKeyFirstCh;
            ++index;
        }
    } else if (m_sortKey == DictKey::DICT_KEY_2) {
        for (const auto &elem:m_vecDictItem) {
            curKeyFirstCh = elem.key2Str[0];
            CalcDictItemIndexSub(index, preKeyFirstCh, curKeyFirstCh);
            preKeyFirstCh = curKeyFirstCh;
            ++index;
        }
    }
}

void ParseBinDict::CalcDictItemIndexSub(uint32 index, char preKeyFirstCh, char curKeyFirstCh)
{
    if (index == 0) {
        m_vecDictItemIndex.emplace_back(std::make_pair(curKeyFirstCh, index));
    } else {
        if (curKeyFirstCh != preKeyFirstCh) {
            m_vecDictItemIndex.emplace_back(std::make_pair(curKeyFirstCh, index));
        }
    }
}

void ParseBinDict::ReleaseResource()
{
    if (m_dictBuf != nullptr) {
        free((void*) m_dictBuf);
        m_dictBuf = nullptr;
    }

    m_dictBufLen = 0;
    m_hasLoad = false;
    m_sortKey = DictKey::DICT_KEY_UNKOWN;
    m_dictMemLayout.Init();

    std::vector<std::pair<char, uint32>>().swap(m_vecDictItemIndex);
    std::vector<DictItem>().swap(m_vecDictItem);
}

bool ParseBinDict::HasErrorDictMemLayout() const
{
    if (m_dictMemLayout.key1StrOcc <= 0 && m_dictMemLayout.key2StrOcc <= 0) {
        LOGW("m_dictMemLayout.key1StrOcc <= 0 && m_dictMemLayout.key2StrOcc <= 0");
        return true;
    }

    if (m_dictMemLayout.key1LenOcc <= 0 && m_dictMemLayout.key2LenOcc <= 0) {
        LOGW("m_dictMemLayout.key1StrOcc <= 0 && m_dictMemLayout.key2StrOcc <= 0");
        return true;
    }

    if (m_dictMemLayout.ziCountOcc <= 0) {
        LOGW("m_dictMemLayout.ziCountOcc <= 0");
        return true;
    }

    if (m_dictMemLayout.wordStrOcc <= 0) {
        LOGW("m_dictMemLayout.wordStrOcc<=0");
        return true;
    }

    if (m_dictMemLayout.wordCountOcc <= 0) {
        LOGW("m_dictMemLayout.wordCountOcc<=0");
        return true;
    }

    if (m_dictMemLayout.freqOcc <= 0) {
        LOGW("m_dictMemLayout.freqOcc <= 0");
        return true;
    }

    return false;
}

END_NAMESPACE_CHINESE_IME