/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: EngineBaseTest
* Author: ZhouKang
* Create: 2021-7-12
*/
#include "EngineBaseTest.h"
#include "gtest/gtest.h"
#include "OverseasChineseEngineIME.h"

void EngineBaseTest::SetUpTestCase()
{
    printf("*************all begin**********\n");

}

void EngineBaseTest::TearDownTestCase()
{
    ChineseIme::Close();
    printf("############# all end############\n");
}

void EngineBaseTest::SetUp()
{
}

void EngineBaseTest::TearDown()
{
    ChineseIme::Close();
    printf("############# test end############\n");
}

