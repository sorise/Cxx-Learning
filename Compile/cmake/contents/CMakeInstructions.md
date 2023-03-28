## [CMake 指令](#)
**介绍**: CMake命令有两种，其中一种是编写命令脚本包括有条件命令、循环命令，还有一种是项目命令包括编译命令、库命令。

----

- [1. CMake 常用命令脚本](#1-cmake-常用命令脚本)
- [2. CMake 项目命令](#2-cmake-项目命令)
- [3. CMake 条件表达式 if](#3-cmake-条件表达式-if)
- [4. CMake 列表 list](#4-cmake-列表-list)
- [5. Cmake file](#5-cmake-file)
- [6. Cmake function](#6-cmake-function)
- [7. Cmake while](#7-cmake-while)
- [8. Cmake foreach](#8-cmake-foreach)

----

### [1. CMake 常用命令脚本](#)
以下列举常见的命令：

|命令|解释|
|:----|:----|
|**1. message**|打印各种日志信息。|
|**2. cmake_minimum_required**|设置CMake的版本信息。|
|**3. set**|将普通变量、缓存变量或环境变量设置为给定值。|
|**4. option**|用于快速设置定义变量并赋值为对应的bool值，会缓存在cmakecache中！|
|**5. unset**|取消设置变量、缓存变量或环境变量。|
|**6. configure_file**|可以复制一个文件到另一个地方，并对内容进行修改。|

#### [1.1 message](#)
在日志中记录指定的消息文本。如果给定了多个消息字符串，则它们被连接到一个消息中，字符串之间没有分隔符。

```shell
message([<mode>] "message text" ...)
```
可选的`＜mode＞`关键字决定消息的类型，这会影响消息的处理方式：
* **FATAL_ERROR** ： CMake 出错，停止编译和生成(信息红色)
* **SEND_ERROR**： CMake 错误，继续处理，但跳过生成。
* **WARNING**： CMake 警告，继续编译(信息红色)
* **AUTHOR_WARNING** ： 开发者警告，继续编译(信息红色)
* **(none) or NOTICE** ： 不设置mode，默认是NOTICE模式，不影响编译和生成，用于打印消息(信息白色)
* **STATUS** ： 编译时状态信息，左边以–和空格开头(信息白色)
* **DEBUG** ： 针对项目本身的开发人员而不是只想构建项目的用户的详细信息消息。这些消息通常不会引起构建项目的其他用户的兴趣，并且通常与内部实现细节密切相关。
使用例子：
```cmake
set(USER_INFORMATION_CXX "这是一个好消息！")
message(WARNING "waning： ${USER_INFORMATION_CXX}")
```

**多行输出:**
```shell
message(STATUS [[

    first line
    second line
    third line
]])

message(STATUS [==[

    first line
    second line
    third line
]==])
```

#### [1.2 cmake_minimum_required](#)
需要最低版本的cmake。

```cmake
cmake_minimum_required(VERSION <min>[...<policy_max>] [FATAL_ERROR])
```
使用例子：

```cmake
# CMake最小版本要求为3.22.1, 小于这个版本，直接禁止编译！
cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
```

#### [1.3 set](#)
set可以设置普通变量，也可以设置环境变量，还可以设置缓存条目

**设置普通变量：**
```cmake
set(<variable> <value>... [PARENT_SCOPE])
```

**设置缓存条目：** 其中CACHE会将变量定义在缓存文件CMakeCache.txt里，可以在下次编译的时候读取。
```cmake
set(<variable> <value>... CACHE <type> <docstring> [FORCE])
```
**设置环境变量：**

```cmake
set(ENV{<variable>} [<value>])
```

#### [1.4 option](#)
用于快速设置定义变量并赋值为对应的bool值，常被用于判断某些操作是否执行。

提供一个选项供用户选择为ON或OFF。如果没有提供初始<值>，则使用OFF。如果\<variable\>已设置为普通变量或缓存变量，则该命令不执行任何操作。

```cmake
option(<variable> "<help_text>" [value])
```
使用例子：
```cmake
option(IS_SHOW "是否打印日志" ON)

IF(IS_SHOW)
    MESSAGE(STATUS "打印日志!")
ENDIF()
```
#### [1.5 unset](#)
取消设置变量、缓存变量或环境变量。

```cmake
#取消设置变量、缓存变量
unset(<variable> [CACHE | PARENT_SCOPE])

#环境变量。
unset(ENV{<variable>})
```


#### [1.6 configure_file](#)
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
* **@ONLY**：在 \<input\> 文件中只使用 @VAR@ 的方式获取变量值，不适用 $\{VAR\} 的方式；


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


### [2. CMake 项目命令](#)
这些命令仅在CMake项目中可用。

|命令|解释|
|:----|:----|
|**1. project**|打印各种日志信息。|
|**2. include_directories**|用于指定头文件搜索路径。|
|**3. add_executable**|用于添加可执行文件。|
|**4. add_library**|用于添加库文件，可以创建静态库或动态库。|
|**5. target_link_libraries**|为 target 添加需要链接的共享库。|
|[**6. link_directories**](#)|向工程添加多个特定的库文件搜索路径。|
|**7. aux_source_directory**|发现一个目录下所有的源代码文件并将列表存储在一个变量中。|
|**8. add_compile_options**|添加编译参数。|
|**9. add_definitions**|添加宏定义。不再推荐使用，建议使用add_compile_definitions|
|[**10. add_subdirectory**](#)|用于添加子项目目录。|
|[**11. target_include_directories**](#)|指定头文件搜索路径，并将搜索路径关联到一个target上。|
|**12. target_compile_options**|设置编译器选项！|
|**13. add_compile_definitions**|添加宏定义。|
|**14. find_library**|查找/指定所需的库(推荐)。|
|**15. target_compile_definitions**|为某个目标定义宏。|
|**16. get_target_property**|获取target的属性。|


#### [2.1 project](#)
设置项目的名称，并将其存储在变量project_name中。从顶级CMakeLists.txt调用时，还会将项目名称存储在变量CMAKE_project_name中，
定义工程名称，并可指定工程支持的语言。

```shell
project(<PROJECT-NAME> [<language-name>...])

project(<PROJECT-NAME>
        [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
        [DESCRIPTION <project-description-string>]
        [HOMEPAGE_URL <url-string>]
        [LANGUAGES <language-name>...])
```
使用例子：
* `PROJECT (HELLO C CXX JAVA)` 指定了工程的名字，并且支持语言是C和C++还有java。
* `PROJECT (HELLO VERSION 1.0.0 LANGUAGES C CXX)`  制定项目名 版本号 语言列表。

```shell
project(network 
    VERSION         1.0.0
    DESCRIPTION     "CMake Learning Demo" 
    HOMEPAGE_URL    "https://gitee.com/cicer/c-learning.git"
    LANGUAGES        C CXX
)
```


#### [2.2 include_directories](#)
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

#### [2.3 add_executable](#)
用于添加可执行文件。第一个参数很重要，被称为target，可以作为target_xxx命令的接收对象。

语法：
```shell
add_executable(<name> [WIN32] [MACOSX_BUNDLE]
               [EXCLUDE_FROM_ALL]
               [source1] [source2 ...])
```

使用例子：
```shell
add_executable(network main.cpp)
```

#### [2.4 add_library](#)
用于添加库文件，可以创建静态库或动态库。第一个参数很重要，被称为target，可以作为target_xxx命令的接收对象。

```shell
add_library(libname [SHARED|STATIC|MODULE] [EXCLUDE_FROM_ALL] source1 source2 ... sourceN)
```
使用例子：

```shell
add_library(test_lib a.cpp b.cpp) #默认生成静态库
add_library(test_lib SHARED a.cpp b.cpp) #生成动态库
```

#### [2.5 target_link_libraries](#)
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

#### [2.6 link_directories](#)
向工程添加多个特定的库文件搜索路径。

将指定目录添加到编译器的头文件搜索路径之下，指定的目录被解释成当前源码路径的相对路径。


使用例子：
```shell
link_directories(/usr/lib/mylibfolder ./lib)
```

#### [2.7 aux_source_directory](#)
发现一个目录下所有的源代码文件并将列表存储在一个变量中，这个指令临时被用来自动构建源文件列表。

```shell
aux_source_directory(<dir> <variable>)
```

使用例子：
```shell
# 定义SRC变量，其值为当前目录下所有的源代码文件
aux_source_directory(. SRC_CPPS)
# 编译SRC变量所代表的源代码文件，生成main可执行文件
add_executable(main ${SRC_CPPS})
```

#### [2.8 add_compile_options](#)
添加编译参数!

```shell
#语法:
add_compile_options(<option> ...)
```

使用例子：
```shell
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -O2 -std=c++17 -Wextra -pedantic -Werror)
endif()
```

#### [2.9 add_definitions](#)
用于添加宏定义，注意该命令没有执行顺序的问题，只要改项目中用了该命令定义宏，那么所有的源代码都会被定义这个宏。

```shell
add_definitions(-DFOO -DBAR ...)
```

使用例子:
```shell
add_definitions("-DUNITTEST")
if(${UNITTEST})
    add_subdirectory(unit-tests)
endif()
```

```cpp
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

#### [2.10 add_subdirectory](#)
添加子项目目录，如果有该条语句，就先会跑去执行子项目的cmake代码，这样会导致一些需要执行后立马生效的语句作用不到，比如include_directories和link_directories如果执行在这条语句后面。

则他们添加的目录在子项目中无法生效。有些命令如target_include_directories和target_link_directories是根据目标target是否被链接使用来生效的，所以这些命令的作用范围与执行顺序无关，
且恰好同一个cmake项目中产生的库文件是可以直接通过名称链接的，无论链接对象是在子目录还是父目录。

```shell
add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
```
* **source_dir** :指定源CMakeLists.txt和代码文件所在的目录。如果它是一个相对路径，它将根据当前目录（典型用法）进行评估，但它也可能是一个绝对路径。
* **binary_dir** :指定放置输出文件的目录。如果它是一个相对路径，它将相对于当前输出目录进行评估，但它也可能是一个绝对路径。如果未指定binary_dir，则在展开任何相对路径之前，将使用source_dir的值（典型用法）
* **EXCLUDE_FROM_ALL** : 当指定了该参数，则子目录下的目标不会被父目录下的目标文件包含进去，父目录的CMakeLists.txt不会构建子目录的目标文件，必须在子目录下显式去构建。
例外情况：当父目录的目标依赖于子目录的目标，则子目录的目标仍然会被构建出来以满足依赖关系（例如使用了target_link_libraries）。

```
├── CMakeLists.txt
├── doc
├── include
│   ├── prints.h
│   └── toolkits.hpp
├── levelDB
│   ├── CMakeLists.txt
│   └── leveldb.cpp
├── main.cpp
└── src
    └── prints.cpp
```

**重点：前后位置问题** 主要相对于其他指令的位置问题!
```shell
include_directories(include)
link_directories(lib)

#增加子目录 但是子目录可以使用  include 的头文件 和连接目录 lib
add_subdirectory(levelDB)
```

```shell
#增加子目录 但是子目录不可以使用  include 的头文件 和连接目录 lib
add_subdirectory(levelDB)

include_directories(include)
link_directories(lib)
```

#### [2.11 target_include_directories](#)
该命令可以指定目标（exe或者so文件）需要包含的头文件路径，命名为 **<target>** 的命令必须是由 **add_Executive()** 或 **add_library()** 之类的命令创建的，并且不能是 ALIAS 目标。

```shell
target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

* **PUBLIC**: 对内对外都公开，可将头文件目录以及链接库传递给连接者,target对应的头文件、源文件都使用。
* **PRIVATE**: 对内公开，对外不公开，不可传递头文件目录以及链接库,target对应的源文件使用。
* **INTERFACE**: 对外公开，对内不公开，可传递头文件目录以及链接库，但内部不可使用头文件及连接库，只能接口传递,target对应的头文件使用。

指定头文件搜索路径，并将搜索路径关联到一个target上，这里的target一般是指生成可执行程序命令里的target或者生成库文件的target，与上

一个命令的不同点在于可以设置导出权限，比如现在我写了一个项目，这个项目引入了其他库，但是我不想让其他库的符号暴露出去（毕竟使用这个

项目的人只关注这个项目的接口，不需要关注其他依赖的接口）可以通过PRIVATE将头文件搜索目录设置不导出的权限。

```shell
add_library(levelDB SHARED src/leveldb.cpp)

target_include_directories(levelDB PUBLIC ${PROJECT_SOURCE_DIR}/include)
```

#### [2.12 target_compile_options](#)
为可执行目标设置了编译选项:

语法：
```cmake
target_compile_options(<target> [BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```
* PRIVATE，编译选项会应用于给定的目标，不会传递给与目标相关的目标
* INTERFACE，给定的编译选项将只应用于指定目标，并传递给与目标相关的目标。
* PUBLIC，编译选项将应用于指定目标和使用它的目标。

使用例子：
```cmake
target_compile_options(compute-areas
  PRIVATE
    "-fPIC"
  )
```

#### [2.13 add_compile_definitions](#)
这个命令是 cmake v3.12 后添加的，相比于 add_difinitions，功能更加单一且参数使用灵活。

语法：
```cmake
add_compile_definitions(<definition> ...)
```

```cmake
add_compile_definitions(VALUE_MACRO=0x10000000)
# 等同于 C++ #define VALUE_MACRO 0x10000000
# 等同于 add_definitions(-DNONE_VALUE_MACRO=0x10000000)
```

**cmake 官方建议不再使用 add_definitions 而用 add_compile_definitions**

#### [2.14 find_library](#)
此命令用于查找库。创建一个缓存条目，或者一个普通变量，如果指定了 NO_CACHE ，由 命名来存储这个命令的结果。如果找到库，结果将存储在变量中，除非清除变量，否则不会重复搜索。如果什么也没找到，结果将是 -NOTFOUND 。

简短模式：
```cmake
find_library (<VAR> name1 [path1 path2 ...])
```

```cmake
find_library (
    <VAR>
    name | NAMES name1 [name2 ...] [NAMES_PER_DIR]
    [HINTS [path | ENV var]... ]
    [PATHS [path | ENV var]... ]
    [REGISTRY_VIEW (64|32|64_32|32_64|HOST|TARGET|BOTH)]
    [PATH_SUFFIXES suffix1 [suffix2 ...]]
    [VALIDATOR function]
    [DOC "cache documentation string"]
    [NO_CACHE]
    [REQUIRED]
    [NO_DEFAULT_PATH]
    [NO_PACKAGE_ROOT_PATH]
    [NO_CMAKE_PATH]
    [NO_CMAKE_ENVIRONMENT_PATH]
    [NO_SYSTEM_ENVIRONMENT_PATH]
    [NO_CMAKE_SYSTEM_PATH]
    [NO_CMAKE_INSTALL_PREFIX]
    [CMAKE_FIND_ROOT_PATH_BOTH |
    ONLY_CMAKE_FIND_ROOT_PATH |
    NO_CMAKE_FIND_ROOT_PATH]
)
```
* **NO_CACHE**: 3.21版中的新内容。搜索的结果将被存储在一个普通的变量中,而不是缓存条目。

#### [2.15 target_compile_definitions](#)
Add compile definitions to a target.

```cmake
#定义一个宏 IS_ENABLE_LAST_FUNC = 1
target_compile_definitions(entities PUBLIC IS_ENABLE_LAST_FUNC=1 )
```

使用这个宏定义: 
```cpp
#if IS_ENABLE_LAST_FUNC && IS_ENABLE_LAST_FUNC==1
    std::shared_ptr<user> miner = createUser("miner");
    std::cout << miner->getName() << " calculate result: " << miner->calculate(1,5) <<"!"<< std::endl;
#endif
```

#### [2.16 get_target_property](#)
获得某个target的属性:

语法: 将target的属性\<property\>值放入变量 VAR中！
```cmake
get_target_property(<VAR> target property)
```

使用例子：
```cmake
set_target_properties ( target_test PROPERTIES AAA 123 )
set_target_properties ( target_test PROPERTIES BBB 456 )

get_target_property ( VAR1 target_test AAA )
get_target_property ( VAR2 target_test BBB )

message ( STATUS "VAR1 = ${VAR1}" )
message ( STATUS "VAR2 = ${VAR2}" )
```

使用:
```cmake
#include 目录
target_include_directories(entities
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

get_target_property(_in_p entities INCLUDE_DIRECTORIES)
MESSAGE(STATUS ${_in_p})
# /home/remix/coding/cpp/network/src/entities
```

获得为定义属性，会在后面加上 `e-NOTFOUND`!
```cmake
get_target_property(not_define entities _NOT_DIRECTORIES)
MESSAGE(STATUS ${not_define})
# not_define-NOTFOUND
```
#### [2.17 macro](#)
CMake的宏！和C++差不多！

```cmake
macro(<name> [<arg1> ...])
  <commands>
endmacro()
```

使用例子:
```cmake
set(USER_NAME "kicker")
MESSAGE(NOTICE "user name: ${USER_NAME}")
#user name: kicker

macro(CHANGE_NAME name)
    set(USER_NAME ${name})
    MESSAGE(NOTICE "macro user name: ${USER_NAME}")
endmacro()

CHANGE_NAME("Remix") 
#[[ 等同于
set(USER_NAME "Remix")
MESSAGE(NOTICE "macro user name: Remix")
]]#
MESSAGE(NOTICE "user name: ${USER_NAME}")
#user name: Remix
```

#### [2.18 include和include_guard](#)
从文件或模块加载并运行CMake代码，类似于C++的 **#include**。include_guard类似于头文件中的 **#ifndef**，防止反复定义！

```cmake
include(<file|module> [OPTIONAL] [RESULT_VARIABLE <var>]
                      [NO_POLICY_SCOPE])
```
* 如果存在OPTIONAL，则如果文件不存在，则不会引发错误。
*  如果给定RESULT_VARIABLE，变量\<var\>将设置为已包含的完整文件名，如果失败，则设置为NOTFOUND。


```cmake
include(xxxx.cmake OPTIONAL) # xxxx.cmake不存在也不会触发warning或error
include(xxxx.cmake) # xxxx.cmake不存在,会触发error
# CMake Error at test_include.cmake:9 (include):
# include could not find requested file: xxxx.cmake

include(test_project.cmake RESULT_VARIABLE var)
message("var: ${var}") 
# var: /home/spring/GitHub/Linux_Code_Test/Samples_CMake/messy_usage/test_project.cmake
 
include(xxxx.cmake OPTIONAL RESULT_VARIABLE var) # xxxx.cmake不存在
message("var: ${var}") 
# var: NOTFOUND
 
include(xxxx.cmake RESULT_VARIABLE var) 
# xxxx.cmake不存在,触发error
# CMake Error at test_include.cmake:20 (include):
# include could not find requested file: xxxx.cmake
```

### [3. CMake 条件表达式 if](#)
条件命令，类似于C语言的条件分支指令。

|命令|解释|
|:----|:----|
|**1. if**| if 表达式|
|**2. elseif**| else if  表达式|
|**3. else**| else 表达式|
|**4. endif**| endif 表达式|

```shell
if(<condition>)
  <commands>
elseif(<condition>) # optional block, can be repeated
  <commands>
else()              # optional block
  <commands>
endif()
```
**逻辑表达式:** **NOT**、**AND**、**OR**
```shell
if(NOT <condition>)

if(<cond1> AND <cond2>)

if(<cond1> OR <cond2>)

if((condition) AND (condition OR (condition)))
```

#### [3.1 关系表达式](#)
可以理解为C++语言里面条件表达式: **==**、**<**、**<=**、**>**、**>=**。

CMake表达式有： EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, STREQUAL, STRLESS, STRLESS_EQUAL, STRGREATER, STRGREATER_EQUAL, VERSION_EQUAL, VERSION_LESS, VERSION_LESS_EQUAL, VERSION_GREATER, VERSION_GREATER_EQUAL, and MATCHES.

使用例子：
```shell
if(<variable|string> MATCHES regex)

if(<variable|string> LESS <variable|string>)

if(<variable|string> GREATER <variable|string>)

if(<variable|string> EQUAL <variable|string>)

if(<variable|string> LESS_EQUAL <variable|string>)

if(<variable|string> GREATER_EQUAL <variable|string>)

if(<variable|string> STRLESS <variable|string>)

if(<variable|string> STRGREATER <variable|string>)

if(<variable|string> STREQUAL <variable|string>)

if(<variable|string> STRLESS_EQUAL <variable|string>)

if(<variable|string> STRGREATER_EQUAL <variable|string>)
```

#### [3.2 版本对比](#)

使用例子：
```shell
if(<variable|string> VERSION_LESS <variable|string>)

if(<variable|string> VERSION_GREATER <variable|string>)

if(<variable|string> VERSION_EQUAL <variable|string>)

if(<variable|string> VERSION_LESS_EQUAL <variable|string>)

if(<variable|string> VERSION_GREATER_EQUAL <variable|string>)
```

#### [3.3 File Operations](#)
文件操作对比运算符！

使用例子： 如果命名的文件或目录存在，则为True。行为仅针对显式完整路径进行定义（前导~/不会扩展为主目录，而是被视为相对路径）。
```shell
if(EXISTS path-to-file-or-directory)
```

如果文件1比文件2新，或者两个文件中的一个不存在，则为True。
```shell
if(file1 IS_NEWER_THAN file2)
```

如果给定的名称是目录，则为True。仅对完整路径定义行为。
```shell
if(IS_DIRECTORY path-to-directory)
```

如果给定的名称是符号链接，则为True。仅对完整路径定义行为。
```shell
if(IS_SYMLINK file-name)
```

如果给定的路径是绝对路径，则为True。注意以下特殊情况：

* 空路径的计算结果为false。
* 在Windows主机上，任何以驱动器号和冒号（例如C:）、正斜杠或反斜杠开头的路径都将计算为true。这意味着像C:\base\dir这样的路径将计算为true，即使路径的非驱动器部分是相对的。
* 在非Windows主机上，任何以波浪号（~）开头的路径的计算结果都为true。

```cmake
if(IS_ABSOLUTE path)
```

#### [3.4 判断是否定义了某个变量](#)
使用IF可以判断是否定义了某个变量！

* IF(DEFINED my_names)  是否定义了普通变量 my_names
* IF(DEFINED CACHE{NETWORK_VERSION_LASTED})  是否定义了缓存变量 NETWORK_VERSION_LASTED
* IF(DEFINED ENV{user_name}) 是否定义了环境变量 user_name

```cmake
SET(my_names [[kicker、miner、kicker]])

IF(DEFINED my_names)
    MESSAGE(STATUS ${my_names})  #[[kicker、miner、kicker]]
ENDIF()

SET(NETWORK_VERSION_LASTED "1.3.4" CACHE STRING "system release version lasted!")

IF(DEFINED CACHE{NETWORK_VERSION_LASTED})
    MESSAGE(STATUS ${NETWORK_VERSION_LASTED}) #1.3.4
ENDIF()

IF(DEFINED ENV{user_name})
    MESSAGE(STATUS "DEFINE user_name")
ELSEIF()
    MESSAGE(STATUS "NO DEFINE user_name")
ENDIF()

```

### [4. CMake 列表 list](#)
CMake的list命令支持需多的操作：读取、搜索、修改、排序！

```shell
Reading
  list(LENGTH <list> <out-var>)
  list(GET <list> <element index> [<index> ...] <out-var>)
  list(JOIN <list> <glue> <out-var>)
  list(SUBLIST <list> <begin> <length> <out-var>)

Search
  list(FIND <list> <value> <out-var>)

Modification
  list(APPEND <list> [<element>...])
  list(FILTER <list> {INCLUDE | EXCLUDE} REGEX <regex>)
  list(INSERT <list> <index> [<element>...])
  list(POP_BACK <list> [<out-var>...])
  list(POP_FRONT <list> [<out-var>...])
  list(PREPEND <list> [<element>...])
  list(REMOVE_ITEM <list> <value>...)
  list(REMOVE_AT <list> <index>...)
  list(REMOVE_DUPLICATES <list>)
  list(TRANSFORM <list> <ACTION> [...])

Ordering
  list(REVERSE <list>)
  list(SORT <list> [...])
```


### [4. CMake 字符串 string](#)
CMake提供了处理字符串的string() 命令，其形式：

```cmake
# 字符串查找和替换
  string(FIND <string> <substring> <out-var> [...])
  string(REPLACE <match-string> <replace-string> <out-var> <input>...)
  string(REGEX MATCH <match-regex> <out-var> <input>...)
  string(REGEX MATCHALL <match-regex> <out-var> <input>...)
  string(REGEX REPLACE <match-regex> <replace-expr> <out-var> <input>...)

# 操作字符串
  string(APPEND <string-var> [<input>...])
  string(PREPEND <string-var> [<input>...])
  string(CONCAT <out-var> [<input>...])
  string(JOIN <glue> <out-var> [<input>...])
  string(TOLOWER <string> <out-var>)
  string(TOUPPER <string> <out-var>)
  string(LENGTH <string> <out-var>)
  string(SUBSTRING <string> <begin> <length> <out-var>)
  string(STRIP <string> <out-var>)
  string(GENEX_STRIP <string> <out-var>)
  string(REPEAT <string> <count> <out-var>)

# 字符串比较
  string(COMPARE <op> <string1> <string2> <out-var>)

# 计算字符串的 hash 值 HASH 选值如下所示:
  string(<HASH> <out-var> <input>)
   # MD5
   # Message-Digest Algorithm 5, RFC 1321.
   # SHA1
   # US Secure Hash Algorithm 1, RFC 3174.
   # SHA224
   # US Secure Hash Algorithms, RFC 4634.
   # SHA256
   # US Secure Hash Algorithms, RFC 4634.
   # SHA384
   # US Secure Hash Algorithms, RFC 4634.
   # SHA512
   # US Secure Hash Algorithms, RFC 4634.
   # SHA3_224
   # Keccak SHA-3.
   # SHA3_256
   # Keccak SHA-3.
   # SHA3_384
   # Keccak SHA-3.
   # SHA3_512
   # Keccak SHA-3.

# 生成字符串
  string(ASCII <number>... <out-var>)
  string(HEX <string> <out-var>)
  string(CONFIGURE <string> <out-var> [...])
  string(MAKE_C_IDENTIFIER <string> <out-var>)
  string(RANDOM [<option>...] <out-var>)
  string(TIMESTAMP <out-var> [<format string>] [UTC])
  string(UUID <out-var> ...)

# json 相关的字符串操作
  string(JSON <out-var> [ERROR_VARIABLE <error-var>]
         {GET | TYPE | LENGTH | REMOVE}
         <json-string> <member|index> [<member|index> ...])
  string(JSON <out-var> [ERROR_VARIABLE <error-var>]
         MEMBER <json-string>
         [<member|index> ...] <index>)
  string(JSON <out-var> [ERROR_VARIABLE <error-var>]
         SET <json-string>
         <member|index> [<member|index> ...] <value>)
  string(JSON <out-var> [ERROR_VARIABLE <error-var>]
         EQUAL <json-string1> <json-string2>)
```

#### 4.1 字符串查找
```cmake
string(FIND inputString subString outVar [REVERSE])
```
- 在 inputString 中查找 subString，将查找到的索引存在 outVar 中，索引从 0 开始。
- 如果没有 REVERSE 选项，则保存第一个查找到的索引，否则保存最后一个查找到的索引。
- 如果没有找到则保存 -1。

需要注意的是，string(FIND) 将所有字符串都作为 ASCII 字符，outVar 中存储的索引也会以字节为单位计算，因此包含多字节字符的字符串可能会导致意想不到的结果。

```cmake
string(FIND abcdefabcdef def fwdIndex)
string(FIND abcdefabcdef def revIndex REVERSE)
message("fwdIndex = ${fwdIndex}\n"  #3
        "revIndex = ${revIndex}")   #9

string(FIND "user is my son!" "son" idx)
string(FIND "user is my son!" "son" revIdx REVERSE)

message(STATUS "son index: ${idx}") #11
message(STATUS "son reverse index: ${revIdx}") #11
```

#### 4.2 替换字符串
```cmake
string(REPLACE matchString replaceWith outVar input...)
```

- 将 input 中所有匹配 matchString 的都用 replaceWith 替换，并将结果保存到 outVar 中。
- 如果有多个 input，它们是直接连接在一起的，没有任何分隔符。
  - 这有时可能会有问题，所以通常建议只提供一个 input 字符串。


我们还可以使用 string() 命令的正则方式替换字符串。
```cmake
string(REGEX MATCH    regex outVar input...)
string(REGEX MATCHALL regex outVar input...)
string(REGEX REPLACE  regex replaceWith outVar input...)
```
- input 字符串同样会在开始匹配正则表达式前进行串联。
- MATCH 只查找第一个匹配的字符串，并保存到 outVar 中。
- MATCHALL 会查找所有匹配的字符串，并保存到 outVar 中，如果匹配到多个，outVar 将是一个列表，列表我们后面会讲。
- REPLACE 会将每一个匹配到的字符串用 replaceWith 替换后，将替换后的完整字符串放到 outVar 中。

```cmake
string(REPLACE "-" ";" VS "kicker-god-need-mike")
MESSAGE(STATUS ${VS}) #kickergodneedmike

string(REGEX MATCH    "[ace]"           matchOne abcdefabcdef)
string(REGEX MATCHALL "[ace]"           matchAll abcdefabcdef)
string(REGEX REPLACE  "([de])" "X\\1Y"  replVar1 abc def abcdef)
string(REGEX REPLACE  "([de])" [[X\1Y]] replVar2 abcdefabcdef)
message("matchOne = ${matchOne}\n"
        "matchAll = ${matchAll}\n"
        "replVar1 = ${replVar1}\n"
        "replVar2 = ${replVar2}")

string(REPLACE "-" ";" VS "kicker-god-need-mike")

foreach(word ${VS})
    message(STATUS "word: ${word}")
endforeach()
```

#### 4.3 截取字符串
```cmake
string(SUBSTRING input index length outVar)
```
- 将 input 字符串从 index 处截取 length 长度放到 outVar 中。
- 如果 length 为 -1 的话，将从 index 到 input 结尾的字符串串保存到 outVar 中。

#### 4.4 常用操作
```cmake
string(LENGTH  input outVar)
string(TOLOWER input outVar)
string(TOUPPER input outVar)
string(STRIP   input outVar)
```
- LENGTH 获取 input 字符串的长度，保存到 outVar 中。
- TOLOWER 将 input 字符串转换成小写保存到 outVar 中。
- TOUPPER 将 input 字符串转换成大写保存到 outVar 中。
- STRIP 将 input 字符串的头尾空格去掉。

#### 4.5 HASH操作
支持对字符串进行HASH操作！
```cmake
string(SHA3_256 HASH_VS "kicker-god-need-mike")
message(STATUS ${HASH_VS})
#bb6818af3a708354d741934bf67034002c61dce6c8fb4182cfbcaf4555de7d25
```

#### [4.6 TIMESTAMP](#) 
获取时间辍 `  string(TIMESTAMP <out-var> [<format string>] [UTC])`  `format string` 查看[官方文档](https://cmake.org/cmake/help/latest/command/string.html#timestamp)

```cmake
string(TIMESTAMP CURRENT_DATETIME "%Y/%m/%d %H:%M:%S")
message(STATUS "running at: ${CURRENT_DATETIME}")
#running at: 2023/03/24 21:34:21
```


### [5. Cmake file](#)
这个命令实在是太强大了，你如果翻一翻这个官方文档就会发现它具备几乎文件系统的所有功能，什么读写文件啊，什么从网上下载文件，本地上传文件之类的它都有，计算文件的相对路径，路径转化等等!
[API](https://cmake.org/cmake/help/latest/command/file.html)

### [6. Cmake function](#)
在CMake中定义函数，首个参数是函数名，随后的是参数！ Cmake函数和C++一样具有自己的作用域，**同时也支持 return()操作！**

重点理解一下参数的意义！怎么获得参数名，怎么获得参数值。

```cmake
function(<name> [<arg1> ...])
  <commands>
endfunction()
```

使用例子：
```cmake
function(get_datetime VAL)
    message(NOTICE "variable : ${VAL}, ${${VAL}}")
    #variable : DT, now

    string(TIMESTAMP temp "%Y/%m/%d %H:%M:%S")
    set(${VAL} ${temp} PARENT_SCOPE)
endfunction()

set(DT "now")
get_datetime(DT)
message(STATUS "running at: ${DT}")
```

### [7. Cmake while](#)
可能用得较少... ,支持**break()** 指令提前结束循环! 支持**continue()**直接下一轮!

```cmake
while(<condition>)
  <commands>
endwhile()
```


### [8. Cmake foreach](#)
CMake 迭代器！支持**break()** 指令提前结束循环! 支持**continue()**直接下一轮!

```cmake
foreach(<loop_var> <items>)
  <commands>
endforeach()
```
* 其中\<items\>是用分号或空格分隔的项目列表
* 在每次迭代开始时，变量\<loop_var\>将被设置为当前项的值。
* \<loop_var\>的作用域仅限于循环作用域。

#### [8.1 简单循环](#)

```cmake
set(VS "kicker;god;need;mike")

foreach(word ${VS})
    message(STATUS "word: ${word}")
endforeach()
#[[
-- word: kicker
-- word: god
-- word: need
-- word: mike
]]#
```

#### [8.2 RANGE 数值迭代](#)
很简单的
```cmake
foreach(<loop_var> RANGE <start> <stop> [<step>])
```

```cmake
foreach(key RANGE 4)
    message(STATUS "number: ${key}")
endforeach()
#[[
-- number: 0
-- number: 1
-- number: 2
-- number: 3
-- number: 4
]]#
```


```cmake
foreach(key RANGE 1 10 2)
    message(STATUS "number: ${key}")
endforeach()
#[[
-- number: 1
-- number: 3
-- number: 5
-- number: 7
-- number: 9
]]#
```

#### [8.3 IN List迭代](#)

```cmake
foreach(<loop_var> IN [LISTS [<lists>]] [ITEMS [<items>]])
```


```cmake
set(A 0;1)
set(B 2;3)
set(C "4;5")
set(D 6;7 8)
set(E "9;10")
foreach(X IN LISTS A B C D E)
    message(STATUS "X=${X}")
endforeach()
```

#### [8.4 迭代 ZIP_LISTS](#)

```cmake
list(APPEND English one two three four)
list(APPEND Bahasa satu dua tiga)

foreach(num IN ZIP_LISTS English Bahasa)
    message(STATUS "num_0=${num_0}, num_1=${num_1}")
endforeach()

#[[
-- num_0=one, num_1=satu
-- num_0=two, num_1=dua
-- num_0=three, num_1=tiga
-- num_0=four, num_1=
]]#
```
