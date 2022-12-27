/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: 下一词预测
* Author: ZhouKang
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_PREDICT_H
#define OVERSEASCHINESEENGINE_PREDICT_H

#include "BasicTypeDef.h"
#include "Utf16Char.h"
#include "string"
#include <vector>
#include <stack>
#include <memory>
#include <algorithm>
#include "Log.h"

BEGIN_NAMESPACE_CHINESE_IME

template<typename NodeType, typename NodeValue>
struct PredictNodeData {
    NodeType ch; // 节点的字符
    NodeValue freq; // 节点的词频
    uint16 childrenCount; // 孩子节点个数
};

template<typename NodeType, typename NodeValue>
struct PredictNode {
    NodeType ch = 0; // 节点的字符
    NodeValue freq = 0; // 节点的词频
    std::vector<std::shared_ptr<PredictNode>> children; // 孩子节点数组
    PredictNode(NodeType c = 0):ch(c)
    { }; // 没有传递词频，是因为需要词频判断之后再设置
};


template<typename NodeType = char16, typename NodeValue = uint16, size_t (* CalcLengthFunc)(
        const NodeType*) = Utf16Strlen>
class Predict {
public:
    typedef PredictNode<NodeType, NodeValue> Node;
    typedef std::shared_ptr<Node> SPNode;

    typedef PredictNodeData<NodeType, NodeValue> NodeData;

    typedef std::basic_string<NodeType> ResultString;
    typedef ResultString InputString;

public:
    Predict()
    { }

    ~Predict()
    {
        ReleaseResource();
    }

    // 输入词条需要提前按照升序排序，这样方便快速构建和查找
    bool Build(const std::vector<std::pair<InputString, uint16>> &pairs)
    {
        InitRoot();

        for (uint32 i = 0; i < pairs.size(); ++i) {
            InsertItem(pairs[i].first.c_str(), pairs[i].second);
        }

        TreeToArray();

        return true;
    }

    bool Build(const std::vector<InputString> &items, const std::vector<NodeValue> &values)
    {
        InitRoot();

        for (uint32 i = 0; i < items.size(); ++i) {
            InsertItem(items[i].c_str(), values[i]);
        }

        TreeToArray();
        return true;
    }

    bool Build(const NodeType** items, const NodeValue* values, uint32 size)
    {
        InitRoot();

        for (uint32 i = 0; i < size; ++i) {
            InsertItem(items[i], values[i]);
        }

        TreeToArray();
        return true;
    };

    void Save(const char* file)
    {
        if (m_nodeCount <= 1) { // 没有节点，或者只有一个根节点，不需要写入文件
            return;
        }

        FILE* fp = nullptr;
        if ((fp = fopen(file, "wb")) == nullptr) {
            return;
        }

        uint32 depth = m_accLayerNodeCountVec.size();
        fwrite(&depth, sizeof(uint32), 1, fp); // 树的深度，也是m_accLayerNodeCountVec的size
        fwrite(m_accLayerNodeCountVec.data(), sizeof(uint32), depth, fp); // 树按层累计节点数目
        fwrite(m_nodeDataArr, sizeof(NodeData), m_nodeCount, fp); // 节点信息
        fclose(fp);
    }

    bool Load(const char* file)
    {
        ReleaseResource();

        FILE* fp = nullptr;
        void* buf = nullptr;
        do {
            fp = fopen(file, "rb");
            if (fp == nullptr) {
                break;
            }
            fseek(fp, 0, SEEK_END);
            long fileSize = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            if (fileSize == 0) {
                break;
            }

            buf = malloc(fileSize);
            if (buf == nullptr) {
                break;
            }

            fread(buf, fileSize, 1, fp);
            fclose(fp);

            m_dictBuf = buf;

            uint32 depth = 0;
            depth = *(uint32*) buf;
            buf = (uint32*) buf + 1;
            m_accLayerNodeCountVec.insert(m_accLayerNodeCountVec.end(), (uint32*) buf, (uint32*) buf + depth);
            buf = (uint32*) buf + depth;
            m_nodeDataArr = (NodeData*) buf;

            return true;
        } while (0);

        if (fp != nullptr) {
            fclose(fp);
        }

        return false;
    }

