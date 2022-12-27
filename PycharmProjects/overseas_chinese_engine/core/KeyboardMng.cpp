/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: KeyboardMng
* Author: Changjie Hu
* Create: 2021-7-12
*/

#include "Log.h"
#include "MakeBinDict.h"
#include "securec.h"
#include "securectype.h"
#include "testBatch/TestBatch.h"
#include "KeyboardMng.h"
#include "SearchResultsMng.h"

BEGIN_NAMESPACE_CHINESE_IME
KeyboardMng* KeyboardMng::GetInstance()
{
    static KeyboardMng keyboardMng;
    return &keyboardMng;
}

KeyboardMng::KeyboardMng() { }

KeyboardMng::~KeyboardMng() { }

bool KeyboardMng::InitDict(const char* sysDictPath, const char* predictDictPath, KeyboardType keyboardType)
{
    if (!m_keyboardTypeBase) {
        m_keyboardTypeBase = std::unique_ptr<KeyboardTypeBase>(new KeyboardTypeBase());
    }

    if (!m_nextWordDicTree) {
        m_nextWordDicTree = std::unique_ptr<NextWordDicTree>(new NextWordDicTree());
    }

    DictType dictType = SearchResultsMng::ParseDictType(keyboardType);
    if (dictType == m_curDictType && dictType == DictType::DICT_TYPE_CANGJIE) {
        if (keyboardType == m_curKeyboardType) {
            return true;
        } else {
            m_curKeyboardType = keyboardType;
            m_keyboardTypeBase->ModifyKeyboardType(keyboardType, sysDictPath);
            return true;
        };
    } else {
        m_curDictType = dictType;
        m_curKeyboardType = keyboardType;
        m_keyboardTypeBase->Close();
        m_nextWordDicTree->ClearDic();

        bool isOk = m_keyboardTypeBase->Init(sysDictPath, keyboardType) &&
            m_nextWordDicTree->LoadDic(predictDictPath);
        if (!isOk) {
            LOGW("Init dict failed!");
            m_curDictType = DictType::DICT_TYPE_UNKOWN;
            m_curKeyboardType = KeyboardType::KBT_UNKNOWN;
        }

        return isOk;
    }
}

void KeyboardMng::Close()
{
    if (m_keyboardTypeBase) {
        m_keyboardTypeBase->Close();
        m_keyboardTypeBase.reset();

        m_curDictType = DictType::DICT_TYPE_UNKOWN;
        m_curKeyboardType = KeyboardType::KBT_UNKNOWN;
    }

    if (m_nextWordDicTree) {
        m_nextWordDicTree->ClearDic();
        m_nextWordDicTree.reset();
    }
}

size_t KeyboardMng::Search(const char* spsBuf, size_t spsLen) const
{
    if (m_keyboardTypeBase) {
        return m_keyboardTypeBase->WordSearch(spsBuf, spsLen);
    }
    return 0;
}

char16* KeyboardMng::GetDisplayPinyin(char16* pinyinBuf, size_t pinyinLen) const
{
    if (m_keyboardTypeBase) {
        return m_keyboardTypeBase->GetDisplayPinyin(pinyinBuf, pinyinLen);
    }
    return nullptr;
}

size_t KeyboardMng::GetCandidatePinyinCount() const
{
    if (m_keyboardTypeBase) {
        return m_keyboardTypeBase->GetCandidatePinyinCount();
    }
    return 0;
}

char16* KeyboardMng::GetCandidatePinyin(size_t pinyinId, char16* pinyinBuf, size_t pinyinLen) const
{
    if (m_keyboardTypeBase) {
        return m_keyboardTypeBase->GetCandidatePinyin(pinyinId, pinyinBuf, pinyinLen);
    }
    return nullptr;
}

size_t KeyboardMng::ChooseCandidatePinyin(size_t pinyinId) const
{
    if (m_keyboardTypeBase) {
        return m_keyboardTypeBase->ChooseCandidatePinyin(pinyinId);
    }
    return 0;
}

void KeyboardMng::ResetSearch() const
{
    if (m_keyboardTypeBase) {
        m_keyboardTypeBase->ResetSearch();
    }
}

char16* KeyboardMng::GetCandidates(size_t candId, char16* candBuf, size_t maxLen) const
{
    if (m_keyboardTypeBase) {
        return m_keyboardTypeBase->GetCandidates(candId, candBuf, maxLen);
    }
    return nullptr;
}

size_t KeyboardMng::GetPredictsCount(const char16* hisBuf, size_t maxLen) const
{
    if (m_nextWordDicTree) {
        return m_nextWordDicTree->GetPredictWordCount(hisBuf, maxLen);
    }

    return 0;
}

char16* KeyboardMng::GetPredicts(size_t preId, char16* preBuf, size_t maxLen) const
{
    if (m_nextWordDicTree == nullptr) {
        return nullptr;
    }

    char16* predicts = nullptr;
    predicts = m_nextWordDicTree->GetPredictWord(preId);
    if (predicts == nullptr) {
        LOGW("The predicts is nullptr!");
        return nullptr;
    } else {
        size_t predictsCount = Utf16Strlen(predicts);
        if (predictsCount >= maxLen) {
            size_t copyLen = (maxLen - 1) * sizeof(char16);
            if (memcpy_s(preBuf, copyLen, predicts, copyLen) == EOK) {
                preBuf[maxLen - 1] = u'\0';
            }
        } else {
            size_t copyLen = predictsCount * sizeof(char16);
            size_t maxCopyLen = (maxLen - 1) * sizeof(char16);
            if (memcpy_s(preBuf, maxCopyLen, predicts, copyLen) == EOK) {
                preBuf[predictsCount] = u'\0';
            }
        }
    }

    return preBuf;
}
#ifdef ___BUILD_MODEL___
bool KeyboardMng::BuildDict(const char* rawDictPath, const char* outDictPath, KeyboardType keyboardType, bool isPredict)
{
    bool isOk = false;

    if (isPredict) {
        // 生成下一词联想词典
        NextWordDicTree nextWordDicTree;
        isOk = nextWordDicTree.BuildDic(rawDictPath, outDictPath);
    } else {
        // 生成系统词典
        MakeBinDict dict;
        const char* version = "2021.7.28_pinyin";
        std::u16string brief = u"kika first version";
        dict.SetDictVersionAndBrief(version, strlen(version), brief.c_str(), brief.length());
        DictType dictType = SearchResultsMng::ParseDictType(keyboardType);
        isOk = dict.MakeProcess(rawDictPath, outDictPath, dictType);
    }

    return isOk;
}
#endif  // ___BUILD_MODEL___

#ifdef OCE_DEBUG
void KeyboardMng::RunTestBatch(const char* inputPath, const char* outputPath, const char* sumPath, const char* dicPath,
    const char* nextWordDicPath, KeyboardType type, bool isPredict)
{
    using std::string;
    TestBatch t;
    t.TestBatchUniform(string(inputPath), string(outputPath), string(sumPath), string(dicPath),
                       string(nextWordDicPath), type, isPredict);
}
#endif  // OCE_DEBUG
END_NAMESPACE_CHINESE_IME