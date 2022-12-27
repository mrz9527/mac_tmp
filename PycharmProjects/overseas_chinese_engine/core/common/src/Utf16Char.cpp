/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: utf16
* Author: ZhouKang
* Create: 2021-7-12
*/
#include <cstdlib>
#include "Utils.h"
#include "Utf16Char.h"

BEGIN_NAMESPACE_CHINESE_IME

#ifdef __cplusplus
extern "C" {
#endif

float Utf16Atof(const char16* utf16Str)
{
    if (utf16Str == nullptr) {
        return .0f;
    }

    std::string str = Utils::Utf16ToUtf8(std::u16string(utf16Str));
    return atof(str.c_str());
}

size_t Utf16Strlen(const char16* utf16Str)
{
    if (utf16Str == nullptr) {
        return 0;
    }
    size_t size = 0;
    while ((char16) '\0' != utf16Str[size]) {
        size++;
    }
    return size;
}

#ifdef __cplusplus
}
#endif
END_NAMESPACE_CHINESE_IME
