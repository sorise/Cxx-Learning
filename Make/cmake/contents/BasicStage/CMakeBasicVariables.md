## [CMake 变量](#)
**介绍**: CMake支持用户自定义变量、环境变量、同时自己也内置了许多变量!

----

- [1. 变量](#1-变量)
- [2. 变量之缓存变量](#2-变量之缓存变量)
- [3. 变量之作用域](#3-变量之作用域)
- [4. 变量注意](#4-变量注意)
- [5. 变量传递给C++](#5-变量传递给c++)
- [6. 数学运算](#6-数学运算)

----
### [1. 变量](#)
和任何其他编程语言一样，CMake 完全可以看做是一种编程语言，他有变量，有函数等。同时几乎每一个编译工具，都支持设置变量，来传递编译信息，控制编译流。CMake变量也基本分自定义变量、内置变量、环境变量。

**CMake变量名称区分大小写**，几乎可以包含任何字符。

```cmake
set(varName value... [PARENT_SCOPE])  #CMake 定义变量的命令如下：
```
在学习这个命令的具体含义之前，我先讲几点 CMake 变量的约定：

* **CMake** [**将所有的变量的值视为字符串**](#)
* 当给出变量的值不包含空格的时候，可以不使用引号，但建议都加上引号，不然一些奇怪的问题很难调试。
* CMake 使用**空格**或者**分号**作为字符串的**分隔符**。
* CMake 中想要获取变量的值，和 shell 脚本一样，采用 **${var}** 形式。但是在 **IF** 控制语句中是直接使用变量名。 
* 使用 **CMake** 变量前，不需要这个变量已经定义了，如果使用了未定义的变量，**那么它的值是一个空字符串**。
   * 默认情况下使用未定义的变量不会有警告信息，但是可以通过 cmake 的 -warn-uninitialized 选项启用警告信息。
   * 使用未定义的变量非常常见，如果出现问题也不一定是因为变量未定义导致的，所以 cmake 的 -warn-uninitialized 选项用处很有限。
* 变量的值可以包含换行，也可以包含引号，不过需要转义。

**参数说明：**
* 第一个参数是必须的，代表要定义的变量的变量名。
* 第二个参数也是必须的，代表要定义的变量的值，根据上述 CMake 变量的约定，我们知道这里的 value 应该是一个字符串，而且不管有没有空格，都建议用引号引起来。
  * 当然第二个参数可以是一个以空格或者分号隔开的字符串，这样定义的变量将是一个列表
* **PARENT_SCOPE** 是一个可选参数，意思是定义的这个变量的作用域属于父作用域

```cmake
set(CPR_TOP_DIR "/root/workspace/code/cpr")

set(CPR_BASE_SOURCE "a.cpp;b.cpp")  #列表

set(CPR_CORE_SOURCE core1.cpp core2.cpp)  #列表

set(PROJECT_IS_NEED_STRINGS "Need" "Doll" "Mike")

foreach(a ${PROJECT_IS_NEED_STRINGS}) #列表打印
    MESSAGE(STATUS "string: ${a}")
endforeach()

set(CPR_BUILD_CMD [[
#!/bin/bash

cmake -S . -B build
cmake --build build
]])

set(shellScript [=[
#!/bin/bash
[[ -n "${USER}" ]] && echo "Have USER"
]=])

SET(my_list “kicker;miner kicker”)
MESSAGE(STATUS ${my_list}) #当作列表了

LIST(LENGTH my_list len)
MESSAGE(STATUS "my_list len: ${len}") #my_list len: 3
```
**避免分号变成列表**,使用`[[]]`。
```cmake
SET(my_list “[[kicker;miner]] kicker”)
MESSAGE(STATUS ${my_list}) #当作列表了

LIST(LENGTH my_list len)
MESSAGE(STATUS "my_list len: ${len}") #my_list len: 2
```
**避免分号 空格变成列表**, 使用`[[]]`。

```cmake
SET(my_list “[[kicker;miner kicker]]”)
MESSAGE(STATUS ${my_list}) #一个字符串

LIST(LENGTH my_list len)
MESSAGE(STATUS "my_list len: ${len}") #my_list len: 1
```

#### [1.1 环境变量](#)
不管是 Windows 还是 linux、macOS 上，都有环境变量的概念。我们最常使用的环境变量要数 PATH 这个环境变量了。Windows 上可以在高级系统设置->环境变量中看到，linux 和 macOS 上可以使用 echo $PATH 这条命令看到环境变量 PATH 的值。

获取系统的环境变量： **`$ENV{NAME}`**  
设置环境变量: **`SET(ENV{变量名} 值`**  

除了 PATH 这个环境变量，对于在日常的开发中，我们通常还会有自定义的环境变量。比如我们安装了一个第三方软件，然后需要将一些安装目录导出到环境变量，这个时候就可能会用到自定义环境变量。

有些环境变量，只有我们自己项目需要，所以就没有必要为整个系统配置这类环境变量。CMake 为我们提供了定义环境变量的方式，这样我们就可以让 CMake 定义的环境变量只在当前运行的 CMake 进程中生效，不会影响到系统或者其他进程的环境变量。

cmake 定义环境变量和获取环境变量的值用法和普通变量类似，只需要多加一个 ENV 标识符。

```cmake
set(ENV{PATH} "$ENV{PATH}:/opt/myDir")

message(STATUS "PATH=$ENV{PATH}")
#PATH:  /home/remix/.cargo/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
#       :/usr/games:/usr/local/games:/snap/bin:/snap/bin:/usr/bin:/home/remix/progress/clion-
#       2022.3.3/bin/ninja/linux/x64
```

设置环境变量和使用环境变量在简单的 CMake 项目中很少见，大型跨平台项目中就比较常见，我们后面讲到跨平台项目的时候就会使用到 CMake 环境变量。

**将环境变量长字符串转换为列表:**

```cmake
set(ENV_PATH_ALL $ENV{PATH})

message(STATUS "PATH: ${ENV_PATH_ALL}")
string(REPLACE ":" ";" PATH_LIST ${ENV_PATH_ALL})

foreach(a ${PATH_LIST})
    MESSAGE(STATUS "env:${a}")
endforeach()
```

#### [1.2 unset 变量](#)

取消一个变量可以使用 unset 指令。

### [2. 变量之缓存变量](#)

与普通变量不同，缓存变量的值可以缓存到 CMakeCache.txt 文件中，当再次运行 cmake 时，可以从中获取上一次的值，而不是重新去评估。**所以缓存变量的作用域是全局的**， CMake还有很多内置缓存变量。

CMake 定义缓存变量的格式如下：
```cmake
set(varName value... CACHE type "docstring" [FORCE])
```
和普通变量比起来，缓存变量携带了更多的信息。缓存变量有类型了，而且可以为缓存变量添加一个说明信息。

从上面的 CMake 定义缓存变量的命令中，我们可以看到，第一个参数依然是变量的名字，第二个参数是变量的值。缓存变量不同于普通变量是从第三个参数开始的，第三个参数是固定 CACHE 这个关键字，表示这条命令定义的是缓存变量。

第四个参数 type 是必选参数，而且其值必须是下列值之一。
- **BOOL**
  - BOOL 类型的变量值如果是 `ON、TRUE、1` 则被评估为真，如果是 `OFF、FALSE、0` 则被评估为假。
  - 当然除了上面列出的值还有其他值，但是判断真假就没那么清晰了，所以建议定义 BOOL 类型的缓存变量的时候，其值就采用上述列出的值。虽然不区分大小写，但是我建议统一使用大小。
- **FILEPATH**
  - 文件路径
- **PATH**
  - 目录路径
- **STRING**
  - 字符串
- **INTERNAL**
  - 内部缓存变量不会对用户可见，一般是项目为了缓存某种内部信息时才使用，cmake 图形化界面工具也对其不可见。
  - 内部缓存变量默认是 FORCE 的。
  - FORCE 关键字代表每次运行都强制更新缓存变量的值，如果没有该关键字，当再次运行 cmake 的时候，cmake 将使用 CMakeCache.txt 文件中缓存的值，而不是重新进行评估。

CMake 自身是将所有变量的值均视为字符串的，这里指定类型只是为了提高 cmake 图形界面工具的用户体验。

第五个参数是一个说明性的字符串，可以为空，只在图形化 cmake 界面会展示。

由于 BOOL 类型的变量使用频率非常高，CMake 为其单独提供了一条命令，**option提供的是缓存变量！**

```cmake
option(optVar helpString [initialValue])
```
第一个参数是变量的名字，第二个参数是提供帮助信息的字符串，可以为空字符串。
initialValue 是可选参数，代表缓存变量的值，如果没有提供，那该缓存变量的值默认为 OFF。

上述命令等价于：
```cmake
set(optVar initialValue CACHE BOOL helpString)
```
不过上述两个命令定义缓存变量是有一点点区别的，option() 命令没有 FORCE 关键字。



#### [2.1 ccmake](#)

`ccmake`是在`cmake`生成之后，对`cmake`的`cache`进行修改，重新生成`makefile`文件。所以需要先执行`cmake`。 生成目录在build之中，重新生成缓存变量。

```cmake
ccmake build
```



#### [2.2 使用命令行传递缓存变量](#)

**-D** `<var>[:<type>]=<value>` 创建一个CMake的缓存条目，直白点说就是向cmake传递变量。

```cmake
cmake -S . -B build -D IS_MORE_BUILD:BOOL=ON
```



### [3. 变量之作用域](#)

做 C/C++ 开发，作用域大家都很清楚是什么样的一个概念。作用域和变量是密切相关的。CMake 有变量的概念，自然也有作用域的概念。不过 CMake 中的作用域可不简简单单针对变量。

在 C/C++ 中，我们可以使用 {}、函数、类等产生新的作用域，同时也有全局作用域的概念。在 CMake 中，通常只有在使用 **add_subdirectory()** 命令或者定义函数的时候产生新的作用域。自 CMake 3.25 开始，可以使用 **block()** 在任意位置产生新的作用域。

* **我们上一讲有讲到 CMake 的缓存变量，CMake 缓存变量的作用域是全局的。**
* **CMake 环境变量的作用域也是全局的，只有 CMake 普通变量的作用域受到不同 CMake 命令的影响。**

我们在定义 CMake 普通变量的时候，如果没有 PARENT_SCOPE 选项，那该变量的作用域就在当前的 CMakeLists.txt 中或者在当前的函数，或者当前的 block() 中。

src/toolkits/CMakeLists.txt
```cmake
set(A_VALUE_STRING "SZ57481" PARENT_SCOPE)

message(STATUS "sub: ${A_VALUE_STRING}") #访问不到为空
```
A_VALUE_STRING 在上级目录的 CMakeLists.txt可以访问到！

src/CMakeLists.txt
```cmake
add_subdirectory(toolkits)  #这个指令很重要  message要放在它下面

message(STATUS "Parent: ${A_VALUE_STRING}") # Parent: SZ57481
```

#### [3.1 常见作用域](#)
其他作用域和程序设计语言的作用于差不多了！

```cmake
function(f)
    set(A ON)
    message(STATUS "A:${A}")
endfunction()

message(STATUS "A Outer:${A}")

f()
# -- A Outer:
# -- A:ON
```

#### [3.2 block 命令](#)

```cmake
block([SCOPE_FOR [VARIABLES] [POLICIES]] [PROPAGATE var...])
endblock()
```
- 该命令需要 cmake >= 3.25 版本
- 该命令用于创建新的作用域（变量作用域、策略作用域）

下面看一些例子：

```cmake
set(x 1)

block()
    set(x 2)   # Shadows outer "x"
    set(y 3)   # Local, not visible outside the block
endblock()

# Here, x still equals 1, y is not defined
```

```cmake
set(x 1)
set(y 3)

block()
    set(x 2 PARENT_SCOPE)
    unset(y PARENT_SCOPE)
    # x still has the value 1 here
    # y still exists and has the value 3
endblock()

# x has the value 2 here and y is no longer defined
```

```cmake
set(x 1)
set(z 5)

block(PROPAGATE x z)
    set(x 2) # Gets propagated back out to the outer "x"
    set(y 3) # Local, not visible outside the block
    unset(z) # Unsets the outer "z" too
endblock()

# Here, x equals 2, y and z are undefined
```

```cmake
set(x 1)
set(z 5)

block(SCOPE_FOR VARIABLES PROPAGATE x z)
    set(x 2) # Gets propagated back out to the outer "x"
    set(y 3) # Local variable, not visible outside the block
    unset(z) # Unsets the outer "z" too
endblock()

# Here, x equals 2, y and z are undefined
```

### [4. 变量注意](#)
除了在 CMakeLists.txt 中定义缓存变量，我们还有其他方式定义缓存变量吗？

方法如下：
```shell
cmake -D myVar:type=someValue ...
```
- 使用上述 cmake 命令 -D 的方式定义缓存变量，只需要第一次运行时指定就行了，如果每次都指定，该缓存变量的值也每次都会更新。
- 可以使用多个 -D 指定多个缓存变量
- 如果 -D 设置一个忽略类型的缓存变量，在 CMakeLists.txt 中也定义了一个同名的缓存变量，并指定类型为 FILEPATH 或者 PATH，那从不同的目录运行 cmake 会有什么不同？

我们可以使用 cmake 命令 -D 的方式定义缓存变量，那有没有类似方式取消缓存变量的定义呢？
```shell
cmake -U 'help*' -U foo ...
```
- 支持通配符 `*` 和 `?`

- 高级变量
  ```cmake
  mark_as_advanced([CLEAR|FORCE] var1 [var2...])
  ```
  - CLEAR：不标记为高级变量
  - FORCE：标记为高级变量
  - 如果这两个关键字都没有，那只有在该变量从未标记过高级变量或非高级变量时才将其标记为高级变量
- Grouped 选项，前缀相同的变量组合在一起



### [5. 变量传递给C++](#)

传递宏变量的方式有很多中，一种可以通过 指令add_definitions传递，还可以通过configure_file来传递：

#### [5.1 add_definitions](#)

用于添加宏定义，注意该命令没有执行顺序的问题，只要改项目中用了该命令定义宏，那么所有的源代码都会被定义这个宏。采用add_definitions定义宏,但是这种定义方式**无法给宏具体值**，使用configure_file可以。

```shell
add_definitions(-DFOO -DBAR ...)
```

使用例子:
```shell
add_definitions("-DUNITTEST")
if(${UNITTEST})
    add_subdirectory(unit-tests)
endif()
#ifdef UNITTEST
Func_TransportClient* getDiagTransportClient()
{
return m_DiagTransportClient;
}

void setDiagTransportClient(Func_TransportClient* m_Client)
{
m_DiagTransportClient = m_Client;
}
#endif
```

#### [5.2 configure_file](3)

configure_file 可以复制一个文件到另一个地方，并对内容进行修改。

```cmake
configure_file(<input> <output>
               [NO_SOURCE_PERMISSIONS | USE_SOURCE_PERMISSIONS |
                FILE_PERMISSIONS <permissions>...]
               [COPYONLY] [ESCAPE_QUOTES] [@ONLY]
               [NEWLINE_STYLE [UNIX|DOS|WIN32|LF|CRLF] ])

```
* **input**：输入的文件名，通常为 `xxx-config.h.in`；
* **output**：输出的文件名，通常为 `xxx-config.h`；
* **@ONLY**：在 \<input\> 文件中只使用 `@VAR@` 的方式获取变量值，不适用 $\{VAR\} 的方式；


这里把PROJECT_SOURCE_DIR中的**networkConfig.h.in**复制到PROJECT_BINARY_DIR并变成 **networkConfig.h**
```cmake
configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/networkConfig.h.in"
        "${CMAKE_SOURCE_DIR}/src/networkConfig.h"
)

```

networkConfig.h.in
```cpp
#define NETWORK_VERSION_MAJOR @NETWORK_VERSION_MAJOR@
#define NETWORK_VERSION_MINOR @NETWORK_VERSION_MINOR@
#define CMAKE_BUILD_TYPE @CMAKE_BUILD_TYPE@

#ifndef NERWORK_CONFIG_H
#define NERWORK_CONFIG_H

    #cmakedefine BUILD_TIME_YEAR @BUILD_TIME_YEAR@
    #cmakedefine BUILD_TIME_MONTH @BUILD_TIME_MONTH@
    #cmakedefine BUILD_TIME_DAY @BUILD_TIME_DAY@
    #cmakedefine BUILD_AUTHOR "@BUILD_AUTHOR@"

    #cmakedefine01 MEM_CHECKER
    #cmakedefine DEBUG_LEVEL @DEBUG_LEVEL@

#endif
```
CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR) 

project(network 
    VERSION         1.0.0
    DESCRIPTION     "CMake Learning Demo" 
    HOMEPAGE_URL    "https://gitee.com/cicer/c-learning.git"
    LANGUAGES        C CXX
)

set(NETWORK_VERSION_MAJOR 1)
set(NETWORK_VERSION_MINOR 0)

string(TIMESTAMP BUILD_TIME_YEAR %y)
string(TIMESTAMP BUILD_TIME_MONTH %m)
string(TIMESTAMP BUILD_TIME_DAY %d)

set(BUILD_AUTHOR "sdc")

set(DEBUG_LEVEL 1 CACHE STRING "debug level")

option(MEM_CHECKER "check memory malloc and free" ON)


configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/networkConfig.h.in"
        "${CMAKE_SOURCE_DIR}/src/networkConfig.h"
)

#源代码
add_subdirectory(src)
```

生成的文件内容
```cpp
#define NETWORK_VERSION_MAJOR 1
#define NETWORK_VERSION_MINOR 0
#define CMAKE_BUILD_TYPE Debug

#ifndef NERWORK_CONFIG_H
#define NERWORK_CONFIG_H

    #define BUILD_TIME_YEAR 23
    #define BUILD_TIME_MONTH 03
    #define BUILD_TIME_DAY 26
    #define BUILD_AUTHOR "sdc"

    #define MEM_CHECKER 1
    #define DEBUG_LEVEL 1

#endif
```
源代码使用：
```cpp
#include "toolkits.h"
#include "user.h"
#include <iostream>
#include "networkConfig.h"
#ifdef CMAKE_BUILD_TYPE && CMAKE_BUILD_TYPE == debug
    #include <assert.h>
#endif


int main(int args,  char * argv[]){
    std::cout << toolkits::Add<double>(3.14, 6.28) << std::endl;
    std::cout << toolkits::myAdd(5, 6)<<std::endl;

#ifdef CMAKE_BUILD_TYPE && CMAKE_BUILD_TYPE == debug
    assert(toolkits::myAdd(5, 5) == 20);
#endif

    user remix(std::string("remix"));
    std::cout << remix.getName() << " calculate result: " << remix.calculate(5,6) <<"!"<< std::endl;
    return 0;
}
```

### [6. 数学运算](#)
有时候，我们需要对 CMake 变量之间进行数学运算，这时候 CMake 提供了 math() 这个命令，命令格式如下：

```cmake
math(EXPR outVar mathExpr [OUTPUT_FORMAT format])
```
这个命令也很简单，直接通过 CMake 变量结合数学运算符组成 mathExpr，然后计算结果会保存到 outVar 中。

OUTPUT_FORMAT 是可选参数，代表输出结果的格式，可以是 HEXADECIMAL：输出 16 进制结果，DECIMAL：输出 10 进制结果。

使用例子：
```cmake
set(x 20)
set(y 20)

math(EXPR result "(${x}+${y})*100" OUTPUT_FORMAT DECIMAL)
math(EXPR result_16 "(${x}+${y})*100" OUTPUT_FORMAT HEXADECIMAL)

message(NOTICE "result:${result}")      #4000
message(NOTICE "result16:${result_16}") #0xfa0
```
