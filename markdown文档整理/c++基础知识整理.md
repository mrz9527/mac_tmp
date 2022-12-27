# c++层面

### c++类的成员初始化顺序

```
按照类的成员的声明顺序来初始化
```

### 类的成员变量初始化方式

```
成员变量声明时即初始化、构造函数体内初始化、初始化列表的方式初始化。
```

### malloc / calloc /realloc区别

```
calloc = malloc + memset(0)
calloc申请内存，并初始化为0.

realloc(p, size):为p重新扩展内存大小
如果p在不在堆中，比如在栈或者静态存储区中，会失败
如果p在堆中，如果size=0，相当于free(p);如果size < oldsize,p会丢失数据，p返回地址还是原来地址;如果size = oldsize,不做任何处理;如果size > oldszie,会在原来地址上看看有没有足够的连续空间来扩展，如果有连续，返回的p还是原来的地址，如果没有，会在其他位置找一块连续的地址p'，然后将p中内容拷贝到p'中，然后free(p)，并返回p'，此时p=p'。

```

### new和malloc区别

```
new是c++关键字、malloc是c函数。

new申请的内存在自由存储区，包括堆、栈、全局/静态存储区（比如全局operator new申请的在堆上，通过new placement的在栈上）
malloc申请的在堆上。

new申请空间+调用构造函数，malloc仅仅只申请空间。

new申请空间时，会自动根据类型计算申请的长度，malloc需要指定申请空间的大小

new成功后返回具体类型的指针，malloc返回void*类型

new失败抛出异常，malloc失败返回null。所以，对new返回的指针做判断，是没有用的。（因为失败了就会崩溃，成功了会继续），可以通过std::nothrow来解决。new (std::nothrow) xxx;
```

### <font color=red>malloc的内存管理</font>

malloc主要通过brk和mmap来管理内存。
内存申请的大小
内存的申请和释放
内存申请的位置
内存释放
虚拟地址空间使用查看
缺页中断的查看
内存泄露和内存碎片

```
申请内存大小size，申请的都是虚拟内存地址，只要在使用该内存时，才会触发缺页中断，申请物理内存，并将物理内存和虚拟内存做映射。
size < 128k，通过brk在堆上申请内存，堆高地址pos + size。
size >= 128k，通过mmap在映射区申请内存，位置由操作系统决定或手动指定。

释放申请的内存时。
释放映射区的内存，是立即释放，还给操作系统
释放堆上的内存时，如果该内存不在堆顶，会交由malloc的内存池来管理，并标记为未使用；当该内存为堆顶时，如果累计释放的连续内存未达到128k，仍然会由malloc的内存池管理，如果累计释放的连续内存超过128k，会发生内存紧缩，即还给操作系统。

频繁的申请和释放内存的缺点？
频繁申请和释放大内存，申请时使用mmap，分配虚拟的内存，使用时发生缺页中断，释放时使用munmap；再次分配、使用和释放，仍然会触发缺页中断，也就是频繁的发生缺页中断，缺页中断在内核态进行，内核态cpu消耗过大。
频繁申请和释放小内存，申请时使用brk，分配虚拟的内存，使用时发生缺页中断，然后释放，该内存可能进入malloc的内存池；再次分配、使用和释放，可能不需要发生缺页中断，但是会产生内存碎片。明面上看内存释放了，但是由于内存碎片的存在，会占用更多的物理内存，造成内次泄露

/proc/pid/mmaps，查看虚拟内存空间使用情况
pmap pid，也能查看虚拟内存空间使用情况
man proc, 查看/proc/pid目录下各个字段的意义

查看进程的缺页中断情况，缺页中断次数越大，说明内核态消耗的cpu大。
ps -o majflt,minflt -C <program_name>
ps -o majflt,minflt -p <pid>
majflt大中断：表示需要读写磁盘，可能是内存对应页面在磁盘中需要load到物理内存中，也可能是此时物理内存不足，需要淘汰部分物理页面至磁盘中。如果进程的内核态CPU使用过多，其中一个原因就可能是单位时间的缺页中断次数多个，可通过以上命令来查看。如果MAJFLT过大，很可能是内存不足。
minflt小中断：如果小中断过大，很可能是频繁分配/释放大块内存(128k)，malloc使用mmap来分配。对于这种情况，可通过mallopt(M_MMAP_THRESHOLD)增大临界值，或程序实现内存池。

内存泄露和内存碎片：
内存泄露：申请了内存，但是没有释放。
内存碎片：申请了内存，也free释放了。首先申请的是小内存，通过brk实现（如果是大内存，释放时一定还给操作系统了），释放时不一定还给操作系统，从而导致存在内存碎片。内存碎片无法通过valgrind来检测。

如果vmSize过大，vmRss也很大，很可能是存在大量内存碎片无法回收，导致占用过大的物理内存。
如果vmSize过大，vmRss较小，说明内存利用率比较小，比如申请了1M大小，只使用了10K，那么物理内存只消耗10k，虚拟内存消耗1M。
```

