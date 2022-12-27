/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: OverseasChineseEngineJni
* Author: ZhouKang
* Create: 2021-7-12
*/
#include "OverseasChineseEngineJni.h"
#include "OverseasChineseEngineIME.h"
#include "Utf16Char.h"
#include "BasicTypeDef.h"
#include "securecutil.h"

namespace {
    const size_t CANDBUFLEN = 256;     // 一个候选词的最大长度
    const size_t INPUTPINYINLEN = 256; // 用户界面输入的最大字符长度
    const size_t PINYINLEN = 8; // 字的拼音串或者注音串等的长度
    const size_t MAX_PATH_LENGTH = 1024;
}

jboolean NativeInitDict(JNIEnv *env, jobject obj, jbyteArray sysDict, jbyteArray predictDict, jint keyboardType)
{
    jbyte *sd = env->GetByteArrayElements(sysDict, 0);
    jbyte *pd = env->GetByteArrayElements(predictDict, 0);

    jsize sdLen = env->GetArrayLength(sysDict);
    jsize pdLen = env->GetArrayLength(predictDict);

    char sysDictPath[MAX_PATH_LENGTH] = {0}; // 系统词路径
    char predictDictPath[MAX_PATH_LENGTH] = {0}; // 下一词路径

    memcpy_s((void *) sysDictPath, MAX_PATH_LENGTH, (void *) sd, (size_t) sdLen);
    sysDictPath[sdLen] = 0;

    memcpy_s((void *) predictDictPath, MAX_PATH_LENGTH, (void *) pd, (size_t) pdLen);
    predictDictPath[pdLen] = 0;

    env->ReleaseByteArrayElements(sysDict, sd, 0);
    env->ReleaseByteArrayElements(predictDict, pd, 0);

    return ChineseIme::InitDict(sysDictPath, predictDictPath, (ChineseIme::KeyboardType) keyboardType);
}

void NativeClose(JNIEnv *env, jobject obj)
{
    ChineseIme::Close();
}

jint NativeSearch(JNIEnv *env, jobject obj, jbyteArray inputString, jint inputStringLen)
{
    jbyte *inputStr = env->GetByteArrayElements(inputString, 0);

    jint ret = (jint) ChineseIme::Search((const char *) inputStr, (size_t) inputStringLen);

    env->ReleaseByteArrayElements(inputString, inputStr, 0);

    return ret;
}

jstring NativeGetCandidate(JNIEnv *env, jobject obj, jint candidateId)
{
    jstring candStr;
    char16_t buf[CANDBUFLEN] = {0};
    if (ChineseIme::GetCandidate((size_t) candidateId, buf, CANDBUFLEN)) {
        candStr = env->NewString((unsigned short *) buf, ChineseIme::Utf16Strlen(&buf[0]));
    } else {
        candStr = env->NewString((unsigned short *) buf, 0);
    }

    return candStr;
}

jstring NativeGetDisplayPinyin(JNIEnv *env, jobject obj)
{
    jstring pinyinStr;
    char16_t buf[INPUTPINYINLEN] = {0};
    if (ChineseIme::GetDisplayPinyin(buf, INPUTPINYINLEN)) {
        pinyinStr = env->NewString((unsigned short *) buf, ChineseIme::Utf16Strlen(&buf[0]));
    } else {
        pinyinStr = env->NewString((unsigned short *) buf, 0);
    }
    return pinyinStr;
}

jint NativeGetCandidatePinyinCount(JNIEnv *env, jobject obj)
{
    return (jint) ChineseIme::GetCandidatePinyinCount();
}

jstring NativeGetCandidatePinyin(JNIEnv *env, jobject obj, jint pinyinId)
{
    jstring pinyinStr;
    char16_t buf[PINYINLEN] = {0};
    if (ChineseIme::GetCandidatePinyin((size_t) pinyinId, buf, PINYINLEN)) {
        pinyinStr = env->NewString((unsigned short *) buf, ChineseIme::Utf16Strlen(&buf[0]));
    } else {
        pinyinStr = env->NewString((unsigned short *) buf, 0);
    }
    return pinyinStr;
}

jint NativeChooseCandidatePinyin(JNIEnv *env, jobject obj, jint pinyinId)
{
    return (jint) ChineseIme::ChooseCandidatePinyin((size_t) pinyinId);
}

void NativeResetSearch(JNIEnv *enc, jobject obj)
{
    ChineseIme::ResetSearch();
}

jint NativeGetPredictsCount(JNIEnv *env, jobject obj, jstring historyBuf)
{
    const jchar *buf = env->GetStringChars(historyBuf, 0);
    jsize bufLen = env->GetStringLength(historyBuf);

    jint ret = ChineseIme::GetPredictsCount((const char16_t *) buf, (size_t) bufLen);
    env->ReleaseStringChars(historyBuf, buf);
    return ret;
}

jstring NativeGetPredicts(JNIEnv *env, jobject obj, jint predictId)
{
    jstring predictStr;
    char16_t buf[static_cast<unsigned>(ChineseIme::ArrayMaxLength::MAX_PREDICT_SIZE)] = {0};
    if (ChineseIme::GetPredicts((size_t) predictId, buf,
                                static_cast<unsigned>(ChineseIme::ArrayMaxLength::MAX_PREDICT_SIZE))) {
        predictStr = env->NewString((unsigned short *) buf, ChineseIme::Utf16Strlen(&buf[0]));
    } else {
        predictStr = env->NewString((unsigned short *) buf, 0);
    }
    return predictStr;
}

