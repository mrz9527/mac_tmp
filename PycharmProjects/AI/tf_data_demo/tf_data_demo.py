# -*- coding: utf-8 -*-

"""
tf.data 模块学习

tf.data.Dataset.from_tensor_slices: 将numpy.ndarray数组转为tf.Tensor数组
tf.data.Dataset.repeat:             将tf.Tensor数组重复count次
tf.data.Dataset.batch:              将tf.Tensor数组按照batch_size进行打包重组
tf.data.Dataset.shuffle:            将tf.Tensor数组重新洗牌
"""

import numpy as np
import tensorflow as tf

print("\n0. input_data = np.arange(16),for循环遍历打印:")
input_data = np.arange(16)
print(input_data)

print("\n1. tf.data.Dataset.from_tensor_slices(input_data),for循环遍历打印:")
dataset = tf.data.Dataset.from_tensor_slices(input_data)
for data in dataset:
    print(data)

print("\n2. dataset = dataset.repeat(2),for循环遍历打印:")
dataset = dataset.repeat(2)
for data in dataset:
    print(data)

print("\n3. dataset = dataset.batch(4),for循环遍历打印:")
dataset = dataset.batch(4)
print(dataset)

for data in dataset:
    print(data)

print("\n4. dataset = dataset.batch(4),for循环遍历打印:")
dataset = dataset.shuffle(10)
for data in dataset:
    print(data)

print("\n455. dataset = dataset.batch(4),for循环遍历打印:")
a1 = np.arange(16).reshape(8, 2)
a2 = np.random.randint(100, size=8)
res = tf.data.Dataset.from_tensor_slices((a1, a2))
for data in res:
    print(data)