### new申请的内存在自由存储区

```
自由存储区=堆+栈+ 静态存储区
通过重载类的operator new函数，来实现new申请的内存在栈上或者静态存储区上。
```

### 模板推理

```c++
// 泛化
template<typename T>
class X{}

// 全特化
template<>
class X<bool> {}

// 偏特化，个数的偏特化
template<typename T>
class X<bool, T> {}

//偏特化，范围的偏特化
template<typename T>
class X<*T> {}
```

### const 左定值右定向

const int* const p; 左定值，右定向

const常函数，可以作为重载来区分

https://blog.csdn.net/qq_37007384/article/details/103733839

### constexpr

```
const既可以表示常量，也表示只读，双重身份。
constexpr用于常量，作用唯一，用于编译期。
```



### 虚函数和多态

https://blog.csdn.net/lirendada/article/details/126379184

```
通过virtual type Method(param) = 0;来声明纯虚函数。
析构函数必须声明为virtual ~xxx(){};

虚函数表属于代码段，在编译器就确定了，
虚表指针是在对象初始化的时候确定的，是对象的对一个成员变量
相同类型的对象，共享同一个虚函数表
多继承，有多个虚表指针

```



### remove_reference

```c++
// 泛化
template<typename T>
struct remove_reference {
  typedef T type;
}

// 左值引用的偏特化
template<typename T>
struct remove_reference<T&> {
  typedef T type;
}

// 右值引用的偏特化
template<typename T>
struct remove_reference<T&&> {
  typedef T type;
}

```

### std::move

将一个左值/右值，转换为右值引用。作用:
主要是将左值强制转为右值引用。因为移动构造和移动赋值函数的形参为右值引用（不是右值），为了得到右值引用，使用std::move

```c++
template<typename T>
constexpr std::remove_reference<T>::type move(T&& t) {
  return static_cast<typename std::remove_reference<T>::type&&>(t);
}

// std::move函数参数为T&&，一个指向模板类型参数的右值引用，通过引用折叠，此参数可以与任意类型匹配。
string s1("hello"), s2;
s2 = std::move(string("world")); // 传入一个右值
s2 = std::move(s1); // 传入一个左值，std::move将左值变为右值引用，然后调用string移动赋值函数。
```

### lambda表达式

```
auto func = [捕获](参数)mutable throw()->返回类型{

};
// 调用
func();
```



### 左值和右值、左值引用和右值引用

```
左值：可以取地址的为左值
右值：不能取地址的为右值。比如字面常量，或者在表达式求值过程中创建的临时对象。

左值引用：绑定到左值上的引用，为左值引用，也就是常规的引用
右值引用：绑定到右值上的引用，为右值引用，右值引用本身是左值，可以取地址。

为什么引入右值引用？要从函数的参数说起。
函数的形参为普通对象时，要调用拷贝构造函数进行拷贝，函数结束时，要进行析构，会造成大量的拷贝和析构操作。其他临时对象同样的道理；
函数的形参为左值引用（常规引用）时，实参只能为左值，不能传递右值。比如:
	class A;
	Foo(A&); 
	A a;
	Foo(a); 正确
	Foo(A()); 不正确，不接受右值
函数的形参为常引用（const A&)时，实参可以是左值，也可以是右值，但是只能读不能写；
函数的形参为右值引用时，实参是右值，资源的转移。
```

https://blog.csdn.net/qq_33850438/article/details/107193177

# 设计模式

### 编译防火墙Pimpl

举例说明为何需要编译防火墙，以及如何实现。

#### 无防火墙

缺点：一个类发生更改，会导致其他源文件重新编译，即编译依赖。
举例：早期的Book类只有一个关键字，name。打印Book类的信息。

