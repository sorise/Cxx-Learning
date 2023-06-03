### [libevent event_base 详解](#)
 **介绍**： event_base配置方法。

-----

- [x] [1. event_base](#1-event_base)
- [x] [2. event_base优先级](#2-event_base优先级)
- [x] [3. 工具函数](#3-工具函数) 
- [x] [4. 事件循环event_loop](#4-事件循环event_loop)

-----

### [1. event_base](#)
使用 libevent 函数之前需要分配一个或者多个 event_base 结构体。一个event_base就是一个Reactor框架。我们在调用任何Libevent的函数前，我们都是需要先申请 event_base 结构体。
对于一个event_base结构来说，它会保存一系列的event事件并且以轮训的方式去关注每一个事件，去查看哪一个事件是就绪的。


**多线程环境中：** 如果设置 event_base 使用锁,则可以安全地在多个线程中访问它。 然而,其事件循环只能 运行在一个线程中。如果需要用多个线程检测 IO,则需要为每个线程使用一个 event_base。

#### [1.1 后端方法设置](#)
**I/O multiplex机制实例数组， 按优先顺序排列的后端方法数组**, 有epoll就使用epoll，没有判断是否可以使用kqueue，在判断使用使用poll、select、iocp。
```cpp
static const struct eventop *eventops[] = {
#ifdef EVENT__HAVE_EVENT_PORTS
	&evportops,
#endif
#ifdef EVENT__HAVE_WORKING_KQUEUE
	&kqops,
#endif
#ifdef EVENT__HAVE_EPOLL
	&epollops,
#endif
#ifdef EVENT__HAVE_DEVPOLL
	&devpollops,
#endif
#ifdef EVENT__HAVE_POLL
	&pollops,
#endif
#ifdef EVENT__HAVE_SELECT
	&selectops,
#endif
#ifdef _WIN32
	&win32ops,
#endif
	NULL
};
```

#### [1.2 创建复杂的event_base](#)
大多数场景使用方法 event_base_new 创建默认的 event_base就足够了， 但是如果希望对创建的 event_base 有更多的控制,就需要使用 event_config。

**event_config** 是一个容纳 event_base 配置信息的不透明结构体。需要 event_base时,将 event_config 传递给 event_base_new_with_config()。

|方法|说明|
|:---|:---|
|struct event_config *event_config_new(void);|创建一个配置项  |
|struct event_base \*event_base_new_with_config(const struct event_config \*cfg);|使用配置项创建 event_base  |
|void event_config_free(struct event_config *cfg);| 释放配置项 |

```cpp
//生成一个配置项
struct event_config *config = event_config_new();

/*
 对 event_config 调用其它函数,设置所需要的 event_base 特征
*/

//用配置项设置 event_base
auto base = event_base_new_with_config(config);

//释放配置项
event_config_free(config);
```
#### [1.3 配置项设置](#)
调用 event_config_avoid_method ()可以通过名字让 libevent 避免使用特定的可用后端 。调用 event_config_require_feature ()让 libevent 不使用不能提供所有指定
特征的后端。

|方法|说明|
|:---|:---|
|event_config_avoid_method|避免使用特定的可用后端   |
|event_config_require_feature|使用特定的可用后端|
|event_config_set_flag| 对libevent本身进行设置|

#### [1.4 event_config_avoid_method 方法](#)
event_config_avoid_method函数就是指出，避免使用指定的多路IO复用函数。其是通过字符串的方式指定的，即参数method。这个字符串将由队列元素
event_config_entry的avoid_method成员变量存储(由于是指针，所以更准确来说是指向)。

```cpp
int event_config_avoid_method(struct event_config *cfg, const char *method);
```
使用:
```cpp
event_config_avoid_method(config, "epoll");
event_config_avoid_method(config, "poll");
//这样再 linux系统中就只能使用 select 了
```

#### [1.5 event_config_require_feature 方法](#)
设置一些选项！还可以通过event_base_get_features获取特征！
```cpp
int event_config_require_features(struct event_config *cfg, enum event_method_feature feature);

enum event_method_feature {
    EV_FEATURE_ET = 0x01, //边沿触发，高效但是容易丢消息，注意与水平触发区分
    EV_FEATURE_O1 = 0x02, //要求具有很多事件的后台方法可以以近似O（1）处理事件；select和poll
    EV_FEATURE_FDS = 0x04, //后台方法可以处理包括sockets在内的各种文件描述符,这样就不能选择 epoll了
    EV_FEATURE_EARLY_CLOSE = 0x08
    /*
     要求后台方法可以使用EV_CLOSED检测链接关闭，而不需要读完所有未决数据才能判断
     支持EV_CLOSED的后台方法不是所有OS内核都支持的
    */
};
```

```cpp
int f = event_base_get_features(base);
if (f & EV_FEATURE_ET){
    std::cout << "open ET" << std::endl;
}
```

#### [1.6 event_config_set_flag 方法](#)
让libevent 在创建 event_base 时设置一个或者多个将在下面介绍的运行时标志。

```cpp
int event_config_set_flag(struct event_config *cfg, int flag);

enum event_base_config_flag {
    EVENT_BASE_FLAG_NOLOCK = 0x01,
    EVENT_BASE_FLAG_IGNORE_ENV = 0x02,
    EVENT_BASE_FLAG_STARTUP_IOCP = 0x04,
    EVENT_BASE_FLAG_NO_CACHE_TIME = 0x08,
    EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST = 0x10,
    EVENT_BASE_FLAG_PRECISE_TIMER = 0x20
};
```

**event_config_set_flag 解释：**
* **EVENT_BASE_FLAG_NOLOCK** 不要为 event_base 分配锁。设置这个选项可以为 event_base 节省一点用于锁定和解锁的时间,但是让在多个线程中访问event_base 成为不安全的 
* **EVENTBASE_FLAG_IGNORE_ENV** :选择使用的后端时,不要检测 EVENT* 环境变量。使用这个标志需要三思，这会让用户更难调试你的程序与 libevent 的交互。
* **EVENT_BASE_FLAG_STARTUP_IOCP**:仅用于 Windows,让 libevent 在启动时就 启用任何必需的 IOCP 分发逻辑,而不是按需启用。
    * `int event_config_set_num_cpus_hint(struct event_config *cfg, int cpus)`; 这个方法只在使用IOCP的Windows系统上有作用，固然在将来它也会被用在其他平台上。
        调用它用来告知event_base充分使用给定数量的CPU。注：这只是1个提议，event_base可能会使用多于或少于你所给定的值。
* **EVENT_BASE_FLAG_NO_CACHE_TIME** ：不是在事件循环每次准备执行超时回调时 检测当前时间,而是在每次超时回调后进行检测。注意:这会消耗更多的CPU时间。
* **EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST**： 告诉 libevent ,如果决定使 用 epoll 后端,可以安全地使用更快的基于 changelist 的后端，防止同一个fd多次激发事件，fd如果复制会有bug。在不使用 epoll 后端的情况下,这个标志是没有效果的。
* **EVENT_BASE_FLAG_PRECISE_TIMER**： 默认使用系统最快的计时机制，如果系统有较慢且更精确的则采用。

```cpp
event_config_set_flag(config, EVENT_BASE_FLAG_NOLOCK); //非多线程
```

#### [1.7 使用iocp的一些设置](#)
需要额外设置一些东西。 
```cpp
struct event_config *config = event_config_new();
event_config_set_flag(config, EVENT_BASE_FLAG_STARTUP_IOCP); 
//初始化 iocp的线程
evthread_use_windows_threads();
//设置cpu
SYSTEM_INFO si;
GetSystemInfo(&si);
event_config_set_num_cpus_hint(config, si);
```

### [2. event_base优先级](#) 
libevent支持为事件设置多个优先级。然而, event_base默认只支持单个优先级。可以调用 event_base_priority_init()设置 event_base 的优先级数目。

```cpp
int event_base_priority_init(struct event_base *base, int n_priorities);
```
成功时这个函数返回 0,失败时返回 -1。base 是要修改的 event_base,n_priorities是要支 持的优先级数目,这个数目至少是 1 。每个新的事件可用的优先级将从 0 (最高) 到 n_priorities-1(最低)。

常量 EVENT_MAX_PRIORITIES 表示 n_priorities 的上限。调用这个函数设置n_priorities时给出更大的值是错误的。

**必须在任何事件激活之前调用这个函数,最好在创建 event_base 后立刻调用。**

#### [2.1 配置例子](#)

```cpp
struct event_base* ConfigBase(){
    struct event_base * base = nullptr;
    struct event_config * config = event_config_new();

    //features、 flag
    event_config_require_features(config, EV_FEATURE_ET);
    event_config_set_flag(config,  EVENT_BASE_FLAG_NOLOCK);

    //创建 event_base
    base = event_base_new_with_config(config);

    //优先级
    event_base_priority_init(base, 128);

    //释放配置对象
    event_config_free(config);
    return base;
}
```

### [3. 工具函数](#)
libevent提供了很多工具函数，如果你自己创建 socket而不是使用evconnlistener的话，这些api还是很有用的！

```cpp
int evutil_make_socket_nonblocking(evutil_socket_t sock); //非阻塞
int evutil_make_listen_socket_reuseable(evutil_socket_t sock);//监听可复用
#define EVUTIL_CLOSESOCKET(s) evutil_closesocket(s) //关闭socket
//将一个IPv4或IPv6地址字符串转换为网络字节序格式的二进制地址
int evutil_inet_pton(int af, const char *src, void *dst);
```

### [4. 事件循环event_loop](#)
一旦创建好事件根基event_base，并且在根基上安插好事件之后，需要对事件循环监控(换句话说就是等待事件的到来，触发事件的回调函数)，有两种方式可以达到上面描述的功能，即： event_base_dispatch 和 event_base_loop:

```cpp
#define EVLOOP_ONCE 0x01
#define EVLOOP_NONBLOCK 0x02
#define EVLOOP_NO_EXIT_ON_EMPTY 0x04

int event_base_dispatch(struct event_base *);
//return 0：成功,   -1 ：发生错误，1 已退出循环，没有待监测或则激活的事件
int event_base_loop(struct event_base *base, int flags);
//return 0：成功,   -1 ：发生错误，1 已退出循环，没有待监测或则激活的事件
```
默认情况下,event_base_loop()函数运行 event_base 直到其中没有已经注册的事件为止。

参数 **flags**
* EVLOOP_ONCE： 相当于epoll_wait阻塞方式&&只调用一次
    * 当没有事件到来时，程序将一直阻塞在event_base_loop函数；直到有任意一个事件到来时，程序才不会阻塞在event_base_loop，将会继续向下执行。
* EVLOOP_NONBLOCK： 相当于epoll_wait非阻塞方式&&只调用一次 
    * 即使没有事件到来，程序也不会阻塞在 event_base_loop
* EVLOOP_NO_EXIT_ON_EMPTY： 等价于event_base_dispatch 
    * 一直循环监控事件 ⇒ 直到没有已经注册的事件 || 调用了 `event_base_loopbreak()` 或 `event_base_loopexit()` 为止

在 **event_base_dispatch** 函数中，实际上调用的是 **event_base_loop(event_base, 0)**；

**event_base_loop()的算法概要：**
```cpp
//还有注册事件再loop中 或者 EVLOOP_NO_EXIT_ON_EMPTY 被设置
while (any events are registered with the loop,
or EVLOOP_NO_EXIT_ON_EMPTY was set){
    if (EVLOOP_NONBLOCK was set, or any events are already active)
        If any registered events have triggered, mark them active.
    else
        Wait until at least one event has triggered, and mark itactive.

    for (p = 0; p < n_priorities; ++p) {
        if (any event with priority of p is active) {
            Run all active events with priority of p.
            break; 
            /* Do not run any events of a less important priority */
        }
    }
    if (EVLOOP_ONCE was set or EVLOOP_NONBLOCK was set) break;
}
```

#### [4.1 退出事件循环](#)
如果想在移除所有已注册的事件之前停止活动的事件循环，可以调用两个稍有不同的函数
```cpp
int event_base_loopexit(struct event_base *base, const struct timeval *tv);	

struct timeval
{
    long tv_sec; /* Seconds.  */
    long tv_usec; /* Microseconds.  */
};
```

让 event_base 在给定时间之后停止循环。如果 tv 参数为 NULL ，event_base 会立即停止循环，没有延时。如果 event_base 当前正在执行任何激活事件的回调，则回调会继续运行，直到运行完所有激活事件的回调之才退出
```cpp
int event_base_loopbreak(struct event_base *base);
```
让 event_base 立即退出循环(即使有其他正在执行任何激活事件的回调)。

有时候需要知道对 event_base_dispatch()或者 event_base_loop()的调用是正常退出的,还是因为调用 event_base_loopexit()或者 event_base_break()而退出的。可
以调 用下述函数来确定是否调用了 loopexit 或者 break函数。

```cpp
int event_base_got_exit(struct event_base *base);
int event_base_got_break(struct event_base *base);
```

#### [4.2 转储event_base的状态](#)
为帮助调试程序(或者调试 libevent),有时候可能需要加入到 event_base 的事件及其状态的完整列表。调用 event_base_dump_events()可以将这个列表输出到指定的
文件中。

```cpp
void event_base_dump_events(struct event_base *base, FILE *f);
```
这个列表是人可读的,未来版本的 libevent 将会改变其格式。