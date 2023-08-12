### [CMake 全局构建参数](#)

**介绍**: CMake 通用编译与构建相关命令和变量。

----

- [x] [1. 全局构建与编译参数](#1-全局构建与编译参数)
- [x] [2. CPP的标准](#2-cpp的标准)
- [x] [3. 编译配置](#3-编译配置)
- [x] [4. 输出路径](#4-输出路径)
- [x] [5. 系统信息](#5-系统信息)



---

### [1. 全局构建与编译参数](#)

| 命名                                                         | 解释                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [add_compile_definitions](http://cmake.org.cn/command/add_compile_definitions.html) | 用于向编译器添加预定义宏定义                                 |
| [add_compile_options](http://cmake.org.cn/command/add_compile_options.html) | 通过调用该命令，可以将特定的编译选项传递给编译器，以影响代码的编译过程和生成的可执行文件。 |
| [add_custom_command](http://cmake.org.cn/command/add_custom_command.html) |                                                              |
| [add_custom_target](http://cmake.org.cn/command/add_custom_target.html) |                                                              |
| [add_definitions](http://cmake.org.cn/command/add_definitions.html) |                                                              |
| [add_dependencies](http://cmake.org.cn/command/add_dependencies.html) |                                                              |
| [add_executable](http://cmake.org.cn/command/add_executable.html) |                                                              |
| [add_library](http://cmake.org.cn/command/add_library.html)  |                                                              |
| [add_link_options](http://cmake.org.cn/command/add_link_options.html) |                                                              |
| [add_test](http://cmake.org.cn/command/add_test.html)        |                                                              |



#### [1.1 add_compile_definitions](#)

例如，在CMakeLists.txt文件中使用`add_compile_definitions`命令添加预定义宏定义：

```
Copy Codeadd_compile_definitions(DEBUG_MODE)
```

这将在编译时定义一个名为`DEBUG_MODE`的宏。在源代码中，可以使用条件编译指令来根据该宏的定义与否选择性地编译不同的代码块：

```cmake
c++Copy Code#ifdef DEBUG_MODE
    // 调试模式下的代码
#else
    // 非调试模式下的代码
#endif
```

注意，`add_compile_definitions`命令在CMake 3.12及以上版本中可用。

#### [1.2 add_compile_options](#)

`add_compile_options`是CMake中的一个命令，用于向编译器添加编译选项。通过调用该命令，可以将特定的编译选项传递给编译器，以影响代码的编译过程和生成的可执行文件。

例如，在CMakeLists.txt文件中使用`add_compile_options`命令添加编译选项：

```cmake
Copy Codeadd_compile_options(-Wall -O2)
```

这将向编译器传递两个选项：`-Wall`表示启用所有警告信息，`-O2`表示启用优化级别2。根据编译器的不同，具体的编译选项可能会有所不同。

除了使用具体的编译选项字符串外，也可以使用CMake内置的变量来传递选项。例如，`${CMAKE_CXX_FLAGS}`表示C++编译器的标志，可以通过`add_compile_options(${CMAKE_CXX_FLAGS})`将其传递给编译器。

注意，使用`add_compile_options`命令添加编译选项时应谨慎，确保选项的正确性和适用性，并遵循编译器的文档和建议。

#### [1.6 add_dependencies](#)

`add_dependencies`是CMake中的一个命令，用于定义目标之间的依赖关系。通过调用该命令，可以指定一个或多个目标依赖于其他目标，以确保在构建过程中正确地解析和构建这些依赖关系。

例如，在CMakeLists.txt文件中使用`add_dependencies`命令定义依赖关系：

```cmake
Copy Codeadd_executable(my_target main.cpp)
add_library(my_library STATIC my_library.cpp)

# 指定my_target依赖于my_library
add_dependencies(my_target my_library)
```

在上述示例中，`my_target`是通过`add_executable`命令定义的目标可执行文件，`my_library`是通过`add_library`命令定义的静态库。`add_dependencies`命令指定了`my_target`依赖于`my_library`，以确保在构建`my_target`时先构建并解析`my_library`。

这种依赖关系的定义对于确保目标之间正确的构建顺序很重要，特别是当目标之间存在源代码文件的相互依赖关系时。

注意，目标之间的依赖关系应当谨慎定义，确保依赖关系的正确性，并避免循环依赖造成的构建问题。



#### [1.8 add_library](#)
CMake中的**add_library命令用于使用指定的源文件向项目(project)中添加库**，可以创建静态库或动态库。第一个参数很重要，被称为target，可以作为target_xxx命令的接收对象。

**普通用法：**
```cmake
add_library(<name> 
[STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL] [<source>...]
) # Normal Libraries
```

添加一个名为\<name\>的库目标(library target)，从命令调用中列出的源文件构建。\<name\>对应于逻辑目标名称，并且在项目中必须是全局唯一(globally unique)的。

构建的库的实际文件名是基于本机平台(native platform)的约定构建的(例如lib<name\>.a或\<name\>.lib).

使用例子：
```cmake
add_library(test_lib a.cpp b.cpp) #默认生成静态库
add_library(test_lib SHARED a.cpp b.cpp) #生成动态库
```

**Object Libraries 用法**  ：**创建对象库(Object Library).对象库编译源文件** 但 **不会将其存档或链接到库中** 。相反，由add_library命令或add_executable命令创建的其它目标可以使用$\<TARGET_OBJECTS:objlib\>形式的表达式作为源来引用对象，其中objlib是对象库名称。

可以使用target_link_libraries命令链接对象库。

```cmake
add_library(<name> OBJECT [<source>...]) # Object Libraries
```

如下语句，将汇编文件转换成二进制目标文件，最终生成`.o`文件
```shell
g++ -c test.s -o test.o
```

例子：


```cmake
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(add_object OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/source/add.cpp) # 在build目录下不会有add_object文件生成
if(TARGET add_object)
    message("target: object library add") # print
endif()
```

或

```cmake
add_library(test_obj OBJECT src/mathTool/cal.cpp)

#必须加这个选项不然会报错
if(NOT MSVC)
	target_compile_options(test_obj "-fPIC")
endif()
add_library(math SHARED $<TARGET_OBJECTS:test_obj>)

add_executable(main main.cpp)

target_link_libraries(main PUBLIC math)
```



##### [1.8.1 给库添加版本信息](#)

通常使用[makefile](https://so.csdn.net/so/search?q=makefile&spm=1001.2101.3001.7020)编译，不是很关注so的版本号,但是如果库本身也有版本那么需要设置一下，我们通过属性来设置。

```cpp
add_library(math SHARED src/mathTool/cal.cpp)
    
SET_TARGET_PROPERTIES(math PROPERTIES VERSION 1.6.0.0 SOVERSION 1)
```

此时，会生成三个文件, 其中 libtaos.so.1.6.0.0为动态库的文件名(realname)，libtaos.so.1为动态库的别名(soname)，libtaos.so为动态库的链接名

> libtaos.so -> libtaos.so.1* 
>
> libtaos.so.1 -> libtaos.so.1.6.0.0*
>
> libtaos.so.1.6.0.0

如果不希望生成符号链接

```
SET_TARGET_PROPERTIES(math PROPERTIES 
    VERSION 1.6.0.0 
    SOVERSION 1
    NO_SONAME ON  #不生产符号链接
    DEBUG_POSTFIX 'd' #在dubug版本 给文件名加一个后缀
)
```



#### [1.9 add_link_options](#)

`add_link_options`是CMake中的一个命令，用于向链接器添加链接选项。通过调用该命令，可以将特定的链接选项传递给链接器，以影响可执行文件或库的链接过程。

例如，在CMakeLists.txt文件中使用`add_link_options`命令添加链接选项：

```cmake
Copy Codeadd_executable(my_target main.cpp)

# 向链接器添加链接选项
add_link_options(-L/path/to/libraries -lmylibrary)
```

上述示例中，`add_link_options`命令向链接器传递了两个选项：`-L/path/to/libraries`表示指定链接器在指定路径下搜索库文件，`-lmylibrary`表示指定链接器链接名为"mylibrary"的库。具体的链接选项和其含义可能因链接器的不同而有所差异。

注意，在使用`add_link_options`命令时，需确保传递的链接选项是有效且符合编译环境和目标平台的要求。对于跨平台项目，可能需要针对不同的操作系统或编译器使用不同的链接选项。

此外，还可以使用CMake内置的变量来传递链接选项。例如，`${CMAKE_EXE_LINKER_FLAGS}`表示可执行文件链接器的标志，可以通过`add_link_options(${CMAKE_EXE_LINKER_FLAGS})`将其传递给链接器。

使用`add_link_options`命令可以实现对链接过程的更加精细的控制和定制化。

### 二、CPP的标准

#### 设置C++标准

`CMAKE_CXX_STANDARD` 变量用于设置创建目标时使用的 `CXX_STANDARD` 目标属性默认值。

### 强制指定标准

出于对编译器的兼容性之考虑，上面这种设置标准的方法并不是强制执行。假设某个项目设置使用 C++ 11 标准，但用户使用的编译器并不支持 **-std=gnu++11** （或等价的）标志，将不会导致错误或警告，而是在允许的情况下添加 -std=gnu++98标志。

换言之，CMake 将自动“衰减”至最接近的标准。如果确实需要强制指定标准，禁用这种自动衰减调整，那么可以通过设置 **CXX_STANDARD_REQUIRED** 实现。

```cmake
set(CMAKE_CXX_STANDARD 11)  # 将 C++ 标准设置为 C++ 11
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # C++ 11 是强制要求，不会衰退至低版本
set(CMAKE_CXX_EXTENSIONS OFF)  # 禁止使用编译器特有扩展
```

### 三、编译配置

#### 3.1  编译优化
DEBUG/Release 模式对于的优化选项：
* **-O0**： 不做任何优化，这是默认的编译选项。
* **-O1**：不影响编译速度的前提下，尽量采用一些优化算法降低代码大小和可执行代码的运行速度
* **-O2**：牺牲部分编译速度，除了执行-O1 所执行的所有优化之外，还会采用几乎所有的目标配置支持的优化算法，用以提高目标代码的运行速度。
* **-O3**： 执行-O2 所有的优化选项，采取很多向量化算法，提高代码的并行执行程度，利用现代 CPU 中的流水线，Cache。

变量**CMAKE_BUILD_TYPE**默认有四个值：
* **DEBUG**：`-g` 默认就是不优化，-g 就是为了生成可调式文件 gdb。
* **Release **：`-O2` 默认采用 -O2。
* **RelWithDebInfo**：`-g -O2` 附带可调式信息
* **MinSizeRel**: `-O3` 尽可能优化

```cpp
set(CMAKE_BUILD_TYPE RelWithDebInfo)
```

**GCC需要指定生成的config是 Debug还是Release， 但是MSVC 直接生成DEBUG、Release 、MinSizeRel、RelWithDebInfo四个版本！**，在生成阶段无法控制配置。



### 4. 输出路径

设置输出文件应该存放在那些目录里面! CONFIG 就是CMAKE_BUILD_TYPE的值。

| 内置变量                                          | 名称                               |
| :------------------------------------------------ | :--------------------------------- |
| **1. CMAKE_ARCHIVE_OUTPUT_DIRECTORY**_\<CONFIG\>  | 表示静态库放在哪个目录里面         |
| **2. CMAKE_LIBRARY_OUTPUT_DIRECTORY**_\<CONFIG\>  | 表示动态库放在哪个目录里面         |
| **3. CMAKE_RUNTIME_LIBRARY_DIRECTORY**_\<CONFIG\> | 表示生成的可执行库放在哪个目录里面 |
| **4. EXECUTABLE_OUTPUT_PATH**                     | 可执行文件输出的存放路径           |
| **5. LIBRARY_OUTPUT_PATH**                        | 库文件输出的存放路径               |
| **6. CMAKE_CURRENT_LIST_DIR**                     | 当前CMakeList.txt所在目录！        |
| 7. **PDB_OUTPUT_DIRECTORY_\<CONFIG\>**            | pdb文件输出路径                    |

**Config的意思**

* RUNTIME_OUTPUT_DIRECTORY_\<CONFIG\>
  * RUNTIME_OUTPUT_DIRECTORY_DEBUG
  * RUNTIME_OUTPUT_DIRECTORY_RELEASE

**还可以通过属性设置输出路径**：

- [ARCHIVE_OUTPUT_DIRECTORY](http://cmake.org.cn/prop_tgt/ARCHIVE_OUTPUT_DIRECTORY.html)   归档输出路径
- [ARCHIVE_OUTPUT_DIRECTORY_\<CONFIG\>](http://cmake.org.cn/prop_tgt/ARCHIVE_OUTPUT_DIRECTORY_CONFIG.html)  某个config下的归档输出路径
- [ARCHIVE_OUTPUT_NAME](http://cmake.org.cn/prop_tgt/ARCHIVE_OUTPUT_NAME.html)  输出名
- [ARCHIVE_OUTPUT_NAME_\<CONFIG\>](http://cmake.org.cn/prop_tgt/ARCHIVE_OUTPUT_NAME_CONFIG.html) 某个config下的归档输出名称
- [PDB_OUTPUT_DIRECTORY](http://cmake.org.cn/prop_tgt/PDB_OUTPUT_DIRECTORY.html) 调试文件存放路径
- [PDB_OUTPUT_DIRECTORY_\<CONFIG\>](http://cmake.org.cn/prop_tgt/PDB_OUTPUT_DIRECTORY_CONFIG.html) 某个config下的调试文件存放路径

```cmake
set(SHARE_LIB_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/dll)

add_library(math SHARED src/mathTool/cal.cpp)

set_target_properties(math PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${SHARE_LIB_OUTPUT_PATH}
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${SHARE_LIB_OUTPUT_PATH}/debug
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SHARE_LIB_OUTPUT_PATH}/release
        DEBUG_POSTFIX d  #给库加一个 d的后缀
        #window 调试文件路径
        PDB_OUTPUT_DIRECTORY ${SHARE_LIB_OUTPUT_PATH}/pdb
        PDB_OUTPUT_DIRECTORY_DEBUG ${SHARE_LIB_OUTPUT_PATH}/pdb/debug
)

```



#### [4.1 source_group](#)

VS中在某一工程内，多个文件散乱的分布在 Source Files/ 目录和 Header Files 目录中, 不便于查看和调试代码，通过使用 source_group 命令, 可以使源码保持原本的目录组织方式 (当然也可以自己重新组织)，**一般用于MSVC**。

source_group() 用于定义生成 IDE 工程时的源码分组 (source groups). 有两种创建方式:

```cmake
source_group(<name> [FILES <src>...] [REGULAR_EXPRESSION <regex>])
source_group(TREE <root> [PREFIX <prefix>] [FILES <src>...])
```

例子：

```cmake
source_group(src FILES Epoll.cpp Task.cpp)
# MSVC Source Files 会有一个同级目录 src 里面有Epoll.cpp Task.cpp

source_group(TREE. include/timer FILES whellTimer.hpp mapTimer.hpp)
```



### [5. 系统信息](#)

| 系统信息变量                  | 名称                                           |
| :---------------------------- | :--------------------------------------------- |
| **1. CMAKE_MAJOR_VERSION**    | CMAKE主版本号，比如2.4.6中的2                  |
| **2. CMAKE_MINOR_VERSION**    | CMAKE次版本号，比如2.4.6中的4                  |
| **3. CMAKE_PATCH_VERSION**    | CMAKE补丁等级，比如2.4.6中的6                  |
| **4. CMAKE_SYSTEM**           | 系统名称，比如Linux-2.6.22                     |
| **5. CMAKE_SYSTEM_NAME**      | 不包含版本的系统名，比如Linux                  |
| **6. CMAKE_SYSTEM_VERSION**   | 系统版本，比如2.6.22                           |
| **7. CMAKE_SYSTEM_PROCESSOR** | 处理器名称，比如i686                           |
| **8. UNIX**                   | 在所有的类Unix平台为**TRUE** ，包括OSX和cygwin |
| **9. WIN32**                  | 在所有的Win32平台为**TRUE** ，包括cygwin       |

#### [5.1 CMake版本信息](#)

```cmake
message(STATUS "cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
# -- cmake version 3.22.1
```
