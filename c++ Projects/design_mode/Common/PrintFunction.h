// author : zhoukang
// date   : 2022-07-06 17:50:07

#ifndef DESIGN_MODE_PRINTFUNCTION_H
#define DESIGN_MODE_PRINTFUNCTION_H

#include "string"

std::string::size_type rfindchars(const std::string &str, const std::string &chars);
std::string _GetFunctionDeclared(std::string prettyFuncon);
std::string _GetFunctionPath(std::string prettyFuncon);

#define GetFunctionPath() _GetFunctionPath(std::string(__PRETTY_FUNCTION__))
#define GetFunctionDeclared() _GetFunctionDeclared(std::string(__PRETTY_FUNCTION__))
#define PrintFunctionPath() printf("call %s()\n", GetFunctionPath().c_str())


#endif //DESIGN_MODE_PRINTFUNCTION_H
