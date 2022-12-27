"""
第一个人工智能程序！用它可以识别出一张图片中是否有猫。
""""""

import numpy as np # numpy是科学计算工具库，这里用来进行向量化
import matplotlib.pyplot as plt # 这个库是用来画图的

import h5py # 这个库是用来加载训练数据集的。我们数据集的保存格式是HDF。Hierarchical Data Format(HDF)是一种针对大量数据进行组织和存储的
            #  文件格式,大数据行业和人工智能行业都用它来保存数据。
import skimage.transform as tf # 这里我们用它来缩放图片

#这是jupyter notebook里的命令, 意思是将那些用matplotlib绘制的图显示在页面里而不是弹出一个窗口
%matplotlib inline 