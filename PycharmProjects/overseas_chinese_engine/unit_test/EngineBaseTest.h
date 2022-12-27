/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: EngineBaseTest
* Author: ZhouKang
* Create: 2021-7-12
*/
#ifndef OVERSEAS_CHINESE_ENGINE_ENGINEBASETEST_H
#define OVERSEAS_CHINESE_ENGINE_ENGINEBASETEST_H

#include "gtest/gtest.h"

class EngineBaseTest : public testing::Test {
public:
    static void SetUpTestCase();

    static void TearDownTestCase();

    virtual void SetUp();

    virtual void TearDown();
};

#endif //OVERSEAS_CHINESE_ENGINE_ENGINEBASETEST_H
