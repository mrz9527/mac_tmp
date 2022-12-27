// author : zhoukang
// date   : 2022-07-11 18:33:42

#ifndef DESIGN_MODE_WOODHOUSEBUILDER_H
#define DESIGN_MODE_WOODHOUSEBUILDER_H

#include "../Framework/HouseBuilder.h"

class WoodHouseBuilder : public HouseBuilder {
protected:
public:
    WoodHouseBuilder();

protected:
    void BuildDoor() override;

    void BuildBase() override;

    void BuildWall(int index) override;

    void BuildFloor() override;
};


#endif //DESIGN_MODE_WOODHOUSEBUILDER_H
