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
* 00 libevent 组件库介绍 [**-->**](./contents/libevent000Introduce.md)
* 01 libevent 入门使用 [**-->**](./contents/libevent001SimpleUse.md)
* 02 libevent event_base [**-->**](./contents/libevent002SettingAndBase.md)
* 03 libevent 普通 event [**-->**](./contents/libevent003Event.md) 
* 04 libevent bufferevent [**-->**](./contents/libevent004Bufferevent.md) 
* 05 libevent bufferevent 高级用法 [**-->**](./contents/libevent005SeniorBufferevent.md)     

参考书籍:
- [《Libevent深入浅出》 - `刘丹冰`](https://github.com/aceld/libevent/)
- [《libevent源码深度剖析》 - `张亮`](https://blog.csdn.net/sparkliang/category_660506.html)
- [《libevent参考手册》 ](./books/libevent参考手册(中文版).pdf)


---
