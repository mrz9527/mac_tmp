/**
 * Project factory_method
 */


#ifndef _TXTSPLITTERFACTORY_H
#define _TXTSPLITTERFACTORY_H

#include "../Framework/ISplitterFactory.h"


class TxtSplitterFactory: public ISplitterFactory {
public:
    virtual ~TxtSplitterFactory(){};

    Splitter *createInstance() override;
};

#endif //_TXTSPLITTERFACTORY_H