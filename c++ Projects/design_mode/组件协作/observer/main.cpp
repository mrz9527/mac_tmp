// author : zhoukang
// date   : 2022-07-06 15:14:26

#include "Application/ConcreteSubject.h"
#include "Application/ConsoleLogObserver.h"
#include "Application/FileLogObserver.h"

int main()
{
   Subject* subject = new ConcreteSubject;
    IObserver* fileLogObserver = new FileLogObserver("./filelog.txt");
    IObserver* consoleLogObserver = new ConsoleLogObserver;

    subject->add_IObserver(fileLogObserver);
    subject->add_IObserver(consoleLogObserver);

    subject->process();

    return 0;
}
