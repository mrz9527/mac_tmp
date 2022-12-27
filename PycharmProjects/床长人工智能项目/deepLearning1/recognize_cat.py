# -- coding: utf-8 --

"""
第一个人工智能程序！用它可以识别出一张图片中是否有猫。
"""

import sys
import numpy as np  # 科学计算工具库，这里用来进行向量化
import matplotlib.pyplot as plt  # 画图库
import h5py  # 用来加载HDF格式的训练数据集
import skimage.transform as tf  # 用来缩放图片


# 加载训练数据集，训练数据已经提前准备好
def load_dataset():
    train_dataset = h5py.File('./datasets/train_catvnoncat.h5', "r")  # 加载训练数据
    train_set_x_orig = np.array(train_dataset["train_set_x"][:])  # 从训练数据中提取图片的特征数据
    train_set_y_orig = np.array(train_dataset["train_set_y"][:])  # 从训练数据中提取图片的标签数据

    test_dataset = h5py.File('./datasets/test_catvnoncat.h5', "r")  # 加载测试数据
    test_set_x_orig = np.array(test_dataset["test_set_x"][:])  # 从测试数据中提取图片的特征数据
    test_set_y_orig = np.array(test_dataset["test_set_y"][:])  # 从测试数据中提取图片的标签数据

    classes = np.array(test_dataset["list_classes"][:])  # 加载标签类别数据，这里的类别只有两种，1代表有猫，0代表无猫

    """
    加入一段测试输出代码
    """
    print("train_set_x_orig.shape = {}".format(train_set_x_orig.shape))
    print("train_set_y_orig.shape = {}".format(train_set_y_orig.shape))

    print("test_set_x_orig.shape = {}".format(test_set_x_orig.shape))
    print("test_set_y_orig.shape = {}".format(test_set_y_orig.shape))

    print("train_set_x_orig.ndim = {}".format(train_set_x_orig.ndim))
    print("train_set_y_orig.ndim = {}".format(train_set_y_orig.ndim))

    print("train_set_x_orig.shape = {}".format(str(train_set_x_orig.shape)))
    print("train_set_y_orig.shape = {}".format(str(train_set_y_orig.shape)))

    """
    output:
    train_set_x_orig.shape = (209, 64, 64, 3)   说明图片像素大小是64*64个像素，每个像素有三个通道颜色，共有209张训练样本
    train_set_y_orig.shape = (209,)             209张训练样本有209个标签
   
    test_set_x_orig.shape = (50, 64, 64, 3)     测试样本有50张图片
    test_set_y_orig.shape = (50,)
    
    train_set_x_orig.ndim = 4
    train_set_y_orig.ndim = 1
    """

    return train_set_x_orig, train_set_y_orig, test_set_x_orig, test_set_y_orig, classes


def show_picture(index, set_x, set_y, predict_y):
    """
    显示图片，设置图片标题为: 索引->是不是猫
    Args:
        index:
        set_x: (12288, 209)
        set_y: (1, 209)
        predict_y:

    Returns:

    """
    if index < 0 or index >= set_x.shape[0]:
        print(f"show_picture error: 索引index越界")
    else:
        title = "index[fact:predict]_" + str(index) + "[" + str(set_y[index]) + ":" + str(
            int(predict_y[index, 0])) + "]"
        plt.title(title)
        # plt.imshow(set_x[:, index])  # plt.imshow()负责对图像进行处理，并显示其格式，但不能显示
        plt.imshow(set_x[:, index].reshape(64, 64, -1))  # plt.imshow()负责对图像进行处理，并显示其格式，但不能显示
        plt.show()  # plt.show()负责图像处理之后的显示


def sigmoid_function(z):
    a = 1 / (1 + np.exp(-z))
    return a


def init_w_and_b(dim):
    """
    这个函数用于初始化权重数组w和阈值b

    :param dim: w的大小，本例为12288，因为一个特征对应一个权重

    :return:
    w：  权重数组
    b：  阈值
    """

    w = np.zeros((dim, 1))  # 创建一个二维数组
    b = 0

    return w, b


def propagate(w, b, X, Y):
    """
    前向计算

    :param w: 权重，类型，ndarray，维度（12288, 1)
    :param b: 阈值
    :param X: 图片的特征数据，ndarray，维度（12288，209）
    :param Y: 图片的标签，0或1，0是有猫，1是无猫，维度（1，209）
    :return: 成本cost， dw，w的梯度， db， b的梯度
    """

    m = X.shape[1]  # 训练样本数

    Z = np.dot(w.T, X) + b  # w.T->1 * 12288; X-> (12288 * 209); Z->(1 * 209)
    A = sigmoid_function(Z)  # 激活函数，A -> (1 * 209)
    cost = -np.sum(
        Y * np.log(A) + (1 - Y) * np.log(1 - A)) / m  # 成本函数，其实在整个计算过程中，没有涉及到成本函数，而返回成本函数的目的，主要是作为验证打印，观察成本函数是不是最后减小了

    dZ = A - Y  # 梯度dL/dZ -> (1 * 209)
    dW = np.dot(X, dZ.T) / m
    dB = np.sum(dZ) / m

    grad = {"dw": dW,
            "db": dB}

    return grad, cost


