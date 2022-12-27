# -*- coding: utf-8 -*-

"""
图像识别
数据预处理：
    图片大小统一
    标签分类采用one_hot_code
    图片颜色值采用归一化为0~1
    数据分为训练集、验证集

模型架构：
    卷积神经网络

过拟合问题解决方法：
    权重参数采用随机初始化、正则化惩罚项、drop-out、数据增强

迁移学习：深度学习必备策略
"""

import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow.keras import layers
from PIL import Image
from tensorflow.keras.preprocessing import image
import tensorflow.keras.backend as K
import os
import glob
import numpy as np


def input_data_by_image_data_augmentation():
    """
    为模型提供预处理好的输入数据，包括：
        读进来的数据会被自动转化成tensor(float32)格式，分别准备训练和验证
        图像数据归一化(0-1)区间
        图片大小统一调整为(64，64)
    :return:
    """
    base_dir = "../data/cats_and_dogs"
    train_dir = os.path.join(base_dir, 'train_')
    validation_dir = os.path.join(base_dir, 'validation_')

    train_data_gen = image.ImageDataGenerator(rescale=1.0 / 255, rotation_range=40, width_shift_range=0.2,
                                              height_shift_range=0.2, shear_range=0.2, zoom_range=0.2,
                                              horizontal_flip=True, fill_mode='nearest')

    validatation_data_gen = image.ImageDataGenerator(rescale=1.0 / 255)

    train_generator = train_data_gen.flow_from_directory(
        train_dir,  # 训练集路径
        batch_size=20,  # batch大小
        target_size=(64, 64),
        class_mode='binary'  # 如果是one_hot，就是categorical，二分类用binary
    )

    validatation_generator = validatation_data_gen.flow_from_directory(
        validation_dir,
        batch_size=20,
        target_size=(64, 64),
        class_mode='binary'
    )

    return train_generator, validatation_generator


def train_model_with_dropout():
    model = tf.keras.Sequential()
    model.add(layers.Conv2D(32, (3, 3), activation='relu', padding='same', input_shape=(64, 64, 3)))
    model.add(layers.MaxPool2D(2, 2))

    model.add(
        layers.Conv2D(64, (3, 3), activation='relu', padding='same'))
    model.add(layers.MaxPool2D(2, 2))

    model.add(layers.Conv2D(128, (3, 3), activation='relu', padding='same'))
    model.add(layers.MaxPool2D(2, 2))

    model.add(layers.Flatten())

    model.add(layers.Dense(512, activation='relu'))
    model.add(layers.Dropout(0.5))
    model.add(layers.Dense(1, activation='sigmoid'))  # 二分类一般用sigmoid，多分类一般用softmax

    model.compile(optimizer=tf.keras.optimizers.Adam(lr=1e-4), loss=tf.keras.losses.binary_crossentropy,
                  metrics=['acc'])

    return model


def load_model():
    pass


def print_result(path):
    name_list = glob.glob(path)
    file_count = len(name_list)

    plt.figure(figsize=(12, 16))
    for i in range(file_count):
        plt.subplot(1, file_count, i + 1)
        img = Image.open(name_list[i])
        plt.imshow(img)


def print_metrics(history):
    train_acc = history.history['acc']
    validation_acc = history.history['val_acc']

    train_loss = history.history['loss']
    validation_loss = history.history['val_loss']

    epochs = range(len(train_acc))

    plt.figure()
    plt.plot(epochs, train_acc, 'g', label='train_acc', )
    plt.plot(epochs, validation_acc, 'b', label='validation_acc')
    title_name = "train and validation accuracy"
    plt.legend(["train", "validation"])
    plt.title(title_name)
    plt.savefig(title_name + "_data_augmentation_with_dropout.jpg")

    plt.figure()
    plt.plot(epochs, train_loss, 'g', label='train_loss')
    plt.plot(epochs, validation_loss, 'b', label='validation_loss')
    title_name = "train and validation loss"
    plt.title(title_name)
    plt.legend(["train", "validation"])
    plt.savefig(title_name + "_data_augmentation_with_dropout.jpg")


# print_result("../data/cats_and_dogs/tmpImg/*")
#  数据预处理
train_generator, validation_generator = input_data_by_image_data_augmentation()

# 网络模型架构
model = train_model_with_dropout()

# 训练网络模型
# 输入数据比较小时，可以使用model.fit()，一次将所有数据加载到内存中
# 输入数据比较大时，应该使用model.fit_generator()，数据存在磁盘中，从磁盘上一次读取一个batch数量的图片
# steps_per_epoch = 训练集样本数/batch_size = 4000 / 20 = 200
# validation_steps = 验证集样本数/batch_size = 2000 / 20 = 100
history = model.fit_generator(
    train_generator,
    steps_per_epoch=200,
    epochs=100,
    validation_data=validation_generator,
    validation_steps=100,
    verbose=2
)

# 保存模型
model_file = "cat_dog_model_by_image_data_augmentation.h5"
model.save(model_file)

# 将训练过程中历史的loss和acc打印出来
print_metrics(history)

# 1B 2D 3C 4A 5C 6ABDE 7B 8C 9B 10B 11B 12A 13C 14B 15A? 16B 17A 18A 19ABCD  20A

# 床长人工智能 第二章：错误题目:6,11,12,15,16

# 修改了6,11,12,16， 第15题不知道答案
# 1B 2D 3C 4A 5C 6ABD 7B 8C 9B 10B 11BC 12AB 13C 14B 15A? 16A 17A 18A 19ABCD  20A
