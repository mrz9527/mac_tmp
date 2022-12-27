# -*- coding: utf-8 -*-

"""
mnist_pkl.gz
50000个训练样本，10000个验证样本。每个样本784个特征数据，划分为28 * 28大小
(50000, 784)
(50000,)
(10000, 784)
(10000,)
"""

import pickle  # 序列化和反序列化
import gzip  # 解压
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow.keras import layers
import numpy as np


def show_mnist_picture(pictures, labels, index):
    print("this is ", labels[index])
    plt.imshow(pictures[index].reshape(28, 28), cmap="gray")
    plt.show()


def get_mnist_data():
    mnist_data_path = "../data/minist/mnist.pkl.gz"
    with gzip.open(mnist_data_path, "rb") as f:
        res = pickle.load(f, encoding="latin-1")

    # ((x_train, y_train), (x_valid, y_valid), _) = res
    return res


((x_train, y_train), (x_valid, y_valid), _) = get_mnist_data()
# show_mnist_picture(x_train, y_train, 100)

model = tf.keras.Sequential()
model.add(layers.Dense(32, activation="relu"))
model.add(layers.Dense(32, activation="relu"))
model.add(layers.Dense(10, activation="softmax"))   # 数字识别，10个数字，属于十分类，输出层需要10个神经元

"""
tf.losses.CategoricalCrossentropy:交叉熵损失函数，适合多分类。
        使用前提：数据预处理，将标签y转化为独热编码one_hot_encode。
        这里没有使用独热编码，直接使用交叉熵损失函数，损失loss一直很大。（某一时刻，loss: 34211594.3501）
        
    尤其是分类问题，标签一定应该为独热编码，标签1和标签5的距离，应该与标签2和标签3的距离，在欧式空间中相等。
"""
# model.compile(optimizer=tf.keras.optimizers.Adam(0.001),
#               loss=tf.losses.CategoricalCrossentropy(),
#               metrics=[tf.keras.metrics.CategoricalCrossentropy()])
#
# model.fit(x_train, y_train, epochs=100, batch_size=64, validation_data=(x_valid, y_valid))


"""
tf.losses.SparseCategoricalCrossentropy:稀疏的交叉熵损失函数
    ：标签是一个数值，不需要是独热编码
"""
model.compile(optimizer=tf.keras.optimizers.Adam(0.001),
              loss=tf.losses.SparseCategoricalCrossentropy(),
              metrics=[tf.keras.metrics.SparseCategoricalAccuracy()])

model.fit(x_train, y_train, epochs=100, batch_size=64, validation_data=(x_valid, y_valid))