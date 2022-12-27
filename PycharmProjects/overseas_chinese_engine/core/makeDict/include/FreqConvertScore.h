/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: 将浮点数词频转化为评分
* Author: ZhouKang
* Create: 2021-7-12
*/
#ifndef MAKE_DICT_FREQCONVERTSCORE_H
#define MAKE_DICT_FREQCONVERTSCORE_H

#include <stdint.h>
#include "BasicTypeDef.h"

BEGIN_NAMESPACE_CHINESE_IME

/** 将浮点数词频转化为评分
 */
class FreqConvertScore {
private:
    FreqConvertScore() { };

public:
    /** 计算放大系数
     *
     * @param minFreq ：最小词频
     * @param maxFreq ：最大词频
     * @param rangeWidth ：可以设置为uint16_t能表示的最大值65535
     */
    static void
    CalcAmplifierCoeff(double minFreq, double maxFreq, size_t rangeWidth = 65535);

    /** 词频标准化
     *
     * @param freq : 词频（double类型）
     * @return ：评分
     */
    static size_t Standardization(double freq);

private:
    static inline void Init()
    {
        m_amplifierCoeff = 0.0;
        m_scoreOff = 0;
        m_rangeWidth = 0;
        m_freqOff = 0;
    }

    static double m_freqOff; // 当词频为负数时，需要偏移，避免log(freq)的参数为负数，导致异常
    static double m_amplifierCoeff;
    static double m_scoreOff; // log(freq)的结果可能为负数，提供偏移，使得评分在[0，m_rangeWidth] 之间
    static size_t m_rangeWidth;
};
END_NAMESPACE_CHINESE_IME

#endif //MAKE_DICT_FREQCONVERTSCORE_H
