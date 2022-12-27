# -*- coding: utf-8 -*-

"""
基于深度神经网络来识别猫
"""

import numpy as np
import h5py
import matplotlib.pyplot as plt

# 自定义工具模块
from testCases import *
from dnn_utils import *

# 设置一些画图
plt.rcParams['figure.figsize'] = (5.0, 4.0)
plt.rcParams['image.interpolation'] = 'nearest'
plt.rcParams['image.cmap'] = 'gray'

np.random.seed(1)
show = True


def my_print(text=True):
    if show == True:
        print(text)

    return


def initialize_parameters_deep(layer_dims):
    """
    Args:
        layer_dims:list类型，包含每层的神经元个数
        例如,layer_dims = [5,4,3]，表示输入层有5个神经元，第一层有4个，最后一层有3个神经元

    Returns:
        parameters:dict类型，包含了每层对应的已经初始化了的w和b
    """
    np.random.seed(1)
    parameters = {}
    L = len(layer_dims)  # 获取神经元的层数

    # w维度：l*(l-1) 神经元数*输入特征数
    # b维度：1*(l-1) 神经元数*1

    # 遍历每一层
    for l in range(1, L):
        # np.random.randn()：标准正态分布
        parameters['W' + str(l)] = np.random.randn(layer_dims[l], layer_dims[l - 1]) / np.sqrt(layer_dims[l - 1])
        parameters['b' + str(l)] = np.zeros((layer_dims[1], 1))

    return parameters


parameters = initialize_parameters_deep([5, 4, 3])


# my_print("W1 = " + str(parameters["W1"]))
# my_print("b1 = " + str(parameters["b1"]))
# my_print("W2 = " + str(parameters["W2"]))
# my_print("b2 = " + str(parameters["b2"]))

def linear_forward(A, W, b):
    """
    线性逻辑回归：Z[l]=W[l]A[l−1]+b[l]Z[l]=W[l]A[l−1]+b[l]
    """

    Z = np.dot(W, A) + b  # 第一层时，A = X
    assert (Z.shape == (W.shape[0], A.shape[1]))
    cache = (A, W, b)  # 元组，反向传播时会用到他们

    return Z, cache


# A, W, b = linear_forward_test_case()
# Z, linear_cache = linear_forward(A, W, b)
# my_print("Z = " + str(Z))

def linear_activation_forward(A_prev, W, b, activation):
    """
    用于实现A=g(Z)，g代表激活函数，在dnn_utils.py中定义了两个激活函数，sigmoid和relu，他们会根据传入的Z计算出A
    """

    Z, linear_cache = linear_forward(A_prev, W, b)

    if activation == "sigmoid":
        A = sigmoid(Z)
    elif activation == "relu":
        A = relu(Z)

    assert (A.shape == (W.shape[0], A_prev.shape[1]))
    cache = (linear_cache, Z)  # 缓存一些变量，后面的反向传播会用到

    return A, cache


# A_prev, W, b = linear_activation_forward_test_case()
# A, linear_activation_cache = linear_activation_forward(A_prev, W, b, activation="sigmoid")
# my_print("with sigmoid:A = " + str(A))
#
# A, linear_activation_cache = linear_activation_forward(A_prev, W, b, activation="relu")
# my_print("with relu:A = " + str(A))


def L_model_forward(X, parameters):
    """
    这个函数构建了一个完整的前向传播过程，这个前向传播一共有L层，前面的L-1层用的激活函数是relu，最后一层使用的是sigmoid
    参数：
        X: 输入的特征你数据
        parameters: 这个字典里面包含了每一层的参数w和b
    """

    caches = []
    A = X

    # 获取参数列表的长度，这个长度的一半就是神经网络的层数
    # 因为这个字典是这样的{w1:,b1:,w2:,b2:,...wl:,bl:}，里面的w1和b1代表了一层
    L = len(parameters) // 2
    my_print("L = {}".format(L))

    # 循环L - 1次，即进行L - 1步前向传播，每一步使用哪个的激活函数都是relu
    for l in range(1, L):
        A_prev = A
        A, cache = linear_activation_forward(A_prev,
                                             parameters['W' + str(l)],
                                             parameters['b' + str(l)],
                                             activation="relu")
        caches.append(cache)

    # 进行最后一层的前向传播，这一层的激活函数是sigmoid，得出的AL就是y'的预测值
    AL, cache = linear_activation_forward(A,
                                          parameters['W' + str(L)],
                                          parameters['b' + str(L)],
                                          activation="sigmoid")
    caches.append(cache)
    assert (AL.shape == (1, X.shape[1]))

    return AL, caches


# X, parameters = L_model_forward_test_case()
# my_print("-----------")
# my_print(f"X.shape = {X.shape}")
# my_print(f"parameter.len = {len(parameters)}")
# my_print(f"parameter = {parameters}")
# AL, caches = L_model_forward(X, parameters)

def compute_cost(AL, Y):
    """
    计算成本，单个样本是损失，多个样本时是成本
    """

    m = Y.shape[1]

    cost = (-1 / m) * np.sum(np.multiply(Y, np.log(AL)) + np.multiply(1 - Y, np.log(1 - AL)))

    my_print(f"m = {m}")
    my_print(f"cost.shape = {cost.shape}")
    my_print("cost = {}".format(cost))
    cost = np.squeeze(cost)  # 确保cost是一个数值，而不是一个数组的形式
    my_print("cost = {}".format(cost))

    assert (cost.shape == ())

    return cost


# Y, AL = compute_cost_test_case()
# my_print("cost = " + str(compute_cost(AL, Y)))

def linear_backward(dZ, cache):
    """
    参数：
    dZ:后面一层的dZ
    cache：前向传播时保存下来的关于本层的一些变量
    """

    A_prev, W, b = cache
    m = A_prev.shape[1]

    dW = np.dot(dZ, cache[0].T) / m
    db = np.sum(dZ, axis=1, keepdims=True) / m
    dA_prev = np.dot(cache[1].T, dZ)

    assert (dA_prev.shape == A_prev.shape)
    assert (dW.shape == W.shape)
    assert (db.shape == b.shape)

    return dA_prev, dW, db


# dZ, linear_cache = linear_backward_test_case()
# dA_prev, dW, db = linear_backward(dZ, linear_cache)
# my_print(f"dA_prev = {dA_prev}")
# my_print(f"dW = {dW}")
# my_print(f"db = {db}")


def linear_activation_backward(dA, cache, activation):
    """
    用于根据本层的dA计算出本层的dZ

    参数：
    dA : 本层的dA
    cache :前向传播时保存的本层的相关变量
    activation: 指示该层使用的是什么激活函数，"sigmoid"或"relu"
    """

    linear_cache, activation_cache = cache
    if activation == "relu":
        dZ = relu_backward(dA, activation_cache)
    elif activation == "sigmoid":
        dZ = sigmoid_backward(dA, activation_cache)

    dA_prev, dW, db = linear_backward(dZ, linear_cache)

    return dA_prev, dW, db

# dAL, linear_activation_cache = linear_activation_backward_test_case()
#
# dA_prev, dW, db = linear_activation_backward(dAL, linear_activation_cache, activation = "sigmoid")
# print ("sigmoid:")
# print ("dA_prev = "+ str(dA_prev))
# print ("dW = " + str(dW))
# print ("db = " + str(db) + "\n")
#
# dA_prev, dW, db = linear_activation_backward(dAL, linear_activation_cache, activation = "relu")
# print ("relu:")
# print ("dA_prev = "+ str(dA_prev))
# print ("dW = " + str(dW))
# print ("db = " + str(db))