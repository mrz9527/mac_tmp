// author : zhoukang
// date   : 2022-07-09 16:50:32

#include<iostream>
#include "Application/StoneHouseBuilder.h"
#include "Framework/House.h"

int main(int argc, char *argv[])
{
    std::cout << "Material:Stone" << std::endl;
    // 构造house
    StoneHouseBuilder *houseBuilder = new StoneHouseBuilder;
    houseBuilder->Init();
    House *house = houseBuilder->GetResult();

    house->show();

    return 0;
}


