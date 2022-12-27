# gperftools

## 安装

```shell
$ git clone https://github.com/gperftools/gperftools
$ cd gperftools
$ ./autogen.sh
$ ./configure
$ make -j4
$ sudo make install
```

如果是需要交叉编译的话，则再重新编译。 ./configure 时指定目标平台，编译工具链即可。只需要编译，不需要安装，后面我们直接 copy so 库到板子上就行：

```
qiushao@qiushao-pc:~/projects/opensources/gperftools$ make clean
qiushao@qiushao-pc:~/projects/opensources/gperftools$ ./configure --host=arm-linux CXX=arm-himix200-linux-g++ CC=arm-himix200-linux-gcc
qiushao@qiushao-pc:~/projects/opensources/gperftools$ make -j
```

编译的结果在 libs 目录下，gperftools 是一个工具集合



## 耗时分析

## 内存泄露

### 链接tcmalloc，生成内存泄露数据

方法1：不侵入代码。

```shell
$ LD_PRELOAD=/path/to/libtcmalloc.so HEAPCHECK=normal ./hwlinux /home/zhoukang/projects/20220218
```

**上面shell语句需要写在同一行，不然没有结果**

- LD_PRELOAD : 指定 libtcmalloc.so 的路径，LD_PRELOAD=/home/zhoukang/env/usr/lib/libtcmalloc.so
- HEAPCHECK: 指定检查等级。包括 minimal, normal, strict, draconian 这四种等级。具体差别参考文档： gperftools/docs/heap_checker.html 。如果 normal 没有检查出来问题的话，再用 draconian 试试。

执行完成后，会直接输出内存泄露的位置

同时在/tmp 目录下生成一个 heap 文件：/tmp/tcmalloc_test.14180.*main*-end.heap。这个文件记录了所有的内存分配信息。

### 分析内存泄露数据

```shell
$ pprof ./hwlinux "/tmp/hwlinux.13986._main_-end.heap" --lines  --text --stack
```

