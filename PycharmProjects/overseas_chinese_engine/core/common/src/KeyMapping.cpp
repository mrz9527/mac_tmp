/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: key map
* Author: ZhouKang
* Create: 2021-7-12
*/
#include <map>

#include "KeyMapping.h"

BEGIN_NAMESPACE_CHINESE_IME
const std::map<char, char16> KeyMapping::bihuaMap = {
    std::map<char, char16>::value_type('1', u'一'),
    std::map<char, char16>::value_type('2', u'丨'),
    std::map<char, char16>::value_type('3', u'丿'),
    std::map<char, char16>::value_type('4', u'丶'),
    std::map<char, char16>::value_type('5', u'乛'),
    std::map<char, char16>::value_type('6', u'*')
};

const std::map<char, char16>KeyMapping::cangjieMap = {
    std::map<char, char16>::value_type('m', u'一'),
    std::map<char, char16>::value_type('l', u'中'),
    std::map<char, char16>::value_type('o', u'人'),
    std::map<char, char16>::value_type('j', u'十'),
    std::map<char, char16>::value_type('y', u'卜'),
    std::map<char, char16>::value_type('r', u'口'),
    std::map<char, char16>::value_type('g', u'土'),
    std::map<char, char16>::value_type('k', u'大'),
    std::map<char, char16>::value_type('v', u'女'),
    std::map<char, char16>::value_type('s', u'尸'),
    std::map<char, char16>::value_type('u', u'山'),
    std::map<char, char16>::value_type('t', u'廿'),
    std::map<char, char16>::value_type('n', u'弓'),
    std::map<char, char16>::value_type('p', u'心'),
    std::map<char, char16>::value_type('i', u'戈'),
    std::map<char, char16>::value_type('q', u'手'),
    std::map<char, char16>::value_type('a', u'日'),
    std::map<char, char16>::value_type('b', u'月'),
    std::map<char, char16>::value_type('d', u'木'),
    std::map<char, char16>::value_type('e', u'水'),
    std::map<char, char16>::value_type('f', u'火'),
    std::map<char, char16>::value_type('w', u'田'),
    std::map<char, char16>::value_type('h', u'竹'),
    std::map<char, char16>::value_type('c', u'金'),
    std::map<char, char16>::value_type('x', u'難'),
    std::map<char, char16>::value_type('z', u'重')
};

const std::map<char, char16>KeyMapping::zhuyinMap = {
    std::map<char, char16>::value_type('U', u'_'),
    std::map<char, char16>::value_type('W', u'ˇ'),
    std::map<char, char16>::value_type('V', u'ˊ'),
    std::map<char, char16>::value_type('X', u'ˋ'),
    std::map<char, char16>::value_type('Y', u'˙'),
    std::map<char, char16>::value_type('0', u'ㄅ'),
    std::map<char, char16>::value_type('1', u'ㄆ'),
    std::map<char, char16>::value_type('2', u'ㄇ'),
    std::map<char, char16>::value_type('3', u'ㄈ'),
    std::map<char, char16>::value_type('4', u'ㄉ'),
    std::map<char, char16>::value_type('5', u'ㄊ'),
    std::map<char, char16>::value_type('6', u'ㄋ'),
    std::map<char, char16>::value_type('7', u'ㄌ'),
    std::map<char, char16>::value_type('8', u'ㄍ'),
    std::map<char, char16>::value_type('9', u'ㄎ'),
    std::map<char, char16>::value_type(':', u'ㄏ'),
    std::map<char, char16>::value_type(';', u'ㄐ'),
    std::map<char, char16>::value_type('<', u'ㄑ'),
    std::map<char, char16>::value_type('=', u'ㄒ'),
    std::map<char, char16>::value_type('>', u'ㄓ'),
    std::map<char, char16>::value_type('?', u'ㄔ'),
    std::map<char, char16>::value_type('@', u'ㄕ'),
    std::map<char, char16>::value_type('A', u'ㄖ'),
    std::map<char, char16>::value_type('B', u'ㄗ'),
    std::map<char, char16>::value_type('C', u'ㄘ'),
    std::map<char, char16>::value_type('D', u'ㄙ'),
    std::map<char, char16>::value_type('E', u'ㄚ'),
    std::map<char, char16>::value_type('F', u'ㄛ'),
    std::map<char, char16>::value_type('G', u'ㄜ'),
    std::map<char, char16>::value_type('H', u'ㄝ'),
    std::map<char, char16>::value_type('I', u'ㄞ'),
    std::map<char, char16>::value_type('J', u'ㄟ'),
    std::map<char, char16>::value_type('K', u'ㄠ'),
    std::map<char, char16>::value_type('L', u'ㄡ'),
    std::map<char, char16>::value_type('M', u'ㄢ'),
    std::map<char, char16>::value_type('N', u'ㄣ'),
    std::map<char, char16>::value_type('O', u'ㄤ'),
    std::map<char, char16>::value_type('P', u'ㄥ'),
    std::map<char, char16>::value_type('Q', u'ㄦ'),
    std::map<char, char16>::value_type('R', u'ㄧ'),
    std::map<char, char16>::value_type('S', u'ㄨ'),
    std::map<char, char16>::value_type('T', u'ㄩ')
};

END_NAMESPACE_CHINESE_IME
