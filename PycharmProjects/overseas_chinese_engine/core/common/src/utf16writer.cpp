/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: char16 writer
* Author: ZhouKang
* Create: 2021-7-12
*/

#include "utf16writer.h"

BEGIN_NAMESPACE_CHINESE_IME

Utf16Writer::Utf16Writer()
{
    m_fp = nullptr;
}

Utf16Writer::~Utf16Writer()
{
    Close();
}


bool Utf16Writer::OpenNewFile(const char* filename, char16 bom)
{
    if (filename == nullptr) {
        return false;
    }
    if ((m_fp = fopen(filename, "wb")) == nullptr) {
        return false;
    }
    fwrite(&bom, sizeof(char16), 1, m_fp);
    return true;
}

size_t Utf16Writer::Write(const char16* char16Buf, size_t length)
{
    if (nullptr == m_fp || nullptr == char16Buf || 0 == length) {
        return 0;
    }
    return fwrite(char16Buf, sizeof(char16), length, m_fp);
}

size_t Utf16Writer::Write(const char* char8Buf, size_t length)
{
    if (nullptr == m_fp || nullptr == char8Buf || 0 == length) {
        return 0;
    }
    return fwrite(char8Buf, sizeof(char), length, m_fp);
}

void Utf16Writer::Close()
{
    if (nullptr != m_fp) {
        fclose(m_fp);
    }
    m_fp = nullptr;
}
END_NAMESPACE_CHINESE_IME
