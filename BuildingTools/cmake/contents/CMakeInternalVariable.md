### [CMake 内置变量](#)
**介绍**: CMake 有许多的内置变量，包括设置编译参数变量，用于检测环境的变量、项目相关变量！

-----

- [1. CMake 常用内置变量](#1-cmake-常用内置变量)
- [2. CMake 输出控制变量](#2-cmake-输出控制变量)
- [3. CMake 系统信息](#3-cmake-系统信息)
- [4. CMake 编译控制命令](#4-cmake-编译控制命令)

----

### [1. CMake 常用内置变量](#)
下面只列出了部分变量的作用，更多的变量请查看文档： [CMake 内置变量...](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html)

|内置变量|名称|
|:----|:----|
| **1. PROJECT_NAME**|项目的名称|
| **2. CMAKE_BINARY_DIR**|项目的二进制文件目录，即编译后的可执行文件和库文件的输出目录，源代码的最顶级目录（即最顶级 CMakeLists.txt 文件所在的位置）,这个变量的值永远不会改变。|
| **3. CMAKE_SOURCE_DIR**|项目的源文件目录，即包含CMakeLists.txt文件的目录，构建目录的最顶级目录。这个变量的值永远不会改变。|
| **4. PROJECT_BINARY_DIR**|项目的二进制文件目录，即编译后的可执行文件和库文件的输出目录|
| **5. PROJECT_SOURCE_DIR**|项目的源文件目录，即包含CMakeLists.txt文件的目录|
| **6. CMAKE_C_STANDARD** |指定C语言的标准版本|
| **7. CMAKE_CXX_STANDARD** |指定C++语言的标准版本|
| **8. CMAKE_EXE_LINKER_FLAGS** |指定链接可执行文件时使用的链接选项|
| **9. CMAKE_SYSTEM_NAME**|指定当前操作系统名称（如Windows、Linux等）|
| **10. CMAKE_SYSTEM_PROCESSOR** |指定当前处理器的类型（如x86、x86_64等）|
| **11. CMAKE_CXX_STANDARD_REQUIRED** | 设置指定的C++编译器版本是必须的，如果不设置，或者为OFF，则指定版本不可用时，会使用上一版本。 |
| **12. CMAKE_CURRENT_SOURCE_DIR**| 当前正在被 CMake 处理的 CMakeLists.txt 文件所在的目录。每当由 add_subdirectory() 调用处理新文件时，它都会更新，当处理该目录完成时，它会被还原回原来的值。|
| **13. CMAKE_CURRENT_BINARY_DIR** |由 CMake 处理的当前 CMakeLists.txt 文件所对应的构建目录。每次调用 add_subdirectory() 时都会更改该目录，当 add_subdirectory() 返回时将其恢复。 |
| **...** |**...** |


#### [1.1 防止内部构建](#)
```cmake
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "内部构建完全不支持！")
endif()
```

#### [1.2 CMAKE_EXE_LINKER_FLAGS](#)
强制使用静态连接，默认使用的是动态链接！
```cmake
set(CMAKE_EXE_LINKER_FLAGS "-static")
```

### [2. CMake 输出控制变量](#)
设置输出文件应该存放在那些目录里面!

|内置变量|名称|
|:----|:----|
| **1. CMAKE_ARCHIVE_OUTPUT_DIRECTORY**|表示静态库放在哪个目录里面|
| **2. CMAKE_LIBRARY_OUTPUT_DIRECTORY**|表示动态库放在哪个目录里面|
| **3. CMAKE_RUNTIME_LIBRARY_DIRECTORY**|表示生成的可执行库放在哪个目录里面|
| **4. EXECUTABLE_OUTPUT_PATH** |可执行文件输出的存放路径|
| **5. LIBRARY_OUTPUT_PATH** |库文件输出的存放路径|
| **6. CMAKE_CURRENT_LIST_DIR**|当前CMakeList.txt所在目录！|

#### [2.1 输出目录](#)
设置输出存放目录的三个内置变量

```cmake
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/lib)  #静态库存放目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/bin)  #可执行文件存放目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/bin)  #动态库存放目录
```

#### [2.2 CMAKE_CURRENT_LIST_DIR](#)
当前CMakeList.txt所在目录！

```cmake
message(STATUS "test: ${CMAKE_CURRENT_LIST_DIR}")
```

### [3. CMake 系统信息](#)

|系统信息变量|名称|
|:----|:----|
| **1. CMAKE_MAJOR_VERSION** |CMAKE主版本号，比如2.4.6中的2|
| **2. CMAKE_MINOR_VERSION** |CMAKE次版本号，比如2.4.6中的4|
| **3. CMAKE_PATCH_VERSION** |CMAKE补丁等级，比如2.4.6中的6|
| **4. CMAKE_SYSTEM** | 系统名称，比如Linux-2.6.22 |
| **5. CMAKE_SYSTEM_NAME** | 不包含版本的系统名，比如Linux |
| **6. CMAKE_SYSTEM_VERSION** | 系统版本，比如2.6.22 |
| **7. CMAKE_SYSTEM_PROCESSOR** | 处理器名称，比如i686 |
| **8. UNIX**|在所有的类Unix平台为**TRUE** ，包括OSX和cygwin|
| **9. WIN32**|在所有的Win32平台为**TRUE** ，包括cygwin|

#### [3.1 CMake版本信息](#)

```cmake
message(STATUS "cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
# -- cmake version 3.22.1
```

### [4. CMake 编译控制命令](#)

|系统信息变量|名称|
|:----|:----|
| **CMAKE_C_COMPILER** |指定C编译器，通常，CMake运行时能够自动检测C语言编译器。进行嵌入式系统开发时，通常需要设置此变量，指定交叉编译器。默认是/usr/bin/gcc。|
| **CMAKE_CXX_COMPILER** |  指定C++编译器，默认 /usr/bin/g++。|
| **CMAKE_BUILD_TYPE** |编译类型(Debug, Release)|
| **CMAKE_CXX_FLAGS** |指定编译C++代码时使用的编译选项, 也可以通过指令 ADD_DEFINITIONS()|
| **CMAKE_C_FLAGS** |指定编译C代码时使用的编译选项,也可以通过指令 ADD_DEFINITIONS()|
|**CMAKE_\<Lang\>_COMPILER_LOADED**|如果为项目启用了语言\<lang\>，则将设置为TRUE。|
|**CMAKE_\<Lang\>_COMPILER_ID**|编译器标识字符串|
|**...**| |
| **...** | **...** |

#### [4.2 CMAKE_BUILD_TYPE](#)
设置一个默认的构建类型(本例中是Release)，并打印一条消息。要注意的是，该变量被设置为缓存变量，可以通过缓存进行编辑：
默认值是 debug
```cmake
#设定编译类型为debug，调试时需要选择debug
set(CMAKE_BUILD_TYPE Debug)
#设定编译类型为release，发布时需要选择release
set(CMAKE_BUILD_TYPE Release)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
```
**通过命令设置:**

```shell
cmake -D CMAKE_BUILD_TYPE=Release  ./Release
```

#### [4.4 CMAKE_CXX_FLAGS](#)

```cpp
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
```
CMAKE_CXX_FLAGS这种大写的变量，一般都是CMake预定义的内建变量，且他们是全局的。该变量可用于设置编译选项。直接使用set修改其值即可。

除了修改该变量的值之外，还可以通过add_compile_options命令来设置编译选项。它们的区别是：
* add_compile_options 命令添加的编译选项是针对所有编译器的(包括c和c++编译器)
* CMAKE_C_FLAGS或CMAKE_CXX_FLAGS变量则是分别只针对c和c++编译器

#### [4.X CMAKE_\<LANG\>_COMPILER_ID](#)
GCC用于GNU编译器集合，AppleClang用于macOS上的Clang, MSVC用于Microsoft Visual Studio编译器

```cmake
message(STATUS ${CMAKE_CXX_COMPILER_ID}) #GNU
message(STATUS ${CMAKE_C_COMPILER_ID}) #GNU
message(STATUS ${CMAKE_Java_COMPILER_ID}) #
```

#### [4.X 编译参数](#)

```cmake
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  list(APPEND CXX_FLAGS "-fno-rtti" "-fno-exceptions")
  list(APPEND CXX_FLAGS_DEBUG "-Wsuggest-final-types" "-Wsuggest-final-methods" "-Wsuggest-override")
  list(APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  list(APPEND CXX_FLAGS "-fno-rtti" "-fno-exceptions" "-Qunused-arguments" "-fcolor-diagnostics")
  list(APPEND CXX_FLAGS_DEBUG "-Wdocumentation")
  list(APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()
```

### [5. 模块相关](#)

|系统信息变量|名称|
|:----|:----|
| **CMAKE_MODULE_PATH** |用分号分隔的目录列表，用于在检查CMake附带的默认模块之前，由include或find_package命令加载的CMake模块的搜索路径。|


#### [5.1 CMAKE_MODULE_PATH](#)
用分号分隔的目录列表，用于在检查CMake附带的默认模块之前，由include或find_package命令加载的CMake模块的搜索路径。

```cmake
include(<file|module> [OPTIONAL] [RESULT_VARIABLE <var>]
                      [NO_POLICY_SCOPE])
```
        

#### [打印变量](#)
**打印这些变量！**

```shell
# 为了分行确定输出内容
message("")

message("0.show_vars_BINARY_DIR = ${show_vars_BINARY_DIR}")
message("1.PROJECT_BINARY_DIR = ${PROJECT_BINARY_DIR}")
message("2.PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("3.CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("4.CMAKE_CURRRENT_BINARY_DIR = ${CMAKE_CURRRENT_BINARY_DIR}")
message("5.CMAKE_CURRENT_LIST_FILE = ${CMAKE_CURRENT_LIST_FILE}")
message("6.CMAKE_CURRENT_LIST_LINE = ${CMAKE_CURRENT_LIST_LINE}")
message("7.CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message("8.CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("9.EXECUTABLE_OUTPUT_PATH = ${EXECUTABLE_OUTPUT_PATH}")
message("10.LIBRARY_OUTPUT_PATH = ${LIBRARY_OUTPUT_PATH}")
message("11.PROJECT_NAME = ${PROJECT_NAME}")
message("12.PROJECT_VERSION_MAJOR = ${PROJECT_VERSION_MAJOR}")
message("13.PROJECT_VERSION_MINOR = ${PROJECT_VERSION_MINOR}")
message("14.PROJECT_VERSION_PATCH = ${PROJECT_VERSION_PATCH}")
message("15.CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message("16.CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
message("17.CMAKE_SYSTEM_VERSION = ${CMAKE_SYSTEM_VERSION}")
message("18.BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message("19.CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
message("20.CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message("21.CMAKE_SYSTEM_PROCESSOR   = ${CMAKE_SYSTEM_PROCESSOR}")
message("22.UNIX = ${UNIX}")
message("23.WIN32   = ${WIN32}")
message("24.CMAKE_BUILD_TYPE   = ${CMAKE_BUILD_TYPE}")
message("25.CMAKE_INCLUDE_PATH   = ${CMAKE_INCLUDE_PATH}")
message("26.LIBRARY_OUTPUT_PATH   = ${LIBRARY_OUTPUT_PATH}")
message("27.CMAKE_CXX_COMPILER   = ${CMAKE_CXX_COMPILER}")
message("28.CMAKE_C_COMPILER   = ${CMAKE_C_COMPILER}")
# 为了分行确定输出内容
message("")
```

**输出：**
```
0.show_vars_BINARY_DIR = 
1.PROJECT_BINARY_DIR = /home/remix/coding/cpp/network/build
2.PROJECT_SOURCE_DIR = /home/remix/coding/cpp/network
3.CMAKE_CURRENT_SOURCE_DIR = /home/remix/coding/cpp/network
4.CMAKE_CURRRENT_BINARY_DIR = 
5.CMAKE_CURRENT_LIST_FILE = /home/remix/coding/cpp/network/CMakeLists.txt
6.CMAKE_CURRENT_LIST_LINE = 19
7.CMAKE_MODULE_PATH = 
8.CMAKE_SOURCE_DIR = /home/remix/coding/cpp/network
9.EXECUTABLE_OUTPUT_PATH = 
10.LIBRARY_OUTPUT_PATH = 
11.PROJECT_NAME = network
12.PROJECT_VERSION_MAJOR = 1
13.PROJECT_VERSION_MINOR = 0
14.PROJECT_VERSION_PATCH = 0
15.CMAKE_SYSTEM = Linux-5.19.0-35-generic
16.CMAKE_SYSTEM_NAME = Linux
17.CMAKE_SYSTEM_VERSION = 5.19.0-35-generic
18.BUILD_SHARED_LIBS = 
19.CMAKE_C_FLAGS = 
20.CMAKE_CXX_FLAGS = 
21.CMAKE_SYSTEM_PROCESSOR   = x86_64
22.UNIX = 1
23.WIN32   = 
24.CMAKE_BUILD_TYPE   = Debug
25.CMAKE_INCLUDE_PATH   = 
26.LIBRARY_OUTPUT_PATH   = 
27.CMAKE_CXX_COMPILER   = /bin/g++
28.CMAKE_C_COMPILER   = /bin/gcc
```


#### [通过变量检测环境执行不同的cmake代码](#)

```cmakelists
#判断当前的操作系统
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(my-logger PUBLIC fmt-header-only pthread)
    message(STATUS "Now is Linux")
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(my-logger PUBLIC fmt-header-only ws2_32)
    message(STATUS "Now is windows")
endif ()
#判断当前使用的编译器
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # Do something for GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # Do something for Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Microsoft")
  # Do something for Microsoft Visual C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # Do something for Clang
endif()

#判断当前的系统架构
if (CMAKE_SYSTEM_PROCESSOR MATCHES "i.86|x86|x86_64|AMD64")
  # Do something for x86 architecture
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
  # Do something for ARM architecture
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(mips|mipsel|mips64)")
  # Do something for MIPS architecture
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc64)")
  # Do something for PowerPC architecture
endif()
```