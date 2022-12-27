/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: SearchResultsMng
* Author: Changjie Hu
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_SEARCHRESULTSMNG_H
#define OVERSEASCHINESEENGINE_SEARCHRESULTSMNG_H

#include <vector>
#include <set>
#include <memory>
#include "common/include/BasicTypeDef.h"
#include "parseDict/include/ParseBinDict.h"
#include "securec.h"
#include "securectype.h"


BEGIN_NAMESPACE_CHINESE_IME

// 保存匹配的结果
struct MatchItem {
    uint32 m_row;               // 匹配到的行
    uint16 m_col;               // 匹配到的列
    KeyMatchType m_matchType;   // 匹配上的类型
    uint32 m_score;             // 加权后的词频

    MatchItem(uint32 row, uint16 col, KeyMatchType matchType, uint32 score)
        :m_row(row), m_col(col), m_matchType(matchType), m_score(score)
    { }
};

// 保存左侧拼音
struct PinyinItem {
    char m_pinyin[8];           // 保存左侧的拼音

    PinyinItem()
    {
        memset_s(m_pinyin, sizeof(m_pinyin), '\0', sizeof(m_pinyin));
    }
};

// 保存词，去重时使用
struct UniqueWordItem {
    char16 m_wordItem[8];       // 保存非重复的词

    UniqueWordItem()
    {
        memset_s(m_wordItem, sizeof(m_wordItem), '\0', sizeof(m_wordItem));
    }

    bool CompareWordItem(const UniqueWordItem &right, size_t index) const;

    bool operator<(const UniqueWordItem &right) const;
};

class SearchResultsMng {
public:
    SearchResultsMng();

    ~SearchResultsMng();

    void Init(std::shared_ptr<ParseBinDict> parseBinDict);

    void SetKeyboardType(KeyboardType keyboardType);

    void ReleaseMemory();

    void ReallocMemory();

    void ClearResults(); // 清空结果，但不释放内存
    size_t GetMatchResultCount(); // 获取匹配到的结果
    void InsertMatchResult(uint32 row, uint16 col, KeyMatchType matchType, uint32 score);

    char16* CopyResult(uint16 index, char16* dst, size_t maxLen); // 将指定index结果拷贝到指定内存
    char* CopyKeyStr(uint32 row, DictKey keyType, char* dstBuf, size_t maxLen);

    void SortResults();

    void UniqueResults();

    size_t GetCandidatePinyinCount();

    char16* GetCandidatePinyin(size_t pinyinId, char16* pinyinBuf, size_t pinyinLen);

    size_t ChooseCandidatePinyin(size_t pinyinId);

    void SetSearchKey(const char* key, size_t len);

    void ConvertKeyToUtf16Pinyin(const char* key, size_t keyLen,
        char16* pinyinBuf, size_t pinyinLen, KeyboardType keyboardType);

    char16* GetDisplayPinyin(char16* pinyinBuf, size_t pinyinLen);

    void ResetAllZhuyinTone(); // 重置所有的注音音调为'\0'
    void SetZhuyinTone(size_t index, char tone); // 设置某位置的注音音调
    char GetZhuyinTone(size_t index); // 获取某位置的注音音调，index不合法则为'\0'
    size_t GetToneNum(); // 获取有效音调都个数

    static char16 ConvertCharToUtf16(char key, KeyboardType keyboardType); // 对笔画、仓颉、注音搜索键码转换成上屏对Unicode码
    static DictKey ParseDictKeyType(KeyboardType keyboardType);

    static DictType ParseDictType(KeyboardType keyboardType);

    static bool IsZhuyinTone(char tone); // tone是否是注音音调
    // 判断指定的键码串是否相对于键盘类型来说是否是合法的
    static bool IsValidSearchKeys(char* spsBuf, KeyboardType keyboardType);

private:
    bool CopyOnePinyin(uint32 row, size_t beginKey, char* dst, size_t dstLen);

    bool ContainPinyin(const char* keyPinyin, size_t keyLen, const char* tarPinyin, size_t tarLen);

    // 根据键的类型拷贝前count个键码串
    char* CopyNKeyStr(uint32 row, DictKey keyType, char* dstBuf, size_t maxLen, size_t count);

