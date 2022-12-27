// author : zhoukang
// date   : 2022-07-07 12:25:14

#include "TxtPrototype.h"
#include "PrintFunction.h"

IPrototype *TxtPrototype::clone()
{
    PrintFunctionPath();
    return new TxtPrototype(*this);
}

void TxtPrototype::split()
{
    PrintFunctionPath();
}
