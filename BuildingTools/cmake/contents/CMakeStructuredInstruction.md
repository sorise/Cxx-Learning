### [CMake 结构化指令](#)
**介绍**：条件命令、循环命令、迭代命令、list、函数、字符串。

----
- [1. CMake 条件表达式 if](#1-cmake-条件表达式-if)
- [2. CMake 列表 list](#2-cmake-列表-list)
- [3. CMake 字符串 string](#3-cmake-字符串-string)
- [4. CMake function](#4-cmake-function)
- [5. CMake while](#5-cmake-while)
- [6. CMake foreach](#6-cmake-foreach)
- [7. CMake 数学运算](#7-cmake-数学运算)

----

### [1. CMake 条件表达式 if](#)
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

**基本条件表达式**
```
if(value)
```
* ON、YES、TRUE、Y 被视为真
* OFF、NO、FALSE、N、IGNORE、NOTFOUND、空字符串、以 -NOTFOUND 结尾的字符串被视为假。
* 如果是一个数字，将根据 C 语言的规则转换成 bool 值。
* 如果上述三种情况都不适用，那该条件表达式将被当作一个变量的名字。
    * 如果没有使用引号，那该变量的值会和为假的值对比，如果匹配上则为假，否则为真。如果其值是空字符串则为假。
    * 如果使用引号
        * cmake 3.1 及以后，如果该字符串不匹配任何为真的值，那该条件表达式为假。
        * cmake 3.1 以前，如果该字符串匹配到任何存在的变量名字，则会按照变量处理。
    * `if(ENV{some_var})` 这种形式的条件表达式永远为假，所以不要使用环境变量。


**逻辑表达式:** **NOT**、**AND**、**OR**
```shell
if(NOT <condition>)

if(<cond1> AND <cond2>)

if(<cond1> OR <cond2>)

if((condition) AND (condition OR (condition)))
```

#### [1.1 关系表达式](#)
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

#### [1.2 版本对比](#)

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
正则表达式:

```cmake
if(value MATCHES regex)
```

```cmake
if("Hi from ${who}" MATCHES "Hi from (Fred|Barney).*")
		message("${CMAKE_MATCH_1} says hello")
endif()
```

### [2. CMake 列表 list](#)
CMake的list命令支持需多的操作：读取、搜索、修改、排序！

```cmake
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
  list(REMOVE_ITEM <list> <value>...) #根据值移除元素
  list(REMOVE_AT <list> <index>...) #根据下表移除
  list(REMOVE_DUPLICATES <list>)   #移除重复元素
  list(TRANSFORM <list> <ACTION> [...])

Ordering
  list(REVERSE <list>)
  list(SORT <list> [...])
```

#### [2.1 创建列表](#)
cmake创建列表有两种方式，带引号`"`或者不带引号，如果带引号，就需要`;`分割符。
不带引号，直接分离即可。

**带引号**: 最后一位后面不需要加分割符！
```cmake
set(names "remix;kicker;tome;killer")  #是一个列表
set(names_no_split "remix kicker tome killer")  #不是一个列表
set(special "[[remix;kicker;tome;killer]]") #special 是一个字符创 内容："remix;kicker;tome;killer"
```
**不带引号**: 这就很简单了！
```cmake
set(names remix kicker tome killer)
set(numbers 1;2;3;4;5)

list(LENGTH names len)
list(LENGTH numbers number_len)
message(NOTICE "names length: ${len}")  #4
message(NOTICE "numbers length: ${number_len}") #5
```

#### [2.2 获得列表长度](#)
获得长度很简单的使用 `LENGTH` 操作即可！

```cmake
set(names remix kicker tome killer)

list(LENGTH names len)

message(NOTICE "names length: ${len}")  #4
```

#### [2.3 获取列表中某个元素](#)
通过索引可以获取某个元素，或者获取某一些元素！

```cmake
set(names remix kicker tome killer)

list(GET names 2 element)    #获得一个元素
list(GET names 1 2 elements) #获得两个元素

message(STATUS "element: ${element}") #tome
message(STATUS "elements: ${elements}") #kicker;tome
```

#### [2.4 insert插入](#)
看看就懂了！

```cmake
set(names remix kicker tome killer)

list(INSERT names 2 "miner" )

foreach (element ${names})
    message(NOTICE "${element}")
endforeach ()
#[[
remix
kicker
miner
tome
killer
]]#
```

#### [2.5 查找](#)

```cmake
set(names remix kicker tome killer)
list(FIND names "tome" result)
message(NOTICE "${result}") #3
```

#### [2.6 排序](#)

```cmake
list(SORT names ORDER ASCENDING)
#升序排列
list(SORT names ORDER DESCENDING)
#降序排列
```

### [3. CMake 字符串 string](#)
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

#### 3.1 字符串查找
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

#### 3.2 替换字符串
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

#### 3.3 截取字符串
```cmake
string(SUBSTRING input index length outVar)
```
- 将 input 字符串从 index 处截取 length 长度放到 outVar 中。
- 如果 length 为 -1 的话，将从 index 到 input 结尾的字符串串保存到 outVar 中。

#### 3.4 常用操作
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

#### 3.5 HASH操作
支持对字符串进行HASH操作！
```cmake
string(SHA3_256 HASH_VS "kicker-god-need-mike")
message(STATUS ${HASH_VS})
#bb6818af3a708354d741934bf67034002c61dce6c8fb4182cfbcaf4555de7d25
```

#### [3.6 TIMESTAMP](#) 
获取时间辍 `  string(TIMESTAMP <out-var> [<format string>] [UTC])`  `format string` 查看[官方文档](https://cmake.org/cmake/help/latest/command/string.html#timestamp)

```cmake
string(TIMESTAMP CURRENT_DATETIME "%Y/%m/%d %H:%M:%S")
message(STATUS "running at: ${CURRENT_DATETIME}")
#running at: 2023/03/24 21:34:21
```

### [4. Cmake function](#)
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

### [5. Cmake while](#)
可能用得较少... ,支持**break()** 指令提前结束循环! 支持 **continue()** 直接下一轮!

```cmake
while(<condition>)
  <commands>
endwhile()
```

```cmake
set(num 10)

while(num GREATER 0)
    message(STATUS "current num = ${num}")
    math(EXPR num "${num} - 1")
endwhile()
```


### [6. Cmake foreach](#)
CMake 迭代器！支持**break()** 指令提前结束循环! 支持 **continue()** 直接下一轮!

```cmake
foreach(<loop_var> <items>)
  <commands>
endforeach()
```
* 其中\<items\>是用分号或空格分隔的项目列表
* 在每次迭代开始时，变量\<loop_var\>将被设置为当前项的值。
* \<loop_var\>的作用域仅限于循环作用域。

#### [6.1 简单循环](#)
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

#### [6.2 RANGE 数值迭代](#)
很简单的。
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

#### [6.3 IN List迭代](#)
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

**ITEMS 用法:**
```cmake
set(names "remix" "cicer"  "umix")
set(friend "mike")
FOREACH (val IN LISTS names ITEMS ${friend} minis)
    MESSAGE(NOTICE "item: ${val}")
ENDFOREACH ()
#[[
item: remix
item: cicer
item: umix
item: mike
item: minis]]
```

#### [6.4 迭代 ZIP_LISTS](#)
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

### [7. CMake 数学运算](#)
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