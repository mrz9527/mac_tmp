# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import sklearn  # 其它库之前我们都介绍过了。这个新库是用于数据挖掘，数据分析和机器学习的库，例如它里面就内置了很多人工智能函数
import sklearn.datasets
import sklearn.linear_model

# 下面两个库是我们自定义的，第一个提供一些工具函数，第二个提供一些单元测试工具函数。
from planar_utils import plot_decision_boundary, sigmoid, load_planar_dataset, load_extra_datasets
from testCases import *

outfile = open("./shallownn.txt", "w")
print_count = 2

np.random.seed(1)  # 设置一个随机数种子，来保证后面我的代码产生的随机数与你们电脑上产生的一样,这样我的结果才能和你们运行的结果一样

X, Y = load_planar_dataset()

shape_X = X.shape  # 得出X的维度,维度是(2,400)，表示（横纵坐标，样本数）
shape_Y = Y.shape  # 得出Y的维度,维度是(1,400)，表示（颜色标签，样本数）
m = Y.shape[1]  # 得出样本数量


# print ('X的维度是: ' + str(shape_X))
# print ('Y的维度: ' + str(shape_Y))
# print ('训练样本的个数是：' + str(m))

# plt.scatter(X[0, :], X[1, :],c=Y.ravel(), s=40, cmap=plt.cm.Spectral)
# plt.show()

def initialize_parameters(n_x, n_h, n_y):
    """
    参数:
    n_x -- 输入层的神经元个数
    n_h -- 隐藏层的神经元个数
    n_y -- 输出层的神经元个数
    """

    np.random.seed(2)

    # 随机初始化第一层（隐藏层）相关的参数w.
    # 每一个隐藏层神经元都与输入层的每一个神经元相连。每一个相连都会有一个对应的参数w。
    # 所以W1的维度是（n_h, n_x）,表示（隐藏层的神经元个数，输入层神经元个数）
    W1 = np.random.randn(n_h, n_x) * 0.01

    # 将第一层的参数b赋值为0，因为w已经非0了，所以b可以为0
    # 因为每一个神经元只有一个对应的b，所以b1的维度是(n_h, 1)，表示（隐藏层神经元个数，1）
    b1 = np.zeros(shape=(n_h, 1))

    # 同理，初始化第二层的w和b
    W2 = np.random.randn(n_y, n_h) * 0.01
    b2 = np.zeros(shape=(n_y, 1))

    # 将初始化好的参数放入一个字典变量中
    parameters = {"W1": W1,
                  "b1": b1,
                  "W2": W2,
                  "b2": b2}

    print("\ninit_parameters1:", file=outfile)
    print(f"W1.shape={W1.shape} b1.shape={b1.shape} W2.shape={W2.shape} b2.shape={b2.shape}", file=outfile)
    print(f"W1={W1}", file=outfile)
    print(f"b1={b1}", file=outfile)
    print(f"W2={W2}", file=outfile)
    print(f"b2={b2}", file=outfile)

    return parameters


def forward_propagation(X, parameters):
    """
    参数:
    X -- 输入特征，维度是 (横纵坐标, 样本数)
    parameters -- 参数w和b

    Returns:
    A2 -- The sigmoid output of the second activation
    cache -- a dictionary containing "Z1", "A1", "Z2" and "A2"
    """

    m = X.shape[1]  # 获取样本数
    # print("样本数：" + str(m))

    # 从字典中取出参数
    W1 = parameters['W1']
    b1 = parameters['b1']
    W2 = parameters['W2']
    b2 = parameters['b2']

    # 实现前向传播算法
    Z1 = np.dot(W1, X) + b1
    A1 = np.tanh(Z1)  # 第一层的激活函数我们使用tanh。numpy库里面已经帮我们实现了tanh工具函数
    Z2 = np.dot(W2, A1) + b2
    A2 = sigmoid(Z2)  # 第二层我们使用sigmoid，因为我们要解决的这个问题属于二分问题。这个函数是我们自己在planar_utils里面实现的。

    # 将这些前向传播时得出的值保存起来，因为在后面进行反向传播计算时会用到他们。
    cache = {"Z1": Z1,
             "A1": A1,
             "Z2": Z2,
             "A2": A2}

    global print_count
    if print_count > 0:
        print("\nprint_count={}\tforward_propagation".format(print_count), file=outfile)
        print(f"X.shape={X.shape},Y.shape={Y.shape}", file=outfile)
        print(f"W1.shape={W1.shape},b1.shape={b1.shape},W2.shape={W2.shape},b2.shape={b2.shape}", file=outfile)
        print(f"Z1.shape={Z1.shape},A1.shape={A1.shape},Z2.shape={Z2.shape},A2.shape={A2.shape}", file=outfile)
        print(f"W1={W1}", file=outfile)
        print(f"b1={b1}", file=outfile)
        print(f"W2={W2}", file=outfile)
        print(f"b2={b2}", file=outfile)
        print(f"Z1={Z1}", file=outfile)
        print(f"A1={A1}", file=outfile)
        print(f"Z2={Z2}", file=outfile)
        print(f"A2={A2}", file=outfile)

    return A2, cache


