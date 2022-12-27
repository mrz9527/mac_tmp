路径问题。windows \； mac /

1. 没有make环境，需要安装minGW环境，然后将mingw32-make.exe改名为make.exe
2. 或者使用visual studio中的nmake工具，类似linux下的make。如果使用nmake，需要提供windows下的脚本，然后再cmake中判断不同的操作系统，使用不同的脚本。

https://cloud.tencent.com/developer/article/1754949

mingw32下载安装

添加mingw32/bin 环境变量

提供build_for_olhct_on_windows.sh及修改的cmakelists.txt



1. 动态库连接不上的问题。git 合并代码的时候，so库文件，它的格式变成了文本文件格式，导致连接不上。

   \n变成了\r\n，连.so和.a库文件也被改动了，导致so变成了文本格式文件

2. 针对在windows下无法编译openblas的问题，第一个是因为在windows下需要安装mingw32编译环境，第二个是需要安装sh或者bash环境。都安装好之后就可以在windows上单独编译openblas了。
3. 但是始终无法在keyboard项目上一起编译openblas。因为在keyboard项目中编译openblas，是采用的脚本方式来编译，在cmakelist中使用了execute_process命令的方式来加载脚本，仍然无法加载。第一个是环境变量不起作用，第二个是路径格式不支持。
4. 所以，现在采用的方式是，只支持在mac下，集成编译opennblas，在windows下，无法集成编译openblas，只能单独编译。而且，现在也提供了so，在windows上不需要重新编译openblas，只需要加载so库即可。

5. 解决了so正常连接，但是识别没有结果的问题，是因为编译openblas的时候，生成了四组so和四组头文件，因为有一个头文件openblas_config.h与cpu架构有关，不同架构下，编译生成的头文件不一样，之前只使用一组头文件，导致头文件与so不匹配。
6. openblas在mac下编译的so和windows下编译的so不一样，在mac下编译的so，识别的速度较快，而windows下编译的so，识别速度很慢，过好几秒才出结果。猜测编译openblas的时候，不仅与手机的cpu架构有关，也与本机pc的cpu有关。
