/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: 下一词预测
* Author: ZhouKang
* Create: 2021-7-12
*/
#include "PredictProcess.h"
#include <string.h>
#include "securecutil.h"

BEGIN_NAMESPACE_CHINESE_IME

char16 PredictProcess::m_predictWords[MAX_PREDICT_COUNT][static_cast<unsigned>(ArrayMaxLength::MAX_PREDICT_SIZE)] = {0};
size_t PredictProcess::m_count = 0;

bool PredictProcess::Load(const char* dictFile)
{
    return m_predict.Load(dictFile);
}

size_t PredictProcess::GetPredictWordCount(const char16* inputString, size_t len)
{
    ClearPredict();

    if (len == 0) {
        return 0;
    }

    std::vector<std::pair<std::u16string, uint16>> result;
    result.reserve(MAX_PREDICT_COUNT);

    size_t startPos = (len > static_cast<unsigned>(ArrayMaxLength::MAX_PREDICT_SIZE)) ?
            (len - static_cast<unsigned>(ArrayMaxLength::MAX_PREDICT_SIZE)) : 0;
    const char16* prefixStr = inputString + startPos;
    for (size_t i = startPos; i < len; ++i) {
        std::vector<std::pair<std::u16string, uint16>> tmpResult;
        tmpResult.reserve(MAX_PREDICT_COUNT);

        m_predict.GetPredict(prefixStr, tmpResult);
        result.insert(result.end(), tmpResult.begin(), tmpResult.end());
        if (result.size() >= MAX_PREDICT_COUNT) {
            break;
        }
        ++prefixStr;
    }

    result.shrink_to_fit();
    size_t wordCount = std::min(result.size(), (size_t) MAX_PREDICT_COUNT);
    for (int i = 0; i < wordCount; ++i) {
        std::u16string &word = result[i].first;
        std::u16string::size_type ziCount = word.size();
        size_t bytes = ziCount * sizeof(char16);
        memcpy_s((void*)PredictProcess::m_predictWords[i], bytes, (void*)word.data(), bytes);
        PredictProcess::m_predictWords[i][ziCount] = 0;
    }

    PredictProcess::m_count = wordCount;
    return PredictProcess::m_count;
}

char16* PredictProcess::GetPredictWord(size_t id)
{
    if (id >= m_count) {
        return nullptr;
    }

    return PredictProcess::m_predictWords[id];
}

void PredictProcess::Close()
{
    m_predict.ReleaseResource();
}

END_NAMESPACE_CHINESE_IME
