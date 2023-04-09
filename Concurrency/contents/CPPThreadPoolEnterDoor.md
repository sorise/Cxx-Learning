### [C++ 线程池](#)
 **介绍**：计算机的资源是有限的，线程也需要栈空间，线程切换也需要付出代价，频繁的创建和销毁线程会产生大量的程序资源消耗，为此线程池孕育而生！

-----
- [x] [1. 线程池概述](#1-线程池概述)
- [x] [2. 任务包装](#2-任务包装)
- [x] [3. 工作线程](#3-工作线程)
- [x] [4. remix线程池](#4-remix线程池)

-----

### [1. 线程池概述](#)
**线程本身需要栈空间本身也需要消耗内存，线程的启动和切换具有固定开销，运行的线程越多，OS所做的上下文切换就越频繁！**

* 编写代码中，“时不时地突然”创建一个线程，这种写法，一般情况下不会出错，但是不稳定的；
* 把一堆线程弄到一起，统一管理调度，循环利用的方式，就叫做线程池。
* 程序启动时，一次性创建好一定数量的线程。这种方式让人更放心，觉得程序代码更稳定。

线程池的本质就是提前创建一堆线程，在程序需要使用线程来执行任务的时候，将任务放置到线程池中交给未执行任务的线程执
行，从而提高了程序的运行效率，提高了线程的利用率。


#### [1.1 线程池数量问题](#)
实际要看具体的业务需求还有服务性配置情况，创建多线程完成业务；考虑可能被阻塞的线程数量，创建多余最大被阻塞线
程数量的线程，如100个线程被阻塞再充值业务，开110个线程就是很合适的。

#### [1.2 线程池组成部分](#)
线程池的组成主要分为3个部分，这三部分配合工作就可以得到一个完整的线程池：

<img width="700px" src="./resources/2244500_1638692410.png" />

* **任务队列**，存储需要处理的任务，由工作的线程来处理这些任务
    * 通过线程池提供的API函数，将一个待处理的任务添加到任务队列，或者从任务队列中删除
    * 已处理的任务会被从任务队列中删除
    * 线程池的使用者，也就是调用线程池函数往任务队列中添加任务的线程就是生产者线程
* **工作线程**（任务队列任务的消费者） ，N个
    * 线程池中维护了一定数量的工作线程, 他们的作用是是不停的读任务队列, 从里边取出任务并处理
    * 工作的线程相当于是任务队列的消费者角色，
    * 如果任务队列为空, 工作的线程将会被阻塞 (使用条件变量/信号量阻塞)
    * 如果阻塞之后有了新的任务, 由生产者将阻塞解除, 工作线程开始工作
* **管理者线程**（不处理任务队列中的任务），1个
    * 它的任务是周期性的对任务队列中的任务数量以及处于忙状态的工作线程个数进行检测
    * 当任务过多的时候, 可以适当的创建一些新的工作线程
    * 当任务过少的时候, 可以适当的销毁一些工作的线程

#### [1.3 线程池模式](#)
线程池根据其中线程的数量可以区分为两种模式：
* **fixed模式线程池**：线程池里面的线程个数是固定不变的，一般是ThreadPool创建时根据当前机器的CPU核心数量进行指定。
* **cached模式线程池**：线程池里面的线程个数是可动态增长的，根据任务的数量动态的增加线程的数量，但是会设置一个线程
数量的阈值（线程过多的坏处上面已经讲过了），任务处理完成，如果动态增长的线程空闲了60s还没
有处理其它任务，那么关闭线程，保持池中最初数量的线程即可。fixed模式线程池：
线程池里面的线程个数是固定不变的，一般是ThreadPool创建时根据当前机器的CPU核心数量进行指
定。

#### [1.4 线程池](#)
线程池的组成
* 维持一个队列
* 创建一定数量的线程，启动线程，调配任务，管理着线程池。
* 线程池目前只需要Start()启动方法，Stop()方法，AddTask()方法。
* Start()：创建一定数量的线程，进入线程循环。
* Stop()：停止线程循环，回收所有的线程。
* AddTask()：添加任务。

#### [1.5 工作者线程](#)
线程池中线程，在线程池中等待并执行任务。
该文使用条件变量condition_variable实现等待和唤醒进制。

### [2. 任务包装](#) 
将带执行的函数和参数包装为一个通用的任务，需要借助C++的新特性：**可变参数模板**、**std::forward**、**std::bind**、**std::packaged_task**、**std::future**。

#### [2.1 任务队列](#)
任务队列中的任务使用`std::function<void()>` 来表示，对于用户的参数使用`std::bind`来绑定到`std::packaged_task`中，并使用 `std::forward` 实现参数完美转发！

```cpp
using Task = std::function<void()>;  
std::queue<Task> waiting_tasks;
```
将任意函数转换为任务Task；
```cpp
template< class F, class ...Args >
auto Commit(F&& f, Args&&...args) -> std::future<decltype(f(args...))>
{
    using ReturnType = decltype(f(args...));
    std::shared_ptr<std::packaged_task<ReturnType()>> task = 
    std::make_shared<std::packaged_task<ReturnType()>>
    (
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    //加入队列
    waiting_tasks.emplace([task](){
        (*task)();
    });

    std::future<ReturnType> future = task->get_future();
    return future;
}
```

### [3. 工作线程](#)
只要线程池启动，工作线程进行while循环，我们先提供一些全局变量！

```cpp
std::queue<std::function<void()>> waiting_tasks; //任务队列
std::mutex _queue_mutex; //任务队列互斥量
std::condition_variable _queue_condition; //条件变量
std::atomic<bool> is_run = true; //表示当前线程是否在运行
```

工作线程执行的任务：
```cpp
void runner(){
    while (true){
        std::function<void()> task_run;
        {
            std::unique_lock<std::mutex> queue_lock(_queue_mutex);
            _queue_condition.wait(queue_lock, []{
                return !waiting_tasks.empty() || !is_run.load();  //线程池不运行了
            });
            if (!is_run.load()){
                printf("the thread pool finish!");
                return;
            }
            task_run = std::move(waiting_tasks.front());
            waiting_tasks.pop();
        }
        task_run();
    }
}
```

#### [3.1 测试运行](#)
一个任务队列，三个任务，两个工作线程！

```cpp
int main(int argc, char *argv[]){

    std::thread worker1(runner);
    std::thread worker2(runner);
    worker1.detach();
    worker2.detach();

    auto f = [](int i)->int{
        std::this_thread::sleep_for(std::chrono::milliseconds (500));
        const std::thread::id id = std::this_thread::get_id();
        std::stringstream strstream;
        strstream << id;
        const std::string thread_id = strstream.str();
        printf("runner id: %s\n", thread_id.c_str());
        const int r = 100;
        return i * r;
    };

    std::future<int> task1 = Commit(f, 51);
    std::future<int> task2 = Commit(f, 41);
    std::future<int> task3 = Commit(f, 31);

    std::cout << "result 1:" << task1.get() << "\n";
    std::cout << "result 2:" << task2.get() << "\n";
    std::cout << "result 3:" << task3.get() << "\n";

    std::this_thread::sleep_for(std::chrono::seconds(3));
    is_run = false;

    _queue_condition.notify_all(); //通知工作线程，检测是否线程池已经停止！
    std::cin.get();
    return 0;
}
```

### [4. remix线程池](#) 

```cpp
//
// Created by remix on 23-4-6.
//
#ifndef __REMIX_THREAD_POOL_H__
#define __REMIX_THREAD_POOL_H__
#include <thread>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <map>
#include <sstream>
#include <queue>
#include <condition_variable>
#include <random>
#include <chrono>
#include <future>
#include <list>
#include <atomic>

//将会使用 懒加载！
//线程池类型
enum class thread_pool_type{
    fixed = 0, //fixed 模式线程池
    flexible = 1 //flexible 动态增长
};

// 单例模式创建一个线程池
class remix_pool{
private:
    std::once_flag _init_flag;
    unsigned int limit = 128; //线程的最大数量
    using Task = std::function<void()>; //表示一个代执行任务
    std::queue<Task> _queue_tasks;            //任务队列
    unsigned int _thread_count; //线程池中的线程数量
    thread_pool_type _pool_type; //线程池类型
    std::vector<std::thread> _workers; //工作线程
    std::mutex _queue_mute; //互斥队列
    std::mutex _workers_mutex; //如果动态增长 则需要此物
    std::condition_variable _condition_tasks; //条件变量 用于阻塞或者唤醒线程
    std::atomic<bool> _is_run{false };     //线程池是否执行, 线程池可以被停止吗？
    std::atomic<int>  _unused_threads{ 0 };  //空闲线程数量
    std::shared_ptr<std::thread> _manager_thread; //管理者线程

    //私有化构造函数
    remix_pool(unsigned int count = 4, thread_pool_type type = thread_pool_type::fixed)
    :_thread_count(count),
     _pool_type(type),
     _manager_thread(nullptr)
    {
        //设置初始化参数！线程池创建要等到，有任务的时候！
        if (count >= limit){
            throw std::runtime_error("the count is grater than " + std::to_string(limit) + "! ");
        }
    }
    //用于回收
    class GC{
    public:
        ~GC(){
            if (remix_pool::pool != nullptr){
                delete pool;
                remix_pool::pool = nullptr;
            }
        }
    };
    void init(){
        //只能调用一次
        _is_run.store(true, std::memory_order_seq_cst); //线程开始运行
        _thread_count = (_thread_count == 0)?4:_thread_count; //初始化线程数量
        addThread(_thread_count); //填充工作线程
        _unused_threads.store(_thread_count); //未使用线程数量

        if (_pool_type == thread_pool_type::flexible){
            //启动管理线程
            _manager_thread = std::make_shared<std::thread>(
                [this]{
                    manager();
                }
             );
        }
    }
    void addThread(unsigned int count){
        if (count == 0) return;
        //只有运行中的线程池可以追加线程
        if (!_is_run.load(std::memory_order_acquire)){
            throw std::runtime_error("The thread pool has stopped running！");
        }
        {
            std::lock_guard<std::mutex> guard(_workers_mutex);
            for(unsigned int i = 0; i < count; i++){
                //工作线程
                _workers.emplace_back([this](){
                    //一直循环
                    while (true){
                        std::function<void()> runner;
                        {
                            std::unique_lock<std::mutex> unique{ _queue_mute };
                            //等待任务队列非空 或者 线程池已经停止！
                            _condition_tasks.wait(unique, [this]{
                                return !_queue_tasks.empty() ||!_is_run.load(std::memory_order_acquire);
                            });//
                            if (!_is_run.load(std::memory_order_acquire)){
                                printf("over!\n");
                                return ; //如果线程池已经停止了 直接返回
                            }
                            runner = std::move(_queue_tasks.front());
                            _queue_tasks.pop();
                            _unused_threads--; //有一个在工作了
                        }
                        runner(); //执行任务

                        if (_pool_type == thread_pool_type::flexible){
                            if (_unused_threads.load(std::memory_order_relaxed) >= 3){
                                //干掉一个空闲线程 but How？
                                return;
                            }
                        }
                        _unused_threads ++; //又有空闲线程了
                    }
                });
            }
        }
    }
    //管理者线程任务
    //周期性检查，任务队列中的剩余任务数量和空闲线程数量
    //剩余任务数量多就 增加
    //只有在 thread_pool_type = flexible 的时候才启动
    void manager(){
        while (true){
            //线程池已经停止则Over
            if (!_is_run.load(std::memory_order_relaxed)) {
                printf("manager over");
                return;
            };
            if (_queue_tasks.size() >= 3){
                addThread(1); //增加一个线程
            }
            //每隔一秒看看
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }


public:
    remix_pool(const remix_pool& other) = delete; //不允许赋值
    remix_pool(remix_pool&& other) = delete; //不允许移动
    remix_pool& operator=(const remix_pool& other) = delete; //不允许赋值拷贝
    remix_pool& operator=(remix_pool&& other) = delete; //不允许赋值拷贝

    //提交任务
    template< class F, class ...Args >
    auto Commit(F&& f, Args&&...args) -> std::future<decltype(f(args...))>
    {
        //懒加载 初始化
        std::call_once(_init_flag, [this]{
            init();
        });
        using ReturnType = decltype(f(args...));
        const std::shared_ptr<std::packaged_task<ReturnType()>>  task = 
        std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        //加入队列
        {
            const std::lock_guard<std::mutex> lock{ _queue_mute };
            _queue_tasks.emplace([task](){
                (*task)();
            });
        }
        std::future<ReturnType> future = task->get_future();
        _condition_tasks.notify_one(); //唤醒
        return future;
    }
    //等待所有的任务执行完毕
    void wait_all_done(){
        while (true){
            {
                std::lock_guard<std::mutex> guard(_queue_mute);
                if (_queue_tasks.empty()) break;
            }
            //等待 0.1s
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    //空闲线程数量
    int unusedThreadCount(){ return _unused_threads; }
    //线程数量
    int ThreadCount(){ return _thread_count; }

    //只能通过静态方法获取
    static remix_pool * pool;

    static remix_pool * createThreadPoolSingleton(int _thread_count, thread_pool_type _type){
        if (pool == nullptr)
        {
            pool = new remix_pool(_thread_count, _type);
            static GC gc;
        }
        return pool;
    }
    //不等任务执行完毕就停止线程池
    void stop_unsafe(){
        _is_run.store(false, std::memory_order_relaxed);
        _manager_thread->join();//杀死管理者线程
        _condition_tasks.notify_all(); // 唤醒所有线程执行 毁灭操作
        for (std::thread & worker: _workers) { //等待工作线程死掉
            if (worker.joinable()) worker.join();
        }
    }

    //等任务执行完毕就停止线程池
    void stop_safely(){
        wait_all_done(); //等待任务执行完成
        _is_run.store(false, std::memory_order_relaxed);
        _condition_tasks.notify_all(); // 唤醒所有线程执行 线程消亡
        _manager_thread->join();//杀死管理者线程
        for (std::thread & worker: _workers) { //等待工作线程死掉
            if (worker.joinable()) worker.join();
        }
    }

    bool state() {return _is_run.load(std::memory_order_relaxed);};

    //析构函数
    ~remix_pool(){
        _is_run.store(false, std::memory_order_relaxed);
        _condition_tasks.notify_all(); // 唤醒所有线程执行 毁灭操作
        for (std::thread & worker: _workers) { //等待工作线程死掉
            if (worker.joinable()) worker.join();
        }
    }
};

remix_pool * remix_pool::pool = nullptr;
#endif //__REMIX_THREAD_POOL_H__
```

#### [4.1 使用例子](#)

```cpp
remix_pool * pool = remix_pool::createThreadPoolSingleton(5, thread_pool_type::fixed);
std::cout << "thread pool: " <<pool->state() << "\n";


auto f = [](int i)->int{
    //std::this_thread::sleep_for(std::chrono::milliseconds (500));
    const std::thread::id id = std::this_thread::get_id();
    std::stringstream strstream;
    strstream << id;
    const std::string thread_id = strstream.str();
    printf("runner id: %s\n", thread_id.c_str());
    const int r = 100;
    return i * r;
};

std::future<int> task1 = pool->Commit(f, 51);
std::cout << "thread pool: " <<pool->state() << "\n";
std::future<int> task2 = pool->Commit(f, 41);
std::future<int> task3 = pool->Commit(f, 31);

std::cout << "result 1:" << task1.get() << "\n";
std::cout << "result 2:" << task2.get() << "\n";
std::cout << "result 3:" << task3.get() << "\n";



std::cout << "thread pool: " <<pool->state() << "\n";
```


-----
时间: [] 