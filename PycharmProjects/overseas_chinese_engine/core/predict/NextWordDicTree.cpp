/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: NextWordDicTree
* Author: YangNing
* Create: 2021-7-12
*/
#include <algorithm>
#include <cstring>
#include "securec.h"
#include "securectype.h"
#include "NextWordDicTree.h"

BEGIN_NAMESPACE_CHINESE_IME

NextWordDicTree::NextWordDicTree()
{
}

NextWordDicTree::~NextWordDicTree()
{
    ClearDic();
#ifdef ___BUILD_MODEL___
    ClearTree();
#endif  // ___BUILD_MODEL___
}

#ifdef ___BUILD_MODEL___
bool NextWordDicTree::BuildDic(const char* rawDictPath, const char* outDictPath)
{
    m_root = new TreeNode();
    if (rawDictPath == nullptr || outDictPath == nullptr) {
        LOGW("txtDictPath == nullptr || outDictPath == nullptr");
        return false;
    }
    std::string srcPath(rawDictPath);
    std::string dicPath(outDictPath);
    int pathType1 = Utils::IsFileOrDir(srcPath);
    if (pathType1 == 0 || pathType1 == 1) { // 0 不存在，1 仅存在文件夹， 2 存在文件
        LOGW("pathType1 == 0 || pathType1 == 1");
        return false;
    }
    std::ifstream in(rawDictPath);
    std::string line;

    if (in) {
        while (getline(in, line)) {
            InsertStrToTree(Split(line, " ")[0], atoi(Split(line, " ")[1].c_str()));
        }
    } else {
        return false;
    }

    FILE* fp = fopen(dicPath.c_str(), "wb");
    bool ret = Serialize(m_root, fp);
    in.close();
    fclose(fp);
    return ret;
}

void NextWordDicTree::ClearTree()
{
    if (m_root != nullptr) {
        ClearTreeNode(m_root);
        m_root = nullptr;
    }
}

void NextWordDicTree::ClearTreeNode(TreeNode* root)
{
    if (root != nullptr) {
        for (auto &mapChild : root->child) {
            ClearTreeNode(mapChild.second);
        }
        delete root;
        root = nullptr;
    }
}

void NextWordDicTree::InsertStrToTree(std::string str, int freq)
{
    if (m_root == nullptr || str == "") {
        return;
    }
    TreeNode* curNode = m_root;

    for (int i = 0; i < str.size();) {
        std::string sub_str = str.substr(i, chSize);
        std::map<std::string, TreeNode*>::iterator iter = curNode->child.find(sub_str);
        if (iter == curNode->child.end()) { // 如果在map中没有找到则插入新节点
            TreeNode* tmp_node = new TreeNode();
            curNode->child.insert(std::pair<std::string, TreeNode*>(sub_str, tmp_node));
            curNode = tmp_node;
        } else { // 如果找到了value即为指向下一个节点的指针
            curNode = iter->second;
        }
        i = i + chSize;
    }
    curNode->frequency = freq;
    curNode->count++;
}

bool NextWordDicTree::Serialize(const TreeNode* node, FILE* fp)
{    // Base case
    if (node == nullptr) {
        return false;
    }

    for (std::map<std::string, TreeNode*>::const_iterator iter = node->child.begin(); iter != node->child.end();
         iter++) {
        NodeData data;
        if (strcpy_s(data.word, sizeof(data.word), iter->first.c_str()) != EOK) {
            return false;
        }
        data.frequency = iter->second->frequency;
        data.brotherCount = node->child.size();
        data.allChildCount = GetAllChildCount(iter->second);
        fwrite(&data, sizeof(NodeData), 1, fp);
        Serialize(iter->second, fp);
    }
    return true;
}

std::vector<std::string> NextWordDicTree::Split(const std::string &str, const std::string &delim)
{
    std::vector<std::string> res;
    if (str == "") {
        return res;
    }
    // 先将要切割的字符串从string类型转换为char*类型
    char* strs = new char[str.length() + 1]; // 不要忘了
    if (strcpy_s(strs, str.length() + 1, str.c_str()) != EOK) {
        delete[] strs;
        strs = nullptr;
        return res;
    }

    char* d = new char[delim.length() + 1];
    if (strcpy_s(d, delim.length() + 1, delim.c_str()) != EOK) {
        delete[] strs;
        strs = nullptr;
        delete[] d;
        d = nullptr;
        return res;
    }

    char* p = strtok(strs, d);
    while (p) {
        std::string s = p; // 分割得到的字符串转换为string类型
        res.push_back(s); // 存入结果数组
        p = strtok(nullptr, d);
    }

    delete[] strs;
    strs = nullptr;
    delete[] d;
    d = nullptr;

    return res;
}
#endif  // ___BUILD_MODEL___

bool NextWordDicTree::LoadDic(std::string dicPath)
{
    FILE* fp = fopen(dicPath.c_str(), "rb");
    if (fp == nullptr) {
        fclose(fp);
        LOGW("下义词词典无法打开");
        return false;
    }
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (fileSize <= 0) {
        fclose(fp);
        LOGW("下义词词典为空");
        return false;
    }

    m_dictBuf = (char*) malloc(fileSize);
    if (m_dictBuf == nullptr) {
        LOGW("下义词m_dicBuf分配空间失败");
        return false;
    }
    fread(m_dictBuf, fileSize, 1, fp);
    fclose(fp);
    return true;
}