```c++
// book.h
class Book {
public:
  name;
  Book(_name):name(_name){};
  void print();
}

// book.cpp
 void Book::print(){
    cout << "name:" << name << std::endl;
 }

//main.cpp
#include "book.h"
int main() {
  Book book("呐喊");
  book.print();
}
```

根据业务需要，添加一个关键字price。

```c++
// book.h
class Book {
public:
  name;
  price;
  Book(_name, _price):name(_name),price(_price){};
  void print();
}

// book.cpp
 void Book::print(){
    cout << "name:" << name << "price:" << price << std::endl;
 }

// main.cpp
#include "book.h"
int main() {
  Book book("呐喊");
  book.print();
}
```

#### 有防火墙

```c++
// book.h
class BookImpl;
class Book {
public:
  void print();
private:
  BookImpl* impl;
}

// book.cpp
 Book::Book() {
   impl = new BookImpl(name);
 }
 Book::~Book() {
   delete impl;
 }

 void Book::print(){
    impl->print();
 }

class BookImpl {
  public:
  name;
  BookImpl(_name):name(_name){};
  void print();
}

 void BookImpl::print(){
    cout << "name:" << name << std::endl;
 }

// main.cpp
int main() {
  Book book;
  book.print();
}
```

### 状态机模式

相当于if else，如果直接使用if else，当状态较多时，会造成代码臃肿且偶合，新增状态时，代码变动较大。

#### 设计思路

<font color=red>有一个上下文类；有一个抽象状态基类，每个具体的状态都继承自抽象状态基类；各个状态对象自己知道下一个处理的对象是谁，在编译时确定；执行时通过具体状态调用上下文Context的方法来执行。</font>

场景：

0.假定初始状态为A，在A状态下，上下文执行相应Do操作；
1.达到一定条件后，到达状态B，在B状态下，上下文执行相应Do操作；
2.达到一定条件后，到达状态C，在C状态下，上下文执行相应Do操作；
3.达到一定条件后，又到达状态A，在A状态下，上下文执行相应Do操作，如此循环往复。
"达到一定条件"，是在当前状态下判断，从而进入下一个条件

```c++
// 上下文的数据，根据数据判断当前状态是否需要切换，数据可以是内部产生和变动，也可以是外界传入的数据
// 假定数据是上下文内部数据，与外界无关。
class Context {
private:
  shared_ptr<State> mState; // 当前状态，用shared_ptr目的是为了解决资源释放，当A状态变为B状态时，原来的A状态和对应资源释放
  int mData;	
public:
  Context(shared_ptr<State> state, int data):mState(state), mData(data){}; // 设置上下文的初始状态
  int GetData() {return mData;};
  void SetData(int data) {mData = data;};
  shared_ptr<State> GetState(){return mState;};
  void SetState(shared_ptr<State> state){mState = state;};
  void Request() {
    mState->Do(this);
  }
  	
}
class State {
public:
  virtual ~State(){};
  virtual void Do(Context* context)=0; // 在这里需要改变数据，设置新的状态，所以需要传入上下文指针
}

class StateA:public State {
  void Do(Context* context) override { // 在这里需要改变数据，设置新的状态，所以需要传入上下文指针
    int data = context->GetData();
    // 变动数据data
    // 从新设置新的数据
    context->SetData(data);
    
    if(data 满足条件B) {
      context->SetState(std::make_shared<State>(new StateB));
    } else {
      //...什么都不做，不改变状态
    }
  }
}

class StateB:public State {
  void Do(Context* context) override { // 在这里需要改变数据，设置新的状态，所以需要传入上下文指针
    int data = context->GetData();
    // 变动数据data
    // 从新设置新的数据
    context->SetData(data);
    
    if(data 满足条件B) {
      context->SetState(std::make_shared<State>(new StateC));
    } else {
      //...什么都不做，不改变状态
    }
  }
}
```

### 职责链模式

#### 设计思路

<font color=red>有一个抽象管理者基类，提供处理请求和设置上级的接口；每个具体的管理者都继承自抽象管理者基类；每个具体的管理者都设置它的上级管理者，根据请求的数据，来判断请求是否交由上级来处理；执行时通过客户端来设置待处理的数据，并调用处理请求接口。</font>

