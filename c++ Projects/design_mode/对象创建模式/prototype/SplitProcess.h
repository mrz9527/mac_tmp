// author : zhoukang
// date   : 2022-07-07 14:35:08

#ifndef DESIGN_MODE_SPLITPROCESS_H
#define DESIGN_MODE_SPLITPROCESS_H

#include "Framework/IProtoType.h"
#include "vector"

class SplitProcess {
private:
    IPrototype* m_prototype;
    // 保存副本
    std::vector<IPrototype*> m_cloneVector;

public:
    SplitProcess(IPrototype* prototype):m_prototype(prototype){};

    // 释放副本资源
    ~SplitProcess();

    void process();
};
#endif //DESIGN_MODE_SPLITPROCESS_H
