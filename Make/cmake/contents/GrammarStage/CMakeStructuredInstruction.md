### [CMake 结构化指令](#)
**介绍**：条件命令、循环命令、迭代命令、list、函数、字符串。

----
- [1. CMake 数学运算](#1-cmake-数学运算)
- [2. CMake 列表 list](#2-cmake-列表-list)
- [3. CMake 字符串 string](#3-cmake-字符串-string)
- [4. CMake function](#4-cmake-function)
- [5. CMake while](#5-cmake-while)
- [6. CMake foreach](#6-cmake-foreach)


----
### [1. CMake 数学运算](#)
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