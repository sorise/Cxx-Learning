socket编程中需要用到的头文件
sys/types.h：数据类型定义

sys/socket.h：提供socket函数及数据结构

netinet/in.h：定义数据结构sockaddr_in

arpa/inet.h：提供IP地址转换函数

netdb.h：提供设置及获取域名的函数

sys/ioctl.h：提供对I/O控制的函数

sys/poll.h：提供socket等待测试机制的函数

其他在网络程序中常见的头文件
unistd.h：提供通用的文件、目录、程序及进程操作的函数

errno.h：提供错误号errno的定义，用于错误处理

fcntl.h：提供对文件控制的函数

time.h：提供有关时间的函数

crypt.h：提供使用DES加密算法的加密函数

pwd.h：提供对/etc/passwd文件访问的函数

shadow.h：提供对/etc/shadow文件访问的函数

pthread.h：提供多线程操作的函数

signal.h：提供对信号操作的函数

sys/wait.h、sys/ipc.h、sys/shm.h：提供进程等待、进程间通讯（IPC）及共享内存的函数

建议：在编写网络程序时，可以直接使用下面这段头文件代码
```cpp
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <fcntl.h>
```
涉及到用户权限及密码验证问题时加入如下语句：
```cpp
#include <shadow.h>
#include <crypt.h>
#include <pwd.h>
```
需要注意的是，应该在编译时链接加密算法库，即增加编译选项： `-lcrypt`


涉及到文件及时间操作加入如下语句：
```cpp
#include <sys/time.h>
#include <utime.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/file.h>
```
涉及到多进程操作时加入如下语句：
```cpp
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
```
涉及到多线程操作时加入如下语句：
```cpp
#include <pthread.h>
#include <sys/poll.h>
```

1.Linux头文件主要包含
　　#include <sys/socket.h>
　　#include <netinet/in.h>
　　#include <netdb.h>
　　#include <arpa/inet.h>
　　
　　而windows下则是包含
　　#include <Winsock2.h>
　　#pragma comment(lib,“Ws2_32.lib”)

2.初始化
windows下需要用WSAStartup();linux下不需要
对应的退出清理用WSACleanup( );linux下不需要

3.关闭socket
windows下closesocket(…)
linux下close(…)

4.socket套接字类型
Linux中socket为整形，
　Windows中为一个SOCKET类型。
　因为linux中的socket与普通的fd一样，所以可以在TCP的socket中，发送与接收数据时，直接使用read和write。而windows只能使用recv和send。

5.获取错误码
windows下WSAGetLastError() ；
linux下设置errno变量 用geterror()取得错误码；

6.设置socket属性
Linux下为 fcntl (fd, F_SETFL, flag | O_NONBLOCK);
Windows下为 ioctlsocket (fd, FIONBIO, (unsigned long *) &flag);

7.毫秒级时间获取
windows下 GetTickCount()
linux下 gettimeofday()

8.WSA宏
windows下WSA相关的操作
linux下WSA相关的操作在Linux不必要，直接去掉即可

9.多线程
多线程: (win)process.h --〉(linux)pthread.h
_beginthread --> pthread_create
_endthread --> pthread_exit
————————————————
版权声明：本文为CSDN博主「Fengfgg」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/Fengfgg/article/details/112229986