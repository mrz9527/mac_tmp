// author : zhoukang
// date   : 2022-07-11 18:33:33

#ifndef DESIGN_MODE_STONEHOUSEBUILDER_H
#define DESIGN_MODE_STONEHOUSEBUILDER_H

#include "../Framework/HouseBuilder.h"

class StoneHouseBuilder : public HouseBuilder {
protected:
    virtual void BuildDoor() override;

public:
    StoneHouseBuilder();

protected:
    virtual void BuildBase() override;
    virtual void BuildWall(int index) override;
    virtual void BuildFloor() override;
};


#endif //DESIGN_MODE_STONEHOUSEBUILDER_H
