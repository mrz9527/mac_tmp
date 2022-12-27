# -*- coding: utf-8 -*-

"""
tf.keras.datasets   包含了深度学习常见的公开数据集。
        包括 mnist 、 fashion_mnist 、 cifar10 、 cifar100等

    mnist: 包含手写数字的10个类别。分别是0~9
    fashion_mnist: 包含服装的10个类别。class_names = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat', 'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']

"""
import os

import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import matplotlib.pyplot as plt


def path_exist(filepath):
    return os.path.exists(filepath)


def set_numpy_print_options():
    # threshold=np.inf：用于设置全部打印，不要用省略号代替
    # linewidth=1000：用于设置一行显示的宽度，避免一行数据太多而换行
    # suppress=True: 不要用科学计算法来打印数据，默认使用科学计数法。
    np.set_printoptions(threshold=np.inf, linewidth=1000, suppress=True)


def plot_image(i, predictions_array, true_labels, imgs, class_names):
    true_label, img = true_labels[i], imgs[i]
    plt.grid(False)
    plt.xticks([])
    plt.yticks([])

    plt.imshow(img, cmap=plt.cm.binary)

    predicted_label = np.argmax(predictions_array)
    if predicted_label == true_label:
        color = 'blue'
    else:
        color = 'red'

    font = {'family': 'Times New Roman',
            'weight': 'normal',
            'size': 10,
            }

    plt.xlabel("{} {:2.0f}% ({})".format(class_names[predicted_label], 100 * np.max(predictions_array),
                                         class_names[true_label]), color=color, fontdict=font)


def plot_value_array(i, predictions_array, true_labels):
    true_label = true_labels[i]
    plt.grid(False)

    plt.xticks(range(10))
    plt.yticks([])
    plot = plt.bar(range(10), predictions_array, color="#777777")
    plt.ylim([0, 1])
    predicted_label = np.argmax(predictions_array)
    plot[predicted_label].set_color('red')
    plot[true_label].set_color('blue')


def plot_one_group(row_count, col_count, i, predicts, test_x, test_y, class_names):
    # i = 0  (1,2)  i = 1 (3,4) i = 2 (5,6) i = 3 (7,8)
    plt.subplot(row_count, col_count, 2 * i + 1)
    plot_image(i, predicts[i], test_y, test_x, class_names)
    plt.subplot(row_count, col_count, 2 * i + 2)
    plot_value_array(i, predicts[i], test_y)


def train_model(train_x, train_y):
    # 开始构建神经网络框架
    model = tf.keras.Sequential()
    model.add(layers.Flatten(input_shape=(28, 28)))  # 将输入为28*28维度的矩阵转化为一维向量，因为全连接的输入层必须是一维向量
    model.add(layers.Dense(128, activation="relu"))
    model.add(layers.Dense(10, activation="softmax"))

    # 设置优化器（反向传播采用的优化器）、损失函数、准确率评测标准
    model.compile(optimizer='adam', loss=tf.keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])

    # 执行训练过程
    model.fit(train_x, train_y, epochs=10)

    return model


def load_model(model_path):
    model = tf.keras.models.load_model(model_path)
    return model


set_numpy_print_options()

fashion_mnist = keras.datasets.fashion_mnist
(train_x, train_y), (test_x, test_y) = fashion_mnist.load_data()
print(
    f"train_x.shape = {train_x.shape}, train_y.shape = {train_y.shape}, test_x.shape = {test_x.shape}, test_y.shape = {test_y.shape}")
print(
    f"type(train_x) = {type(train_x)}, type(train_y) = {type(train_y)}, type(test_x) = {type(test_x)}, type(test_y) = {type(test_y)}")

class_names = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat',
               'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']

# # 开始构建神经网络框架
model_file_path = "fashion_model.h5"
if not path_exist(model_file_path):
    print("训练模型:")
    model = train_model(train_x, train_y)
    model.save(model_file_path)
else:
    print("加载模型:")
    model = load_model(model_file_path)

# 训练完之后对模型进行评估. 评估时使用测试集进行评估
test_loss, test_acc = model.evaluate(test_x, test_y, verbose=2)
print("test_loss:\n", test_loss)
print("test_acc:\n", test_acc)

# 预测
predicts = model.predict(test_x)

# 绘制预测的结果
# plt.subplots_adjust(hspace=0.5)
#
# count = 12
# row_count = int(count / 2)
# col_count = 2 * 2
# for i in range(count):
#     plot_one_group(row_count, col_count, i, predicts, test_x, test_y, class_names)
#
# plt.show()


"""
weights = model.get_weights()
for weight in weights:
    print(weight.shape)

output:
(784, 128)
(128,)
(128, 10)
(10,)

分析:
    输入特征: (60000, 28, 28)     6w个图片样本，每个样本的长宽是28 * 28
    flatten: (60000, 784)       将每个样本从28 * 28 转为 784 * 1。因为全连接的单个输入样本必须是向量，而不是矩阵。
    
    隐藏层: 隐藏层使用128个神经元。
        (784, 128)        权重参数weight矩阵维度为 784 * 128
        (128,)            偏置参数bias维度为(128,)
    
    输出层: 使用10个神经元。（因为是10分类）
        (128,10)          权重参数weight矩阵维度为 128 * 10
        (10,)             偏置参数bias维度为(10,)
    
    
    model.get_weights()的返回值，不仅包含了所有的权重参数，还包括了偏置参数。
"""

# print(train_x.shape)
# weights = model.get_weights()
# for weight in weights:
#     print(weight.shape)
