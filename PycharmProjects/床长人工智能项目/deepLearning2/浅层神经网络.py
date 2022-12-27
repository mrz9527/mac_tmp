# -*- coding: utf-8 -*-

"""
深度学习中，分不清两个矩阵应该点乘还是叉乘时，先并排写成两个矩阵及每个元素含义，再按照指定计算表达式来确定
比如
w = [w1,w2,w3]
x= [x1, x2,x3]
z = x1*w1+x2*w2+x3*w3

z = w ? x
z = [w1,w2,w3] ? [x1, x2,x3]
z = np.dot(w,x.T)
"""

import numpy as np
import matplotlib.pyplot as plt
import sklearn  # 数据挖掘、数据分析、机器学习的库，内置很多人工智能函数
import sklearn.datasets
import sklearn.linear_model
from planar_utils import plot_decision_boundary, sigmoid, load_planar_dataset, load_extra_datasets  # 工具库
from testCases import *  # 单元测试函数

outfile = open("./浅层神经网络.txt", "w")
print_count = 2

# 设置随机种子
np.random.seed(1)
# 加载训练数据集
X, Y = load_planar_dataset()  # X:(2,400)  Y:(1, 400)

# 绘制并显示离散图
plt.scatter(X[0, :], X[1, :], c=Y.ravel(), s=40, cmap=plt.cm.Spectral)
plt.show()

# 2.浅层神经网络
"""
输入层，隐藏层，输出层
"""


def init_parameters(n_x: object, n_h: object, n_y: object) -> object:
    """
    初始化浅层神经网络的参数w和b
    Args:
        n_x:输入层神经元个数
        n_h:隐藏层神经元个数
        n_y:输出层神经元个数

    Returns:
    """

    # 设置随机种子
    np.random.seed(2)
    W1 = np.random.randn(n_h, n_x) * 0.01
    b1 = np.zeros(shape=(n_h, 1))

    W2 = np.random.randn(n_y, n_h) * 0.01
    b2 = np.zeros(shape=(n_y, 1))

    params = {"W1": W1,  # W1:(n_h, n_x) -> (4, 2)
              "b1": b1,  # b1:(n_h, 1) -> (4, 1)
              "W2": W2,  # W2:(n_y, n_h) -> (1, 4)
              "b2": b2}  # b2:(n_y, 1) -> (1, 1)

    print("\ninit_parameters1:", file=outfile)

    print(f"W1.shape={W1.shape} b1.shape={b1.shape} W2.shape={W2.shape} b2.shape={b2.shape}", file=outfile)
    print(f"W1={W1}", file=outfile)
    print(f"b1={b1}", file=outfile)
    print(f"W2={W2}", file=outfile)
    print(f"b2={b2}", file=outfile)
    return params


def forward_propagation(X, paramters):
    """
    前向传播
    Args:
        X: 训练集特征    (n_x, m)
        paramters: 模型参数

    Returns:
    """

    W1 = paramters["W1"]  # (n_h, n_x)
    b1 = paramters["b1"]  # (n_h, 1)
    W2 = paramters["W2"]  # (n_y, n_h)
    b2 = paramters["b2"]  # (n_y, 1)

    Z1 = np.dot(W1, X) + b1
    A1 = np.tanh(Z1)  # 隐藏层采用双曲正切激活函数

    Z2 = np.dot(W2, A1) + b2
    A2 = sigmoid(Z2)  # 输出层采用sigmoid激活函数

    cache = {"Z1": Z1,  # (n_h, m) -> (4, 3)
             "A1": A1,  # (n_h, m) -> (4, 3)
             "Z2": Z2,  # (n_y, m) -> (1, 3)
             "A2": A2}  # (n_y, m) -> (1, 3)

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


def compute_cost(A2, Y):
    m = Y.shape[1]
    L = -(np.multiply(Y, np.log(A2)) + np.multiply(1 - Y, np.log(1 - A2)))  # 损失函数，L:(1, m)
    cost = np.sum(L) / m

    global print_count
    if print_count > 0:
        print("\nprint_count={}\tcompute_cost".format(print_count), file=outfile)
        print(f"cost={cost}", file=outfile)

    return cost


