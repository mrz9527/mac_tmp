// author : zhoukang
// date   : 2022-07-07 12:22:20

#ifndef DESIGN_MODE_IPROTOTYPE_H
#define DESIGN_MODE_IPROTOTYPE_H

class IPrototype {
public:
    virtual ~IPrototype()
    {};

    virtual IPrototype *clone() = 0;

    // 稳定接口
    virtual void split() = 0;
};

#endif //DESIGN_MODE_IPROTOTYPE_H
