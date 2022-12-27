/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: OverseasChineseEngineIME
* Author: Changjie Hu
* Create: 2021-7-12
*/

#include "KeyboardMng.h"
#include "OverseasChineseEngineIME.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace ChineseIme;

bool ChineseIme::InitDict(const char* sysDictPath, const char* predictDictPath, ChineseIme::KeyboardType keyboardType)
{
    return KeyboardMng::GetInstance()->InitDict(sysDictPath, predictDictPath, keyboardType);
}

void ChineseIme::Close()
{
    KeyboardMng::GetInstance()->Close();
}

size_t ChineseIme::Search(const char* spsBuf, size_t spsLen)
{
    return KeyboardMng::GetInstance()->Search(spsBuf, spsLen);
}

char16* GetDisplayPinyin(char16* pinyinBuf, size_t pinyinLen)
{
    return KeyboardMng::GetInstance()->GetDisplayPinyin(pinyinBuf, pinyinLen);
}

size_t GetCandidatePinyinCount()
{
    return KeyboardMng::GetInstance()->GetCandidatePinyinCount();
}

char16* GetCandidatePinyin(size_t pinyinId, char16* pinyinBuf, size_t pinyinLen)
{
    return KeyboardMng::GetInstance()->GetCandidatePinyin(pinyinId, pinyinBuf, pinyinLen);
}

size_t ChooseCandidatePinyin(size_t pinyinId)
{
    return KeyboardMng::GetInstance()->ChooseCandidatePinyin(pinyinId);
}

char16* GetCandidate(size_t candId, char16* candBuf, size_t maxLen)
{
    return KeyboardMng::GetInstance()->GetCandidates(candId, candBuf, maxLen);
}

void ChineseIme::ResetSearch()
{
    KeyboardMng::GetInstance()->ResetSearch();
}

size_t GetPredictsCount(const char16* hisBuf, size_t maxLen)
{
    return KeyboardMng::GetInstance()->GetPredictsCount(hisBuf, maxLen);
}

char16* GetPredicts(size_t preId, char16* preBuf, size_t maxLen)
{
    return KeyboardMng::GetInstance()->GetPredicts(preId, preBuf, maxLen);
}

#ifdef ___BUILD_MODEL___
bool BuildDict(const char* rawDictPath, const char* outDictPath, KeyboardType keyboardType, bool isPredict)
{
    return KeyboardMng::GetInstance()->BuildDict(rawDictPath, outDictPath, keyboardType, isPredict);
}
#endif  // ___BUILD_MODEL___

#ifdef OCE_DEBUG
void RunTestBatch(const char* inputPath, const char* outputPath, const char* sumPath, const char* dicPath,
    const char* nextWordDicPath, KeyboardType type, bool isPredict)
{
    KeyboardMng::GetInstance()->RunTestBatch(inputPath, outputPath, sumPath, dicPath, nextWordDicPath, type, isPredict);
}
#endif // OCE_DEBUG

#ifdef __cplusplus
}
#endif