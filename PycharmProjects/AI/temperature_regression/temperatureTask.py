# -*- coding: utf-8 -*-

"""
Scikit-learn(sklearn)的定位是通用机器学习库
TensorFlow(tf)的定位主要是深度学习库。

sklearn、tf、keras结合使用。
sklearn肩负基本的数据清理任务；keras用于对问题进行小规模实验验证想法；而tf用于在完整的的数据上进行严肃的调参任务

神经网络模型步骤:
1. 神经网络的准备阶段
1.1 pandas从csv中导入数据:pd.read_csv()
features = pd.read_csv(excel_file)

1.2. 独热编码:pd.get_dummies(features)。将输入特征features中，字符串等非数值的数据用数值数据表示
features = pd.get_dummies(features)

1.3. 从数据中提取标签和特征，以及去除pd.dataframe中的行索引和列索引
labels = features["actual"]
labels = np.array(labels)
features = features.drop(labels=['actual', 'forecast_noaa', 'forecast_acc', 'forecast_under'], axis=1)
features = np.array(features)

1.4. 将数据标准化
features = preprocessing.StandardScaler().fit_transform(features)

2. 神经网络的构建
2.1 创建神经网络模型对象model
model = tf.keras.Sequential()

2.2 添加2个隐藏层和1个输出层,每一层都采用全连接方式Dense
创建网络层layers.Dense()
    创建的网络层有16个神经元，权重w的初始化方式为正态分布，施加权重w的l2正则化惩罚项(惩罚因子为0.03)
    layers.Dense(16, kernel_initializer='random_normal', kernel_regularizer=tf.keras.regularizers.l2(0.03)
添加网络层model.add()

model.add(layers.Dense(16, kernel_initializer='random_normal', kernel_regularizer=tf.keras.regularizers.l2(0.03)))
model.add(layers.Dense(32, kernel_initializer='random_normal', kernel_regularizer=tf.keras.regularizers.l2(0.03)))
model.add(layers.Dense(1, kernel_initializer='random_normal', kernel_regularizer=tf.keras.regularizers.l2(0.03)))

2.3 设置反向传播优化策略和损失函数
反向传播采用随机梯度下降法，学习率为0.01；损失函数采用先平方和，后求均值。
根据是分类模型还是回归模型，选择合适的loss函数
model.compile(optimizer=tf.keras.optimizers.SGD(0.01), loss="mean_squared_error")

2.4 开始训练 model.fit
输入特征:input_features
标签: labels
验证集比例:validation_split,输入数据中的0.25拿出来作为验证集
迭代次数: epochs
小批量更新个数: batch_size
model.fit(input_features, labels, validation_split=0.25, epochs=10, batch_size=64)

2.5 神经网络模型概况
输出神经网络的层次，以及每一层的参数个数(w和b)，以及总的参数个数
model.summary()

3. 神经网络调优
activation：激活函数的选择，一般常用relu
kernel_initializer:权重w的初始化
bias_initialzier: 偏置b的初始化
kernel_regularizer: 权重w的正则化
bias_regularizer: 偏置b的正则化
units: 神经元的个数

神经网络的层数和每层的神经元个数，一般参考开源的或者优质论文、或者类似项目中的参数。
"""
import datetime
import pandas as pd
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers
import matplotlib.pyplot as plt
from sklearn import preprocessing  # 用于预处理，将输入特征数据的标准化，标准化后，数据变换后在指定范围之间，可以避免数据相差太大。


def set_pandas_print_options():
    pd.set_option("display.max_columns", None)
    pd.set_option("display.max_rows", None)
    pd.set_option("display.width", 1000)


def set_numpy_print_options():
    # threshold=np.inf：用于设置全部打印，不要用省略号代替
    # linewidth=1000：用于设置一行显示的宽度，避免一行数据太多而换行
    # suppress=True: 不要用科学计算法来打印数据，默认使用科学计数法。
    np.set_printoptions(threshold=np.inf, linewidth=1000, suppress=True)


# 打印输入数据
def print_input(dates, features):
    """
    绘制输入数据
    """
    plt.style.use('fivethirtyeight')

    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(nrows=2, ncols=2, figsize=(10, 10))
    fig.autofmt_xdate(rotation=45)

    # 标签值
    ax1.plot(dates, features['actual'])
    ax1.set_xlabel('')
    ax1.set_ylabel('temperature')
    ax1.set_title('max temp')

    # 昨天
    ax2.plot(dates, features['temp_1'])
    ax2.set_xlabel('')
    ax2.set_ylabel('temperature')
    ax2.set_title('previous max temp')

    # 前天
    ax3.plot(dates, features['temp_2'])
    ax3.set_xlabel('Date')
    ax3.set_ylabel('temperature')
    ax3.set_title('Two Days Prior max temp')

    # 我的逗比朋友
    ax4.plot(dates, features['friend'])
    ax4.set_xlabel('Date')
    ax4.set_ylabel('temperature')
    ax4.set_title('Friend Estimate')

    plt.tight_layout(pad=2)
    plt.show()


# 转换时间格式
def get_date(features):
    years = features['year']
    months = features['month']
    days = features['day']

    dates = [str(int(year)) + "-" + str(int(month)) + "-" + str(int(day)) for year, month, day in
             zip(years, months, days)]
    dates = [datetime.datetime.strptime(date, "%Y-%m-%d") for date in dates]
    return dates


# 读取excel数据
excel_file = "../data/temps0.csv"
features = pd.read_csv(excel_file)

# 设置打印格式
set_pandas_print_options()
set_numpy_print_options()

# 转换时间格式
dates = get_date(features)

# 打印输入的数据
# print_input(dates, features)

# 独热编码：将输入特征features中，字符串等非数值的数据用数值数据表示
features = pd.get_dummies(features)
# print(features.head(5))


print(__doc__)
print("预处理输入特征数据和标签...")
labels = features["actual"]
labels = np.array(labels)
features = features.drop(labels=['actual', 'forecast_noaa', 'forecast_acc', 'forecast_under'], axis=1)
feature_column_names = list(features.columns)
features = np.array(features)  # 去除dateframe中保留下来的行索引和列索引

print("将数据标准化...")
input_features = preprocessing.StandardScaler().fit_transform(features)
print(f"input_feature.shape = {input_features}")
# print(features)

print("构建神经网络: 2个隐藏层，1个输出层，每一层都采用全连接方式...")
print("神经网络的层数和每层的神经元个数，一般参考开源的或者优质论文、或者类似项目中的参数。")
model = tf.keras.Sequential()  # 按顺序构造网络模型

# print("tf.keras.Sequential() return type: ", type(model))

model.add(layers.Dense(16, kernel_initializer='random_normal', kernel_regularizer=tf.keras.regularizers.l2(0.03)))
model.add(layers.Dense(32, kernel_initializer='random_normal', kernel_regularizer=tf.keras.regularizers.l2(0.03)))
model.add(layers.Dense(1, kernel_initializer='random_normal', kernel_regularizer=tf.keras.regularizers.l2(0.03)))

print("设置反向传播优化策略和损失函数: 反向传播采用随机梯度下降法，学习率为0.01；损失函数采用先平方和，后求均值...")
model.compile(optimizer=tf.keras.optimizers.SGD(0.001), loss="mean_squared_error")

model.fit(input_features, labels, validation_split=0.25, epochs=100, batch_size=64)

print(input_features.shape)
print(labels.shape)

# model.summary()

# result = model.predict(input_features)
# print(result)
