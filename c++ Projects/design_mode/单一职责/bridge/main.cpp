// author : zhoukang
// date   : 2022-07-14 18:01:48

#include "Framework/IMsgBusiness.h"
#include "Framework/IMsgPlatform.h"

#include "Application/MsgPcPlatform.h"
#include "Application/MsgBusinessPerfect.h"

int main()
{
    IMsgPlatform *msgPlatform = new MsgPcPlatform;
    IMsgBusiness *msgBusiness = new MsgBusinessPerfect(msgPlatform);

    msgBusiness->Login("zhoushiqi", "a12345678");
    msgBusiness->SendMessage("hello,world");

    return 0;
}