// author : zhoukang
// date   : 2022-07-11 18:33:33

#include "StoneHouseBuilder.h"
#include "PrintFunction.h"
#include "StoneHouse.h"

void StoneHouseBuilder::BuildDoor()
{
    PrintFunctionPath();
    // .... 构造门
    pHouse->setDoor("StoneDoor");
}

void StoneHouseBuilder::BuildBase()
{
    PrintFunctionPath();
    pHouse->setBase("StoneBase");
}

void StoneHouseBuilder::BuildWall(int index)
{
    PrintFunctionPath();
    pHouse->setWall(index, "StoneWall");
}

void StoneHouseBuilder::BuildFloor()
{
    PrintFunctionPath();
    pHouse->setFloor("StoneFloor");
}

StoneHouseBuilder::StoneHouseBuilder()
{
    pHouse = new StoneHouse;
}
