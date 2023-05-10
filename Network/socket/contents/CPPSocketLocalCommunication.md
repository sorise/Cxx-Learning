### [C++ 本地 Socket 实现进程通信](#)
 **介绍**： 套接字(Socket)也是一种常用进程间通信方式！

-----

- [x] [1. 本地套接字通信](#1-本地套接字通信)
- [x] [2. 服务端](#2-服务端)
- [x] [3. 客户端](#3-客户端)

-----

### [1. 本地套接字通信](#)
利用本地套接字，也可以进程间通信，本地套接字和命名管道一样都利用**伪文件**,当调用bind函数后，就会生成本地套接字对应的伪装文件

和网络套接字不同的地方是：
* 使用的结构体不一样，本地套接字使用 **sockaddr_un**
* socket domain类似为 AF_UNIX
* 客户都也必须调用bind函数,但是不能调用 listen！

```cpp
#include <sys/un.h>
struct sockaddr_un{
    sun_family;//AF_LOCAL
    sun_path;//本地套接字对应的伪装文件的名字，可以加路径 108字节
}
```

#### [1.1 创建本地套接字](#)
基本流程如下：

```cpp
const char* fileName = "server.socket";
auto localFd = wrap::Socket(AF_UNIX, SOCK_STREAM, 0);

sockaddr_un serv{ AF_LOCAL };
strcpy(serv.sun_path, fileName);
unlink(fileName); //删除一下过去的文件

//最好使用这个
auto len = offsetof(struct sockaddr_un,sun_path) + strlen(serv.sun_path);

wrap::Bind(localFd,(struct sockaddr*)&serv,len );

listen(localFd, 16);
```


### [2. 服务端](#) 

```cpp
const char* fileName = "/tmp/server.socket";
auto localFd = wrap::Socket(AF_UNIX, SOCK_STREAM, 0);

sockaddr_un serv{ AF_LOCAL };
strcpy(serv.sun_path, fileName);
unlink(fileName); //删除一下过去关联的文件

auto len = offsetof(struct sockaddr_un,sun_path) + strlen(serv.sun_path);

wrap::Bind(localFd,(struct sockaddr*)&serv,len);
listen(localFd, 16);
std::cout << "the local socket server is running:" << std::endl;
while (true){
    sockaddr_un clientAddr{  };
    socklen_t clen = sizeof(clientAddr);
    auto clientFd = accept(localFd ,(struct sockaddr*)&clientAddr, &clen);

    len = len - offsetof(struct sockaddr_un,sun_path);
    clientAddr.sun_path[len] = '\0';

    std::cout << "client socket fileName: " << clientAddr.sun_path << std::endl;

    char buf[2048];
    size_t readCount = recv(clientFd, buf, sizeof(buf), 0);
    while (readCount > 0){
        for (int j = 0; j < readCount; ++j) {
            if (isalpha(buf[j])){
                buf[j] = toupper(buf[j]);
            }
        }
        send(clientFd,  buf, readCount, 0);
        readCount = recv(clientFd, buf, sizeof(buf), 0);
    };
    wrap::Close(clientFd);
}
wrap::Close(localFd);
```

### [3. 客户端](#) 

```cpp
const char* fileName = "/tmp/client.socket";
unlink(fileName); //删除一下过去关联的文件

auto clientFd = wrap::Socket(AF_UNIX, SOCK_STREAM, 0);

sockaddr_un serv{ AF_LOCAL };
strcpy(serv.sun_path, fileName);

auto len = offsetof(struct sockaddr_un,sun_path) + strlen(serv.sun_path);

wrap::Bind(clientFd,(struct sockaddr*)&serv,len);

std::cout << "the local socket client is running:" << std::endl;

struct sockaddr_un toServer{};
toServer.sun_family = AF_LOCAL;
strcpy(toServer.sun_path, "/tmp/server.socket");

auto len_ = offsetof(struct sockaddr_un,sun_path) + strlen(toServer.sun_path);

int ret = connect(clientFd, (struct sockaddr*)&toServer, len_);

if (ret >= 0){
    char buf[2048] = {0};
    while(fgets(buf,sizeof(buf), stdin) != nullptr){
        int result = send(clientFd, buf, sizeof(buf), 0);
        if(result == -1){
            perror("send error");
            exit(1);
        }
        auto readCount = read(clientFd, buf, sizeof(buf));
        buf[readCount] = '\0';
        std::cout<< buf;
    }
}else{
    std::cout << "errno: " << errno << std::endl;
}


wrap::Close(clientFd);
```

-----
 