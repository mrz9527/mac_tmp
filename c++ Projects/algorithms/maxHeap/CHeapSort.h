// author : zhoukang
// date   : 2022-07-20 16:55:04

#ifndef ALGORITHMS_CHEAPSORT_H
#define ALGORITHMS_CHEAPSORT_H

#include "vector"

/* Compare 为比较器，greater比较器，得到最大堆，less比较器，得到最小堆*/
template<typename T, typename Compare>
class CHeapSort {
private:
    std::vector<T> arr;
    Compare compare;
    bool bSort = true; // 初始时没有元素，相当于是有序的，每次添加元素，相当于变成无序

public:
    // 添加元素：在末尾添加元素
    void push_back(T &&elem)
    {
        arr.emplace_back(std::forward<T>(elem));
        bSort = false;
    }

    void Show(void(*callback)(const T &))
    {
        for (auto elem:arr) {
            callback(elem);
        }
    }

    // 弹出第一个元素
    // 弹出之后，将最后一个元素填充到第一个，保证完全二叉树
    // 必须保证arr非空，否则异常
    T Pop()
    {
        if (!bSort)
            MoveMaxToTop1(arr.size());
        T elem(arr[0]);
        arr.erase(arr.begin());
        return elem;
    }

    // 每次Top返回最大的那个元素
    // 必须保证arr非空
    T Top()
    {
        if (!bSort)
            MoveMToTop1(arr.size());
        return arr[0];
    }

    bool Empty()
    {
        return arr.empty();
    }


    void Sort()
    {
        // 采用in_place方式，不占用额外空间
        int nSize = arr.size();
        while (nSize > 1) {
            MoveMToTop1(nSize);
            std::swap(arr[0], arr[nSize - 1]);
            --nSize;
        }

        std::reverse(arr.begin(), arr.end());
        bSort = true;
    }

private:
    // 完全二叉树，节点位置对应关系
    // 第i个元素(从下标0开始）
    // 左孩子:2i+1    右孩子:2i+2
    // 父亲:(i-1)/2

    //举例:
    /*
                 0
             1         2
           3   4     5     6
         7  8 9 10 11 12 13 14

     */

    /* len表示取arr数组的前len个元素中最大(最小)的那个，移动到arr[0]位置
     * len表示前len个元素，因为使用的是堆排序，需要保证符合完全二叉树
     * 用法:MoveMToTop1(arr.size())；表示找所有元素中最大(最小)的。
     * */
    void MoveMToTop1(int len)
    {
        if (len <= 1) return;

        int childPos = len - 1;
        int parentPos = (childPos - 1) / 2;

        if (childPos % 2) { // 最后一个叶子节点是左节点
            if (compare(arr[childPos], arr[parentPos])) {
                std::swap(arr[childPos], arr[parentPos]);
                --childPos;
                --parentPos;
            }
        }

        int tmpPos = childPos;
        while (parentPos >= 0) {
            // 左右子节点比较
            tmpPos = (compare(arr[childPos], arr[childPos - 1])) ? childPos : childPos - 1;

            // 子节点和父节点比较
            if (compare(arr[tmpPos], arr[parentPos])) {
                std::swap(arr[tmpPos], arr[parentPos]);
            }

            childPos -= 2; // 当前的两个子节点已经比较完成，向前推2个位置
            --parentPos;
        }
    }
};

/* ArrayIterator指的是类似数组的顺序容器中的迭代器，比如vector
 * */
template<typename ArrayIterator, typename Compare>
void MyHeapSort(ArrayIterator begin, ArrayIterator end, const Compare& compare) {
    int nSize = end - begin;
    if(end - begin <= 1) {
        return;
    }

    ArrayIterator tmpIter = end;
    while (tmpIter - begin > 1) {
        MHeap(begin, tmpIter, compare);
        std::swap(*begin, *(--tmpIter));
    }

    std::reverse(begin, end);
}

#include<algorithm>

template<typename ArrayIterator, typename  Compare>
void MHeap(ArrayIterator begin, ArrayIterator end, const Compare& compare){
    int nSize = end - begin; // if(nSize <=1) return

    int childPos = nSize - 1;
    int parentPos = (childPos - 1) / 2;

    ArrayIterator childIter = --end;
    ArrayIterator parentIter = begin + parentPos;

    if (childPos % 2) { // 最后一个叶子节点是左节点
        if (compare(*childIter, *parentIter)) {
            std::swap(*childIter, *parentIter);
            --childIter;
            --parentIter;
        }
    }

    ArrayIterator tmpIter = childIter;
    while (parentIter >= begin) {
        // 左右子节点比较
        tmpIter = (compare(*childIter, *(childIter-1))) ? childIter : childIter - 1;

        // 子节点和父节点比较
        if (compare(*tmpIter, *parentIter)) {
            std::swap(*tmpIter, *parentIter);
        }

        childIter -= 2; // 当前的两个子节点已经比较完成，向前推2个位置
        --parentIter;
    }
}




#endif //ALGORITHMS_CHEAPSORT_H
