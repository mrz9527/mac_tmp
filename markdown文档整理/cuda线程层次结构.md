# kenerl_name<<<grid, block>>>()

# grid：线程网格

在host中

```
dim3 grid; 默认初始化为grid.x = 1, grid.y = 1, grid.z = 1。
一般使用一维或者二维grid，第三维不使用，默认为grid.z=1。

grid.x : 一个grid中block的行数
grid.y : 一个grid中block的列数

grid用于核函数的执行配置。
```

在device中

```
内置gridDim变量。
gridDim.x : 一个grid中block的行数
gridDim.y : 一个grid中block的列数

gridDim用于获取block的行列数。
```

举例: 一个二维grid，包括3 * 4的block。gridDim.x = 3和gridDim.y = 4，grid中block排列方式如下：

```
(0,0) (0,1) (0,2) (0,3)
(1,0) (1,1) (1,2) (1,3)
(2,0) (2,1) (2,2) (2, 3)
```

通用形式：一个二维grid中，block的排列

```
(0,0) 							...		(0, gridDim.x - 1)

... 								... 	...

(gridDimx - 1,0) 		...		(gridDim.x - 1, gridDim.y -1)
```

# block：线程块

在host中

```
dim3 block; 默认初始化为block.x = 1, block.y = 1, block.z = 1。
一般使用一维或者二维block，第三维很少使用，默认为block.z=1。

block.x : 一个block中thread的行数
block.y : 一个block中thread的列数

block用于核函数的执行配置。
```

在device中

```
内置blockDim变量。
blockDim.x : 一个block中thread的行数
blockDim.y : 一个block中thread的列数

内置blockIdx变量
blockIdx.x : 当前线程对应的线程块在grid中的行索引
blockIdx.y : 当前线程对应的线程块在grid中的列索引
```

举例: 一个二维block，包括3 * 4的thread。blockDim.x = 3和blockDim.y = 4，block中thread排列方式如下：

```
(0,0) (0,1) (0,2) (0,3)
(1,0) (1,1) (1,2) (1,3)
(2,0) (2,1) (2,2) (2, 3)
```

通用形式：一个二维block中，thread的排列

```
(0,0) 							...		(0, blockDim.x - 1)

... 								... 	...

(blockDimx - 1,0) 		...		(blockDim.x - 1, blockDim.y -1)
```

# 

那么，计算一维数组的元素下标对应的id。

grid(1,1,1)， block(nElem,1,1)一维



**通用 grid二维，block二维，计算thread的一维线性索引**

用于对应一维数据data[]

```
blockId = blockIdx.x * gridDim.y + blockIdx.y

threadId = blockId * blockDim.x * blockDim.y + threadIdx.x * blockDim.y + threadIdx.y
			= (blockIdx.x * gridDim.y + blockIdx.y) * blockDim.x * blockDim.y + threadIdx.x * blockDim.y + threadIdx.y
```

如果grid是一维，block是一维

```
gird是一维：blockIdx.y = 0, gridDim.y = 1
block是一维：threadIdx.y = 0, blockDim.y = 1 
带入得：
threadId = (blockIdx.x * 1 + 0) * blockDim.x * 1 + threadIdx.x * 1 + 0
				= blockIdx.x * blockDim.x + threadIdx
```

grid划分成1维，block划分为2维

```
gird是一维：blockIdx.y = 0, gridDim.y = 1
带入得：
threadId = (blockIdx.x * 1 + 0) * blockDim.x * blockDim.y + threadIdx.x * blockDim.y + threadIdx.y
				= blockIdx.x * blockDim.x * blockDim.y + threadIdx.x * blockDim.y + threadIdx.y
```



**通用 grid一维，block二维，计算thread的二维线性索引**

用于对应二维数据data[]

```
blockId = blockIdx.x * gridDim.y + blockIdx.y

threadId = blockId * blockDim.x * blockDim.y + threadIdx.x * blockDim.y + threadIdx.y
			= (blockIdx.x * gridDim.y + blockIdx.y) * blockDim.x * blockDim.y + threadIdx.x * blockDim.y + threadIdx.y
```

如果grid是一维，block是一维

```
gird是一维：blockIdx.y = 0, gridDim.y = 1
block是一维：threadIdx.y = 0, blockDim.y = 1 
带入得：
threadId = (blockIdx.x * 1 + 0) * blockDim.x * 1 + threadIdx.x * 1 + 0
				= blockIdx.x * blockDim.x + threadIdx
```

grid划分成1维，block划分为2维

```
gird是一维：blockIdx.y = 0, gridDim.y = 1
带入得：
threadId = (blockIdx.x * 1 + 0) * blockDim.x * blockDim.y + threadIdx.x * blockDim.y + threadIdx.y
				= blockIdx.x * blockDim.x * blockDim.y + threadIdx.x * blockDim.y + threadIdx.y
```

# 线程层次结构

## a. 二维数组a，用二维grid和二维block来表示

![image-20220917203411628](/Users/xm210408/Library/Application Support/typora-user-images/image-20220917203411628.png)

```
i = blockDim.x * blockIdx.x + threadIdx.x;
j = blockDim.y * blockIdx.y + threadIdx.y;
```

## b.二维数组a，用grid(1,1,1)和二维block表示

与**a**的计算公式一致，**b**的情况是**a**的特例。
此时blockIdx.x ===0, blockIdx.y ===0, blockDim.x ===1, blockDim.y === 1

```
简化为: 
i = threadIdx.x
j = threadIdx.y;
```

## 学习建议

先按照一维或者二维数据，先绘制表格，然后在表格基础上划分grid和block，通过图表的方式来理解线程索引和内存索引之间的对应关系。



疑问1：核函数从启动到运行结束，很慢，能不能提前启动核函数，cpu以通知的方式告知device有了新的数据？

疑问2：能不能一次启动多个核函数？



盲区：

gpu core sm sp grid block thread wrap 



初步了解keyboard中帧序列相关代码，重新实现一套gif解码的工作量进行评估。

对中英大模型进行重测，目前在0817版本上测试完了，然后正在测0316版本上测试。

继续看了一下推理加速相关的知识。





帧序列cpp + java作为一个整体的

还包括渲染