```c++
//请求的类型，请求的值，请求的内容
struct Data {
  string type;
  int value;
  string text;
}
class Manager {
private:
	string name; // 负责人名字
	shared_ptr<Manager> mLeader; // 负责人上级
public: 
  string GetName(){return name;};
  void SetLeader(shared_ptr<Manager> leader) { mLeader = leader;};
  shared_ptr<Manager> GetLeader(){return mLeader;};
  virtual void HandlerRequest(const Data& data)=0;
  void PrintHanderResult(const Data& data, string result) {
    cout << "type:" + type + ", value:" + value + ", text:" + text << ", 处理意见:" << result << endl;
  }
}

class Manager1 : public Manager {
public:
	void HandlerRequest(const Data& data) {
    if(data.type == "请假" && data.value <=3) {
      PrintHanderResult(data, "同意");
    } else {
      auto leader = GetLeader();
      leader->HandlerRequest(data);
    }
  }
}

class Manager11 : public Manager {
public:
	void HandlerRequest(const Data& data) {
    if((data.type == "请假" && data.value <=5) ||
       (data.type == "加薪" && data.value <=800)) {
      PrintHanderResult(data, "同意");
    } else {
      PrintHanderResult(data, "反对");
    }
  }
} 
```



### 命令模式

<font color=red想象中的命令模式：发出一个命令，立即执行。类似函数调用</font>
<font color=green>实际的命令模式：发出一系列不同命令，将命令存放到容器中，然后按下开始后，统一执行</font>

命令模式类似消息队列。

```
相似点：
	都是把一个个命令或者消息放入一个容器中。
不同点：
	命令模式是把所有命令都放入容器之后，才开始执行。
	消息队列是把一个个消息放入队尾，同时从队头中取出消息去处理。
```

```c++
// 命令模式设计要领：
/*
0. 命令的接收者Recieved，也是命令的真正操作，所有的命令对应的操作都在这里提供。
1. 有一个抽象命令类Command，提供undo、redo、execute的方法，同时提供一个成员变量recieved，在Command构造函数中通过参数的形式赋值
2. 很多具体的命令类ConcreateCommand，这些具体命令的创建由客户端来创建，根据命令的型，调用recieved底层对应的操作。
3. 有一个调用者或者叫做命令管理者Invoke类，有三个命令容器，一个待执行命令容器，一个redo命令容器，一个undo命令容器，有一个接收命令的功能AddCommand()，有一个统一执行功能Process()，在Process里面遍历每一个接收到的命令，并执行。
*/

class

class Command {
public:
  virtual ~Command(){};
  virtual void Execute()=0;
  virtual void Undo()=0;
}

class CommandA : public Command {
public:
  private:
  ParamA mParam;
public:
  CommandA(ParamA param): Command(), mParam(param) {}; // 命令的参数由客户端传入
  void Execute() {
    // 根据命令的参数mParam，操作相应的功能
    cout << "CommandA正在执行，参数为:" << mParam << endl;
  }
  void UnDo() {
    // 反操作
    cout << "撤销CommandA执行，参数为:" << mParam << endl;
  }
}

class CommandB : public Command {
private:
  ParamB mParam;
public:
  CommandB(ParamB param): Command(), mParam(param) {}; // 命令的参数由客户端传入
  void Execute() {
    // 根据命令的参数mParam，操作相应的功能
    cout << "CommandB正在执行，参数为:" << mParam << endl;
  }
  void UnDo() {
    // 反操作
    cout << "撤销CommandB执行，参数为:" << mParam << endl;
  }
}
using CommandPtr=shared_ptr<Command>;
using RecievedPtr = shared_ptr<Recieved>;
class Recieved {
private:
  vector<CommandPtr> commands;
  void AddCommand(CommandPtr command) {
    commands.push_back(command);
  }
  void Process() {
    for (auto command : commands)
      command->Execute();
  }
}

int main() {
  // 创建命令
  CommandPtr commandA = make_shared<Command>(new CommandA(paramA));
  CommandPtr commandB = make_shared<Command>(new CommandB(paramB));
  // 创建命令的接收者或执行者
  RecievedPtr recieved = make_shared<Recieved>(new Recieved);
  
  // 发送命令请求给命令接收者
  recieved->AddCommand(commandA);
  recieved->AddCommand(commandB);
  
  // 执行命令
  recieved->Process();
  
  // 撤销命令
  commandB->Undo();
}
```



### 观察者模式

```c++
// 单一事件监听
class Listener {
  virtual ~Listener() {};
  virtual void doEvent()=0;
}
```

