// author : zhoukang
// date   : 2022-07-14 18:59:23

#ifndef DESIGN_MODE_IMSGBUSINESS_H
#define DESIGN_MODE_IMSGBUSINESS_H

#include "string"
#include "IMsgPlatform.h"

// 与业务相关
// 需要先实现平台相关接口，才能再实现业务相关
// 因为业务接口中会调用具体与平台相关的接口
class IMsgBusiness {
public:
    IMsgPlatform* msgPlatform;

    IMsgBusiness(IMsgPlatform *msgPlatform) : msgPlatform(msgPlatform)
    {};

    // 与业务相关接口
    virtual void Login(std::string username, std::string password)=0;
    virtual void SendMessage(std::string msg)=0;

    virtual ~IMsgBusiness(){};
};

#endif //DESIGN_MODE_IMSGBUSINESS_H
