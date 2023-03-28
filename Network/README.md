### [C++ Network Programming](#)
**介绍**： C++ 20到现在都没有形成一个标准的跨平台的网络库，所以网络编程还需要不同的平台： **Unix** **Windows**。所以最好使用已经成熟的网络库，例如：
**Boost.Asio 、asio、grpc**。做网络编程需要摆脱传统Web的Http思维，深入了解TCP,UDP等底层网络协议，再到成熟阶段可以设计一个网络协议。

----
**C/C++** 语言的网络编程，包括socket基础，多进程/多线程网络服务端、I/O复用的三种方案（*select/poll/epoll*）。

**笔记目录:**

* [01] C++ 网络编程入门 [**-->**](./contents/CPPNetworkProgramming.md)



<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>

**重磅好消息：C++ 23 将加入标准网络库**，networking是基于**asio**的一套网络API，是所有C++第三方网络库中最完善、代码质量可信度最高、代码风
格最统一（因为作者只有一个人）的一个库，作者从2003年开始就提议将其纳入C++标准库，直到2023年才确定一定会加入标准库，
c++ 参考手册 [https://zh.cppreference.com/w/cpp](https://zh.cppreference.com/w/cpp) 。

**C++ 开源网络库**

----

* Boost.Asio : 用于网络和底层I/O编程的跨平台的C++库。
* libev : 功能齐全，高性能的时间循环，轻微地仿效libevent，但是不再像libevent一样有局限性，也修复了它的一些bug。
* libevent : 事件通知库libuv : 跨平台异步I/O。
* libco : 协程，微信支持8亿用户同时在线的底层IO库。功能强大
* libgo : golang风格的并发框架，C++11实现协程库
* ACE : C++面向对象网络变成工具包
* Casablanca : C++ REST SDKcpp-netlib : 高级网络编程的开源库集合
* libCurl : 多协议文件传输库
* Mongoose : 非常轻量级的网络服务器
* Muduo : 用于Linux多线程服务器的C++非阻塞网络库
* net_skeleton : C/C++的TCP 客户端/服务器库
* POCO : 用于构建网络和基于互联网应用程序的C++类库，可以运行在桌面，服务器，移动和嵌入式系统。
* RakNet : 为游戏开发人员提供的跨平台的开源C++网络引擎。
* Tufao : 用于Qt之上的C++构建的异步Web框架。
* WebSocket++ : 基于C++/Boost Aiso的websocket 客户端/服务器库
* ZeroMQ : 高速，模块化的异步通信库








