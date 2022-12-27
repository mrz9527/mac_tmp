# -*- coding: utf-8 -*-
import numpy as np

# 自定义dtype类型，这里相当于定义了一个结构体，结构体有三个字段
dt = np.dtype([("name", 'S20'), ("age", np.int8), ("score", np.float32)])
print(dt)
print(type(dt))

value = np.array([("zhangsan", 18, 98.5), ("lisi", 17, 98), ("wangwu", 20, 100)], dtype=dt)
print(value)
print(value.shape)
print(value['name'])