void NextWordDicTree::ClearDic()
{
    if (m_dictBuf != nullptr) {
        free(m_dictBuf);
        m_dictBuf = nullptr;
    }
}

void NextWordDicTree::SearchPrefix(std::string str)
{
    m_searchResult.clear();
    if (m_dictBuf == nullptr) {
        return;
    }
    long pos = 0;
    bool isMatch = false;
    NodeData nodeData;
    const int nodeLen = 10; // node 长度
    for (int i = 0; i < str.size();) {
        isMatch = false;
        nodeData = *reinterpret_cast<NodeData*>(m_dictBuf + pos * nodeLen);
        std::string sub_str = str.substr(i, chSize);
        if (nodeData.word == sub_str) {
            isMatch = true;
            i = i + chSize;
            pos = pos + 1;
            continue;
        }
        pos = pos + nodeData.allChildCount + 1;
        for (int m = 0; m < nodeData.brotherCount - 1; m++) {
            nodeData = *reinterpret_cast<NodeData*>(m_dictBuf + pos * nodeLen);
            if (nodeData.word == sub_str) {
                isMatch = true;
                i = i + chSize;
                pos = pos + 1;
                break;
            }
            pos = pos + 1 + nodeData.allChildCount;
        }
        if (!isMatch) {
            break;
        }
    }

    if (isMatch && nodeData.allChildCount > 0) {
        AddStrToRet(pos, "", true);
    } else {
        return;
    }
}

bool NextWordDicTree::Comp(const Pair &a, const Pair &b)
{
    return a.frequency > b.frequency;
}

size_t NextWordDicTree::GetPredictWordCount(const char16* inputString, size_t len)
{
    if (len == 0 || inputString == nullptr) {
        LOGW("The inputString is nullptr or len equals 0!");
        return 0;
    }
    const int maxBufLen = 128;
    char16 searchBuf[maxBufLen] = {'\0'}; // 因为上层传过来的搜索字符串可能没有'\0'结束符，因此需拷贝一份带'\0'的字符串
    size_t searchBufLen = sizeof(searchBuf);
    if (searchBufLen > len) {
        if (memcpy_s(searchBuf, maxBufLen, inputString, len*sizeof(char16)) != EOK) {
            return 0;
        }
        searchBuf[len] = '\0';
    } else {
        if (memcpy_s(searchBuf, maxBufLen, inputString, (searchBufLen - 1)*sizeof(char16)) != EOK) {
            return 0;
        }
        searchBuf[searchBufLen - 1] = '\0';
    }
    char16* searchStr = searchBuf;
    PredictAndSort(Utils::Utf16ToUtf8(searchStr));
    return m_searchResultSorted.size();
}

char16* NextWordDicTree::GetPredictWord(size_t id)
{
    if (id >= m_searchResultSorted.size()) {
        return nullptr;
    }

    strRet = Utils::Utf8ToUtf16(m_searchResultSorted[id].word);
    return const_cast<char16*>(strRet.c_str());
}

void NextWordDicTree::PredictAndSort(std::string str)
{
    m_searchResultSorted.clear();
    std::string subStr = "";
    const int strLenSearch = 3; // 最多截取3个字搜索下一词
    const int frenqAdd = 10000; // 搜索长度长的结果排序靠前
    for (int i = 0, pos = str.size() - chSize; i < strLenSearch && pos >= 0; i++, pos = pos - chSize) {
        subStr = str.substr(pos, chSize) + subStr;
        SearchPrefix(subStr);

        for (int j = 0; j < m_searchResult.size(); j++) {
            m_searchResult[j].frequency = m_searchResult[j].frequency + frenqAdd * i;
        }
        m_searchResultSorted.insert(m_searchResultSorted.end(), m_searchResult.begin(), m_searchResult.end());
    }
    sort(m_searchResultSorted.begin(), m_searchResultSorted.end(), Comp);
}

void NextWordDicTree::AddStrToRet(long pos, std::string str, bool isBrother)
{
    const int nodeLen = 10; // node 长度
    NodeData nodeData = *reinterpret_cast<NodeData*>(m_dictBuf + pos * nodeLen);
    if (nodeData.frequency > 0) {
        Pair pair = {str + nodeData.word, nodeData.frequency};
        m_searchResult.push_back(pair);
    }

    if (nodeData.allChildCount > 0) {
        AddStrToRet(pos + 1, str + nodeData.word, true);
    }
    if (isBrother) {
        int posInner = nodeData.allChildCount;
        for (int m = 0; m < nodeData.brotherCount - 1; m++) {
            AddStrToRet(pos + 1 + posInner, str, false);
            NodeData nodeDataB = *reinterpret_cast<NodeData*>(m_dictBuf + (pos + 1 + posInner) * nodeLen);
            posInner = posInner + nodeDataB.allChildCount + 1;
        }
    }
}

unsigned short NextWordDicTree::GetAllChildCount(const TreeNode* node) const
{
    unsigned short count = node->child.size();
    for (std::map<std::string, TreeNode*>::const_iterator iter = node->child.begin(); iter != node->child.end();
        iter++) {
        count = count + GetAllChildCount(iter->second);
    }
    return count;
}

END_NAMESPACE_CHINESE_IME