### [C++ 并发操作的同步](#)
**介绍**：我们不仅需要保护共享数据，还需要一些操作来令独立线程的行为同步，让多个线程按照一定的顺序执行，这就叫线程的同步！

-----

- [x] [1. 并发线程同步概述](#1-并发线程同步概述)
- [x] [2. 条件变量机制](#2-条件变量机制)
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

想要使用条件变量，需要配合 `mutex、unique_lock` 使用！

因为条件变量也是一种变量，可能会有多个线程对其访问，因此在使用条件变量前，我们需要锁定一个 `mutex`，然后这样才可以做好并发操作

核心类是：**condition_variable** 和 **condition_variable_any** 。

### [3 condition_variable](#)
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

如果传递给 **wait** 函数的`predicate`可调用对象，执行后返回false，那么直接解锁互斥量，并堵塞到本行(wait所在行)！
堵塞到其他某个线程调用`notify_one`或`notify_all`函数为止, 然后重新在互斥量上获得锁，然后再次测试`predicate`，如果
返回false，则直接继续堵塞，返回true, 唤醒线程，返回wait。此时互斥量还是被当前线程锁住的!

若wait函数不传递第二个参数，当作直接返回false,直接堵塞，堵塞到其他某个线程调用`notify_one`或`notify_all`函数为止！

如果执行后返回true，wait函数直接返回！直接之后的代码！

#### [3.3 使用例子](#)
向队列中输入一些数据，然后使得一个线程去打印，有数据的时候就打印，没数据的时候就等待！

```cpp
void PrintData(std::mutex &mx, std::queue<int> &channel, std::condition_variable &condition, bool isPrint){
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

输入数据的函数，在主线程运行！
```cpp
void use_condition_variable(){
    std::mutex mtx;
    std::queue<int> channel;
    std::condition_variable condition;
    bool isPrint = false;

    std::thread task(PrintData, std::ref(mtx), std::ref(channel),std::ref(condition), std::ref(isPrint));

    //第一次
    std::unique_lock<std::mutex> u(mtx);
    condition.wait(u, [&](){return !isPrint;});
    for (int i = 0; i < 10; ++i) {
        channel.push(i);
    }
    isPrint = true;
    u.unlock();
    condition.notify_all();

    task.join();
}

int main(int argc, char *argv[]){
    use_condition_variable();
    return 0;
}
```

### [4.](#) 

### [5.](#) 

-----

`时间`: `[]` 