### [CMake 静态库和动态库](#)
**介绍**:  使用CMake为C++ 程序编译静态库和链接动态库。CMake有 add_library指令用来创建静态库或动态库的命令、link_directories指令设置库搜索路径、
target_link_libraries用于将可执行文件或库文件链接到库文件或可执行文件。

----

- [x] [1. 静态库和动态库](#1-静态库和动态库)
- [x] [2. 使用CMake创建库](#2-使用cmake创建库)
- [x] [3. CMake创建库指令](#3-cmake创建库指令)

-----

### [1. 静态库和动态库](#)
库是写好的现有的、成熟的、可以复用的代码。现实中每个程序都要依赖很多基础的底层库，不可能每个人的代码都从零开始，因此库的存在意义非同寻常。

本质上来说库是一种可执行代码的二进制形式，可以被操作系统载入内存执行。

库有两种 `(命名规则：linux、macos、windows)`：**静态库**`（libxxx.a、libxxx.a、xxx.lib）` 和 **动态库** `（libxxx.so、libxxx.dylib、xxx.lib/.dll）`，首选要区分动态链接和静态链接！

**静态链接**: 就是在装载之前，就完成所有的符号引用的一种链接方式，将别人的代码整合进自己的程序。
* **优点**：静态链接的优点在于程序**运行不会受库的影响**, **直接把库的源代码和自己的源代码合在一起成为一个可执行文件！**
* **缺点**
    * 静态链接实现的方法就是要**将程序需要的资源放到程序中**，同时程序内部解决一个需要调用库里面的代码的问题时，需要拷贝一份内部的解决方法到对应的位置，这就会不可避免的导致程序存储占用增加。
    * 并且程序在运行时需要加载到内存里，**程序体积大就会导致加载时间长**。
    * 程序的开发与发布流程受模块制约， **只要有一个依赖模块更新，那么就需要重新编译打包整个代码**。


**动态链接**： 基本思想就是将对符号的重定位推迟到程序运行时才进行。
* **优点**：动态链接是从库里面找方法，这就意味着程序本身并不包含库，**也就说明动态链接的程序存储占用会比较小**。**在加载到内存时体积小的程序也会比较快**。
* **缺点**：动态链接需要到库里面去寻找方法，这就导致如果库因为某些原因无法使用，比如上文中说的食堂需要装修就好比是库需要更新换代了，此时程序就无法运行了。


#### [1.1 Windows 动态库](#)
Windows系统中其静态库文件后缀是 `.lib`,  **在Windows系统中在动态库构建的时候，一般也会生成一个.lib文件**， 也就是动态库有 `xxx.lib` 和 `xxx.dll` 两个文件。

在动态库进行静态加载时，会使用这个.lib文件，这样的文件并不是静态库，而是成为导入库（Import Library），它包含了一段桩代码，用于获取导入符号的地址；
另外，导入库会描述dll中导出的符，用于链接时的符号决议。所以，要搞清楚.lib文件不一定等于静态库。 **这里的符号 (Symbol)是指一个函数，或者变量的起始地址。**

* Windows动态库的 **.lib**：**函数地址索引**
* Windows动态库的 **.dll**： **函数二进制代码**

#### [1.2 MacOS 动态库](#)
静态库和linux系统一样 `libxxx.a`， 但是动态库后缀有些区别一般命名为： `libxxx.dylib` 。

#### [1.3 linux ldd命令](#)
如果需要看一个可执行文件所依赖的动态库，并显示所依赖动态库的查找路径。
```shell
Usage: ldd [OPTION]... FILE...
      --help              print this help and exit
      --version           print version information and exit
  -d, --data-relocs       process data relocations
  -r, --function-relocs   process data and function relocations
  -u, --unused            print unused direct dependencies
  -v, --verbose           print all information
```

使用例子：
```shell
fan@carey:/$ ldd /bin/cat
linux-vdso.so.1 (0x00007fff5cefb000)
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f5412e04000)
/lib64/ld-linux-x86-64.so.2 (0x00007f5412fff000)
```

### [2. 使用CMake创建库](#)
方法非常肩带,测试项目目录：
```shell
├─.idea
├─build
├─cmake
├─include
│  └─mathTool
│    └─ cal.h  # 有一个 cal 类
│─src
│  └─mathTool
│    └─ cal.cpp # 对cal 类的定义
├─muse.cpp  #引用了cal类
├─CMakeLists.txt
```

include/mathTool/cal.h
```cpp
#ifndef MUSE_CAL_H
#define MUSE_CAL_H
class  cal{
public:
    cal(const int& , const int& );
    int addResult();
    int multiResult();
    int reduceResult();
    double divisionResult();
private:
    int a;
    int b;
};
#endif //MUSE_CAL_H
```

src/mathTool/cal.h
```cpp
#include "mathTool/cal.h"

cal::cal(const int & _a, const int & _b):a(_a),b(_b) {}

int cal::addResult() {
    return  a + b;
}

int cal::multiResult() {
    return a * b;
}

int cal::reduceResult() {
    return a - b;
}

double cal::divisionResult() {
    return (double)a / (double)b;
}
```

muse.cpp
```cpp
#include <iostream>
#include "mathTool/cal.h"

int main() {
    cal me(20, 30);
    std::cout << "result:" << me.multiResult() << std::endl;
    return 0;
}
```

#### [2.1 创建静态库](#)
使用命令add_library、target_link_libraries即可：

**CMakeLists.txt**
```cpp
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(muse
        VERSION         0.0.1
        DESCRIPTION     "cmake demo"
        HOMEPAGE_URL    "https://github.com/sorise"
        LANGUAGES       C CXX
)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)

#文件目录
include_directories(include)

#静态库
add_library(mathTool STATIC src/mathTool/cal.cpp)

add_executable(main main.cpp)

target_link_libraries(main mathTool)
```

命令：
```shell
cmake -S . -B build  #生成
cmake --build build  #构建
```


#### [2.2 创建动态库](#)
对于动态库的创建，在linux系统中只需要修改如下即可： `add_library(mathTool SHARED src/mathTool/cal.cpp)`，但是在Windows系统中却不能如此，
默认情况下无法生成一个lib文件，所以需要做一些处理。

**CMakeLists.txt**
```cpp
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(muse
        VERSION         0.0.1
        DESCRIPTION     "cmake demo"
        HOMEPAGE_URL    "https://github.com/sorise"
        LANGUAGES       C CXX
)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)

#文件目录
include_directories(include)

#动态库 改成 SHARED即可
add_library(mathTool SHARED src/mathTool/cal.cpp)

add_executable(main main.cpp)

target_link_libraries(main mathTool)
```

命令：
```shell
cmake -S . -B build  #生成
cmake --build build  #构建
```

#### [2.3 Windows 动态库](#)
在Windows系统中在动态库构建的时候，一般还会生成一个 **.lib 文件**存放**函数地址索引**，但是默认情况下却不能生成，需要我们做一些处理。

需要对源代码做一些修改, 首先我们需要如下的宏：
```cpp
__declspec( dllimport ) declarator
__declspec( dllexport ) declarator
```

如果你的系统是Windows，那么cmake 自动给库项目添加 **库名称_EXPORTS 预处理变量**, 当前项目名称为：muse, 所以是 muse_EXPORTS。
```cpp
#ifndef _WIN32
    #define XCPP_WIN_API
#else
    #ifdef muse_EXPORTS
        #define XCPP_WIN_API __declspec(dllexport)   //库项目调用
    #else
        #define XCPP_WIN_API __declspec(dllimport)  //调用库库项目调用
    #endif
#endif

class XCPP_WIN_API cal{
public:
    cal(const int& , const int& );
    int addResult();
    int multiResult();
    int reduceResult();
    double divisionResult();
private:
    int a;
    int b;
};
```

然后再运行：
```
cmake -S . -B build   #生成
#使用 vscode 打开 build 里面的项目即可！
```

### [3. CMake创建库指令](#)

#### [3.1 include_directories](#)
用于指定头文件搜索路径，优点是简单直接，缺点是无法进行权限控制，一旦被执行后，后续的所有代码都能搜索到对应的文件路径。

当前CMakeList.txt中的所有目标以及所有在其调用点之后添加的子目录中的所有目标将具有此头文件搜索路径。

```shell
include_directories ([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])
```

默认情况下， **include_directories** 命令会将目录添加到列表最后，可以通过命令设置 **CMAKE_INCLUDE_DIRECTORIES_BEFORE** 变量为ON来改变它默认行为，将目录添加到列表前面。
* 每次调用 **include_directories** 命令时使用 **AFTER** 或 **BEFORE**选项来指定是添加到列表的前面或者后面。
* 如果使用 **SYSTEM** 选项，会把指定目录当成系统的搜索目录。该命令作用范围只在当前的 **CMakeLists.txt**。

使用例子：
```shell
include_directories(sub) 
include_directories(sub2) #默认将sub2添加到列表最后
include_directories(BEFORE sub3) #可以临时改变行为，添加到列表最前面
# 将/usr/lib/mylibfolder 和 ./lib 添加到库文件搜索路径
```

#### [3.2 add_library](#)
用于添加库文件，可以创建静态库或动态库。第一个参数很重要，被称为target，可以作为target_xxx命令的接收对象。

```shell
add_library(libname [SHARED|STATIC|MODULE] [EXCLUDE_FROM_ALL] source1 source2 ... sourceN)
```
使用例子：

```shell
add_library(test_lib a.cpp b.cpp) #默认生成静态库
add_library(test_lib SHARED a.cpp b.cpp) #生成动态库
```

#### [3.3 target_link_libraries](#)
用于将可执行文件或库文件链接到库文件或可执行文件。身为target_xxx的一员，很明显第二个参数也可以进行权限控制。

语法：
```shell
target_link_libraries(<target>
                      <PRIVATE|PUBLIC|INTERFACE> <item>...
                     [<PRIVATE|PUBLIC|INTERFACE> <item>...]...)
```

使用例子：
```shell
add_executable(network main.cpp)

add_library(prints SHARED ./src/prints.cpp)
#什么都不写就是静态库
#中间写一个 SHARED 就是动态库

target_link_libraries(network prints)
#将动态库 prints 连接到 network
```

#### [3.4 link_directories](#)
向工程添加多个特定的库文件搜索路径。

将指定目录添加到编译器的头文件搜索路径之下，指定的目录被解释成当前源码路径的相对路径。


使用例子：
```shell
link_directories(/usr/lib/mylibfolder ./lib)
```