    void GetPredict(const NodeType* item, std::vector<std::pair<ResultString, NodeValue> > &result)
    {
        uint32 nodeIndex = 0;
        if (Search(item, nodeIndex)) {
            GetSuffix(nodeIndex, result);
            SortByValue(result);
        }
    }

    void ReleaseResource()
    {
        if (m_root != nullptr) {
            m_root.reset();
        }

        if (m_dictBuf) {
            free(m_dictBuf);
            m_dictBuf = nullptr;
            std::vector<uint32>().swap(m_accLayerNodeCountVec);
        }
    }

private:
    void TreeToArray()
    {
        if (m_nodeCount <= 1) {
            return;
        }

        //开辟空间
        NodeData* nodeDataArr = (NodeData*) malloc(sizeof(NodeData) * m_nodeCount);
        if (nodeDataArr == nullptr) {
            return;
        }
        std::vector<uint32> accLayerNodeCountVec; // 累计至当前层的节点树
        // dequeNode用来存储每一层的节点
        std::deque<SPNode> dequeNode;
        dequeNode.push_back(m_root);

        uint32 curLayerNodeCount = 1; // 当前层节点数目
        uint32 accLayerNodeCount = 0; // 累计到上一层节点数目

        while (!dequeNode.empty()) {
            uint32 nextLayerNodeCount = 0; // 下一层节点数目
            uint32 curLayerNodePos = 0; // 当前层节点的索引[0, curLayerNodeCount)
            while (curLayerNodePos < curLayerNodeCount) { // 当前层节点索引 和当前层节点数目
                SPNode curNode = dequeNode.front(); // 获取当前层第curLayerNodePos个元素
                dequeNode.pop_front(); // 弹出当前层第curLayerNodePos个元素
                uint32 noodeDataIndex = accLayerNodeCount +
                                        curLayerNodePos; // curNode在nodeDataArr中的下标index = accLayerNodeCount + curLayerNodePos
                NodeToNodeData(curNode, nodeDataArr[noodeDataIndex]); // 将当前元素curNode写入nodeDataArr中

                nextLayerNodeCount += curNode->children.size();
                for (const auto &childNode:curNode->children) {
                    dequeNode.push_back(childNode);
                }
                ++curLayerNodePos;
                //dequeNode.insert(dequeNode.end(), curNode->children.begin(), curNode->children.end()); //将所有孩子放入队列
            }

            accLayerNodeCount += curLayerNodeCount; // 累计至当前层节点数目
            accLayerNodeCountVec.push_back(accLayerNodeCount - 1); // 存放当前层最后一个节点的编号

            curLayerNodeCount = nextLayerNodeCount; // 更新当前层节点数目，为下一层循环做准备
        }

        ReleaseResource();
        m_nodeDataArr = nodeDataArr;
        m_accLayerNodeCountVec.insert(m_accLayerNodeCountVec.end(), accLayerNodeCountVec.begin(),
                                      accLayerNodeCountVec.end());
    };

    void NodeToNodeData(const SPNode &node, NodeData &nodeData)
    {
        nodeData.ch = node->ch;
        nodeData.freq = node->freq;
        nodeData.childrenCount = node->children.size();
    }

    void InsertItem(const NodeType* item, NodeValue value)
    {
        size_t itemLen = CalcLengthFunc(item);
        if (itemLen <= 0) {
            return;
        }

        SPNode node = m_root;
        bool find = true; // 判断是否存在某个孩子节点，他的字符串等于ch
        for (size_t i = 0; i < itemLen; ++i) { // 遍历字符串
            NodeType ch = item[i];
            typename std::vector<SPNode>::iterator it = node->children.begin();
            if (find) {
                for (; it < node->children.end(); ++it) { // 遍历node的孩子节点
                    if ((*it)->ch == ch) { // 找到了
                        break;
                    }
                }
                if (it >= node->children.end()) { // 只在词的最后一个字符添加词频，主要是用于区分当前节点是否是一个完整词
                    find = false;
                }
            }

            if (!find) {
                SPNode sp(new Node(ch));
                node->children.push_back(sp);
                node = sp;

                ++m_nodeCount; // 每添加一个节点，节点树个数+1
            } else {
                node = *it;
            }
        }

        if ((find && node->freq < value) || (!find)) {
            node->freq = value;
        }
    };

