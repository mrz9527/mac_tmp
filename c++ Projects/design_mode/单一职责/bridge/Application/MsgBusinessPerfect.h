// author : zhoukang
// date   : 2022-07-14 19:11:29

#ifndef DESIGN_MODE_MSGBUSINESSPERFECT_H
#define DESIGN_MODE_MSGBUSINESSPERFECT_H

#include "../Framework/IMsgBusiness.h"

/*******************实现业务接口: 完整版业务*********************/
class MsgBusinessPerfect: public IMsgBusiness{
public:
    MsgBusinessPerfect(IMsgPlatform *msgPlatform);

    void Login(std::string username, std::string password) override;

    void SendMessage(std::string msg) override;
};


#endif //DESIGN_MODE_MSGBUSINESSPERFECT_H
