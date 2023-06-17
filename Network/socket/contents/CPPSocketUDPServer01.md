### [C++ Socket Server 基础](#)
 **介绍**：在一些特殊场景下例如游戏、online视频聊天、电话、多播、广播都是基于UDP实现的，应用同样非常广泛，所以掌握UDP Server是非常必要的！

-----

- [x] [1. UDP Server基础知识](#1-udp-server基础知识)
- [x] [2. UDP与connect](#2-udp与connect)
- [x] [3. UDP 高并发设计](#3-udp-高并发设计)
- [x] [4. epoll-udp-reactor 并发模型](#4-epoll-udp-reactor-并发模型)
- [x] [5. tcpdump简单使用](#5-tcpdump简单使用)

-----

### [1. UDP Server基础知识](#)
向上提供无连接不可靠服务,适用于实时性要求高（IP电话、视频会议等）,能够容忍少量数据包的丢弃的应用。
支持一对一，一对多，多对一和多对多交互通信， 而**TCP只能一对一通信**， **同时UDP的首部开销小**，只需要八个字节， 而TCP：最小20字节，最大60字节。

**UDP报文结构：**

<img width="600px" src="./assets/042147281873892.png" />

UDP协议设计的长度最大值为65535,但实际上由于IP不可靠，如果数据长度大于1472,就会发生IP分片，当IP层组包发生错误，那么包就会被丢弃，导致丢弃整个IP数据报分片，而UDP不可靠不重传，
导致一个大的报文直接无法重组还原，还由一堆垃圾数据在缓存区。

数据分为两种，一种是带内数据，一种是带外数据。
带内数据就是我们平常传输或者说是口头叫的数据，带外数据就是我们接下来讲的内容。

许多的传输层都具有带外数据（也称为 经加速数据 ）的概念，想法就是连接的某段发生了重要的事情，希望迅速的通知给对端。
这里的迅速是指这种通知应该在已经排队了的带内数据之前发送，也就是说，带外数据拥有更高的优先级。带外数据可以使用一条独立的传输层连接，也可以映射到传输普通数据的连接中。**其中，UDP没有实现带外数据**。

TCP中telnet、rlogin和ftp等，除了这样的远程非活跃应用之外，几乎很少有使用到带外数据的地方。

#### [1.1 UDP和IO复用](#)
UDP 编程一般不需要用到系统的 epoll 函数，不朽的经典《Unix网络编程卷一》中注明：**“大多数情况下，TCP服务器是并发的，UDP的服务器是迭代的”** 。 

##### [1.2 UDP报文传输](#)
一定要记住： **UDP是报文传输**, **TCP是流式传输** , 对于TCP而言，如果客户端发送4096字节的数据，那么客户端可以 recv(1024) 调用多次，分次夺取数据，
因为TCP是流传输，是支持这样做的，但是UDP，不性，你必须一次性全部读取出来 recvfrom(4096)。

TCP 通信读取方式是支持下面的操作，而UDP不支持。
```cpp
//客户端
send(4096)
send(512)

//服务端
recv(1024)
recv(512)
recv(512)
recv(1024)
recv(512)
recv(1024)
```
UDP的正确读取方式,必须一一对应！ [**UDP必须一次性读取完整个报文。**](#)， **如果不一一对应，会有 分片重组异常**！
```cpp
//客户端
sendto(512)    //一个报文
sendto(1024)

//服务端
recvfrom(512)
recvfrom(1024)
```

对于UDP报文，一定是一个报文一个报文的发送，一个报文一个报文的接受！


#### [1.3 UDP和MTU](#)
一个以太帧最大为1518字节 （14字节以太首部，20字节IP首部，UDP8/TCP20，因此IP包每次最大为1500==MTU。去掉协议头UDP有效数据1472字节，TCP为1460字节。还有最后的4字节CRC）

以太网(Ethernet)数据帧的长度必须在**46-1500**字节之间,这是由以太网的物理特性决定的。这个1500字节被称为链路层的MTU(最大传输单元)， **由于以太网帧的限制，长度不能超过1472字节**：

* 因为IP数据报的首部为20字节,所以IP数据报的数据区长度最大为1480字节.
* 因为UDP数据报的首部8字节,所以UDP数据报的数据区最大长度为1472字节.

虽然是1472,最好还是小于1472， 推荐使用1400，如果是游戏之类的 推荐 512字节, 鉴于Internet(非局域网)上的标准MTU值为576字节，所以建议在进行Internet的UDP编程时，最好将UDP的数据长度控制在548字节 (576-8-20)以内。

<img width="800px" src="./assets/image-20230605170932396.png" />



如果我们定义的TCP和UDP包没有超过范围，那么我们的包在IP层就不用分包了，这样传输过程中就避免了在IP层组包发生的错误；如果超过范围，既IP数据报大于1500字节，发送方IP层就需要将数据包分成若干片，而接收方IP层就需要进行数据报的重组。

更严重的是，如果使用UDP协议，当IP层组包发生错误，那么包就会被丢弃。接收方无法重组数据报，将导致丢弃整个IP数据报。UDP不保证可靠传输；但是TCP发生组包错误时，该包会被重传，保证可靠传输。



#### [1.4 IP 分片](#)
当IP数据报的长度超过帧的MTU时，它将被分片传输。分片可能发生在发送端，也可能发送在中转路由器上，而且可能在传输过程中被多次分片，但只有在最终的目标机器上，这些分片才会被内核中的IP模块重新组装。

IP的分片和重组需要的信息由头部的三个字段提供：**16位数据报标识**、**3位标志**、**13位偏移**。一个IP数据报的每个分片都具有自己的IP头部，他们具有相同的标识值，但具有不同的片偏移。并且除了最后一个分片外，其他分片都将设置MF标志。此外，每个分片的IP头部的总长度字段将被设置为该分片的长度。

- 3位标志：标识是否IP分片.第一位无用，第二位0：允许分片，1：不允许。第三位0：最后一片，1：后面还有分片
- 13位片偏移：此分片在原始数据的偏移，用于分片重组，因为13位，所以支持的最大字节为8192

<img width="700px" src="./assets/image-20230605171448684.png" />


#### [1.5 缓存区问题](#)
与TCP类似的，UDP也有可能出现缓冲区被填满后，再接收数据时丢包的现象。**由于它没有TCP滑动窗口的机制，通常采用如下两种方法解决：

* 服务器应用层设计流量控制，控制发送数据速度。
* 借助setsockopt函数改变接收缓冲区大小。如：


#### [1.6 UDP 提供的服务](#)
我们可以在UDP协议的基础上构建新的协议，根据需要的服务不同可以分为三类：
* **尽力可靠** -- 音视频通话
* **无序可靠** -- 例如迅雷分片下载文件
* **有序可靠** -- 金融转钱


### [2. UDP与connect](#)
一般而言connect系统调用是TCP 套接字使用的，UDP是一个无连接的协议，因此socket函数connect()似乎对UDP是没有意义的，然而事实不是这样。它可以用来指定本地端口和本地地址,来建立**虚拟连接**，并且还有许多的好处。

参考：[https://www.jianshu.com/p/6fbb43f78d48](https://www.jianshu.com/p/6fbb43f78d48)
#### [2.1 区别](#)
UDP的发送端可以调用connect系统调用，来创建一个连接，**UDP中connect操作与TCP中connect操作有着本质区别**。

```cpp
int connect(int socket, const struct sockaddr *address, socklen_t address_len);
```

* connect 调用引起 TCP 三次握手，建立 TCP 有效连接不同，UDP connect 函数的调用，并不会引起和服务器目标端的网络交互，也就是说，并不会触发所谓的”握手“报文发送和应答,**内核仅仅把对端ip和port记录下来。** 。
* UDP中可以多次调用connect,TCP只能调用一次connect。

UDP多次调用connect有两种用途:
* 指定一个新的ip&port连结. 
* 断开和之前的ip&port的连结.

#### [2.2 connect 使得效率变高了](#)
适用于udp一对一的经常不断发送数据的情况，可以提高效率。比如：

* 未connect的udp发送数据为，建立连接->发送数据->断开连接->建立连接->发送数据->断开连接。每次发送报文可能要做路由查询
* connect的udp发送数据为，建立连接->发送数据->发送数据->发送数据->断开连接。



#### [2.3 消耗变少了](#)
sendto的参数到内核空间以后，内核需要分配内存来存储这些参数值，当数据包从网卡发出去之后，内核还需要释放掉这块内存；
下次再调用sendto的时候，内核就需要再次分配内存存放这些临时的数据结构，周而复始，就会形成一个 不断地分配和释放临时内存的过程。

如果调用了connect，内核中就已经维护了一个”连接”，就可以调用send函数来进行发送数据了，内核中维护的这个 连接，包含了 发送端和接收端的地址， 并且永久维护一个数据结构，存储这些地址信息，后面每次进行发送数据，内核就不需要再分配删除内存了。

我们都知道Linux系统有用户空间和内核空间之分，接收数据，数据从网卡收上来，需要先交给系统内核的，然后内核再交给上层应用程序（处于用户空间）。
发送数据，也是一样，反过来，数据需要从用户空间拷贝到内核，内核处理完之后，再交给网卡发出去。

那么在用户态和内核态进行切换的时候，是非常耗时的，对于一个高性能的服务来说 应该减少这种切换，但是如果切换确实无法避免，那么应该尽量减少切换时的数据拷贝。
没有调用connect的udp socket，只能调用sendto发送数据，我们可以看到sendto的参数比send要多，这就意味着每次系统调用的时候都会多拷贝一些数据到内核空间。

#### [2.4 获取错误信息](#)
ECONNREFUSED错误，相信大家在写UDP socket程序的时候，都有遇到 这个“连接错误”的提示。UDP不是无连接的吗？怎么会有连接错误呢？而且如果我不调用connect函数，这个错误就不会报上来的，这是为什么呢？

首先我们要明白，这个错误是ICMP带过来的。

当一个 UDP socket 去 connect 对端 时，并没有发送任何的数据包，其效果仅仅是在内核建立了一个五元组映射，该映射的作用正是为了和 UDP 带外的 ICMP 控制通道捆绑在一起。当调用了connect之后，内核协议栈就维护了一个从源到目的地的单向连接。
当下层有ICMP错误信息返回时，内核就可以根据这个五元组(源IP地址,源端口,目的IP地址,目的端口和传输层协议这五个量组成)找到是那个UDP socket发的包失败了，进而可以把错误信息传输给该socket，应用程序就可以得到该错误信息了。

但是对于一个没有connect的udp socket， 内核将数据发出去之后，没有维护五元组，ICMP有错误返回的时候，内核就找不到是哪个 udp socket出的错，所以用户层也就无法收到这个错误信息的。

总之，我们在udp编程的时候，还是有必要进行connect的。


#### [2.5 UDP 连接和断开套接字的过程是怎样的？](#)
UDP 连接套接字不是发起连接请求的过程，而是记录目的地址和端口到套接字的映射关系,断开套接字则相反，将删除原来记录的映射关系。

### [3. UDP 高并发设计](#)
不能仅仅使用recvfrom接受所有数据，因为单队列有脏数据堵塞的问题，我们需要借鉴 TCP 的思想，创建fd，实现 UDP-EPOLL 模型，同时 client 也需要bind端口哦！

目前最常用的**服务器模型**有：
* 循环服务器：服务器在同一时刻只能响应一个客户端的请求，我们所学的基础知识只能实现UDP循环服务器！
* 并发服务器：服务器在同一时刻可以响应多个客户端的请求

UDP循环服务器的实现方法：
* UDP循环服务器每次从套接字上读取一个客户端的请求->处理->然后将结果返回给客户机。
* 因为UDP是非面向连接的，没有一个客户端可以老是占住服务端。只要处理过程不是死循环，服务器对于每一个客户机的请求总是能够满足。
* [但缺点是明显的一次只能处理一个用户请求](#)

#### 3.1 随机端口方式并发模型
tftpd 程序采用的是随机端口的方式。也就是说如果有一个新的客户端过来，服务器会再随机绑定一个端口号，生成一个新的socket和客户端通信。

这种方式由于每个客户端有会有独立的端口号 和 socket。故而我们可以采用多线程的方式去处理。每个线程处理一个socket和端口。

**弊端**： 这种方式只适用于小型的UDP并发服务器。
* 我们知道，端口号实际上是一个 16位二进制数字表示。而16位二进制数字的最大值是 65535 。也就是说这种方式的UDP并发服务器最多只能并发65535。
* 但实际上是做不到这么多的，因为有些端口号有其他用途，而且如果UDP服务器把所有端口号都占领了，那对于整个系统来说是一场灾难，因为没有多余的端口号可以用了。


#### 3.2 数据队列方式
对于UDP服务器，我们先创建以下几个重要线程：
* **接收线程**: 这个接收线程只从socket中读取数据，然后筛选分发到对应的数据队列中，不对数据再进一步处理。例如客户端1 的数据，就只会发送到数据队列1，客户端2的数据只会发送到数据队列2. 如果是新的客户端，则创建新的队列。
* **监听线程**:负责查询是否有新的数据队列（也就是说有没有新的客户端发数据过来，因为新的客户端发送的数据，会被放到一个新建的数据队列中去），如果有，取出数据队列，然后创建一个新的线程去处理这个新的数据队列。
数据处理线程，该线程由监听线程创建，用于从数据队列中拿出数据进行处理。

这样的话，每个客户端有对应一个数据队列，每个数据队列都对应一个线程。故而可以做到多线程并发。

#### 3.3 多进程并发方式
服务器（知名端口）等待一下客户的到来，当一个客户到来后，记下其IP和port，然后同理，服务器fork一个子进程，建立一个socket再bind一个随机端口，然后建立与客户的连接，并处理该客户的请求。
父进程继续循环，等待下一个客户的到来。在tftpd中就是使用这种技术的

#### 3.4 基于端口复用的多udp socket epoll 反应堆模型
思想借鉴自:[https://zhuanlan.zhihu.com/p/296666065](https://zhuanlan.zhihu.com/p/296666065),设置过套接字选项（SO_REUSEADDR，SO_REUSEPORT）的套接字可以在某些情况下绑定同一个套接字， UDP端口复用有以下三种情况。

* **情况1**：两个套接字只要源IP地址不一样，可以绑定同一个端口，不管SO_REUSEADDR和SO_REUSEPORT设置为多少都一样。
* **情况2**：两个套接字源IP地址相同，如果两个套接字SO_REUSEADDR或者SO_REUSEPORT都设置为1，可以绑定同一个端口。
* **情况3**：两个套接字源IP地址相同，除了情况2可以绑定同一个端口，其他情况都不能绑定同一个端口，通常第二个套接字会提示bind错误。

```cpp
int on_addr = 1;
setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on_addr, sizeof(on_addr));
 
int on_port = 1;
setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &on_port, sizeof(on_port));
```

**绑定同一端口的多个套接字如何接收数据**， UDP socket在协议栈里不能以4元组来组织，而只能通过 `{本地IP地址，本地端口}` 来组织，即便是connected UDP也不行。
这意味着一些高负载流式的UDP服务（用UDP传输流式数据，比如quic）在 连接数 （即唯一四元组的数量）很多时，socket无法通过四元组很好地进行散列，而只能退化成一条链表：

* 服务端的端口与IP均相同。（相关的socket会hash到同一个bucket）
* 客户端的端口与IP无法被利用（按照UDP语义和规范，它们‘可能’会发生变化）。

异步处理是搞定大并发的根本，接下来的问题是，如何让一个就绪的socket和一个业务逻辑连接对应起来，这个问题在同步模型下并不存在，因为一个线程只处理一个连接。

如果UDP也能像TCP一样，每一个用户接进来就为之创建一个单独的socket为其专门服务该多好，这样在大并发的时候，就可以充分复用内核UDP层的socket查找结论加上epoll的通知机制了。
理论上这是可行的，因为UDP的4元组可以唯一识别一个与之通信的客户端，虽然UDP生成无连接，不可靠，但是为每一个连接的客户端创建一个socket并没有破坏UDP的语义，只是改变了UDP
的编程模型而已，内核协议栈依然不会去刻意维护一个UDP连接，也不会进行任何的数据确认。

需要说明的是，这种方案仅仅对“长连接”的UDP有意义，比如OpenUOM这类。因为UDP是没有连接的，那么你也就不知道一个客户端什么时候会永远停止发送数据，因此必然要通过定时器来定时关闭那些在一定时间段内没有数据的socket。


### [4. 套接字设置](#)

```cpp
#include <iostream>
#include <chrono>
#include <ctime>
#include <net/sockets/wrapper.hpp>
#include "serializer/binarySerializer.h"
#include "rup/conf.h"
#include <poll.h>
#include <sys/epoll.h>

void useServer(){
    auto udp_socket_fd = muse::Socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in server_address{
            AF_INET,
            htons(15000),
            htonl(INADDR_ANY)
    };

    //绑定端口号 和 IP
    muse::Bind(udp_socket_fd, (const struct sockaddr*)&server_address, sizeof(server_address));

    while (true){
        const unsigned int buffer_size = 4096;
        char recvBuf[buffer_size] = { '\0' };

        memset(recvBuf, 0, buffer_size);

        sockaddr_in link_addr{};
        socklen_t len = sizeof(link_addr);

        int recvLen = recvfrom(udp_socket_fd, recvBuf, buffer_size, 0 ,
                               (struct sockaddr*)&link_addr, &len);
        char* addr_ip = inet_ntoa(link_addr.sin_addr);
        std::cout << "connect from " << addr_ip  << " - "
                  << std::to_string(link_addr.sin_port) << std::endl;

        //停止服务器

        std::string  message = "Get Message:" + std::to_string(recvLen) +
                               " from "+ addr_ip + " - " + std::to_string(link_addr.sin_port) + "\n";

        if (recvLen == -1){
            continue;
        }
        else if(recvLen >= sizeof(protocolHeader)) {
            auto header = (protocolHeader*)recvBuf;
            header->count  = ntohs(header->count);
            header->totalSize = ntohl(header->totalSize);
            header->pieceSize = ntohs(header->pieceSize);;
            header->order = ntohs(header->order);
            header->currentLength = ntohs(header->currentLength);
            header->acceptMinOrder = ntohs(header->acceptMinOrder);
            header->capacity = ntohl(header->capacity);

            std::cout << "header count:" << header->count << std::endl;
            std::cout << "header totalSize:" << header->totalSize << std::endl;
            std::cout << "header pieceSize:" << header->pieceSize << std::endl;
            std::cout << "header order:" << header->order << std::endl;
            std::cout << "header currentLength:" << header->currentLength << std::endl;
            std::cout << "header capacity:" << header->capacity << std::endl;

            int h = sizeof(protocolHeader);
            auto start = recvBuf + h;

            muse::BinarySerializer serializer;
            serializer.write(start, recvLen - h);

            int a;
            std::list<std::string> names;
            std::vector<double> scores ;
            serializer.output(a);
            serializer.outputArgs(names,scores);

            for (auto& name: names) {
                std::cout << name << std::endl;
            }
        }

        auto write_count = sendto(udp_socket_fd, message.c_str() , sizeof(char)*message.size(),
                                  0, (struct sockaddr*)&link_addr, len);
        if(write_count <= 0)
        {
            std::cout << "write error" << std::endl;
            break;
        }
    }
    std::cout << "server end!\n" << std::flush;
    close(udp_socket_fd);
}


namespace epollSetting{
    int globEpollFd;
}

auto Epoll_ctl(int _epfd, int _op, int _fd, struct epoll_event *_event) ->decltype(epoll_ctl(_epfd,_op,_fd, _event)){
    auto v = epoll_ctl(_epfd,_op,_fd, _event);
    if (v != 0){
        throw std::runtime_error(std::to_string(_fd) +
         ": error happen in Epoll_ctl ! errno: " + std::to_string(errno));
    }
}

struct EventHandler {
    static const int BUFFER_LENGTH = 4096;
    int fd{0};                                                 //要监听的文件描述符
    uint32_t events{0};                                        //对应的监听事件
    void *arg{nullptr};                                        //泛型参数
    void (*callBack)(int fd, uint32_t events, void *arg){};    //回调函数
    bool status;                                               //是否在监听:1->在红黑树上(监听), 0->不在(不监听)
    char *buf{nullptr};
    unsigned long len{0};
    sockaddr_in address{};
    std::time_t last_active{0};                                //记录每次加入红黑树 g_efd 的时间值
    EventHandler(int _fd, uint32_t _events, bool _status)
    :fd(_fd), events(_events),status(_status){}
};


//处理读事件 arg 就是 EventHandler*
void clientReadCallBack(int sonUdpFd, uint32_t event, void *arg){
    auto handler = (EventHandler *)arg;
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);;
    const unsigned int buffer_size = 4096;

    char recvBuf[buffer_size] = { '\0' };
    auto recvLen = recvfrom(sonUdpFd, recvBuf, buffer_size, 0 ,(struct sockaddr*)&addr, &len);
    char* addr_ip = inet_ntoa(addr.sin_addr);
    std::cout << "son socket get data: "<< handler->fd << " --------------------------------------------------------------------" << std::endl;
    if (recvLen == -1){
        std::cout << "error " << std::endl;
        return;
    }
    else if(recvLen >= sizeof(protocolHeader)) {
        auto header = (protocolHeader*)recvBuf;
        header->count  = ntohs(header->count);
        header->totalSize = ntohl(header->totalSize);
        header->pieceSize = ntohs(header->pieceSize);;
        header->order = ntohs(header->order);
        header->currentLength = ntohs(header->currentLength);
        header->acceptMinOrder = ntohs(header->acceptMinOrder);
        header->capacity = ntohl(header->capacity);

        std::cout << "header count:" << header->count << std::endl;
        std::cout << "header totalSize:" << header->totalSize << std::endl;
        std::cout << "header pieceSize:" << header->pieceSize << std::endl;
        std::cout << "header order:" << header->order << std::endl;
        std::cout << "header currentLength:" << header->currentLength << std::endl;
        std::cout << "header capacity:" << header->capacity << std::endl;

        int h = sizeof(protocolHeader);
        auto start = recvBuf + h;

        muse::BinarySerializer serializer;
        serializer.write(start, recvLen - h);

        int a;
        std::list<std::string> names;
        std::vector<double> scores ;
        serializer.output(a);
        serializer.outputArgs(names,scores);

        for (auto& name: names) {
            std::cout << "name: " << name ;
        }
        std::cout << std::endl;
    }
    std::string  message = "Get Message:" + std::to_string(recvLen) +
                           " from "+ addr_ip + " - " + std::to_string(addr.sin_port) + "\n";

    send(sonUdpFd, message.c_str(), sizeof(char)*message.size(),0);


    sockaddr_in addrClose{};
    socklen_t clen = sizeof(addrClose);;
    addrClose.sin_family = AF_UNSPEC;
    auto result = connect(sonUdpFd, (struct sockaddr*)&addrClose, clen );
    if (result == -1){
        std::cout << "close error !" << std::endl;
        exit(0);
    }
    Epoll_ctl(epollSetting::globEpollFd, EPOLL_CTL_DEL, sonUdpFd,nullptr);
    auto close_result = close(sonUdpFd);
    std::cout << "close_result :"<< close_result << std::endl;
}

//处理新到的客户端
void serverCallBack(int udpFd, uint32_t event, void *arg){
    auto handler = (EventHandler *)arg;
    auto monitor_event = EPOLLIN | EPOLLRDHUP |EPOLLERR;
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);;
    const unsigned int buffer_size = 4096;
    char recvBuf[buffer_size] = { '\0' };
    auto recvLen = recvfrom(udpFd, recvBuf, buffer_size, 0 ,(struct sockaddr*)&addr, &len);
    char* addr_ip = inet_ntoa(addr.sin_addr);

    //停止服务器
    std::string  message = "Get Message:" + std::to_string(recvLen) + " from "+ addr_ip + " - " + std::to_string(addr.sin_port) + "\n";
    std::cout << "server get data: "<< handler->fd <<" --------------------------------------------------------------------" << std::endl;
    if (recvLen == -1){
        std::cout << "error " << std::endl;
        return;
    }
    else if(recvLen >= sizeof(protocolHeader)) {
        auto header = (protocolHeader*)recvBuf;
        header->count  = ntohs(header->count);
        header->totalSize = ntohl(header->totalSize);
        header->pieceSize = ntohs(header->pieceSize);;
        header->order = ntohs(header->order);
        header->currentLength = ntohs(header->currentLength);
        header->acceptMinOrder = ntohs(header->acceptMinOrder);
        header->capacity = ntohl(header->capacity);

        std::cout << "header count:" << header->count << std::endl;
        std::cout << "header totalSize:" << header->totalSize << std::endl;
        std::cout << "header pieceSize:" << header->pieceSize << std::endl;
        std::cout << "header order:" << header->order << std::endl;
        std::cout << "header currentLength:" << header->currentLength << std::endl;
        std::cout << "header capacity:" << header->capacity << std::endl;

        int h = sizeof(protocolHeader);
        auto start = recvBuf + h;

        muse::BinarySerializer serializer;
        serializer.write(start, recvLen - h);

        int a;
        std::list<std::string> names;
        std::vector<double> scores ;
        serializer.output(a);
        serializer.outputArgs(names,scores);

        for (auto& name: names) {
            std::cout << "name: " << name << std::endl;
        }
    }

    auto write_count = sendto(udpFd, message.c_str() , sizeof(char)*message.size(),
                              0, (struct sockaddr*)&addr, len);

    int son_fd = -1;
    if ((son_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("child socket");
        exit(1);
    } else {
        printf("parent:%d  new:%d\n", handler->fd, son_fd);
    }

    int on_addr = 1; //地址复用 端口复用
    setsockopt(son_fd, SOL_SOCKET, SO_REUSEADDR, &on_addr, sizeof(on_addr));

    int on_port = 1; //地址复用 端口复用
    setsockopt(son_fd, SOL_SOCKET, SO_REUSEPORT, &on_port, sizeof(on_port));

    sockaddr_in server_address{
            AF_INET,
            htons(10086),
            htonl(INADDR_ANY)
    };

    auto ret = bind(son_fd, (struct sockaddr *) &server_address, sizeof(server_address));
    if (ret){
        perror("child bind filed");
        exit(1);
    } else {

    }

    addr.sin_family = PF_INET;
    if (connect(son_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
        perror("child connect failed");
        exit(1);
    } else {
        std::cout << "socket connect success" << std::endl;
    }

    //设置超时时间
    struct timeval timeout = {3,0};
    setsockopt(son_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

    //处理新到连接
    auto* clientHandler = new EventHandler(son_fd, monitor_event, false);
    struct epoll_event temp {monitor_event, {.ptr = clientHandler}};
    clientHandler->status = true;
    clientHandler->callBack = clientReadCallBack;
    clientHandler->address.sin_addr.s_addr = addr.sin_addr.s_addr;
    clientHandler->address.sin_port = addr.sin_port;
    clientHandler->last_active = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    Epoll_ctl(epollSetting::globEpollFd, EPOLL_CTL_ADD, son_fd, &temp);
}

void UDPEpollConnectServerTest(){
    const int OPEN_MAX_CLIENT_CONNECTION = 2049;
    auto udpServerSocketID = muse::Socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    sockaddr_in server_address{
            AF_INET,
            htons(10086),
            htonl(INADDR_ANY)
    };

    //绑定端口号 和 IP
    muse::Bind(udpServerSocketID, (const struct sockaddr*)&server_address, sizeof(server_address));

    auto epollFd = epoll_create(128);

    if (epollFd == -1 ){
        throw std::runtime_error(std::to_string(__LINE__) + "in errno: " + std::to_string(errno));
    }

    epollSetting::globEpollFd = epollFd;

    struct epoll_event temp{EPOLLIN | EPOLLERR}, epollQueue[OPEN_MAX_CLIENT_CONNECTION];
    //处理新到连接
    EventHandler serverHandler(udpServerSocketID, EPOLLIN|EPOLLERR, true);
    serverHandler.callBack = serverCallBack;
    struct epoll_event serverEpollEvent{EPOLLIN | EPOLLERR, {.ptr = &serverHandler}};

    Epoll_ctl(epollFd, EPOLL_CTL_ADD, udpServerSocketID, &serverEpollEvent);

    int on_addr = 1; //地址复用 端口复用
    setsockopt(udpServerSocketID, SOL_SOCKET, SO_REUSEADDR, &on_addr, sizeof(on_addr));

    int on_port = 1; //地址复用 端口复用
    setsockopt(udpServerSocketID, SOL_SOCKET, SO_REUSEPORT, &on_port, sizeof(on_port));

    bool running  = true;
    std::cout << "the udp server start to run:" << std::endl;
    while (running){
        auto readyCount = epoll_wait(epollFd, epollQueue,OPEN_MAX_CLIENT_CONNECTION, -1);
        for (int i = 0; i < readyCount; ++i) {
            auto handler = (EventHandler *)epollQueue[i].data.ptr;
            if (epollQueue[i].events & EPOLLERR){
                std::cout << "the connection has error so end :socket fd:" << handler->fd << std::endl;
                running = false; //有错误停止运行
                break;
            }
            if (epollQueue[i].events & EPOLLRDHUP){
                std::cout << "the connection has been end :socket fd:" << handler->fd << std::endl;
                Epoll_ctl(epollFd, EPOLL_CTL_DEL,handler->fd,nullptr);
                close(handler->fd);
            }else{
                if (handler != nullptr) handler->callBack(handler->fd, epollQueue[i].events, handler);
            }
        }
    }
}


int main() {
    UDPEpollConnectServerTest();
    return 0;
}



```

### [5. tcpdump简单使用](#) 

监听任何网卡端口为8888的数据包
```shell
tcpdump -i any 'port 8888'
```

监听任何网卡，tcp数据且端口为8888的数据包。
```shell
tcpdump -i any 'tcp port 8888'
```

监听任何网卡， tcp数据且 源端口为8888的数据包。
```shell
tcpdump -i any 'tcp src port 8888'
```


监听任何网卡， tcp数据且目地端口为8888的数据包。
```shell
tcpdump -i any 'tcp dst port 8888'
```

监听任何网卡， tcp数据且源端口为8888的数据包或者目地端口为9999的udp包
```shell
tcpdump -i any 'tcp src port 8888 or udp dst port 9999'
```


监听任何网卡， tcp数据且源端口为8888的数据包或者目地端口为9999的udp包 以 ASCII和十六进制显示详细的输出。
```shell
tcpdump -i any 'tcp src port 8888 or udp dst port 9999' -XX -nn -vv
```



-----

