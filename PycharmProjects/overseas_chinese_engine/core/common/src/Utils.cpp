/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: utils
* Author: ZhouKang
* Create: 2021-7-12
*/
#include <cstdlib>
#include <string>

#ifdef _WIN32
#include <io.h>
#define ACCESS _access
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#include "boost/locale/encoding_utf.hpp"
#include "securecutil.h"
#include "Utils.h"

BEGIN_NAMESPACE_CHINESE_IME

void Utils::DfsFolder(std::string folderPath, bool deep, std::vector<std::string> &fileNameVec)
{
#ifdef WIN32
    _finddata_t fileInfo;
    string strfind = folderPath + "\\*";
    long handle = _findfirst(strfind.c_str(), &fileInfo);
    if (handle == -1L) {
        return;
    }

    do {
        // 判断是否有子目录
        if (fileInfo.attrib & _A_SUBDIR) {
            if (deep && (strcmp(fileInfo.name, ".") != 0) && (strcmp(fileInfo.name, "..") != 0)) {
                string newPath = folderPath + "\\" + fileInfo.name;
                DfsFolder(newPath, deep);
            }
        } else {
            string filename = (folderPath + "\\" + fileInfo.name);
            fileNameVec.push_back(filename + string(fileInfo.name));
        }
    }while (_findnext(handle, &fileInfo) == 0);

    _findclose(handle);
#else
    DIR* dp = nullptr;
    struct dirent* entry = nullptr;
    struct stat statbuf;
    if ((dp = opendir(folderPath.c_str())) == nullptr) {
        return;
    }

    while ((entry = readdir(dp)) != nullptr) {
        if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) {
            continue;
        }
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            std::string tmpFolderPath = folderPath;
            tmpFolderPath.append("/").append(entry->d_name);
            if (deep) {
                DfsFolder(tmpFolderPath, deep, fileNameVec);
            }
        } else {
            std::string filename = entry->d_name;
            fileNameVec.push_back(folderPath + "/" + filename);
        }
    }
    closedir(dp);
#endif
}

namespace {
    const int PATH_TYPE_ERROR = 0;
    const int PATH_TYPE_DIR = 1;
    const int PATH_TYPE_FILE = 2;
}
// 0 不存在，1文件夹， 2文件
int Utils::IsFileOrDir(std::string path)
{
#ifdef WIN32
    DWORD dwAttr = GetFileAttributes(path.c_str());
        if (dwAttr == 0xFFFFFFFF) {
            return PATH_TYPE_ERROR;
        } else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) { // 文件夹
            return PATH_TYPE_DIR;
        } else { // 文件
            return PATH_TYPE_FILE;
        }
#else
    if (access(path.c_str(), 0) == 0) {
        struct stat st;
        memset_s(&st, sizeof(struct stat), 0, sizeof(struct stat));
        stat(path.c_str(), &st);

        if (S_ISDIR(st.st_mode)) {
            return PATH_TYPE_DIR;
        } else {
            return PATH_TYPE_FILE;
        }
    } else {
        return PATH_TYPE_ERROR;
    }
#endif
}

std::string Utils::RelPath2AbsPath(const char* relativePath)
{
    char dir[PATH_MAX] = {0};
#ifdef _WIN32
    _fullpath(dir, relativePath, sizeof(dir)/sizeof(dir[0]));
#else
    realpath(relativePath, dir);
#endif
    return dir;
}

std::string Utils::Utf16ToUtf8(std::u16string s)
{
    return boost::locale::conv::utf_to_utf<char>(s.c_str());
}

std::u16string Utils::Utf8ToUtf16(std::string s)
{
    return boost::locale::conv::utf_to_utf<char16_t>(s.c_str());
}

std::u32string Utils::Utf16ToUtf32(std::u16string s)
{
    return boost::locale::conv::utf_to_utf<char32_t>(s.c_str());
}

std::u16string Utils::Utf32ToUtf16(std::u32string s)
{
    return boost::locale::conv::utf_to_utf<char16_t>(s.c_str());
}

std::string Utils::Utf32ToUtf8(std::u32string s)
{
    return boost::locale::conv::utf_to_utf<char>(s.c_str());
}

std::u32string Utils::Utf8ToUtf32(std::string s)
{
    return boost::locale::conv::utf_to_utf<char32_t>(s.c_str());
}

END_NAMESPACE_CHINESE_IME
