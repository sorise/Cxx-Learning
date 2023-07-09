### [CMake 全局构建参数](#)

**介绍**: CMake 通用编译与构建相关命令和变量。

----

- [x] [1. 全局构建与编译参数](#1-全局构建与编译参数)
- [x] [2. CPP的标准](#2-cpp的标准)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)



---

### 1. 全局构建与编译参数

- [add_compile_definitions](http://cmake.org.cn/command/add_compile_definitions.html)
- [add_compile_options](http://cmake.org.cn/command/add_compile_options.html)
- [add_custom_command](http://cmake.org.cn/command/add_custom_command.html)
- [add_custom_target](http://cmake.org.cn/command/add_custom_target.html)
- [add_definitions](http://cmake.org.cn/command/add_definitions.html)
- [add_dependencies](http://cmake.org.cn/command/add_dependencies.html)
- [add_executable](http://cmake.org.cn/command/add_executable.html)
- [add_library](http://cmake.org.cn/command/add_library.html)
- [add_link_options](http://cmake.org.cn/command/add_link_options.html)
- [add_test](http://cmake.org.cn/command/add_test.html)



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

