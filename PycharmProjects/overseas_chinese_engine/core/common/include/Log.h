/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: log
* Author: Changjie Hu
* Create: 2021-7-12
*/
#ifndef MAKE_DICT_LOG_H
#define MAKE_DICT_LOG_H

#include "BasicTypeDef.h"

BEGIN_NAMESPACE_CHINESE_IME

/*!
 * debug 模式，开启DEBUG宏定义，启用打印日志功能
 * release 模式，关闭DEBUG宏定义，关闭打印日志功能
 */
#ifdef OCE_DEBUG
#ifdef OCE_JNILOG
#include <android/log.h>
#define LOG_TAG "oceTag"
#define LOGI(format, ...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG,  \
                            "file: %s, line: %d: " format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGW(format, ...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG,  \
                            "file: %s, line: %d: " format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, \
                            "file: %s, line: %d: " format, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define LOGI(format, ...) printf("file: %s, line: %d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGW(format, ...) printf("file: %s, line: %d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGE(format, ...) printf("file: %s, line: %d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif // OCE_JNILOG
#else
#define LOGI(format, ...)
#define LOGW(format, ...)
#define LOGE(format, ...)
#endif // OCE_DEBUG

END_NAMESPACE_CHINESE_IME

#endif // MAKE_DICT_LOG_H
