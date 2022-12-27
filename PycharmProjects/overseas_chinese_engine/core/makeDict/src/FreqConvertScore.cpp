/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: 将浮点数词频转化为评分
* Author: ZhouKang
* Create: 2021-7-12
*/
#include "FreqConvertScore.h"
#include <math.h>

BEGIN_NAMESPACE_CHINESE_IME

double FreqConvertScore::m_amplifierCoeff = 0.0;
double FreqConvertScore::m_scoreOff = 0;
size_t FreqConvertScore::m_rangeWidth = 0;
double FreqConvertScore::m_freqOff = 0;

void FreqConvertScore::CalcAmplifierCoeff(double minFreq, double maxFreq, size_t rangeWidth)
{
    if (rangeWidth <= 0) {
        rangeWidth = 65535;
    }
    Init();

    m_rangeWidth = rangeWidth;
    if (minFreq < 0.0) {
        m_freqOff = minFreq - 1e-9;
        minFreq -= m_freqOff; // 做偏移
        maxFreq -= m_freqOff;
    }

    double max_log_freq = log(maxFreq);
    double min_log_freq = log(minFreq); // 此时min_log_freq = 1

    double delta = max_log_freq - min_log_freq;
    if (fabs(delta / min_log_freq) < 1e-9) {
        m_amplifierCoeff = 1.0;
    } else {
        m_amplifierCoeff = 1.0 * rangeWidth / (delta);
    }
    m_scoreOff = min_log_freq * m_amplifierCoeff;
}

size_t FreqConvertScore::Standardization(double freq)
{
    int32_t score = (int32_t)(log(freq - m_freqOff) * m_amplifierCoeff - m_scoreOff);

    if (score < 0) {
        score = 0;
    }
    if (score > m_rangeWidth) {
        score = m_rangeWidth;
    }
    return (size_t) score;
}
END_NAMESPACE_CHINESE_IME