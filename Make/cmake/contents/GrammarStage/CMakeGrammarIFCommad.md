

### [CMake 流程语句](#)
 **介绍**：CMake IF 判断语句和CMake策略初探！

-----

- [x] [1.  条件语句 IF](#1-条件语句-if)
- [x] [2. CMake策略](#2-cmake-策略)
- [x] [3. Cache变量覆盖策略](#2-cache变量覆盖策略)

-----

### [1.  条件语句 IF](#)
条件命令，类似于C语言的条件分支指令，CMake的if语句用于根据某个条件来控制编译或链接选项。它用于在当前目标或者某个依赖项目中检查一个特定的宏定义或者变量是否被定义，如果定义了，则在编译或链接选项中启用相应的功能。

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
endif()  # end block
```

**基本条件表达式**
```
if(<常量>)
if(<变量>)
```
* ON、YES、TRUE、Y 被视为真
* OFF、NO、FALSE、N、IGNORE、NOTFOUND、空字符串、以 -NOTFOUND 结尾的字符串被视为假。
* 如果是一个数字，将根据 C 语言的规则转换成 bool 值。
* 如果上述三种情况都不适用，那该条件表达式将被当作一个变量的名字。
    * 如果没有使用引号，那该变量的值会和为假的值对比，如果匹配上则为假，否则为真。如果其值是空字符串则为假。
    * **如果使用引号**
        * cmake 3.1 及以后，如果该字符串不匹配任何为真的值，**那该条件表达式为假**。
        * cmake 3.1 以前，如果该字符串匹配到任何存在的变量名字，则会按照变量处理。
    * `if(ENV{some_var})` 这种形式的条件表达式永远为假，所以不要使用环境变量，**环境变量总为假**
*  变量 如果未定义 被视为 假。纵观，综观

```cmake
set(val "NOTFOUND")
if (val)  #false
    MESSAGE("i found that")
else ()
    MESSAGE("i did not found that")
endif ()
#i did not found that

if ("cancel")  #false
    MESSAGE("i enter the if")  #没有进入 if
endif ()

option(Enable_Test "是否启动白盒测试" ON)

if (Enable_Test)
    MESSAGE("start test")
endif ()
#start test
```

**逻辑表达式:** **NOT**、**AND**、**OR**, 必须全部大写

```
if(NOT <condition>)

if(<cond1> AND <cond2>)

if(<cond1> OR <cond2>)

if((condition) AND (condition OR (condition)))
```

例子：

```cmake
option(Enable_Test "是否启动测试" ON)
option(Enable_Test_White "是否启动白盒测试" ON)

if (Enable_Test AND Enable_Test_White)
    MESSAGE("start test")
endif ()
```

#### [1.1 关系表达式](#)
可以理解为C++语言里面条件表达式: **==**、**<**、**<=**、**>**、**>=**。

CMake表达式有： EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, STREQUAL, STRLESS, STRLESS_EQUAL, STRGREATER, STRGREATER_EQUAL, MATCHES.

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


#### [1.2 版本对比](#)
专门用于对比软件版本，VERSION_EQUAL, VERSION_LESS, VERSION_LESS_EQUAL, VERSION_GREATER, VERSION_GREATER_EQUAL

使用例子：
```shell
if(<variable|string> VERSION_LESS <variable|string>)

if(<variable|string> VERSION_GREATER <variable|string>)

if(<variable|string> VERSION_EQUAL <variable|string>)

if(<variable|string> VERSION_LESS_EQUAL <variable|string>)

if(<variable|string> VERSION_GREATER_EQUAL <variable|string>)
```

```cmake
MESSAGE(NOTICE ${CMAKE_SYSTEM})      #Linux-5.19.0-38-generic
MESSAGE(NOTICE ${CMAKE_SYSTEM_NAME}) #Linux
MESSAGE(NOTICE ${UNIX})              #1
MESSAGE(NOTICE ${WIN32})             #
IF (CMAKE_SYSTEM_NAME MATCHES "Linux") #TRUE
    MESSAGE(NOTICE "the os is linux")
ENDIF()
```

#### [1.3 File Operations](#)
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

#### [1.4 判断是否存在](#)
使用IF可以判断是否定义了某个变量、表达式、策略、命令！

* IF(DEFINED my_names)  是否定义了普通变量 my_names
* IF(DEFINED CACHE{NETWORK_VERSION_LASTED})  是否定义了缓存变量 NETWORK_VERSION_LASTED
* IF(DEFINED ENV{user_name}) 是否定义了环境变量 user_name
* IF(EXISTS VAL) 判断文件路径是否存在
* IF(COMMAND name) 是否定义了某个变量
* IF(POLICY name)  是否定义了某个策略
* IF(TARGET name) 是否定义了某个目标
* IF(TEST name) 是否定义了某个测试


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

其他命令：
```cmake
if(COMMAND name)
if(POLICY name)
if(TARGET name)
if(TEST name)               # Available since CMake 3.4
if(value IN_LIST listVar)   # Available since CMake 3.3

if(DEFINED SOMEVAR)           # Checks for a CMake variable (regular or cache)
if(DEFINED CACHE{SOMEVAR})    # Checks for a CMake cache variable
if(DEFINED ENV{SOMEVAR})      # Checks for an environment variable
```

#### [1.5 正则表达式](#)
利用正则表达式来进行字符串匹配

```cmake
if(value MATCHES regex)
if("Hi from ${who}" MATCHES "Hi from (Fred|Barney).*")
        message("${CMAKE_MATCH_1} says hello")
endif()
```

### [2. CMake 策略](#)

在 CMake 中，**策略**（Policy）是一种用来管理 CMake 行为和语义变更的机制。它**定义了 CMake 如何处理源代码中的旧版本行为**，并提供一组规则来解释和弥补不兼容的更改。

**CMake 策略**主要用于处理以下两种情况：

1. **变量语义**：在 CMake 中用于表示某些状态或设置的变量，在新版本中可能会有不兼容的更改。策略可用于规定 CMake 对旧版本语义的处理方式，以保证后续版本之间的兼容性。例如，CMP0075 策略指定了在引用未定义的变量时的行为，CMP0042 策略指定了项目名称的可选字符集。
2. **模块语义**：CMake 中的模块可能会更改或扩展默认的 CMake 行为。某些更改可能会破坏现有项目的构建过程。策略可用于管理模块的行为，以确保 Cisco 或重载模块函数不会破坏现有项目的构建过程。例如，CMP0054 策略规定了 FindPackageHandleStandardArgs 函数仅会处理 DEBUG_CONFIGURATIONS 变量的列表值。

[策略编号查询](http://cmake.org.cn/manual/cmake-policies.7.html)



#### 2.1 set_policy和make_policy

在 CMake 中，set_policy 命令用于设置策略（Policy）的行为方式，而 make_policy 命令用于强制使用给定的策略。

set_policy中POLICY 选项指定策略的名称或编号；NEW 和 OLD 选项指定新旧版本行为的处理方式。如果 NEW 和 OLD 选项都没有指定，则默认使用新版本行为，否则可能会导致一些不兼容性问题。

POLICY 选项指定要强制执行的策略名称或编号；VERSION 选项可用于指定要求 CMake 最低兼容版本的版本号。如果未指定 VERSION，则该策略将在当前和后续版本中生效

需要注意的是，set_policy 和 make_policy 命令本质上是一种针对后向兼容性问题的解决方案，旨在确保项目在升级到新版本的 CMake 时可以继续工作。因此，在使用这两个命令时，应该考虑使用已知行为的 API 和变量以规避可能的问题。

#### 2.2 set_policy

set_policy 是 CMake 用于设置 Policy 行为的命令。Policy 是 CMake 中管理行为和语义变更的机制，用于解释和弥补不兼容的更改，实现向后兼容性。使用 set_policy 命令可以设置策略的行为方式。

其基本语法如下：

```
set_policy(POLICY policy_id [NEW behavior] [OLD behavior])
```

其中，POLICY 选项指定要设置的 Policy 的名称或编号，NEW 和 OLD 选项用于指定新旧版本的处理方式。

如果 NEW 和 OLD 选项都没有指定，则默认使用 NEW 版本行为；否则，可能会导致一些不兼容性问题。开发者在编写 CMakeLists.txt 文件时，应该考虑策略的影响，并尽可能使用已知行为的 API 和变量。

例如，以下命令将 CMP0022 Policy 的行为强制设置为旧版本（OLD）：

```
set_policy(CMP0022 OLD)
```

当然，也可以仅指定 NEW 或 OLD 中的一个，例如：

```
set_policy(CMP0043 NEW)
```

在执行 CMake 时，可以使用 -D 选项将某些 Policy 值设置为 NEW 或 OLD，例如：

```
cmake -D policy:CMP0022=OLD /path/to/source
```

需要注意的是，CMake Policy 主要用于向后兼容性问题，在编写 CMakeLists.txt 文件时应该尽可能遵循 CMake 的最佳实践和编程规范，以减少构建过程中的不兼容问题。

#### [2.3 make_policy](#)

CMake Policy（策略）是一种用来管理 CMake 行为和语义变更的机制。它定义了 CMake 如何处理源代码中的旧版本行为，并提供一组规则来解释和弥补不兼容的更改，从而实现向后兼容性。

在 CMake 3.0 及以上版本中，Policy 已经成为 CMake 的核心设计之一，并且可以通过 set_policy 命令来设置其行为方式。该命令的语法为：

```
set_policy(POLICY policy_id [NEW behavior] [OLD behavior])
```

其中，POLICY 选项指定策略的名称或编号，NEW 和 OLD 选项用于指定新旧版本行为的处理方式。

如果 NEW 和 OLD 选项都没有指定，则默认使用新版本行为；否则，可能会导致一些不兼容性问题。开发者在编写 CMakeLists.txt 文件时，应该考虑策略的影响，并尽可能使用已知行为的 API 和变量。

在执行 CMake 时，可以使用 -D 选项将某些 Policy 值设置为 NEW 或 OLD，例如：

```
cmake -D policy:CMP0022=OLD /path/to/source
```

另外，make_policy 命令也可以用于强制使用给定的策略，其语法为：

```
make_policy(POLICY policy_id [VERSION version_number])
```

其中，POLICY 选项指定要强制执行的策略名称或编号；VERSION 选项可用于指定要求 CMake 最低兼容版本的版本号。如果未指定 VERSION，则该策略将在当前和后续版本中生效。

需要注意的是，CMake Policy 主要用于向后兼容性问题，在编写 CMakeLists.txt 文件时应该尽可能遵循 CMake 的最佳实践和编程规范，以减少构建过程中的不兼容问题。

```cmake
if(POLICY CMP0990)
  cmake_policy(SET CMP0990 NEW)
endif()
```

### [3. Cache变量覆盖策略](#) 
如何cache变量和普通变量重名了，将会在当前范围内删除任何同名的普通变量。设置策略后，将不会删除。 3.21版本之后支持！
```cmake
cmake_policy(SET CMP0126 NEW)
```

访问cache需要使用 **$CACHE{varible_name}**

```cmake
set(CODING_PRO_NAME  "Redmi Note 12 Pro")
set(CODING_PRO_NAME "Xiao Mi 13 Pro" CACHE STRING "phone")

message(${CODING_PRO_NAME}) #Redmi Note 12 Pro
message( $CACHE{CODING_PRO_NAME})  #Xiao Mi 13 Pro
```