    const SPNode Search(const NodeType* item)
    {
        if (m_root == nullptr)
            return nullptr;

        size_t itemLen = CalcLengthFunc(item);
        if (itemLen <= 0)
            return nullptr;

        SPNode node = m_root;

        for (size_t i = 0; i < itemLen; ++i) {
            bool find = false;
            for (int j = 0; j < node->children.size(); ++j) {
                if (node->children[j]->ch == item[i]) {
                    find = true;
                    node = node->children[j];
                    break;
                }
            }
            if (!find) {
                return nullptr;
            }
        }

        return node;
    };

    bool Search(const NodeType* item, uint32 &nodeIndex)
    {
        size_t itemLen = CalcLengthFunc(item);
        if (itemLen <= 0)
            return false;

        uint32 curNodeIndex = 0;  // 当前节点的索引
        uint32 firstChildNodeIndex = 0; // 第一个孩子节点的索引

        for (size_t i = 0; i < itemLen; ++i) {
            NodeData tmp = {item[i], 0, 0}; // 主要按照字符查，其余数据段置0
            if (!FirstChildIndex(curNodeIndex, firstChildNodeIndex)) {
                return false;
            }

            void* find = std::bsearch(&tmp, m_nodeDataArr + firstChildNodeIndex,
                                      m_nodeDataArr[curNodeIndex].childrenCount, sizeof(NodeData),
                                      [](const void* left, const void* right) -> int {
                                          return ((NodeData*) left)->ch - ((NodeData*) right)->ch;
                                      });
            if (find == nullptr)
                return false;
            curNodeIndex = (NodeData*) find - m_nodeDataArr;
        }
        nodeIndex = curNodeIndex;

        return true;
    };

    bool IsValidIndex(uint32 NodeIndex)
    {
        if (NodeIndex < 0 || NodeIndex >= m_accLayerNodeCountVec[m_accLayerNodeCountVec.size() - 1]) { // 父节点超出索引范围
            return false;
        }
        return true;
    }

    // 约定根节点所在层为第0层，不存在时返回-1
    int GetLayerIndex(uint32 nodeIndex)
    {
        if (!IsValidIndex(nodeIndex)) { // 索引无效
            return -1;
        }

        std::vector<uint32>::iterator it = std::upper_bound(m_accLayerNodeCountVec.begin(),
                                                            m_accLayerNodeCountVec.end(), nodeIndex);
        return it - m_accLayerNodeCountVec.begin();

    }

    // return false: 没有找到孩子节点
    bool FirstChildIndex(uint32 parentNodeIndex, uint32 &firstChildNodeIndex)
    {
        if (!IsValidIndex(parentNodeIndex)) {
            return false;
        }

        if (m_nodeDataArr[parentNodeIndex].childrenCount == 0) {
            return false;
        }

        if (parentNodeIndex == 0) {
            firstChildNodeIndex = 1;
        } else {
            std::vector<uint32>::iterator it = std::lower_bound(m_accLayerNodeCountVec.begin(),
                                                                m_accLayerNodeCountVec.end(), parentNodeIndex);

            uint32 layerIndex = it - m_accLayerNodeCountVec.begin();
            if (layerIndex < 1) {
                LOGW("layerIndex<1");
                return false;
            }

            firstChildNodeIndex = *it + 1;
            uint32 startPos = m_accLayerNodeCountVec[layerIndex - 1];
            for (uint32 curLayerNodePos = startPos + 1; curLayerNodePos < parentNodeIndex; ++curLayerNodePos) {
                firstChildNodeIndex += m_nodeDataArr[curLayerNodePos].childrenCount;
            }
        }
        return true;
    }

