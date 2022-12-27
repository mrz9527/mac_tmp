/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: OverseasChineseEngineIME
* Author: Changjie Hu
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_OVERSEASCHINESEENGINEIME_H
#define OVERSEASCHINESEENGINE_OVERSEASCHINESEENGINEIME_H

#include <cstdlib>
#include "common/include/BasicTypeDef.h"

#ifdef __cplusplus
extern "C" {
#endif

BEGIN_NAMESPACE_CHINESE_IME
// 初始化词典
bool InitDict(const char* sysDictPath, const char* predictDictPath, KeyboardType keyboardType);

// 释放资源
void Close();

// 根据输入的键码串spsBuf进行查询，返回候选词的个数，返回0代表无候选项，查询失败
size_t Search(const char* spsBuf, size_t spsLen);

// 上屏时使用
char16* GetDisplayPinyin(char16* pinyinBuf, size_t pinyinLen);

// 获取候选拼音个数，九键时调用，用来显示候选拼音
size_t GetCandidatePinyinCount();

// 获取候选拼音，九键时调用，用来显示候选拼音
char16* GetCandidatePinyin(size_t pinyinId, char16* pinyinBuf, size_t pinyinLen);

// 选取候选拼音，九键时调用
size_t ChooseCandidatePinyin(size_t pinyinId);

// 重置，清空结果
void ResetSearch();

// candId为候选词id，candBuf保存候选词
char16* GetCandidate(size_t candId, char16* candBuf, size_t maxLen);

// 根据hisBuf的字符串推荐下一词，返回下一词推荐的个数
size_t GetPredictsCount(const char16* hisBuf, size_t maxLen);

// 根据下一词的id获取下一词
char16* GetPredicts(size_t preId, char16* preBuf, size_t maxLen);

#ifdef ___BUILD_MODEL___
// 生成词典
bool BuildDict(const char* rawDictPath, const char* outDictPath, KeyboardType keyboardType, bool isPredict);
#endif  // ___BUILD_MODEL___

#ifdef OCE_DEBUG
// 测试
void RunTestBatch(const char* inputPath, const char* outputPath, const char* sumPath, const char* dicPath,
    const char* nextWordDicPath, KeyboardType type, bool isPredict);

#endif // OCE_DEBUG

END_NAMESPACE_CHINESE_IME

#ifdef __cplusplus
}
#endif

#endif // OVERSEASCHINESEENGINE_OVERSEASCHINESEENGINEIME_H
