// author : zhoukang
// date   : 2022-07-14 19:10:17

#include "MsgPcPlatform.h"
#include "stdio.h"

void MsgPcPlatform::PlaySound(std::string soundInfo)
{
    // ...
    printf("Pc端播放音效: %s\n", soundInfo.c_str());
}

void MsgPcPlatform::Connect()
{
    // ...
    printf("Pc端网络连接\n");
}
