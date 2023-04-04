### [C++ 在线程之间共享数据](#)
**介绍**：在线程之间共享数据是操作系统的基本知识了，保护共享数据可以使用互斥量(mutex，超时互斥量、递归互斥量、超时递归互斥量、共享互斥量)和各种加锁工具。

-----
- [x] [1. 线程间共享数据的问题](#1-线程间共享数据的问题)
- [x] [2. 使用互斥保护共享数据](#2-使用互斥保护共享数据)
- [x] [3. mutex](#3-mutex)
- [x] [4. lock_guard](#4-lock_guard)
- [x] [5. unique_lock](#5-unique_lock)
- [x] [6. 死锁](#6-死锁)
- [x] [7. 单次调用](#7-单次调用)
- [x] [8. 其他互斥量](#8-其他互斥量)
- [x] [9. timed_mutex](#9-timed_mutex)
- [x] [10. recursive_mutex](#10-recursive_mutex)
- [x] [11. recursive_timed_mutex](#11-recursive_timed_mutex)
- [x] [12. shared_mutex](#12-shared_mutex)
- [x] [13. shared_lock](#13-shared_lock)

-----

### [1. 线程间共享数据的问题](#)
归根结底，多线程共享数据的问题主要是**数据写操作**引发的，如果线程之间只有读操作，就根本不会有问题。由于线程的异步性，不同的线程对同一数据项的写顺序是不可预测的，由于顺序不可预测。
而写操作需要正确的顺序才能使得数据项不出错误。

> 例如双休链表的中删除一个节点需要先找到需要删除的节点 **A**、将**A**节点中的前驱节点的`next`指针指向后驱节点，将后驱节点的 `prev`指针指向
**A**节点的前驱节点，然后再删除节点**A**。如果以上操作顺序错乱，双向链表将会导致双向链表被破坏。

* **A. 线程的异步性导致写操作顺序不可预测，不正确的写顺序会破坏数据结构** 

条件竞争也是一个问题，当操作有几个线程负责，它们争先让线程执行各自的操作。
> 例如抢票，几个线程分别处理抢票请求，减少总票的数量。这种情况下线程的执行次序并不重要，只要保持每次修改总票数的线程只有一个就行了，同时要防止恶性竞争，因为一个操作需要多个CPU指针，多个
线程同时修改同一个数据项，可能分别在执行多个CPU指令中的一个，导致非预期错误。

* **B. 多个线程的条件竞争，多个线程竞争执行同一任务(数据竞争)**
  * **把对数据结构的修改全部操作指令组织成一个原子事务**
  * **实现共享数据的互斥 mutex**

**总结：**
* [**实现线程的同步**](#)
* [**实现数据的互斥，按照数据库事务的概念来执行线程操作**](#)

### [2. 使用互斥保护共享数据](#) 
当访问共享数据前，**使用互斥量将相关数据锁住**，再当访问结束后，再将数据解锁。线程库需要保证，当一个线程使用特定互斥量锁住共
享数据时，其他的线程想要访问锁住的数据，都必须等到之前那个线程对数据进行解锁后，才能进行访问。这就保证了所有线程能看到共
享数据是自洽的，不变量没有被破坏。

**互斥mutex是通用的手段，但不是万能灵药，需要妥善组织和编排代码，互斥本身也有问题，表现形式是** [**死锁**](#) **对共享数据的过渡保护和欠保护都会导致问题！**

`C++` 中通过实例化 `std::mutex` 创建互斥量，通过调用成员函数 `lock()` 进行上锁，`unlock()` 进行解锁。
但是在实际操作过程中，不建议直接调用 `std::mutex` 成员函数，因为代码可能存在异常或者程序员会遗漏 `unlock` 成员，导致死锁。

在C++11中，通常利用 `std::lock_guard` 模板类进行mutex的上锁与解锁,它们使用 RAII 手法创建。 在 `std::lock_guard` 的构造函数中对mutex进行**加锁**， 在其析构函数中进行**解锁**。

在C++11中，也提供了 `std::unique_lock` 模板类进行mutex的上锁与解锁，`std::unique_lock` 比 `std::lock_guard` 更灵活，但效率会稍微低一下。

**所在头文件** `#include<mutex>`
|`方法`|`说明`|`参数个数`|
|:---|:---|:---|
|**std::mutex**|创建互斥量|`需要和 lock() 和 unlock() 函数配合`|`有因异常而遗漏lock()的风险`|
|**std::lock_guard<>**|模板类进行mutex的上锁与解锁。|`构造函数中对mutex进行` **加锁**， `在其析构函数中进行` **解锁**|
|**std::unique_lock<>**|模板类进行mutex的上锁与解锁，但是效率低一些|`和lock_guard差不多`|
|**std::scope_lock<> C++17**|C++ 17 引入，增强版的lock_guard,用于多个互斥体的免死锁 RAII 封装器|`explicit scoped_lock( MutexTypes&... m );`|


```cpp
#include<algorithm>
#include<iostream>
#include<mutex>
#include<list>
#include<thread>
std::list<double> scores;
std::mutex mutex_list_score;

//写操作
void add_to_list(double val){
    std::lock_guard<std::mutex> guard(mutex_list_score); //加一个互斥量
    scores.push_back(val);
}

//读操作
bool find_list(double target){
    std::lock_guard<std::mutex> guard(mutex_list_score); //加一个互斥量
    bool isHave = false;
    std::for_each(scores.begin(), scores.end(), [target, &isHave](double v){
        if (target == v)
        {
            isHave = true;
        }
    });
    printf("is have : %d\n", isHave);
    return isHave;
}
```
`std::unique_lock` 要比 `std::lock_guard` 更灵活，但是更灵活的代价是占用空间相对更大一点且相对更慢一点。 **lock_guard** 只关心 `mutex` 的所有权，并不关注其锁定或者未锁定的状态。
而 **unique_lock** 会维护 `mutex`的状态，甚至可以构造一个空的，不带mutex的**unique_lock**。

#### [2.1 多线程无互斥情况](#)
启动四个线程，让这个四个线程同时抢票，总票数50张，每个线程各自尝试100次，每次尝试有可能失败。类似于用户问一句票价，不满意就不买，然后离开窗口。

```cpp
#include<iostream>
#include<thread>
#include <random>
#include<map>
#include<mutex>
```
全局变量
```cpp
std::mutex ticket_mutex; //暂时用不到
std::map<int, double> userIDs; //记录买票人 和 购票钱
int ticket = 50; //五十张票
double price = 5.16; //票价
double income = 0; //卖票收入
```

售票操作
```cpp
void sell(int id, double money){
    if (money < price)
    {
        //钱不够
        printf("user ID: %d  money: %.2lf is not enough\n", id, money);   
        return;
    }
    if (ticket > 0) //还有没有票
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(100)); //小睡0.1s;
        //用于模仿耗时任务，一旦开启线程之间就可能奔溃，这也是为啥要用互斥的原因。
        ticket--; //减少一张
        income += money; //增加一份钱
        userIDs[id] = money; 
        printf("user ID: %d buy the %d ticket by %.2lf\n", id, ticket + 1, money);   
    }
}
```
每个线程轮番执行的售票任务
```cpp
//count 表示每个窗口面对多少个用户
//start 表示用户起始ID 区分不同线程的用户
void task(int count, int start){ 
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < count; i++)
    {
        double seed = std::generate_canonical<double, 12>(gen);
        double money = (seed * 10); //用户预期的票价，低于票价 用户不买票
        sell(i + start, money);
    }
}
```

**开启线程**
```cpp
int main(int argc, char *argv[]){
    std::thread window1(task, 100, 0); 
    std::thread window2(task, 100, 100);
    std::thread window3(task, 100, 200);
    std::thread window4(task, 100, 300);

    window1.join();
    window2.join();
    window3.join();
    window4.join();

    printf("leave ticket: %d\n", ticket);
    printf("income: %.5lf\n", income);
    int i = 0;
    for (std::pair<int, double> &&u : userIDs)
    {
        printf("[%d] id: %d, money: %.2lf \n",i++, u.first, u.second);
    }
    return 0;
}
```

**执行分析：** 由于电脑是多核的，正常情况下执行没有任何错误，因为 sell 函数是一个不耗时的任务， 很快就结束了，几乎不会发生什么冲突。

**如果说我们让售票任务，增加一点睡眠时间模拟耗时任务**  然后执行就错误了，剩余票数为负数，而不是0，票被多买了。
```cpp
std::this_thread::sleep_for(std::chrono::milliseconds(10)); //小睡0.01s;
```
**如果说我们让售票任务，多增加一点睡眠时间模拟耗时任务** 程序直接崩溃了，有一定几率崩溃，毕竟是异步的，不可预测。
```cpp
std::this_thread::sleep_for(std::chrono::milliseconds(100)); //小睡0.1s;
```

这告诉我们保护共享数据的重要性，多线程代码需要考虑线程之间的关系是互斥还是同步、异步，不同的情况需要使用不同的处理方法！
### [3. mutex](#)
[官方文档](https://zh.cppreference.com/w/cpp/thread/mutex)， mutex 类是能用于保护共享数据免受从多个线程同时访问的同步原语，
`std::mutex` 是 `C++11` 中最基本的互斥量，`std::mutex` 对象提供了独占所有权的特性——即 **不支持递归地对 std::mutex 对象上**锁，而 `std::recursive_lock` 则可以递归地对互斥量对象上锁。

> **非递归互斥**： 不太懂

> **递归互斥**: 递归互斥锁与标准互斥锁唯一不同的唯一方法是，同一线程可以多次获取递归互斥锁。

调用方线程从它成功调用 `lock()` 或 `try_lock()` 开始，到它调用 `unlock()` 为止占有 `mutex` 。

线程占有 `mutex` 时，所有其他线程若试图要求 `mutex` 的所有权，则将阻塞（对于 `lock` 的调用）或收到 false 返回值（对于 try_lock ）。

调用方线程在调用 `lock` 或 `try_lock` 前必须不占有` mutex` 。

> 若 mutex 在仍为任何线程所占有时即被销毁，或在占有 mutex 时线程终止，则行为未定义。

**反复尝试加锁:**
```cpp
#include<mutex>
std::mutex _mutex;

//一直等
while(!_mutex.try_lock()){
    //没锁到 继续询问
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); //小睡0.01s 再尝试;
}
//do some things
_mutex.unlock();
```

**一次尝试加锁:**
```cpp
#include<mutex>
std::mutex _mutex;

if(_mutex.try_lock()){
    //do some things
    _mutex.unlock();
}else{
    //获取互斥量失败
}
```

**阻塞等待：** 也不问可不可以加速，直接硬等，在复杂场景下容易造成死锁！
```cpp
void holder(std::mutex &mx){
    mx.lock();//阻塞等待，直到获得锁
    printf("holder get lock\n");
    //等待五秒
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    printf("holder unlock\n");
    mx.unlock();
}
void waiter(std::mutex &mx){
    mx.lock();
    printf("waiter get lock\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    printf("waiter unlock\n");
    mx.unlock();
}

int main() {
    std::cout << "Start, Concurrency!" << std::endl;
    std::thread task_hold(holder, std::ref(context_mx));
    std::thread wait_hold(waiter, std::ref(context_mx));
    task_hold.join();
    wait_hold.join();
    return 0;
}
```
#### [3.1 mutex的内容](#)
需要了解其构造函数，成员函数。她的复制构造函数和赋值运算符都是被禁止调用的，核心方法就三个而已！

```cpp
mutex();
mutex( const mutex& ) = delete; //复制构造函数被删除。
mutex& operator=() = delete; //不可赋值
~mutex();
```
所以说要想将mutex传递给函数，只能传递引用的方式，因为他不支持复制。
传递给线程的可调用对象只能用 **std::ref()**的方式！

**公开方法:**
* **lock()** 锁定互斥，若互斥不可用则阻塞
* **try_lock()**  尝试锁定互斥，若互斥不可用则返回 false. 锁定成功返回 true。
* **unlock()**  解锁互斥


#### [3.2 错误使用mutex](#)
那么该如何给线程加上一个mutex呢？ 如下的方式，很明显是不知道 **死锁** 是什么东西。

```cpp
std::mutex ticket_mutex;

void sell(int id, double money){
    ticket_mutex.lock();
    if (money < price)
    {
        //钱不够
        printf("user ID: %d  money: %.2lf is not enough\n", id, money);   
        return;
    }
    if (ticket > 0) //还有没有票
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //小睡0.01s;
        ticket--; //减少一张
        income += money; //增加一份钱
        userIDs[id] = money; 
        printf("user ID: %d buy the %d ticket by %.2lf\n", id, ticket, money);   
    }
    ticket_mutex.unlock();
}
```

#### [3.3 正确使用mutex](#)
正确加锁应该是先询问是否可以加锁，可以加锁才能加锁，随机加锁会导致线程之间的死锁！

```cpp
void sell(int id, double money){
    while (!ticket_mutex.try_lock())
    {
        //等一会儿再尝试
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); //小睡0.02s;
    }
    if (money < price) //钱够没有
    {
        //钱不够
        printf("user ID: %d  money: %.2lf is not enough\n", id, money);   
    }else{
        if (ticket > 0) //还有没有票
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); //小睡0.01s;
            ticket--; //减少一张
            income += money; //增加一份钱
            userIDs[id] = money; 
            printf("user ID: %d buy the %d ticket by %.2lf\n", id, ticket, money);   
        }else{
            printf("ticket is empty sorry for user ID: %d!\n",id);   
        }
    }
    ticket_mutex.unlock();   
}
```

#### [3.4 lock_guard](#)
你会发现使用lock_guard代码会很简洁！当然新增的代码在lock_guard内部，好处还是不有的
毕竟如果发生异常，锁还可以被释放！

```cpp
void sell(int id, double money){
    std::lock_guard<std::mutex> guard(ticket_mutex); //加互斥量
    if (money < price) //钱够没有
    {
        printf("user ID: %d  money: %.2lf is not enough\n", id, money);   
    }else{
        if (ticket > 0) //还有没有票
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); //小睡0.01s;
            ticket--; //减少一张
            income += money; //增加一份钱
            userIDs[id] = money; 
            printf("user ID: %d buy the %d ticket by %.2lf\n", id, ticket, money);   
        }else{
            printf("ticket is empty sorry for user ID: %d!\n",id);   
        }
    }
}
```

[**我们会发现对于共享数据，一旦出现游离的指针或者引用，这种互斥保护就形同虚设。对于共享数据类型，要设计良好的程序接口，避免泄露其指针和引用给到不按照先获
得互斥锁定再操作的函数，一定要不留后门，对于以上的 ticket 等数据，很明显就有这个问题，所有的地方都可以访问！**](#)
如下所示：
```cpp
class some_data
{
  int a;
  std::string b;
public:
  void do_something();
};

class data_wrapper
{
private:
  some_data data;
  std::mutex m;
public:
  template<typename Function>
  void process_data(Function func)
  {
    std::lock_guard<std::mutex> l(m);
    func(data);    // 1 传递“保护”数据给用户函数
  }
};

some_data* unprotected;

void malicious_function(some_data& protected_data)
{
  unprotected=&protected_data;
}

data_wrapper x;
void foo()
{
  x.process_data(malicious_function);    // 2 传递一个恶意函数
  unprotected->do_something();    // 3 在无保护的情况下访问保护数据
}
```

### [4. lock_guard](#) 
有些人 `lock()` 后总会忘记 `unlock()` 导致其他线程执行不到，卡在`lock()` 处，并且由于异常原因，可能导致 `unlock()`始终不会被执行，造成严重的死锁。
`C++11` 提供了类 `lock_guard`，它是互斥体包装器，`std::lock_guard<>`,针对互斥实现了 `RAII`，也就是在构造的时候加锁，在析构的时候解锁。

[**获得互斥 m 的所有权而不试图锁定它，它是严格基于作用域的,它通过作用域完成 try_to_lock 这个过程！**](#) 

```cpp
explicit lock_guard( mutex_type& m );
lock_guard( mutex_type& m, std::adopt_lock_t t );
lock_guard( const lock_guard& ) = delete;
operator=() = delete;  //不可复制赋值
~lock_guard();
```
std::lock_guard严格基于作用域(scope-based)的锁管理类模板，构造时是否加锁是可选的(不加锁时假定当前线程已经获得锁的所有权)，析构时自动释放锁，所有权不可转移，对象生存期内不允许手动加锁和释放锁。

std::lock_guard 对象并不负责管理 Mutex 对象的生命周期，lock_guard 只是简化了 Mutex 对象的上锁和解锁操作，方便线程对互斥量上锁，即在某个 lock_guard对象的声明周期内，它所管理的锁对象会一直保持
上锁状态；而 lock_guard的生命周期结束之后，它所管理的锁对象会被解锁。

**构造函数解析**
```cpp
std::lock_guard<std::mutex> guard1(_mutex_); //_mutex_ 没有锁定，在构造函数里面锁定
std::lock_guard<std::mutex> guard1(_mutex_, std::adopt_lock); //_mutex_ 已经锁定了，不需要再锁
```


lock_guard的源代码：
```cpp
template <class _Mutex>
class lock_guard { // class with destructor that unlocks a mutex
public:
    using mutex_type = _Mutex;
	//无adopt_lock参数，构造时加锁
    explicit lock_guard(_Mutex& _Mtx) : _MyMutex(_Mtx) { // construct and lock
        _MyMutex.lock();
    }
	//有adopt_lock参数，构造时不加锁
    lock_guard(_Mutex& _Mtx, adopt_lock_t) : _MyMutex(_Mtx) {} // construct but don't lock
	//析构解锁
    ~lock_guard() noexcept {
        _MyMutex.unlock();
    }
	//屏蔽拷贝构造
    lock_guard(const lock_guard&) = delete; 
    lock_guard& operator=(const lock_guard&) = delete; 

private:
    _Mutex& _MyMutex;
};
```


#### [4.1 读写例子](#)
生产者消费者模式，一个线程负责生产消息，一个线程负责消费消息！

引用文件和全局数据！
```cpp
#include<algorithm>
#include<iostream>
#include<fstream>
#include<mutex>
#include<thread>
#include<map>
#include <sstream>


using std::cout;
using std::endl;
using std::cin;

std::mutex msg_mutex;
std::map<int, std::string> message_memory;
int save_message_count = 0;
int read_message_count = 0;
int save_count = 0;
```

创造消息的线程绑定的可调用对象！
```cpp
void createMessage(){
    while (true)
    {
        std::lock_guard<std::mutex> guard(msg_mutex);
        if (save_count < 3)
        {
            std::string ns = "this is a new message " + std::to_string(save_message_count);
            printf("%d create: %s\n",save_message_count, ns.c_str());
            message_memory[save_message_count] =  ns;
            save_message_count ++;
            save_count ++;
        } else{
            std::this_thread::yield();
        }
    }
}
```

消费消息的线程绑定的可调用对象！
```cpp
void readMessage(){
    while (true)
    {
        std::lock_guard<std::mutex> guard(msg_mutex);
        if (save_count > 0)
        {
            std::stringstream ss;
            ss << std::this_thread::get_id();
            std::string r_mssage = ss.str();
            r_mssage = r_mssage + " read: "+  message_memory[read_message_count] + "\n";
            printf("%s", r_mssage.c_str());
            read_message_count++;
            save_count --;
        }
    }
}
```

主函数：
```cpp
int main(int argc, char *argv[]){

    msg_mutex.lock();

    std::thread rt1(readMessage);
    std::thread rt2(readMessage);
    std::thread cr(createMessage);

    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    //开始生产消费
    msg_mutex.unlock();

    rt1.detach();
    rt2.detach();
    cr.detach();

    //不让程序结束
    char msg[256];
    cin.getline(msg, 256); //等待运行
    return 0;
}
```

#### [4.2 造成死锁](#)
也就是最常见的相互等待死锁模型，有两个互斥量, 每个线程都只持有一个，等待另一个。

```cpp
std::mutex mutex_A;
std::mutex mutex_B;
int count = 0;

void deadLock(std::mutex& mx1, std::mutex& mx2){
   std::lock_guard<std::mutex> guard1(mx1);
   std::cout <<"get the first mutex " << std::this_thread::get_id() << "\n" << std::flush;
   std::this_thread::sleep_for(std::chrono::milliseconds(10)); //小睡0.01s;

   std::lock_guard<std::mutex> guard2(mx2);
   std::cout <<"get the second mutex " << std::this_thread::get_id() << "\n" << std::flush;
   std::this_thread::sleep_for(std::chrono::milliseconds(10)); //小睡0.01s;
}

int main(int argc, char *argv[]){

   std::thread t1([&]{ deadLock(mutex_A, mutex_B);});
   std::thread t2([&]{ deadLock(mutex_B, mutex_A);});
   
   t1.join();
   t2.join();
   return 0;
}
```
**防范死锁的建议**： [**始终按照相同顺序对两个互斥量进行加锁！，如果我们总是先锁互斥A，再锁互斥B，那么就可以避免死锁。**](#)


### [5. unique_lock](#) 
与std:::lock_gurad 基本一致，但更加灵活的锁管理类模板，构造时是否加锁是可选的，在对象析构时如果持有锁会自动释放锁，所有权可以转移。

对象生命期内允许手动加锁和释放锁， 但提供了更好的上锁和解锁控制，尤其是在程序抛出异常后先前已被上锁的 Mutex 对象可以正确进行解锁操作，极大地简化

允许延迟锁定、锁定的有时限尝试、递归锁定、所有权转移和与条件变量一同使用。

* **unique_lock也可以加std::adopt_lock参数，表示互斥量已经被lock，不需要再重复lock。该互斥量之前必须已经lock，才可以使用该参数。**
* **请注意，unique_lock和lock_guard都不能复制，lock_guard不能移动，但是unique_lock可以！**
* **unique_lock被析构时，会检查互斥量是否被lock过，如果已经被lock了，那么会调用一次 unlock。**

```cpp
// unique_lock 可以移动，不能复制
std::unique_lock<std::mutex> guard1(_mu);
std::unique_lock<std::mutex> guard2 = guard1;  // error
std::unique_lock<std::mutex> guard2 = std::move(guard1); // ok

// lock_guard 不能移动，不能复制
std::lock_guard<std::mutex> guard1(_mu);
std::lock_guard<std::mutex> guard2 = guard1;  // error
std::lock_guard<std::mutex> guard2 = std::move(guard1); // error
```

#### [5.1 构造函数](#)
构造 unique_lock ，可选地锁定提供的互斥
```cpp
unique_lock() noexcept;
unique_lock( unique_lock&& other ) noexcept;
explicit unique_lock( mutex_type& m );
unique_lock( mutex_type& m, std::defer_lock_t t ) noexcept; //不锁定关联互斥。
unique_lock( mutex_type& m, std::try_to_lock_t t ); 
//通过调用 m.try_lock() 尝试锁定关联互斥而不阻塞。若 Mutex 不满足可锁定 (Lockable) 则行为未定义。
unique_lock( mutex_type& m, std::adopt_lock_t t ); //表示已经加锁，不需要再加锁
template< class Rep, class Period >
unique_lock( mutex_type& m,
             const std::chrono::duration<Rep,Period>& timeout_duration );
template< class Clock, class Duration >
unique_lock( mutex_type& m,
             const std::chrono::time_point<Clock,Duration>& timeout_time );
```

```cpp
std::unique_lock<std::mutex> lk_b(m_b, std::defer_lock); //你先别lock 等我之后手动lock, 你获取mutex所有权就可以
std::unique_lock<std::mutex> lk_c(m_c, std::adopt_lock); //已经锁了，别锁了
std::unique_lock<std::mutex> lk_c(m_c, std::try_to_lock); //尝试锁，锁不到不阻塞！
```

#### [5.2 成员函数](#)
* lock() 锁定关联互斥
* try_lock() 尝试锁定关联互斥，若互斥不可用则返回
* try_lock_for()  试图锁定关联的可定时锁定 (`TimedLockable`) 互斥，若互斥在给定时长中不可用则返回
* try_lock_until() 尝试锁定关联可定时锁定 (`TimedLockable`) 互斥，若抵达指定时间点互斥仍不可用则返回。
* unlock() 解锁关联互斥
* owns_lock() 测试锁是否占有其关联互斥, 常常和std::try_to_lock标记构造函数一起用
* release() 将关联互斥解关联而不解锁它, 会返回互斥量的指针，如果互斥量还需要unlock ，需要手动unlock。不然会造成程序崩溃。

```cpp
std::unique_lock<std::mutex> ulock(ticket_mutex, std::try_to_lock);
if (ulock.owns_lock())
{
    //拿到锁了
}else{
    //没拿到
}
```

**非成员函数：**
* **std::move(unique_lock uq)** 转移的所有权

```cpp
std::unique_lock<std::mutex> guard_uql(ticket_mutex);

std::unique_lock<std::mutex> ulock(std::move(guard_uql)); //转移所有权
````



#### [5.3 当做lock_guard使用](#)
如果我们只传递一个互斥量，不传递第二个参数，那么unique_lock的使用方式和lock_gurad没有什么区别！

```cpp
void sell(int id, double money) {
    std::unique_lock<std::mutex> ulock(ticket_mutex); //和 lock_guard 一样
    if (money < price)
    {
        //钱不够
        printf("user ID: %d  money: %.2lf is not enough\n", id, money);
        return;
    }
    if (ticket > 0) //还有没有票
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(30)); //小睡0.1s;
        //用于模仿耗时任务，一旦开启线程之间就可能奔溃，这也是为啥要用互斥的原因。
        ticket--; //减少一张
        income += money; //增加一份钱
        userIDs[id] = money;
        printf("user ID: %d buy the %d ticket by %.2lf\n", id, ticket + 1, money);
    }
}
```

#### [5.4 一次尝试](#)
启动四个线程，去竞争完成一个加1操作。

```cpp
#include<iostream>
#include<mutex>
#include<thread>
#include<map>
#include <sstream> 

int counter = 0; 
void connect(std::mutex & mtx) {
    //三个线程，只执行一次内容
    std::unique_lock<std::mutex> lock(mtx, std::try_to_lock); //尝试去锁
    if (lock.owns_lock())
    {
        std::thread::id id = std::this_thread::get_id();
        std::stringstream strstream;
        strstream << id;
        std::string mystring = strstream.str();
        //执行
        if (counter == 0)
        {
            counter++;
            printf("the thread: %s finish the work!\n", mystring.c_str());
        }
        else {
            printf("the thread: %s can not finish the work!\n", mystring.c_str());
        }
    }
}

int main(int argc, char* argv[]) {
    std::mutex single; //互斥量 只执行一次

    std::thread t1(connect, std::ref(single));
    std::thread t2(connect, std::ref(single));
    std::thread t3(connect, std::ref(single));
    std::thread t4(connect, std::ref(single));


    t1.join();
    t2.join();
    t3.join();
    t4.join();

    printf("the counter: %d \n", counter);
    return 0;
}
```

#### [5.5 用于指定锁定策略的标签常量](#)
用于指定锁定策略的标签常量,可以传递给 lock_guard 和 unique_lock 作为第二个构造函数参数。它们都是被定义的空结构体！

|类型|效果|
|:---|:---|
|defer_lock_t|不获得互斥的所有权|
|try_to_lock_t|尝试获得互斥的所有权而不阻塞|
|adopt_lock_t|假设调用方线程已拥有互斥的所有权|

```cpp
std::unique_lock<std::mutex> lock(mtx, std::try_to_lock); //尝试去锁
```


### [6. 死锁](#)
最常见的死锁原因自然是加锁操作的不规范，但即使是没有加锁，也是可以造成死锁的，例如自个关联一个线程的 std::thread ，若它们同时在对象的 std::thread实例上调用join()，这样也能够制造出死锁。
[**预防死锁的基本思想： 只要另一线程有可能在等待当前线程，那么当前线程千万不能反过来等待它，任何同步机制导致的循环等待都会导致死锁出现。**](#)

* **避免嵌套锁** 如果需要多个锁，一定要一次性使用 std::lock() 函数，借助单独的调用动作一次获取全部锁来避免死锁。
* **一旦持锁，就要避免调用由用户提供的程序结构**
* **依从固定顺序获取锁**
* **按照层级加锁** 规定锁的层级划分，就是要求按照特定方式规定加锁的顺序。
* **将准则推广到锁操作以外** 

#### [6.1 lock函数](#)
**std::lock()** 是C++ 11 标准库提供的专门解决死锁问题的函数，可以同时锁定多个互斥量而不造成死锁。
当然解锁方式只能是用 **std::lock_guard<std::mutex> guard2(mutex, std::adopt_lock);**

```cpp
template< class Lockable1, class Lockable2, class... LockableN >
void lock( Lockable1& lock1, Lockable2& lock2, LockableN&... lockn );
```

```cpp
std::mutex mutex_A;
std::mutex mutex_B;
int count = 0;

void notdeadLock(std::mutex& mx1, std::mutex& mx2){

   std::lock(mx1, mx2);//使用lock_guard来解锁！

   std::lock_guard<std::mutex> guard2(mx1, std::adopt_lock);
   std::cout <<"get the first mutex " << std::this_thread::get_id() << "\n" << std::flush;
   std::this_thread::sleep_for(std::chrono::milliseconds(10)); //小睡0.01s;

   std::lock_guard<std::mutex> guard2(mx2, std::adopt_lock);
   std::cout <<"get the second mutex " << std::this_thread::get_id() << "\n" << std::flush;
   std::this_thread::sleep_for(std::chrono::milliseconds(10)); //小睡0.01s;
}

int main(int argc, char *argv[]){

   std::thread t1([&]{ notdeadLock(mutex_A, mutex_B);});
   std::thread t2([&]{ notdeadLock(mutex_B, mutex_A);});
   
   t1.join();
   t2.join();
   return 0;
}
```

#### [6.2 try_lock函数](#)
尝试锁定每个给定的多个互斥量 ，通过以从头开始的顺序调用 try_lock 。
若调用 try_lock 失败，则不再进一步调用 try_lock ，并对任何已锁对象调用 unlock ，返回锁定失败对象的 0 底下标。
若调用 try_lock 抛出异常，则在重抛前对任何已锁对象调用 unlock 。 **成功时为 -1 ，否则为锁定失败对象的 0 底下标值。**

```cpp
template< class Lockable1, class Lockable2, class... LockableN >
int try_lock( Lockable1& lock1, Lockable2& lock2, LockableN&... lockn );
```

```cpp
int result = std::try_lock(foo_count_mutex, bar_count_mutex);
if (result == -1) {
    overall_count += foo_count + bar_count;
    foo_count = 0;
    bar_count = 0;
    std::cout << "overall: " << overall_count << '\n';
    foo_count_mutex.unlock();
    bar_count_mutex.unlock();
}
```

#### [6.3 锁粒度](#)
锁的粒度是一个摆手术语(hand-waving term)，用来描述一个锁保护着的数据量大小。 **可以简单的理解为，共享代码段的长短，越长粒度越粗，效率越低，越短，粒度越低，效率越高！**

* 一个细粒度锁(a fine-grained lock)能够保护较小的数据量
*  一个粗粒度锁(a coarse-grained lock)能够保护较多的数据量

简而言之，我们应当针对待保护数据量的大小选择合适的粒度，太大则过犹不及，太小则不足以保护

互斥锁保证了线程间的同步，但是却将并行操作变成了串行操作，这对性能有很大的影响，所以我们要尽可能的减小锁定的区域，也就是使用细粒度锁

如下所示，共享代码段也就几行，是细粒度锁！
```cpp
void createMessage(){
   while (true)
   {
      std::lock_guard<std::mutex> guard(msg_mutex);
      if (save_count < 3)
      {
         std::string ns = "this is a new message " + std::to_string(save_message_count);
         message_memory[save_message_count] =  ns;
         save_message_count ++;
         save_count ++;
      }
   }
}
```

### [7. 单次调用](#)
C++ 提供了once_flag 和 call_once **用于在初始化过程中保护共享数据**。对于需要利用共享资源的每一项操作，要先执行前判断该数据是否已经初始化，若没有，则及时初始化，然后方可使用。

```cpp
#include<iostream>
#include<mutex>
#include<list>
#include<thread>
#include<map>
#include <sstream> 
#include<numeric>

int counter = 0;
void connect(std::mutex & mtx) {
    //三个线程，只执行一次内容
    //这种写法效率太低
    std::unique_lock<std::mutex> lock(mtx, std::try_to_lock); //尝试去锁
    if (lock.owns_lock())
    {
        std::thread::id id = std::this_thread::get_id();
        std::stringstream strstream;
        strstream << id;
        std::string mystring = strstream.str();
        //执行
        if (counter == 0) //判断是否初始化
        {
            counter = 30; //对共享数据初始化
            printf("the thread: %s finish the work!\n", mystring.c_str());
        }
        else {
            printf("the thread: %s can not finish the work!\n", mystring.c_str());
        }
    }
}

int main(int argc, char* argv[]) {
    std::mutex single; //互斥量 只执行一次

    std::thread t1(connect, std::ref(single));
    std::thread t2(connect, std::ref(single));
    std::thread t3(connect, std::ref(single));
    std::thread t4(connect, std::ref(single));


    t1.join();
    t2.join();
    t3.join();
    t4.join();

    printf("the counter: %d \n", counter);
    return 0;
}
```

> 设计模式：程序灵活、维护方便、但是别人接管、阅读代码会非常痛苦！国外是现有项目模块化拆分需求，再有设计模式。我们是现有设计模式，再有程序！
>
> 不需要为了设计模式而设计模式，很多人写程序就是拿着设计模式去套程序，这里要用一个设计模式，那里要用一个设计模式。
就一个几千行，一两万行代码非要搞个模块拆分，弄一堆设计模式，有必要吗？ 代码搞这么晦涩干吗，别人阅读起来很难受的。

#### [7.1 once_flag，call_once](#)
在多线程中，有一种场景是某个任务只需要执行一次，可以用 `C++11`中的 `std::call_once`函数配合 `std::once_flag`来实现。多个线程同时调用某个函数，`std::call_once`可以保证多个线程对该函数只调用一次。 

```cpp
struct once_flag
{
    constexpr once_flag() noexcept;
    once_flag(const once_flag&) = delete;
    once_flag& operator=(const once_flag&) = delete;
};
template<class Callable, class ...Args>
void call_once(once_flag& flag, Callable&& func, Args&&... args);
```

```cpp
int counter = 0;

std::once_flag _flag;

void connect(std::mutex & mtx) {
    //三个线程，只执行一次内容
    std::call_once(_flag, []() {
        std::thread::id id = std::this_thread::get_id();
        std::stringstream strstream;
        strstream << id;
        std::string mystring = strstream.str();
        counter += 30; //对共享数据初始化
        printf("the thread: %s finish the work!\n", mystring.c_str());
    });
}

int main(int argc, char* argv[]) {
    std::mutex single; //互斥量 只执行一次

    std::thread t1(connect, std::ref(single));
    std::thread t2(connect, std::ref(single));
    std::thread t3(connect, std::ref(single));
    std::thread t4(connect, std::ref(single));


    t1.join();
    t2.join();
    t3.join();
    t4.join();

    printf("the counter: %d \n", counter);
    return 0;
}
```

**如果方法是类的成员函数**

```cpp
std::call_once(con_flag, &className::get_connection, this); //类内部调用
std::call_once(con_flag, &className::get_connection, &objName); //类外部调用
```

#### [7.2 单例设计模式](#)
**单例模式**就是：一个类**只能**有一个实例，并提供对该实例的全局访问点。通俗地说，就是一个类只能创建一个对象，并且在程序的任何地方都能够访问到该对象。

**例如：配置文件类**

```cpp
//单例类
class Application
{
public:
    ~Application(){
        printf("Singleton disapper! \n");
    };
    const std::string & Name() {
        return appName;
    }
private:
    std::string appName;
    //私有化构造函数
    Application(std::string name){
        if (name != "")
        {
            appName = name;
        }
        printf("Singleton create! \n");
    };
    //私有类
    class GcApplication
    {
    public:
        ~GcApplication() {
            if (Application::application != nullptr)
            {
                delete application;
                Application::application = nullptr;
            }
        };
    };
public:
    //使用静态函数创建单例类
    static Application* application;
    static Application* getApplicaionSingleton(std::string name = "") {
        if (application == nullptr)
        {
            application = new Application(name);
            static GcApplication gc; //用于释放 Application
        }
        return application;
    }
};
//静态数据初始化
Application * Application::application = nullptr;
```

**使用单例类**

```cpp
Application* app = Application::getApplicaionSingleton("Unix App");
cout << app->Name() << "\n"; //Unix App

Application* tpp = Application::getApplicaionSingleton("Remix App");
cout<< tpp->Name() << "\n";  //Unix App

Application* epp = Application::getApplicaionSingleton("Kick App");
cout << epp->Name() << "\n"; //Unix App

Application* wpp = Application::getApplicaionSingleton();
cout << wpp->Name() << "\n"; //Unix App
```

#### [7.3 单例多线程设计模式](#)

单例类如果在主线程 main中创建是没有问题的，在创建其他线程之前初始化。那么一切是没问题的，就怕它不是这样运行的，现在中一般可能在多线程中环境中初始化。这时候就有大问题了，需要一些设计方式。 使用 once_flag，call_once来解决。

```cpp
std::once_flag single_flag;

//单例类
class Application
{
public:
    ~Application(){
        printf("Singleton disapper! \n");
    };
    const std::string & Name() {
        return appName;
    }
private:
    std::string appName;
    //私有化构造函数
    Application(std::string name){
        if (name != "")
        {
            appName = name;
        }
        printf("Singleton create! \n");
    };
    class GcApplication
    {
    public:
        ~GcApplication() {
            if (Application::application != nullptr)
            {
                delete application;
                Application::application = nullptr;
            }
        };
    };
public:
    //使用静态函数创建单例类
    static Application* application;
    static Application* getApplicaionSingleton(std::string name = "") {
        //多线程环境中，只执行一次
        std::call_once(single_flag, [&]() {
            if (application == nullptr)
            {
                application = new Application(name);
                static GcApplication gc; //用于释放 Application
            }
         });
        return application;
    }
};
//静态数据初始化
Application * Application::application = nullptr;

void func(const char* name) {
    Application* app = Application::getApplicaionSingleton(name);
    printf("%s \n", app->Name().c_str());
}

int main(int argc, char* argv[]) {
    
    std::thread t1(func, "Unix Applicaion");
    std::thread t2(func, "Remix Applicaion");
    std::thread t3(func, "Kick Applicaion");
    std::thread t4(func, "Unix App");

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
```

**不适应 call_one 的解决方案：**

```cpp
static Application* getApplicaionSingleton(std::string name = "") {
    //多线程环境中，只执行一次
    if (application == nullptr) //双重判断
    {
        std::unique_lock<std::mutex> lock(mtx_sin);
        if (application == nullptr)
        {
            application = new Application(name);
            static GcApplication gc; //用于释放 Application
        }
    }
    return application;
}
```



### [8. 其他互斥量](#)
C++提供了多个互斥量, `std::timed_mutex` 增加了带时限的 `try_lock`。即 `try_lock_for`、`try_lock_until`。

* **std::timed_mutex** :  是能用于保护数据免受多个线程同时访问的同步原语。
* **std::recursive_mutex**:  **递归锁** 是同步原语，能用于保护共享数据免受从个多线程同时访问。`std::mutex` 在一个线程中不能反复加锁多次， `但是它可以`
* **std::recursive_timed_mutex**:   头文件位于： `#include<shared_mutex>`。 可以理解为 `timed_mutex` 和 `recursive_mutex` 的结合体！
* **std::shared_mutex** : **拥有二个访问级别：**  [**常常用于读写互斥，可以多个读，但是只能一个写，写的时候不能读！**](#)
  * *共享* - 多个线程能共享同一互斥的所有权。
  * *独占性* - 仅一个线程能占有互斥。
* **std::shared_timed_mutex**,  这是shared_mutex和timed_mutex的结合体，新增了 `try_lock_shared_for` 、 `try_lock_shared_until`方法，防止长期阻塞。

### [9 timed_mutex]()

`mutex` 获取锁失败时，线程会一直阻塞。 `timed_mutex` 带超时功能。在规定的等待时间内，没有获取锁，线程不会一直阻塞，代码会继续执行。

#### [9.1 构造函数](#)

```cpp
timed_mutex();
timed_mutex( const timed_mutex& ) = delete;
operator=  //[被删除]不可复制赋值
```

#### [9.2 方法](#)
* **void lock();** 锁定互斥。若另一线程已锁定互斥，则到 `lock` 的调用将阻塞执行，直至获得锁。错误发生时抛出 [std::system_error](https://zh.cppreference.com/w/cpp/error/system_error) 
* **void unlock()**; 解锁互斥
* **bool try_lock();**  尝试锁定互斥。立即返回。成功获得锁时返回 true ，否则返回 false 。
* **try_lock_for(time)** 尝试锁互斥。阻塞直到经过指定的 `timeout_duration` 。成功获得锁时返回 true ， 否则返回 false 。若 `timeout_duration` 小于或等于 `timeout_duration.zero()` ，则函数表现同 [try_lock()](https://zh.cppreference.com/w/cpp/thread/timed_mutex/try_lock) 。

```cpp
template< class Rep, class Period >
bool try_lock_for( const std::chrono::duration<Rep,Period>& timeout_duration );
```

* **try_lock_until()** 尝试所互斥。阻塞直至抵达指定的 `timeout_time` 或得到锁，阻塞的时长可以，但也可以不，小于或大于 timeout_time - Clock::now()。

```cpp
template< class Clock, class Duration >
bool try_lock_until( const std::chrono::time_point<Clock,Duration>& timeout_time );
//若成功获得锁则为 true ，否则为 false 。
```

使用方法：

```cpp
auto now=std::chrono::steady_clock::now();
test_mutex.try_lock_until(now + std::chrono::seconds(10));
```

**mutex 使用方法：**

```cpp
// 头文件 <mutex>
class timed_mutex; //(C++11 起)

std::timed_mutex myMutex;
std::chrono::milliseconds timeout(100);  //100毫秒
if (myMutex.try_lock_for(timeout))
{
	//在100毫秒内获取了锁
	//业务代码
	myMutex.unlock();  //释放锁
}else
{
	//在100毫秒内没有获取锁
	//业务代码
}
```

**使用例子：**

```cpp
int counter = 0;

std::once_flag _flag;

void connect(std::timed_mutex& mtx) {
    std::chrono::milliseconds timeout(100); //0.1s
    std::thread::id id = std::this_thread::get_id();
    std::stringstream strstream;
    strstream << id;
    std::string mystring = strstream.str();

    if (mtx.try_lock_for(timeout))
    {
        counter += 30; //对共享数据初始化
        printf("the thread: %s finish the work!\n", mystring.c_str());
        mtx.unlock();
    }
    else {
        printf("the thread: %s dont have the lock!\n", mystring.c_str());
    }

}

int main(int argc, char* argv[]) {
    std::timed_mutex single; //互斥量 只执行一次

    std::thread t1(connect, std::ref(single));
    std::thread t2(connect, std::ref(single));
    std::thread t3(connect, std::ref(single));
    std::thread t4(connect, std::ref(single));


    t1.join();
    t2.join();
    t3.join();
    t4.join();
    printf("the counter: %d \n", counter);
    return 0;
}
/*
the thread: 4156 finish the work!
the thread: 18640 finish the work!
the thread: 18928 finish the work!
the thread: 9008 finish the work!
the counter: 120
*/
```



### [10. recursive_mutex](#)
提供能被同一线程递归锁定的互斥设施，线程占有 `recursive_mutex`时，若其他线程要求 `recursive_mutex` 所有权，调用lock将被阻塞，调用`try_lock`将返回false。

> 可锁定`recursive_mutex`的最大次数未指定的，但到达该数后，对 lock 的调用将抛出 `std::system_error` 而对 `try_lock` 的调用返回`false`;
>
> 若 `recursive_mutex` 在仍被线程占有时被销毁，则程序行为未定义。`recursive_mutex` 满足 `mutex`和 标准布局类型的所有要求。


* **void lock();** 锁定互斥。若另一线程已锁定互斥，则到 `lock` 的调用将阻塞执行，直至获得锁。错误发生时抛出 [std::system_error](https://zh.cppreference.com/w/cpp/error/system_error) 
* **void unlock()**; 解锁互斥
* **bool try_lock();**  尝试锁定互斥。立即返回。成功获得锁时返回 true ，否则返回 false 。

  
```cpp
class X {
    std::recursive_mutex m;
    std::string shared;
public:
    void fun1() {
        std::lock_guard<std::recursive_mutex> lk(m);
        shared = "fun1";
        std::cout << "in fun1, shared variable is now " << shared << '\n';
    }
    void fun2() {
        std::lock_guard<std::recursive_mutex> lk(m);
        shared = "fun2";
        std::cout << "in fun2, shared variable is now " << shared << '\n';
        fun1(); // 递归锁在此处变得有用
        std::cout << "back in fun2, shared variable is " << shared << '\n';
    };
};

int main(int argc, char* argv[]) {
    X x;
    std::thread t1(&X::fun1, &x);
    std::thread t2(&X::fun2, &x);
    t1.join();
    t2.join();

    return 0;
}
```



**调用方线程在从它成功调用 lock 或 try_lock 开始的时期里占有 recursive_mutex 。此时期间，**调用方线程**可以多次锁定/解锁互斥元。结束的时候lock与unlock次数匹配正确就行。**

```cpp
void func1(std::recursive_mutex &mtx.lock();, const char * name) {
    mtx.lock();
    mtx.lock();
    printf("the thread: %s finish the work!\n", name);
    mtx.unlock();
    mtx.unlock();
}
```



#### [10.1 递归加锁](#)
如下代码，如果不使用递归锁，而使用 `std::mutex` 将会直接报错。

`func1`函数和`func2`函数都获取了同一个锁，而`func2`函数又会调用`func1`函数。如果`mtx` 锁是个非递归锁，则这个程序会立即死锁。因此在为一段程序加锁时要格外小心，否则很容易因为这种调用关系而造成死锁。

```cpp

void func1(std::recursive_mutex &mtx, const char * name) {
    std::lock_guard<std::recursive_mutex> lock(mtx);
    printf("the thread: %s finish the work!\n", name);
}

void func2(std::recursive_mutex& mtx, const char* name) {
    std::lock_guard<std::recursive_mutex> lock(mtx);
    func1(mtx, name);
    printf("the thread: %s finish the work!\n", name);
}

int main(int argc, char* argv[]) {
    std::recursive_mutex mtx;

    std::thread t1(func1, std::ref(mtx), "001");
    std::thread t2(func2, std::ref(mtx), "002");
    t1.join();
    t2.join();

    return 0;
}
```

### [11. recursive_timed_mutex](#)
以类似 std::recursive_mutex 的方式， recursive_timed_mutex 提供排他性递归所有权语义。另外， recursive_timed_mutex 通过 try_lock_for 与 try_lock_until 方法，提供带时限地试图要求 recursive_timed_mutex 所有权的能力。

**方法和构造函数与timed_mutex一样！ 只是多了可以递归加锁的功能！**

```cpp
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <sstream>
 
std::mutex cout_mutex; // 控制到 std::cout 的访问
std::recursive_timed_mutex rt_mutex;
 
void job(int id)
{
    using Ms = std::chrono::milliseconds;
    std::ostringstream stream;
 
    for (int i = 0; i < 3; ++i) {
        if (rt_mutex.try_lock_for(Ms(100))) {
            stream << "success ";
            std::this_thread::sleep_for(Ms(100));
            rt_mutex.unlock();
        } else {
            stream << "failed ";
        }
        std::this_thread::sleep_for(Ms(100));
    }
    std::lock_guard<std::recursive_timed_mutex> lock(rt_mutex);
    std::cout << "[" << id << "] " << stream.str() << "\n";
}
 
int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(job, i);
    }
 
    for (auto& i: threads) {
        i.join();
    }
```

### [12. shared_mutex](#)
在头文件 `#include <shared_mutex>`  **若一个线程已获取独占性锁（通过** **lock 、 try_lock** ）**，则无其他线程能获取该锁（包括共享的）**。仅当任何线程均未获取独占性锁时，共享锁能被多个线程获取（通过 **lock_shared 、 try_lock_shared** ）。**C++ 17** 新出的具有独占模式和共享模式的锁。共享模式能够被 **std::shared_lock** 占有。[**在一个线程内，同一时刻只能获取一个锁（共享或独占性）。**](#)

**std::shared_mutex** 是读写锁，提供两种访问权限的控制：**共享性（shared）和排他性（exclusive）**



#### [12.1 构造函数](#)
若构造不成功则抛出 **std::system_error **。

```cpp
shared_mutex();
shared_mutex( const shared_mutex& ) = delete;
operator=() = delete;
```



#### [12.2 排他性锁定](#)
* **void lock();** 锁定互斥。若另一线程已锁定互斥，则到 `lock` 的调用将阻塞执行，直至获得锁。错误发生时抛出 **std::system_error**。
* **void unlock()**; 解锁互斥
* **bool try_lock();**  尝试锁定互斥。立即返回。成功获得锁时返回 true ，否则返回 false 。



####  [12.3 共享锁定](#)
* **void lock_shared();** 获得互斥的共享所有权。若另一线程以排他性所有权保有互斥，则到 `lock_shared` 的调用将阻塞执行，直到能取得共享所有权。
* **bool try_lock_shared()**; 尝试以共享模式锁定互斥。立即返回。成功获得锁时返回 true ，否则返回 false 。
* **void unlock_shared();**  将互斥从调用方线程的共享所有权释放。



```cpp
#include<shared_mutex>

std::mutex msg_mutex;
std::map<int, std::string> message_memory;
int Counter = 0;

void writeMessage(std::shared_mutex &mtx) {
    int i = 120;
    while (i-- > 0)
    {
        mtx.lock();
        Counter++; 
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(30)); //小睡0.01s;
    }
}

void readMessage(std::shared_mutex& mtx) {
    int i = 100;
    while (i-- > 0)
    {
        if (mtx.try_lock_shared())
        {
            printf("[%d] counter: %d \n", i, Counter);
            mtx.unlock_shared();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30)); //小睡0.01s;
    }
}

int main(int argc, char* argv[]) {
    std::shared_mutex share_mtx;

    std::thread rt1(readMessage, std::ref(share_mtx));
    std::thread rt2(readMessage, std::ref(share_mtx));
    std::thread cr(writeMessage, std::ref(share_mtx));


    rt1.join();
    rt2.join();
    cr.join();
    return 0;
}
```



#### [12.4 共享读互斥写](#)
这是一个官方给出的例子。

```cpp
#include <iostream>
#include <mutex>  // 对于 std::unique_lock
#include <shared_mutex>
#include <thread>
 
class ThreadSafeCounter {
 public:
  ThreadSafeCounter() = default;
 
  // 多个线程/读者能同时读计数器的值。
  unsigned int get() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return value_;
  }
 
  // 只有一个线程/写者能增加/写线程的值。
  void increment() {
    std::lock_guard<std::shared_mutex> lock(mutex_);
    value_++;
  }
 
  // 只有一个线程/写者能重置/写线程的值。
  void reset() {
    std::lock_guard<std::shared_mutex> lock(mutex_);
    value_ = 0;
  }
 
 private:
  mutable std::shared_mutex mutex_;
  unsigned int value_ = 0;
};
 
int main() {
  ThreadSafeCounter counter;
 
  auto increment_and_print = [&counter]() {
    for (int i = 0; i < 3; i++) {
      counter.increment();
      std::cout << std::this_thread::get_id() << ' ' << counter.get() << '\n';
      // 注意：写入 std::cout 实际上也要由另一互斥同步。省略它以保持示例简洁。
    }
  };
 
  std::thread thread1(increment_and_print);
  std::thread thread2(increment_and_print);
 
  thread1.join();
  thread2.join();
}
```



### [13. shared_lock](#)
类 shared_lock 是通用**共享互斥所有权包装器（unique_lock则是独占互斥所有权包装器）**，允许延迟锁定、定时锁定和锁所有权的转移。**锁定 shared_lock ，会以共享模式锁定关联的共享互斥**（`std::unique_lock` 可用于以排他性模式锁定）,`shared_lock` 类可移动，但不可复制。 

#### [13.1 构造函数](#)

```cpp
shared_lock() noexcept;
shared_lock( shared_lock&& other ) noexcept;
explicit shared_lock( mutex_type& m );
```


#### [13.2 方法](#)
* **void lock();** 以共享模式锁定关联互斥。等效于调用 mutex()->lock_shared() 。
* **void unlock()**; 解锁互斥
* **bool try_lock();**  尝试以共享模式锁定关联互斥而不阻塞。立即返回。成功获得锁时返回 true ，否则返回 false 。
* **try_lock_for(time)** 尝试以共享模式锁定关联互斥。阻塞直至经过 `timeout_duration` 或得到锁，之先来到者。成功获得锁时返回 true ，否则返回 false 。等效于调用 mutex()->try_lock_shared_for(timeout_duration) 。
* **try_lock_until(time_point)** 试图以共享模式锁定关联互斥。阻塞直至抵达给定的 `timeout_time` 或得到互斥，之先到来者。成功获得互斥时返回 true ，否则返回 false 。可能阻塞长于直至抵达 `timeout_time` 。


```cpp
#include <shared_mutex>

std::shared_timed_mutex m;
int i = 10;
using namespace std::chrono;
 
void read()
{
   // 两个线程都获得对整数 i 的访问
   std::shared_lock<std::shared_timed_mutex> slk(m);
   std::cout << "read i as " << i << "...\n"; // 此不同步
   std::this_thread::sleep_for(10ms);
   std::cout << "woke up...\n";
}
 
int main()
{
   std::thread(read).join();
   std::thread(read).join();
   return 0;
}
```








-----
`时间`: `[既要、又要、还要..., 大力⋯努力⋯全力⋯，并且要紧抓⋯狠抓⋯齐抓]` 