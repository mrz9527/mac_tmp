stack of overflow

掘金

github

知乎

b站



内存分析：包括内存泄露和内存占用，valgrind

性能分析工具:	gperftools simperf

代码调试:

段错误种类

​	非法内存：内存不存在（访问的内存不存在）、内存受保护（比如内存指向了代码段）、内存只读（char *p="hello"; *p='H';常量区)

​	数组越界：

​	栈溢出：比如递归函数调用次数过多、栈上数组容量开销过大等

段错误

1.debug调试

2.分析core文件

3.注册信号处理函数，backtrace相关函数，打印函数调用堆栈



4.printf打印日志信息

5.屏蔽代码的方式，对于找到代码出错的地方



学生工具

学打字

键盘手势

金山练字



学做笔记

代码笔记：markdown

latex

word

excel



数学图形：geogebra



编程python



分布式版本控制

git





大文件读

大文件传输（零拷贝）

mmap和shmget区别？



epoll？

左值和右值？

libevent

nginx

monkey?

redis?

