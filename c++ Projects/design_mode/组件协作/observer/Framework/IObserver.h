/**
 * Project observer
 */


#ifndef _OBSERVER_H
#define _OBSERVER_H

// 观察者是个接口类
class IObserver {
public:
    virtual ~IObserver(){};

/** 事件发生后，观察者回调
 */
    virtual void update(char* log) = 0;
};

#endif //_OBSERVER_H