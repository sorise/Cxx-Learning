###  [CMake 构建参数](#)

**介绍**：编译过程与链接过程需要向**编译器及链接器**传入不同的参数，而这些参数分为以下几类：

- 编译选项（包括宏定义）
- 头文件路径
- 链接库文件名称
- 链接库查找路径

----



### 一. 常用命令

| 命令                                                         | 解释                             |
| :----------------------------------------------------------- | :------------------------------- |
| [target_link_libraries](http://cmake.org.cn/command/target_link_libraries.html) | 给目标添加链接库                 |
| [target_include_directories](http://cmake.org.cn/command/target_include_directories.html) | 给目标添加头文件库               |
| [target_compile_options](http://cmake.org.cn/command/target_compile_options.html) | 添加编译参数                     |
| [target_compile_definitions](http://cmake.org.cn/command/target_compile_definitions.html) | 给目标传递编译宏                 |
| [target_link_directories](http://cmake.org.cn/command/target_link_directories.html) | 给目标添加链接库所在文件         |
| [target_compile_features](http://cmake.org.cn/command/target_compile_features.html) | 添加编译特性                     |
| [target_link_options](http://cmake.org.cn/command/target_link_options.html) | 添加链接选项                     |
| [target_precompile_headers](http://cmake.org.cn/command/target_precompile_headers.html) | 添加要预编译的头文件列表         |
| [target_sources](http://cmake.org.cn/command/target_sources.html) | 该函数声明了向target添加源文件。 |

 

#### [1. target_link_libraries](#)

用于将可执行文件或库文件链接到库文件或可执行文件，很明显第二个参数也可以进行权限控制。

```cpp
target_link_libraries(<target>
  <PRIVATE|PUBLIC|INTERFACE> <item>...
 [<PRIVATE|PUBLIC|INTERFACE> <item>...]...)
```

- **PUBLIC**: 对内对外都公开，可将头文件目录以及链接库传递给连接者,target对应的头文件、源文件都使用。
- **PRIVATE**: 对内公开，对外不公开，不可传递头文件目录以及链接库,target对应的源文件使用。
- **INTERFACE**: 对外公开，对内不公开，可传递头文件目录以及链接库，但内部不可使用头文件及连接库，只能接口传递,target对应的头文件使用。

使用例子：

```cmake
add_executable(network main.cpp)

add_library(prints SHARED ./src/prints.cpp)
#什么都不写就是静态库
#中间写一个 SHARED 就是动态库

target_link_libraries(network prints)
#将动态库 prints 连接到 network
```



#### [2. target_include_directories](#)

该命令可以指定目标（exe或者so文件）需要包含的头文件路径，目标必须是由 **add_executive()** 或 **add_library()** 之类的命令创建的，并且不能是 ALIAS 目标。

```cmake
target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

**参数说明：**

* SYSTEM：告诉编译器路径可能是系统路径，解决一些平台警告信息
* AFTER：最后,把路径放在查找的最后面。
* BEFORE：前面,把路径放在查找的最前面。

- **PUBLIC**: 对内对外都公开，可将头文件目录以及链接库传递给连接者,target对应的头文件、源文件都使用。
- **PRIVATE**: 对内公开，对外不公开，不可传递头文件目录以及链接库,target对应的源文件使用。
- **INTERFACE**: 对外公开，对内不公开，可传递头文件目录以及链接库，但内部不可使用头文件及连接库，只能接口传递,target对应的头文件使用。

 

```cmake
add_library(levelDB SHARED src/leveldb.cpp)

target_include_directories(levelDB 
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)
```

#### [3. target_compile_options](#)

`target_compile_options`是CMake中的一个命令，用于为指定的目标（例如可执行文件、动态库或静态库）添加编译选项。通过调用该命令，可以向目标的编译过程中传递特定的编译选项。

例如，在CMakeLists.txt文件中使用`target_compile_options`命令为目标添加编译选项：

```cmake
Copy Codeadd_executable(my_target main.cpp)

# 为my_target添加编译选项
target_compile_options(my_target PRIVATE -Wall -O2)
```

在上述示例中，`target_compile_options`命令向`my_target`目标添加了两个编译选项：`-Wall`表示启用所有警告信息，`-O2`表示启用优化级别2。通过设置`PRIVATE`关键字，这些编译选项仅适用于`my_target`目标自身，而不会传递给依赖于它的其他目标。

除了使用具体的编译选项字符串外，也可以使用CMake内置的变量来传递选项。例如，`${CMAKE_CXX_FLAGS}`表示C++编译器的标志，可以通过`target_compile_options(my_target PRIVATE ${CMAKE_CXX_FLAGS})`将其传递给编译器。

注意，使用`target_compile_options`命令添加编译选项时应谨慎，确保选项的正确性和适用性，并遵循编译器的文档和建议。此外，编译选项的传递应根据目标的需求进行合理的设置，以确保代码的正确性和性能优化。

#### [4. target_compile_definitions](#)

此函数专门向目标添加预设宏声明及定义。add_definitions	也可以添加全局宏定义，但不再推荐使用，因为它会为所有的目标添加宏，建议使用**add_compile_definitions**。



```cmake
target_compile_definitions(<target>
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])

```

例子：

```cpp
target_compile_definitions(sample PRIVATE BUILD_DLL PUBLIC "-DPI=3.14159")
```



#### [6. target_compile_features](#)

 CMake中的**target_compile_features命令用向target添加预期的编译器功能**,  命名的\<target\>必须由诸如add_executable或add_library之类的命令创建，并且不能为ALIAS target。需要注意的事情是不同的编译器例如GCC、MSVC支持的特性存在不同。

```cmake
target_compile_features(<target> <PRIVATE|PUBLIC|INTERFACE> <feature> [...])
```

**例子**：

```cmake
add_executable(main samples/sample_add.cpp)

# 将 C++ 标准设置为使用 C++ 11
target_compile_features(main PRIVATE cxx_std_11)

# 将 C++ 标准设置为使用自动的合适标准
target_compile_features(hello_cpp11 PUBLIC cxx_auto_type)

# 打印所有的特性
foreach (var IN LISTS CMAKE_CXX_COMPILE_FEATURES)
    message(${var})
endforeach ()
```

**通过CMAKE_CXX_COMPILE_FEATURES 判断编译器是否支持C++11**，这个列表变量存储了当前支持的所有特性。

```cmake
cxx_return_type_deduction
cxx_variable_templates
cxx_std_17
cxx_std_20
cxx_std_23
...
```

如果设置的特性不支持，该操作会报错。	

也可以如此设置：

```cmake
set_target_properties(myTarget PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
```



### 二、source_group

`source_group`是CMake中的一个命令，用于组织源代码文件到逻辑组中进行管理。通过调用该命令，可以将相关的源文件分组，并在IDE中以相应的目录结构显示，使项目的组织更加清晰和可读。

```cmake
source_group(<name> [FILES <src>...] [REGULAR_EXPRESSION <regex>])
source_group(TREE <root> [PREFIX <prefix>] [FILES <src>...])
```

例如，在CMakeLists.txt文件中使用`source_group`命令来组织源代码文件：

```cmake
# 添加可执行文件
add_executable(my_target main.cpp)

# 添加其他源代码文件
set(SOURCE_FILES 
    source1.cpp
    source2.cpp
    source3.cpp
)

# 将源文件分组
source_group("Source Files" FILES ${SOURCE_FILES})

# 将分组后的源文件与可执行文件关联
target_sources(my_target PRIVATE ${SOURCE_FILES})
```

在上述示例中，`source_group`命令将`SOURCE_FILES`中的源文件分组为"Source Files"组。这样，在IDE中查看项目文件结构时，源文件将按照分组形式显示。

`source_group`命令接受两个参数：第一个参数是组名称，用于标识源文件所属的组；第二个参数是要分组的源文件列表。可以根据需要将源文件分组到不同的组中，以实现更好的组织和可视化效果。

最后，使用`target_sources`命令将分组后的源文件与目标（如可执行文件、库）关联起来，确保这些源文件被编译和链接到对应的目标中。

通过使用`source_group`，可以更好地组织和管理较大项目中的源代码文件，并在IDE中以合理的目录结构展示，提高项目的可读性和可维护性。

```cmake
source_group(TREE . PREFIX include/rpc FILES protocal.hpp reactor.hpp)
```
