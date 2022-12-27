# -*- coding: utf-8 -*-

"""
迁移学习：使用现有的网络框架
    主要复用现有网络框架中的卷积层，一般不复用全连接层
"""
import os
import matplotlib.pyplot as plt

# 导入模型ResNet50
from tensorflow.keras.applications.resnet import ResNet50
from tensorflow.keras.applications.resnet import ResNet101
from tensorflow.keras import layers
import tensorflow as tf


def load_resnet_model(input_shape=(75, 75, 3)):
    pre_trained_model = ResNet101(input_shape=input_shape, include_top=False, weights='imagenet')
    for layer in pre_trained_model.layers:
        layer.trainable = False
    return pre_trained_model


def generate_new_model(pre_trained_model):
    # 为全连接层做准备
    x = layers.Flatten()(pre_trained_model.input)
    # 加入全连接层，这个需要重点训练
    x = layers.Dense(1024, activation='relu')(x)
    # 输出层
    x = layers.Dense(1, activation='sigmoid')(x)

    # 构建模型序列
    new_model = tf.keras.Model(pre_trained_model.input, x)
    new_model.compile(optimizer=tf.keras.optimizers.Adam(lr=0.001), loss=tf.keras.losses.binary_crossentropy,
                      metrics=['acc'])
    return new_model


pre_trained_model = load_resnet_model()
new_model = generate_new_model(pre_trained_model)



