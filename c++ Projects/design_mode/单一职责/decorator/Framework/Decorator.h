/**
 * Project Untitled
 */


#ifndef _DECORATOR_H
#define _DECORATOR_H

#include "Stream.h"

/*
    装饰器模式的特点：继承和组合同时使用
    继承目的：是为了继承接口，使得功能扩展类也能具有Stream中的函数接口
    组合目的：是为了使用具体类的功能，比如stream为FileStream*类型，可以直接使用strem->read()，调用的就是文件流的读函数
**/
class Decorator: public Stream {
protected: 
    Stream* stream;
public:
    Decorator(Stream *stream);
};

#endif //_DECORATOR_H