// author : zhoukang
// date   : 2022-07-21 18:59:36

#ifndef ALGORITHMS_NAMEAGE_H
#define ALGORITHMS_NAMEAGE_H

#include "string"

using NameAge = std::pair<std::string, int>;

// 优先按照年龄second比较，再按照姓名first比较
class LessCompareNameAge {
public:
    bool operator()(const NameAge &left, const NameAge &right) const
    {
        return (left.second < right.second) || (!(right.second < left.second) && left.first < right.first);
    }
};

class GreaterCompareNameAge {
public:
    bool operator()(NameAge &left, NameAge &right)
    {
        return left.second > right.second || (!(right.second > left.second) && left.first > right.first);
    }
};

void PrintNameAge(const NameAge &elem);

#endif //ALGORITHMS_NAMEAGE_H
