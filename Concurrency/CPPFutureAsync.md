### [C++ 并发操作的同步](#)
**介绍**：我们不仅需要保护共享数据，还需要一些操作来令独立线程的行为同步，让多个线程按照一定的顺序执行，这就叫线程的同步！

-----

- [x] [1. 并发线程同步概述](#1-并发线程同步概述)
- [x] [2. 条件变量机制](#2-条件变量机制)
- [x] [3. condition_variable](#3-condition_variable)
- [x] [4. 创建一个后台任务与获得返回值](#4-创建后台任务与返回值)
- [x] [5. future](#5-future)

-----

### [1. 并发线程同步概述](#)
我们不仅需要保护共享数据，还需要一些操作来令独立线程的行为同步，让多个线程按照一定的顺序执行，**多个线程在以下几个场景中需要进行同步**。

* 线程等待另一个线程完成，才可以开始执行自己的任务。 [**线程执行的任务有先后顺序要求**](#)
* 线程等待某一个特定事件的发生，或者是线程等待某个条件变为true，才可以开始执行。  [**事件/条件等待**](#)
* 限定线程等待一定时间 [**阻塞等待**](#)

#### [1.1 处理办法](#)
首先想到的一种方法是：**通过互斥量设置条件标识**，线程A完成任务后，就修改条件，让线程B去运行。但是这种方法十分浪费，因为**需要反复循环检测条件标识**，浪费CPU处理时间。

其次是：**回调模式**，线程A执行完任务以后唤醒B线程去执行。C++在头文件`<condition_variable>`中提供了许多的工具来完成这个功能！

#### [1.2 先写后读演示](#)
使用条件变量实现一个生产消费者，创建两个线程，一个写数据，一个读数据。总共读写五十次！

写进程：
```cpp
void writeToChannel(std::mutex &mx, std::queue<int> &channel, std::condition_variable &condition){
    int count = 50;
    while (count > 0){
        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::uniform_int_distribution generator(0,10);
        int data_ = generator(engine);
        if ( data_ < 4 ){
            const std::lock_guard<std::mutex> guard(mx);
            channel.push(data_);
            printf("[%d] write data is: [%d]\n",51-count, data_);
            count--;
            condition.notify_one();
        }
        //wait 0.5 seconds
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
}
```

读进程：
```cpp
void readFromChannel(std::mutex &mx, std::queue<int> &channel, std::condition_variable &condition){
    int count = 50;
    while (count > 0){
       std::unique_lock<std::mutex> lk(mx);
       condition.wait(lk,[&]()->bool {return !channel.empty();});
       //条件判断成功了 wait函数会自动将 lk的互斥量解锁，然后再锁住。
       int data_ = channel.front();
       channel.pop();
       printf("read data is: [%d]\n",data_);
       lk.unlock();
       count--;
    }
}
```

测试运行：
```cpp
void TestConditionVariable(){
    std::mutex mtx;
    std::queue<int> channel;
    std::condition_variable condition;

    std::thread writer(writeToChannel,std::ref(mtx),std::ref(channel),std::ref(condition));
    std::thread reader(readFromChannel,std::ref(mtx),std::ref(channel),std::ref(condition));

    writer.join();
    reader.join();
}

/*
[1] write data is: [3]
read data is: [3]
[2] write data is: [3]
read data is: [3]
[3] write data is: [0]
read data is: [0]
[4] write data is: [3]
read data is: [3]
[5] write data is: [3]
read data is: [3]
[6] write data is: [0]
read data is: [0]
*/
```

### [2. 条件变量机制](#) 
条件变量是允许多个线程相互交流的同步原语。它允许一定量的线程等待（可以定时）另一线程的提醒，然后再继续。条件变量始终关联到一个互斥，
在标头 `<condition_variable>` 定义。

想要使用条件变量，需要配合 `mutex、unique_lock` 使用！

因为条件变量也是一种变量，可能会有多个线程对其访问，因此在使用条件变量前，我们需要锁定一个 `mutex`，然后这样才可以做好并发操作

核心类是：**condition_variable** 和 **condition_variable_any** 。

### [3. condition_variable](#)
当 `std::condition_variable` 对象的某个 `wait` 函数被调用的时候，它使用 `std::unique_lock` (通过 `std::mutex`) 来锁住当前线程。
当前线程会一直被阻塞，直到另外一个线程在相同的 `std::condition_variable` 对象上调用了 `notification` 函数来唤醒当前线程。

`std::condition_variable` 对象通常使用 `std::unique_lock<std::mutex` 来等待，如果需要使用另外的 `lockable` 类型，可以使用 `std::condition_variable_any` 类

#### [3.1 构造函数](#)
构造函数非常简单，只有一个无参默认构造函数，不支持、复制赋值、复制、移动操作！

|构造函数|解释|
|:----|:----|
|condition_variable()|构造 std::condition_variable 类型对象。|
|condition_variable(const condition_variable&) = delete| 复制构造函数被删除。|

#### [3.2 函数](#)
**condition_variable** 常用成员函数。

|函数|解释|
|:----|:----|
|wait(unique_lock<>,predicate)|当predicate为false时wait()函数才会使线程等待，在收到其他线程通知时只有当pred返回true时才会被唤醒。|
|wait_for(unique_lock<>,chrono::duration)|等待一段时间|
|wait_for(unique_lock<>,chrono::duration,predicate)|直到条件变量被唤醒，或到指定时限时长后|
|wait_until(unique_lock<>,chrono::duration)|阻塞当前线程，直到条件变量被唤醒，或直到抵达指定时间点|
|wait_until(unique_lock<>,chrono::duration,predicate)|阻塞当前线程，直到条件变量被唤醒，或直到抵达指定时间点|
|notify_one()|通知一个正在等待的线程|
|notify_all()|通知所有正在等待的线程|

```cpp
std::unique_lock<std::mutex> lk(mx);
auto predicate = [&]()->bool {return !channel.empty();}
condition.wait(lk, predicate);
int data_ = channel.front();
```

#### [3.3 wait方法解释](#)
如果传递给 **wait** 函数的`predicate`可调用对象，执行后返回false，那么直接解锁互斥量，并堵塞到本行(wait所在行)！

堵塞到其他某个线程使用 condition_variable对象 调用`notify_one`或`notify_all`函数为止, 然后重新在互斥量上获得锁，然后再次测试`predicate`，如果
返回false，则直接继续堵塞，返回true, 唤醒线程，返回wait。此时互斥量还是被当前线程锁住的!

若wait函数不传递第二个参数，当作直接返回false,直接堵塞，堵塞到其他某个线程调用`notify_one`或`notify_all`函数为止！

如果执行后返回true，wait函数直接返回！直接执行之后的代码！

#### [3.4 使用例子](#)
向队列中输入一些数据，然后使得一个线程去打印，有数据的时候就打印，没数据的时候就等待！

```cpp
void PrintData(std::mutex &mx, std::queue<int> &channel, std::condition_variable &condition, bool& isPrint){
    while (true){ //一直运行等待打印！
        printf("print thread start ----- \n");
        std::unique_lock<std::mutex> unique(mx);
        condition.wait(unique,[&]()->bool {return isPrint;});
        printf("print start ----- \n");
        while (!channel.empty()){
            int &v = channel.front();
            channel.pop();
            printf("channel value: %d\n",v);
        }
        isPrint = false;//打印完成！
        unique.unlock();
    }
}
```

输入数据的函数，在主线程运行， 使得其输出两次！
```cpp
void use_condition_variable(){
    std::mutex mtx;
    std::queue<int> channel;
    std::condition_variable condition;
    bool isPrint = false;
    std::thread task(PrintData, std::ref(mtx), std::ref(channel),std::ref(condition), std::ref(isPrint));
    task.detach();

    //等一段时间
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //等待0.2秒
    std::unique_lock<std::mutex> lock(mtx);
    for (int i = 0; i < 10; ++i) {
        channel.push(i);
    }
    isPrint = true;
    std::mutex* t= lock.release();
    t->unlock();
    t = nullptr;
    condition.notify_all();//通知


    //等一段时间
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::unique_lock<std::mutex> tlock(mtx);
    for (int i = 0; i < 10; ++i) {
        channel.push(i);
    }
    isPrint = true;
    std::mutex* t2= tlock.release();
    t2->unlock();
    t2 = nullptr;
    condition.notify_all();//通知

    std::cin.get();
}


int main(int argc, char *argv[]){
    use_condition_variable();
    return 0;
}
```

### [4. 创建一个后台任务与获得返回值](#) 
这一类并发操作应该是很常见的，比如说开启一个线程执行一个计算任务，然后获得返回值，进行下一步操作。
C++提供了 **std::futurn** 、**std:async** API来实现个功能,它们定义在头文件 **#include\<future\>** 定义

**启动一个异步任务**: 在C++的意识是启动一个线程执行对应的入口函数，然后返回一个 `std::future` 对象！

|函数|解释|
|:----|:----|
|future|等待被异步设置的值 (类模板)，类似于unique_ptr。|
|shared_future|等待被异步设置的值（可能为其他 future 所引用）,类似于 shared_ptr。|
|async|函数模板 async 异步地运行函数 （潜在地在可能是线程池一部分的分离线程中），并返回最终将保有该函数调用结果的 std::future 。|
|promise|存储一个值以进行异步获取|
|packaged_task|打包一个函数，存储其返回值以进行异步获取|
|future_status|枚举 - 指定 std::future 和 std::shared_future 的 wait_for 和 wait_until 函数所返回的 future 状态。|
|launch|枚举 - 指定 std::async 所用的运行策略|

```cpp
//future_status 枚举值:
enum class future_status {
    ready,
    timeout,
    deferred
};

//launch 枚举值：
enum class launch : /* unspecified */ {
    async =    /* 运行新线程，以异步执行任务 */,
    deferred = /* 调用方线程上首次请求其结果时执行任务（惰性求值） */,
    /* implementation-defined */
};
```

**使用例子:**
```cpp
int add_special(const int & a,const int &b){
    std::cout << "secondary thread ID: " <<std::this_thread::get_id() << std::endl << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return a*b+a*a*b*b+a+b;
}

int main(int argc, char *argv[]){
    std::cout << "Main thread ID: " <<std::this_thread::get_id() << std::endl << std::flush;
    int point_x = 10;
    int point_y = 5;
    std::future<int> result = std::async(add_special, std::ref(point_x) , std::ref(point_y));
    int value = result.get();//将会卡在这里 500毫秒，等待结果返回！
    std::cout<<"result: " << value << "\n" << std::flush;
    return 0;
}
```

#### [4.1 async](#)
std::async 是一个**函数模板**，用来启动一个异步任务。相对于 thread ，std::future 是更高级的抽象，异步返回结果保存在 std::future 中，使用者可以不必进行线程细节的管理。

std::async 有两种启动策略：
1. std::launch::async:：函数必须以异步方式运行，即创建新的线程。
2. std::launch::deferred：函数只有在 std:async 所返回的期值的 get 或 wait 得到调用时才执行、并且调用方会阻塞至运行结束，否则不执行。

若没有指定策略，则会执行默认策略，将会由操作系统决定是否启动新的线程。
```cpp
template< class Function, class... Args>
std::future<std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>>
    async( Function&& f, Args&&... args );

template< class Function, class... Args >
std::future<std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>>
    async( std::launch policy, Function&& f, Args&&... args );
```

**它传参数的方式和 std::thread是一样的！**

```cpp
int start_value = 6;
int times = 5;
std::future<double> result = std::async(std::launch::deferred, [](int& start, int& time){
    double real = 0;
    std::mt19937_64 engine(start);
    std::normal_distribution<double> generator(10,16);
    for (int i = 0; i < time; i++)
        real += generator(engine);
    return real;
},std::ref(start_value), std::ref(times));//按引用传递

int value = result.get();
std::cout << "result: " << value<< std::endl;
```

### [5 future](#)
类模板 std::future 提供访问异步操作结果的机制, （通过 std::async 、 std::packaged_task 或 std::promise 创建的）异步操作能提供一个 std::future 对象给该异步操作的创建者。

然后，异步操作的创建者能用各种方法查询、等待或从 std::future 提取值。若异步操作仍未提供值，则这些方法可能阻塞。

异步操作准备好发送结果给创建者时，它能通过修改链接到创建者的 std::future 的共享状态（例如 std::promise::set_value ）进行。

**构造函数：** 只有无参默认构造函数和移动构造函数，不支持拷贝。

#### [5.1 接口方法](#)
常用的方法只有一个 get 方法， 多了解一点就是wait。

|函数|解释|
|:----|:----|
|get()|获取异步任务返回的结果，如果任务线程已经执行完毕，直接返回，否则阻塞等待！|
|valid()|检查 future 是否拥有共享状态|
|wait()|等待结果变得可用|
|std::future_status wait_for(duration) const;|等待结果，如果在指定的超时间隔后仍然无法得到结果，则返回。|
|wait_until|等待结果，如果在已经到达指定的时间点时仍然无法得到结果，则返回。|

```cpp
int add_special(const int & a,const int &b);

std::future<int> result = std::async(add_special, std::ref(point_x) , std::ref(point_y));
```

#### [5.2 配合launch](#)
使用launch去制定执行策略：

```cpp
std::future<int> result = std::async(std::launch::deferred, 
                add_special, std::ref(point_x) , std::ref(point_y));
//如果result不调 get或者wait方法，线程都不会执行
//执行任务的线程甚至可能是当前线程，而不是新开一个线程
```

### [6. packaged_task](#) 
类模板 std::packaged_task **包装**任何可调用 (Callable) 目标（函数、 lambda 表达式、 bind 表达式或其他函数对象），使得能
异步调用它。其返回值或所抛异常被存储于能通过 std::future 对象访问的共享状态中，**可以作为线程池的构件单元！**

```cpp
template< class R, class ...Args >
class packaged_task<R(Args...)>;
```
td::packaged_task 是多态、具分配器的容器：可在堆上或以提供的分配器分配存储的可调用对象。


使用例子：
```cpp
double get_random_value(const int& start,const int& time){
    double real = 0;
    std::mt19937_64 engine(start);
    std::normal_distribution<double> generator(10,16);
    for (int i = 0; i < time; i++)
    real += generator(engine);
    //等等 2 秒
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return real;
}

int main(int argc, char *argv[]){
    int start_value = 6;
    int times = 5;

    std::packaged_task<double(const int& start,const int& time)> myTaskPkg(get_random_value);

    std::thread task_runner(std::ref(myTaskPkg), std::ref(start_value), std::ref(times));
    task_runner.join();

    std::future<double> result = myTaskPkg.get_future();

    std::cout << "result: "<< result.get() << std::endl;
    //result: 75.4892
    return 0;
}
```

#### [6.1 构造函数](#)
默认构造函数，支持移动操作，不支持复制拷贝，但是支持赋值移动操作符。

```cpp
packaged_task() noexcept;
template <class F>
explicit packaged_task( F&& f );
packaged_task( packaged_task&& rhs ) noexcept;
template <class F, class Allocator>
explicit packaged_task( std::allocator_arg_t, const Allocator& a, F&& f );
```

#### [6.2 接口方法](#)
常用的方法只有一个 get 方法， 多了解一点就是wait。

|函数|解释|
|:----|:----|
|get_future()|返回与承诺的结果关联的 std::future！|
|valid()|检查任务对象是否拥有合法函数|
|operator()|执行函数|
|reset|重置状态，抛弃任何先前执行的存储结果|

#### [6.3 直接调用packaged_task](#)
packaged_task对象也是一个可调用对象， 是可以直接调用的，前提是已经具有可调用函数了。

```cpp
int start_value = 6;
int times = 5;

std::packaged_task<double(const int& start,const int& time)> myTaskPkg(get_random_value);
myTaskPkg(start_value, times);
std::future<double> result = myTaskPkg.get_future();

std::cout << "result: "<< result.get() << std::endl;
```

#### [6.4 任务容器](#)
以下有两个函数，将这个两个函数包装以下，然后用一个列表作为容器，再使用线程进行执行。

```cpp
double get_random_value(const int& start,const int& time){
    double real = 0;
    std::mt19937_64 engine(start);
    std::normal_distribution<double> generator(10,16);
    for (int i = 0; i < time; i++)
    real += generator(engine);
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    return real;
}

double run(const int& start,const int& time){
    double real = 0;
    for (int i = 0; i < time; i++){
         real += (double)start * 3.14;
    }
    return real;
}
```

```cpp
void taskRunner(std::list<std::packaged_task<double(const int& start,const int& time)>> &taskContainer){
    for (auto &&task: taskContainer) {
        task(12,10);
        std::future<double> result = task.get_future();
        std::cout << "result: " <<result.get() << std::endl;
    }
    //清空，因为任务已经运完了
    taskContainer.erase(taskContainer.begin(), taskContainer.end());
}
```

```cpp
int main(int argc, char *argv[]){
    //任务容器，存放各种想要被调用的任务
    std::list<std::packaged_task<double(const int& start,const int& time)>> taskContainer;

    //创建包装的任务
    std::packaged_task<double(const int& start,const int& time)> myTaskPkg_1(get_random_value);
    std::packaged_task<double(const int& start,const int& time)> myTaskPkg_2(run);

    //加入容器
    taskContainer.push_back(std::move(myTaskPkg_1));//必须使用移动操作，因为packaged_task不支持拷贝操作
    taskContainer.push_back(std::move(myTaskPkg_2));//移动完以后 myTaskPkg_x就是空了

    std::thread th(taskRunner, std::ref(taskContainer));

    th.join();
    return 0;
}
```

### [7. promise](#) 
这个东西**可以实现线程间的通信**，C++11中的std::promise是个模板类。一个std::promise对象可以存储由future对象(可能在另一个线程中)检索
的T类型的值或派生自std::exception的异常，并提供一个同步点。


-----

`时间`: `[2023-04-03 19:53]` 