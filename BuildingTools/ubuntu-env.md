### [Ubuntu 配置 C/C++ 开发环境](#)
 **介绍**: 在ubuntu系统中配置C++开发环境，gcc、g++、gbd、make、cmake、clion！

-----
- [x] [1. 安装和配置编译器](#1-安装和配置编译器)
- [x] [2. 安装 cmake](#2-安装-cmake)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)
-----

### [1. 安装和配置编译器](#)
安装C++的编译器还有项目管理器CMake，CMake是一个开源、跨平台的**编译**、**测试**和**打包**工具。

#### [1.1 更新和升级系统软件](#)
在终端中输入以下命令对软件进行刷新，保证后续安装的软件都是最新的：
```
sudo apt update
```
update 命令只会获得系统上所有包的最新信息，并不会下载或者安装任何一个包、更新包缓存（可以知道包的哪些版本可以被安装或升级）。

```
sudo apt upgrade
```

**apt upgrade** 命令来把这些包下载和升级到最新版本。

#### [1.2 安装 build-essential 软件包](#)
**build-essential** 包含了 GNU 编辑器集合、GNU 调试器、其他编译软件所必需的开发库和工具，简单来说，安装了 build-essential 就相当于安装了 gcc、g++、make 等工具。

```
sudo apt install build-essential
```
查看 gcc 版本： 
```
gcc --version
```
查看 g++ 版本：
```
g++ --version
```
查看 make 版本：
```
make --version
```

#### [1.3 安装 gdb](#)
GBD是UNIX及UNIX-like下的调试工具。
```
sudo apt install gdb
```
查看 gdb 版本：
```
gdb --version
```

### [2. 安装 cmake](#) 
CMake是一个开源、跨平台的编译、测试和打包工具，它使用比较简单的语言描述编译、安装的过程，输出 Makefile 或者 project 文件，再去执行构建。

```
sudo apt install cmake
```
查看 cmake 版本：
```
cmake --version
```


### [3.](#) 

### [4.](#) 

### [5.](#) 

-----
时间: [] 