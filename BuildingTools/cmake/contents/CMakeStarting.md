### [CMake 简单使用](#)  
**介绍**: CMake是一个跨平台的高级编译配置工具，可以用简单的语句来描述所有平台的安装(编译过程)。 
能够输出各种各样的makefile或者project文件, Cmake 并不直接建构出最终的软件，而是产生其他工具的脚本（如Makefile ），然后再依这个工具的构建方式使用。

----

- [1. CMake 简介](#1-cmake-简介)
- [2. CMake 基本语法规则](#2-cmake-基本语法规则)
- [3. CMake 关键字](#3-cmake-关键字)
- [4. CMake 构建](#4-cmake-构建)

----

### [1. Cmake 简介](#)
**CMake 就是一个用于构建、测试和软件打包的开源跨平台工具！**学习CMake的**目的**: 为将来处理大型的C/C++/JAVA项目做准备。

以下方法难以保证 安装 cmake 最新版本。
```shell
yum install cmake #redhat centos 系linux 安装

sudo apt install cmake #ubuntu 系 安装 cmake
```

linux 系统查看是否安装了cmake。
```shell
cmake --version 
```

**G++和CMake的区别**： GCC（GNU Compiler Collection）`/`g++ 将源文件编译（Compile）成可执行文件或者库文件； 
g++是编译器，CMake是指挥编译器如何编译的！

#### [1.1 小例子](#)
我们先用一下cmake体验一下！ 先不管具体语法配置说明！

```shell
$ mkdir enviroment  #新建一个文件夹
$ cd ./enviroment
```

然后添加一个新文件 main.cpp
```cpp
#include<iostream>
#include<list>
#include<algorithm>

int main(int argc, char *argv[]){
    std::list<double> scores = {98.75, 75.25, 95.26, 84.5,98.56,95.62};

    double sum = 0;
    std::for_each(scores.begin(), scores.end(), [&sum](const double &v){
        sum += v;
    });

    std::cout << "average score: " << sum / scores.size() << std::endl;

    std::cout << "sum:"<< sum << std::endl;
    return 0;
}
```
2、步骤二，添加 CMakeLists.txt文件
```txt
cmake_minimum_required(VERSION 3.22.1)

project(TSM)

SET(SRC_LIST main.cpp)

MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})

MESSAGE(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})

ADD_EXECUTABLE(tsm ${SRC_LIST})
```

这里隐式定义了两个CMAKE的变量
* `<project_name>_BINARY_DIR`，本例中是 **HELLO_BINARY_DIR**
* `<project_name>_SOURCE_DIR`，本例中是 **HELLO_SOURCE_DIR**

MESSAGE关键字就可以直接使用者两个变量，当前都指向当前的工作目录，后面会讲外部编译问题：如果改了工程名，这两个变量名也会改变

**解决**：**CMake** 定义两个预定义变量： **PROJECT_BINARY_DIR** 和 **PROJECT_SOURCE_DIR** ，这两个变量和 **HELLO_BINARY_DIR**，**HELLO_SOURCE_DIR**是一致的。
即使改了工程名也没有关系。

解决方案！按下不表
```cmake
MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
```

注意：工程名的 HELLO 和生成的可执行文件 hello 是没有任何关系的

**3. 步骤三** 使用cmake，生成makefile文件, 运行指令 **`.` 表示当前目录**。
```shell
cmake .
```
目录下就生成了这些文件： CMakeFiles, CMakeCache.txt, cmake_install.cmake 等文件，并且生成了Makefile。 现在不需要理会这些文
件的作用，以后你也可以不去理会。最关键的是，它自动生成了Makefile。

**4. 步骤四**：使用make命令编译！ 最终生成了Hello的可执行程序！ 然后执行即可！
```shell
remix@remix-A34S:~/code/vscode/enviroment$ make
[ 50%] Building CXX object CMakeFiles/hello.dir/main.cpp.o
[100%] Linking CXX executable hello
[100%] Built target hello
remix@remix-A34S:~/code/vscode/enviroment$ ./hello 
hello! cmake!
```

### [2. 基本语法规则](#)
**指令是大小写无关的**， **参数和变量是大小写相关的**, 但推荐你全部使用大写指令。

下面是一个意思
```
project(TSM)

PROJECT(TSM)
```
变量使用 **${}** 方式取值，但是在 **IF** 控制语句中是直接使用变量名。 
```
IF(DEBUG){

}
```

指令(参数 1 参数 2...) 参数使用括弧括起，参数之间使用**空格**或**分号**分开。 以上面的 **ADD_EXECUTABLE** 指令为例，如果存在另外一个func.cpp源文件 就要写成如下两种形式  

```shell
ADD_EXECUTABLE(tsm main.cpp func.cpp)
```

```shell
ADD_EXECUTABLE(tsm main.cpp;func.cpp)
```

#### [2.1 注意事项](#)
[注意事项](#): **SET(SRC_LIST main.cpp)**  可以写成 **SET(SRC_LIST “main.cpp”)**，如果源文件名中含有空格，就必须要加双引号。

[注意事项](#): **ADD_EXECUTABLE(tsm main)**  后缀可以不写，因为cmake会自动去找.c和.cpp，但是不推荐这样写，因为可能会有这两个文件: main.cpp 和 main。


### [3. CMake 关键字](#)


#### [3.1 PROJECT](#)
可以用来指定工程的名字和支持的语言，默认支持所有语言。

* PROJECT (HELLO) 指定了工程的名字，并且支持所有语言—建议
* PROJECT (HELLO CXX) 指定了工程的名字，并且支持语言是C++
* PROJECT (HELLO C CXX) 指定了工程的名字，并且支持语言是C和C++
* PROJECT (HELLO C CXX JAVA) 指定了工程的名字，并且支持语言是C和C++还有java
* PROJECT (HELLO VERSION 1.0.0 LANGUAGES C CXX)  制定项目名 版本号 语言列表

#### [3.2 SET](#)
用来显示的指定变量的。

* SET(SRC_LIST main.cpp) SRC_LIST变量就包含了main.cpp
* SET(SRC_LIST main.cpp a.cpp b.cpp); 


#### [3.3 MESSAGE](3)
向终端输出用户定义的信息或变量的值，MESSAGE([SEND_ERROR | STATUS | FATAL_ERROR] “message to display” …)
* SEND_ERROR 产生错误,生成过程被跳过。
* STATUS 输出前缀为—的信息。
* FATAL_ERROR 立即终止所有cmake过程。
 
```shell
MESSAGE(STATUS "binary directory is:" ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "source directory is:" ${PROJECT_SOURCE_DIR})
```
输出信息为：
```shell
-- binary directory is: /home/remix/code/vscode/enviroment
-- source directory is: /home/remix/code/vscode/enviroment
```

#### [3.4 ADD_EXECUTABLE](#)
ADD_EXECUTABLE(可执行文件名  生成该可执行文件的源文件)

* **ADD_EXECUTABLE(hello ${SRC_LIST})**
* **ADD_EXECUTABLE(hello main.cpp)**

说明SRC_LIST变量中的源文件需要编译出名为hello的可执行文件
#### [3.5 ADD_SUBDIRECTORY](#)
向当前工程添加存放源文件的子目录，并可以指定中间二进制和目标二进制的存放位置

```shell
ADD_SUBDIRECTORY(src_dir [binary_dir] [EXCLUDE_FROM_ALL])
```

EXCLUDE_FROM_ALL 含义： 将这个目录从编译过程中排除

```shell
SET(EXECUTABLE_OUTPUT_PATH${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH${PROJECT_BINARY_DIR}/lib)更改生成的库文件路径
```
```shell
ADD_SUBDIRECTORY(src bin)
```
将 src 子目录加入工程并指定编译输出(包含编译中间结果)路径为bin 目录! 如果不进行 bin 目录的指定，那么编译结果(包括中间结果)都将存放在build/src 目录!

#### [3.6 cmake_minimum_required](#)
指定cmake的最小版本！
```
cmake_minimum_required(VERSION 3.22.0)
```

#### [3.7 include_directories](#)
向工程添加多个特定的头文件搜索路径 --->相当于指定g++编译器的-I参数
**语法**: 
```
include_directories([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)
```
```shell
# 将/usr/include/myincludefolder 和 ./include 添加到头文件搜索路径
include_directories(/usr/include/myincludefolder ./include)
```

### [4. CMake 构建](#)
CMake构建有两种基本构建模式，外部构建是最常用的构建方式！
* **内部构建**: 也就是源代码在CMakeLists.txt同一级目录进行编译，上述例子就是内部构建，他生产的临时文件特别多，不方便清理。
* **外部构建**: 就会把生成的临时文件放在build目录下，不会对源文件有任何影响强烈使用外部构建方式。

内部构建文件列表：
```shell
-rw-rw-r-- 1 remix remix 13783  8月 30 10:28 CMakeCache.txt
drwxrwxr-x 5 remix remix  4096  8月 31 15:55 CMakeFiles
-rw-rw-r-- 1 remix remix  1638  8月 30 10:28 cmake_install.cmake
-rw-rw-r-- 1 remix remix   241  8月 31 15:55 CMakeLists.txt
-rwxrwxr-x 1 remix remix 16520  8月 30 10:57 hello
-rw-rw-r-- 1 remix remix   120  8月 30 10:57 main.cpp
-rw-rw-r-- 1 remix remix  5114  8月 31 15:55 Makefile
```
我们推荐使用外部构建的方式！
#### [4.1 外部构建](#)
在项目目录下新建一个 build目录，用来存放临时文件！

文件目录：
```shell
drwxrwxr-x 2 remix remix 4096  8月 31 16:29 build
-rw-rw-r-- 1 remix remix  231  8月 31 16:29 CMakeLists.txt
-rw-rw-r-- 1 remix remix  120  8月 30 10:57 main.cpp
```
进入build文件目录运行。
```shell
cmake ..  #..表示上级目录
``` 
这样产生的临时文件都存放到了 build文件夹中！

在build目录中 运行make来构建工程 可执行文件会生成到 build文件夹中！
```shell
remix@remix-A34S:~/code/vscode/enviroment/build$ make
CMake Warning (dev) at CMakeLists.txt:9:
  Syntax Warning in cmake code at column 37

  Argument not separated from preceding token by whitespace.
This warning is for project developers.  Use -Wno-dev to suppress it.

-- This is BINARY dir /home/remix/code/vscode/enviroment/build
-- This is SOURCE dir /home/remix/code/vscode/enviroment
-- Configuring done
-- Generating done
-- Build files have been written to: /home/remix/code/vscode/enviroment/build
[ 50%] Building CXX object CMakeFiles/hello.dir/main.cpp.o
[100%] Linking CXX executable hello
[100%] Built target hello
```
一条命令搞定，这样就不需要跳转目录了！ 使用cmake进行构建，生成文件放在build目录下！
```
cmake -S . -B build
```
编译, 编译代码目录为build。如果在build目录可以将后一个build改为 `.`
```
cmake --build build
```


#### [4.2 进一步工程化](#)
首先就像前端项目一样，我们需要一个 **src目录用来存放源文件**  添加一个子目录 **documents**，用来放置这个工程的文档 

* 在工程目录添加文本文件,版权文件： COPYRIGHT 项目说明 README.MD
* 在工程目录添加一个run.sh脚本，用来调用二进制文件
* 将构建后的目标文件放入构建目录的 bin 子目录中

#### [4.3 工程源代码目录](#)
文件目录: 首先清楚build文件夹里面的内容， **每个目录下都要有一个 CMakeLists.txt 说明**。
```
├── build
├── documents
├── README.MD
├── CMakeLists.txt
└── src
    ├── CMakeLists.txt
    └── main.cpp
```
外部 CMakeLists.txt的写法
```shell
cmake_minimum_required(VERSION 3.22.1)

PROJECT(TSM)

add_subdirectory(src bin)
```

src 中的 CMakeLists.txt 写法
```shell
set(ENTRY main.cpp)

ADD_EXECUTABLE(tsm ${ENTRY})
```

终端 进入 build 文件夹中 运行命令: 
```shell
cmake ..
```
build 目录里面会生成一个 bin目录， 里面会生成一堆文件

然后在build 文件夹中 执行 命令 make。
```shell
make
```
将会在 tsm 中生成可执行文件！

#### [4.4 更改二进制的保存路径](#)
SET指令重新定义，EXECUTABLE_OUTPUT_PATH 和 LIBRARY_OUTPUT_PATH 变量 来指定最终的目标二进制的位置。

```shell
SET(EXECUTABLE_OUTPUT_PATH PROJECTBINARYDIR/bin)
SET(LIBRARYOUTPUTPATH{PROJECT_BINARY_DIR}/lib)
```
哪里要改变目标存放路径，就在哪里加入上述的定义，应该在src下的CMakeLists.txt下写

----
[2022/8/17 第一次修订]