/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: basic type define
* Author: Changjie Hu
* Create: 2021-7-12
*/
#ifndef MAKE_DICT_BASICTYPEDEF_H
#define MAKE_DICT_BASICTYPEDEF_H

#include <cstdint>
#include <cstddef>
#include <map>

// 全局宏定义
#define BEGIN_NAMESPACE_CHINESE_IME namespace ChineseIme {
#define END_NAMESPACE_CHINESE_IME   }

BEGIN_NAMESPACE_CHINESE_IME
// 类型定义
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using char16 = char16_t;

// 定义词典头DictHeader中各个部分的字节尺寸
enum class DictHeadSize : short {
    DICT_HEADER_SIZE = 512,
    DICT_HEADER_VERSION_SIZE = 40,
    DICT_HEADER_FILESIZE_SIZE = 4,
    DICT_HEADER_BRIEF_SIZE = 212,
    DICT_HEADER_WORDITEMCOUNT_SIZE = 4
};

enum class ArrayMaxLength : unsigned {
    // 定义保存的搜索键码串的最大长度
    MAX_KEY_LENGTH = 128,
    // 定义保存的注音音调的最大长度
    MAX_TONE_LENGTH = 32,
    // 最大的预测字符数
    MAX_PREDICT_SIZE = 4
};

enum class DictType {
    DICT_TYPE_PINYIN = 0,
    DICT_TYPE_ZHUYIN,
    DICT_TYPE_CANGJIE,
    DICT_TYPE_BIHUA,

    DICT_TYPE_UNKOWN
};

// 键盘类型
enum class KeyboardType {
    KBT_PINYIN_9 = 0,   // 拼音9键
    KBT_PINYIN_26,      // 拼音26键
    KBT_ZHUYIN_9,       // 注音9键
    KBT_ZHUYIN_26,      // 注音26键
    KBT_BIHUA_JIANTI,   // 笔画简体
    KBT_BIHUA_FANTI,    // 笔画繁体
    KBT_CANGJIE,        // 仓颉
    KBT_CANGJIE_FAST,   // 快速仓颉

    KBT_UNKNOWN,        // 未知
};

// 匹配类型
enum class KeyMatchType {
    KMT_UNMATCH = 0,            // 不匹配
    KMT_COMPLETED_MATCH,        // 完全匹配
    KMT_PART_MATCH,             // 部分匹配
    KMT_COMMON_PREFIX_MATCH,    // 最长公共前缀匹配
    KMT_CONTAIN_MATCH,          // 包含匹配
    KMT_FIRST_KEY_MATCH,        // 键码串首字符匹配，属于包含匹配的特例
};

// 词典中的key1和key2
enum class DictKey {
    DICT_KEY_1,
    DICT_KEY_2,

    DICT_KEY_UNKOWN
};

using OccupyByte = uint8;
#pragma pack(push, 1)

// 类型长度，用于计算偏移
struct DictMemoryLayout {
    // 禁止直接赋值，建议使用SetMemLayout来修改
    OccupyByte ziCountOcc = 1;
    OccupyByte key1LenOcc = 0; // 未知，0或者1
    OccupyByte key2LenOcc = 0; // 未知，0或者1
    OccupyByte key1StrOcc = 0; // 未知，0或者1
    OccupyByte key2StrOcc = 0; // 未知，0或者1
    OccupyByte wordCountOcc = 2;
    OccupyByte wordStrOcc = 2;
    OccupyByte toneStrOcc = 0; // 未知，0或者1
    OccupyByte freqOcc = 2;

public:
    void Init(OccupyByte ziCount = 1, OccupyByte wordCount = 2,
            OccupyByte wordStr = 2, OccupyByte freq = 2)
    {
        ziCountOcc = ziCount;
        wordCountOcc = wordCount;
        wordStrOcc = wordStr;
        freqOcc = freq;
    }

    bool SetMemLayout(DictType dictType)
    {
        bool setFlag = false;
        if (dictType == DictType::DICT_TYPE_PINYIN) {
            SetMemLayout(true, false, true, true, false);
            setFlag = true;
        } else if (dictType == DictType::DICT_TYPE_CANGJIE) {
            SetMemLayout(true, true, true, true, false);
            setFlag = true;
        } else if (dictType == DictType::DICT_TYPE_ZHUYIN) {
            SetMemLayout(true, false, true, true, true);
            setFlag = true;
        } else if (dictType == DictType::DICT_TYPE_BIHUA) {
            SetMemLayout(false, true, false, true, false);
            setFlag = true;
        } else {
            SetMemLayout(false, false, false, false, false);
            setFlag = false;
        };
        return setFlag;
    }

private:
    void SetMemLayout(bool key1Len, bool key2Len, bool key1Str, bool key2Str, bool toneStr)
    {
        key1LenOcc = key1Len ? 1 : 0;
        key2LenOcc = key2Len ? 1 : 0;
        key1StrOcc = key1Str ? 1 : 0;
        key2StrOcc = key2Str ? 1 : 0;
        toneStrOcc = toneStr ? 1 : 0;
    };
};

#pragma pack(pop)
END_NAMESPACE_CHINESE_IME // end namespace ChineseIme
#endif // MAKE_DICT_BASICTYPEDEF_H