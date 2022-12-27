#include "stdio.h"
#include "Framework.h"
#include "PrintFunction.h"

void Framework::step1()
{
    PrintFunctionPath();
}

void Framework::step3()
{
    PrintFunctionPath();
}

void Framework::step5()
{
    PrintFunctionPath();
}

void Framework::run()
{
    printf("%s() start...\n", GetFunctionPath().c_str());
    step1();
    if (step2()) {
        step3();
    }
    for (int i = 0; i < 4; ++i) {
        step4();
    }
    step5();
    printf("%s() end\n", GetFunctionPath().c_str());
}