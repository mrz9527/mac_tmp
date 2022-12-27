// author : zhoukang
// date   : 2022-07-14 19:00:10

#ifndef DESIGN_MODE_IMSGPLATFORM_H
#define DESIGN_MODE_IMSGPLATFORM_H

#include <string>

// 与平台相关
class IMsgPlatform {
public:
    // 与平台相关接口
    virtual void PlaySound(std::string soundInfo)=0;
    virtual void Connect()=0;

    virtual ~IMsgPlatform(){};
};

#endif //DESIGN_MODE_IMSGPLATFORM_H
