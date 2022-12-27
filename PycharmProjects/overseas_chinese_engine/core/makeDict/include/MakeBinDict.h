/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: dict maker
* Author: ZhouKang
* Create: 2021-7-12
*/
#ifndef MAKE_DICT_DICTRESOURCE_H
#define MAKE_DICT_DICTRESOURCE_H

#include "DictInfo.h"
#include <string>
#include "Utf16Char.h"

#include "Predict.h" // 用于生成下一词

#include "Utils.h"

BEGIN_NAMESPACE_CHINESE_IME

/* MakeBinDict类使用方法:
 *  MakeBinDict dict;
 *  dict.SetDictVersionAndBrief(...);
 *  dict.MainProcess(...);
 */
class MakeBinDict {
public:
    MakeBinDict() { };

    ~MakeBinDict();

public:
    //对外接口函数
    /** 设置词典的版本信息和简介信息
     *
     * @param version 版本信息字符串
     * @param versionLen 版本信息字符串长度
     * @param brief 简介信息字符串
     * @param briefLen 简介信息字符串长度
     */
    void SetDictVersionAndBrief(const char* version, size_t versionLen, const char16* brief, size_t briefLen);

    /** 生成词典的入口函数
     *
     * @param txtDictPath 词表路径，只能是目录路径
     * @param binDictPath 词典路径，可以是目录路径，也可以是文件路径
     * @param dictType    词典类型
     * @return true:成功，false：失败
     *
     * 说明：目前路径参数可输入相对路径和绝对路径，便于调试，后期只支持绝对路径
     */
    bool MakeProcess(const char* txtDictPath, const char* binDictPath, DictType dictType = DictType::DICT_TYPE_UNKOWN);

private:

    void AllocResource(size_t n = 40000);

    void ReleaseResource();

    bool SetDictMemoryLayout(DictType dictType);

    char16* ReadWordTableFile(const char* wordTableFile, size_t &utf16bufLen);

    bool ParseWordTable(char16* utf16buf, size_t utf16bufLen);

    void FillDictHeader();

    bool WriteDictFile(const char* dictFilePath) const;

    // 下一词
    // 要在ParseWordTable之后调用
    bool MakePredict(const char* predictDictFile, bool enablePredict = false);

private:

    /**提取key1和key2
     * 说明：
     *      zhong guo94664 486 中国, vecKeyStr只存储了zhong, nextStr = guo94664 486 中国
     *          返回false
     *      zhong guo 94664 486 中国 vecKeyStr存储了zhong guo 94664 486, nextStr = 中国
     *          返回true
     */
    static bool ExtractKeyStr(char16* str, char16** nextStr, std::vector<std::string> &vecKeyStr);

    /** 提取词和词频 */
    static bool ExtractWordAndFreq(char16* str, char16** nextStr, std::vector<std::u16string> &vecWordAndFreq);

    /** 主要用于生成词典过程中，如果发生解析错误，跳过到下一行 */
    static void JumpToNextLine(char16* str, char16** nextStr);

private:
    DictHeader m_dictHeader;
    std::vector<DictWordItem> m_vecDictWordItem;
};

END_NAMESPACE_CHINESE_IME

#endif //MAKE_DICT_DICTRESOURCE_H
