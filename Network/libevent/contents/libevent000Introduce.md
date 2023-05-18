### [libevent 简介](#)
 **介绍**： libevent 是c实现的异步事件库，注册异步事件，检测异步事件，根据事件的触发先后顺序，调用相对应回调函数处理事件。

-----
- [x] [1. libevent 组件](#1-libevent-组件)
- [x] [2. ](#2-)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)
-----

### [1. libevent 组件](#)
Libevent是开源社区的一款高性能的I/O框架库，由下列组件构成：

1. **evutil**：用于抽象不同平台网络实现差异的通用功能。
2. event 和 event_base：libevent 的核心，为各种平台特定的、基于事件的非阻塞 IO 后端提供抽象 API，让程序可以知道套接字何时已经准备好，可以读或者写，并且处理基
本的超时功能，检测 OS 信号。
3. bufferevent：为 libevent 基于事件的核心提供使用更方便的封装。除了通知程序套接字
已经准备好读写之外，还让程序可以请求缓冲的读写操作，可以知道何时 IO 已经真正
发生。（bufferevent 接口有多个后端，可以采用系统能够提供的更快的非阻塞 IO 方式，如 Windows 中的 IOCP。）
4. **evbuffer**：在 bufferevent 层之下实现了缓冲功能，并且提供了方便有效的访问函数。
5. **evhttp**：一个简单的 HTTP 客户端/服务器实现。
6. **evdns**：一个简单的 DNS 客户端/服务器实现。
7. **evrpc**：一个简单的 RPC 实现。

#### [1.1 libevent 库](#)
创建 libevent 时，默认安装下列库：
* libevent_core：所有核心的事件和缓冲功能，包含了所有的 event_base、evbuffer、bufferevent 和工具函数。
* libevent_extra：定义了程序可能需要，也可能不需要的协议特定功能，包括 HTTP、DNS 和 RPC。
* libevent：这个库因为历史原因而存在，它包含 libevent_core 和 libevent_extra 的内容。不应该使用这个库，未来版本的 libevent 可能去掉这个库。

某些平台上可能安装下列库：
* libevent_pthreads：添加基于 pthread 可移植线程库的线程和锁定实现。它独立于libevent_core，这样程序使用 libevent 时就不需要链接到 pthread，除非是以多线程方式使用 libevent。
* libevent_openssl：这个库为使用 bufferevent 和 OpenSSL 进行加密的通信提供支持。它独立于 libevent_core，这样程序使用 libevent 时就不需要链接到 OpenSSL，除非是进行加密通信。

##### [1.1.1 ](#)

### [2.](#) 

|方法|返回值|参数个数|说明|
|:---|:---|:---|:----|
|||||
|||||
|||||

### [3.](#) 

### [4. 套接字设置](#)

#### [4.1 ](#)


### [5.](#) 

-----
时间: [] 