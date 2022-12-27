/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: KeyboardMng
* Author: Changjie Hu
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_KEYBOARDMNG_H
#define OVERSEASCHINESEENGINE_KEYBOARDMNG_H

#include <memory>
#include "common/include/BasicTypeDef.h"
#include "KeyboardTypeBase.h"
#include "predict/NextWordDicTree.h"

BEGIN_NAMESPACE_CHINESE_IME
class KeyboardMng {
public:
    static KeyboardMng* GetInstance();

    bool InitDict(const char* sysDictPath, const char* predictDictPath, KeyboardType keyboardType);

    void Close();

    size_t Search(const char* spsBuf, size_t spsLen) const;

    char16* GetDisplayPinyin(char16* pinyinBuf, size_t pinyinLen) const;

    size_t GetCandidatePinyinCount() const;

    char16* GetCandidatePinyin(size_t pinyinId, char16* pinyinBuf, size_t pinyinLen) const;

    size_t ChooseCandidatePinyin(size_t pinyinId) const;

    void ResetSearch() const;

    char16* GetCandidates(size_t candId, char16* candBuf, size_t maxLen) const;

    size_t GetPredictsCount(const char16* hisBuf, size_t maxLen) const;

    char16* GetPredicts(size_t preId, char16* preBuf, size_t maxLen) const;

#ifdef ___BUILD_MODEL___
    bool BuildDict(const char* rawDictPath, const char* outDictPath, KeyboardType keyboardType, bool isPredict);
#endif  // ___BUILD_MODEL___

#ifdef OCE_DEBUG
    void RunTestBatch(const char* inputPath, const char* outputPath, const char* sumPath, const char* dicPath,
        const char* nextWordDicPath, KeyboardType type, bool isPredict);
#endif  // OCE_DEBUG

private:
    KeyboardMng();

    ~KeyboardMng();

private:
    std::unique_ptr<KeyboardTypeBase> m_keyboardTypeBase; // 查词
    std::unique_ptr<NextWordDicTree> m_nextWordDicTree; // 下一词推荐

    DictType m_curDictType = DictType::DICT_TYPE_UNKOWN; // 当前词典类型
    KeyboardType m_curKeyboardType = KeyboardType::KBT_UNKNOWN; // 当前键盘类型
};
END_NAMESPACE_CHINESE_IME

#endif // OVERSEASCHINESEENGINE_KEYBOARDMNG_H
