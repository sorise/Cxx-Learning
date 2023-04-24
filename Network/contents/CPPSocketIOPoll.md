### [C++ IO 多路复用 poll，epoll](#)
 **介绍**： 专属于Linux的网络IO复用模式，并不支持windows! 相比于select没有描述符个数的限制。

-----

- [x] [1. poll概述](#1-poll概述)
- [x] [2. poll 服务器](#2-poll-服务器)
- [x] [3. epoll](#3-epoll)
- [x] [4. linux 使用 epoll 需要的设置](#4-linux-使用-epoll-需要的设置)
- [x] [5. epoll 系统调用](#5-epoll 系统调用)

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


-----
