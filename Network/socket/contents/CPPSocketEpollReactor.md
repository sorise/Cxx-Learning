### [C++ IO epoll 反应堆模型](#)
 **介绍**： 对epoll较为完整的使用方式，服务端读写较为完整的封装模式， 属于Libevent库的核心思想， 主要技术为 epoll接口 + 非阻塞 + ET边沿触发 + union epoll_data. void*ptr。

-----

- [x] [1. epoll反应堆模型](#1-epoll反应堆模型)
- [x] [2. 基于堆存储的epoll反应堆](#2-基于堆存储的epoll反应堆)
- [x] [3. epoll知识点](#)

-----

### [1. epoll反应堆模型](#)
之前的服务器模式都是监听 EPOLLIN 事件，也就是只关心读，但是服务器也会向客户端发送数据，如果写数据多的花，写操作就很可能阻塞服务器，或者写任务无法完成，我们必须考虑将读写事件串起来。


**epoll普通模型**
1. 创建服务端 socket，设置服务端socket为非阻塞模式，设置TCP_DEFER_ACCEPT、TCP_NODELAY、SO_LINGER、SO_REUSEADDR等等！
2. 绑定端口，开启监听
3. 创建epoll池(红黑树) `auto epollFd = epoll_create(1);` 
4. 创建 struct epoll_event 设置事件。 `struct epoll_event server_et{EPOLLIN | EPOLLERR, {.fd =  server_fd}};`
5. 服务器 fd 添加到 epoll中 `epoll_ctl(epollFd, EPOLL_CTL_ADD, server_fd, &server_et);` 
6. epoll_wait 设置监听 `auto readyCount = epoll_wait(epollFd, epollQueue, 2048 , -1)`
7. 满足条件，对返回的数组epollQueue， 进行 `readyCount`次轮询。
8. 判断 `epollQueue[i].data.fd == server_fd` 如果是， 有新连接，调用 `accept` 然后 将新连接加入 epoll中
9. 如果是读事件，读取数据
10. 然后向客户端写数据，完毕后，进行下一步
11. 处理完毕所有事件后回到epoll_wait

#### [1.1 epoll普通模型的问题](#)
当服务器向客户端写数据的时候，如果滑动窗口不够、写缓存已经满了，**将会导致整个流程被卡住**，当然服务端也可以使用 非阻塞写，但是这样就需要反复轮询了，借助EPOLLOUT事件可以解决这个问题。
为了实现这个想法，我们需要一些数据结构的帮助！

epoll_data有一个数据域 `void *ptr` 这是一个无类型指针，可以指向用户自定义任意类型数据， 这就是我们需要利用的, 来实现回调写的模式！

```cpp
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

**比如说：** 
```cpp
struct package{
    int fd;
    void (*callBack)(int fd);
};

struct package server {
	server_fd, 
	[](int fd) { /* 执行一些任务 */ }
};

struct epoll_event server_et{EPOLLIN | EPOLLERR, {.ptr =  &server}};

epoll_ctl(epollFd, EPOLL_CTL_ADD, server_fd, &server_et);
```
这样做，只需要传递给 callBack不一样的任务，然后每个事件来了，都执行统一的callBack就行了， 而且还可以给package添加更多的字段！


#### [1.2 epoll反应堆模型](#)
这个模型将读写都纳入了 epoll  监听中！ 重点是 9 - 13步！

1. 创建服务端 socket，设置服务端socket为非阻塞模式，设置TCP_DEFER_ACCEPT、TCP_NODELAY、SO_LINGER、SO_REUSEADDR等等！
2. 绑定端口，开启监听
3. 创建epoll池(红黑树) `auto epollFd = epoll_create(1);` 
4. 创建 struct epoll_event 设置事件。 `struct epoll_event server_et{EPOLLIN | EPOLLERR, {.fd =  server_fd}};`
5. 服务器 fd 添加到 epoll中 `epoll_ctl(epollFd, EPOLL_CTL_ADD, server_fd, &server_et);` 
6. epoll_wait 设置监听 `auto readyCount = epoll_wait(epollFd, epollQueue, 2048 , -1)`
7. 满足条件，对返回的数组epollQueue， 进行 `readyCount`次轮询。
8. 判断 `epollQueue[i].data.fd == server_fd` 如果是， 有新连接，调用 `accept` 然后 将新连接加入 epoll中
9. 如果是读事件，读取数据，然后将`fd` 从epoll池(红黑树)摘下来，然后监听 `fd` 的写事件 ( `EPOLLOUT` ) 再设置 回调函数 `callBack`
10. 再将其 挂回红黑树。
11. 等待 epoll_wait 返回，如果返回 需要写的fd，调用 callBack 将数据写给客户端
12. 写完毕后将`fd` 从epoll池(红黑树)摘下来。
13. 修改监听事件为 EPOLLIN 然后，再挂回去！
14. 处理完毕所有事件后回到epoll_wait

### [2. 基于堆存储的epoll反应堆](#) 
首先是 ptr指向的数据结构～ ，在这种情况下，epollFd必须全局化!

```cpp
namespace epollSetting{
    int globEpollFd;
}

struct EventHandler {
    static const int BUFFER_LENGTH = 4096;
    int fd{0};                                                 //要监听的文件描述符
    uint32_t events{0};                                             //对应的监听事件
    void *arg{nullptr};                                              //泛型参数
    void (*callBack)(int fd, uint32_t events, void *arg){};       //回调函数
    bool status;                                             //是否在监听:1->在红黑树上(监听), 0->不在(不监听)
    char *buf{nullptr};
    unsigned long len{0};
    sockaddr_in address{};
    std::time_t last_active{0};                                       //记录每次加入红黑树 g_efd 的时间值
    EventHandler(int _fd, uint32_t _events, bool _status)
    :fd(_fd), events(_events),status(_status){}
};
```

```cpp
void serverCallBack(int fd, uint32_t event, void *arg);  //处理新到的客户端
void clientReadCallBack(int fd, uint32_t event, void *arg); //处理读事件 arg 就是 EventHandler*
void clientWriteCallBack(int fd, uint32_t event, void *arg); //处理写事件 arg 就是 EventHandler*
```


```cpp
//处理写事件 arg 就是 EventHandler*
void clientWriteCallBack(int fd, uint32_t event, void *arg){
    wrap::Epoll_ctl(epollSetting::globEpollFd, EPOLL_CTL_DEL, fd, nullptr);
    auto handler = (EventHandler*)arg;
    auto write_count = send(fd ,handler->buf , handler->len,0);
    if (write_count <= 0){
        throw std::runtime_error(std::to_string(fd) +
                                 ": error happen in write ! errno: " + std::to_string(errno));
    }
    //挂回去
    free(handler->buf); //释放掉
    handler->len = 0;
    handler->buf = nullptr;
    handler->callBack = clientReadCallBack;
    auto monitor_event = EPOLLIN;
    handler->last_active = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct epoll_event temp{monitor_event, {.ptr = handler}};
    wrap::Epoll_ctl(epollSetting::globEpollFd, EPOLL_CTL_ADD, fd, &temp);
}
```

```cpp
//处理读事件 arg 就是 EventHandler*
void clientReadCallBack(int fd, uint32_t event, void *arg){
    auto handler = (EventHandler *)arg;
    char buffer[2049]; //缓冲区
    auto read_count = recv(fd, buffer, 2048, 0);
    if (read_count == 0) {
        std::cout << "the connect " << std::to_string(fd) <<" has been end !" << std::endl;
        close(fd); //关闭了
        delete handler;
        handler = nullptr;
    }
    if (read_count < 0){
        if (errno == EINTR){
            return;
        }
        //需要关闭
        if (errno == ECONNRESET){
            std::cout << "the connect " << std::to_string(fd)
                      <<" has been end !" << std::endl;
            delete handler;
            handler = nullptr;
            wrap::Epoll_ctl(epollSetting::globEpollFd, EPOLL_CTL_DEL, fd , nullptr);
            close(fd); //关闭了
            return;
        }
        wrap::Epoll_ctl(epollSetting::globEpollFd, EPOLL_CTL_DEL, fd , nullptr);
        throw std::runtime_error(std::to_string(fd) +
                                 " read error - errno: " + std::to_string(errno) ); //发生错误
    }
    if (read_count > 0 ){
        char *addrIP = inet_ntoa(handler->address.sin_addr);
        buffer[read_count] = '\0';
        std::cout << addrIP <<" - " << ntohs(handler->address.sin_port) << ": " << buffer << std::flush;
        std::string  message = "get bytes " + std::to_string(read_count) + "\n";
        char *sendData = new char[message.size() + 1];
        for (int i = 0; i < message.size(); ++i) {
            sendData[i] = message[i];
        }
        sendData[message.size()] = '\0';
        auto monitor_event = EPOLLOUT;
        auto clientWriteHandler = handler;
        handler->callBack = clientWriteCallBack;
        handler->len = message.size();
        handler->buf = sendData;
        handler->status = true;
        handler->last_active = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct epoll_event temp{monitor_event, {.ptr = clientWriteHandler}};
		wrap::Epoll_ctl(epollSetting::globEpollFd, EPOLL_CTL_MOD, fd , &temp);
        //重新放回去
    }
}
```

```cpp
//处理新到的客户端
void serverCallBack(int fd, uint32_t event, void *arg){
    auto monitor_event = EPOLLIN;
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);;
    auto clientFd =  wrap::Accept(fd, (struct sockaddr*)&addr, &len);
    char *addrIP = inet_ntoa(addr.sin_addr);
    std::cout << "new socket connection: " << clientFd << " from ip: "<< addrIP <<" port: "
              << ntohs(addr.sin_port)  << std::endl;
    auto* clientHandler = new EventHandler(clientFd, monitor_event, false);
    struct epoll_event temp {monitor_event, {.ptr = clientHandler}};
    clientHandler->status = true;
    clientHandler->callBack = clientReadCallBack;
    clientHandler->address.sin_addr.s_addr = addr.sin_addr.s_addr;
    clientHandler->address.sin_port = addr.sin_port;
    clientHandler->last_active = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    wrap::Epoll_ctl(epollSetting::globEpollFd, EPOLL_CTL_ADD, clientFd ,&temp);
}
```

#### [2.1 运行函数](#)
通过上面的三个函数， 就对通用化很多了！ 直接调用 `handler->callBack(handler->fd, epollQueue[i].events, handler);` 就不需要判断事件、socket类型了！

```cpp
void createReactor(){
    auto serverSocketFd = wrap::initServer(15000, 128);
    auto epollFd = epoll_create(1);
    if (epollFd == -1 ){
        throw std::runtime_error("error is happen! errno: " + std::to_string(errno));
    }
    epollSetting::globEpollFd = epollFd;
    struct epoll_event epollQueue[OPEN_MAX_CLIENT_CONNECTION];

    EventHandler serverHandler(serverSocketFd, EPOLLIN|EPOLLERR, true);
    serverHandler.callBack = serverCallBack;
    struct epoll_event server_ev {EPOLLIN|EPOLLERR,{.ptr = &serverHandler}};
    wrap::Epoll_ctl(epollFd, EPOLL_CTL_ADD,serverSocketFd, &server_ev);
    bool running  = true;
    while (running){
        auto readyCount = epoll_wait(epollFd, epollQueue,
                                     OPEN_MAX_CLIENT_CONNECTION, -1);
        for (int i = 0; i < readyCount; ++i) {
            if (epollQueue[i].events & EPOLLERR){
                running = false; //有错误停止运行
                break;
            }
            auto handler = (EventHandler *)epollQueue[i].data.ptr;
            handler->callBack(handler->fd, epollQueue[i].events, handler);
        }
    }
}
```

### [3. epoll知识点](#)

* [epoll 是线程安全的！](#)



-----