# 这个函数被用来计算成本
def compute_cost(A2, Y, parameters):
    """
    参数:
    A2 -- 神经网络最后一层的输出结果
    Y -- 数据的颜色标签
    """

    m = Y.shape[1]

    logprobs = np.multiply(np.log(A2), Y) + np.multiply((1 - Y), np.log(1 - A2))
    cost = - np.sum(logprobs) / m

    global print_count
    if print_count > 0:
        print("\nprint_count={}\tcompute_cost".format(print_count), file=outfile)
        print(f"cost={cost}", file=outfile)

    return cost


# 然后就是反向传播
def backward_propagation(parameters, cache, X, Y):
    """
    参数:
    parameters -- 参数w和b
    cache -- 前向传播时保存起来的一些数据
    X -- 输入特征
    Y -- 标签
    """
    m = X.shape[1]  # 获取样本数

    W1 = parameters['W1']
    W2 = parameters['W2']

    A1 = cache['A1']
    A2 = cache['A2']

    # 根据我们文章中介绍的算法公式来计算梯度（偏导数）
    dZ2 = A2 - Y
    dW2 = (1 / m) * np.dot(dZ2, A1.T)
    db2 = (1 / m) * np.sum(dZ2, axis=1, keepdims=True)
    dZ1 = np.multiply(np.dot(W2.T, dZ2), 1 - np.power(A1, 2))
    dW1 = (1 / m) * np.dot(dZ1, X.T)
    db1 = (1 / m) * np.sum(dZ1, axis=1, keepdims=True)

    grads = {"dW1": dW1,
             "db1": db1,
             "dW2": dW2,
             "db2": db2}

    global print_count
    if print_count > 0:
        print("\nprint_count={}\tbackward_propagation".format(print_count), file=outfile)
        print(f"X.shape={X.shape},Y.shape={Y.shape}", file=outfile)
        print(f"W1.shape={W1.shape},W2.shape={W2.shape}", file=outfile)
        print(f"A1.shape={A1.shape},A2.shape={A2.shape}", file=outfile)
        print(f"dW1.shape={dW1.shape},db1.shape={db1.shape},dW2.shape={dW2.shape},db2.shape={db2.shape}", file=outfile)
        print(f"X={X}", file=outfile)
        print(f"Y={Y}", file=outfile)
        print(f"W1={W1}", file=outfile)
        print(f"W2={W2}", file=outfile)
        print(f"A1={A1}", file=outfile)
        print(f"A2={A2}", file=outfile)
        print(f"dZ2={dZ2}", file=outfile)
        print(f"dW2={dW2}", file=outfile)
        print(f"db2={db2}", file=outfile)
        print(f"dZ1={dZ1}", file=outfile)
        print(f"dW1={dW1}", file=outfile)
        print(f"db1={db1}", file=outfile)

    return grads  # 返回计算得到的梯度


