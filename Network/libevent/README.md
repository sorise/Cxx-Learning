### [C/C++ 网络库： libevent](#)
**介绍**： 一个基于C语言编程的事件驱动，基于Reactor模型网络模型，开源、跨平台、稳定的高性能网络库！ [https://libevent.org/](https://libevent.org/)

----
支持 Windows、 Linux、 `*BSD` 和 Mac Os；支持多种 I/O 多路复用技术：epoll、 poll、 dev/poll、 select 和 kqueue 等；支持 I/O，定时器和信号等事件；注册事件优先级。

安装使用：
```shell
$ ./configure   # 检查安装环境，生成 makefile
$ make          # 生成库、头文件
$ make verify   # (optional)
$ sudo make install  # 拷贝到指定目录 .so,.a安装在 /usr/local/lib
$ # 英文文档 https://libevent.org/libevent-book/
```

**目录:**
* 01 libevent 入门 [**-->**](./contents/libevent001simpleuse.md)


---
