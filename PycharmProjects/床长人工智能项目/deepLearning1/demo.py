# -*- coding: utf-8 -*-

"""
绘制高斯曲面
"""

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

x = np.linspace(-3, 3, 1000)
y = np.linspace(-3, 3, 1000)
x, y = np.meshgrid(x, y)
w0 = 1
gaussian = np.exp(-((pow(x, 2) + pow(y, 2)) / pow(w0, 2)))

# 二维面振幅分布图
plt.figure()
plt.imshow(gaussian)

# 三维曲面振幅分布图
fig = plt.figure()
ax = Axes3D(fig)
ax.plot_surface(x, y, gaussian, cmap='jet')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.show()