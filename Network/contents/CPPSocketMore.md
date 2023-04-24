### [C++ Socket 多进程、多线程服务器](#)
 **介绍**： Socket怎么实现可以同时服务多个客户请求的并发服务器！

-----

- [x] [1. 并发服务器](#1-并发服务器)
- [x] [2. linux TCP 多进程服务器](#2-linux-tcp-多进程服务器)
- [x] [3. linux TCP 多线程服务器](#3-linux-tcp-多线程服务器)
- [x] [4. 套接字设置](#4-套接字设置)
- [x] [5 shutdown函数](#5-shutdown函数)

-----

### [1. 并发服务器](#)
平常写的简单Socket客户端服务器通信，服务器每次只能处理一个客户的请求，它实现简单但效率很低，通常这种服务器被称为迭代服务器/单进程、线程服务器。 
然而在实际应用中，不可能让一个服务器长时间地为一个客户服务，而需要其具有同时处理多个客户请求的能力，**这种同时可以处理多个客户请求的服务器称为并发服务器**，其效率很高却实现复杂。在实际应用中，并发服务器应用的最广泛。

linux有3种实现并发服务器的方式：多进程并发服务器，多线程并发服务器，IO复用。

### [2. linux TCP 多进程服务器](#) 
基本思想很简单就是每来一个连接就创建一个进程去处理这个连接，处理完成后进程自己消亡！ ([**已经不再使用，可以直接调到第三节！**](3-多线程服务器))
创建多进程服务器需要借助linux系统下的fork()函数，[windows 并没有提供类似于fock的api，但是提供了创建进程的函数CreateProcess](#)。

getpid()  returns  the  process ID (PID) of the calling process.
```cpp
#include <sys/types.h>
#include <unistd.h>

pid_t getpid(void);
```

#### [2.1 fork](#)
fork（）是一个系统调用，用于创建进程。创建的这个进程与原来进程几乎完全相同。这个新产生的进程称为子进程。 fork调用的一个奇妙之处就是它仅仅被 **调用一次**，却能够 **返回两次**，它可能有三种不同的返回值：
* 负值：创建子进程失败。
* 零：返回到新创建的子进程。
* 正值：返回父进程或调用者。该值包含新创建的子进程的进程ID

由于在复制时复制了父进程的堆栈段，所以两个进程都停留在fork函数中，等待返回。因此fork函数会返回两次，一次是在父进程中返回，另一次是在子进程中返回，这两次的返回值是不一样的。

<img width="400px" src="./assets/a9d3fd1f4134970a6e32724395cad1c8a7865d1c.png" />

```cpp
#include<unistd.h>
pid_t fork( void);
```
fork 函数会新生成一个进程，调用 fork 函数的进程为父进程，新生成的进程为子进程。在父进程中返回子进程的 **pid**，在子进程中返回 **0**，失败返回 **-1**。

```cpp
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
 
int main(int argc,char *argv[]){
    pid_t pid=fork();
    if ( pid < 0 ) {
        fprintf(stderr,"错误！");
    } else if( pid == 0 ) {
        printf("子进程空间");
        exit(0);
    } else {
        printf("父进程空间，子进程pid为%d",pid);
    }
    // 可以使用wait或waitpid函数等待子进程的结束并获取结束状态
    exit(0);
}
```

#### [2.2 vfork](#)
vfork()会产生一个新的子进程.但是vfork创建的子进程与父进程共享数据段,而且由vfork()创建的。

* [vfork(): 子进程与父进程共享数据段](#)
* [vfork(): 保证子进程先运行，在调用exec()或exit()之前，与父进程数据共享，在exec()或exit()调用之后，父进程才能运行](#)
* [在使用vfork函数在调用exec()或exit()之前，子进程依赖于父进程的进一步动作，将会导致死锁。](#)

```cpp
#include<unistd.h>
pid_t vfork(void);
```
当使用 vfork()创 建新进程时，父进程将被暂时阻塞，而子进程则可以借用父进程的地址空间运行。这个奇特 状态将持续直到子进程要么退出，要么调用 execve()，至此父进程才继续执行。
fork当需要改变共享数据段中变量的值，则拷贝父进程（写时复制）；vfork()的子进程里不能访问/修改数据。

#### [2.3 linux资源拷贝方式](#)
在创建新进程时，要进行资源拷贝。Linux 有三种资源拷贝的方式：

* **共享**：新老进程共享通用的资源。当共享资源时，两个进程共同用一个数据结构，不需要为新进程另建。
* **直接拷贝**：将父进程的文件、文件系统、虚拟内存等结构直接拷贝到子进程中。子进程创建后，父子进程拥有相同的结构。
* **写时复制（Copy on Write）**：拷贝虚拟内存页是相当困难和耗时的工作，所以能不拷贝就最好不要拷贝，如果必须拷贝，也要
尽可能地少拷贝。为此，Linux 采用了 Copy on Write技术，把真正的虚拟内存拷贝推迟到两个进程中的任一个试图写虚拟页
的时候。如果某虚拟内存页上没有出现写的动作，父子进程就一直共享该页而不用拷贝。

[fork 用于普通进程的创建，采用的是 Copy on Write 方式；而 vfork 使用完全共享的创建，新老进程共享同样的资源，完全没有拷贝。](#)

#### [2.4 僵死进程](#)
僵死进程概念： [子进程先于父进程结束，父进程没有调用 wait 获取子进程退出码。](#24-僵死进程)

僵死进程的危害：
* 僵死进程的PID还占据着，意味着海量的子进程会占据满进程表项，会使后来的进程无法fork.
* 僵尸进程的内核栈无法被释放掉，为啥会留着它的内核栈，因为在栈的最低端，有着thread_info结构，它包含着 struct_task 结构，这里面包含着一些退出信息

如何处理僵死进程：父进程通过调用  **wait()** 完成。

```cpp
pid_t  wait(int *status);
```
进程一旦调用了wait，就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，如果让它找到了这样一个已经变成僵尸的
子进程，wait就会收集这个子进程的信息，并把它彻底销毁后返回；如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止。 

#### [2.5 fork和多线程](#)
1. **多线程中某个线程调用 fork()，子进程会有和父进程相同数量的线程吗？**

在Linux中，fork的时候只复制当前线程到子进程，也就是说除了调用fork的线程外，其他线程在子进程中“蒸发”了。

2. **父进程被加锁的互斥锁 fork 后在子进程中是否已经加锁？**

调用fork的时候，会复制父进程的所有锁到子进程中。
假设在fork之前，一个线程对某个锁进行的lock操作，即持有了该锁，然后另外一个线程调用了fork创建子进程。可是在子进程中持有那个锁的线程却"消失"了，从子进程的角度来看，这个锁被“永久”的上锁了，因为它的持有者“蒸发”了。

#### [2.6 子进程回收](#)
进程的回收通过wait()和waitpid()两个函数实现，但是每次只能回收一个子进程，如果创建了多个子进程的话需要调用多次，如下：


```cpp
#include <sys/wait.h> 
pid_t wait(int *statloc); //参数 statloc 返回子进程的终止状态（一个整数）
```
当调用该函数时，如果有一个子进程 已经终止，则该函数立即返回，并释放子进程所有资源，返回值是终止子进程的 ID 号。如果当前没有终止的子进程，但有正
在执行的子进程，则 wait 将阻塞直到有子进程终止时才返 回。如果当前既没有终止的子进程，也没有正在执行的子进程，则返回错误-1

```cpp
#include <sys/wait.h> 
waitpid(pid_t pid ,int *statloc, int option);
```
当参数 pid 等于-1 而 option 等于 0 时，该函数等同于 wait()函数。 参数 pid 指定了父进程要求知道哪些子进程的状态，当 pid 取-1 时，要求知道任何一个子进程的终止状态。
当 pid 取值大于 0 时，要求知道进程号为 pid 的子进程的终止状态。当 pid 取值小于-1 时，要求知道进程组号为 pid 的绝对值的子进程的终止状态。
* 参数 option 让用户指定附加选项。最常用的选项是 WNO_HANG，它通知内核在没有已 终止子进程时不要阻塞。
* 当前有终止的子进程时，返回值为子进程的 ID 号，同时参数 statloc 返回子进程的终止 状态。否则返回值为-1。
* 和wait较大的不同是waitpid可以循环调用，等待所有任意进程结束，而wait只有一次机会。

```cpp
//最后要回收所有子进程
for(int j = 0 ; j < child_process_count ; j++){
	wait(NULL);
}
```
一般的回收机制都属于阻塞回收，父进程阻塞等待子进程技术，收到子进程的退出状态。然而在实验中我需要实现的属于client-server模型。父进程属于守护进程一类，开启TCP套接字等待链接，
每当有请求到来，便fork一个子进程传输信息并自由退出。父进程并不关注子进程的退出状态，是否正常都不影响今后的服务，但子进程变成僵尸进程便麻烦了，随着时间的进行，僵尸进程一大
堆，虽然占用资源不多，但终究是个隐患。

在这个模型里，父进程是不能阻塞回收子进程的，不然不保证其他请求可以正常到达。

**解决办法：**

1、建立信号处理handle
```cpp
signal(SIGCHLD, sigchld_handler);
```
* SIGUSR1 用户自定义信号 默认处理：进程终止
* SIGUSR2 用户自定义信号 默认处理：进程终止
2、信号处理handle的实现
```cpp
static void sigchld_handler(int signo) {
	pid_t pid;
	pid = waitpid(-1, &waiter, WNOHANG);
}
```
* 这里采用waitpid，说明不能阻塞；
* -1表示回收任意子进程；
* WNOHANG表示不阻塞。

#### [2.7 例子 1](#)
部分片段！

```cpp
/* 绑定listenfd描述符  */ 
if(bind(listenfd,(struct sockaddr*)(&servaddr),sizeof(struct sockaddr))==-1){
fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
exit(1);
}
/* 监听listenfd描述符*/
if(listen(listenfd,5)==-1){
	fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
	exit(1);
}
for ( ; ; )  {
clilen = sizeof(cliaddr);
/* 服务器阻塞,直到客户程序建立连接  */
if((connfd=accept(listenfd,(struct sockaddr*)(&cliaddr),&clilen))==-1){
	fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
	exit(1);
}
//有客户端建立了连接后
if ( (childpid = fork()) == 0) { /*子进程*/
	close(listenfd);    /* 关闭监听套接字*/
	str_echo(connfd);   /*处理该客户端的请求*/
	exit (0);
}
close(connfd);/*父进程关闭连接套接字，继续等待其他连接的到来*/
}

void str_echo(int sockfd){
 ssize_t n;
    char  buf[MAXLINE];
    again:
      while ( (n = read(sockfd, buf, MAXLINE)) > 0)
          write(sockfd, buf, n);
      if (n < 0 && errno == EINTR)//被中断，重入
          goto again;
      else if (n < 0){//出错
        fprintf(stderr,"read error:%s\n\a",strerror(errno));
        exit(1);
      }
}
```

#### [2.8 例子 2](#)

```cpp
void perr_exit(const char *s){
    perror(s);
    exit(-1);
}

// 读取指定大小的内容
ssize_t Readn(int fd, void *vptr, size_t n)
{
    size_t nleft;  //usigned int 剩余未读取的字节数
    ssize_t nread; //int 实际读到的字节数
    char *ptr;

    ptr = vptr;
    nleft = n;

    while (nleft > 0){
        if ((nread=read(fd, ptr, nleft))<0)
        {
            if(errno=EINTR)
                nread=0;
            else
                return -1;
        } else if (nread == 0)
            break;

        nleft -= nread;
        ptr += nread;

    }
    return n - nleft;


}

ssize_t Writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft >0){
        if ((nwritten = write(fd, ptr, nleft))<=0){
            if (nwritten <0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

static ssize_t  my_read(int fd, char *ptr)
{
    static int read_cnt;
    static char *read_ptr;
    static char read_buf[100];

    if (read_cnt <=0 )
    {
        again:
        if((read_cnt = read(fd, read_buf, sizeof(read_buf)))<0){
            if (errno==EINTR)
                goto again;
            return -1;
        } else if (read_cnt == 0)
            return 0;
        read_ptr = read_buf;
    }
    read_cnt--;
    *ptr = *read_ptr++;

    return 1;
}

ssize_t Readline(int fd, void *vptr, size_t maxlen){
    ssize_t n, rc;
    char c, *ptr;
    ptr = vptr;

    for (n=1; n<maxlen; n++){
        if((rc =my_read(fd, &c))==1){
            *ptr++ = c;
            if (c == "\n")
                break;
        }else if (rc==0){
            *ptr = 0;
            return n-1;
        } else
            return -1;
    }
    *ptr = 0;
    return n;
}


int main(int agrv, char *agrc[])
{
  int lfd, cfd;
    pid_t pid;
  
    struct sockaddr_in srv_addr, clt_addr;
  
    socklen_t clt_addr_len;
    char buf[BUFSIZ];
    int ret,i;

    memset(&srv_addr, 0, sizeof(srv_addr));  // 将地址结构清0
//    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(7000);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    Bind(lfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));

    Listen(lfd, 128);
    clt_addr_len = sizeof(clt_addr);
    while (1){

        cfd = Accept(lfd, (struct sockaddr *)&clt_addr, &clt_addr_len);
        // 创建子进程
        pid = fork();
        if (pid < 0) {
            perr_exit("fork error");

        } else if (pid==0){
            // 子进程
            close(lfd);
            break;
        } else{
            // 注册信号捕捉
            struct sigaction act;
            act.sa_handler = catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            ret = sigaction(SIGCHLD, &act, NULL);
            if (ret!=0){
               perr_exit("sigaction error");
            }
            close(cfd);
            continue;
        }
    }
    if (pid == 0){
        for (;;){
        ret = read(cfd, buf, sizeof(buf));
        if(ret == 0){
            // 检测到客户端关闭了
            close(cfd);
            exit(1);
        }
        for (i=0; i<ret; i++){
            cout << buf[i] << endl;
            buf[i] = toupper(buf[i]);
        }
        request = buf;
        cout <<request << endl;
      
        write(cfd, buf, ret);
             write(STDOUT_FILENO, buf, ret);
        }
    }
}
```
#### [2.9 缺点](#)
传统的网络服务器程序大都在新的连接到达时，fork一个子进程来处理。虽然这种模式很多年使用得很好，但fork有一些问题：

* fork是昂贵的。fork时需要复制父进程的所有资源，包括内存映象、描述字等；目前的实现使用了一种写时拷贝（copy-on-write）技术，可有效避免昂贵的复制问题，但fork仍然是昂贵的;
* fork子进程后，父子进程间、兄弟进程间的通信需要进程间通信IPC机制，给通信带来了困难；
* 多进程在一定程度上仍然不能有效地利用系统资源;
* 系统中进程个数也有限制。

### [3. linux TCP 多线程服务器](#) 
基本思想就是一个线程去处理一个连接, 学习完C++并发编程，应该都懂这些内容！

#### [3.1 小例子](#)
```cpp
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <list>


/*
int Socket(int domain,int type,int protocol){
    auto socket_fd = socket(AF_INET, SOCK_STREAM, 0);
 .....
*/

void socketHandler(int socketId, sockaddr_in socketAddress){
    char* addr_ip = inet_ntoa(socketAddress.sin_addr);
    std::cout << "connect from " << addr_ip  << " - " << std::to_string(socketAddress.sin_port) << std::endl;
    while (true){
        char buffer[4096]; //缓冲区
        auto read_count = read(socketId, buffer, 4096);
        if (read_count <= 0) {
            std::cout << "the connect has been end!" << std::endl;
            break;
        }
        buffer[read_count] = '\0';
        std::cout << "message: " << buffer << std::flush;
        std::string  message = "Get Message:" + std::to_string(read_count) + 
			" from "+ addr_ip + " - " + std::to_string(socketAddress.sin_port) + "\n";
        auto write_count = write(socketId, message.c_str() , sizeof(char)*message.size());
        if(write_count <= 0)
        {
            std::cout << "write error" << std::endl;
            break;
        }
    }
    close(socketId); //关掉！
}

int main() {
    std::cout << "start server at 127.0.0.1 running in 15000:" << "\n";

    //创建套接字
    auto fd = Socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address{
            AF_INET,
            htons(15000),
            htonl(INADDR_ANY)
    };
    //绑定端口号 和 IP
    Bind(fd, (const struct sockaddr*)&server_address, sizeof(server_address));
    //开启监听模式
    Listen(fd, 20);
    //等待连接

    while (true){
        sockaddr_in link_addr{};
        socklen_t len = sizeof(link_addr);
        auto new_socket_fd = Accept(fd, (struct sockaddr*)&link_addr, &len);
        std::thread task(socketHandler, new_socket_fd, link_addr);
        task.detach();
    }

    for (auto &td: stacks) {
        if (td.joinable())
            td.join();
    }
    close(fd); //关闭socket
    return 0;
}
```

### [4. 套接字设置](#)
可以使用setsockopt函数设置socket的一些选项，例如阻塞或者非阻塞，缓冲区大小之类的，超时时间等等！

```cpp
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
//成功时返回0，失败时返回-1。
int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
```
参数：  
* sockfd : Socket的句柄！ 
* level: SOL_SOCKET 、 IPPROTO_TCP 、IPPROTO_IP 和 IPPROTO_IPV6
* optname：选项
* optval：指针，指向存放选项值的缓冲区
* optlen：optval缓冲区长度

两种套接口的选项：一种是布尔型选项，允许或禁止一种特性；另一种是整型或结构选项。

#### [4.1 选项](#)
套接子具有许多的特性，理解这些特性根据实际需要进行更改也十分重要！ 套接字的可选项是分层的

|协议层|选项名|类型|说明|能否设置|
|:---|:---|:---|:----|:----|
|SOL_SOCKET |SO_TYPE |int |套接字类型 - UDP/TCP | No |
| |SO_REUSEADDR |bool |继续重用该socket | YES |
| |SO_REUSEPORT |bool |设置端口重用，用于解决多进程同时监听同一端口的问题。 | YES|
| |SO_KEEPALIVE |bool |发送 “保持活动”包 | YES |
| |SO_OOBINLINE |bool |在常规数据流中接收带外数据。| YES |
| |SO_RCVBUF |int |为接收确定缓冲区大小 | YES |
| |SO_DEBUG |bool |记录调试信息 | YES |
| |SO_ERROR |int |获取错误状态并清除 |NO |
| |SO_SNDTIMEO |发送超时 |int |YES |
| |SO_RCVTIMEO |接受超时 |int |YES |
| |SO_LINGER |发送完再关闭 |结构体 linger |YES |
| |SO_RCVLOWAT |接收低潮限度 |int 字节数量|YES |
| |SO_SNDLOWAT |发送低潮限度 |int 字节数量 |YES |
|` `| | | | |
|IPPROTO_IP |IP_TOS | |服务类型（TOS）字段 |YES |
|` `| | | | |
|` ` | | | | |
|` ` | | | | |
|IPPROTO_TCP |TCP_NODELAY |bool |禁止发送合并的Nagle算法 |YES | 
|` ` |TCP_DEFER_ACCEPT |bool |这是为防止空连接的攻击 |YES|
|` ` | | | | |
|` ` | | | | |
|` ` | | | | |


#### [4.2 获得Socket的类型](#)
获得Socket的类型,值为SOCK_DGRAM、SOCK_STREAM......

```cpp
unsigned int sock_type; //结果保存在此地！

socklen_t optLen = sizeof(sock_type);

auto isSuccess = getsockopt(udp_socket_fd, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optLen);

if(isSuccess != -1){
    printf("SOCK_DGRAM: %d - %d\n", sock_type, SOCK_DGRAM );
    //SOCK_DGRAM: 2 - 2
}
```

#### [4.3 防止空连接攻击](#)
使用TCP_DEFER_ACCEPT可以减少用户程序hold的连接数，也可以减少用户调用epoll_ctl和epoll_wait的次数，从而提高了程序的性能。
设置listen套接字的TCP_DEFER_ACCEPT选项后， 只当一个链接有数据时是才会从accpet中返回（而不是三次握手完成)。**这是为防止空连接的攻击**！

```cpp
#include <netinet/tcp.h>
int timeout=1;
setsockopt( listen_fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, sizeof(int) )
```
* timeout : 当服务端一直没接受到数据后,会重发SYN/ACK给客户端，当超过(net.ipv4.tcp_synack_retries = 5)次后， 就开始timeout的计时.
超时时间过之后，客户端还没有数据发送过来，服务端就会丢掉这个请求
* timeout = 0表示取消 TCP_DEFER_ACCEPT选项

性能四杀手：内存拷贝，内存分配，进程切换，系统调用。TCP_DEFER_ACCEPT 对性能的贡献，就在于 减少系统调用了。

#### [4.4 地址重用 - 01](#)
设置地址复用，一般情况下服务端主动关闭后，由于有TIME_WAIT状态需要等待一会儿后，才能重启服务器，不然会报错
端口已经被占有，通过设置地址复用，可以直接复用还没有完全关闭的Socket，这样就可以实现瞬间重启了，而不需要等待连接的彻底消亡！

如果在已经处于 ESTABLISHED状态下的socket(一般由端口号和标志符区分）调用close(socket)
（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket：
```cpp
int opt = 1;//这个值不能乱填的
auto state = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
//执行地址复用
if (state == -1) throw std::runtime_error("地址复用失败！");
Bind(fd, (const struct sockaddr*)&server_address, sizeof(server_address));
```
**注意：必须在调用bind函数之前设置SO_REUSEADDR选项。**

#### [4.4 地址重用 - 02](#)
如果要已经处于连接状态的soket在调用close(socket)后强制关闭，不经历TIME_WAIT的过程：

```cpp
int opt = 0;
setsockopt(fd,SOL_SOCKET ,SO_REUSEADDR,(const char*)& opt,sizeof(opt));
//执行地址复用
```

#### [4.5 超时设置](#)
在send(),recv()过程中有时由于网络状况等原因，发收不能预期进行,而设置收发时限：

```cpp
int nNetTimeout=1000; // 1秒
// 发送时限
setsockopt(socket, SOL_S0CKET, SO_SNDTIMEO, (char *)&nNetTimeout,sizeof(int));
// 接收时限
setsockopt(socket, SOL_S0CKET, SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));
```

#### [4.6 存活坚持](#)
设置存活检测!
```cpp
int opt = 1;
if (setsockopt (m_nSock, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int)) == -1)
{
    return 0;
}
```

#### [4.7 缓冲区大小](#)
在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节(异步)；系统默认的状态发送和接收一次为8688字节(约为8.5K)。
在实际的过程中如果发送或是接收的数据量比较大，可以设置socket缓冲区，避免send(),recv()不断的循环收发：

```cpp
// 接收缓冲区
int nRecvBufLen = 32 * 1024; //设置为32K
setsockopt( s, SOL_SOCKET, SO_RCVBUF, ( const char* )&nRecvBufLen, sizeof( int ) );
//发送缓冲区
int nSendBufLen = 32*1024; //设置为32K
setsockopt( s, SOL_SOCKET, SO_SNDBUF, ( const char* )&nSendBufLen, sizeof( int ) );
```

#### [4.8 不经过发送缓冲区拷贝](#)
在发送数据的时，不执行由系统缓冲区到socket缓冲区的拷贝，以提高程序的性能。
```cpp
int nZero = 0;
setsockopt( socket, SOL_SOCKET, SO_SNDBUF, ( char * )&nZero, sizeof( nZero ) );
```

#### [4.8 不经过接受缓冲区拷贝](#)
在接收数据时，不执行将socket缓冲区的内容拷贝到系统缓冲区：

```cpp
int nZero = 0;
setsockopt( s, SOL_SOCKET, SO_RCVBUF, ( char * )&nZero, sizeof( int ) );
```

#### [4.9 UDP广播](#)
一般在发送UDP数据报的时候，希望该socket发送的数据具有广播特性：
```cpp
BOOL bBroadcast = TRUE;
setsockopt( s, SOL_SOCKET, SO_BROADCAST, ( const char* )&bBroadcast, sizeof( BOOL ) );
```

#### [4.10 accept设置](#)
在client连接服务器过程中，如果处于非阻塞模式下的socket在connect()的过程中可以设置connect()延时,直到accpet()被调用(此设置只
有在非阻塞的过程中有显著的作用，在阻塞的函数调用中作用不大)。

```cpp
BOOL bConditionalAccept = TRUE;
setsockopt( s, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, ( const char* )&bConditionalAccept, sizeof( BOOL ) );
```

#### [4.11 关闭前先发完](#)
某些具体程序要求待未发送完的数据发送出去后再关闭socket，可通过设置让程序满足要求。

```cpp
struct linger {
    u_short l_onoff;
    u_short l_linger;
};
struct linger m_sLinger;

m_sLinger.l_onoff = 1; //在调用close(socket)时还有数据未发送完，允许等待
m_sLinger.l_linger = 5; //设置等待时间为5秒

setsockopt( s, SOL_SOCKET, SO_LINGER, (const char*)&m_sLinger, sizeof(struct linger));
```

#### [4.12 接收低潮限度、发送低潮限度](#)
每个套接口都有一个接收低潮限度和一个发送低潮限度。
* 接收低潮限度：对于TCP套接口而言，接收缓冲区中的数据必须达到规定数量，内核才通知进程“可读”。比如触发select或者epoll，返回“套接口可读”。
* 发送低潮限度：对于TCP套接口而言，和接收低潮限度一个道理。
```cpp
int sndlowat = 2048 ; //2k 字节
setsockopt(c->fd, SOL_SOCKET, SO_SNDLOWAT, (const void *) &sndlowat, sizeof(int));
```
理解接收低潮限度：如果应用程序没有调用recv()去读取socket的接受缓冲区的数据，则接受缓冲区数据将注一直保存在接受缓冲区中，所以随着接受
缓冲区接受到更多发送端发送缓冲区中的数据，则肯定会导致接受缓冲区溢出，所以设置一个接受低潮限度，当epoll监听到某一个socket的接受缓冲
区的数据超过了接受低潮限度，则触发读就绪，使得epoll循环返回，开始处理读I/O事件。

接收低潮限度：默认为1字节

理解发送低潮限度：如果应用程序没有调用send()来copy应用程序buff中的数据到socket发送缓冲区中，则随着发送缓冲区的数据被内核通过tcp协议
发送出去，最后socket发送缓冲区的数据越来越少，可用的剩余空间越来越多，最后超过发送缓冲区的发送低潮限度，则epoll监听到这个socket可写，使
得epoll循环返回，开始处理写I/O事件。

发送低潮限度：默认为2048字节

### [5 shutdown函数](#)
调用 close()/closesocket() 函数意味着完全断开连接，即不能发送数据也不能接收数据，这种“生硬”的方式有时候会显得不太“优雅”。

```cpp
int shutdown(int sock, int howto);  //Linux
int shutdown(SOCKET s, int howto);  //Windows
//成功时返回0，失败时返回-1。
```
howto 在 Linux 下有以下取值：
* SHUT_RD：断开输入流。套接字无法接收数据（即使输入缓冲区收到数据也被抹去），无法调用输入相关函数。
* SHUT_WR：断开输出流。套接字无法发送数据，但如果输出缓冲区中还有未传输的数据，则将传递到目标主机。
* SHUT_RDWR：同时断开 I/O 流。相当于分两次调用 shutdown()，其中一次以 SHUT_RD 为参数，另一次以 SHUT_WR 为参数。

howto 在 Windows 下有以下取值：
* SD_RECEIVE：关闭接收操作，也就是断开输入流。
* SD_SEND：关闭发送操作，也就是断开输出流。
* SD_BOTH：同时关闭接收和发送操作。

#### [5.1 close()/closesocket()和shutdown()的区别](#)
确切地说，close() / closesocket() 用来关闭套接字，将套接字描述符（或句柄）从内存清除，之后再也不能使用该套接字，与C语言中的 fclose() 类似。
应用程序关闭套接字后，与该套接字相关的连接和缓存也失去了意义，TCP协议会自动触发关闭连接的操作。

shutdown() 用来关闭连接，而不是套接字，不管调用多少次 shutdown()，套接字依然存在，直到调用 close() / closesocket() 将套接字从内存清除。

调用 close()/closesocket() 关闭套接字时，或调用 shutdown() 关闭输出流时，都会向对方发送 FIN 包。FIN 包表示数据传输完毕，计算机收到 FIN 包就知道不会再有数据传送过来了。

默认情况下，close()/closesocket() 会立即向网络中发送FIN包，不管输出缓冲区中是否还有数据，而shutdown() 会等输出缓冲区中的数据传输完毕再发送FIN包。
也就意味着，**调用 close()/closesocket() 将丢失输出缓冲区中的数据，而调用 shutdown() 不会**。

#### [5,2 close缺点](#)
会一次性将读写文件描述符都关闭。

close关闭连接，计数器-1，为0时释放底层为维护连接所构建的数据结构，该套接字被删除。

如果多个（多进程共用一个socket）文件描述符（socket）指向同一个连接时(dup函数赋值的文件描述符/或者子进程继承了父进程的文件描述符)，如果仅仅close其中一个文件描述符时，只
要其他的fd还是打开状态，那么连接就不会断开，直到所有的文件描述符都被close之后


