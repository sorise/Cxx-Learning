### [CMake 宏](#)

**介绍**:  在 CMake 中，宏指的是一段可以重复利用的代码片段，类似于函数，但它并不返回值。 宏可以定义在 CMakeLists.txt 文件中，使用起来比函数更加灵活。

----

### [1. 定义宏](#)

宏的定义格式如下，**宏名称的大小写不敏感！**

```cmake
macro(<macro_name> <arg1> <arg2> ... )
    # macro body
endmacro()
```

其中 `<macro_name>` 是宏的名称，`<arg1>`、`<arg2>` 是宏参数列表。定义好宏后，可以使用 `call` 命令来调用这个宏。下面是一个简单的示例：

```cmake
macro(my_macro arg1 arg2)
    message("Argument 1: ${arg1}")
    message("Argument 2: ${arg2}")
endmacro()

my_macro("hello" "world")
```

在这个示例中，我们定义了一个名为 `my_macro` 的宏，它接受两个参数 `arg1` 和 `arg2`，然后输出这两个参数的值。最后，我们调用了这个宏，并传递了 `"hello"` 和 `"world"` 作为参数。

除了参数的传递，宏还支持局部变量的定义。使用 `${variable}` 语法可以引用宏中定义的变量。下面是一个包含局部变量的宏示例：

```cmake
macro(my_macro_with_variable arg1 arg2)
    set(local_var "Local variable")
    message("Argument 1: ${arg1}")
    message("Argument 2: ${arg2}")
    message("${local_var}")
endmacro()

my_macro_with_variable("hello" "world")
```

在这个示例中，我们定义了一个名为 `my_macro_with_variable` 的宏，它接受两个参数 `arg1` 和 `arg2`。在宏的内部，我们定义了一个名为 `local_var` 的局部变量，并输出了这个局部变量的值。最后，我们调用了这个宏，并传递了 `"hello"` 和 `"world"` 作为参数。

总之，在 CMake 中，宏是一种非常方便的工具，可以帮助我们重复使用代码，并提高代码的可读性和灵活性。

**宏的参数并不是变量，这个和C++的宏函数参数是一样的，本质上也是文本替换**。



#### [1.1 宏的调用](#)

宏的定义例子

```
macro(foo)
  <commands>
endmacro()
```

能够使用如下的调用方式。

```
foo()
Foo()
FOO()
cmake_language(CALL foo)
```

通常，宏使用所有小写名称。