    void GetSuffix(SPNode node, std::vector<ResultString> &result, std::vector<NodeValue> &values)
    {
        if (node.get() == nullptr || node == m_root) {
            return;
        }

        result.reserve(256);
        values.reserve(256);

        //std::vector<NodeType> vecCh; //用于临时存储字符串
        ResultString vecCh;
        std::stack<std::pair<SPNode, uint16>> stk; //存的是父节点 和 存的是父节点的第index个孩子节点被访问
        stk.push(std::make_pair(node, 0));

        while (!stk.empty()) {
            //找到叶子节点
            SPNode curNode = stk.top().first;
            uint16 childIndex = stk.top().second;
            while (childIndex < curNode->children.size()) {
                curNode = curNode->children[childIndex]; // 获取父节点的第childIndex个孩子节点
                stk.push(std::make_pair(curNode, 0)); // 将该孩子节点，以及第0个孙子节点添加进来
                vecCh.push_back(curNode->ch); // 添加字符，构建字符串

                childIndex = 0;
            }

            //得到叶子节点，判断叶子结点词频
            if (curNode->freq > 0 && vecCh.size() > 0) {
                ResultString str(vecCh.data(), vecCh.size());
                result.push_back(str);//构建结果集合
                values.push_back(curNode->freq);
            }

            // 将叶子节点弹出栈顶
            stk.pop();
            vecCh.pop_back();

            if (!stk.empty()) { // 主要用于更新父节点的孩子节点索引，所以先pop获取父节点，及上一次孩子节点索引，在push，刷新孩子节点索引
                curNode = stk.top().first;
                childIndex = stk.top().second;
                childIndex += 1;

                stk.pop();

                stk.push(std::make_pair(curNode, childIndex));
            }
        }

        result.shrink_to_fit();
        values.shrink_to_fit();
    };

    void GetSuffix(uint32 nodeIndex, std::vector<std::pair<ResultString, NodeValue> > &result)
    {
        if (!IsValidIndex(nodeIndex)) {
            return;
        }

        result.reserve(256);

        //std::vector<NodeType> vecCh; //用于临时存储字符串
        ResultString vecCh;
        std::stack<std::pair<uint32, uint16>> stk; //存的是父节点的NodeIndex 和 父节点的第index个孩子节点被访问
        stk.push(std::make_pair(nodeIndex, 0));

        while (!stk.empty()) {
            uint32 curNodeIndex = stk.top().first; // 当前节点
            uint16 childIndex = stk.top().second;   // 当前节点的第childIndex个孩子待访问

            while (childIndex < m_nodeDataArr[curNodeIndex].childrenCount) {
                uint32 firstChildNodeIndex = 0;
                if (!FirstChildIndex(curNodeIndex, firstChildNodeIndex)) {
                    LOGW("FirstChildIndex error");
                    return;
                }
                curNodeIndex = firstChildNodeIndex + childIndex;
                stk.push(std::make_pair(curNodeIndex, 0));
                vecCh.push_back(m_nodeDataArr[curNodeIndex].ch); // 添加字符，构建字符串

                childIndex = 0;
            }

            //得到叶子节点，判断叶子结点词频
            if (m_nodeDataArr[curNodeIndex].freq > 0 && vecCh.size() > 0) {
                ResultString str(vecCh.data(), vecCh.size());
                result.emplace_back(std::make_pair(str, m_nodeDataArr[curNodeIndex].freq));
            }

            // 将叶子节点弹出栈顶
            stk.pop();
            vecCh.pop_back();

            if (!stk.empty()) { // 主要用于更新父节点的孩子节点索引，所以先pop获取父节点，及上一次孩子节点索引，在push，刷新孩子节点索引
                curNodeIndex = stk.top().first;
                childIndex = stk.top().second;
                childIndex += 1;

                stk.pop();
                stk.push(std::make_pair(curNodeIndex, childIndex));
            }
        }

        result.shrink_to_fit();
    };

    void SortByValue(std::vector<std::pair<ResultString, NodeValue> > &result)
    {
        std::stable_sort(result.begin(), result.end(), [](
                const std::pair<ResultString, NodeValue> &left,
                const std::pair<ResultString, NodeValue> &right) {
            return left.second > right.second;
        });
    }

    void InitRoot()
    {
        m_root.reset(new Node);
        m_nodeCount = 1;
    }

private:
    SPNode m_root = nullptr; // 根节点
    uint32 m_nodeCount = 0; // 记录节点的数量，便于读写文件时分配buf长度

    void* m_dictBuf = nullptr;
    NodeData* m_nodeDataArr = nullptr;
    std::vector<uint32> m_accLayerNodeCountVec; // 主要用于快速定位树的节点在m_nodeDataArr中的下标,m_accLayerNodeCountVec.size()就是整颗树的深度，第i个元素存放的是树的第i层最后一个节点在m_nodeDataArr中的索引
};


END_NAMESPACE_CHINESE_IME


#endif //OVERSEASCHINESEENGINE_PREDICT_H
