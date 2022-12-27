// author : zhoukang
// date   : 2022-07-11 18:22:17

#ifndef DESIGN_MODE_HOUSEBUILDER_H
#define DESIGN_MODE_HOUSEBUILDER_H

#include "House.h"
class HouseBuilder {
protected:
    House* pHouse;

public:
    void Init();
    House* GetResult();


protected:
    // 房子零部件的构造与房子的成员字段相关。
    virtual void BuildDoor()=0;
    virtual void BuildBase()=0;
    virtual void BuildWall(int index)=0;
    virtual void BuildFloor()=0;
};


#endif //DESIGN_MODE_HOUSEBUILDER_H
