### [安装 libevent 相关依赖包](#)
 **介绍**：libevent还依赖一些包来组成完整应用，包括zlib、openssl， 前者用于解压缩，后者用于加解密传输。

-----
- [x] [1. ](#1-)
- [x] [2. 安装 openSSL](#2-安装-openssl)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)
-----

### [1.](#)

#### [1.1](#)

##### [1.1.1 ](#)

### [2. Windows 安装 openSSL](#) 
[参考文档](https://github.com/openssl/openssl/blob/master/NOTES-WINDOWS.md), **安装** openSSL 之前需要先，安装perl和 Netwide Assembler (NASM)，前者用于编译 `Configure` ，后者用于perl汇编调用。
使用vs2022的命令工具行 `x64_x86 Cross Tools Command Prompt for VS 2022` 使用nmake编译

```cpp
perl Configure {VC-WIN32|VC-WIN64A|VC-WIN64I|VC-CE} --prefix=%OUTPUT%
```

### [3.](#) 

### [4. 套接字设置](#)

#### [4.1 ](#)


### [5.](#) 

-----
时间: [] 