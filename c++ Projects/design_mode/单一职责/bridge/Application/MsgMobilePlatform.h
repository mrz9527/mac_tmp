// author : zhoukang
// date   : 2022-07-14 19:10:28

#ifndef DESIGN_MODE_MSGMOBILEPLATFORM_H
#define DESIGN_MODE_MSGMOBILEPLATFORM_H

#include "../Framework/IMsgPlatform.h"
class MsgMobilePlatform: public IMsgPlatform{
public:
    void PlaySound(std::string soundInfo) override;

    void Connect() override;
};


#endif //DESIGN_MODE_MSGMOBILEPLATFORM_H
