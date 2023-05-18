### [libevent Bufferevent 事件](#)
 **介绍**： bufferevent 由一个底层的传输端口(如套接字 ),一个读取缓冲区和一个写入缓冲区组成。

-----

- [x] [1. bufferevent](#1-bufferevent)
- [x] [2. bufferevent 创建和销毁](#2-bufferevent-创建和销毁)
- [x] [3. 启动 bufferevent](#3-启动-bufferevent)
- [x] [4. bufferevent 设置](#4-bufferevent-设置)
- [x] [5. 操作bufferevent中的数据](#5-操作bufferevent中的数据)

-----

### [1. bufferevent](#)
与通常的事件在底层传输端口已经就绪,可以读取或者写入的时候执行回调不同的是,bufferevent 在读取或者写入了足够量的数据之后调用用户提供的回调。

有多种共享公用接口的 bufferevent 类型:
* **基于套接字的 bufferevent** :使用 `event_*` 接口作为后端,通过底层流式套接字发送或者接收数据的 bufferevent。
* **异步 IO bufferevent** :使用 `Windows IOCP` 接口,通过底层流式套接字发送或者接收数据的 bufferevent(仅用于 Windows,试验中)。
* **过滤型 bufferevent** :将数据传输到底层 bufferevent 对象之前,处理输入或者输出数据的 bufferevent:比如说,为了压缩或者转换数据。
* **成对的 bufferevent** :相互传输数据的两个 bufferevent。

> 当前 bufferevent 只能用于像 TCP 这样的面向流的协议,将来才可能会支持 像 UDP 这样的面向数据报的协议。

每个 bufferevent 都有一个输入缓冲区和一个输出缓冲区 ,它们的类型都是“structevbuffer”。 

有数据要写入到 bufferevent 时,添加数据到输出缓冲区 ;bufferevent 中有数据供读取的时候,从输入缓冲区抽取(drain)数据。

#### [1.1 回调](#)
bufferevent 有三个回调函数，一个读取回调和一个写入回调、事件回调。
* **默认情况下**,从底层传输端口读取了 **任意** 量的数据之后会调用读取回调。
* 输出缓冲区中足够量的数据被清空到底层传输端口后写入回调会被调用。
* 事件回调 当与bufferevent关联的socket连接发送关闭、发生异常的时候触发事件。

调 bufferevent_setcb 去设置回调函数：
```cpp
void bufferevent_setcb(
    struct bufferevent *bufev,
    bufferevent_data_cb readcb, 
    bufferevent_data_cb writecb,
    bufferevent_event_cb eventcb, 
    void *cbarg);

typedef void (*bufferevent_data_cb)(struct bufferevent *bev, void *ctx);
typedef void (*bufferevent_data_cb)(struct bufferevent *bev, void *ctx);
typedef void (*bufferevent_event_cb)(struct bufferevent *bev, short what, void *ctx);
```

#### [1.2 水位](#)
每个 bufferevent 有四个水位, 可以控制回调何时调用！

* **读取低水位** : 读取操作使得输入缓冲区的数据量在此级别或者更高时 ,读取回调将被调用。**默认值为 0**,所以每个读取操作都会导致读取回调被调用。
* **读取高水位** : 输入缓冲区中的数据量达到此级别后, bufferevent 将停止读取,直到输入缓冲区中足够量的数据被抽取 ,使得数据量低于此级别 。默认值是无限 ,所以永远不会因为输入缓冲区的大小而停止读取。
* **写入低水位** : 写入操作使得输出缓冲区的数据量达到或者低于此级别时 ,写入回调将被调用。**默认值是 0**,所以只有输出缓冲区空的时候才会调用写入回调。
* **写入高水位** : bufferevent 没有直接使用这个水位。它在 bufferevent 用作另外一个 bufferevent 的底层传输端口时有特殊意义，例如过滤型bufferevent。

#### [1.3 bufferevent 事件类型标志](#)
bufferevent 目前有六个个事件类型，读写、读到文件结尾、发生错误、超时、连接达成(可用于客户端)！

```cpp
#define BEV_EVENT_READING	0x01	/**< error encountered while reading */
#define BEV_EVENT_WRITING	0x02	/**< error encountered while writing */
#define BEV_EVENT_EOF		0x10	/**< eof file reached 连接关闭了 */
#define BEV_EVENT_ERROR		0x20	/**< unrecoverable error encountered */
#define BEV_EVENT_TIMEOUT	0x40	/**< user-specified timeout reached */
#define BEV_EVENT_CONNECTED	0x80	/**< connect operation finished. */
```
### [2. bufferevent 创建和销毁](#) 
bufferevent创建和回调函数设置是分开进行的。


#### [2.1 创建基于套接字的bufferevent](#)
基于套接字的 bufferevent 是最简单的,它使用 libevent 的底层事件机制来检测底层网络套 接字是否已经就绪,可以进行读写操作,并且使用底层网络调用(如 readv 、writev 、 WSASend、WSARecv)来发送和接收数据。

如果想以后设置文件描述符,可以设置fd为-1。
```cpp
struct bufferevent * bufferevent_socket_new(
    struct event_base *base, 
    evutil_socket_t fd, 
    enum bufferevent_options options
);

enum bufferevent_options {
	/** 如果设置了，那么当这个bufferevent被释放时，我们将关闭底层文件描述符 fd/bufferevent/anything。 */
	BEV_OPT_CLOSE_ON_FREE = (1<<0),

	/** 如果设置了，并且启用了线程处理，则此缓冲区上的操作受锁保护 */
	BEV_OPT_THREADSAFE = (1<<1),

	/** 如果设置了，回调将在事件循环中延迟运行。*/
	BEV_OPT_DEFER_CALLBACKS = (1<<2),

	/** 默认情况下,如果设置 bufferevent 为线程安全 的,则 bufferevent 会在调用
        用户提供的回调时进行锁定。设置这个选项会让 libevent 在执行回调的时候不进行锁定。*/
	BEV_OPT_UNLOCK_CALLBACKS = (1<<3)
};
```

#### [2.2 在bufferevent上启动链接](#)
address 和 addrlen 参数跟标准调用 connect()的参数相同。如果还没有为bufferevent 设置套接字,调用函数将为其分配一个新的流套接字,并且设置为非阻塞的。
```cpp
int bufferevent_socket_connect(
    struct bufferevent *bev,
    struct sockaddr *address, 
    int addrlen
);
```
常用于客户端操作！

调用bufferevent_socket_connect() 将告知libevent 套接字还未连接,直到连接成功之前不应该对其进行读取或者写入操作。

如果使用 bufferevent_socket_connect() 发起连接,将只会收 到BEV_EVENT_CONNECTED 事件。如果自己调用 connect(),则连接上将被报告为写入事 件。

#### [2.3 延迟回调](#)
默认情况下,bufferevent 的回调在相应的条件发生时立即被执行 。(evbuffer 的回调也是这样的,随后会介绍)在依赖关系复杂的情况下 ,这种立即调用会制造麻烦 。

比如说,假如某个回调在 evbuffer A 空的时候向其中移入数据 ,而另一个回调在evbuffer A 满的时候从中取出数据。这些调用都是在栈上发生的,在依赖关系足够复杂的时候,有栈溢出的风险。

要解决此问题,可以请求 bufferevent(或者 evbuffer)延迟其回调。条件满足时,延迟回调不会立即调用,而是在 event_loop()调用中被排队,然后在通常的事件回调之后执行。


#### [2.4 释放 bufferevent事件](#)
这个函数释放 bufferevent。bufferevent 内部具有引用计数,所以,如果释放 时还有未决的延迟回调,则在回调完成之前 bufferevent 不会被删除。

如果设置了 BEV_OPT_CLOSE_ON_FREE 标志,并且 bufferevent 有一个套接字或者底层 bufferevent 作为其传输端口,则释放 bufferevent 将关闭这个传输端口。
```cpp
void bufferevent_free(struct bufferevent *bufev);
```

#### [2.5 获得回调函数和参数](#)
尚且不知道干嘛用阿！

```cpp
void bufferevent_getcb(struct bufferevent *bufev,
bufferevent_data_cb *readcb_ptr,
bufferevent_data_cb *writecb_ptr,
bufferevent_event_cb *eventcb_ptr,
void **cbarg_ptr);
```

### [3. 启动 bufferevent](#) 
可以启用或者禁用 bufferevent 上的 EV_READ、EV_WRITE 或者 EV_READ |EV_WRITE 事件。没有启用读取或者写入事件时, bufferevent 将不会试图进行数据读取或者写入。

```cpp
void bufferevent_enable(struct bufferevent *bufev, short events);
void bufferevent_disable(struct bufferevent *bufev, short events);
short bufferevent_get_enabled(struct bufferevent *bufev);
```
默认情况下,新创建的 bufferevent 的写入是启用的,但是读取没有启用。 可以调用bufferevent_get_enabled()确定 bufferevent 上当前启用的事件。

events 参数：
```cpp
//e_event 事件处理方式参数：
#define EV_TIMEOUT 0x01 // 超时事件
#define EV_READ 0x02    // 读事件
#define EV_WRITE 0x04   // 写事件
#define EV_SIGNAL 0x08  // 信号事件
#define EV_PERSIST 0x10 // 周期性触发
#define EV_ET 0x20 // 边沿模式
#define EV_FINALIZE     0x40
#define EV_CLOSED	0x80

#define BEV_EVENT_READING	0x01	/**< 读取时遇到错误 */
#define BEV_EVENT_WRITING	0x02	/**< 写入时遇到错误 */
#define BEV_EVENT_EOF		0x10	/**< eof file reached 连接关闭了 */
#define BEV_EVENT_ERROR		0x20	/**< unrecoverable error encountered */
#define BEV_EVENT_TIMEOUT	0x40	/**< user-specified timeout reached */
#define BEV_EVENT_CONNECTED	0x80	/**< connect operation finished. */
```

### [4. bufferevent 设置](#)
如何设置水位线，如何设置超时时间！

#### [4.1 水位线设置](#)
bufferevent_setwatermark()函数调整单个 bufferevent 的读取水位、写入水位,或者同时调整二者。
(如果 events 参数设置了 EV_READ,调整读取水位。如果 events设置了 EV_WRITE 标志,调整写入水位)。
```cpp
void bufferevent_setwatermark(
    struct bufferevent *bufev, 
    short events,
    size_t lowmark,  //底水位
    size_t highmark); //高水位
```

```cpp
bufferevent_setcb(bev, read_callback,write_callback, event_callback, arg);
bufferevent_enable(bev, EV_WRITE | EV_READ | EV_CLOSED | BEV_EVENT_EOF);
bufferevent_setwatermark(bev, EV_READ, 20, 100); //只有已经获取20字节了，才会执行回调
bufferevent_setwatermark(bev, EV_WRITE, 40, 500); //只有已经写入40字节了，才会执行回调， 写高暂时无效
```
对于高水位, **0表示“无限”** 。

#### [4.2 超时时间设置](#)
函数可以设置超时时间,第一个参数是bev，第二个参数是读超时时间，第三个是写超时时间。

```cpp
int bufferevent_set_timeouts(
    struct bufferevent *bufev, 
    const struct timeval *timeout_read, 
    const struct timeval *timeout_write
);
```
使用方法：
```cpp
struct timeval val {3,0};
bufferevent_set_timeouts(bev, &val, nullptr);
```

#### [4.3 设置 bufferevent 优先级](#)
这个函数调整 bufferevent 的优先级为 pri。

```cpp
int bufferevent_priority_set(struct bufferevent *bufev, int pri);
```

### [5. 操作bufferevent中的数据](#) 
通过bufferevent得到evbuffer，如果只是通过网络读取或者写入数据 ,而不能观察操作过程,是没什么好处的，bufferevent 提供了下列函数用于观察要写入或者读取的数据。

将数据 data使用bufferevent送到写缓冲区，等数据写完毕以后，触发写回调 writecb。

```cpp
int bufferevent_write(struct bufferevent *bufev,const void *data, size_t size);
```
当读缓冲有数据以后触发回调函数 readcb --> 使用 bufferevent_read读取数据

从读缓冲区读取数据，然后将读到的数据冲缓冲区清空！
```cpp
size_t bufferevent_read(struct bufferevent *bufev, void *data, size_t size);
```

#### [5.1 主动触发](#)
bufferevent_trigger()函数的作用是触发指定bufferevent缓冲事件对象的读写事件或异常事件。
```cpp
void bufferevent_trigger(struct bufferevent *bufev, short iotype,int options);
```
* bufev：指向要触发事件的bufferevent缓冲事件对象的指针。
* events：表示要触发的事件类型，可以是EV_READ、EV_WRITE或者EV_EVENT组合。
* options：可选项，为0表示仅触发一次事件，为BEV_TRIG_DEFERRED表示延迟触发事件。

需要注意的是，bufferevent_trigger()函数并不会真正地读取或写入数据，而只是模拟触发相应的事件，从而让事件回调函数被调用。

例如，以下代码可以手动触发一个读取事件：

```cpp
bufferevent_trigger(bufev, BEV_EVENT_READ, 0);
```

#### [5.2 bufferevent 的清空操作](#)
清空 bufferevent 要求 bufferevent 强制从底层传输端口读取或者写入尽可能多的数据 ,而忽略其他可能保持数据不被写入的限制条件 。

```cpp
int bufferevent_flush(struct bufferevent *bufev,
short iotype, enum bufferevent_flush_mode state);
```
iotype 参数应该是 EV_READ、EV_WRITE 或者 EV_READ | EV_WRITE,用于指示应该处 理读取、写入,还是二者都处理。 state 参数可以是 BEV_NORMAL、
BEV_FLUSH 或者 BEV_FINISHED。BEV_FINISHED 指示应该告知另一端,没有更多数据需要发送了; 而 BEV_NORMAL 和 BEV_FLUSH 的区别依赖于具体的bufferevent 类型。

### [6. evBuffer](#)
bufferevent的两个读写缓存区就是两个evbuffer对象， libevent 的 evbuffer 实现了为向后面添加数据和从前面移除数据而优化的字节队列。

**evbuffer 用于处理缓冲网络 IO 的“缓冲”部分**。
缓冲区由evbuffer和evbuffer_chain组成，其中evbuffer_chain是真正存储数据的一块内存，通过链表将一个一个的evbuffer_chain连接在一起，组成内存池。

<img src="./assets/20180425105042967.jpeg" width="500px"  />


#### [6.1 创建和释放 evbuffer](#)
这两个函数的功能很简明: evbuffer_new() 分配和返回一个新的空 evbuffer, 而evbuffer_free()释放 evbuffer 和其内容。

```cpp
struct evbuffer *evbuffer_new(void);

void evbuffer_free(struct evbuffer *buf);
```

#### [6.2 evbuffer与线程安全](#)
默认情况下,在多个线程中同时访问 evbuffer 是不安全的。如果需要这样的访问,可以调 用 evbuffer_enable_locking() 。

如果 lock 参数为 NULL , libevent 会使用evthread_set_lock_creation_callback 提供的锁创建函数创建一个锁 。否则,libevent 将 lock 参数用作锁。
```cpp
int evbuffer_enable_locking(struct evbuffer *buf, void *lock);
void evbuffer_lock(struct evbuffer *buf);
void evbuffer_unlock(struct evbuffer *buf);
```
evbuffer_lock()和 evbuffer_unlock()函数分别请求和释放 evbuffer 上的锁。

可以使用这两个函数让一系列操作是原子的。如果 evbuffer 没有启用锁,这两个函数不做任何操作。

> 注意:对于单个操作,不需要调用 evbuffer_lock()和evbuffer_unlock(): 如果 evbuffer启用了锁,单个操作就已经是原子的 。只有在需要多个操作连续执行 ,不让其他线程介入的时候,才需要手动锁定 evbuffer。

#### [6.3 检查evbuffer](#)
evbuffer_get_length 返回 evbuffer 存储的字节数。

```cpp
size_t evbuffer_get_length(const struct evbuffer *buf);
//这个函数返回 evbuffer 存储的字节数,它在2.0.1-alpha 版本中引入。
```

这个函数返回连续地存储在 evbuffer 前面的字节数。evbuffer 中的数据可能存储在多个分隔
开的内存块中，这个函数返回当前第一个块中的字节数。
```cpp
size_t evbuffer_get_contiguous_space(const struct evbuffer *buf);
```

#### [6.4 获取bufferevent的读缓冲区和写缓冲区](#)
获取读写缓冲区bufferevent_get_input和bufferevent_get_output。

```cpp
struct evbuffer *bufferevent_get_input(struct bufferevent *bufev);

struct evbuffer *bufferevent_get_output(struct bufferevent *bufev);
```

#### [6.5 buffer](#)
bufferevent_write_buffer 移除 buf 的所有内 容,将其放置到输出缓冲区的末尾。成功时这些函数都返回 0,发生错误时则返回-1。

```cpp
int bufferevent_write_buffer(struct bufferevent *bufev, struct evbuffer *buf);
```

函数抽空输入缓冲区的所有内容,将其放置到 buf 中,成功时返 回0,失败时返回 -1。
```cpp
int bufferevent_read_buffer(struct bufferevent *bufev, struct evbuffer *buf);
```

### [7. bufferevent 过滤器](#)
bufferevent filter 过滤器可以在读取前和写入后对数据进行一系列的预处理操作，比如压缩和加密。
对用户来讲，过滤器就像一个黑盒子，用户仍然只需要完成读取和写入，而不必知晓过滤器中的操作。

#### [7.1 bufferevent_filter_new](#) 
要使用过滤器，需要先明白 bufferevent_filter_new 函数和结构体 evbuffer 。
```cpp
struct bufferevent *
    bufferevent_filter_new(
    struct bufferevent *underlying,
    bufferevent_filter_cb input_filter,
    bufferevent_filter_cb output_filter,
    int options,
    void (*free_context)(void *),
    void *ctx
);
```

### [X. 类型获取 bufferevent](#)

#### [X.X 获取与设置](#)
这些函数设置或者返回基于 fd 的事件的文件描述符。只有基于套接字的 bufferevent 支持 setfd()。两个函数都在失败时返回-1；setfd()成功时返回0。

```cpp
int bufferevent_setfd(struct bufferevent *bufev, evutil_socket_t fd);

evutil_socket_t bufferevent_getfd(struct bufferevent *bufev);
```

### [X.2 bufferevent_get_underlying](#)
返回作为底层传输的另一个bufferevent，当然只适用于filtering bufferevents。

```cpp
struct bufferevent *bufferevent_get_underlying(struct bufferevent *bufev);
```

### [X.3 加锁](#)
有时候需要确保对 bufferevent 的一些操作是原子地执行的。为此， libevent 提供了手动锁定和解锁 bufferevent 的函数。

```cpp
void bufferevent_lock(struct bufferevent *bufev);
void bufferevent_unlock(struct bufferevent *bufev);
```

