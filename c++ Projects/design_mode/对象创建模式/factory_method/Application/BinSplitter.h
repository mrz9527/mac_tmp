/**
 * Project factory_method
 */


#ifndef _BINSPLITTER_H
#define _BINSPLITTER_H

#include "../Framework/Splitter.h"

class BinSplitter: public Splitter{
public:
    virtual ~BinSplitter(){};
    virtual void split() override;
};

#endif //_BINSPLITTER_H