    // 根据已选的前缀拼音加上当前左侧拼音得到过滤的拼音串
    void GetCurFilterPinyin(size_t pinyinId, char* dstFilterPinyin, size_t maxLen);

    size_t GetValidPrefixKeyCount(); // 获取有效的前缀key的字符个数，即不统计空格
    void FilterAndUniqueResults(const char* filterPinyin, size_t filterPinLen);
    void FilterAndUniqueResults(const char* filterPinyin, size_t filterPinLen,
        const std::vector<MatchItem> &vecMatchItems);

    void SortMatchItems(std::vector<MatchItem> &vecMatchItems);

    void SortFilterResults();

    void InsertDeDuplicatedMatchItems(const std::vector<MatchItem> &vecMatchItems); // 将匹配的中间结果插入到去重结果的容器中

    void InsertCandidatePinyin(const std::set<uint32> &setMatchRows);

    void ResetAllZhuyinTone(char* toneBuf, size_t maxLen); // 重置toneBuf的注音音调为'\0'
    void SetZhuyinTone(char* toneBuf, size_t maxLen, size_t index, char tone); // 设置toneBuf某位置的注音音调
    char GetZhuyinTone(char* toneBuf, size_t maxLen, size_t index); // 获取toneBuf某位置的注音音调，index不合法则为'\0'
    size_t GetToneNum(char* toneBuf, size_t maxLen); // 获取toneBuf有效音调都个数
    // 对搜索的键码串与出词的键码串对比并拼接，用作给上屏的键码串使用
    void CompareAndConcatenate(const char* searchKey, char* keyBuf, size_t keyBufLen);
    // 对于9键的键码串字符判断是否是对应的26键字符，比方说拼音9键的2对应a,b,c
    bool IsKey2MapToKey1(char key2, char key1, KeyboardType keyboardType);
    char GetDefaultKey2ToKey1(char key2, KeyboardType keyboardType);

    void ProcessCandidatePinyin(); // 对左侧拼音列表进行去重排序等处理

    size_t GetMaxMatchLen(const char* searchKey, const char* keyBuf);

    MatchItem GetMatchItem(size_t index, bool& isOk);

    DictKey GetDictKeyType(KeyboardType keyboardType);

    static char16 FindUtf16ByChar(const std::map<char, char16>& mapUtf16, char key);

private:
    KeyboardType m_keyboardType = KeyboardType::KBT_UNKNOWN; // 当前的键盘类型
    std::vector<MatchItem> m_vecMatchItems; // 搜索键码串匹配后的结果，作为中间结果
    std::vector<MatchItem> m_vecFirstKeyMatchItems; // 搜索键码串首字符匹配的结果，如果m_vecMatchItems为空，则用此结果，保证出词
    std::vector<MatchItem> m_vecFilterItems; // 保存过滤后的结果，如9键类型时选择左侧拼音列表过滤及注音音调过滤及去重后的结果
    std::vector<MatchItem> m_vecUniqueItems; // 保存对原始匹配结果去重后的结果
    std::vector<PinyinItem> m_vecCandPinyin; // 保存左侧的候选拼音，9键使用
    std::vector<PinyinItem> m_vecLastCandPinyin; // 保存最后一次可以匹配上的候选拼音，9键使用
    std::set<uint32> m_setMatchRows; // 保存匹配的行索引，给后续9键左侧拼音列表使用
    std::set<uint32> m_setFirstKeyMatchRows; // 保存首字符匹配的行索引
    bool m_isFiler = false; // 是否过滤
    char m_searchKey[static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)]; // 保存当前搜索的键码串
    char m_prefixKey[static_cast<unsigned>(ArrayMaxLength::MAX_KEY_LENGTH)]; // 保存当前已选的前缀
    char m_zhuyinTone[static_cast<unsigned>(ArrayMaxLength::MAX_TONE_LENGTH)]; // 保存当前搜索的键码串中的音调，只注音26键有效

    std::shared_ptr<ParseBinDict> m_parseBinDict; // 词典解析指针，由创建者销毁
};

END_NAMESPACE_CHINESE_IME
#endif // OVERSEASCHINESEENGINE_SEARCHRESULTSMNG_H
