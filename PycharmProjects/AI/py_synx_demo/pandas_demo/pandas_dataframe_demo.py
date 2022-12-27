# -*- coding: utf-8 -*-

"""
pd.series是列数据
pd.dataframe是由多个pd.series组合的表格
"""

import pandas as pd
import numpy as np

data = {
  "mango": [420, 380, 390],
  "apple": [50, 40, 45],
  "pear": [1, 2, 3],
  "banana": [23, 45,56]
}
df = pd.DataFrame(data)
print(df)

data = {
  "calories": [420, 380, 390],
  "duration": [50, 40, 45]
}

# 数据载入到 DataFrame 对象
df = pd.DataFrame(data)
print(df)
# 返回第一行
print(df.loc[0])
# 返回第二行
print(df.loc[1])
