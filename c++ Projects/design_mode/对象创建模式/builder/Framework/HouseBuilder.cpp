// author : zhoukang
// date   : 2022-07-11 18:22:17

#include "HouseBuilder.h"

void HouseBuilder::Init()
{
    // 构建地基
    BuildBase();
    // 构建四面墙
    for (int i = 0; i < 4; ++i) {
        BuildWall(i);
    }
    // 构建门
    BuildDoor();
    // 构建房顶
    BuildFloor();
}

House *HouseBuilder::GetResult()
{
    return pHouse;
}