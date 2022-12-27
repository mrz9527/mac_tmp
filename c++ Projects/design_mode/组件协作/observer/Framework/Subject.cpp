/**
 * Project IObserver
 */


#include "Subject.h"

/**
 * Subject implementation
 */


/**
 * @param IObserver
 */
void Subject::add_IObserver(IObserver* IObserver) {
    m_IObserverList.push_back(IObserver);
}

void Subject::remove_IObserver(IObserver* IObserver) {
    m_IObserverList.remove(IObserver);
}

void Subject::notifyAll(char *log)
{
    auto iter = m_IObserverList.begin();
    auto end = m_IObserverList.end();

    while (iter != end) {
        (*iter)->update(log);
        ++iter;
    }
}