### [CMake 属性](#)

**介绍**: 在CMake中，只有目标才有属性，属性是指给目标或变量附加的元数据，它们用来在构建过程中提供额外的上下文信息。

----

> cmake_print_properties 内容 暂未添加！

### [1. property 属性](#)

属性是 CMake 的关键概念，属性会影响构建过程的方方面面，从源文件如何编译成对象文件，到二进制文件的安装位置，再到打包安装程序的目录等。

为了方便理解 CMake 中的属性，我这里先举一个例子：我们可以把人当作是一个对象，人有姓名、性别、年龄、身高等，这些就可以称为人这个对象的属性。

在 CMake 中，我们可以把各种 CMake 实体也看作是一个对象，这些对象都可以有自己的属性。CMake 中的实体有**目录、target、SOURCE、TEST、缓存变量**等等，我们甚至可以把整个构建过程本身也看作是一种 CMake 的实体。



#### 1.1 属性和变量的区别

在 CMake 中，属性和变量都是用于管理构建过程中的设置和配置的机制，但二者的作用和使用方式略有不同。

**变量是一种存储数据的机制**，可以用于存储字符串、列表、布尔值等类型的数据。开发者可以使用 set 命令将变量设置为特定的值，也可以通过 get 命令获取某个变量的值，以及通过 if 和 foreach 等命令来控制程序流程。

例如：

```cmake
set(HELLO_WORLD "Hello, World!")
message(${HELLO_WORLD})
```

这会将名为 HELLO_WORLD 的变量设置为字符串 "Hello, World!"，并将其输出到终端。

另一方面，**属性是一种与目标（目录、文件、库等）相关联的键值对**，在 CMakeLists.txt 文件中通过 **set_target_properties** 或 **set_directory_properties** 命令设置。属性中的键是一个字符串，用于标识属性的名称，而值则可以是一个字符串或其他允许的数据类型。

例如，以下命令设置一个库目标的属性：

```cmake
add_library(my_library my_src.cpp)
set_target_properties(my_library PROPERTIES VERSION 1.0.0)
```

这会将名为 my_library 的库目标设置版本号为 1.0.0。

总的来说，**变量用于存储通用的数据或控制程序流程，而属性则用于设置目标的一些特殊行为或元数据**。变量在整个 CMakeLists.txt 中都可用，而属性则局限于目标所在的作用域。

 **有时候我们很容易把属性和变量搞混淆，属性不像变量那样持有独立值，它提供特定于其附加实体的信息，这是属性和变量的根本区别**。

* 属性是依附目标 ，变量独立存在。
* 属性是元数据，变量是存储通用数据或控制程序流程。

### [2. 设置和获取属性](#)

设置属性使用如下命令：

```cmake
set_property(<GLOBAL                      |
              DIRECTORY [<dir>]           |
              TARGET    [<target1> ...]   |
              SOURCE    [<src1> ...]
                        [DIRECTORY <dirs> ...]
                        [TARGET_DIRECTORY <targets> ...] |
              INSTALL   [<file1> ...]     |
              TEST      [<test1> ...]     |
              CACHE     [<entry1> ...]    >
             [APPEND] [APPEND_STRING]
               <name> [<value1> ...])
```

set_property() 命令是设置属性的通用命令，第一个参数代表的是需要设置属性的实体，从上述命令的签名可以看出，可以设置属性的实体有：GLOBAL、TARGET、SOURCE、INSTALL、TEST、CACHE。

