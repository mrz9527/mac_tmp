/**
 * Project IObserver
 */


#ifndef _SUBJECT_H
#define _SUBJECT_H

#include "list"
#include "IObserver.h"

class Subject {
public: 
    std::list<IObserver*> m_IObserverList;

public:
    virtual ~Subject(){};
    
/** 添加观察者（观察者添加订阅)
 * @param IObserver
 */
void add_IObserver(IObserver* IObserver);

/** 删除观察者(观察者取消订阅）
 * @param IObserver
 */
void remove_IObserver(IObserver* IObserver);
protected:

/** 事件发生后，通知观察者，并给观察者提供信息
 * @param IObserver
 */
virtual void notifyAll(char* log);

/** 事务处理，当状态发生变化时，调用notifyAll通知观察者
 * @param IObserver
 */

public:
virtual void process()=0;
};

#endif //_SUBJECT_H