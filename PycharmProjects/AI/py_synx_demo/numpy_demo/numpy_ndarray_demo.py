# -*- coding: utf-8 -*-

"""
numpy.empty(): 创建一个空的数组，未初始化
numpy.empty_like(): 创建一个形状和类型与指定数组相同的数组，未初始化
numpy.zeros(): 创建一个全0数组
numpy.zeros_like(): 创建一个形状和类型与指定数组相同的数组，初始化为全0
numpy.ones(): 创建一个全1数组
numpy.ones_like()
numpy.arange():创建一个等间隔的一维数组，创建的原则是[start, end), step。可以是整型数组，也可以是浮点型数组
numpy.linspace():创建一个等间隔（等差）的一维数组，创建的原则是[start, end), num。或者[start, end], num。可以是整形，也可以是浮点型

numpy.arange和linspace对比:
相同点：都可以生成等间隔数组。
不同点：
    arange是通过步长来设置等间隔，不知道生成的数组元素个数(只能通过计算得到);
    linspace是通过元素个数来设置等间隔的，不知道生成的数组的步长(只能通过计算得到);
    linspace可以包含终止点，通过参数end=True/False来指定
"""

import numpy
import numpy as np

print(__doc__)
print()
print("1. 创建数组，数组类型为numpy.ndarray:")
global_type = np.int8
global_type_str = "np.int8"
empty_array = np.empty((3, 4), dtype=global_type)
print(f"创建一个空数组(数组内容未初始化) : np.empty((3, 4), dtype={global_type_str}): empty_array =\n {empty_array}")

zero_array = np.zeros((3, 4), global_type)
print(f"创建一个全0数组(数组内容全为0) : np.zeros((3, 4), dtype={global_type_str}): zero_array =\n {zero_array}")

one_array = np.ones((3, 4), global_type)
print(f"创建一个全1数组(数组内容全为1) : np.ones((3, 4), dtype={global_type_str}): one_array = \n {one_array}")

empty_like_array = np.empty_like(one_array)
print(f"创建一个形状和类型与指定数组相同的随机数组 : np.empty_like(one_array): empty_like_array =\n {empty_like_array}")

arange_array = np.arange(0.5, 20.2, step=0.4)
print(f"创建一个等间隔的一维数组 : np.arange(0.5, 20.2, step=0.4): arange_array =\n {arange_array}")

linspace_array = np.linspace(0, 10, 10, endpoint=False)
print(f"创建一个等间隔的一维数组 : np.linspace(0, 10, 10, endpoint=False): linspace_array =\n {linspace_array}")

linspace_array = np.linspace(0, 10, 10, endpoint=True)
print(f"创建一个等间隔的一维数组 : np.linspace(0, 10, 10, endpoint=True): linspace_array =\n {linspace_array}")

linspace_array = np.linspace(1, 10, 10, endpoint=True)
print(f"创建一个等间隔的一维数组 : np.linspace(1, 10, 10, endpoint=True): linspace_array =\n {linspace_array}")
