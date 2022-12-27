/**
 * Project factory_method
 */


#ifndef _TXTSPLITTER_H
#define _TXTSPLITTER_H

#include "../Framework/Splitter.h"

class TxtSplitter : public Splitter {
public:
    virtual ~TxtSplitter(){};
    virtual void split() override;
};

#endif //_TXTSPLITTER_H