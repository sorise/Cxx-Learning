### [libevent Bufferevent 事件](#)
 **介绍**： bufferevent 由一个底层的传输端口(如套接字 ),一个读取缓冲区和一个写入缓冲区组成。

-----

- [x] [1. bufferevent](#1-bufferevent)
- [x] [2. bufferevent 创建和销毁](#2-bufferevent-创建和销毁)
- [x] [3. 启动 bufferevent](#3-启动-bufferevent)
- [x] [4. bufferevent 设置](#4-bufferevent-设置)
- [x] [5. 操作bufferevent中的数据](#5-操作bufferevent中的数据)
- [x] [6. evBuffer](#6-evBuffer)
- [x] [7. 类型获取 bufferevent](#7-类型获取-bufferevent)

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
默认情况下,bufferevent 的回调在相应的条件发生时立即被执行 。(evbuffer 的回调也是这样的,随后会介绍)在依赖关系复杂的情况下 ,这种立即调用会制造麻烦 。

比如说,假如某个回调在 evbuffer A 空的时候向其中移入数据 ,而另一个回调在evbuffer A 满的时候从中取出数据。这些调用都是在栈上发生的,在依赖关系足够复杂的时候,有栈溢出的风险。

要解决此问题,可以请求 bufferevent(或者 evbuffer)延迟其回调。条件满足时,延迟回调不会立即调用,而是在 event_loop()调用中被排队,然后在通常的事件回调之后执行。
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

#### [2.3 释放 bufferevent事件](#)
这个函数释放 bufferevent。bufferevent 内部具有引用计数,所以,如果释放 时还有未决的延迟回调,则在回调完成之前 bufferevent 不会被删除。

如果设置了 BEV_OPT_CLOSE_ON_FREE 标志,并且 bufferevent 有一个套接字或者底层 bufferevent 作为其传输端口,则释放 bufferevent 将关闭这个传输端口。

```cpp
void bufferevent_free(struct bufferevent *bufev);
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

#### [4.3 设置优先级](#)
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

说明：
```cpp
struct evbuffer_chain;

struct evbuffer {  
    struct evbuffer_chain *first;  
    struct evbuffer_chain *last;  
    //这是一个二级指针。使用*last_with_datap时，指向的是链表中最后一个有数据的evbuffer_chain。  
    //所以last_with_datap存储的是倒数第二个evbuffer_chain的next成员地址。  
    //一开始buffer->last_with_datap = &buffer->first;此时first为NULL。所以当链表没有节点时  
    //*last_with_datap为NULL。当只有一个节点时*last_with_datap就是first。      
    struct evbuffer_chain **last_with_datap;  

    size_t total_len;//链表中所有chain的总字节数  

    ...  
}; 
```

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

#### [6.3 检查 evbuffer](#)
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
//返回输入缓冲区。用户不得在此缓冲区上设置回调。
struct evbuffer *bufferevent_get_output(struct bufferevent *bufev);
/*
返回输出缓冲区。用户不得在此缓冲区上设置回调。
当使用过滤器时，如果操作了输出缓冲区，则需要手动触发过滤器。
*/
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

#### [6.6 evbuffer_add*](#)
evbuffer_add这个函数添加 data 处的 datalen 字节到 buf 的末尾，成功时返回0，失败时返回-1。

```cpp
/**
 * @breif: 添加数据到 evbuffer 的结尾处
 *
 * @param buf: 待添加数据的 evbuffer 对象
 * @param data: 数据指针
 * @param datlen: 数据长度，单位 byte
 *
 * @return: 成功返回0，失败返回-1
 */
int evbuffer_add(struct evbuffer *buf, const void *data, size_t datlen);

int evbuffer_add_buffer(struct evbuffer* dst, struct evbuffer * src);
//evbuffer_add_buffer()将 src 中的所有数据移动到 dst 末尾，成功时返回0，失败时返回-1。

int evbuffer_remove_buffer(	struct evbuffer* src, struct evbuffer * dst,size_t datlen);
//evbuffer_remove_buffer()函数从 src 中移动 datlen 字节到 dst 末尾，尽量少进行复制。如
//果字节数小于 datlen，所有字节被移动。函数返回移动的字节数。
```

#### [6.7 evbuffer_prepend*](#)
如果想在链表的前面添加数据可以使用 evbuffer_prepend。

```cpp
int evbuffer_prepend(struct evbuffer* buf, const void * data, size_t size);
int evbuffer_prepend_buffer(struct evbuffer* dst, struct evbuffer *src);
```

这个函数修改缓冲区的最后一块，或者添加一个新的块，使得缓冲区足以容纳datlen 字节，而不需要更多的内存分配
```cpp
int evbuffer_expand(struct evbuffer* buf, size_t datlen);
```

#### [6.8 格式化添加数据](#)
evbuffer_add_printf 和 evbuffer_add_vprintf 添加格式化的数据到evbuffer的尾部 ,格式参数和其他参数的处理分别与C 库函数printf和vprintf 相同。
```cpp
int evbuffer_add_printf(struct evbuffer *buf, const char *fmt, ...)
#ifdef __GNUC__
  __attribute__((format(printf, 2, 3)))
#endif
;
```

```cpp
int evbuffer_add_vprintf(struct evbuffer *buf, const char *fmt, va_list ap)
#ifdef __GNUC__
	__attribute__((format(printf, 2, 0)))
#endif
;
```

```cpp
evbuffer_add_printf(buf, "Hello %s %d.%d.%d", "world", 2, 0, 1);
```

#### [6.9 重新排列 evbuffer 的内部布局](#)
有时候需要取出 evbuffer 前面的 N 字节，将其看作连续的字节数组。要做到这一点，首先必须确保缓冲区的前面确实是连续的。

```cpp
unsigned char *evbuffer_pullup(struct evbuffer *buf, ev_ssize_t size);
```
evbuffer_pullup()函数“线性化”buf 前面的 size 字节，必要时将进行复制或者移动，以保证这些字节是连续的，占据相同的内存块。
如果 size 是负的，函数会线性化整个缓冲区。如果 size 大于缓冲区中的字节数，函数返回 NULL。否则，evbuffer_pullup()返回指向 buf 中首字节的指针。

调用 evbuffer_pullup()时使用较大的 size 参数可能会非常慢，因为这可能需要复制整个缓冲区的内容。

使用 evbuffer_get_contiguous_space()返回的值作为尺寸值调用 evbuffer_pullup()不会导致任何数据复制或者移动。

#### [6.10 从 evbuffer 中移除数据](#)
主要有两个函数，

```cpp
int evbuffer_drain(struct evbuffer *buf, size_t len);
/**
 * @brief: 从 evbuffer 的开始处读取指定长度的数据
 *         若 evbuffer 中的数据不足指定长度，则尽可能多的读取数据
 *
 * @param buf: 待读取数据的 evbuffer 对象
 * @param data: 数据指针
 * @param datlen: 数据长度，单位 byte
 *
 * @return: 成功返回读取的字节数，失败返回-1
 */
int evbuffer_remove(struct evbuffer *buf, void *data, size_t datlen);
```
**evbuffer_remove** 函数从 buf 前面复制和移除 datlen 字节到 data 处的内存中。如果可用字节少于 datlen，函数复制所有字节。失败时返回-1，否则返回复制了的字节数。

evbuffer_drain函数的行为与 evbuffer_remove相同,**只是它不进行数据复制**: 而只是将数据从缓冲区前面移除。成功时返回0，失败时返回-1。

#### [6.11 从 evbuffer 中复制出数据](#)
evbuffer_copyout()的行为与evbuffer_remove()相同，**但是它不从缓冲区移除任何数据。也就是说，它从 buf前面复制datlen 字节到data 处的内存中。**

```cpp
ev_ssize_t evbuffer_copyout(struct evbuffer* buf, void * data, size_t datlen);
ev_ssize_t evbuffer_copyout_from(
    struct evbuffer* buf,
	const struct evbuffer_ptr* pos,
	void* data_out, size_t datlen
);
```

#### [6.12 evbuffer_readln](#)
evbuffer_readln()函数从evbuffer前面取出一行，用一个新分配的空字符结束的字符串返回这一行。

如果n_read_out不是NULL，则它被设置为返回的字符串的字节数, 如果没有整行供读取，函数返回空，返回的字符串不包括行结束符。

**如果**
```cpp
char* evbuffer_readln(
    struct evbuffer * buffer, 
    size_t * n_read_out,
    enum evbuffer_eol_style eol_style
);
            
enum evbuffer_eol_style 
{
    EVBUFFER_EOL_ANY,
    EVBUFFER_EOL_CRLF,
    EVBUFFER_EOL_CRLF_STRICT,
    EVBUFFER_EOL_LF,
    EVBUFFER_EOL_NUL
};
```

evbuffer_readln()理解的4种行结束格式：

|参数|说明|
|:----|:----|
|EVBUFFER_EOL_LF|行尾是单个换行符(也就是\n，ASCII值是0x0A)|
|EVBUFFER_EOL_CRLF_STRICT|行尾是一个回车符，后随一个换行符(也就是\r\n，ASCII值是0x0D 0x0A)|
|EVBUFFER_EOL_CRLF|	行尾是一个可选的回车，后随一个换行符(也就是说，可以是\r\n或者\n) 。这种格式对于解析基于文本的互联网协议很有用，因为标准通常要求\r\n的行结束符，而不遵循标准的客户端有时候只使用\n。|
|EVBUFFER_EOL_ANY |	行尾是任意数量、任意次序的回车和换行符。这种格式不是特别有用。它的存在主要是为了向后兼容|


#### [6.13 evbuffer_peek](#)
函数可以在不删除或复制evbuffer的情况下查看evbuffer内部的数据。通过在“vec_out”数组中填充指向缓冲区内一个或多个数据区的指针，可以返回指向数据的指针。

```cpp
/*
* @buffer：  要窥视的evbuffer，
* @len：  尝试窥探的字节数。如果len为负数，我们将尽可能多地填充vec_out。如果len为
          负数，并且没有提供vec_out，我们将返回获取缓冲区中所有数据所需的evbuffer_iovec的数量。
* @start_at：  一个evbuffer_ptr，指示我们应该开始查找数据的点。NULL表示“在缓冲区的开头。”
* @vec_out： evbuffer_iovec的数组
* @n_vec ：vec_out的长度。如果为0，我们只计算需要多少扩展数据块才能指向请求的数据量。
*/

int evbuffer_peek(
    struct evbuffer *buffer, 
    ev_ssize_t len,
    struct evbuffer_ptr *start_at,
    struct evbuffer_iovec *vec_out, int n_vec
);
```
调用 evbuffer_peek()的时候,通过vec_out 给定一个evbuffer_iovec 数组,数组的长度是n_vec.函数会让每个结构体包含指向evbuffer 内部内存块的指针(iov_base)和块中数据长度.

如果 len小于0,evbuffer_peek()会试图填充所有evbuffer_iovec 结构体。 否则,函数会进行填充,直到使用了所有结构体,或者见到len 字节为止。
如果函数可以给出所有请求的数据,则返回实际使用的结构体个数;否则,函数返回给出所有请求数据所需的结构体个数。

如果 ptr为 NULL,函数从缓冲区开始处进行搜索.否则,从ptr 处开始搜索。

#### [6.14 在evbuffer中搜索内容](#)
evbuffer_ptr结构体指示evbuffer中的一个位置，包含可用于在evbuffer中迭代的数据。

结构体e vbuffer_ptr 中的 pos 为偏移量，如果为 -1 则没查询到，大于 -1 则搜索到了匹配的复制。
```cpp
struct evbuffer_ptr 
{
	ev_ssize_t pos;
	struct 
	{
		/* internal fields*/
	} _internal;
};
```

evbuffer_search()函数在缓冲区中查找含有len个字符的字符串what。

函数返回包含字符串位置，或者在没有找到字符串时包含 `-1` 的evbuffer_ptr结构体。

如果提供了start 参数，则从指定的位置开始搜索；否则，从开始处进行搜索。

**evbuffer_search_range()** 函数和 **evbuffer_search** 行为相同，只是它只考虑在end之前出现的what。

**evbuffer_search_eol()** 函数像 **evbuffer_readln()** 一样检测行结束，但是不复制行，而是返回指向行结束符的 evbuffer_ptr，如果 eol_len_out 非空，则它被设置为EOL字符串长度。

```cpp
struct evbuffer_ptr evbuffer_search(struct evbuffer* buffer,
    const char* what, 
    size_t len, 
    const struct evbuffer_ptr * start
);
struct evbuffer_ptr evbuffer_search_range(struct evbuffer* buffer,
    const char* what, size_t len, 
    const struct evbuffer_ptr * start,
    const struct evbuffer_ptr* end
);
struct evbuffer_ptr evbuffer_search_eol(struct evbuffer* buffer,
    struct evbuffer_ptr* start, 
    size_t * eol_len_out,
    enum evbuffer_eol_style eol_style
);
```

**evbuffer_ptr_set** 函数操作 buffer中的位置pos， 成功时函数返回0，失败时返回 -1 。
```cpp
enum evbuffer_ptr_how 
{
	EVBUFFER_PTR_SET, //指针被移动到缓冲区中的绝对位置position
	EVBUFFER_PTR_ADD  //向前移动position字节  
};

int evbuffer_ptr_set(	struct evbuffer* buffer, 
    struct evbuffer_ptr * pos,
    size_t position, 
    enum evbuffer_ptr_how how
);
```

#### [6.15 evbuffer_reserve_space](#)
时候需要能够直接向evbuffer添加数据，而不用先将数据写入到字符数组中，然后再使用evbuffer_add()进行复制。有一对高级函数可以完成
这种功能：evbuffer_reserve_space()和evbuffer_commit_space()。跟evbuffer_peek()一样，这两个函数使用evbuffer_iovec结构体来提供对evbuffer内部内存的直接访问。

```cpp
struct evbuffer_iovec 
{
	void* iov_base;
	size_t iov_len;

};
int evbuffer_reserve_space(
    struct evbuffer *buf, 
    ev_ssize_t size,
    struct evbuffer_iovec *vec, 
    int n_vec
);

int evbuffer_commit_space(
    struct evbuffer *buf,
    struct evbuffer_iovec *vec, 
    int n_vecs
);
```


#### [6.16 读写小例子](#)

```cpp
struct evbuffer *buffer = evbuffer_new();

evbuffer_enable_locking(buffer, nullptr);

const char *block_01 = {"122222222222222222222222222\r\n"};
const char *block_02 = {"333333333333333333333333335\r\n"};

evbuffer_add(buffer,block_01, sizeof(char)* strlen(block_01));
evbuffer_add(buffer,block_02, sizeof(char)* strlen(block_01));

size_t len;
char *first_line = evbuffer_readln(buffer,&len, EVBUFFER_EOL_CRLF);
//122222222222222222222222222
char *second_line = evbuffer_readln(buffer,&len, EVBUFFER_EOL_CRLF);
//333333333333333333333333335

std::cout <<"first_line: " <<first_line << std::endl;
std::cout <<"second_line: " <<second_line << std::endl;

free(first_line);
free(second_line);

evbuffer_free(buffer);
```


### [7. 类型获取 bufferevent](#)
获取bufferevent相关的属性！

#### [7.1 获得回调函数和参数](#)
尚且不知道干嘛用阿！

```cpp
void bufferevent_getcb(
    struct bufferevent *bufev,
    bufferevent_data_cb *readcb_ptr,
    bufferevent_data_cb *writecb_ptr,
    bufferevent_event_cb *eventcb_ptr,
    void **cbarg_ptr
);
```

#### [7.2 bufferevent_get_underlying](#)
返回作为底层传输的另一个bufferevent，当然只适用于filtering bufferevents。

```cpp
struct bufferevent *bufferevent_get_underlying(struct bufferevent *bufev);
```

#### [7.3 加锁](#)
有时候需要确保对 bufferevent 的一些操作是原子地执行的。为此， libevent 提供了手动锁定和解锁 bufferevent 的函数。

```cpp
void bufferevent_lock(struct bufferevent *bufev);
void bufferevent_unlock(struct bufferevent *bufev);
```

#### [7.4 fd 获取与设置](#)
这些函数设置或者返回基于 fd 的事件的文件描述符。只有基于套接字的 bufferevent 支持 setfd()。两个函数都在失败时返回-1；setfd()成功时返回0。

```cpp
int bufferevent_setfd(struct bufferevent *bufev, evutil_socket_t fd);

evutil_socket_t bufferevent_getfd(struct bufferevent *bufev);
```

#### [7.5 获取 event_base](#)
这个函数返回 bufferevent 的 event_base。

```cpp
struct event_base *bufferevent_get_base(struct bufferevent *bev);
```

