// author : zhoukang
// date   : 2022-07-14 19:11:17

#include "MsgBusinessLite.h"

void MsgBusinessLite::Login(std::string username, std::string password)
{
    msgPlatform->Connect();
    printf("[精简版用户登录],username:%s   password:%s\n", username.c_str(), password.c_str());
    // ...
}

void MsgBusinessLite::SendMessage(std::string msg)
{
    printf("[精简版发送消息]:%s\n", msg.c_str());
    // ...
}

MsgBusinessLite::MsgBusinessLite(IMsgPlatform *msgPlatform) : IMsgBusiness(msgPlatform)
{}
