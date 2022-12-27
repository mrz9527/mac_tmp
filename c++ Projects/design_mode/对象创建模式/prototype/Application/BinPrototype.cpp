// author : zhoukang
// date   : 2022-07-07 14:16:57

#include "BinPrototype.h"
#include "PrintFunction.h"

IPrototype *BinPrototype::clone()
{
    PrintFunctionPath();
    // 间接调用拷贝构造
    // 为什么不直接调用拷贝构造呢？直接调用拷贝构造，就变成了具体细节，脱离个依赖倒置原则
    // 使用虚函数clone，可以通过晚绑定的方式来间接实现拷贝构造
    return new BinPrototype(*this);
}

void BinPrototype::split()
{
    PrintFunctionPath();
}
