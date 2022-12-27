/**
 * Project observer
 */


#include "ConsoleLogObserver.h"

#include "iostream"

/**
 * ConsoleLogObserver implementation
 */
void ConsoleLogObserver::update(char *log)
{
    std::cout << log << std::endl;
}
