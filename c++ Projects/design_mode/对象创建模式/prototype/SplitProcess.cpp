// author : zhoukang
// date   : 2022-07-07 14:32:59

#include "SplitProcess.h"
#include "PrintFunction.h"

void SplitProcess::process()
{
    PrintFunctionPath();
    if(m_prototype) {
        m_prototype->split();
        IPrototype* clonePrototype = m_prototype->clone();
        m_cloneVector.push_back(clonePrototype);
    }
}

SplitProcess::~SplitProcess()
{
    if(m_cloneVector.size()){
        // 释放副本资源
    }
}
