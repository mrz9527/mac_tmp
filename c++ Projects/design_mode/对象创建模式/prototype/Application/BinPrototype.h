// author : zhoukang
// date   : 2022-07-07 14:16:57

#ifndef DESIGN_MODE_BINPROTOTYPE_H
#define DESIGN_MODE_BINPROTOTYPE_H

#include "../Framework/IProtoType.h"

class BinPrototype : public IPrototype{
public:
    IPrototype *clone() override;

    void split() override;
};


#endif //DESIGN_MODE_BINPROTOTYPE_H
