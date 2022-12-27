// author : zhoukang
// date   : 2022-07-14 19:11:29

#include "MsgBusinessPerfect.h"

/**/
void MsgBusinessPerfect::Login(std::string username, std::string password)
{
    msgPlatform->Connect();
    printf("[完整版用户登录],username:%s   password:%s\n", username.c_str(), password.c_str());
    // ...
    msgPlatform->PlaySound("登录成功");
}

void MsgBusinessPerfect::SendMessage(std::string msg)
{
    printf("[完整版发送消息]:%s\n", msg.c_str());
    // ...
    msgPlatform->PlaySound("发送消息成功");
}

MsgBusinessPerfect::MsgBusinessPerfect(IMsgPlatform *msgPlatform) : IMsgBusiness(msgPlatform)
{}
