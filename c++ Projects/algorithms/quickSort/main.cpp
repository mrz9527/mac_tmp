// author : zhoukang
// date   : 2022-07-21 18:57:30

#include "CQuickSort.h"
#include "../nameAge/NameAge.h"


//// [left, right]
//void partition(std::vector<int>& arr, int left, int right)
//{
//    if (left >= right)
//        return;
//
//    int src_left = left, src_right = right;
//
//    int base = arr[left];
//
//    while (left < right) {
//        while (left < right && arr[right] >= base) --right;
//        if (left < right) arr[left++] = arr[right];
//
//        while(left < right && arr[left] < base) ++left;
//        if(left < right) arr[right--] = arr[left];
//    }
//
//    arr[left] = base;
//    partition(arr, src_left, left -1);
//    partition(arr, left + 1, src_right);
//}
//
//void quick_sort(std::vector<int>& arr)
//{
//    if (arr.size() <= 1)
//        return;
//
//    int mid = rand() / arr.size();
//    std::swap(arr[0], arr[mid]);
//    partition(arr, 0, arr.size() - 1);
//}

int main()
{
    using MyQuickSort = CQuickSort<NameAge, LessCompareNameAge>;

    MyQuickSort myQuickSort;
    myQuickSort.push_back(std::make_pair("zhoukang", 13));
    myQuickSort.push_back(std::make_pair("zhangsan", 33));
    myQuickSort.push_back(std::make_pair("zhoushiqi", 5));
    myQuickSort.push_back(std::make_pair("zhoushiyi", 1));
    myQuickSort.push_back(std::make_pair("xingliyan", 32));
    myQuickSort.push_back(std::make_pair("wangerxiao", 13));
    myQuickSort.push_back(std::make_pair("dazhuang", 22));

    printf("init--------------\n");
    myQuickSort.Show(PrintNameAge);
    printf("--------------\n");

    //myQuickSort.Sort_Iter();
    //myQuickSort.quick_sort();
    myQuickSort.Sort_Iter_BaseFrom3Num();
    printf("end--------------\n");
    myQuickSort.Show(PrintNameAge);
}