jboolean NativeBuildDict(JNIEnv *env, jobject obj, jbyteArray rawDictPath, jbyteArray outDictPath,
                         int keyboardType, jboolean isPredict)
{
    jboolean jret = JNI_TRUE;
#ifdef ___BUILD_MODEL___
    jbyte *rawDict = env->GetByteArrayElements(rawDictPath, 0);
    jbyte *outDict = env->GetByteArrayElements(outDictPath, 0);

    jsize rawDictLen = env->GetArrayLength(rawDictPath);
    jsize outDictLen = env->GetArrayLength(outDictPath);

    char rawDictBuf[MAX_PATH_LENGTH] = {0};
    char outDictBuf[MAX_PATH_LENGTH] = {0};

    memcpy_s((void*) rawDictBuf,(size_t)rawDictLen, (void*) rawDict, (size_t)rawDictLen);
    memcpy_s((void*) outDictBuf,(size_t)outDictLen, (void*) outDict, (size_t)outDictLen);

    env->ReleaseByteArrayElements(rawDictPath, rawDict, 0);
    env->ReleaseByteArrayElements(outDictPath, outDict, 0);

    jret = ChineseIme::BuildDict(rawDictBuf, outDictBuf, (ChineseIme::KeyboardType) keyboardType, isPredict);
#endif
    return jret;
}

jboolean NativeBatchTest(JNIEnv *env, jobject obj, jbyteArray testFile, jbyteArray funcLogFile,
                         jbyteArray perfLogFile, jbyteArray sysDictFile, jbyteArray predictDictFile,
                         jint keyboardType, jboolean isPredict)
{
#ifdef OCE_DEBUG
    jbyte *str1 = env->GetByteArrayElements(testFile, 0);
    jbyte *str2 = env->GetByteArrayElements(funcLogFile, 0);
    jbyte *str3 = env->GetByteArrayElements(perfLogFile, 0);
    jbyte *str4 = env->GetByteArrayElements(sysDictFile, 0);
    jbyte *str5 = env->GetByteArrayElements(predictDictFile, 0);

    jsize len1 = env->GetArrayLength(testFile);
    jsize len2 = env->GetArrayLength(funcLogFile);
    jsize len3 = env->GetArrayLength(perfLogFile);
    jsize len4 = env->GetArrayLength(sysDictFile);
    jsize len5 = env->GetArrayLength(predictDictFile);

    char testFilePath[MAX_PATH_LENGTH] = {0};
    char funcLogFilePath[MAX_PATH_LENGTH] = {0};
    char perfLogFilePath[MAX_PATH_LENGTH] = {0};
    char sysDictFilePath[MAX_PATH_LENGTH] = {0};
    char predictDictFilePath[MAX_PATH_LENGTH] = {0};

    memcpy_s((void *) testFilePath, (size_t) len1, (void *) str1, (size_t) len1);
    memcpy_s((void *) funcLogFilePath, (size_t) len2, (void *) str2, (size_t) len2);
    memcpy_s((void *) perfLogFilePath, (size_t) len3, (void *) str3, (size_t) len3);
    memcpy_s((void *) sysDictFilePath, (size_t) len4, (void *) str4, (size_t) len4);
    memcpy_s((void *) predictDictFilePath, (size_t) len5, (void *) str5, (size_t) len5);

    env->ReleaseByteArrayElements(testFile, str1, 0);
    env->ReleaseByteArrayElements(funcLogFile, str2, 0);
    env->ReleaseByteArrayElements(perfLogFile, str3, 0);
    env->ReleaseByteArrayElements(sysDictFile, str4, 0);
    env->ReleaseByteArrayElements(predictDictFile, str5, 0);

    ChineseIme::RunTestBatch(testFilePath, funcLogFilePath, perfLogFilePath, sysDictFilePath,
                             predictDictFilePath, (ChineseIme::KeyboardType) keyboardType,
                             isPredict);
#endif
    return JNI_TRUE;
}

static JNINativeMethod gMethods[] = {
    {"initDict",                "([B[BI)Z",              (void *) NativeInitDict},
    {"close",                   "()V",                   (void *) NativeClose},
    {"search",                  "([BI)I",                (void *) NativeSearch},
    {"getCandidate",            "(I)Ljava/lang/String;", (void *) NativeGetCandidate},
    {"getDisplayPinyin",        "()Ljava/lang/String;",  (void *) NativeGetDisplayPinyin},
    {"getCandidatePinyinCount", "()I",                   (void *) NativeGetCandidatePinyinCount},
    {"getCandidatePinyin",      "(I)Ljava/lang/String;", (void *) NativeGetCandidatePinyin},
    {"chooseCandidatePinyin",   "(I)I",                  (void *) NativeChooseCandidatePinyin},
    {"resetSearch",             "()V",                   (void *) NativeResetSearch},
    {"getPredictsCount",        "(Ljava/lang/String;)I", (void *) NativeGetPredictsCount},
    {"getPredicts",             "(I)Ljava/lang/String;", (void *) NativeGetPredicts},
    {"batchTest",               "([B[B[B[B[BIZ)Z",       (void *) NativeBatchTest},
    {"buildDict",               "([B[BIZ)Z",             (void *) NativeBuildDict}
};

static bool RegisterNativeMethods(JNIEnv *env)
{
    bool ret = false;

    jclass cls = env->FindClass("com/android/inputmethod/zh/engine/OverseasChineseEngineServices");
    if (cls == NULL) {
        return ret;
    }

    if (env->RegisterNatives(cls, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) == 0) {
        ret = true;
    }

    env->DeleteLocalRef(cls);

    return ret;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env = nullptr;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    if (env == nullptr) {
        return result;
    }

    if (RegisterNativeMethods(env) == false) {
        return result;
    }
    return JNI_VERSION_1_4;
}
