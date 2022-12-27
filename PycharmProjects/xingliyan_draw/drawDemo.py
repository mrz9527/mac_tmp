# -*- coding: utf-8 -*-

import xlrd  # 读取xlsx
import numpy as np  # 数组
import matplotlib.pyplot as plt  # 画图

book = xlrd.open_workbook("draw.xlsx")  # xlrd 最新版2.0.1不支持xlsx格式，需要按照旧版的xlrd==1.2.0
sheet1 = book.sheets()[0]  # 按照索引获取指定sheet

nrows = sheet1.nrows  # 获取sheet行
# print(nrows)

ncols = sheet1.ncols
# print(ncols)

# 索引下标从0开始
row_index = 13
start_col = 3
end_col = 9

graph_label = sheet1.row_values(row_index, start_colx=start_col, end_colx=end_col)  # 图的label在第14行
print(graph_label)

graph_unit = sheet1.row_values(++row_index, start_colx=start_col, end_colx=end_col)  # 图的单位
print(graph_unit)

# 数据
start_row = 15
end_row = 18

dataset = []
for row_index in range(start_row, end_row + 1):
    # dataset.append(sheet1.row_values(row_index, start_colx=start_col, end_colx=end_col))
    dataset += sheet1.row_values(row_index, start_colx=start_col, end_colx=end_col)

dataArr = np.array(dataset)
print(dataArr.shape)

dataArr = dataArr.reshape(4, -1)
print(dataArr)

# f:figure, ax:axis
f, ax1 = plt.subplots(figsize=(10, 8))

ax2 = ax1.twinx()

line1 = ax1.bar(dataArr[:, 0], dataArr[:, 1], width=0.2, label=graph_label[1])

line2, = ax2.plot(dataArr[:, 0], dataArr[:, 2], marker='.', label=graph_label[2], alpha=0.7, color="grey",
                  linestyle="-")

line3, = ax2.plot(dataArr[:, 0], dataArr[:, 3], marker='.', label=graph_label[3], alpha=0.7, color="red", linestyle="-")

line4, = ax2.plot(dataArr[:, 0], dataArr[:, 4], marker='.', label=graph_label[4], alpha=0.7, color="blue",
                  linestyle="-")

ax1.set_xlabel(graph_label[0] + r":h")
ax1.set_ylabel(graph_label[1] + r":%")
ax2.set_ylabel(str(graph_label[2:]) + r":%")

# plt.legend()
plt.legend(handles=(line1,line2, line3, line4), loc="best")

plt.savefig("./draw.png")
plt.show()
