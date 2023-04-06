### [C++ 内存模型和原子操作](#)
**介绍**：动态内存模型可以理解为**存储一致性模型**，主要是从行为上来看多个线程对同一个对象读写操作时所做的约束，动态内存理解起来会有少许复杂，涉及到内存、Cache、CPU的各个层次的交互。


-----
- [x] [1. C++ 内存模型](##1-c-内存模型)
- [x] [2. C++ 原子操作](#2-c-原子操作)
- [x] [3. std::atomic](#3-stdatomic)
- [x] [4. std::atomic_flag](#4-stdatomic_flag)
- [x] [5. std::memory_order](#5-stdmemory_order)
- [x] [6. 原子操作相关API](#6-原子操作相关api)

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

### [3. std::atomic](#) 
标准原子类型在头文件 **#include \<atomic\>** 和头文件 **\<memory\>** 中定义，这些类型的操作全部都是原子化的，并且C++内置的
原子操作也仅仅支持这些类型。通过互斥量加锁的方式，我们自己也可以自己实现相关操作的原子化！

这些原子类型几乎都具备成员函数 **is_lock_free** 准许使用者判断某一给定类型上的操作是能够由 **原子指令** 直接实现(true)，还是要
**借助编译器和程序库的内部锁**来实现(false)。

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

#### [3.1 支持原子操作的类型](#)
每个 std::atomic 模板的实例化和全特化定义一个原子类型， 可以在多线程中实现对单个变量操作的原子化，相当于在底层加锁了， 
[相关API https://zh.cppreference.com/w/cpp/atomic/atomic](https://zh.cppreference.com/w/cpp/atomic/atomic) 。

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

**对指针类型的特化** 对所有指针类型的部分特化 `std::atomic<U*>`。

#### [3.2 构造函数](#)
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
#### [3.3 成员函数](#)
成员函数很许多，看看就懂！ **只有可以使用的成员函数支持原子操作**

|函数|说明|
|:----|:----|
|bool is_lock_free()|检查原子对象是否免锁|
|store(T arg,std::memory_order order)|原子地以非原子对象替换原子对象的值|
|load|原子地加载并返回原子变量的当前值| 
| **静态 is_always_lock_free** |指示该类型是否始终免锁|
|fetch_add(T arg,std::memory_order order)|原子地将参数加到存储于原子对象的值，并返回先前保有的值|
|fetch_sub(T arg,std::memory_order order)|原子地从存储于原子对象的值减去参数，并获得先前保有的值|
|fetch_and(T arg,std::memory_order order)|原子地进行参数和原子对象的值的逐位与，并获得先前保有的值|
|fetch_or(T arg,std::memory_order order)|原子地进行参数和原子对象的值的逐位或，并获得先前保有的值|
|fetch_xor(T arg,std::memory_order order)|原子地进行参数和原子对象的值的逐位异或，并获得先前保有的值|
|自增、自减运算符| `operator++`、`operator++(int)`、`operator--`、`operator--(int)`|
|其他运算符|`operator+=`、`operator-=`、`operator&=`、`operator\|=`、`operator^=`|

```cpp
std::atomic<int> count;

count++; //支持原子操作

count = count + 1; //不支持原子操作！ cout 的结果不确定！

count.fetch_add(2, std::memory_order_relaxed); //原子操作
```

#### [3.4 std::memory_order](#)
**对于原子类型上的每一种操作，我们都可以提供额外的参数** std::memory_order， 它指定内存访问，包括常规的非原子内存访问，如何围绕原子操作排序。

[**通常情况下，默认使用 memory_order_seq_cst，所以你如果不确定怎么这些 memory order，就用这个。**](#)
```cpp
typedef enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
} memory_order;
```

```cpp
void do_work()
{
    data.fetch_add(1, std::memory_order_relaxed);
}
```


### [4. std::atomic_flag](#) 
std::atomic_flag 是原子布尔类型。不同于所有 std::atomic 的特化，它保证是免锁的。不同于 std::atomic\<bool\> ， std::atomic_flag 不提供加载或存储操作。

c++11 \<atomic\> 头文件中最简单的原子类型: 。atomic_flag 一种简单的原子布尔类型，只支持两种操作，test-and-set 和 clear,只有两种状态，`0/1` 。

**只能如此操作！**，永远以置0状态开始，
```cpp
std::atomic_flag flag = ATOMIC_FLAG_INIT; //置 0
```

#### [4.1 构造](#)
std::atomic_flag 构造函数如下, std::atomic_flag **只有默认构造函数，拷贝构造函数已被禁用**，因此不能从其他的 std::atomic_flag 对象构造一个新的 std::atomic_flag 对象。
```cpp
atomic_flag() noexcept = default;
atomic_flag (const atomic_flag&T) = delete;
```
如果在初始化时没有明确使用 ATOMIC_FLAG_INIT初始化，那么新创建的 std::atomic_flag 对象的状态是未指定的（unspecified）（既没有被 set 也没有被 clear。）另外，a
tomic_flag不能被拷贝，也不能 move 赋值。

ATOMIC_FLAG_INIT: 如果某个 std::atomic_flag 对象使用该宏初始化，那么可以保证该 std::atomic_flag 对象在创建时处于 clear 状态。

#### [4.2 test_and_set](#)
**原子地设置标志为 true 并返回其先前值** , std::atomic_flag 的 test_and_set 函数原型如下：

```cpp
bool test_and_set (memory_order sync = memory_order_seq_cst) volatile noexcept;
bool test_and_set (memory_order sync = memory_order_seq_cst) noexcept;
```
test_and_set() 函数检查 std::atomic_flag 标志，如果 std::atomic_flag 之前没有被设置过，则设置 std::atomic_flag 的标
志，并返回先前该 std::atomic_flag 对象是否被设置过，如果之前 std::atomic_flag 对象已被设置，则返回 true，否则返回 false。

#### [4.3 clear](#)
**原子地设置标志值为 false** , 清除 std::atomic_flag 对象的标志位，即设置 atomic_flag 的值为 false。clear 函数原型如下：

```cpp
void clear (memory_order sync = memory_order_seq_cst) volatile noexcept;
void clear (memory_order sync = memory_order_seq_cst) noexcept;
```
清除 std::atomic_flag 标志使得下一次调用 std::atomic_flag::test_and_set 返回 false。


#### [4.4 例子](#)
下面先看一个简单的例子，main() 函数中创建了 10 个线程进行计数，率先完成计数任务的线程输出自己的 ID，后续完成计数任务的线程不会输出自身 ID：

```cpp
#include <iostream>              // std::cout
#include <atomic>                // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <thread>                // std::thread, std::this_thread::yield
#include <vector>                // std::vector

std::atomic<bool> ready(false);    // can be checked without being set
std::atomic_flag winner = ATOMIC_FLAG_INIT;    // always set when checked

void count1m(int id)
{
    while (!ready) {
        std::this_thread::yield();
    } // 等待主线程中设置 ready 为 true.

    for (int i = 0; i < 1000000; ++i) {
    } // 计数.

    // 如果某个线程率先执行完上面的计数过程，则输出自己的 ID.
    // 此后其他线程执行 test_and_set 是 if 语句判断为 false，
    // 因此不会输出自身 ID.
    if (!winner.test_and_set()) {
        std::cout << "thread #" << id << " won!\n";
    }
};

int main()
{
    std::vector<std::thread> threads;
    std::cout << "spawning 10 threads that count to 1 million...\n";
    for (int i = 1; i <= 10; ++i)
        threads.push_back(std::thread(count1m, i));
    ready = true;

    for (auto & th:threads)
        th.join();

    return 0;
}
```


### [5. std::memory_order](#) 
memory_order 枚举值的解释，请参考`《C++ 并发编程实战》Edition 2 P149-167` 的内容，属于理论内容！


#### [5.1 memory_order_seq_cst](#)
顺序一致性内存次序，所有以它为标记的操作形成但一的**全局总操作顺序** (分布式系统中的线性一致性模型)，它要求在所有线程间进行全局同步，代价最高，使用不当造成严重性能顺序。
整个多线程的执行就好像在一个单线程上面执行一样！


#### [5.2 memory_order_relaxed](#)
**宽松次序**：我不管你们多线程之间的代码谁先执行，谁后执行，但是单个线程内部的**原子操作**代码一定是先后执行的！

```cpp
void threaTask(){
    x.store(true, std::memory_order::memory_order_relaxed); //先执行
    y.store(23, std::memory_order::memory_order_relaxed);//后执行
}
```
#### [5.3 memory_order_acquire和memory_order_release](#)
前者可以理解为读操作，后者可以理解为写操作，比起 **宽松次序** 多的就是在多线程之间维持 写-读 顺序！
如下锁是，线程2 先执行，因为要读数据得等写操作完成

```cpp
//后执行 线程1 
void threaAcquire(){
    v.store(true, std::memory_order::memory_order_release); //后执行  读
}

//先执行 线程2 
void threaRelease(){
    int result = v.load(std::memory_order::memory_order_acquire); //先执行 写
}
```

#### [5.4 memory_order_acq_rel](#)
这个操作是写读操作，对读取和写入施加 acquire-release 语义，无法被重排。


#### [5.5 memory_order_consume](#)
数据依赖 - （前序依赖 和 携带依赖） 消费者的意思，它的执行一定在  memory_order_acq_rel memory_order_release memory_order_seq_cst 标记的原子操作之后
执行～

### [6. 原子操作相关API](#)
为了内存同步顺序提供了许多的API！

#### [6.1 atomic_thread_fence](#)
建立非原子和宽松原子访问的以 order 指示的内存同步顺序，而无关联的原子操作，栅栏也被称为 **内存卡**、**内存屏障** 。

```cpp
//线程1 肯定后执行完 因为 memory_order_acquire
void threaRelease(){
    while(!y.load(stf::memory_order_relaxed));
    std::atomic_thread_fence(std::memory_order::memory_order_acquire); 
    z = x.load( std::memory_order::memory_order_relaxed);
}

//线程2 先执行 因为 memory_order_release
void threaAcquire(){
    x.store(true, std::memory_order::memory_order_relaxed); //先执行
    std::atomic_thread_fence(std::memory_order::memory_order_release); // 栅栏
    y.store(true, std::memory_order::memory_order_relaxed); //先执行
}
```

-----
时间: [] 