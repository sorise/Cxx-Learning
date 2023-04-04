### [C++ 内存模型和原子操作](#)
**介绍**：动态内存模型可以理解为**存储一致性模型**，主要是从行为上来看多个线程对同一个对象读写操作时所做的约束，动态内存理解起来会有少许复杂，涉及到内存、Cache、CPU的各个层次的交互。


-----
- [x] [1. C++ 内存模型](##1-c-内存模型)
- [x] [2. C++ 原子操作](#2-c-原子操作)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)
-----

### [1. C++ 内存模型](#)
C++ 内存模型牵涉到两个方面：基本结构和并发，基本机构关系到整个程序在内存中的布局，对并发很重要。在C++中“对象”定义是某一存储范围,
这些对象有些是内置基本类型(int、short、flost...), 还有自定义数据类型例如 机构体、类。其本身是一个对象，由子对象构成！

**并发与内存模型的关系**，首先要理解对于一条C++程序代码，可能会被翻译成好几条汇编代码，然后到CPU层面，又会对应好多条指令，
即使在同一线程中，彼此没有依赖关系的指令会被乱序执行。再考虑到编译器优化、 处理器优化、Cache。

> 现代处理器允许指令乱序执行，以避免因指令等待资源而导致处理器处于闲置状态，提高执行速度。

> C++程序员想要写出高性能的多线程程序必须理解内存模型，编译器会给你的程序做静态优化，CPU为了提升性能也有动态乱序执行的行为。
```cpp
x = 1;//没有依赖关系，随便它们哪个先执行
y = 2;
```
然后又由于采用多线程来执行代码，程序最终执行的结果，是多个线程交织执行的结果， 指令的乱序执行是必然的事情，我们知道任何线程对数据(对象)的改动都有可能引起条件竞
争，同时一些数据对象(双端链表)的修改必须采取正确的修改顺序，不然会导致未定义行为。

总之，实际编程中程序不会完全按照你原始代码的顺序来执行，因此内存模型就是程序员、编译器、CPU
之间的契约。编程、编译、执行都会在遵守这个契约的情况下进行，在这样的规则之上各自做自己的优化，从而提升程序的性能。

[**你可能回想，我们利用互斥量加锁的方式来解决这个问题**, 通过互斥量确实可以解决，使得写操作原子化，但是C++还提供了**原子操作**，不用加锁来解决这个问题！](#)

#### [1.1 存储一致性模型](#)
通常来说，存储一致性模型对访存事件次序施加的限制越弱，越有利于提高性能，但增加了编程难度。且限制越强的存储器模型程序可以在限制较弱的存储器模型上运行，很少会导致错误。

> 如果学习过分布式系统应该好理解一点。

* **顺序一致性模型（SC）**
* **完全存储定序模型（TSO）**
* **部分存储定序模型（PSO）**
* **处理器一致性模型（PC）**
* **弱一致性模型（WO）**
* **释放一致性模型（RC）**
* **松散一致性模型（RMO）**
* ** **

#### [1.2 内存的顺序](#)
内存的顺序描述了计算机CPU获取内存的顺序，内存的排序可能静态也可能动态的发生：

静态内存排序：编译器期间，编译器对内存重排
动态内存排序：运行期间，CPU乱序执行


### [2. C++ 原子操作](#) 
明明有互斥量可解决问题，为什么还要原子操作呢？ 如果你想要理解什么是**无锁结构**，那么请学下去！

**原子操作**： 在C++程序中不会被打断的程序执行片段！ 比互斥量更加底层，所以效率更高，但是互斥量也有一个好处，
就是它可以给一个大代码段进行操作，但是原子操作是作用到单个变量上面！

[**真理: 代码段用互斥量生成临界区，单个变量作为临界资源直接上原子操作！**](#)

使用互斥量解决，运行时间(real    0m0.091s)
```cpp
std::mutex mtx;
int i = 0;

auto add = [](std::mutex &mx,int & value, int times)->void {
    for (int c = 0; c < times; c++) {
        std::lock_guard<std::mutex> guard(mx);
        value++;
    }
};

//最后 i的值 = 两百万
std::thread task1(add, std::ref(mtx), std::ref(i), 1000000);
std::thread task2(add, std::ref(mtx), std::ref(i), 1000000);

task1.join();
task2.join();
std::cout << "the actually value: "<< i << std::endl;
```
原子操作解决，运行时间(real    0m0.027s) 
```cpp
std::atomic<int> i = 0;
auto add = [](std::atomic<int> & value, int times)->void {
    for (int c = 0; c < times; c++) value++;
};
//最后 i的值 = 两百万
std::thread task1(add,std::ref(i), 1000000);
std::thread task2(add,  std::ref(i), 1000000);
task1.join();
task2.join();
std::cout << "the actually value: "<< i << std::endl;
```

### [3. atomic](#) 
在头文件 **#include \<atomic\>** 和头文件 **\<memory\>** 定义 中定义，C++ 在底层帮你完成了对内置数据类型的操作原子化。
每个 std::atomic 模板的实例化和全特化定义一个原子类型， 可以在多线程中实现对单个变量操作的原子化，相当于在底层加锁了， 
[相关API https://zh.cppreference.com/w/cpp/atomic/atomic](https://zh.cppreference.com/w/cpp/atomic/atomic) 。

```cpp
template< class T >
struct atomic;

//定义在 头文件 <memory> 中！
template< class U >
struct atomic<U*>;

template<class U>
struct atomic<std::shared_ptr<U>>;

//<stdatomic.h> 定义 C++ 20
template<class U>
struct atomic<std::weak_ptr<U>>;
```

**对整数类型的特化**

以下列整数类型之一实例化时，std::atomic 提供适合于整数类型的额外原子操作，例如 fetch_add、fetch_sub、fetch_and、fetch_or、fetch_xor：

* 字符类型 char、char8_t (C++20 起)、char16_t、char32_t 和 wchar_t；
* 标准有符号整数类型：signed char、short、int、long 和 long long；
* 标准无符号整数类型：unsigned char、unsigned short、unsigned int、unsigned long 和 unsigned long long；

任何标头 \<cstdint\> 中的 typedef 所需的额外整数类型。
另外，结果的 std::atomic<整数> 特化拥有标准布局、平凡默认构造函数 (C++20 前)和平凡析构函数。定义有符号整数算术为使用补码；无未定义结果。


**对浮点类型的特化**

以无 cv 限定的浮点类型（float、double、long double 和无 cv 限定的扩展浮点类型 (C++23 起)）实例化时，std::atomic 提供适合于浮点类型的额外原子操作，例如 fetch_add 和 fetch_sub。

另外，结果的 std::atomic<浮点> 特化拥有标准布局和平凡析构函数。

无操作导致未定义行为，即使结果不能以浮点类型表示。有效的浮点环境可能与调用方线程的浮点环境不同。

#### [3.1 构造函数](#)
原子对象不支持拷贝构造函数操作！

```cpp
atomic() noexcept = default;
constexpr atomic( T desired ) noexcept;
atomic( const atomic& ) = delete;
```

**operator=**, 不支持返回引用，或者赋值操作！
```cpp
T operator=( T desired ) noexcept;
T operator=( T desired ) volatile noexcept;

atomic& operator=( const atomic& ) = delete;
atomic& operator=( const atomic& ) volatile = delete;
```
#### [3.2 成员函数](#)
成员函数很许多，看看就懂！

|函数|说明|
|:----|:----|
|bool is_lock_free()|检查原子对象是否免锁|

 



### [4.](#) 

### [5.](#) 

-----
时间: [] 