```c++
// 多个事件监听
class Listener {
  virtual ~Listener() {};
  virtual void doEvent()=0;
}
class ConcreteListener:public Listener {
  void doEvent() {
    // ...
  }
}

class Observer {
  vector<Listener*> listeners;
  void add(Listener* listener) {
    listeners.push_back(listener);
  }
  void del(Listener* listener) {
		// 查找listener对应iter
    listeners.erase(iter);
  }
  
  void Notify() {
    for(auto listener:listeners) {
      listener->doEvent();
    }
  }
}
```



### 命令模式

## c++11 新特性

decltype和auto

std::async

### std::function

对可调用实体进行封装，包括普通函数、lambda表达式、函数指针、其他函数对象。**主要用于回调。**

```c++
int f1(int a, int b);

class A {
  static int f2(int a, int b);
}

auto f3 = [](int a, int b)->int{};

class F4{
  int operator()(int a, int b);
}

class B{
  int f5(int a, string s, int b);
}

std::function<int(int,int)> func1 = f1; // 普通函数
std::function<int(int,int)> func2 = &A::f2; // 静态成员函数
std::function<int(int,int)> func3 = f3; // 静态成员函数
F4 f4;
std::function<int(int,int)> func4 = f4; // 仿函数
// f5为类的成员函数，无法直接关联f5，原因为f5()实际上有4个参数，第一个参数为this本身，第3个参数为s，且两个int参数没有挨着。
// 需要借助std::bind()，将this和s提前绑定上。

B b;
std::function<int(int,int)> func5 = std::bind(&B::f5, b, std::placeholders::_1, "hello", std::placeholders::_2); // 仿函数
```

### std::bind

```c++
// 给函数绑定参数，是std::bind1st和std::bind2st的升级版。

class B{
  int f5(int a, string s, int b);
}

using namespace std::placeholders;
int main() {
 	B b;
	std::function<int(int,int)> func5 = std::bind(&B::f5, b, _1, "hello", _2); 
  // 前面长长的一段声明直接用auto即可，在编译期自行推导
	auto func6 = std::bind(&B::f5, b, _1, "world", _2); 
  func5(100, 200);
  func6(300,400);
}
```



std::future



2. # 进程管理

   进程间通信

   管道、消息队列、共享内存，各自的特点

   

   线程

   分离式线程和join式线程

   同步和互斥

   

   锁（锁的类型）

   条件变量

   原子操作

   

   线程的回收

   

   进程和线程的控制

   

   信号事件

   信号的种类

   从cpu的角度，中断会发生什么

   

   # 任务调度、内核态、用户态切换

   cpu切换、内核态切换

   

   系统资源（内核资源、外界设备）等资源，都必须进入内核态才能调度

   

   磁盘操作

   磁盘读写

   用户态<---->内核态<---->磁盘

   磁盘按块来读取

   从磁盘读到内核缓冲区，页缓存

   读缓存和写缓存

   

   网络编程

   服务端bind listen, accept、read write

   io多路复用

   select poll epoll

   事件驱动

   libevent

   libcurl

   

   网络架构 和网络模型

   应用层	http

   传输层	tcp

   网络层	ip

   链路层	mac

   

   http和grpc

   

   linux 网络内核参数修改

   /proc/sys/net/ipv4/

   

   网络工具

   网络层工具ping、tracer

   传输层工具 telnet  

   nc 、tcpdump netstat ss

   

   `````````````
   
   `````````````

   ps top

   

   

   io零拷贝

   

   文件读写、io同步与异步、阻塞与非阻塞、缓冲与非缓冲

   

   

   docker

   

   mysql

   redis

   

   

   cuda

   triton server

   

   opengl

   mfc

   

   数据结构

   

   算法

   贪心算法、排序算法（归并排序、快排)、动态规划算法、分治算法

   层次遍历和深度遍历。

   

   openblas



```c++
struct Point{
  float x, y;
  Point(float _x, float _y):x(_x), y(_y){};
  Point():x(0),y(0){};
}
using Dir=Point;
class Player {
private:
  Point pt; // 玩家的位置数据
  float speed = 2; // 速度大小
  void leftMove(const Point& newPt) {
    Point oldPt(pt);
    pt = newPt;
    return oldPt;
  } 
}
class Command {
private:
  Player* player;
public:
  Command(Player* p):player(p){};
  virtual ~KeyCommand(){};
  virtual void execute()=0;
  virtual void redo()=0;
  virtual void undo()=0;
}

class MoveCommand:Command {
public:
  MoveCommand(Player* p, const Point& newPt):Command(p),newPt(newPt) {};
  void execute() {
    oldPt = player->move(newPt); // 旧的数据肯定来源于player，旧的数据保存到命令中
  }
  void undo() {
		newPt = player->move(oldPt);
  }
  void redo() {
    oldPt = player->move(newPt);
    // execute();
  }
  
private:
  Point oldPt = {0, 0};
  Point newPt;
}
```











