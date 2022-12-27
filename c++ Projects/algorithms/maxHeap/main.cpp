#include <iostream>

#include "CHeapSort.h"

#include "../nameAge/NameAge.h"

using MyMinHeap = CHeapSort<NameAge, LessCompareNameAge>;

using MyHeapEx = std::vector<std::pair<std::string, int>>;
int main()
{
    //MyMinHeap minHeap;
    MyHeapEx minHeap;
    minHeap.push_back(std::make_pair("zhoukang", 13));
    minHeap.push_back(std::make_pair("zhangsan", 33));
    minHeap.push_back(std::make_pair("zhoushiqi", 5));
    minHeap.push_back(std::make_pair("zhoushiyi", 1));
    minHeap.push_back(std::make_pair("xingliyan", 32));
    minHeap.push_back(std::make_pair("wangerxiao", 13));
    minHeap.push_back(std::make_pair("dazhuang", 22));

    //minHeap.Show(PrintNameAge);
    std::for_each(minHeap.begin(), minHeap.end(), PrintNameAge);
    printf("--------------\n");

    //minHeap.Sort();
    MyHeapSort(minHeap.begin(), minHeap.end(), LessCompareNameAge());
    //minHeap.Show(PrintNameAge);
    std::for_each(minHeap.begin(), minHeap.end(), PrintNameAge);

    return 0;
}