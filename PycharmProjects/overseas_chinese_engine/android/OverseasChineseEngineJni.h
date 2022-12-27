/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: OverseasChineseEngineJni
* Author: ZhouKang
* Create: 2021-7-12
*/
#ifndef OVERSEASCHINESEENGINE_OVERSEASCHINESEENGINEJNI_H
#define OVERSEASCHINESEENGINE_OVERSEASCHINESEENGINEJNI_H

#include "jni.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief 初始化词典
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @param sysDict 系统词典路径
 * @param predictDict 预测词典路径
 * @param keyboardType 键盘类型
 * @return true，词典初始化成功；false，词典初始化失败
 */
JNIEXPORT jboolean JNICALL
NativeInitDict(JNIEnv *env, jobject obj, jbyteArray sysDict, jbyteArray predictDict,
               jint keyboardType);

/*! @brief 卸载词典
 *
 * @param env jni环境
 * @param obj 调用者对象
 */
JNIEXPORT void JNICALL NativeClose(JNIEnv *env, jobject obj);

/*! @brief 基于键码串查询候选词，返回候选词个数
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @param inputString 键码串，这里的键码串是拉丁(latin)字符串字母，java层需要提前转化为拉丁字符串，注音的拉丁字符串必须全为大写
 * @param inputStringLen 键码串字符长度
 * @return 候选词个数
 */
JNIEXPORT jint JNICALL
NativeSearch(JNIEnv *env, jobject obj, jbyteArray inputString, jint inputStringLen);

/*! @brief 根据候选词的索引id，返回对应的候选词
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @param candidateId 候选词的索引id
 * @return 候选词，ode编码
 */
JNIEXPORT jstring JNICALL NativeGetCandidate(JNIEnv *env, jobject obj, jint candidateId);

/*! @brief 获取上屏的键码串（真实的键码串）
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @return 用于上屏的键码串，unicode编码
 */
JNIEXPORT jstring JNICALL NativeGetDisplayPinyin(JNIEnv *env, jobject obj);

/*! @brief （在拼音九键和注音九键等九键模式下，）获取九键模式左侧拼音个数，用于显示左侧候选拼音
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @return 左侧候选拼音个数
 */
JNIEXPORT jint JNICALL NativeGetCandidatePinyinCount(JNIEnv *env, jobject obj);

/*! @brief 根据左侧候选拼音的索引id，获取对应的候选拼音字符串
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @param pinyinId 左侧候选拼音的索引id
 * @return 候选拼音字符串，unicode编码
 */
JNIEXPORT jstring JNICALL NativeGetCandidatePinyin(JNIEnv *env, jobject obj, jint pinyinId);

/*! @brief 根据左侧候选拼音的索引id，选择左侧候选拼音，重新返回候选词的个数
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @param pinyinId 左侧候选拼音的索引id
 * @return 候选词个数
 */
JNIEXPORT jint JNICALL NativeChooseCandidatePinyin(JNIEnv *env, jobject obj, jint pinyinId);

/*! @brief 清空输入键码串
 *
 * @param env jni环境
 * @param obj 调用者对象
 */
JNIEXPORT void JNICALL NativeResetSearch(JNIEnv *env, jobject obj);

/*! @brief 根据历史词，返回预测词个数
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @param historyBuf 历史词，unicode编码
 * @return 预测词个数
 */
JNIEXPORT jint JNICALL NativeGetPredictsCount(JNIEnv *env, jobject obj, jstring historyBuf);

/*! @brief 根据预测词索引id，返回对应预测词
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @param predictId 预测词索引id
 * @return 预测词，unicode编码
 */
JNIEXPORT jstring JNICALL NativeGetPredicts(JNIEnv *env, jobject obj, jint predictId);

/*! @brief 生成词典
 *
 * @param env jni环境
 * @param obj 调用者对象
 * @param rawDictPath  输入词典路径
 * @param outDictPath  输出词典路径
 * @param keyboardType 键盘类型
 * @param isPredict 词典类型标志，true: 预测词; false: 系统词
 * @return
 */
JNIEXPORT jboolean JNICALL
NativeBuildDict(JNIEnv *env, jobject obj, jbyteArray rawDictPath, jbyteArray outDictPath,
          int keyboardType, jboolean isPredict);

/*!
 * 批量测试
 */
JNIEXPORT jboolean JNICALL
NativeBatchTest(JNIEnv *env, jobject obj, jbyteArray testFile, jbyteArray funcLogFile,
                jbyteArray perfLogFile, jbyteArray sysDictFile, jbyteArray predictDictFile,
                jint keyboardType, jboolean isPredict);
#ifdef __cplusplus
};
#endif

#endif // OVERSEASCHINESEENGINE_OVERSEASCHINESEENGINEJNI_H
