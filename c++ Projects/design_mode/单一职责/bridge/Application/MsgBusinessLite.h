// author : zhoukang
// date   : 2022-07-14 19:11:17

#ifndef DESIGN_MODE_MSGBUSINESSLITE_H
#define DESIGN_MODE_MSGBUSINESSLITE_H

#include "../Framework/IMsgBusiness.h"
class MsgBusinessLite: public IMsgBusiness{
public:
    MsgBusinessLite(IMsgPlatform *msgPlatform);

    void Login(std::string username, std::string password) override;

    void SendMessage(std::string msg) override;
};


#endif //DESIGN_MODE_MSGBUSINESSLITE_H
