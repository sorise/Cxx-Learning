### [C++ IO 多路复用 poll，epoll](#)
 **介绍**： 专属于Linux的网络IO复用模式，并不支持windows! 相比于select没有描述符个数的限制。

-----

- [x] [1. poll概述](#1-poll概述)
- [x] [2. poll 服务器](#2-poll-服务器)
- [x] [3. epoll](#3-epoll)
- [x] [4. linux 使用 epoll 需要的设置](#4-linux-使用-epoll-需要的设置)
- [x] [5. epoll 系统调用](#5-epoll-系统调用)
- [x] [6. epoll和poll的区别](#6-epoll和poll的区别)
- [x] [7. epoll 服务器实现](#7-epoll-服务器实现)

-----

### [1. poll概述](#)
poll本质上和select没有区别，用于检测一组文件描述符上的可读可写和出错事件， 但是它没有最大连接数的限制，原因是它是基于链表来存储的。

```cpp
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct pollfd {
    int   fd;         /* file descriptor */
    short events;     /* requested events */
    short revents;    /* returned events */
};

```
各**参数**含义如下：
* `fds` :一个结构体，用来保存各个描述符的相关状态。
* `nfds` : fds数组的大小，即里面包含有效成员的数量。
* `timeout`:设定的超时时间。（以毫秒为单位）
    * `-1` 则阻塞等待
    * `0` 不阻塞，立即返回
    * `>0` ,超时时间

**返回值** 与select()十分相似，当返回正值时，代表满足响应事件的文件描述符的个数，如果返回0则代表在规定时间内没有事件发生。如发现返回为负则应该立即查看 errno，因为这代表有错误发生。
* `-1`：有错误产生
* `0`：超时时间到，而且没有描述符有状态变化
* `>0`：有状态变化的描述符个数


#### [1.1 事件宏](#)
struct pollfd的event字段是由开发者设置的，告诉内核我们关注什么事件。而revents字段是poll函数返回时由内核设置，说明该fd发生了什么事件。

|事件宏|描述|是否可以作为输入event|是否可以作为输出revents|
|:---|:---|:---|:----|
|POLLIN |普通或优先级带 **数据可读**|是 |是 |
|POLLOUT |普通或优先级 **数据可写** |是 |是 |
|POLLRDNORM |普通数据可读 |是 |是 |
|POLLRDBAND |优先级带数据可读 |是 |是 |
|POLLPRI |高优先级数据可读，例如TCP外带数据！ |是 |是 |
|POLLWRNORM |普通数据可写 |是 |是 |
|POLLWRBAND |优先级带数据可写 |是 |是 |
|POLLRDHUP |TCP连接被对端关闭，或者关闭了写操作|是 |是 |
|POLLHUP |发生挂起 |否 |是 |
|POLLERR |发生错误 |否 |是 |
|POLLNVAL |描述字不是一个打开的文件 |否 |是 |


#### [1.2 优点](#)
* poll不要求开发者计算最大文件描述符加1的大小。
* 与select相比， poll在处理大数量的文件描述符时速度更快。
* poll没有最大连接数的限制，因为其存储fd的数组没有长度限制。
* 在调用poll函数时，只需对参数进行一次设置就好。
* 可以将各种事件，分开监听！


### [2. poll 服务器](#) 
一个接受客户端请求的服务器！


#### [2.1 封装错误处理](#)

```cpp
#ifndef POLL_WRAP_HPP
#define POLL_WRAP_HPP
#include <iostream>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <exception>

int Socket(int domain,int type,int protocol){
    auto socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (socket_fd == -1) {
        throw std::runtime_error("create the socket failed!");
    }
    return socket_fd;
}

int Bind(int socket_fd, const struct sockaddr *addr, socklen_t addrlen){
    auto result = bind(socket_fd, addr, addrlen);
    if (result == -1) {
        throw std::runtime_error("bind occur error, please check the data sockaddr!");
    }
    return result;
}

void Listen(int socket_fd, int backlog){
    auto result =  listen(socket_fd, backlog);
    if (result == -1) {
        throw std::runtime_error("listen occur error,the socket failed!");
    }
}

int Accept(int socket_fd, struct sockaddr *addr, socklen_t *addrlen){
    auto new_socket_fd = accept(socket_fd, addr, addrlen);
    if (new_socket_fd == -1) {
        throw std::runtime_error("accept occur error,the socket failed!");
    }
    return new_socket_fd;
}

int Close(int fd){
    auto result = close(fd);
    if (result == -1) {
        throw std::runtime_error("close the socket failed! - errno: "+ std::to_string(errno));
    }
    return result;
}

int initServer(unsigned short port, unsigned short max_connection){
    auto fd = Socket(AF_INET, SOCK_STREAM,0);
    /* 允许地址复用 */
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt)) == -1){
        throw std::runtime_error("set socket error！");
    }
    /* 设置为非阻塞 */
    int newSocketFlag = fcntl(fd, F_GETFL, 0) | O_NONBLOCK;
    fcntl(fd,F_GETFL, newSocketFlag);
    /* 设置发送缓存低潮 */
    int sendLowBufferSize = 2048 ; //2k 字节
    setsockopt(fd, SOL_SOCKET, SO_SNDLOWAT, (const void *) &sendLowBufferSize, sizeof(sendLowBufferSize));
    /* 设置关闭前先发完 */
    linger m_sLinger{1,5};
    setsockopt( fd, SOL_SOCKET, SO_LINGER, (const char*)&m_sLinger, sizeof(struct linger));
    /* 服务器地址 */
    sockaddr_in serverAddress{
            AF_INET,
            htons(port),
            htonl(INADDR_ANY)
    };
    /* 绑定 */
    Bind(fd, (const struct sockaddr *)&serverAddress, sizeof(serverAddress));
    /* 监听 */
    Listen(fd, max_connection);
    std::cout << "start server at 127.0.0.1 running in "<< std::to_string(port) <<":" << "\n";
    return fd; //返回套接字
}
#endif //POLL_WRAP_HPP
```
#### [2.2 服务器](#)

```cpp
#include <iostream>
#include "wrap.hpp"
#include <poll.h>

int main() {
    const int MAX_CONNECTION = 1024;
    auto fd = initServer(15000, 40);
    struct pollfd fds[MAX_CONNECTION];

    fds[0] = { fd, POLLIN, 0 };
    for (int i = 1; i < 1024 ; i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }

    int max_index = 0;
    struct sockaddr_in client_addr {};

    while (true){
        int result = poll(fds, max_index + 1, -1);
        if (result == -1) throw std::runtime_error("error in poll! errno: " + std::to_string(errno));
        //来了新连接
        socklen_t len;
        if (fds[0].revents & POLLIN){
            auto clientFd = Accept(fds[0].fd, (struct sockaddr *)&client_addr, &len);
            char *addrIP = inet_ntoa(client_addr.sin_addr);
            std::cout << "new socket connection: " << clientFd << " from ip: "<< addrIP <<" port: "
                << ntohs(client_addr.sin_port)  << std::endl;

            int j = 1;
            //看看有没有空闲位置
            for (; j <= max_index; j++) {
                if (fds[j].fd < 0) {
                    fds[j].fd = clientFd;
                }
            }
            if (max_index + 1 == MAX_CONNECTION) {
                std::string message{"the server is overload! please try after a while!"};
                send(clientFd, message.c_str(), sizeof(char) * message.size(),0);
            }
            //没有空闲，则增加
            if (j == max_index + 1){
                max_index++;
                fds[max_index].fd = clientFd;
            }
        }

        //处理客户端读
        for (int i = 1; i <= max_index; ++i) {
            if (fds[i].fd != -1 && (fds[i].revents & POLLIN)){
                char buffer[4096]; //缓冲区
                auto read_count = recv(fds[i].fd, buffer, 4096, 0);
                if (read_count == 0) {
                    std::cout << "the connect " << std::to_string(fds[i].fd) <<" has been end !" << std::endl;
                    close(fds[i].fd); //关闭了
                    fds[i].fd = -1; //取消了

                    //降低上限
                    if (i == max_index){
                        while (max_index >= 1 && fds[max_index].fd < 0)
                            max_index--;
                    }
                }
                if (read_count == -1){
                    if (errno == EINTR){
                        i--; //被中断 重新读取
                        continue;
                    }
                    throw std::runtime_error(std::to_string(fds[i].fd) +
                     " read error - errno: " + std::to_string(errno) ); //发生错误
                }
                if (read_count > 0){
                    buffer[read_count] = '\0';
                    std::cout << "message: " << buffer << std::flush;
                    std::string  message = "get bytes " + std::to_string(read_count) + "\n";
                    auto write_count = write(fds[i].fd, message.c_str() , sizeof(char)*message.size());
                }
            }
        }

        //std::cout << "max_index: " << max_index << std::endl << std::flush;
    }
    return 0;
}
```

### [3. epoll](#) 
在 Linux 系统之中有一个核心武器：epoll 池，在高并发的，高吞吐的 IO 系统中常常见到 epoll 的身影, 这是**必须要掌握**的IO多路复用实现。
epoll 全名 eventpoll，在 Linux 内核下以一个文件系统模块的形式实现，所以有人常说 epoll 其实本身就是文件系统也是对的；

epoll 则不同，从 epoll_wait 醒来的时候就能精确的拿到就绪的 fd 数组，不需要任何测试，拿到的就是要处理的。

epoll 的使用非常简单，只有下面 3 个系统调用。

* epoll_create 负责创建一个池子，一个监控和管理句柄 fd 的池子, 用户可以创建多个 epoll 池。
* epoll_ctl 负责管理这个池子里的 fd 增、删、改；
* epoll_wait 就是负责打盹的，让出 CPU 调度，但是只要有“事”，立马会从这里唤醒；

```cpp
#include <sys/epoll.h>

int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```
**红黑树**, Linux 内核对于 epoll 池的内部实现就是用红黑树的结构体来管理这些注册进程来的句柄 fd, 红黑树是一种平衡二叉树，时间复杂度为 O(log n)，就算
这个池子就算不断的增删改，也能保持非常稳定的查找性能。

在 epoll_ctl 的内部实现中，除了把句柄结构用红黑树管理，另一个**核心**步骤就是设置**poll回调**。

Linux 设计成一切皆是文件的架构，这个不是说说而已，而是随处可见。实现一个文件系统的时候，就要实现这个文件调用，这个结构体用 struct file_operations 来表示。这个结构体有非常多的函数
```cpp
struct file_operations {
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    __poll_t (*poll) (struct file *, struct poll_table_struct *);
    int (*open) (struct inode *, struct file *);
    int (*fsync) (struct file *, loff_t, loff_t, int datasync);
    // ....
};
```

* 这个 poll 事件回调机制则是 epoll 池高效最核心原理
* epoll 池管理的句柄只能是支持了 file_operations->poll 的文件 fd。换句话说，如果一个“文件”所在的文件系统没有实现 poll 接口，那么就用不了 epoll 机制。

#### [3.1 poll 怎么设置](#)
简单来说就是挂了个钩子，设置了唤醒的回调路径。epoll 跟底层对接的回调函数是：ep_poll_callback，这个函数其实很简单，做两件事情：

* 把事件就绪的 fd 对应的结构体放到一个特定的队列（就绪队列，ready list）；
* 唤醒 epoll ，活来啦！

当 fd 满足可读可写的时候就会经过层层回调，最终调用到这个回调函数，把对应 fd 的结构体放入就绪队列中，从而把 epoll 从 epoll_wait 出唤醒。
这个对应结构体叫做 `epitem` ，每个注册到 epoll 池的 fd 都会对应一个。

就绪队列就简单了，因为没有查找的需求了呀，只要是在就绪队列中的 epitem ，都是事件就绪的，必须处理的。所以就绪队列就是一个最简单的双指针链表。


#### [3.2 总结](#)
I/O多路复用：epoll就是一种典型的I/O多路复用技术: epoll技术的最大特点是支持高并发。
传统多路复用技术select，poll，在并发量达到1000-2000，性能就会明显下降，epoll，从linux内核2.6引入的，2.6之前是没有的

**epoll和kqueue(freebsd)技术类似：单独一台计算机支撑少则数万，多则数十上百万并发连接的核心技术。**

* **epoll事件驱动机制，在单独的进程或者单独的线程里运行，收集/处理事件没有进程/线程之间切换的消耗，高效。**
* 内部管理 fd 使用了高效的红黑树结构管理，做到了增删改之后性能的优化和平衡；
* epoll 池添加 fd 的时候，调用 file_operations->poll ，把这个 fd 就绪之后的回调路径安排好。通过事件通知的形式，做到最高效的运行；
* epoll 池核心的两个数据结构：红黑树和就绪列表。红黑树是为了应对用户的增删改需求，就绪列表是 fd 事件就绪之后放置的特殊地点，epoll 池只需要遍历这个就绪链表，就能给用户返回所有已经就绪的 fd 数组；



#### [3.3 哪些 fd 可以用 epoll 来管理](#)
由于并不是所有的 fd 对应的文件系统都实现了 poll 接口，所以自然并不是所有的 fd 都可以放进 epoll 池，那么有哪些文件系统的 file_operations 实现了 poll 接口？

首先说，类似 ext2，ext4，xfs 这种常规的文件系统是没有实现的，换句话说，这些你最常见的、真的是文件的文件系统反倒是用不了 epoll 机制的。

但是网络套接字socket是支持的，网络也是 epoll 池最常见的应用地点。Linux 下万物皆文件，socket 实现了一套 socket_file_operations 的逻辑（ net/socket.c ）：

有的，很多。其实 Linux 下还有两个很典型的 fd ，常常也会放到 epoll 池里。

* eventfd：eventfd 实现非常简单，故名思义就是专门用来做事件通知用的。使用系统调用 eventfd 创建，这种文件 fd 无法传输数据，只用来传输事件，常常用于生产消费者模式的事件实现；
* timerfd：这是一种定时器 fd，使用 timerfd_create 创建，到时间点触发可读事件；

### [4. linux 使用 epoll 需要的设置](#)
linux系统默认打开的文件数量是有限制的，默认值是1024,也就是说Socket编程最多接受1023个用户连接，虽然epoll可以超过这个限制，
但是也需要对linux系统进行设置，修改默认打开的文件数量，使其能够支持更多的连接数量。

查看系统中的默认打开文件最大数量。
```shell
ulimit -a 
#open files                          (-n) 1024
```
修改方式(ubuntu 22.04 kernel 5.19.0-40-generic )

```shell
sudo vi /etc/security/limits.conf 
```
修改内容，或者添加如下内容， 一般在云服务器中默认值会设置为超过1024!
```shell
# End of file
root soft nofile 65535
root hard nofile 65535
* soft nofile 65535
* hard nofile 65535
```
修改完毕后，需要注销重启一下！

### [5. epoll 系统调用](#) 
epoll的系统调用只有三个，使用起来比较简单，但是原理需要了解一下。

#### [5.1 epoll_create()函数](#)
负责创建一个池子，一个监控和管理句柄 fd 的池子, 用户可以创建多个 epoll 池。 `size > 0` 参数指定这个池子的大小！
```cpp
#include <sys/epoll.h>
int epoll_create(int size);
```
**参数:**
* 从Linux 2.6.8开始，`size` 参数被忽略，但必须大于零；


**返回值:**
* 返回池子的句柄，这个epoll对象最终要用close()函数关闭，因为文件描述符/句柄 总是关闭的。
* 如果失败，返回 `-1`， 并且设置 `errno`。

底层实现：
```cpp
struct eventpoll *ep = (struct eventpoll*)calloc(1, sizeof(struct eventpoll)); 
```

#### [5.2 epoll_ctl()函数](#)
把一个socket以及这个socket相关的事件添加到这个epoll对象描述符中去，目的就是通过这个epoll对象来监视这个socket上数据的来往情况；当有数据来往时，系统会通知我们 。

```cpp
#include <sys/epoll.h>
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

typedef union epoll_data {
	void *ptr; /* 指向用户自定义数据 */
	int fd;    /* 注册的文件描述符, socket fd */
	uint32_t u32; /* 32-bit integer */
	uint64_t u64; /* 64-bit integer */
} epoll_data_t;

struct epoll_event {
	uint32_t events;   /* 描述epoll事件 */
	epoll_data_t data; /* 见上面的结构体 */
};
```
正常返回0, 如果失败，返回 `-1`， 并且设置 `errno`。

**参数**
* epfd是epoll_create的返回值
* op：动作，添加/删除/修改 
    * EPOLL_CTL_ADD：向红黑书添加一个需要监视的描述符
    * EPOLL_CTL_DEL：从红黑书中删除一个描述符
    * EPOLL_CTL_MOD：修改红黑书中一个描述符
* sockid：表示客户端连接，就是你从accept()这个是红黑树里边的key;
* event：事件信息，这里包括的是 一些事件信息

```cpp
epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd ,&temp);
epoll_ctl(epollFd, EPOLL_CTL_DEL, epollQueue[i].data.fd, nullptr); //删除
```

常用的epoll事件描述如下：
* **EPOLLIN**：描述符处于可读状态
* **EPOLLOUT**：描述符处于可写状态
* **EPOLLET**：将epoll event通知模式设置成edge triggered
* **EPOLLONESHOT**：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里。
* **EPOLLHUP**：本端描述符产生一个挂断事件，默认监测事件
* **EPOLLRDHUP**：对端描述符产生一个挂断事件
* **EPOLLPRI**：由带外数据触发
* **EPOLLERR**：描述符产生错误时触发，默认检测事件
* **EPOLLEXCLUSIVE**： kernel 4.5 +支持水平触发模式的EPOLLEXCLUSIVE标志， 这个标志会保证一个事件只有一个epoll_wait()会被唤醒，避免了"惊群效应",并且可以完美的在多个ＣＰＵ之间进行扩展。
    * POLLEXCLUSIVE可以仅在EPOLL_CTL_ADD操作中使用；尝试将其与EPOLL_CTL_MOD一起使用会产生错误
* **EPOLLWAKEUP**：  linux提供了autosleep的电源管理功能，如果events中设置了 EPOLLWAKEUP, 还需要为autosleep创建一个唤醒源 ep_create_wakeup_source 获取被监听fd上的相关事件。
    * 如果当前系统支持autosleep功能，支持休眠；则允许用户传入 EPOLLWAKEUP 标志；
	* 否则，从用户传入的数据中去除掉 EPOLLWAKEUP 标志
    * 如果events里设置了EPOLLWAKEUP

#### [5.3 epoll_wait()函数](#)
阻塞等待注册的事件发生，返回事件的数目，并将触发的事件写入events数组中。

```cpp
#include <sys/epoll.h>
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```
**参数:**
* events: **是一个传出数组**, 用来记录被触发的events，返回那些满足条件的那些fd结构体！
* maxevents: 返回的events的最大个数 1024
* 参数timeout描述在函数调用中阻塞时间上限，单位是ms：
    * timeout = -1 表示调用将一直阻塞，直到有文件描述符进入ready状态或者捕获到信号才返回；
    * timeout = 0 用于非阻塞检测是否有描述符处于ready状态，不管结果怎么样，调用都立即返回；
    * timeout > 0 表示调用将最多持续timeout时间，如果期间有检测对象变为ready状态或者捕获到信号则返回，否则直到超时。

**返回值**：成功时，epoll_wait（）返回为请求的I / O准备就绪的文件描述符的数目；如果在请求的超时毫秒内没有文件描述符准备就绪，则返回零。发生错误时，epoll_wait（）返回-1并正确设置errno。
* EBADF：epfd不是有效的文件描述符。
* EFAULT：具有写许可权不能访问事件指向的存储区。
* EINTR：在任何请求的事件发生或超时到期之前，信号处理程序中断了该调用；参见signal（7）。
* EINVAL：epfd不是epoll文件描述符，或者maxevents小于或等于零。

#### [5.4 LT和ET模式](#)
和poll 的事件宏相比，epoll 新增了一个事件宏 EPOLLET，这就是所谓的边缘触发模式（Edge Trigger，ET），而默认的模式我们称为 水平触发模式（Level Trigger，LT）。这两种模式的区别在于：
* 对于水平触发模式，一个事件只要有，就会一直触发； **适用于阻塞/非阻塞socket模式**
* 对于边缘触发模式，只有一个事件从无到有才会触发。 **只适用于非阻塞socket模式** , 最好采用循环读的模式！

```cpp
struct epoll_event event;
event.events = EPOLLIN | EPOLLET; //修改为边缘触发模式，默认是水平！

//这种模式下就建议使用 accept4
accept4(socket_fd, addr, addrlen, SOCK_NONBLOCK);

//以下把数据读光
while(read() >0){
    /* ... */
}
```

**解释**
* 说的有点抽象，以 socket 的读事件为例，对于水平模式，**只要 socket 上有未读完的数据，就会一直产生 EPOLLIN 事件**；
* 而对于边缘模式，socket 上每新来一次数据就会触发一次，如果上一次触发后，未将 socket 上的数据读完，也不会再触发，除非再新来一次数据。
* 对于 socket 写事件，如果 socket 的 TCP 窗口一直不饱和，会一直触发 EPOLLOUT 事件；
* 而对于边缘模式，只会触发一次，除非 TCP窗口由不饱和变成饱和再一次变成不饱和，才会再次触发 EPOLLOUT 事件。

### [6. epoll和poll的区别](#)
epoll的效率并不意味着一定高于poll，**一般在socket连接数量较大而活跃的连接较少的情况下，epoll模型效率更高！**

* **在连接数少、且活跃度较高的时候**，使用select、poll每次能够检测出多个fd，相比起epoll频繁调用回调函数，修改就绪队列，可能性能更好。
* 如果要在select和poll之间选择，**就看对传入fd是否有数量限制**。
* **select和poll基于的是循环**， **epoll基于回调**
* select，poll需自己主动不断轮询所有fd集合，直到设备就绪，期间可能要睡眠和唤醒多次交替。而epoll其实也需调用epoll_wait不断轮询就绪链表，期间也可能多次睡眠和唤醒交替，但它是设备就绪时，调用回调函数，把就绪fd放入就绪链表，并唤醒在epoll_wait中进入睡眠的进程。
虽然都要睡眠和交替，但 **select和poll在“醒着”时要遍历整个fd集合**，**而epoll在“醒着”的时候只需判断就绪链表是否为空，节省大量CPU时间**，这就是回调机制带来的性能提升。
#### [6.1 epoll的优缺点](#)
**优点**：高效、使用简单，基于回调！
* 没有最大并发连接的限制，能打开的FD的上限远大于1024（1G的内存上能监听约10万个端口）；
* 效率提升，不是轮询的方式，不会随着FD数目的增加效率下降。只有活跃可用的FD才会调用callback函数；
* Epoll最大的优点就在于它只管你“活跃”的连接，而跟连接总数无关，因此在实际的网络环境中，Epoll的效率就会远远高于select和poll。
**缺点**:
* linux 系统专属, unix系统都不支持！

### [7. epoll 服务器实现](#)
wrap.hpp和前面一样的！

```cpp
#include <iostream>
#include "wrap.hpp"
#include <sys/epoll.h>

#define OPEN_MAX_CLIENT_CONNECTION 2048

auto Epoll_ctl(int _epfd, int _op, int _fd, struct epoll_event *_event) ->
        decltype(epoll_ctl(_epfd,_op,_fd, _event)){
    auto v = epoll_ctl(_epfd,_op,_fd, _event);
    if (v != 0){
        throw std::runtime_error(std::to_string(_fd) +
        ": error happen in Epoll_ctl ! errno: " + std::to_string(errno));
    }
}

void createEpollServer(){
    auto server_fd = socket_wrap::initServer(15000, 128);
    struct epoll_event temp{EPOLLIN | EPOLLERR}, epollQueue[OPEN_MAX_CLIENT_CONNECTION];
    struct epoll_event server_et{EPOLLIN | EPOLLERR, {.fd =  server_fd}};
    auto epollFd = epoll_create(1);
    if (epollFd == -1 ){
        throw std::runtime_error("error is happen! errno: " + std::to_string(errno));
    }
    Epoll_ctl(epollFd, EPOLL_CTL_ADD, server_fd, &server_et);

    bool running  = true;
    while (running){
        auto readyCount = epoll_wait(epollFd, epollQueue,
                                     OPEN_MAX_CLIENT_CONNECTION, -1);
        for (int i = 0; i < readyCount; ++i) {
            //非读事件
            if (epollQueue[i].events & EPOLLERR){
                running = false; //有错误停止运行
                break;
            }
            //有新连接
            if (epollQueue[i].data.fd == server_fd){
                sockaddr_in address{};
                socklen_t len;
                auto clientFd = socket_wrap::Accept(server_fd, (struct sockaddr *)&address, &len);
                temp.data.fd = clientFd;
                temp.events = EPOLLIN;
                /* --- 受到新连接日志消息 --- */
                char *addrIP = inet_ntoa(address.sin_addr);
                std::cout << "new socket connection: " << clientFd << " from ip: "<< addrIP <<" port: "
                          << ntohs(address.sin_port)  << std::endl;
                Epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd ,&temp);
            }
            else{
                char buffer[4096]; //缓冲区
                auto read_count = recv(epollQueue[i].data.fd, buffer, 4096, 0);
                if (read_count == 0) {
                    std::cout << "the connect " << std::to_string(epollQueue[i].data.fd) 
                        <<" has been end !" << std::endl;
                    Epoll_ctl(epollFd, EPOLL_CTL_DEL, epollQueue[i].data.fd, nullptr); //删除
                    close(epollQueue[i].data.fd); //关闭了
                }
                if (read_count < 0){
                    if (errno == EINTR){
                        i--; //被中断 重新读取
                        continue;
                    }
                    //需要关闭
                    if (errno == ECONNRESET){
                        std::cout << "the connect " << std::to_string(epollQueue[i].data.fd) 
                            <<" has been end !" << std::endl;
                        Epoll_ctl(epollFd, EPOLL_CTL_DEL, epollQueue[i].data.fd, nullptr); //删除
                        close(epollQueue[i].data.fd); //关闭了
                    }
                    throw std::runtime_error(std::to_string(epollQueue[i].data.fd) +
                                             " read error - errno: " + std::to_string(errno) ); //发生错误
                }

                if (read_count > 0 ){
                    buffer[read_count] = '\0';
                    std::cout << "message: " << buffer << std::flush;
                    std::string  message = "get bytes " + std::to_string(read_count) + "\n";
                    auto write_count = write(epollQueue[i].data.fd,
                                             message.c_str() , sizeof(char)*message.size());
                    if (write_count <= 0){
                        throw std::runtime_error(std::to_string(epollQueue[i].data.fd) +
                                                 ": error happen in write ! errno: " + std::to_string(errno));
                    }
                }

            }
        }
    }
}


int main() {
    createEpollServer();
    return 0;
}
```

### [8. 同步IO](#)
select，poll，epoll本质上都是同步I/O，因为他们都需要在读写事件就绪后自己负责进行读写，也就是说这个读写过程是阻塞的，而异步I/O则无需自己负责进行读写，异步I/O的实现会负责把数据从内核拷贝到用户空间。

-----
