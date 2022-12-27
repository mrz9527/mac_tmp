// author : zhoukang
// date   : 2022-07-14 19:10:28

#include "MsgMobilePlatform.h"
#include "stdio.h"
#include "PrintFunction.h"

void MsgMobilePlatform::PlaySound(std::string soundInfo)
{
    PrintFunctionPath();
    // ...
    printf("移动端播放音效: %s\n", soundInfo.c_str());
}

void MsgMobilePlatform::Connect()
{
    PrintFunctionPath();
    // ...
    printf("移动端网络连接\n");
}
