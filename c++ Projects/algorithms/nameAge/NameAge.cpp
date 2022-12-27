// author : zhoukang
// date   : 2022-07-21 19:00:04

#include "NameAge.h"

void PrintNameAge(const NameAge &elem)
{
    printf("优先级:%d, name:%s\n", elem.second, elem.first.c_str());
}