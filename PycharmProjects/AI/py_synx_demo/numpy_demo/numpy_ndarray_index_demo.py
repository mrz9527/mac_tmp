# -*- coding: utf-8 -*-

"""
numpy.ndarray数组索引支持:
    numpy_array[][]。多个维度的索引放在多个[]中
    numpy_array[,]。多个维度的索引可以放在一个[]中.

python自带的容器, list、set、dict
    python_array[][]。多个维度的索引放在多个[]中
    python_array[,]。不支持。
"""

import numpy as np

print(__doc__)
arange_array = np.arange(30).reshape(5, 6)
print("0. arange_array = np.arange(30).reshape(5, 6), arange_array =\n", arange_array)

print("arange_array[2, 3] = ", arange_array[2, 3])
print("arange_array[2][3] = ", arange_array[2][3])

lst = [[0, 1, 2], [3, 4, 5], [6, 7, 8]]
print("1. lst = [[0, 1, 2], [3, 4, 5], [6, 7, 8]], lst=\n", lst)
print(lst[1][2])
print("error: print(lst[1, 2])")
