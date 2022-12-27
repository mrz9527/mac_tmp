// author : zhoukang
// date   : 2022-07-11 18:33:42

#include "WoodHouseBuilder.h"
#include "WoodHouse.h"
#include "PrintFunction.h"

void WoodHouseBuilder::BuildDoor()
{
    PrintFunctionPath();
    pHouse->setDoor("WoodDoor");
}

void WoodHouseBuilder::BuildBase()
{
    PrintFunctionPath();
    pHouse->setBase("WoodBase");
}

void WoodHouseBuilder::BuildWall(int index)
{
    PrintFunctionPath();
    pHouse->setWall(index, "WoodWall");
}

void WoodHouseBuilder::BuildFloor()
{
    PrintFunctionPath();
    pHouse->setFloor("WoodFloor");
}

WoodHouseBuilder::WoodHouseBuilder()
{
    pHouse = new WoodHouse;
}
