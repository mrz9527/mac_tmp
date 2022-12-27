

grammer语法纠错相关。

nlprule编译和安装。需要提前安装依赖环境，包括nodejs rust webpack相关，目前能正常运行，但是这个nlprule的功能很有限，拼写错误不支持纠正，第二个是语法错误的能力也很弱。这个可能是需要自己来配置语法规则。

nlprule是基于language tool， language tool的语法规则是通过xml文件来配置的，，但是nlprule里面没有找到xml，猜测nlprule的语法规则打包成二进制了，需要跟到代码里面去看。











语法规则，

nodejs

rust

wasm-pack



sudo npm install -g webpack@3.1.5



webpack-dev-server 3.1.5 打包工具

webpack-dev-server: command not found



拼写错误，不能纠正，语法纠正也不理想，可能是需要自己设置规则。







日语已经交付，编译通过，跑通即可



```
0    heap.Add(make_pair("zhoukang", 13));
1    heap.Add(make_pair("zhangsan", 33));
2    heap.Add(make_pair("zhoushiqi", 5));
3    heap.Add(make_pair("zhoushiyi", 1));
4    heap.Add(make_pair("xingliyan", 32));
5    heap.Add(make_pair("wangerxiao", 13));
6    heap.Add(make_pair("dazhuang", 22));
```





手写源码的重构

完善jni接口

在c++端添加了批量测试

代码规范

面试c++开发

以及思维工作的接收