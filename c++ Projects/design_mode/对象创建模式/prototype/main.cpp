// author : zhoukang
// date   : 2022-07-07 12:08:18

#include "TxtPrototype.h"
#include "SplitProcess.h"
int main()
{
    IPrototype* prototype = new TxtPrototype;
    SplitProcess* splitProcess = new SplitProcess(prototype);
    splitProcess->process();

    // 释放资源
    delete prototype;
    delete splitProcess;

    return 0;
}
