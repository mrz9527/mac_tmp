# -*- coding: utf-8 -*-

import os
import shutil
import glob

import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

from tensorflow.keras.preprocessing import image


def print_result(path):
    name_list = glob.glob(path)
    file_count = len(name_list)

    plt.figure(figsize=(12, 16))
    for i in range(file_count):
        plt.subplot(1, file_count, i + 1)
        img = Image.open(name_list[i])
        plt.imshow(img)
    plt.show()


def img_rotate(degree, target_size: tuple = None):
    """
    为模型提供预处理好的输入数据，包括：
        读进来的数据会被自动转化成tensor(float32)格式，分别准备训练和验证
        图像数据归一化(0-1)区间
        图片大小统一调整为(64，64)
    :return:
    """

    base_dir = "../data/cats_and_dogs/data_augmentation"
    src_dir = base_dir + "/src"
    out_dir = base_dir + "/rotation_" + str(degree)
    if os.path.exists(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)

    data_gen = image.ImageDataGenerator(rotation_range=degree)
    gen = image.ImageDataGenerator()

    if target_size is not None:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=True, target_size=target_size)
    else:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=True)

    np_data = np.concatenate([data.next() for i in range(data.n)])
    data_gen.fit(np_data)

    if target_size is not None:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen', target_size=target_size)
    else:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen')

    # gen_data本身是个yield生成器，在迭代过程中，才真正的执行flow_from_directory中的代码
    # 此时会进行图像处理操作，并保存图片到输出路径out_dir，然后从yeild处返回。
    for i in range(gen_data.n):
        gen_data.next()


def img_translate(rate, target_size: tuple = None):
    """
    为模型提供预处理好的输入数据，包括：
        读进来的数据会被自动转化成tensor(float32)格式，分别准备训练和验证
        图像数据归一化(0-1)区间
        图片大小统一调整为(64，64)
    :return:
    """

    base_dir = "../data/cats_and_dogs/data_augmentation"
    src_dir = base_dir + "/src"
    out_dir = base_dir + "/translate_" + str(rate)
    if os.path.exists(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)

    data_gen = image.ImageDataGenerator(width_shift_range=rate, height_shift_range=rate, fill_mode='wrap')
    gen = image.ImageDataGenerator()

    if target_size is not None:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=True, target_size=target_size)
    else:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=True)

    np_data = np.concatenate([data.next() for i in range(data.n)])
    data_gen.fit(np_data)

    if target_size is not None:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen', target_size=target_size)
    else:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen')

    # gen_data本身是个yield生成器，在迭代过程中，才真正的执行flow_from_directory中的代码
    # 此时会进行图像处理操作，并保存图片到输出路径out_dir，然后从yeild处返回。
    for i in range(gen_data.n):
        gen_data.next()


def img_zoom(zoom_range, target_size: tuple = None):
    """
    为模型提供预处理好的输入数据，包括：
        读进来的数据会被自动转化成tensor(float32)格式，分别准备训练和验证
        图像数据归一化(0-1)区间
        图片大小统一调整为(64，64)
    :return:
    """

    base_dir = "../data/cats_and_dogs/data_augmentation"
    src_dir = base_dir + "/src"
    out_dir = base_dir + "/zoom_" + str(zoom_range)
    if os.path.exists(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)

    # zoom_range:0~1为放大，大于1为缩小
    data_gen = image.ImageDataGenerator(zoom_range=zoom_range)
    gen = image.ImageDataGenerator()

    if target_size is not None:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=True, target_size=target_size)
    else:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=True)

    np_data = np.concatenate([data.next() for i in range(data.n)])
    data_gen.fit(np_data)

    if target_size is not None:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen', target_size=target_size)
    else:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen')

    # gen_data本身是个yield生成器，在迭代过程中，才真正的执行flow_from_directory中的代码
    # 此时会进行图像处理操作，并保存图片到输出路径out_dir，然后从yeild处返回。
    for i in range(gen_data.n):
        gen_data.next()


def img_channel_translate(channel_tanslate, target_size: tuple = None):
    """
    为模型提供预处理好的输入数据，包括：
        读进来的数据会被自动转化成tensor(float32)格式，分别准备训练和验证
        图像数据归一化(0-1)区间
        图片大小统一调整为(64，64)
    :return:
    """

    base_dir = "../data/cats_and_dogs/data_augmentation"
    src_dir = base_dir + "/src"
    out_dir = base_dir + "/channel_translate_" + str(channel_tanslate)
    if os.path.exists(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)

    # zoom_range:0~1为放大，大于1为缩小
    data_gen = image.ImageDataGenerator(channel_shift_range=channel_tanslate)
    gen = image.ImageDataGenerator()

    if target_size is not None:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=True, target_size=target_size)
    else:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=True)

    np_data = np.concatenate([data.next() for i in range(data.n)])
    data_gen.fit(np_data)

    if target_size is not None:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen', target_size=target_size)
    else:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen')

    # gen_data本身是个yield生成器，在迭代过程中，才真正的执行flow_from_directory中的代码
    # 此时会进行图像处理操作，并保存图片到输出路径out_dir，然后从yeild处返回。
    for i in range(gen_data.n):
        gen_data.next()


def img_vertical_flip(vertical_flip, target_size: tuple = None):
    """
    为模型提供预处理好的输入数据，包括：
        读进来的数据会被自动转化成tensor(float32)格式，分别准备训练和验证
        图像数据归一化(0-1)区间
        图片大小统一调整为(64，64)
    :return:
    """

    base_dir = "../data/cats_and_dogs/data_augmentation"
    src_dir = base_dir + "/src"
    out_dir = base_dir + "/vertical_flip_" + str(vertical_flip)
    if os.path.exists(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)

    # zoom_range:0~1为放大，大于1为缩小，是随机翻转
    data_gen = image.ImageDataGenerator(vertical_flip=vertical_flip)
    gen = image.ImageDataGenerator()

    if target_size is not None:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=False, target_size=target_size)
    else:
        data = gen.flow_from_directory(src_dir, batch_size=1, class_mode=None, shuffle=False)

    np_data = np.concatenate([data.next() for i in range(data.n)])
    data_gen.fit(np_data)

    if target_size is not None:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen', target_size=target_size)
    else:
        gen_data = data_gen.flow_from_directory(src_dir, batch_size=1, shuffle=False, save_to_dir=out_dir,
                                                save_prefix='gen')

    # gen_data本身是个yield生成器，在迭代过程中，才真正的执行flow_from_directory中的代码
    # 此时会进行图像处理操作，并保存图片到输出路径out_dir，然后从yeild处返回。
    for i in range(gen_data.n):
        gen_data.next()


img_rotate(60)
img_translate(0.3)
img_zoom(0.5)
img_channel_translate(100)
img_vertical_flip(True)
