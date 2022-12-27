/**
 * Project observer
 */


#ifndef _CONCRETESUBJECT_H
#define _CONCRETESUBJECT_H

#include "../Framework/Subject.h"

class ConcreteSubject : public Subject {
public:
    virtual void process() override;
};

#endif //_CONCRETESUBJECT_H