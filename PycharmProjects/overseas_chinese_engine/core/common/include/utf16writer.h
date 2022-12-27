/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: char16 writer
* Author: Changjie Hu
* Create: 2021-7-12
*/
#ifndef MAKE_DICT_UTF16WRITER_H
#define MAKE_DICT_UTF16WRITER_H

#include "BasicTypeDef.h"

BEGIN_NAMESPACE_CHINESE_IME

class Utf16Writer {
private:
    FILE* m_fp;

public:
    Utf16Writer();

    ~Utf16Writer();

    /** 创建一个新的utf16文件
     *
     * @param filename : 文件路径
     * @param bom :文件编码标记，0xfffe，utf16-le
     * @return
     */
    bool OpenNewFile(const char* filename, char16 bom = 0xfeff);

    size_t Write(const char16* char16Buf, size_t length);

    size_t Write(const char* char8Buf, size_t length);

    void Close();
};

END_NAMESPACE_CHINESE_IME

#endif //MAKE_DICT_UTF16WRITER_H
