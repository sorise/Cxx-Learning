## [CMake 指令](#)
**介绍**: CMake命令有两种，其中一种是编写命令脚本包括有条件命令、循环命令，还有一种是项目命令包括编译命令、库命令。

----

- [1. CMake 常用命令脚本](#1-cmake-常用命令脚本)
- [2. CMake 项目命令](#2-cmake-项目命令)
- [3. 库相关命令](#3-库相关命令)
- [4. 编译相关指令](#4-编译相关指令)

----

### [1. CMake 常用脚本命令](#)
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


### [2. CMake 常用项目命令](#)
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
|**17. macro**|CMake的宏！和C++差不多！|
|**18. include和include_guard**|从文件或模块加载并运行CMake代码|


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

### [3. 库相关命令](#)
这些命令仅在CMake项目中可用。

|命令|解释|
|:----|:----|
|**1. add_dependencies**|添加顶级目标之间的依赖关系。|
|**2. add_custom_target**|添加一个没有输出的目标，这样它将始终被构建。|
|**3. add_custom_command**|将自定义构建规则添加到生成的构建系统。|

#### [3.1 add_dependencies](#)
添加顶级目标之间的依赖关系，例如Chart库依赖于 Drawding 库和 GUI 库，那么就需要先编译Drawding和GUI库，然后再编译生成Chart库，这样可以保证正确的构建关系。
目标可以是可执行目标、静态、动态库。
```cmake
add_dependencies(<target> [<target-dependency>]...)
```
使用例子：
```cmake
add_dependencies(Chart Drawding GUI)
```
这个命令是直接定义依赖关系，但是通过**target_link_libraries** 可以定义依赖关系：

```cpp
target_link_libraries(Chart
    PRIVATE
    Drawding
    GUI
)
```
下面的命令也隐式定义了依赖关系！

#### [3.2 add_custom_target](#)
比较难的一个指令！

```cmake
add_custom_target(Name [ALL] [command1 [args1...]]
    [COMMAND command2 [args2...] ...]
    [DEPENDS depend depend depend ... ]
    [BYPRODUCTS [files...]]
    [WORKING_DIRECTORY dir]
    [COMMENT comment]
    [JOB_POOL job_pool]
    [VERBATIM] [USES_TERMINAL]
    [COMMAND_EXPAND_LISTS]
    [SOURCES src1 [src2...]])
```

* **ALL**： 表明该目标会被添加到默认的构建目标，使得它每次都被运行；
* **COMMAND**： 指定要在构建时执行的命令行；
* **DEPENDS**： 指定命令所依赖的文件；
* **COMMENT**： 在构建时执行命令之前显示给定消息；
* **WORKING_DIRECTORY**：   使用给定的当前工作目录执行命令。如果它是相对路径，它将相对于对应于当前源目录的构建树目录；
* **BYPRODUCTS**：  指定命令预期产生的文件。

#### [3.3 add_custom_command](#)

```cmake
add_custom_command(OUTPUT output1 [output2 ...]
    COMMAND command1 [ARGS] [args1...]
    [COMMAND command2 [ARGS] [args2...] ...]
    [MAIN_DEPENDENCY depend]
    [DEPENDS [depends...]]
    [BYPRODUCTS [files...]]
    [IMPLICIT_DEPENDS <lang1> depend1
                    [<lang2> depend2] ...]
    [WORKING_DIRECTORY dir]
    [COMMENT comment]
    [DEPFILE depfile]
    [JOB_POOL job_pool]
    [VERBATIM] [APPEND] [USES_TERMINAL]
    [COMMAND_EXPAND_LISTS])
```


### [4. 编译相关指令](#)
与编译阶段相关的命令的cmake命令。对于不同的编译器有自己相关的设置和优化选项，需要查询对应的编译器文档进行设置！

|命令|解释|
|:----|:----|
|**1. target_compile_definitions**| 转 [2.15](#215-target_compile_definitions)|
|**2. target_compile_features**|向目标添加所需的编译器功能。|
|**3. target_compile_options**|向目标添加所需的编译器功能。|
|**4. target_include_directories**|添加头文件搜索目录|
|**5. target_link_directories**|将链接目录添加到目标。|
|**6. target_link_libraries**|转[2.5](#25-target_link_libraries)|
|**7. target_link_options**|...|
|**8. target_precompile_headers**|...|
|**9. target_sources**|...|

#### [4.2 target_compile_features](#)
添加预期的编译器功能(compiler features), 如果该功能(feature)未在 **CMAKE_C_COMPILE_FEATURES**，**CMAKE_CUDA_COMPILE_FEATURES**或**CMAKE_CXX_COMPILE_FEATURES**变量中列出，则CMake将报告错误。

如果该功能的使用需要额外的编译器标志(compiler flag)，例如-std=gnu++11，则会自动添加该标志。

需要INTERFACE, PUBLIC和PRIVATE关键字来指定功能的作用域(scope of the features).
* PRIVATE和PUBLIC项将填充\<target\>的COMPILE_FEATURES属性。
* PUBLIC和INTERFACE项将填充\<target\>的INTERFACE_COMPILE_FEATURES属性。对相同的\<target\>重复调用会追加项(Repeated calls for the same \<target\> append items)。

```cmake
target_compile_features(<target> <PRIVATE|PUBLIC|INTERFACE> <feature> [item ...])
```
target_compile_features的参数 item 可以使用语法为$<...>的"生成器表达式"。


**c++ compile_features 参数：**
```cmake
foreach(feature ${CMAKE_CXX_COMPILE_FEATURES})
    MESSAGE(NOTICE ${feature})
endforeach()

target_compile_features(main PRIVATE cxx_std_11)
#[[
cxx_std_98
cxx_template_template_parameters
cxx_std_11
cxx_alias_templates
cxx_alignas
cxx_alignof
cxx_attributes
cxx_auto_type
cxx_constexpr
cxx_decltype
cxx_decltype_incomplete_return_types
cxx_default_function_template_args
cxx_defaulted_functions
cxx_defaulted_move_initializers
cxx_delegating_constructors
cxx_deleted_functions
cxx_enum_forward_declarations
cxx_explicit_conversions
cxx_extended_friend_declarations
cxx_extern_templates
cxx_final
cxx_func_identifier
cxx_generalized_initializers
cxx_inheriting_constructors
cxx_inline_namespaces
cxx_lambdas
cxx_local_type_template_args
cxx_long_long_type
cxx_noexcept
cxx_nonstatic_member_init
cxx_nullptr
cxx_override
cxx_range_for
cxx_raw_string_literals
cxx_reference_qualified_functions
cxx_right_angle_brackets
cxx_rvalue_references
cxx_sizeof_member
cxx_static_assert
cxx_strong_enums
cxx_thread_local
cxx_trailing_return_types
cxx_unicode_literals
cxx_uniform_initialization
cxx_unrestricted_unions
cxx_user_literals
cxx_variadic_macros
cxx_variadic_templates
cxx_std_14
cxx_aggregate_default_initializers
cxx_attribute_deprecated
cxx_binary_literals
cxx_contextual_conversions
cxx_decltype_auto
cxx_digit_separators
cxx_generic_lambdas
cxx_lambda_init_captures
cxx_relaxed_constexpr
cxx_return_type_deduction
cxx_variable_templates
cxx_std_17
cxx_std_20
cxx_std_23
]]#
```

#### [4.3 target_compile_options](#)
为源文件的编译添加选项!

语法:
```cmake
add_compile_options(<option> ...)
```

```cmake

add_executable(cmake_gcc_options_try_c main.c)
 
target_compile_options(cmake_gcc_options_try_c
                       PUBLIC -W -Wall -Wextra -pedantic -pedantic-errors)
```

#### [4.8 target_precompile_headers](#)
可以减少编译时间，对头文件进行预编译, 这样就可以不用包含头文件了，CMake会自动包含！

```cmake
target_precompile_headers(<target>
  <INTERFACE|PUBLIC|PRIVATE> [header1...]
  [<INTERFACE|PUBLIC|PRIVATE> [header2...] ...])
```

使用例子:
```cmake
add_executable(network main.cpp)
target_precompile_headers(network
        PUBLIC
        networkConfig.h
        <iostream>
)
```
main.cpp 就可以不用包含iostream和networkConfig.h就可以使用其成员。

```cmake
#include "toolkits.h"

int main(int args,  char * argv[]){
    std::cout << toolkits::Add<double>(3.14, 6.28) << std::endl;
    std::cout << toolkits::myAdd(5, 6)<<std::endl;
}
```


#### [4.9 target_sources](#)
为目标添加源文件：

```cmake
target_sources(<target>
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

小例子:
```cmake
add_library(toolkits SHARED toolkits.cpp)

target_sources(toolkits PUBLIC testSource.cpp)
```









