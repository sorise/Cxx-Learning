### [C++ 并发操作的同步](#)
**介绍**：我们不仅需要保护共享数据，还需要一些操作来令独立线程的行为同步，让多个线程按照一定的顺序执行，这就叫线程的同步！

-----

- [x] [1. 并发线程同步概述](#1-并发线程同步概述)
- [x] [2. ](#2-)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)

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

想要使用条件变量，需要配合mutex、unique_lock使用！

因为条件变量也是一种变量，可能会有多个线程对其访问，因此在使用条件变量前，我们需要锁定一个mutex，然后这样才可以做好并发操作

核心类是：**condition_variable** 和 **condition_variable_any** 。

#### [2.1 condition_variable](#)
当 std::condition_variable 对象的某个 wait 函数被调用的时候，它使用 std::unique_lock(通过 std::mutex) 来锁住当前线程。
当前线程会一直被阻塞，直到另外一个线程在相同的 std::condition_variable 对象上调用了 notification 函数来唤醒当前线程。

std::condition_variable 对象通常使用 `std::unique_lock<std::mutex` 来等待，如果需要使用另外的 lockable 类型，可以使用 std::condition_variable_any 类

condition_variable 常用成员函数:
- wait(unique_lock<>,predicate)：阻塞当前线程直到条件变量被唤醒
- notify_one()：通知一个正在等待的线程
- notify_all()：通知所有正在等待的线程

### [3.](#) 

### [4.](#) 

### [5.](#) 

-----

`时间`: `[]` 