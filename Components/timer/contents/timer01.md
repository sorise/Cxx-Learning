### [C++ 定时器 - 八股文](#)
**介绍**:  C++ 八股取士、经典题目！

----

定时任务的组织方式：时间序(过期时间 ：红黑树、最小堆)、执行序(跳表、时间轮)



#### 1. 定时器的作用，为什么要有定时器

一个服务器运行驱动逻辑有三种方式：**定时事件**、**网络事件**、**信号事件**。其通常与网络模块协同处理。

* 基于事件驱动业务的开展

* 复用系统调用



#### 2. 实现定时器的几种数据结构


* 单线程定时器：红黑树、最小堆实现方案 `----->` 通常与网络事件协调处理，可以使用epoll。
* 多线程定时器：跳表、时间轮方案 ，专用一个线程来运行定时器 使用 usleep

  * 跳表，可以通过加锁的方式**提供并发读写**。
  * 时间轮，linux 内核定时器采用的方式, 锁的粒度非常小。 **skynet** 、**netty**、 **kafka**。



* 最小堆：libevent(最小二叉堆)、go、libev(最小四叉堆)
* 红黑树：nginx、workflow


#### 2.1 网络单线程定时器
协调网络处理，服用epoll_wait

```cpp
#include <sys/epoll.h>

int epfd = epoll_create();

const int event_max_size = 64;

struct epoll_event ev[event_max_size] = {0};

while(true){
    /*
     -1
     0 没有事件立即返回，有事就拷贝到ev数组中
     t > 0 阻塞等待 tms
    */
    //timeout 可以取定时任务离当前的时间
    auto n = epoll_wait(epfd, ev, event_max_size, timeout);
    
    for(int i = 0; i < event_max_size; i++){
        /* 处理网络事件 */
    }
    //取出触发的定时任务，执行定时任务。
}
```



#### 3. 最小堆、红黑树的问题

两者在插入和删除时的时间复杂度是 O(log<sup>n</sup>) , <span style="color:#0238e3;font-weight:600">**基于有序队列的计时器中，所维护的计时器总数量越多，则每次用户创建新的计时器时的延迟越高，在需要反复创建大量计时器的场合下，性能不佳**</span> 。

