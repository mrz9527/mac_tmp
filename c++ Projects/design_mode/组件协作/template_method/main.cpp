// author : zhoukang
// date   : 2022-07-06 11:26:17
#include "Application/Application.h"
#include "iostream"

int main()
{
    int val;
    std::cin >> val;
    Framework *framework = new Application(val);
    framework->run();
    return 0;
}
