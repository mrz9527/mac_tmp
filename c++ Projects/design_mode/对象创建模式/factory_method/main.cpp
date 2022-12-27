// author : zhoukang
// date   : 2022-07-06 17:35:41

#include "SplitProcess.h"
#include "Application/BinSplitterFactory.h"

int main()
{
    ISplitterFactory* factory = new BinSplitterFactory;
    SplitProcess splitProcess(factory);
    splitProcess.process();

    delete factory;
    return 0;
}