def backward_propagation(parameters, cache, X, Y):
    """
    反向传播
    Args:
        parameters: 模型参数，w和b
        cache:      模型中间计算结果，Z，A
        X:          训练集特征
        Y:          训练集标签

    Returns:
        grads:返回梯度
    """
    W1 = parameters["W1"]
    b1 = parameters["b1"]
    W2 = parameters["W2"]
    b2 = parameters["b2"]

    Z1 = cache["Z1"]
    A1 = cache["A1"]
    Z2 = cache["Z2"]
    A2 = cache["A2"]

    """
    W1:(n_h, n_x) -> (4, 2)
    b1:(n_h, 1) -> (4, 1)
    W2:(n_y, n_h) -> (1, 4)
    b2:(n_y, 1) -> (1, 1)
    
    Z1:(n_h, m) -> (4, 3)
    A1:(n_h, m) -> (4, 3)
    Z2:(n_y, m) -> (1, 3)
    A2:(n_y, m) -> (1, 3)
    """

    """
    dL/dZ2 = A2 - Y
    dZ2/dA1 = W2
    dA1/dZ1 = 1-a^2     双曲正切激活函数tanh的导数
    
    dL/dZ1 = dZ2 * W2 * (1-a^2)
    
    双曲正切函数tanh的导数为1-a^2
    dL/dW1 = (dL/dA2 * dA2/dZ2) * dZ2/dA1 * dA1/dZ1 * dZ1/dW1
           = (dL/dZ2 * dZ2/dA1 * dA1/dZ1) * dZ1/dW1
           = dL/dZ1 * dZ1/dW1
    """
    m = X.shape[1]
    dZ2 = A2 - Y
    """
    dw2 = dz2 * x2(a1) 
    dz2 = [[dz21 dz22 dz23]]  
    A1  = [[a11 a12 a13]
           [a21 a22 a23]
           [a31 a32 a33]
           [a41 a42 a43]]
    dw2 = [[dw21 dw22 dw23 dw24]]
    dw21 = (a21*dz21 + a22*dz22 + a23*dz23)/m
    dw22 = ...
    dw23 = ...
    dw24 = ...
    dw2 = np.dot(dz2,A1.T)
    
    db2 = dz2
    db21 = (dz21 + dz22 + dz23) / m
    db2 = np.sum(dz)/m
    
    dz1 = w2*dz2*(1-A1^2)
    w2 = [[w21 w22 w23 w24]]
    dz2= [[dz21 dz22 dz23]]
    A1 = [[a11 a12 a13]
          [a21 a22 a23]
          [a31 a32 a33]
          [a41 a42 a43]]
    dz1 = [[dz11 dz12 dz13]
           [dz21 dz22 dz23]
           [dz31 dz32 dz33]
           [dz41 dz42 dz43]]    
    """
    dW2 = np.dot(dZ2, A1.T) / m
    db2 = np.sum(dZ2, axis=1, keepdims=True) / m
    dZ1 = np.multiply(np.dot(W2.T, dZ2), 1 - np.power(A1, 2))
    dW1 = np.dot(dZ1, X.T) / m
    db1 = np.sum(dZ1, axis=1, keepdims=True) / m

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

    return grads


def update_parameters(parameters, grads, learn_rate=1.2):
    """
    更新参数w和b
    Args:
        parameters:上一次参数w和b
        grads:偏导数
        learn_rate:学习率

    Returns:

    """
    W1 = parameters["W1"]
    b1 = parameters["b1"]
    W2 = parameters["W2"]
    b2 = parameters["b2"]

    dW1 = grads["dW1"]
    db1 = grads["db1"]
    dW2 = grads["dW2"]
    db2 = grads["db2"]

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

    W1 = W1 - learn_rate * dW1
    b1 = b1 - learn_rate * db1
    W2 = W2 - learn_rate * dW2
    b2 = b2 - learn_rate * db2

    parameters = {"W1": W1,
                  "W2": W2,
                  "b1": b1,
                  "b2": b2}

    if print_count > 0:
        print("更新参数后:", file=outfile)
        print(f"W1.shape={W1.shape},b1.shape={b1.shape},W2.shape={W2.shape},b2.shape={b2.shape}", file=outfile)
        print(f"W1={W1}", file=outfile)
        print(f"b1={b1}", file=outfile)
        print(f"W2={W2}", file=outfile)
        print(f"b2={b2}", file=outfile)

    return parameters


def shallow_neural_network_model(X, Y, n_h, num_iterations=10000, print_cost=False):
    """
    浅层神经网络模型
    Args:
        X:训练集输入特征，隐式包含输入层的神经元个数
        Y:训练集标签，隐式包含输出层神经元个数
        n_h:隐藏层的神经元个数
        num_iterations:迭代次数
        print_cost:是否打印成本

    Returns:

    """

    np.random.seed(3)
    n_x = X.shape[0]
    n_y = Y.shape[0]
    parameters = init_parameters(n_x, n_h, n_y)

    # 成本
    cost = []

    for i in range(num_iterations):
        # 前向传播
        A2, cache = forward_propagation(X, parameters)

        # 计算成本
        J = compute_cost(A2, Y)

        cost.append(J)
        # 反向传播计算偏导
        grads = backward_propagation(parameters, cache, X, Y)
        # 更新迭代
        parameters = update_parameters(parameters, grads)

        if print_cost and i % 1000 == 0:
            print("在训练{}次后，成本是{}".format(i, J))

        global print_count
        print_count -= 1

    return parameters, cost


def predict(parameters, X):
    """
    预测
    Args:
        parameters:训练出来的模型参数
        X:预测数据

    Returns:

    """
    A2, cache = forward_propagation(X, parameters)
    predictions = np.round(A2)  # 对结果进行四舍五入，小于0.5就是0，否则就是1

    return predictions


print(f"X.shape={X.shape},X={X}", file=outfile)
print(f"Y.shape={Y.shape},Y={Y}", file=outfile)
parameters, cost = shallow_neural_network_model(X, Y, n_h=4, num_iterations=10000, print_cost=True)
print(f"parameters={parameters}")
predictions = predict(parameters, X)
print('预测准确率是: %d' % float((np.dot(Y, predictions.T) + np.dot(1 - Y, 1 - predictions.T)) / float(Y.size) * 100) + '%')

# 预测结果等高线
plot_decision_boundary(lambda x: predict(parameters, x.T), X, Y.ravel())

plt.figure()
plt.title("成本函数")
plt.plot(cost)
plt.show()

outfile.close()
