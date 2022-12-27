# -*- coding: utf-8 -*-
# import tensorflow as tf

import numpy as np

a = np.array([1, 2, 3])
print(a)

a = np.array([[1, 2, 3], [3, 4, 5]])
print(a.shape)

a = np.array([1, 2, 3, 4, 5], ndmin=2)
print(a.shape)

a = np.array([1,  2,  3], dtype=complex)
print (a.shape)

#
# tf_version = tf.__version__
# # gpu_available = tf.test.is_gpu_available()
# gpu_available = tf.config.list_physical_devices('GPU')
#
# print(f"tensorflow version: {tf_version}, GPU available: {gpu_available}")
#
# a = tf.constant([1.0, 2.0], name='a')
# b = tf.constant([1.0, 3.0], name='b')
# result = tf.add(a, b, name='add')
# print("result:", result)