### c++开发阶段：

版本控制：git

构建工具：cmake

编辑器： clion、vim、sublime



数据库：redis



单元测试：googletest

内存分析：valgrind

性能分析：gperftools

https://drive.google.com/file/d/1t5-JJTBs_JlVUz16WhMA9SxmiBuNYusK/view?usp=sharing



### android开发阶段

adb

jni



### 部署阶段

docker

nginx



socket    

多线程中：socket连接池    数据库连接池

池的思想



### linux常用命令

```
ulimit

# 文本处理相关
sed
sort
awk
tail
head
find
grep

# 系统资源相关
ps
top
free

# 磁盘相关
df
fdisk
mount

# 网络相关
curl
wget
ping
telnet
ssh
sftp
tcpdump

```





### 高并发

```
select

poll

epoll

协程


进程间通信

rpc

分布式、高并发、多线程、io复用、rpc、集群
```

### 消息中间件

一般是rpc + 消息中间件

（redis、kafka、RabbitMQ、ActiveMQ、zeroMQ、RocketMQ）    



### 多进程通信

```
不同机器之间：socket（是消息接收和发送）、rpc（是函数调用）
同一机器上: 消息队列、管道、共享内存、信号量

共享内存 + 信号量
共享内存没有提供锁机制，需要配合信号量使用

共享内存：是同一机器上多进程数据传递最有效的方式。操作系统将不同进程之间的共享内存安排为同一物理内存，进程可以将共享内存连接到它们自己的地址空间中，如果某个进程修改了共享内存中的数据，其他的进程读取到的数据也将改变。

linux ipcs 命令查看共享内存

共享内存和mmap？

消息队列 + 消息中间件
```



### 多线程通信

```
多线程同步：全局变量 + 互斥锁
多线程异步：全局变量 + 互斥锁 + 条件变量。

```



信号



### 前端

```
html 

javascript

css
```



mmap 内存文件映射



redis内存缓存

人工智能 深度学习 rnn  ncnn transformer nlp python random  pandas 



分析动态库或可执行程序

```
objdump
ldd
readelf
```



http服务开发框架

```
python框架：flask
```



tcp服务开发框架

```
socket:网络通信
thread ：多线程
fork：多进程
select
poll
epoll
服务清理程序
服务日志
```





网络服务端性能测试，包括：

```
服务端的并发能力：同时接收客户端连接的最大数量，测试accept，不是测试业务处理，查看cpu和内存使用率变化（磁盘io、网络io）
客户端的业务请求不要太频繁。
网络通信，一般磁盘io不是网络通信瓶颈 

服务端的业务处理能力：服务端每秒能响应多少个业务请求。比如每秒能查询多少余额，每秒能多少次转账,测试send recv

客户端业务响应时效

网络带宽
```



网络测试工具

```
流量监控
iperf3 : 测试网络带宽
nload

curl 

nc

telnet：远程登录

tcpdump

// 七层网络，网络协议，协议头格式，抓包的数据及格式

// arp icamp tcp ip udp  
```



### 数据传输协议

protobuf

json

xml

txt

bin





深度学习相关

音视频相关

nlp相关





python

```
工具包
requests:    http请求
json
openpyxl:excel操作
```





高等数学

```
链式求导
拉格朗日乘子法
0范数、向量范数
傅里叶变换
卷积
贝塞尔曲线
插值拟合逼近
```



线性代数

```
线性方程组->矩阵方程->求解矩阵方程->采用拟矩阵来计算
特征值
特征根
矩阵的范数
数值分析方法中求解线性方程组（或求解矩阵方程）
```



概率论与数理统计

```
频率、均值
概率、期望
方差

概率模型：高斯分布、二项式分布等相关

概率密度
概率分布
贝叶斯概率
贝叶斯估计
极大似然估计
先验概率
后验概率
隐马尔科夫
弱大数定理
强大数定理

```

