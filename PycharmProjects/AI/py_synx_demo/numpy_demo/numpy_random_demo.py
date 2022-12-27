# -*- coding: utf-8 -*-

"""
np.random.random	生成0到1之间的随机数
np.random.uniform	生成均勻分布的随机数
np.random.randn	    生成标准正态的随机数
np.random.randint	生成随机的整数
np.random.normal	生成正态分布
np.random.shuffle	随机打乱顺序
np.random.seed	    设置随机数种子
numpy.random.rand   生成随机的浮点数

numpy.random.choice 从一维数组中随机元素来组成随机数组
"""
import numpy as np

print("---学习numpy.random模块---\n")
print(__doc__)
normal_distribute_array = np.random.randn(3, 4)
print(
    f"创建一个正态分布数组 : normal_distribute_array = np.random.randn(3, 4): normal_distribute_array =\n {normal_distribute_array}")
print(normal_distribute_array.dtype)

rand_int = np.random.randint(100)
print(f"创建一个随机整数，范围在[0, 100)之间,np.random.randint(100): rand_int =\n {rand_int}")

rand_int_array = np.random.randint(100, size=(3, 4))
print(f"创建一个随机整数数组，范围在[0, 100)之间,np.random.randint(100, size=(3, 4)): rand_int_array =\n {rand_int_array}")

rand_float = np.random.rand()
print(f"创建一个随机浮点数, np.random.rand(): rand_float =\n {rand_float}")

rand_float_array = np.random.rand(3, 4)
print(f"创建随机浮点数数组, np.random.rand(3, 4): rand_float_array =\n {rand_float_array}")

rand_int_array = rand_int_array.reshape(-1)
rand_choice = np.random.choice(rand_int_array)
print(f"从数组中随机选择一个数, np.random.choice(rand_int_array): rand_choice =\n {rand_choice}")

rand_choice_array = np.random.choice(rand_int_array, size=(3, 4))
print(
    f"从数组中随机选择元素来组成随机数组(未初始化), np.random.choice(rand_int_array, size=(3,4)): rand_choice_array =\n {rand_choice_array}")
