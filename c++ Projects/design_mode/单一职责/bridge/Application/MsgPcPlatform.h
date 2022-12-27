// author : zhoukang
// date   : 2022-07-14 19:10:17

#ifndef DESIGN_MODE_MSGPCPLATFORM_H
#define DESIGN_MODE_MSGPCPLATFORM_H

#include "../Framework/IMsgPlatform.h"
class MsgPcPlatform: public IMsgPlatform {
public:
    void PlaySound(std::string soundInfo) override;

    void Connect() override;
};


#endif //DESIGN_MODE_MSGPCPLATFORM_H
