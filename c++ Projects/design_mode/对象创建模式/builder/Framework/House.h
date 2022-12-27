// author : zhoukang
// date   : 2022-07-11 18:15:22

#ifndef DESIGN_MODE_HOUSE_H
#define DESIGN_MODE_HOUSE_H

#include "string"

class House {
protected:
    std::string base;
    std::string door;
    std::string wall[4];
    std::string floor;

public:
    virtual ~House()
    {};

    // 其他行为operator
    void show();

    void setDoor(std::string doorMaterial);
    void setBase(std::string baseMaterial);
    void setWall(int index, std::string wallMaterial);
    void setFloor(std::string floorMaterial);
};


#endif //DESIGN_MODE_HOUSE_H
