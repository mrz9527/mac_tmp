// author : zhoukang
// date   : 2022-07-07 12:25:14

#ifndef DESIGN_MODE_TXTPROTOTYPE_H
#define DESIGN_MODE_TXTPROTOTYPE_H

#include "../Framework/IProtoType.h"
class TxtPrototype : public IPrototype{
public:
    // 如果类有多个成员变量，针对每个成员变量，都是实现自己的拷贝构造函数，而且是深拷贝构造
    virtual IPrototype *clone() override;
    virtual void split() override;
};


#endif //DESIGN_MODE_TXTPROTOTYPE_H
