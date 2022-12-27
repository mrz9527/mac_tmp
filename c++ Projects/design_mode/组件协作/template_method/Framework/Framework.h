// author : zhoukang
// date   : 2022-07-06 11:21:55

#ifndef INC_0705_LIBRARY_H
#define INC_0705_LIBRARY_H

class Framework {
public:
    virtual ~Framework()
    {};

    void run();

protected:
    void step1();

    void step3();

    void step5();

    virtual bool step2() = 0;

    virtual void step4() = 0;
};

#endif //INC_0705_LIBRARY_H
