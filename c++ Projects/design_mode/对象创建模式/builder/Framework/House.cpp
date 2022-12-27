// author : zhoukang
// date   : 2022-07-11 18:52:47

#include "House.h"
#include "PrintFunction.h"
#include "iostream"

void House::show()
{
    std::cout << "打印房子的结构:" << std::endl;
    std::cout << "\t地基:" << base << std::endl;
    for (int i = 0; i < 4; ++i)
        std::cout << "\t墙" << i + 1 << ":" << wall[i] << std::endl;

    std::cout << "\t门:" << door << std::endl;

    std::cout << "\t房顶:" << floor << std::endl;
}

void House::setDoor(std::string doorMaterial)
{
    door = doorMaterial;
}

void House::setBase(std::string baseMaterial)
{
    base = baseMaterial;
}

void House::setWall(int index, std::string wallMaterial)
{
    wall[index] = wallMaterial;
}

void House::setFloor(std::string floorMaterial)
{
    floor = floorMaterial;
}
