// author : zhoukang
// date   : 2022-07-07 11:31:15
#include "PrintFunction.h"

std::string::size_type rfindchars(const std::string &str, const std::string &chars)
{
    auto pos = std::string::npos;
    auto tmp = std::string::npos;
    for (int i = 0; i < chars.size(); ++i) {
        tmp = str.rfind(chars[i]);
        if (tmp != std::string::npos) {
            if (pos == std::string::npos)
                pos = tmp;
            else
                pos = pos > tmp ? pos : tmp;
        }
    }
    return pos;
}

std::string _GetFunctionPath(std::string prettyFuncon)
{
    auto pos = prettyFuncon.find('(');
    if(pos!=std::string::npos)
        prettyFuncon.erase(prettyFuncon.begin()+pos, prettyFuncon.end());
    pos = rfindchars(prettyFuncon, " \t*&~");
    if(pos!=std::string::npos)
        prettyFuncon.erase(prettyFuncon.begin(), prettyFuncon.begin() + pos + 1);
    return prettyFuncon;
}

std::string _GetFunctionDeclared(std::string prettyFuncon)
{
    return prettyFuncon;
}