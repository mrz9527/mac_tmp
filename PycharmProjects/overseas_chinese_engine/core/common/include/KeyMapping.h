/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: log
* Author: ZhouKang
* Create: 2021-7-12
*/

#ifndef OVERSEASCHINESEENGINE_KEYMAPPING_H
#define OVERSEASCHINESEENGINE_KEYMAPPING_H

#include "BasicTypeDef.h"

BEGIN_NAMESPACE_CHINESE_IME

// 键码映射
class KeyMapping {
public:
    static const std::map<char, char16> bihuaMap;
    static const std::map<char, char16> cangjieMap;
    static const std::map<char, char16> zhuyinMap;

private:
    KeyMapping() = delete;
};
END_NAMESPACE_CHINESE_IME
#endif // OVERSEASCHINESEENGINE_KEYMAPPING_H
