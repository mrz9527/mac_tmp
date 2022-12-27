/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: char16
* Author: Changjie Hu
* Create: 2021-7-12
*/
#ifndef MAKE_DICT_UTF16CHAR_H
#define MAKE_DICT_UTF16CHAR_H

#include <stdlib.h>
#include <inttypes.h>
#include "BasicTypeDef.h"

BEGIN_NAMESPACE_CHINESE_IME
#ifdef __cplusplus
extern "C" {
#endif
float Utf16Atof(const char16* utf16Str);
size_t Utf16Strlen(const char16* utf16Str);

#ifdef __cplusplus
}
#endif
END_NAMESPACE_CHINESE_IME

#endif // MAKE_DICT_UTF16CHAR_H
