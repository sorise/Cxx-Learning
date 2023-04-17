### [C++ Socket 继续学习](#)
 **介绍**：xxx

-----
- [x] [1. 封装错误处理](#1-封装错误处理)
- [x] [2. ](#2-)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)
-----

### [1. 封装错误处理](#)
在socket建立过程中，各个API都有出错的可能，错误处理就显得很重要，但是每个地方都写if判断就很麻烦，所以可以进行简单的封装，
避免每个地方都进行麻烦的错误处理！

#### [1.1 简单的函数封装](#)
如下所示，只是简单的错误判断！

```cpp
int Socket(int domain,int type,int protocol){
    auto socket_fd = socket(AF_INET, SOCK_STREAM, 0);
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
```

#### [1.2 面向对象式的封装](#)
基本思想很简单，只是麻烦些，需要良好的面向对象设计能力！

```cpp
#ifndef _SOCKET_H_
#define _SOCKET_H_
/*
基础socket类，封装c socket接口
*/
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
 
#include <assert.h>
#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
//由于bzero在windows系统下没有 这里定义一个bzero
#define bzero(point, size) memset(point, 0, size)
#else
#include <string.h>
//统一socket句柄的类型名
typedef int SOCKET;
#define INVALID_HANDLE_VALUE (-1)
#endif
 
class Socket
{
public:
	Socket();
	Socket(SOCKET sock, const char* ip, int port);
	~Socket();
 
	//Windows系统中使用socket函数需要先调用WSAStartup需要初始化版本
	static bool InitSocket();
	//Windows系统中使用socket函数完毕后需要调用WSACleanup释放
	static void UnInitSocket();
 
	//设置可重用地址
	void SetReuseAddr();
	//设置非阻塞
	void SetSockNonBlock();
	//设置接收缓冲区大小
	void SetRecvBuffSize(int size);
	//设置发送缓冲区大小
	void SetSendBuffSize(int size);
	//地址绑定
	bool Bind(const char* ip, int port);
	//启动监听
	bool Listen(int backlog);
 
	//接收连接,返回一个已分配内存上的Socket指针, 需要手动管理释放,失败返回 nullptr
	Socket* Accept();
	//接收连接, 返回连接的socket
	SOCKET Accept(struct sockaddr_in * addr, int * addrlen);
 
	//发起连接
	int Connect(const char* ip, int port);
	//发起连接
	int Connect(SOCKET sock, const char* ip, int port);
	//发送数据
	int Send(const char* buf, int size);
	//接收数据
	int Recv(char* buf, int size);
	//关闭套接字
	int Close();
 
public:
	SOCKET m_Sock;
	char m_Ip[16];
	short m_Port;
};
```

### [2.](#) 

|方法|返回值|参数个数|说明|
|:---|:---|:---|:----|
|||||
|||||
|||||

### [3.](#) 

### [4.](#) 

### [5.](#) 

-----
时间: [] 