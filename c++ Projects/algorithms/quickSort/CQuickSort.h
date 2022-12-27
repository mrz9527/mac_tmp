// author : zhoukang
// date   : 2022-07-20 16:55:04

#ifndef ALGORITHMS_CQUICKSORT_H
#define ALGORITHMS_CQUICKSORT_H

#include "vector"
#include "stack"

template<typename T, typename Compare>
class CQuickSort {
private:
    std::vector<T> arr;
    Compare compare;

    void (* callback)(const T&);

public:
    void push_back(T&& elem)
    {
        arr.emplace_back(std::forward<T>(elem));
    }

    //
    void Show(void(* call)(const T&))
    {
        callback = call;
        for (auto elem:arr) {
            callback(elem);
        }
    }

    int GetMid(int left, int right)
    {
        return (left + right) / 2;
    }

    // 三路快排法[left, right)
    void Sort_3Round(int left, int right)
    {

        int mid = (right + left) / 2;

        if (compare(arr[left], arr[mid])) {
            if (compare(arr[mid], arr[right])) {
                std::swap(arr[mid], arr[left + 1]);
                OneTimeProcess(left + 1, right - 1);
            }
        }

    }

    // 普通快排 + 递归
    void Sort_recursive()
    {
        Sort_recursive(0, arr.size() - 1);
    }

    // 普通快排 + 迭代
    void Sort_Iter()
    {
        int nSize = arr.size();
        if (nSize <= 1) return;

        std::stack<std::pair<int, int>> stack;
        stack.push(std::make_pair(0, nSize - 1));

        while (!stack.empty()) {
            auto elem = stack.top();
            // 处理
            int left = elem.first, right = elem.second;
            int mid = OneTimeProcess(left, right);

            stack.pop();

            if (mid - left > 1)
                stack.push(std::make_pair(left, mid - 1));

            if (right - mid > 1)
                stack.push(std::make_pair(mid + 1, right));
        }
    }

    // 普通快排 + 迭代
    void Sort_Iter_BaseFrom3Num()
    {
        int left = 0;
        int right = arr.size() - 1;

        // 三数取中，调整基准
        if (AdjustBaseFrom3NumEx()) {
            ++left;
            --right;
        }
        std::stack<std::pair<int, int>> stack;
        stack.push(std::make_pair(left, right));

        while (!stack.empty()) {
            auto elem = stack.top();
            // 处理
            int left = elem.first, right = elem.second;

            // 暂时不考虑边界情况
            if (right <= left) {
                // 不添加到栈
            };
            if (right - left == 1) {
                if (!compare(arr[0], arr[1])) {
                    std::swap(arr[0], arr[1]);
                }
                // 不添加到栈
            }

            int mid = OneTimeProcess(left, right);

            stack.pop();

            if (mid - left > 1)
                stack.push(std::make_pair(left, mid - 1));

            if (right - mid > 1)
                stack.push(std::make_pair(mid + 1, right));
        }
    }

    void partition(int left, int right)
    {
        if (left >= right)
            return;

        int src_left = left, src_right = right;

        T base = arr[left];

        while (left < right) {
            while (left < right && compare(base, arr[right])) --right;
            if (left < right) arr[left++] = arr[right];

            while (left < right && compare(arr[left], base)) ++left;
            if (left < right) arr[right--] = arr[left];
        }

        arr[left] = base;
        partition(src_left, left - 1);
        partition(left + 1, src_right);
    }

    void quick_sort()
    {
        if (arr.size() <= 1)
            return;

        srand(time(0));
        int base = rand() % arr.size();
        printf("base = %d\n", base);
        std::swap(arr[0], arr[base]);
        partition(0, arr.size() - 1);
    }

private:
    // 取第一个元素作为基准
    int OneTimeProcess(int left, int right)
    {
        int base = left;
        int i = left + 1;
        int j = right;

        for (; j > i; --j) {
            if (compare(arr[j], arr[base])) {
                for (; i < j; ++i) {
                    if (compare(arr[base], arr[i])) {
                        std::swap(arr[i], arr[j]);
                        break;
                    }
                }

                // 结束当前循环条件:
                // 1. [v][>v][<v]，循环结束时，交换[<v]和[>v]的位置，j-i==1，退出
                // 2. [v][>v][>v],或者[v][<v][<v]，循环结束时,j == i，退出
                if (j - i == 1 || j == i) {
                    break;
                }

                ++i;
            }
        }

        int mid = i;
        if (compare(arr[base], arr[i])) {
            mid = base;
        } else {
            std::swap(arr[base], arr[i]);
        }
        return mid;
    }

    // 普通快排 + 递归
    void Sort_recursive(int left, int right)
    {
        if (left >= right) {
            return;
        }

        int mid = OneTimeProcess(left, right);

        Sort_recursive(left, mid - 1);
        Sort_recursive(mid + 1, right);
    }

    // 3数取中法：选择初始基准
    bool AdjustBaseFrom3NumEx()
    {
        int nSize = arr.size();
        if (nSize <= 2) return false;

        int left = 0;
        int right = nSize - 1;
        int mid = (left + right) / 2;

        if (compare(arr[left], arr[mid])) {
            if (!compare(arr[mid], arr[right])) {
                std::swap(arr[right], arr[mid]);
            }
            std::swap(arr[left + 1], arr[mid]);
        } else { // mid left
            if (compare(arr[left], arr[right])) { // mid left right
                std::swap(arr[left], arr[mid]);
            } else {    // mid right left
                std::swap(arr[left], arr[mid]);
                std::swap(arr[mid], arr[right]);
            }
            std::swap(arr[left + 1], arr[mid]);
        }
        return true;
    }

    void AdjustBaseFrom3Num()
    {
        int nSize = arr.size();
        if (nSize <= 2) return;

        int left = 0;
        int right = nSize - 1;
        int mid = (left + right) / 2;

        if (compare(arr[left], arr[mid])) {
            if (!compare(arr[mid], arr[right])) {
                std::swap(arr[right], arr[mid]);
            }
            std::swap(arr[left + 1], arr[mid]);
        } else { // mid left
            if (compare(arr[left], arr[right])) { // mid left right
                std::swap(arr[left], arr[mid]);
            } else {    // mid right left
                std::swap(arr[left], arr[mid]);
                std::swap(arr[mid], arr[right]);
            }
            std::swap(arr[left + 1], arr[mid]);
        }
    }
};

#endif //ALGORITHMS_CHEAPSORT_H
