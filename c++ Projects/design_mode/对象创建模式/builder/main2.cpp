// author : zhoukang
// date   : 2022-07-09 16:50:32

#include<iostream>
using namespace std;

/************************************************************************
 * 文件说明
 * 构造者模式示例
*************************************************************************/

class  Computer
{
public:
    std::string mCpu; //必须
    std::string mRam; //必须
    std::string mKeyBoard;//可选
    std::string mDisplay;//可选
    int mUsbCount;//可选
public:
    class Builder
    {
    public:
        std::string mCpu; //必须
        std::string mRam; //必须
        std::string mKeyBoard;//可选
        std::string mDisplay;//可选
        int mUsbCount;//可选
    public:
        Builder(std::string cpu,std::string ram)
        {
            this->mCpu = cpu;
            this->mRam = ram;
        }
        Builder setCpu(std::string &cpu)
        {
            this->mCpu = cpu;
            return *this;
        }
        Builder setRam(std::string ram)
        {
            this->mRam = ram;
            return *this;
        }
        Builder setKeyBoard(std::string keyboard)
        {
            this->mKeyBoard = keyboard;
            return *this;
        }
        Builder setDisplay(std::string display)
        {
            this->mDisplay = display;
            return *this;
        }
        Builder setUsbCount(int usbcount)
        {
            this->mUsbCount = usbcount;
            return *this;
        }
        Computer * build()
        {
            return new Computer(this);
        }
    };
    Computer(Builder * builder)
    {
        this->mCpu = builder->mCpu;
        this->mRam = builder->mRam;
        this->mKeyBoard = builder->mKeyBoard;
        this->mDisplay = builder->mDisplay;
        this->mUsbCount = builder->mUsbCount;
    }
    Computer(Computer * computer)
    {
        this->mCpu = computer->mCpu;
        this->mRam = computer->mRam;
        this->mKeyBoard = computer->mKeyBoard;
        this->mDisplay = computer->mDisplay;
        this->mUsbCount = computer->mUsbCount;
    }
public:
    void display()
    {
        std::cout << "###################" <<std::endl;
        std::cout << "mCpu = " << mCpu <<std::endl;
        std::cout << "mRam = " << mRam <<std::endl;
        std::cout << "mKeyBoard = " << mKeyBoard << std::endl;
        std::cout << "mDisplay = " << mDisplay  << std::endl;
        std::cout << "mUsbCount = " << mUsbCount << std::endl;
        std::cout << "###################" <<std::endl;
    }
};
int main(int argc,char * argv[])
{
    Computer * computer = new Computer(Computer::Builder("I3","Inter")
                                               .setDisplay("32寸")
                                               .setKeyBoard("双飞燕")
                                               .setUsbCount(2).build());
    computer->display();
    Computer * computer_1 = new Computer(Computer::Builder("None","ARM")
                                                 .setKeyBoard("Rk")
                                                 .setUsbCount(1).build());
    computer_1->display();
    return 0;
}