- **GLOBAL** 实体代表全局属性
- **DIRECTORY** 实体后面可以跟一个目录（这个目录必须是 CMake 已知的），如果没有明确指出，则是当前目录。
  - 相关特定于设置目录属性的命令：[set_directory_properties()](https://cmake.org/cmake/help/latest/command/set_directory_properties.html#command:set_directory_properties)
- **TARGET** 实体代表 CMake 已知的 target
  - 相关特定于设置 target 属性的命令：[set_target_properties()](https://cmake.org/cmake/help/latest/command/set_target_properties.html#command:set_target_properties)
- **SOURCE** 实体代表和源码文件相关的属性。如果没有跟子命令，那源码的属性只对当前目录中定义的 target 可见。
  - DIRECTORY 子命令用于指定目录，在这些指定的目录中定义的 target 均对设置的源码属性可见。
  - TARGET_DIRECTORY 子命令使得其指定的 target 目录对设置的源码属性可见。
  - 相关特定于设置源码文件属性的命令：[set_source_files_properties()](https://cmake.org/cmake/help/latest/command/set_source_files_properties.html#command:set_source_files_properties)
- INSTALL 实体指定安装的文件路径
- TEST 实体指定单元测试
  - 相关特定于设置单元测试属性的命令：[set_tests_properties()](https://cmake.org/cmake/help/latest/command/set_tests_properties.html#command:set_tests_properties)
- **CACHE** 实体指定缓存变量
- PROPERTY 参数是必须的，用于制定需要设置的属性的名字，名字后面需要跟相应的值。

APPEND 和 APPEND_STRING 这两个可选的参数会影响到属性值追加时候的行为，通过一个表格来理解。

| 原始值 | 新的值 | 没有关键字 | APPEND  | APPEND_STRING |
| ------ | ------ | ---------- | ------- | ------------- |
| foo    | bar    | bar        | foo;bar | foobar        |
| a;b    | c;d    | c;d        | a;b;c;d | a;bc;d        |

获取属性使用如下命令：

签名如下：
```cmake
get_property(<variable>
    <GLOBAL                                                 |
    DIRECTORY [<dir>]                                       |
    TARGET    <target>                                      |
    SOURCE    <source>
        [DIRECTORY <dir> | TARGET_DIRECTORY <target>]       |
    INSTALL   <file>                                        |
    TEST      <test>                                        |
    CACHE     <entry>                                       |
    VARIABLE           >
    PROPERTY <name>
    [SET | DEFINED | BRIEF_DOCS | FULL_DOCS])
```

可以获取属性值的实体有：GLOBAL、DIRECTORY、TARGET、SOURCE、INSTALL、TEST、CACHE、VARIABLE。

除了 VARIABLE 外，其他实体的含义在 ```set_property()``` 命令中已经讲过。VARIABLE 实体顾名思义，就是过去附着在变量上的属性。

**SET 选项** 如果给定，那要获取的属性**如果设置过值**，则 variable 被设置为真。

**DEFINED 选项**如果给定，如果有要获取的这个属性名字，则 variable 被设置为真。

BRIEF_DOCS | FULL_DOCS 选项如果给定，那 variable 被设置为要获取的属性的文档字符串。

**上述四个可选关键字，只有 SET 在实际项目中用到，其他三个几乎不用**，**除非明确**使用 [define_property()](https://cmake.org/cmake/help/latest/command/define_property.html#command:define_property) 命令为某个属性设置过信息。

其签名如下：
```cmake
define_property(<GLOBAL | DIRECTORY | TARGET | SOURCE | TEST | VARIABLE | CACHED_VARIABLE>
    PROPERTY <name> [INHERITED]
    [BRIEF_DOCS <brief-doc> [docs...]]
    [FULL_DOCS <full-doc> [docs...]]
    [INITIALIZE_FROM_VARIABLE <variable>])
```

**例子**：

```
define_property(GLOBAL PROPERTY SYSTEM_USE_COMPILER
        BRIEF_DOCS "system default compiler"
        FULL_DOCS  "the project in current system default compiler"
        "GCC"
)

get_property(global_System GLOBAL PROPERTY SYSTEM_USE_COMPILER BRIEF_DOCS)
```





**define_property 和 set_property 命令的作用不同**

- define_property：用于定义自定义属性，指定属性名称、值类型、文档等属性元数据。该命令并不设置属性的实际值，只是定义了属性的元数据。
- set_property：用于设置属性的值，可以是 CMake 内置属性也可以是自定义属性。该命令根据指定的属性名称和作用域设置属性的值。

举例来说，假设要为一个名为 my_target 的目标添加一个自定义属性 my_attribute，并将其默认值设置为 "my value"。可以使用以下命令：

```
define_property(TARGET my_target PROPERTY my_attribute
                 BRIEF_DOCS "A brief description of my_attribute."
                 FULL_DOCS "A full description of my_attribute."
                 VALUE_TYPE STRING
                 IMPLICIT_VALUE "my value")
```

这会在 my_target 目标上定义一个名为 my_attribute 的字符串属性，表示该目标的 my_attribute 值类型为字符串，初始值为 "my value"。

然后，可以使用以下命令为该目标的 my_attribute 属性设置实际的属性值：

```
set_property(TARGET my_target PROPERTY my_attribute "new value")
```

这会将 my_target 目标的 my_attribute 属性设置为 "new value"。

因此，define_property 用于定义自定义属性的元数据，而 set_property 用于为自定义属性和内置属性设置实际值。需要注意的是，开发者必须了解属性的作用域和兼容性问题，以避免产生不必要的更改。



### [3. 全局属性](#)

全局属性设置与获取，在任意一个子目录都可以访问到。

```cmake
#设置全局属性
set_property(GLOBAL PROPERTY TEST_ENABLE_START "test global start!")
#获取全局属性
get_property(global_Var GLOBAL PROPERTY TEST_ENABLE_START)
#打印全局属性
message(${global_Var})
```



#### [3.1 get_cmake_property](#)

 CMake 预定于了大量的全局属性，这些属性我们大多数情况下不会通过 set_porperty() 命令去更改它们的值，但是在项目中有时我们会使用 get_property() 命令去获取这些属性的值，帮助我们对 CMake 项目进行更好的组织和管理。

对于获取 CMake 全局属性，除了 get_property() 命令，CMake 还为我们提供了一个简化版本的命令：get_cmake_property()，在项目中也建议使用这个命令。

get_cmake_property() 命令可以获取任何类型全局属性的值，同时它也能获取一些行为和全局属性类似的属性值，这些属性称为伪属性，并且伪属性的值只能通过 get_cmake_property() 命令获取。

下面我们先来看看如何使用 get_cmake_property() 命令获取伪属性的值。

- VARIABLES
  - 返回所有常规（即非缓存）变量的列表。因为常规变量很多，我们可以使用一个 foreach() 命令将获取到的属性值一个一个地打印出来。在这里我们可以看到以 CMAKE_ 开头的 CMake 预定义变量，以 PROJECT_ 开头的当调用 project() 命令后自动生成的变量，以 _ 开头的 CMake 内部变量，以及以项目名字(study_cmake)加下划线开头的和项目相关的变量。我们可以直接打印这些变量的值。
- CACHE_VARIABLES
  - 返回所有缓存变量的列表。 同样也有和项目名字相关的缓存变量。
- COMMANDS
  - 返回所有已定义的命令、函数和宏的列表。命令由 CMake 预先定义，而函数和宏可以由 CMake（通常通过模块）或项目本身定义。一些返回的名称可能对应于未记录或不打算供项目直接使用的内部实体。这些名称可能以不同于最初定义的大小写形式返回。
- MACROS
  - 仅返回已定义宏的列表。这将是 COMMANDS 伪属性的子集，但请注意名称的大小写可能与 COMMANDS 伪属性报告的不同。
- COMPONENTS
  - 返回由 install() 命令定义的所有组件的列表，由于 install() 命令我们现在还没有学到，本讲不对其进行演示。

最后我们看两个使用 get_cmake_property() 命令获取全局属性的例子，CMake 官网列出的全局属性建议大家都看看其含义，对以后更好地使用 CMake 全局属性有很大帮助。

- CMAKE_C_KNOWN_FEATURES
  - 获取当前 CMake 版本已知的 C 标准和特性。
- FIND_LIBRARY_USE_LIB64_PATHS
  - 获取 find_library() 命令是否应该自动搜索 lib64 目录。



### [4. 目录属性](#)

CMake 的一个重要的概念是 Target（目标），它是要构建的文件的抽象，可以是可执行文件、库文件或其他类型的文件。每个 Target 都可以有一些属性，这些属性控制着它们的编译和链接方式、输出类型和路径等。



#### [4.1 Target 属性的作用](#)

在 CMake 中，Target 属性对于控制如何将源代码转化为目标文件（可执行文件、库文件或其他类型的文件）至关重要。Target 属性控制了从源代码到最终输出的所有步骤，包括源代码的编译、链接器的选项和输出文件的路径等。

**在 CMake 中，Target 属性具有以下作用**：

- **控制如何编译源代码，例如使用哪些编译器选项、预处理器定义、包含路径等**。
- **控制如何链接目标文件，例如使用哪些库、库的搜索路径、链接器选项等**。
- **控制输出文件的类型和路径，例如是否为库、可执行文件或其他类型文件，以及输出路径**。

此外，Target 属性还可以影响如何在开发人员的 IDE 项目中呈现目标，包括如何组织和显示文件、图标和其他属性。

总之，Target 属性是将源代码转化为目标文件的关键，几乎涵盖了所有将源代码转化为目标文件所需的详细信息。

#### [4.2 Target 属性的设置和获取](#)

在 CMake 中，设置 Target 属性有多种方法。除了通用的 ```set_property()``` 和 ```get_property()``` 命令之外，还提供了一些针对 Target 的命令，如 ```set_target_properties()``` 和 ```get_target_property()```。

```set_target_properties()``` 命令用于设置 Target 的属性，它的语法如下：

```cmake
set_target_properties(target1 [target2...]
    PROPERTIES
        propertyName1 value1
        [propertyName2 value2] ...
)
```

其中，target1 和 target2 是 Target 的名称，propertyName1 和 propertyName2 是 Target 属性的名称，value1 和 value2 是相应属性的值。

例如，可以通过以下命令来设置一个目标的输出名称和输出路径：
```cmake
set_target_properties(MyTarget
  PROPERTIES
    OUTPUT_NAME "mylib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
```

该命令将 MyTarget 目标的输出名称设置为 mylib，输出路径设置为 ${CMAKE_BINARY_DIR}/bin。

get_target_property() 命令用于获取一个目标的属性值。其语法如下：

```cmake
get_target_property(resultVar target propertyName)
```

其中，resultVar 表示获取到的属性值将会存储在该变量中，target 表示需要获取属性的目标，propertyName 表示需要获取的属性名。

例如，可以通过以下命令来获取一个目标的输出路径：

```cmake
get_target_property(output_path MyTarget RUNTIME_OUTPUT_DIRECTORY)
```

该命令将会把 MyTarget 目标的 RUNTIME_OUTPUT_DIRECTORY 属性值存储到 output_path 变量中。

总之，目标属性是 CMake 中非常重要的一部分，它们控制着目标的编译、链接、输出路径等各个方面。在编写 CMakeLists.txt 文件时，需要对目标属性有一定的了解，并且可以使用相关命令来获取和设置它们。

关于 CMake Target 属性就先学到这里，我们后续还会经常使用到。



### [5. 目录属性](#)

**这些属性只会到设置的目录中被访问到，是有效的。** 通过对目录设置和获取属性，可以方便我们对项目的组织和管理。

除了属性通用的命令可以设置和获取目录属性外，CMake 也为我们提供了专门用于设置和获取目录属性的命令。 

```cmake
set_directory_properties(PROPERTIES prop1 value1 [prop2 value2] ...)
```

CMake 设置目录属性的通用命令如下：

```cmake
set_property(DIRECTORY [<dir>]
    [APPEND] [APPEND_STRING]
    PROPERTY <name> [<value1> ...]
)
```

这两个命令用于设置一个目录的属性，影响该目录下的所有子目录和目标。该命令可以设置一些常用的目录属性，如编译器选项、编译器特性、链接选项等。

例如：
```cmake
set_directory_properties(PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    COMPILE_OPTIONS "-Wall;-Wextra;-pedantic"
)
```

```cmake
set_property(DIRECTORY
    PROPERTY CXX_STANDARD 17
)
```

有设置目录属性的命令，也就有获取目录属性的命令。

```cmake
get_directory_property(<variable> [DIRECTORY <dir>] <prop-name>)
get_directory_property(<variable> [DIRECTORY <dir>] DEFINITION <var-name>)
```

第一种形式非常简单，对应获取由设置目录属性的命令设置的目录属性，例如：

```cmake
get_directory_property(CXX_STANDARD_USED DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CXX_STANDARD)
message("CXX_STANDARD_USED: ${CXX_STANDARD_USED}")
```

对于第二种形式，看上去和目录属性没有太大关系，但是非常有用，比如我们在 lib 目录设置了一个变量，然后想在 app 目录使用这个变量的值，就可以使用第二种形式获取到。

lib 目录

```cmake
cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

set(LIB_VERSION "1.0.0")
```

app 目录

```cmake
cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

get_directory_property(LIB_VERSION_DEFINITION DIRECTORY "${CMAKE_SOURCE_DIR}/lib" DEFINITION LIB_VERSION)

message("Library version: ${LIB_VERSION_DEFINITION}")
```

关于 CMake 目录属性，掌握这些知识点就能够很好地帮助我们组织 CMake 项目了，大家记得自己动手练习消化哦，我们下期见。



### [6. 源文件属性](#)
属性还可以提供有关源文件的其他信息，以修改 CMake 或构建工具如何处理该文件，例如，它们可以指示该文件是否作为构建的一部分生成，使用哪个编译器，与文件一起使用的非编译器工具选项等。

开发者可以使用 ```set_source_files_properties()``` 和 ```get_source_file_property()``` 命令来设置和获取源文件属性。例如，可以使用以下命令设置源文件属性，以防止将其与其他源文件组合在一个 Unity Build 中：

```cmake
add_executable(MyApp small.cpp big.cpp tall.cpp thin.cpp)

set_source_files_properties(big.cpp PROPERTIES SKIP_UNITY_BUILD_INCLUSION YES)
```

其中，SKIP_UNITY_BUILD_INCLUSION 属性用于防止该文件被包含到 Unity Build 中。

此外，CMake 3.18 或更高版本还提供了其他选项来指定应该在哪个目录范围内搜索或应用源文件属性。例如，DIRECTORY 选项可以用于指定应该设置源文件属性的一个或多个目录，TARGET_DIRECTORY 选项可以指定要设置源文件属性的目标的名称。

```cmake
set_property(SOURCE sources...
    [DIRECTORY dirs...]
    [TARGET_DIRECTORY targets...]
    [APPEND | APPEND_STRING]
    PROPERTY propertyName values...
)
set_source_files_properties(sources...
    [DIRECTORY dirs...]
    [TARGET_DIRECTORY targets...]
    PROPERTIES
        propertyName1 value1
        [propertyName2 value2] ...
)

get_property(resultVar SOURCE source
    [DIRECTORY dir | TARGET_DIRECTORY target]
    PROPERTY propertyName
    [DEFINED | SET | BRIEF_DOCS | FULL_DOCS]
)

get_source_file_property(resultVar source
    [DIRECTORY dir | TARGET_DIRECTORY target]
    propertyName
)
```

需要注意的是，对于一些 CMake 生成器（特别是 Unix Makefiles 生成器），源文件和源文件属性之间的依赖关系比较强。如果使用源文件属性来修改特定源文件的编译器标志而不是整个目标的标志，那么更改源文件的编译器标志将导致所有目标源文件被重新构建，而不仅仅是受影响的源文件。

这是由于在 Makefile 中，测试每个单独源文件的编译器标志是否发生了变化会带来很大的性能损失。因此，相关的 Makefile 依赖关系是在目标级别实现的。

通常情况下，开发者可能会使用源文件属性将版本信息传递给一个或两个源文件作为编译器定义。但是，如上所述，使用源文件属性也可能会降低构建性能，因为这些文件不会参与 Unity Build。因此，在使用源文件属性时，开发者需要注意潜在的性能问题，并考虑其他替代方案。

最后我们再来看一个例子：

假设有一个 CMake 项目，其中有一个 src 目录，其中包含以下源文件：

```shell
src/
├── main.cpp
├── helper.cpp
├── helper.h
├── utils.cpp
└── utils.h
```

现在，我们希望在编译 main.cpp 时使用一个特定的编译器选项，而在编译 helper.cpp 时不使用该选项，并且在编译 utils.cpp 时禁用 Unity 构建（即不将其与其他文件合并到一个单独的源文件中）。

为了实现这些要求，我们可以在 CMakeLists.txt 文件中添加以下代码：

```cmake
cmake_minimum_required(VERSION 3.18)
project(MyProject)

# 添加可执行文件
add_executable(MyApp src/main.cpp src/helper.cpp src/utils.cpp)

# 为 main.cpp 添加一个编译器选项
set_source_files_properties(src/main.cpp PROPERTIES COMPILE_FLAGS "-O2")

# 禁用 utils.cpp 的 Unity 构建
set_source_files_properties(src/utils.cpp PROPERTIES SKIP_UNITY_BUILD_INCLUSION YES)
```

这样，CMake 将为 src/main.cpp 添加 -O2 选项，而对于 src/helper.cpp，CMake 将使用默认选项。而 src/utils.cpp 将被标记为不参与 Unity 构建。

需要注意的是，如果在项目中定义多个目标，那么 SKIP_UNITY_BUILD_INCLUSION 属性可能会影响所有使用了该文件的目标，而不仅仅是与当前目标相关的文件。因此，需要谨慎地使用该属性。

对于大多数项目，我们都不需要这么细粒度的控制，所以几乎不会用到源码属性。所以本讲内容如果没有理解，不必担心，它不会影响我们后续的学习。通过后续对统一构建的深入学习，本讲的例子就能够轻松理解了。

#### [6.1 给源文件传递宏](#)

使用 **COMPILE_DEFINITIONS** 传递宏！

```cmake
set_property(SOURCE muse.cpp PROPERTY COMPILE_DEFINITIONS "TEST_MACRO=123")
```

访问宏！

```cpp
std::cout << TEST_MACRO << std::endl; //123
```







### [7. 缓存变量属性](#)

CMake 缓存变量是用于存储用户定义的变量，可以在多次构建过程中保持不变的一种变量。CMake 的缓存变量属性是针对缓存变量的属性，主要是为了影响 CMake GUI 和 ccmake 工具的使用体验，而不会直接影响构建过程。下面将详细介绍 CMake 缓存变量属性的相关信息：

- 变量类型（TYPE）
  - 每个缓存变量都有一个类型，类型必须是 BOOL、FILEPATH、PATH、STRING 或 INTERNAL 中的一种。使用 get_property() 命令和属性名 TYPE 可以获取变量的类型。变量的类型会影响 CMake GUI 和 ccmake 工具中的变量呈现方式以及用于编辑其值的小部件。类型为 INTERNAL 的变量将不会在 GUI 中显示。

- 高级变量（ADVANCED）
  - 使用 mark_as_advanced() 命令可以将缓存变量标记为高级变量。实际上，这只是设置布尔类型的 ADVANCED 缓存变量属性。CMake GUI 和 ccmake 工具都提供了一个选项，用于显示或隐藏高级缓存变量。用户可以选择关注基本变量还是查看完整列表。

- 帮助字符串（HELPSTRING）
  - 缓存变量的帮助字符串通常作为 set() 命令的一部分进行设置，但也可以使用 HELPSTRING 缓存变量属性进行修改或读取。帮助字符串用作 CMake GUI 中的工具提示以及 ccmake 工具中的单行帮助提示。

- 变量值列表（STRINGS）
  - 如果缓存变量的类型是 STRING，则 CMake GUI 将查找名为 STRINGS 的缓存变量属性。如果不为空，则该属性应为该变量的有效值列表，并且 CMake GUI 将以该列表中的值作为下拉列表框而不是任意文本输入小部件呈现该变量。在 ccmake 中，按下该缓存变量上的 enter 键将循环显示所提供的值。注意，CMake 并不强制缓存变量必须是 STRINGS 属性中的值，这只是为了方便 CMake GUI 和 ccmake 工具使用。

总之，CMake 缓存变量属性是为了影响 CMake GUI 和 ccmake 工具的使用体验而设置的属性。它们不会直接影响构建过程，而是为了让用户更方便地使用 CMake。

###  [8. 单元测试属性](#)

CMake 为单元测试提供了简化版本的设置和获取属性的命令，如下：

```cmake
set_tests_properties(test1 [test2...]
    PROPERTIES
        propertyName1 value1
        [propertyName2 value2] ...
)
get_test_property(resultVar test propertyName)
```

签名和 CMake target 属性类似，使用方法也类似，这里就不赘述了。

属性是 CMake 的关键部分。许多命令都有能力设置、修改或查询各种类型的属性，其中一些对项目之间的依赖关系有进一步的影响。

一般情况下，使用通用的 ```set_property()``` 命令可以完全操作除了特殊的全局伪属性以外的所有属性，这使得开发人员能够预测属性的行为，并在需要时提供灵活的 APPEND 功能。尽管属性特定的设置器在某些情况下更为方便，例如允许一次设置多个属性，但它们缺乏 APPEND 功能可能会导致一些项目选择仅使用 ```set_property()```。使用属性特定命令替换属性值而不是附加到属性值上是一个常见的错误。

对于目标属性，强烈建议使用各种 ```target_…()``` 命令而不是直接操作关联的目标属性。这些命令不仅操作特定目标的属性，还设置目标之间的依赖关系，以便 CMake 可以自动传播一些属性。

源属性提供了对编译器选项等级别的细粒度控制。然而，这些属性可能会对项目的构建行为产生不良影响。特别是，当仅有少数源文件的编译选项发生更改时，一些 CMake 生成器可能会重建比必要更多的内容。Xcode 生成器还有限制，无法支持特定于配置的源文件属性。