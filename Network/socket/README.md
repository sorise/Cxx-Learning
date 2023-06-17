### [C++ Socket Networking](#)
**介绍**： socket就是套接字的意思，用于描述地址和端口。应用程序通过socket向网络发出请求或者回应。

----
**C/C++** 语言的网络编程，包括socket基础，多进程/多线程网络服务端、I/O复用的三种方案（*select/poll/epoll*），[这里只记录最基本的
Socket编程知识！](#)

**Socket 基础:**

* 00 C++ Socket errno 错误号 查询文档 [**-->**](./contents/errno.md)
* 01 C++ Socket 编程入门 [**-->**](./contents/CPPNetworkProgramming.md)
    * 01 windows Socket 编程入门 补充 [**-->**](./contents/CPPWinSocketThresholdMinner.md)
* 02 C++ Socket TCP 创建流程 [**-->**](./contents/CPPSocketContinue.md)
* 03 C++ Socket UDP 创建流程 [**-->**](./contents/CPPSocketUDPContinue.md)
* 04 C++ Socket 并发服务器 [**-->**](./contents/CPPSocketMore.md)
* 05 C++ Socket IO复用、Select、设置socket为非阻塞[**-->**](./contents/CPPSocketIOSelect.md)
* 06 C++ Socket poll、epoll IO [**-->**](./contents/CPPSocketIOPoll.md)
* 07 C++ Socket epoll 反应堆 [**-->**](./contents/CPPSocketEpollReactor.md)
* 08 C++ Socket 本地套接字通信 [**-->**](./contents/CPPSocketLocalCommunication.md)

**UDP Server：** 由于TCP的网络库较多而UDP较少，所以进行补充！

* 01 C++ Socket Server 基础 [**-->**](./contents/CPPSocketUDPServer01.md) 


----
参考网站/书籍:
- [win32 winsock-reference](https://learn.microsoft.com/zh-cn/windows/win32/winsock/winsock-reference)
- [Unix 网络编程 卷1+卷2](#)
- [Linux 网络编程 edition2](#)
