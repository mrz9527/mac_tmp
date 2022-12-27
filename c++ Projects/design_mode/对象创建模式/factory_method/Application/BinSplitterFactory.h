/**
 * Project factory_method
 */


#ifndef _BINSPLITTERFACTORY_H
#define _BINSPLITTERFACTORY_H

#include "../Framework/ISplitterFactory.h"


class BinSplitterFactory: public ISplitterFactory {
public:
    virtual ~BinSplitterFactory(){};

    Splitter *createInstance() override;
};

#endif //_BINSPLITTERFACTORY_H