# 用上面得到的梯度来进行梯度下降（更新参数w和b，使其更优化）
def update_parameters(parameters, grads, learning_rate=1.2):
    """
    参数:
    parameters -- 参数w和b
    grads -- 梯度
    """

    W1 = parameters['W1']
    b1 = parameters['b1']
    W2 = parameters['W2']
    b2 = parameters['b2']

    dW1 = grads['dW1']
    db1 = grads['db1']
    dW2 = grads['dW2']
    db2 = grads['db2']

    global print_count
    if print_count > 0:
        print("\nprint_count={}\tupdate_parameters".format(print_count), file=outfile)
        print("更新参数前:", file=outfile)
        print(f"W1.shape={W1.shape},b1.shape={b1.shape},W2.shape={W2.shape},b2.shape={b2.shape}", file=outfile)
        print(f"dW1.shape={dW1.shape},db1.shape={db1.shape},dW2.shape={dW2.shape},db2.shape={db2.shape}", file=outfile)
        print(f"W1={W1}", file=outfile)
        print(f"b1={b1}", file=outfile)
        print(f"W2={W2}", file=outfile)
        print(f"b2={b2}", file=outfile)
        print(f"dW1={dW1}", file=outfile)
        print(f"db1={db1}", file=outfile)
        print(f"dW2={dW2}", file=outfile)
        print(f"db2={db2}", file=outfile)

    # 根据梯度和学习率来更新参数，使其更优
    W1 = W1 - learning_rate * dW1
    b1 = b1 - learning_rate * db1
    W2 = W2 - learning_rate * dW2
    b2 = b2 - learning_rate * db2

    parameters = {"W1": W1,
                  "b1": b1,
                  "W2": W2,
                  "b2": b2}

    if print_count > 0:
        print("更新参数后:", file=outfile)
        print(f"W1.shape={W1.shape},b1.shape={b1.shape},W2.shape={W2.shape},b2.shape={b2.shape}", file=outfile)
        print(f"W1={W1}", file=outfile)
        print(f"b1={b1}", file=outfile)
        print(f"W2={W2}", file=outfile)
        print(f"b2={b2}", file=outfile)

    return parameters


def nn_model(X, Y, n_h, num_iterations=10000, print_cost=False):
    """
    Arguments:
    X -- 输入特征
    Y -- 标签
    n_h -- 隐藏层的神经元个数
    num_iterations -- 训练多少次
    print_cost -- 是否打印出成本
    """

    np.random.seed(3)
    n_x = X.shape[0]  # 根据输入特征的维度得出输入层的神经元个数
    n_y = Y.shape[0]  # 根据标签的维度得出输出层的神经元个数

    # 初始化参数
    parameters = initialize_parameters(n_x, n_h, n_y)
    W1 = parameters['W1']
    b1 = parameters['b1']
    W2 = parameters['W2']
    b2 = parameters['b2']

    # 在这个循环里进行训练，一次一次地对参数进行优化
    for i in range(0, num_iterations):
        # 进行前向传播
        A2, cache = forward_propagation(X, parameters)

        # 计算出本次的成本
        cost = compute_cost(A2, Y, parameters)
        print("i = {} cost = {}".format(i, cost))

        # 进行反向传播。根据前向传播得到的一些值算出梯度。
        grads = backward_propagation(parameters, cache, X, Y)

        # 根据梯度对参数进行一次优化（下降）
        parameters = update_parameters(parameters, grads)

        # 将本次训练的成本打印出来
        if print_cost and i % 1000 == 0:
            print("在训练%i次后，成本是: %f" % (i, cost))

        global print_count
        print_count -= 1

    return parameters


# 这个函数可以利用上面学习到的参数来对新数据进行预测
def predict(parameters, X):
    """
    参数:
    parameters -- 训练得出的参数（学习到的参数）
    X -- 预测数据
    """

    # 预测其实就是简单地执行一次前向传播
    A2, cache = forward_propagation(X, parameters)
    predictions = np.round(A2)  # 对结果进行四舍五入，小于0.5就是0，否则就是1

    return predictions


print(f"X.shape={X.shape},X={X}", file=outfile)
print(f"Y.shape={Y.shape},Y={Y}", file=outfile)
parameters = nn_model(X, Y, n_h=4, num_iterations=10000, print_cost=True)
# print(f"parameters={parameters}", file=outfile)

predictions = predict(parameters, X)
print('预测准确率是: %d' % float((np.dot(Y, predictions.T) + np.dot(1 - Y, 1 - predictions.T)) / float(Y.size) * 100) + '%')
outfile.close()
