### [CMake 常用功能 Message and include](#)
 **介绍**： 前者用来打印消息，或者记录日志消息，后者用于运行 **.cmake** 脚本。

-----

- [x] [1.  MESSAGE](#1-message)
- [x] [2. MESSAGE 模块查找](#2-message-模块查找)
- [x] [3.  与message 相关的变量](#3-与message-相关的变量)
- [x] [4. 控制message的样式](#4-控制message的样式)
- [x] [5. include](#5-include)

-----

### [1. MESSAGE](#)
因为一个大型项目的CMakeLists.txt里面的内容会特别多，所以就需要注释和日志消息，还可以利用 `cmake` 脚本实现内容拆分。

#### [1.1 message 指令](#)
在日志中记录指定的消息文本。如果给定了多个消息字符串，则它们被连接到一个消息中，字符串之间没有分隔符。

```cmake
message([<mode>] "message text" ...) # General messages
message(<checkState> "message text" ...) # Reporting checks
```
可选的**mode**关键字决定消息的**类型/级别**，这会影响消息的处理方式：
```cmake
set(USER_INFORMATION_CXX "这是一个好消息！")
message(WARNING "waning： ${USER_INFORMATION_CXX}")
```

```cmake
#不指定消息日志级别
message("start generator:")
#单行消息
message(STATUS "variable.cmake: ${CMAKE_CURRENT_SOURCE_DIR}")

#多行消息
message(NOTICE [[
    first line
    second line
    third line
]])
```

#### [1.2  消息的类型 ](#)

CMake 的日志级别主要包括以下几种, 这些日志级别可以通过 CMake 的`CMAKE_MESSAGE_LOG_LEVEL`变量进行控制，默认级别为`STATUS`。在 CMakeLists.txt 文件中，我们可以使用`message`命令输出指定级别的日志信息，如下所示：

* **FATAL_ERROR** ： CMake 出错，停止编译和生成(信息红色) - 消息输出到 stderr 2
* **SEND_ERROR**： CMake 错误，继续处理，但跳过生成 - 消息输出到 stderr 2
* **WARNING**： CMake 警告，打印代码路径和行号，继续编译(信息红色)  - 消息输出到 stderr 2
* **AUTHOR_WARNING** ： 开发者警告，继续编译(信息红色) 
* **(none) or NOTICE** ： 不设置mode，默认是NOTICE模式，不影响编译和生成，用于打印消息(信息白色)  消息输出到 stderr 2
* **STATUS** ： 编译时状态信息，左边以 `–` 和空格前缀开头(信息白色)
* **VERBOSE**： 针对项目用户的详细信息
* **DEBUG** ： 针对 项目本身的开发人员而不是只想构建项目的用户的详细信息消息。这些消息通常不会引起构建项目的其他用户的兴趣，并且通常与内部实现细节密切相关。
* **TRACE**： 非常低级实现细节的细粒度消息，开发者消息



```cmake
# 输出状态信息 
message(STATUS "Configuring MyProject...")
 
# 输出警告信息
message(WARNING "Possible undefined behavior detected!")
 
# 输出作者警告信息
message(AUTHOR_WARNING "Please ensure that the following dependencies are installed...")
 
# 输出错误信息
message(SEND_ERROR "The library is missing or incompatible!")
 
# 输出致命错误信息
message(FATAL_ERROR "Could not find required header files!")
```

#### [1.3 设置显示的日志级别](#)
默认有些消息是不打印的，需要设置消息级别才能打印出来， **--log-level命令行选项可用于控制显示哪些消息**。若不指定，默认不会显示verbose, debug, trace消息。也可通过CMAKE_MESSAGE_LOG_LEVEL变量设置。

```shell
cmake -S . -B build  --log-level=VERBOSE
```

#### [1.4 设置日志输出地方](#)
这个方法很简单，只需要用一个重定位符号即可，下面命名只是将标准输出消息输出到了文件中，并没有几率错误输出消息：

```cpp
 cmake -S . -B build  --log-level=VERBOSE > cmake.log
```
将错误输出也输入到文件中, linux 系统中标准输出为 `stdout` 也叫1 ，错误输出为 `stderr` 也叫 2。

```shell
cmake -S . -B build  --log-level=VERBOSE > cmake.log 2>&1
```

#### [1.5 设置消息缩进](#)
通过变量 CMAKE_MESSAGE_INDENT 来设置缩进：
```cmake
set(CMAKE_MESSAGE_INDENT "-- ")
```


### [2. MESSAGE 模块查找](#) 
可以使用check 查找模块库:

* *CHECK_PASS*  : 记录检查失败结果
* CHECK_FAIL : 记录检查的成功结果
* CHECK_START : 开始记录将要执行检查的消息


```cmake
set(CMAKE_MESSAGE_INDENT "-- ")
message(CHECK_START "find openssl")
#
# 查找 代码
message(CHECK_PASS "success")
#
message(CHECK_FAIL "find openssl filed")
set(CMAKE_MESSAGE_INDENT "")  #取消缩颈
```

### [3. 与message 相关的变量](#)
目前只有CMAKE_MESSAGE_INDENT用法清晰且功能正常。

#### [3.1 CMAKE_MESSAGE_LOG_LEVEL](#)
**CMAKE_MESSAGE_LOG_LEVEL** 是 CMake 中用于控制 CMake 生成的日志输出级别的变量。

当我们使用 CMake 构建项目时，CMake 会输出一定量的信息和警告，这些信息和警告通常包括编译器选项、链接器选项、源文件列表等，以帮助开发者更好地理解和调试运行中的 CMake。而CMAKE_MESSAGE_LOG_LEVEL变量可以帮助我们控制 CMake 输出的日志信息的级别和数量。

该变量有以下可选值：

- STATUS：输出状态信息，默认级别；
- WARNING：输出警告信息；
- AUTHOR_WARNING：输出作者警告信息；
- SEND_ERROR：输出错误信息，并终止 CMake 运行；
- FATAL_ERROR：输出致命错误信息，并终止 CMake 运行。

通过设置`CMAKE_MESSAGE_LOG_LEVEL`变量来控制日志输出级别，我们可以在构建过程中过滤多余的信息，并将注意力集中在最重要的信息上，从而提高开发效率和代码质量。

举例来说，如果我们想要屏蔽 `WARNING` 级别的日志信息，可以在 CMakeLists.txt 文件中添加如下语句：

```
set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")
```

这样，CMake 就会忽略所有 `WARNING` 级别的日志信息，仅输出 `STATUS` 级别及更高级别的信息。

#### [3.2 CMAKE_MESSAGE_INDENT](#)
设置缩进符号

```cpp
set(CMAKE_MESSAGE_INDENT "-- ")
```


#### [3.3 CMAKE_MESSAGE_CONTEXT](#)
**CMAKE_MESSAGE_CONTEXT** 是 CMake 中用于设置日志信息上下文的变量，可以帮助我们更清晰地了解日志信息的来源和环境。

当 CMake 输出日志信息时，除了输出级别和具体信息外，还会包括一些额外的信息，如日志信息的来源、行号、文件名等。而`CMAKE_MESSAGE_CONTEXT`变量可以帮助我们定制这些额外的信息，以便更好地定位问题和跟踪日志信息。

该变量通常在 CMake 生成日志信息时设置，可以接受多个值，例如：

```
set(CMAKE_MESSAGE_CONTEXT "MyProject" "CMakeLists.txt")
```

这样，当 CMake 输出日志信息时，就会在每条日志信息的前面添加上`[MyProject][CMakeLists.txt]`的标识，表示该日志信息来自于 MyProject 项目的 CMakeLists.txt 文件。

通过设置`CMAKE_MESSAGE_CONTEXT`变量，我们可以更好地了解日志信息的来源和环境，并快速定位和解决问题，提高代码质量和开发效率。



### [4. 控制message的样式](#)
我们可以修改输出的样式：

```cmake
string(ASCII 27 Esc)  #首先我们需要 ASCII 码
#ESC
set(R "${Esc}[0;31m")
set(E "${Esc}[m")

# 输出状态信息
message(NOTICE "message output show color...")

message(STATUS "${R}红色内容${E} 默认颜色")
```

设置格式：`\033[1:31:40m `  高亮显示， 前景色为红色，背景色为 黑色。

**显示方式**：

| 值   | 默认终端设置 |
| ---- | ------------ |
| 0 | 默认终端设置|
| 1 |高亮显示 |
| 4 |使用下划线|
| 5 |闪烁|
| 7 |反白显示|
| 8 |不可见|

**颜色**  

|前景色|背景色|颜色|
| ---- | ---- | ---- |
|30|40|黑色|
|31|41|红色|
|32|42|绿色|
|33|43|黄色|
|34|44|蓝色|
|35|45|紫红色|
|36|46|青蓝色|
|37|47|白色|

```cmake
string(ASCII 27 Esc)
#ESC
set(R "${Esc}[1;34;40m") #蓝色文字
set(E "${Esc}[m")

# 输出状态信息
message(NOTICE "message output show color...")

message(STATUS "${R}这是一段日志消息，你需要非常的重视${E} 默认颜色")
```

### [5. include](#)

从文件或模块加载并运行CMake代码，类似于C++的 **#include**。


#### [5.1 include和include_guard](#)
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

#### [5.2 include_guard](#)
为CMake当前正在处理的文件提供包含保护，可选参数指定保护的范围：

```cpp
include_guard([DIRECTORY|GLOBAL])
```
* DIRECTORY：include guard适用于当前目录及以下子目录。该文件只会在该目录范围内包含一次，但可能会被该目录以外的其他文件再次包含（即父目录或其他目录，而不是由当前文件或其子目录中的add_subdirectory（）或include（）拉入）。
* GLOBAL：include guard适用于整个构建。无论范围如何，当前文件只包含一次。

```cpp
include_guard(GLOBAL)

#当前cmake文件only包含一次！
```

