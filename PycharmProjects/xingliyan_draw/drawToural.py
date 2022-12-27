# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

X = np.linspace(-3, 3, 50)
Y1 = 2 * X + 1
Y2 = X ** 2

# 图像层
plt.figure(num=1)  # 图像

# 坐标轴层
plt.xlabel("x")  # 绘制坐标标签x
plt.ylabel("y")  # 绘制坐标标签y

plt.xlim((-1, 1))  # 设置x轴作图区域为(-1,1)，可以通过移动来查看x轴其他作图区域
plt.ylim((-1.5, 1.5))  # 设置y轴作图区域为(-1.5,1.5)，可以通过移动来查看y轴其他作图区域

x_ticks = np.array([-1, -0.5, 0, 0.2, 0.3, 0.8, 1])
plt.xticks(x_ticks)  # 设置x轴的刻度
plt.yticks(ticks=[-1.5, -1, -0.5, 0, 0.5, 1, 1.5],  # y刻度
           labels=[r"$badest$", r"$beder$", r"$bed$", r"$normal$", r"$good$", r"$ooder$",
                   r"$goodest$"])  # y刻度对应的文字标签，加$$是为了让字体显示为数学格式字体

# gca = get current axis"
ax = plt.gca()  # 获取坐标轴

# 坐标轴有四条骨架线（left,right,bottom,top)
# 隐藏top和right骨架线
ax.spines['right'].set_color('none')  # 设置坐标轴骨架线（图像的四条坐标轴边线），右边线为none
ax.spines['top'].set_color('none')  # 上边线为none

# 设置x轴为bottom骨架线，y轴为left骨架线
ax.xaxis.set_ticks_position('bottom')
ax.yaxis.set_ticks_position('left')

# 移动坐标轴位置（骨架线位置）
# ax.spines['bottom'].set_position(("data", 0))
# ax.spines['left'].set_position(("data", 0))

# 绘制层
xy1, = plt.plot(X, Y1, label="X-Y1")  # 绘制图形曲线X-Y1，如果要在legend中，手动输入该曲线xy1，函数返回值的"xy1,"中的逗号不能少，否则报错
xy2 = plt.plot(X, Y2, color='red', linestyle="--", label="X-Y2")  # label是曲线的名字，图例当中会默认显示曲线的名字
# plt.legend(loc="best")                                                # 显示图例，默认显示所有曲线曲线的图例
plt.legend(handles=[xy1, ], loc="best")  # 显示指定曲线的图例

plt.show()
