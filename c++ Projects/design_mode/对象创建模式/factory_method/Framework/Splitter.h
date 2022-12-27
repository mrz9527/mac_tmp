/**
 * Project factory_method
 */


#ifndef _SPLITTER_H
#define _SPLITTER_H

class Splitter {
public:
    virtual ~Splitter(){};
    virtual void split() = 0;
};

#endif //_SPLITTER_H