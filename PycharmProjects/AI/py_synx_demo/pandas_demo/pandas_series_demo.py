# -*- coding: utf-8 -*-

import pandas as pd

"""
list方式创建series
a = ["google", "baidu", "360"]
pd.Series(a) :   按列排布。且自动添加索引(默认从0开始）
0    google
1     baidu
2       360
"""

print("基于list，pd.Series的构建:")
a = ["google", "baidu", "360"]
# my_series = pd.Series(a, index=['x', 'y', 'z'])
my_series = pd.Series(a)
print(my_series)

"""
字典方式创建series。
字典的key成为series的索引，value成为series的列数据
"""
print("基于dict，pd.Series的构建:")
mydict = {1: "张三", 2: "李四", 5: "王五"}
my_series = pd.Series(mydict)
print(my_series)


"""
获取series指定索引的列数据
"""
print("\npd.Series元素的获取:")
y_value = my_series[2]
print("my_series['y'] = ", y_value)
