/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: utils
* Author: Changjie Hu
* Create: 2021-7-12
*/
#ifndef MAKE_DICT_UTILS_H
#define MAKE_DICT_UTILS_H

#include <string>
#include <vector>
#include <cstddef>
#include "BasicTypeDef.h"

BEGIN_NAMESPACE_CHINESE_IME
class Utils {
public:
// 根据目录，获取目录下的文件
    static void DfsFolder(std::string folderPath, bool deep, std::vector<std::string> &fileNameVec);

// 判断路径类型(可以输入相对路径和绝对路径），0 不存在；1 文件夹；2 文件
    static int IsFileOrDir(std::string path);

// 根据相对路径（相对当前工作目录），计算绝对路径
    static std::string RelPath2AbsPath(const char* relativePath);

// UTF8与UTF16互相转换
    static std::string Utf16ToUtf8(std::u16string s);

    static std::u16string Utf8ToUtf16(std::string s);

// UTF16与UTF32互相转换
    static std::u32string Utf16ToUtf32(std::u16string s);

    static std::u16string Utf32ToUtf16(std::u32string s);

// UTF8与UTF32互相转换
    static std::u32string Utf8ToUtf32(std::string s);

    static std::string Utf32ToUtf8(std::u32string s);
};
END_NAMESPACE_CHINESE_IME

#endif // MAKE_DICT_UTILS_H