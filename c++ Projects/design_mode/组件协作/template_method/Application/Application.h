// author : zhoukang
// date   : 2022-07-06 11:22:42

#ifndef INC_0705_APPLICATION_H
#define INC_0705_APPLICATION_H

#include "../Framework/Framework.h"

class Application : public Framework {
public:
    int val;

    explicit Application(int v);

protected:
    virtual bool step2() override;

    virtual void step4() override;
};

#endif //INC_0705_APPLICATION_H
