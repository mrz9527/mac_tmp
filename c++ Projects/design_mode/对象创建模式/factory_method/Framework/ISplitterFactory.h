/**
 * Project factory_method
 */


#ifndef _ISPLITTERFACTORY_H
#define _ISPLITTERFACTORY_H

#include "Splitter.h"

class ISplitterFactory {
public:
    virtual ~ISplitterFactory(){};
    virtual Splitter* createInstance() = 0;
};

#endif //_ISPLITTERFACTORY_H