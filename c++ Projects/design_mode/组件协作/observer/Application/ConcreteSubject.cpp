/**
 * Project observer
 */


#include "ConcreteSubject.h"

/**
 * ConcreteSubject implementation
 */


void ConcreteSubject::process()
{
    char log[100]{0};
    for (int i = 0; i < 100; ++i) {
        memset(log, 0, sizeof(log));
        sprintf(log, "当前进度, %d!\n", i);
        notifyAll(log);
    }
}