**New in version 3.18:** 使用 [cmake_language(CALL ...)](http://cmake.org.cn/command/cmake_language.html#command:cmake_language)  命令也可用于调用宏。



#### [1.2 位置参数传递](#)

位置参数是最常用的参数类型。它们以固定的顺序传递给宏，并按照声明时的顺序进行解析。在宏中，可以通过 `${ARGV}` 变量来获取所有位置参数的列表，或者通过 `${ARGC}` 变量来获取位置参数的个数。

指定参数类型与个数的参数传递方法, 宏my_macro必须传递两个参数。

```cmake
macro(my_macro arg1 arg2)
    message("Argument 1: ${ARGV}") #hello world
	message("Argument 1: ${arg1}") #hello
    message("Argument 2: ${arg2}") #world
endmacro()

my_macro("hello" "world")
```



#### [1.3 非固定参数传递](#)

你可以再宏定义的时候不指定参数的类型和个数，然后使用CMake的变量获得参数个数和参数数组

* **ARGN** 包含传入非位置参数的 list， 与ARGV不同的是并不是代表所有参数，而是指宏或者函数声明的参数之后的所有参数。
* **ARGC** 代表的是函数或者宏传递的参数个数。
* **ARGV** 所有参数组成的数组，包括位置参数！
  * 除了上述三个默认参数外，每个参数还可以使用 ARGVx 的形式引用，其中 x 代表的是参数的编号，例如： ARGV0 ARGV1 ARGV2

```cmake
macro(print_args arg1 arg2)
        message("Argument 1: ${arg1}") #Argument 1: hello
        message("Argument 2: ${arg2}") #Argument 2: world

        MESSAGE(${ARGN})  #extra1extra2
        MESSAGE(${ARGC})  # 4
        MESSAGE(${ARGV})  #helloworldextra1extra2

        foreach(word ${ARGN})
                message(argument "word: ${word}")  
        endforeach()
        #[[
        	argumentword: extra1
			argumentword: extra2
        ]]
endmacro()

print_args("hello" "world" "extra1" "extra2")
```

#### [1.4 ARGN](#)

在 CMake 中，`ARGN` 是一个特殊的变量，用于存储一个宏的所有位置参数中除了前面指定过的参数之外的其余参数。通常情况下，这个变量通常作为一个可变参数的容器来使用。

下面是一个简单的示例，展示如何使用 `ARGN` 变量：

```cmake
macro(print_args arg1 arg2)
    message("Argument 1: ${arg1}")
    message("Argument 2: ${arg2}")
    if(ARGN)
        message("Extra arguments: ${ARGN}")
    else()
        message("No extra arguments.")
    endif()
endmacro()

print_args("hello" "world" "extra1" "extra2")
```

在这个示例中，我们定义了一个名为 `print_args` 的宏，它接受两个位置参数 `arg1` 和 `arg2`，然后输出这些参数的值。如果有更多的位置参数，它们将会被存储到 `ARGN` 变量中，并在最后输出。

需要注意的是，当使用 `ARGN` 变量时，前面的位置参数和关键字参数必须显式地声明，否则它们将被认为是不正确的额外的参数。

下面是一个使用关键字参数和 `ARGN` 变量的示例：

```cmake
macro(print_args_with_keyword arg1 arg2)
    foreach(key_value IN LISTS ARGV)
        if(key_value MATCHES "^([a-zA-Z0-9_]+):(.+)$")
            set(${CMAKE_MATCH_1} "${CMAKE_MATCH_2}")
            message("Set keyword argument: ${CMAKE_MATCH_1}=${${CMAKE_MATCH_1}}")
        endif()
    endforeach()

    message("Argument 1: ${arg1}")
    message("Argument 2: ${arg2}")
    if(ARGN)
        message("Extra arguments: ${ARGN}")
    else()
        message("No extra arguments.")
    endif()
endmacro()

print_args_with_keyword("hello" arg3:"!" arg2:"world" "extra1", "extra2")
```

在这个示例中，我们定义了一个名为 `print_args_with_keyword` 的宏，它接受两个位置参数 `arg1` 和 `arg2`，以及两个关键字参数 `arg2` 和 `arg3`。所有额外的位置参数都将存储在 `ARGN` 变量中，并在最后输出。

总之，在 CMake 中，`ARGN` 变量是一个非常有用的工具，可以帮助我们编写更加灵活和可重用的宏。**但是如何具体从ARGN中解析出值需要借助到cmake_parse_arguments 命令，1.7将介绍其用法！**

#### [1.5 与函数连同的参数问题](#)

CMake 宏在使用 ARGN 的时候，需要特别注意，下面我们看一个容易出错的例子：

```cmake
# WARNING: This macro is misleading
macro(dangerous)
    # Which ARGN?
    foreach(arg IN LISTS ARGN)
        message("Argument: ${arg}")
    endforeach()
endmacro()

function(func)
    dangerous(1 2)
endfunction()

func(3)
```

cmake 执行配置的时候，上述代码输出为:

```
Argument: 3
```

是不是有点诧异，但是当我们回到宏的本质，宏就是一段可以直接粘贴到调用处的代码，当我们把它修改成下面这个样子，一切就明了了。

```cmake
function(func)
    # Now it is clear, ARGN here will use the arguments from func
    foreach(arg IN LISTS ARGN)
        message("Argument: ${arg}")
    endforeach()
endfunction()
```

如果非要用第一种形式，那建议使用函数替换宏，函数就不会有上述问题。

本讲我们主要认识了 CMake 函数和宏的命名参数、未命名参数和默认参数。下一讲我们继续学习 CMake 函数和宏的关键字参数。

#### 1.6  从宏返回值
宏可以像函数那样 return 特定变量，通过将它们作为参数传入，来指定要设置的变量的名称。唯一的区别是，在调用 set() 命令时不应该在宏中使用 PARENT_SCOPE 关键字，因为宏已经修改了调用者范围内的变量。

下面是一个例子：
```cmake
macro(func resultVar1 resultVar2)
    set(${resultVar1} "First result")
    set(${resultVar2} "Second result")
endmacro()
```

**在宏中使用 return() 命令一定要非常小心，因为宏相当于直接将宏主体粘贴到调用的地方**，所以 return() 命令在宏中使用产生的效果与在什么地方调用宏有很大的关系，我们通过一个例子来学习其行为：

```cmake
macro(inner)
    message("From inner")
    return() # Usually dangerous within a macro
    message("Never printed")
endmacro()

function(outer)
    message("From outer before calling inner")
    inner()
    message("Also never printed")
endfunction()

outer()
```

上述例子宏展开后等同于：
```cmake
function(outer)
    message("From outer before calling inner")
    
    # === Pasted macro body ===
    message("From inner")
    return()
    message("Never printed")
    # === End of macro body ===
    
    message("Also never printed")
endfunction()

outer()
```

所以，```message("Also never printed")``` 这条打印永远也不会打印。

上面的例子说明，我们在函数内部调用了宏，然后在宏的内部调用 return() 命令，我们实际上是从函数中返回了。这就是为什么在宏中使用 return() 命令要非常小心的原因。



#### 1.7 cmake_parse_arguments 属性参数

以上只是但还不足以让我们写出像 CMake 内置的一些命令那样功能强大的函数和宏，比如：

```cmake
target_link_libraries(targetName
    <PRIVATE|PUBLIC|INTERFACE> item1 [item2 ...]
    [<PRIVATE|PUBLIC|INTERFACE> item3 [item4 ...]]
    ...
)
```

对于 CMake 内置的 target_link_libraries() 命令，它的作用就是在链接阶段，将某个 target 依赖的 target 链接上。比如我们的一个可执行程序依赖一个库，这个时候就需要用到这个命令。

从这个命令（也可以叫做函数）的签名可以看出，它是有关键字参数的，这些关键字是：`<PRIVATE|PUBLIC|INTERFACE>`。

如果我们需要编写这样的函数或者宏，如果只是上一讲的知识是不够的，CMake 给我们提供了另一个内置的命令：`cmake_parse_arguments()`，在这个内置命令的帮助下，我们也能写出 `target_link_libraries()` 这样的函数或者宏。

```cmake
cmake_parse_arguments(<prefix> <options> <one_value_keywords>
                      <multi_value_keywords> <args>...)

cmake_parse_arguments(PARSE_ARGV <N> <prefix> <options>
                      <one_value_keywords> <multi_value_keywords>)
```

`cmake_parse_arguments()` 命令有两种形式，第一种形式所有的 CMake 版本都支持。

```cmake
# Needed only for CMake 3.4 and earlier
include(CMakeParseArguments)

cmake_parse_arguments(
    prefix
    valuelessKeywords singleValueKeywords multiValueKeywords
    argsToParse...
)
```

在 CMake 3.5 以前的版本，`cmake_parse_arguments()` 命令是 CMakeParseArguments 这个模块提供的命令，关于 CMake 模块，我们后面会讲，现在可以理解成他是一个可选的功能，如果我们要用其中的功能，就的先 include 它。

不过在 CMake 3.5 版本开始，`cmake_parse_arguments()` 命令已经是一个默认的命令，不需要再 `include(CMakeParseArguments)` 了。

第二种形式是在 CMake 3.7 版本中引入的，而且只有 CMake 函数可以使用，CMake 宏是不支持这种形式的。

```cmake
# Available with CMake 3.7 or later, do not use in macros
cmake_parse_arguments(
    PARSE_ARGV startIndex
    prefix
    valuelessKeywords singleValueKeywords multiValueKeywords
    multi_value_keywords
)
```

- prefix 前缀是为了防止参数命名冲突！
- valuelessKeywords
  - 这种关键字参数没有额外的参数，只是一个独立的关键字，在调用函数的时候，有这个关键字和没有这个关键字代表着两种不同的情况，功能类似于传递了一个 BOOL 类型的命名参数。
- singleValueKeywords
  - 这种关键字参数需要一个额外的参数，而且只能有一个。
- multiValueKeywords
  - 这种关键字参数后面可以有零个或者多个额外的参数。

以上三个关键字参数在命名方面，虽然官方没有约定，但建议都是用大写字母加下划线的形式。另外关键字参数的名字不要太长，否则可能带来意想不到的后果。

**使用一个小例子来说明它的用法:**

```cmake
macro(parse_test)
    set(CMAKE_MESSAGE_INDENT "-- ")
    message("ARGC: ${ARGC}")
    message("ARGN: ${ARGN}")

    cmake_parse_arguments(
            "PARSE"
            "Opt01;Opt02" #Option 可选值 valuelessKeywords
            "Name;Age" #单值  singleValueKeywords
            "Targets"  #多值  multiValueKeywords
             ${ARGN}    #待解析参数
    )
    MESSAGE( "PARSE_Opt01 : ${PARSE_Opt01}" )
    MESSAGE( "PARSE_Opt02 : ${PARSE_Opt02}" )
    MESSAGE( "PARSE_Name : ${PARSE_Name}" )
    MESSAGE( "PARSE_Age : ${PARSE_Age}" )
    MESSAGE( "PARSE_Targets : ${PARSE_Targets}" )

endmacro()

parse_test(Opt01 Opt02 Name "remix" Age 18 Targets binary json timer )

#[[
-- ARGC: 10
-- ARGN: Opt01;Opt02;Name;remix;Age;18;Targets;binary;json;timer
-- PARSE_Opt01 : TRUE 
-- PARSE_Opt02 : TRUE
-- PARSE_Name : remix
-- PARSE_Age : 18
-- PARSE_Targets : binary;json;timer
]]
```

* `<prefix>_KEYWORDS_MISSING_VALUES` 如果发生多值参数缺失，会指出缺失的参数
* `<prefix>_UNPARSED_ARGUMENTS` 内置变量说明是否传递了非解析变量 


### [2. Cmake function](#)
在CMake中定义函数，首个参数是函数名，随后的是参数！ Cmake函数和C++一样具有自己的作用域，**同时也支持 return()操作！**

重点理解一下参数的意义！怎么获得参数名，怎么获得参数值。

```cmake
function(<name> [<arg1> ...] [<outVar>])
  <commands>
  <return(PROPAGATE ${outVar})>
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

 CMake 函数和宏的主要区别。函数会创建一个新的作用域，而宏不会；函数内部定义或者修改变量对函数外部同名的变量没有影响，除非明确需要传播出去。宏和其调用者共享变量和作用域，因此在宏内部定义变量或者修改变量会影响到外部同名的变量。

在知道了这个前提后，我们继续学习本讲的内容，如何从函数和宏返回一个值。


#### 2.1 参数
* **ARGN** 包含传入非位置参数的 list， 与ARGV不同的是并不是代表所有参数，而是指宏或者函数声明的参数之后的所有参数。
* **ARGC** 代表的是函数或者宏传递的参数个数。
* **ARGV** 所有参数组成的数组，包括位置参数！
  * 除了上述三个默认参数外，每个参数还可以使用 ARGVx 的形式引用，其中 x 代表的是参数的编号，例如： ARGV0 ARGV1 ARGV2


#### 2.2 函数与cmake_parse_arguments
函数使用，然后继续学习 `cmake_parse_arguments()` 命令的使用。

```cmake
function(func)
    # Define the supported set of keywords
    set(prefix ARG)
    set(noValues ENABLE_NET COOL_STUFF)
    set(singleValues TARGET)
    set(multiValues SOURCES IMAGES)
    
    # Process the arguments passed in
    include(CMakeParseArguments)
    cmake_parse_arguments(
        ${prefix}
        "${noValues}" "${singleValues}" "${multiValues}"
        ${ARGN}
    )
    
    # Log details for each supported keyword
    message("Option summary:")
    
    foreach(arg IN LISTS noValues)
        if(${prefix}_${arg})
            message(" ${arg} enabled")
        else()
            message(" ${arg} disabled")
        endif()
    endforeach()
    
    foreach(arg IN LISTS singleValues multiValues)
        # Single argument values will print as a string
        # Multiple argument values will print as a list
        message(" ${arg} = ${${prefix}_${arg}}")
    endforeach()
endfunction()

# Examples of calling with different combinations
# of keyword arguments

func(SOURCES foo.cpp bar.cpp
    TARGET MyApp
    ENABLE_NET
)

func(COOL_STUFF
    TARGET dummy
    IMAGES here.png there.png gone.png
)
```

上面的例子的输出如下:

```cmake
Option summary:
    ENABLE_NET enabled
    COOL_STUFF disabled
    TARGET = MyApp
    SOURCES = foo.cpp;bar.cpp
    IMAGES =
Option summary:
    ENABLE_NET disabled
    COOL_STUFF enabled
    TARGET = dummy
    SOURCES =
    IMAGES = here.png;there.png;gone.png
```

上述例子，我们使用了 `cmake_parse_arguments()` 命令的第一种形式，当然因为这个例子是封装的一个 CMake 函数，自然也就可以使用 `cmake_parse_arguments()` 命令的第二种形式，如下:

```cmake
cmake_parse_arguments(
    PARSE_ARGV 0
    ${prefix}
    "${noValues}" "${singleValues}" "${multiValues}"
)
```

如果传入的参数通过 `valuelessKeywords singleValueKeywords multiValueKeywords` 三种关键字参数都不能解析，那就会保存在一个叫做 `<prefix>_UNPARSED_ARGUMENTS` 的变量中，这个变量是一个列表。

我们一起来看一个例子：

```cmake
function(demoArgs)
    set(noValues "")
    set(singleValues SPECIAL)
    set(multiValues EXTRAS)
    
    cmake_parse_arguments(
        PARSE_ARGV 0
        ARG
        "${noValues}" "${singleValues}" "${multiValues}"
    )
    
    message("Left-over args: ${ARG_UNPARSED_ARGUMENTS}")
    
    foreach(arg IN LISTS ARG_UNPARSED_ARGUMENTS)
        message("${arg}")
    endforeach()
endfunction()

demoArgs(burger fries "cheese;tomato" SPECIAL secretSauce)
```

上述例子输出如下：

```
Left-over args: burger;fries;cheese\;tomato
burger
fries
cheese;tomato
```

上面这个例子，我们特意传入了一个包含分号的参数 `"cheese;tomato"`，我们使用的是 `cmake_parse_arguments()` 命令的第二种形式去解析参数，所有它有效地识别出 `"cheese;tomato"` 是一个参数，而不是两个，如果使用 `cmake_parse_arguments()` 命令的第一种形式，就会不一样，大家不妨自己试一下。

singleValueKeywords 关键字参数需要一个额外的参数，如果在调用的时候没有给它传递额外的参数会发生什么呢？

从 CMake 3.15 版本开始，**如果没有给需要额外参数的关键字参数传递额外的参数**，那一个默认的变量：`<prefix>_KEYWORDS_MISSING_VALUES ` 将会保存这个关键字来支持这个**错误**。来看一个例子：

```cmake
function(demoArgs)
    set(noValues "")
    set(singleValues SPECIAL)
    set(multiValues ORDINARY EXTRAS)
    
    cmake_parse_arguments(
        PARSE_ARGV 0
        ARG
        "${noValues}" "${singleValues}" "${multiValues}"
    )
    
    message("Keywords missing values: ${ARG_KEYWORDS_MISSING_VALUES}")
endfunction()

demoArgs(burger fries SPECIAL ORDINARY EXTRAS high low)
```

在这个例子中，因为 SPECIAL 关键字参数是明确需要一个额外的参数的，如果没有传递，可能会导致错误，但是 CMake 不会报错，所以我们要特别注意。

`cmake_parse_arguments()` 命令的第一种形式其实还有其他用途，我们再来看一个使用 `cmake_parse_arguments()` 命令的第一种形式的例子：

```cmake
function(libWithTest)
    # First level of arguments
    set(groups LIB TEST)
    cmake_parse_arguments(GRP "" "" "${groups}" ${ARGN})
    
    # Second level of arguments
    set(args SOURCES PRIVATE_LIBS PUBLIC_LIBS)
    cmake_parse_arguments(LIB "" "TARGET" "${args}" ${GRP_LIB})
    cmake_parse_arguments(TEST "" "TARGET" "${args}" ${GRP_TEST})
    
    add_library(${LIB_TARGET} ${LIB_SOURCES})
    target_link_libraries(${LIB_TARGET}
        PUBLIC ${LIB_PUBLIC_LIBS}
        PRIVATE ${LIB_PRIVATE_LIBS}
    )
    
    add_executable(${TEST_TARGET} ${TEST_SOURCES})
    target_link_libraries(${TEST_TARGET}
        PUBLIC ${TEST_PUBLIC_LIBS}
        PRIVATE ${LIB_TARGET} ${TEST_PRIVATE_LIBS}
    )
endfunction()

libWithTest(
    LIB
        TARGET Algo
        SOURCES algo.cpp algo.h
        PUBLIC_LIBS SomeMathHelpers
    TEST
        TARGET AlgoTest
        SOURCES algoTest.cpp
        PRIVATE_LIBS gtest_main
)
```



这个例子很好的展示了通过 `cmake_parse_arguments()` 命令解析多级参数的方式。

虽然这些功能相当强大，但该命令仍然有一些限制。内置命令能够支持重复关键字。例如，像target_link_libraries（）这样的命令允许在同一个命令中多次使用PRIVATE、PUBLIC和INTERFACE关键字。cmake_parse_arguments（）命令不支持这一点。它只会返回与关键字最后一次出现相关的值，并丢弃较早的值。只有当使用多级关键字集并且关键字在任何给定的正在处理的参数集中只出现一次时，关键字才能重复。

####  2.3 从函数返回值
从 CMake 3.25 开始，我们可以在 return() 命令中使用 PROPAGATE 关键字来轻松返回值。所以要使用这个语法，我们必须将 CMP0140 这个策略设置为 NEW。现在我们还没有学习到 CMake 策略，我们只需要安装 CMake 3.25 及其以上版本，然后使用 ```cmake_minimum_required(VERSION 3.25 FATAL_ERROR)``` 命令声明最小 CMake 版本就行，这条命令会自动为我们讲 CMP0140 这个策略设置为 NEW。

下面看一个例子：
```cmake
# This ensures that we have a version of CMake that supports
# PROPAGATE and that the CMP0140 policy is set to NEW
cmake_minimum_required(VERSION 3.25)

function(doSomething outVar)
    set(${outVar} 42)
    return(PROPAGATE ${outVar})
endfunction()

doSomething(result)
# Here, a variable named result now holds the value 42
```

上面的例子，函数调用者希望通过 outVar 这个变量返回一个值。所以我们只需要在函数内部对这个变量赋值后通过 return() 命令的 PROPAGATE 关键字返回就行。

这里有个技巧，为了不让函数中自己定义的变量覆盖调用者作用域内的变量，通常在 return() 命令中使用 PROPAGATE 关键字传播出去的变量都要求定义成函数的命名变量，这样变量的名字和作用完全由调用者决定，函数只是对其值做操作。

如果函数内部还有新的作用域，return() 命令是从函数内部新的作用域调用的，会是什么情况呢？我们看看下面这个例子：
```cmake
cmake_minimum_required(VERSION 3.25)

function(doSomething outVar)
    set(${outVar} 42)
    block()
        set(${outVar} 27)
        return(PROPAGATE ${outVar})
    endblock()
endfunction()

doSomething(result)
# Here, a variable named result now holds the value 27
```

上面使用的 return() 命令在 CMake 3.25 以及更新的版本才支持。那如果使用的 CMake 版本小于 3.25 版本该怎么实现这个功能呢？不知道大家还记得我们在讲 CMake 变量的时候，set() 命令有一个 PARENT_SCOPE 关键字，我们可以借助这个关键字完整相同的功能。如下：
```cmake
function(func resultVar1 resultVar2)
    set(${resultVar1} "First result" PARENT_SCOPE)
    set(${resultVar2} "Second result" PARENT_SCOPE)
endfunction()

func(myVar otherVar)
message("myVar: ${myVar}")
message("otherVar: ${otherVar}")
```



### [宏和函数的区别](#)

* 宏本质是文本替换(代码复制)
* 函数的参数是变量、宏的参数是直接取值
* 函数有作用域，而宏没有作用域
* 函数有返回值 
