/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: 下一词预测
* Author: ZhouKang
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_PREDICTPROCESS_H
#define OVERSEASCHINESEENGINE_PREDICTPROCESS_H

#include "BasicTypeDef.h"
#include "Predict.h"
#include "Utf16Char.h"

BEGIN_NAMESPACE_CHINESE_IME
/* 内部排序规则：
     前缀越长，相应的后缀排序越靠前
     同一前缀，相应后缀的词频越大越靠前
*/
enum {
    MAX_PREDICT_COUNT = 128;
};

class PredictProcess {
public:
    PredictProcess() = default;

    bool Load(const char* dictFile);

    void Close();

    size_t GetPredictWordCount(const char16* inputString, size_t len);

    char16* GetPredictWord(size_t id);

private:

    void inline ClearPredict()
    {
        PredictProcess::m_count = 0;
    };

private:
    Predict<char16, uint16> m_predict;

    static char16 m_predictWords[MAX_PREDICT_COUNT][static_cast<unsigned>(ArrayMaxLength::MAX_PREDICT_SIZE)];
    static size_t m_count;
};

END_NAMESPACE_CHINESE_IME

#endif //OVERSEASCHINESEENGINE_PREDICTPROCESS_H
