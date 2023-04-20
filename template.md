### [标题](#)
 **介绍**：xxx

-----
- [x] [1. ](#1-)
- [x] [2. ](#2-)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)
-----

### [1.](#)

#### [1.1](#)

##### [1.1.1 ](#)

### [2.](#) 

|方法|返回值|参数个数|说明|
|:---|:---|:---|:----|
|||||
|||||
|||||

### [3.](#) 

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
|` `| | | | |
|IPPROTO_IP |IP_TOS | |服务类型（TOS）字段 |YES |
|` `| | | | |
|` ` | | | | |
|` ` | | | | |
|IPPROTO_TCP |TCP_NODELAY |bool |禁止发送合并的Nagle算法 |YES | 
|` ` | | | | |
|` ` | | | | |
|` ` | | | | |
|` ` | | | | |


### [5.](#) 

-----
时间: [] 