def optimize(w, b, X, Y, num_iterations, learning_rate, print_cost=False):
    """

    :param w: 权重， 维度(12288, 1)
    :param b: 阈值
    :param X: 训练集的特征数据，维度(12288, 209)
    :param Y: 训练集的标签数据，维度(1, 209)
    :param num_iterations:  迭代优化次数
    :param learning_rate: 学习率，用来控制优化步进的参数
    :param print_cost: 为True时，每优化100次就把成本cost打印出来，以便观察成本的变化

    :return:
    params : 优化后的w和b，
    costs :每优化100次，将成本记录下来，成本越小，表示参数越优化
    """
    costs = []

    for i in range(num_iterations):
        grad, cost = propagate(w, b, X, Y)  # 计算出梯度和成本

        # 从字典中取出梯度
        dw = grad["dw"]
        db = grad["db"]

        # 进行梯度下降
        w = w - learning_rate * dw
        b = b - learning_rate * db

        if i % 100 == 0:
            costs.append(cost)
            if print_cost == True:
                print(f"优化{i}次后成本是{cost}")

    params = {"w": w,
              "b": b}

    return params, costs


def predict(w, b, X):
    """
    对测试图片进行预测。

    说明：测试数据进行预测时，只需要计算到逻辑回归函数（即线性回归 +  激活函数）即可。
    成本函数不需要计算，成本函数主要是在训练阶段计算；

    :param w: 训练之后的权重，维度(12288, 1)
    :param b: 训练之后的阈值
    :param X: 测试图片的特征数据，这里的测试图片有50张，所以维度是(12288, 50)
    :return: Y_prediction, 对每张图片的预测结果
    """

    m = X.shape[1]  # 获取测试数据样本数（也就是测试图片的数量）
    Y_prediction = np.zeros((1, m))

    A = sigmoid_function(np.dot(w.T, X) + b)  # 对测试图片进行预测，
    for i in range(A.shape[1]):
        if A[0, i] >= 0.5:
            Y_prediction[0, i] = 1

    return Y_prediction


def model(X_train, Y_train, X_test, Y_test, num_iterations=2000, learning_rate=0.5, print_cost=False):
    """
    识别猫的神经网络模型

    Args:
        X_train: 训练集的特征数据， 维度（12288， 209）
        Y_train: 训练集的标签
        X_test:  测试集的特征数据，维度是(12288, 50)
        Y_test:  测试集的标签，维度是(1, 50)
        num_iterations: 需要训练/优化的次数
        learning_rate: 学习步进，用来控制优化步进的参数
        print_cost: 为True时，每优化100次就把成本cost打印出来，以便我们观察成本的变化

    Returns:
        d:  返回一些信息
    """

    # 初始化待训练的参数
    w, b = init_w_and_b(X_train.shape[0])

    # 使用训练数据来训练优化参数
    parameters, costs = optimize(w, b, X_train, Y_train, num_iterations, learning_rate, print_cost)

    # 从字典中分别取出训练好的w和b
    w = parameters["w"]
    b = parameters["b"]

    # 使用训练好的w和b来分别对训练图片和测试图片进行预测
    Y_prediction_train = predict(w, b, X_train)
    Y_prediction_test = predict(w, b, X_test)

    # 打印预测的准确率
    print("对训练图片的预测准确率为：{}%".format(100 - np.mean(np.abs(Y_prediction_train - Y_train)) * 100))
    print("对测试图片的预测准确率为：{}%".format(100 - np.mean(np.abs(Y_prediction_test - Y_test)) * 100))

    d = {"costs": costs,
         "Y_prediction_test": Y_prediction_test,
         "Y_prediction_train": Y_prediction_train,
         "w": w,
         "b": b,
         "learning_rate": learning_rate,
         "num_iterations": num_iterations}

    return d



def show_cost(info):
    costs = np.squeeze(info['costs'])
    plt.plot(costs, label='plot')  # label:添加图例
    plt.ylabel('cost')  # 成本
    plt.xlabel('iterations (per hundreds)')  # 横坐标为训练次数，以100为单位
    plt.title("Learning rate =" + str(info["learning_rate"]))
    plt.legend()  # legend 显示图例
    plt.savefig("./cost.jpg")
    plt.show()


if __name__ == "__main__":
    # 加载训练数据集和测试数据集
    train_set_x_orig, train_set_y_orig, test_set_x_orig, test_set_y_orig, classes = load_dataset()

    # 数据降维 (209, 64, 64, 3) -> (209, 12288)，再转置得到(12288, 209)
    train_set_x_flat = train_set_x_orig.reshape(train_set_x_orig.shape[0], -1).T
    test_set_x_flat = test_set_x_orig.reshape(test_set_x_orig.shape[0], -1).T

    # 数据扁平化 (也就是归一化，因为图片的RGB颜色是分布在0~255,除以255后，颜色分布在0~1之间），扁平化后还用原来的变量代替
    train_set_x_orig = train_set_x_flat / 255
    test_set_x_orig = test_set_x_flat / 255

    # 神经网络模型，返回模型相关信息
    info = model(train_set_x_orig, train_set_y_orig, test_set_x_orig, test_set_y_orig, num_iterations=2000,
                 learning_rate=0.005, print_cost=True)

    # 打印测试集中指定索引的图片，图片标题采用index[fact:predict]，图片实际结果（是否有猫）：预测结果（是否有猫）
    # index = 8
    # show_picture(index, test_set_x_orig, test_set_y_orig, info["Y_prediction_test"].T)

    # 显示梯度下降过程中成本变化，并保存为jpg格式图片
    # show_cost(info)

    # 查看自己下载的图片是不是能预测正确
    my_image = "myPic.jpg"
    fname = "images/" + my_image

    num_px = 64
    image = np.array(plt.imread(fname))
    print(image.shape)
    my_image = tf.resize(image, (num_px, num_px), mode='reflect').reshape((1, num_px * num_px * 3)).T
    my_predicted_image = predict(info["w"], info["b"], my_image)

    plt.imshow(image)
    plt.show()
    print("预测结果为 " + str(int(np.squeeze(my_predicted_image))))


