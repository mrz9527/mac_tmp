// author : zhoukang
// date   : 2022-07-06 17:38:00

#include "Splitter.h"
#include "ISplitterFactory.h"

/* 设计模式不是为了消除变化，而是为了隔离变化。变化肯定是存在的。
 * */
class SplitProcess{
public:
    ISplitterFactory* m_factory;

    // 通过外界传入factroy类型。factory实例化的位置肯定是具体的，是变化的，将变化的位置放入外界
    // 虽然没有消除变化，但是隔离了变化，从而使得SplitProcess是稳定的
    SplitProcess(ISplitterFactory* factory):m_factory(factory){};

    void process()
    {
        Splitter* splitter  // 通过声明抽象类的方式来适配对象的声明
            = m_factory->createInstance();  // 通过抽象的工厂接口类的方式来适配对象的创建
        splitter->split();  // 通过抽象的方式来适配对象的方法的调用

        delete splitter;
        splitter = nullptr;
    }
};