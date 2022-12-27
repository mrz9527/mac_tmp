// author : zhoukang
// date   : 2022-07-06 11:19:09
#include "Application.h"
#include "PrintFunction.h"

Application::Application(int v) : val(v)
{}

bool Application::step2()
{
    PrintFunctionPath();
    return val > 0;
}

void Application::step4()
{
    PrintFunctionPath();
}

