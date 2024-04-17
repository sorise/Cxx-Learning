### [高级用法 - CMake 生成器表达式](#)

**介绍**：CMake的生成器表达式不算是特别常用，但是有一些场景可能是必须要使用的；或者在针对不同编译类型设置不同编译参数的时候可以巧妙应用，从而减少配置代码。

----

- [1. 执行时机与作用](#1-执行时机与作用)
- [2. 常用的生成器表达式](#2-常用的生成器表达式)

----

### 1. 执行时机与作用

**执行 cmake 生成构建工程（脚本）时候分两个阶段**：

* 第一阶段是**配置阶段**，从顶层 **CMakeLists.txt 开始解析生成 CMakeCache.txt, CMakeFiles**，这步就像执行 bash 等脚本一样，逐行解析。**Configuring done 代表配置阶段结束**。

* 第二阶段是**生成阶段**，生成真正的**构建脚本**（ `.sln/Makefile/build.ninja` 等）。**"$<>"** 这种奇怪语法叫生成表达式 `(generator expression)` 它们在配置阶段会被逐字记录，在生成阶段才会被解析。`Generating done` 代表生成阶段结束。

[值得强调的是，生成表达式被展开是在生成构建系统的时候，所以不能通过解析配置CMakeLists.txt阶段的`MESSAGE()`命令打印，文末会介绍其调试方法。](#)



**它虽然比较不好理解，但是也有一些好处**：

1. **构建选项变化了配置阶段生成的东西几乎不变**，提高构建速度
2. **生成阶段有全局视角，不依赖于cmake文件中语句的顺序**（下面举个例子，复杂点的工程很容易碰到类似需求）



如果你选择GCC作为编译器会引入头文件库： `/opt/include/GNU`, 如果使用Clange会引入头文件库 `/opt/include/Clang`, 或者MSVC....

```cmake
target_include_directories(tgt PRIVATE /opt/include/$<CXX_COMPILER_ID>)
```

#### 1.1 作用解释

许多目标属性的上下文中允许使用[生成器表达式](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7))，例如 [LINK_LIBRARIES](https://cmake.org/cmake/help/latest/prop_tgt/LINK_LIBRARIES.html#prop_tgt:LINK_LIBRARIES)、[INCLUDE_DIRECTORIES](https://cmake.org/cmake/help/latest/prop_tgt/INCLUDE_DIRECTORIES.html#prop_tgt:INCLUDE_DIRECTORIES)、[COMPILE_DEFINITIONS](https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_DEFINITIONS.html#prop_tgt:COMPILE_DEFINITIONS) 等。它们也可以在使用命令填充这些属性时使用，例如 [target_link_libraries()](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries)、[target_include_directories()](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories)、[target_compile_definitions()](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions) 等。

[生成器表达式](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7))可用于启用条件链接、编译时使用的条件定义、条件包含目录等。这些条件可能基于构建配置、目标属性、平台信息或任何其他可查询信息。

有不同类型的[生成器表达式](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7))，包括逻辑表达式、信息表达式和输出表达式。

逻辑表达式用于创建条件输出。基本表达式是 `0` 和 `1` 表达式。`$<0:...>` 生成空字符串，`<1:...>` 生成 `...` 的内容。它们也可以嵌套。



### 2. 常用的生成器表达式

#### 2.1 逻辑运算符

逻辑运算很多语言都是需要的，CMake生成器表达式中有这些：

1. `$<BOOL:string>`：如果字符串为空、`0`；不区分大小写的`FALSE`、`OFF`、`N`、`NO`、`IGNORE`、`NOTFOUND`；或者区分大小写以`-NOTFOUND`结尾的字符串，则为0，否则为1
2. `$<AND:conditions>`：逻辑与，`conditons`是以逗号分割的条件列表
3. `$<OR:conditions>`：逻辑或，`conditons`是以逗号分割的条件列表
4. `$<NOT:condition>`：逻辑非

> 一般来说，条件是列表的，都是使用逗号进行分割，后面不再赘述。



#### 2.2 条件表达式

这便是本文的核心了，主要有两个格式：

1. `$<condition:true_string>`：如果条件为真，则结果为`true_string`，否则为空
2. `$<IF:condition,str1,str2>`：如果条件为真，则结果为`str1`，否则为`str2`

这里的条件一般情况下就是前面介绍的**布尔生成器表达式**。 比如要根据编译类型指定不同的编译选项，可以像下面这样：

```cmake
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
```

但是使用生成器表达式可以简化成：

```cmake
add_compile_options("$<$<CONFIG:Debug>:-g;-O0>")
add_compile_options($<$<CONFIG:Release>:-O2>)
```



#### 2.3 字符串比较

1. `$<STREQUAL:string1,string2>`：判断字符串是否相等
2. `$<EQUAL:value1,value2>`：判断数值是否相等
3. `$<IN_LIST:string,list>`：判断`string`是否包含在`list`中，`list`使用分号分割

> 注意这里的`list`是在逗号后面的列表，所以其内容需要使用分号分割。

####  2.4 变量查询

这个会是比较常用的，在实际使用的时候会根据不同CMake内置变量生成不同配置，核心就在于“判断”：

1. `$<TARGET_EXISTS:target>`：判断目标是否存在
2. `$<CONFIG:cfgs>`：判断编译类型配置是否包含在`cfgs`列表（比如"release,debug"）中；不区分大小写
3. `$<PLATFORM_ID:platform_ids>`：判断CMake定义的平台ID是否包含在`platform_ids`列表中
4. `$<COMPILE_LANGUAGE:languages>`：判断编译语言是否包含在`languages`列表中

#### 2.5 字符串值生成器表达式

请注意，前面都是铺垫，这里才是使用生成器表达式的主要目的：**生成特定的字符串**。 比如官方的例子：基于编译器ID指定include目录：

```text
include_directories(/usr/include/$<CXX_COMPILER_ID>/)
```

根据编译器的类型，`$<CXX_COMPILER_ID>`会被替换成对应的ID（比如“GNU”、“Clang”）。

#### 2.6 转义字符

这比较好理解，因为有一些字符有特殊含义，所以可能需要转义，比如常用的`$<COMMA>`和`$<SEMICOLON>`，分别表示`,`和`;`。

#### 2.7 字符串操作

常用的有`$<LOWER_CASE:string>`、`$<UPPER_CASE:string>`用于转换大小写。



#### 获取变量值

获取变量的值和前文提到的变量查询很类似，前面说的变量查询是判断是否存在于指定列表中或者等于指定值。语法格式是类似的，以`CONFIG`为例：

1. 获取变量值：`$<CONFIG>`
2. 判断是否存在于列表中：`$<CONFIG:cfgs>`



### 应用例子：



#### CMake使用生成器表达式（Generator Expression）添加编译和链接选项

```cmake
target_compile_options(${PROJECT_NAME}
    PRIVATE
        # Enable All Warnings
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /sdl>
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
        $<$<CXX_COMPILER_ID:Clang,AppleClang>:
            -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic>
 
    PRIVATE
        # Enable Optimizations
        $<$<CXX_COMPILER_ID:MSVC>:
            $<$<CONFIG:Debug>:/Od /GS /RTC1>
            $<$<CONFIG:Release>:/O2 /Oi /Ot /GL /Gy>
        >
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
            $<$<CONFIG:Debug>:-O0>
            $<$<CONFIG:Release>:-O3 -march=native>
        >
 
    PRIVATE
        # Enable Address Sanitizer
        $<$<CXX_COMPILER_ID:GNU,Clang>:
            $<$<VERSION_GREATER:${CMAKE_VERSION},3.12>:
                $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
                $<$<CONFIG:RelWithDebInfo>:-fsanitize=address -fno-omit-frame-pointer>
            >
        >
)
 
if (${CMAKE_VERSION} VERSION_GREATER 3.12)
    target_link_options(${PROJECT_NAME}
        PRIVATE
            # Enable Address Sanitizer
            $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
                $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
                $<$<CONFIG:RelWithDebInfo>:-fsanitize=address -fno-omit-frame-pointer>
            >
    )
endif()
```

