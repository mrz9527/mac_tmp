/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: NextWordDicTree
* Author: YangNing
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_NEXTWORDDICTREE_H
#define OVERSEASCHINESEENGINE_NEXTWORDDICTREE_H
// Enable the following line when building the binary dictionary model.
// #define ___BUILD_MODEL___
#include "string"
#include "vector"
#include "BasicTypeDef.h"
#include "Utils.h"
#include "Utf16Char.h"
#include "Log.h"
#ifdef ___BUILD_MODEL___
#include "fstream"
#endif  // ___BUILD_MODEL___

BEGIN_NAMESPACE_CHINESE_IME

class NextWordDicTree {
public:
    NextWordDicTree();
    ~NextWordDicTree();
#ifdef ___BUILD_MODEL___
    bool BuildDic(const char* rawDictPath, const char* outDictPath);
#endif  // ___BUILD_MODEL___
    bool LoadDic(std::string dicPath);
    void ClearDic();
    size_t GetPredictWordCount(const char16* inputString, size_t len);
    char16* GetPredictWord(size_t id);
private:
    struct TreeNode {
        int count; // 表示以该汉字结束的字符串个数
        int frequency; // 词频
        std::map<std::string, TreeNode*> child; // 键值为当前汉字，实值为后面汉字节点的指针
    };

    struct Pair {
        std::string word;
        int frequency;
    };

    struct NodeData {
        char word[4];
        unsigned short frequency;
        unsigned short brotherCount;
        unsigned short allChildCount;
    };
#ifdef ___BUILD_MODEL___
    void InsertStrToTree(std::string str, int freq); // 插入字符串构建字典树
    void ClearTree();
    void ClearTreeNode(TreeNode* root);
    bool Serialize(const TreeNode* node, FILE* fp);
    std::vector<std::string> Split(const std::string &str, const std::string &delim);
#endif  // ___BUILD_MODEL___
    void SearchPrefix(std::string str); // 返回所有前缀为str的字符串
    void PredictAndSort(std::string str);
    void AddStrToRet(long pos, std::string str, bool isBrother);
    unsigned short GetAllChildCount(const TreeNode* node) const;
    static bool Comp(const Pair &a, const Pair &b);
private:
#ifdef ___BUILD_MODEL___
    TreeNode* m_root = nullptr;
#endif  // ___BUILD_MODEL___
    std::vector<Pair> m_searchResult;
    std::vector<Pair> m_searchResultSorted;
    char* m_dictBuf = nullptr;
    std::u16string strRet;
    int chSize = 3;
};
END_NAMESPACE_CHINESE_IME
#endif // OVERSEASCHINESEENGINE_NEXTWORDDICTREE_H
