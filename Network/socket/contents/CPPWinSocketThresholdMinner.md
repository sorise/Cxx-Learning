### [C++ Socket Windows 编程入门](#)
**介绍**： Windows Socket 套接字编程相关知识， [使用 Windows 套接字 2](https://learn.microsoft.com/zh-cn/windows/win32/api/_winsock/) ， win socket需要一些额外的操作，打开网络库/启动网络库，启动了这个库，这个库里的函数/功能才能使用。。

-----

- [x] [1. 启动网络库 winsock2](#1-启动网络库-winsock2)
- [x] [2. ](#2-)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)


----
### [1. 启动网络库 winsock2](#)
WSAStartup，即WSA(Windows Sockets Asynchronous，Windows异步套接字)的启动命令, WSAStartup必须是应用程序或DLL调用的第一个Windows Sockets函数。

```cpp
int WSAAPI WSAStartup(
  WORD      wVersionRequested,
  LPWSADATA lpWSAData
);
```
**返回值**： 如果成功， WSAStartup 函数将返回零。 否则，它将返回下面列出的错误代码之一。

**参数化**：
* wVersionRequested：(双字节) 调用方可以使用的最高版本的 Windows 套接字规范。 高顺序字节指定次要版本号;低顺序字节指定主版本号。
    * Windows 套接字规范的当前版本为版本 `2.2`， 还有 `2.1`,` 2.0`,`1.1` ...。
* lpWSAData： 指向 WSADATA 数据结构的指针，该结构用于接收 Windows 套接字实现的详细信息。


使用例子：
```cpp
#ifdef WIN32
    #include <winsock2.h>
    #include <Windows.h>
    // Need to link with Ws2_32.lib
    #pragma comment(lib, "ws2_32.lib")
#endif

#include <iostream>
#include <functional>

using namespace std;

int main() {
#ifdef WIN32
    //初始化socket Dll 库
    WSADATA wsa;
    auto result = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (result!=0)
    {
        std::cerr << "WSAStartup error code:" << result << std::endl;
        return 1;
    }else{
        std::cout << "WSAStartup start successfully" << std::endl;
    }
    //版本检测是否加载正确
    if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2) {
        printf("未发现版本正确的 Winsock.dll\n");
        WSACleanup();
        return 1;
    }
    else{
        printf("The Winsock 2.2 dll was found okay\n");
		
    }
    WSACleanup(); //结束DLL
#endif
    return 0;
}

```

#### 1.2 CMake设置

我们还需引入响应的库文件。

```cmake
cmake_minimum_required(VERSION 3.25)

project(maker)

#解决 visual studio 编码问题
if (WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

set(CMAKE_CXX_STANDARD 20)

add_executable(use main.cpp)

#引入socket库
target_link_libraries(use ws2_32 wsock32)
```

