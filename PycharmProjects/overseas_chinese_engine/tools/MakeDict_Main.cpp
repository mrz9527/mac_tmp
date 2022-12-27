// author : zhoukang
// date   : 2021-09-24 10:03:17

#include "BasicTypeDef.h"
#include "Utils.h"
#include "MakeBinDict.h"
#include "NextWordDicTree.h"

#include <stdio.h>
#include <map>
#include <libgen.h>
#include <vector>

using StdString = std::string;
using SysDictMap = std::map<std::string, ChineseIme::DictType>;

namespace {
    // 拼音9键
    const char* PINYIN_9KEY     = "pinyin_9key.dict";
    // 仓颉（普仓和快仓）
    const char* CANGJIE         = "cangjie.dict";
    // 注音全键
    const char* ZHUYIN_FULLKEY  = "zhuyin.dict";
    // 注音9键
    const char* ZHUYIN_9KEY     = "zhuyin_9key.dict";
    // 笔画简体9键
    const char* BIHUA_SIMP      = "bihua_simp.dict";
    // 笔画繁体9键
    const char* BIHUA_TRAD      = "bihua_trad.dict";
    // 预测词简体
    const char* PREDICT_SIMP    = "predict_simp.dict";
    // 预测词繁体
    const char* PREDICT_TRAD    = "predict_trad.dict";
}

void GetDictMap(StdString txtDictPath, SysDictMap &sysDictMap, std::vector<StdString> &predictDictFileVec) {
    int pathType = ChineseIme::Utils::IsFileOrDir(txtDictPath);

    // 路径类型判断 : 0 不存在，1文件夹， 2文件
    if (pathType == 0) {
        return;
    }

    // 获取所有文件
    std::vector<StdString> vecTxtDictFile;
    StdString srcPath = ChineseIme::Utils::RelPath2AbsPath(txtDictPath.c_str());
    if (pathType == 1) {
        ChineseIme::Utils::DfsFolder(txtDictPath, false, vecTxtDictFile);
    } else {
        vecTxtDictFile.emplace_back(txtDictPath);
    }

    // 判断文件词典类型
    for (auto file: vecTxtDictFile) {
        StdString fileName = basename(const_cast<char*>(file.c_str()));
        if (fileName.compare(PINYIN_9KEY) == 0) {
            sysDictMap[fileName] = ChineseIme::DictType::DICT_TYPE_PINYIN;
        } else if (fileName.compare(CANGJIE) == 0) {
            sysDictMap[fileName] = ChineseIme::DictType::DICT_TYPE_CANGJIE;
        } else if (fileName.compare(ZHUYIN_FULLKEY) == 0) {
            sysDictMap[fileName] = ChineseIme::DictType::DICT_TYPE_ZHUYIN;
        } else if (fileName.compare(ZHUYIN_9KEY) == 0) {
            sysDictMap[fileName] = ChineseIme::DictType::DICT_TYPE_ZHUYIN;
        } else if (fileName.compare(BIHUA_SIMP) == 0) {
            sysDictMap[fileName] = ChineseIme::DictType::DICT_TYPE_BIHUA;
        } else if (fileName.compare(BIHUA_TRAD) == 0) {
            sysDictMap[fileName] = ChineseIme::DictType::DICT_TYPE_BIHUA;
        } else if (fileName.compare(PREDICT_SIMP) == 0) {               // 预测词
            predictDictFileVec.emplace_back(fileName);
        } else if (fileName.compare(PREDICT_TRAD) == 0) {
            predictDictFileVec.emplace_back(fileName);
        }
    }
}

bool GetFileNameAndExt(StdString file, StdString& fileName, StdString& fileExt)
{
    size_t pos = file.rfind('.');
    if (pos == StdString::npos){
        return false;
    }

    fileName = file.substr(0, pos);
    fileExt = file.substr(pos + 1);
    return true;
}

bool MakeSysDict(StdString strTxtDictFile, StdString strDatDictFile, ChineseIme::DictType dictType) {
    ChineseIme::MakeBinDict dict;
    const char* version = "2021.7.28_pinyin";
    std::u16string brief = u"kika first version";
    dict.SetDictVersionAndBrief(version, strlen(version), brief.c_str(), brief.length());
    return dict.MakeProcess(strTxtDictFile.c_str(), strDatDictFile.c_str(), dictType);
}

bool MakePredictDict(StdString strTxtDictFile, StdString strDatDictFile) {
    ChineseIme::NextWordDicTree dict;
    return dict.BuildDic(strTxtDictFile.c_str(), strDatDictFile.c_str());
}


int main(int argc, char **argv) {
    printf("argc = %d \n", argc);
    if (argc != 3) {
        printf("usage: makeDict txtDictPath datDictPath\n"
               "       txtDictPath 文本词典路径，可以是文件名，可以是目录\n"
               "       datDictPath  二进制词典路径，目录\n");
        return 0;
    }

    char *txtDictPath = argv[1];
    char *datDictPath = argv[2];

    SysDictMap sysDictMap;
    std::vector<StdString> predictDictFileVec;
    GetDictMap(txtDictPath, sysDictMap, predictDictFileVec);

    // 路径判断
    int txtDictPathType = ChineseIme::Utils::IsFileOrDir(txtDictPath);
    if (txtDictPathType == 0){
        printf("%s :不是有效的路径\n", txtDictPath);
        return 0;
    }

    // 获取txtDictDir
    StdString strTxtDictDir;
    StdString strTxtDictPath = ChineseIme::Utils::RelPath2AbsPath(txtDictPath);
    if (txtDictPathType == 1) {
        strTxtDictDir = strTxtDictPath;
    } else {
        strTxtDictDir = dirname(const_cast<char*>(strTxtDictPath.c_str()));
    }


    if(ChineseIme::Utils::IsFileOrDir(datDictPath) != 1) { // 1 为文件夹路径
        printf("%s :不是有效的目录路径\n", datDictPath);
        return 0;
    }
    // 获取strDatDictDir
    StdString strDatDictDir = ChineseIme::Utils::RelPath2AbsPath(datDictPath);

    StdString fileName, fileExt;
    for(auto elem: sysDictMap){
        StdString strTxtDictFile = strTxtDictDir +"/" + elem.first;
        if (!GetFileNameAndExt(elem.first, fileName, fileExt)){
            continue;
        }
        StdString strDatDictFile = strDatDictDir +"/" + fileName + ".dat";
        if(!MakeSysDict(strTxtDictFile, strDatDictFile, elem.second)){
            printf("%s 生成系统词 %s 失败\n", strTxtDictFile.c_str(), strDatDictFile.c_str());
        } else {
            printf("%s 生成系统词 %s 成功\n", strTxtDictFile.c_str(), strDatDictFile.c_str());
        }
    }

    for(auto elem: predictDictFileVec){
        StdString strTxtDictFile = strTxtDictDir +"/" + elem;
        if (!GetFileNameAndExt(elem, fileName, fileExt)){
            continue;
        }
        StdString strDatDictFile = strDatDictDir +"/" + fileName + ".dat";

        if (!MakePredictDict(strTxtDictFile, strDatDictFile)) {
            printf("%s 生成预测词 %s 失败\n", strTxtDictFile.c_str(), strDatDictFile.c_str());
        } else {
            printf("%s 生成预测词 %s 成功\n", strTxtDictFile.c_str(), strDatDictFile.c_str());
        }
    }

    return 0;
}