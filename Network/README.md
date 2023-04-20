### [C++ Network Programming](#)
**介绍**： C++ 20到现在都没有形成一个标准的跨平台的网络库，所以网络编程还需要不同的平台： **Unix** **Windows**。所以最好使用已经成熟的网络库，例如：
**Boost.Asio 、asio、grpc**。做网络编程需要摆脱传统Web的Http思维，深入了解TCP,UDP等底层网络协议，再到成熟阶段可以设计一个网络协议，**恐怕2030年之前是使用不上标准网络库了**。

----
**C/C++** 语言的网络编程，包括socket基础，多进程/多线程网络服务端、I/O复用的三种方案（*select/poll/epoll*），[这里只记录最基本的
Socket编程知识！](#)

**笔记目录:**

* 01 C++ Socket 编程入门 [**-->**](./contents/CPPNetworkProgramming.md)
* 02 C++ Socket TCP 创建流程 [**-->**](./contents/CPPSocketContinue.md)
* 03 C++ Socket UDP 创建流程 [**-->**](./contents/CPPSocketUDPContinue.md)
* 04 C++ Socket 并发服务器 [**-->**](./contents/CPPSocketMore.md)
* 05 C++ Socket Select IO [**-->**](./contents/CPPSocketIOSelect.md)





----
参考网站/书籍:
- [win32 winsock-reference](https://learn.microsoft.com/zh-cn/windows/win32/winsock/winsock-reference)
- [Unix 网络编程 卷1+卷2](#)
- [Linux 网络编程 edition2](#)
