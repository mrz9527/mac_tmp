# 组件协作模式
template method 模板方法模式
strategy    策略模式
observer/event    观察者模式/事件模式

组件协作模式解决的是框架和应用之间的问题。
一般框架是稳定的，应用时变化的。
采用的是晚绑定的方式。

##template method

###解决的问题
主框架流程不变，子步骤可能变化。

###分析
稳定的部分：
主框架整体流程稳定。可以打包成编译成so。
变化的部分：
不同的用户，可能实现不同的子步骤。

###设计模式特点
无论应用层怎么实现接口，框架层不变。

###实现细节
框架层：
不变的流程和步骤，用普通函数实现；
变化的步骤，用虚函数的方式提供接口，供应用层来实现。
析构函数必须是虚函数。

应用层：
继承框架类，并实现虚函数接口。

主函数main：
Library* lib = new Application;
lib->run();

## strategy

## observer/event
###解决的问题
通知依赖关系，一个目标对象的状态发生变化，所有的依赖对象（依赖目标）都将得到通知。
案例：一个模块用于事务处理，将事务的日志信息以不同的方式输出。比如输出到文件或者到控制台，或者到窗口中。

###分析
稳定的部分：
事务的处理，也就是目标对象
变化的部分：
不同的输出方式，也就是观察者。给一个IObserver接口类，提供了更新机制。每个观察者都继承这个接口类并实现接口。

###实现细节
框架层
抽象类 Subject 目标对象
    普通函数：添加、删除和通知观察者的方式是稳定的。
    虚函数：subject的事务处理是可变的。

接口类 IObserver 观察者接口
    虚函数：每个观察者对观察到的事件处理的方式不一样

应用层
具体类 ConcreteSubject 具体目标对象
    实现Subject的虚函数，用于处理具体的事务

具体类 ConcreteObserver（FileLogObserver\ConsoleLogObserver\FormLogObserver 具体观察者
    实现更新函数

主函数
Subject* sub = new ConcreteSubject;
IObserver* fileLogObserver = new FileLogObserver;
IObserver* consoleLogObserver = new ConsoleLogObserver;
sub->add(fileLogObserver);
sub->add(consoleLogObserver);

