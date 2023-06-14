### [CMake 常见内置变量](#)

 **介绍**：一些只要使用CMake 管理项目就一定会用到的内置变量，或者大部分常见都会使用的变量。

----
- [x] [1. 与项目有关的变量](#1-与项目有关的变量)
- [x] [2. 项目构建有关的变量 ](#2-项目构建有关的变量)
- [x] [3. 描述系统的变量](#3-描述系统的变量 )
- [x] [4. 描述编译与输出的变量](#4-描述编译与输出的变量)

---

### [1.  与项目有关的变量](#)

包括记录项目的名称、版本信息之类的。

|内置变量|名称|
|:----|:----|
| **PROJECT_NAME** |项目的名称|
| **CMAKE_BINARY_DIR** |项目的二进制文件目录，即编译后的可执行文件和库文件的输出目录，源代码的最顶级目录（即最顶级 `CMakeLists.txt` 文件所在的位置）,这个变量的值永远不会改变。|
| **CMAKE_SOURCE_DIR** |项目的源文件目录，即包含 `CMakeLists.txt` 文件的目录，构建目录的最顶级目录。这个变量的值永远不会改变。|
| **PROJECT_BINARY_DIR** |项目的二进制文件目录，即编译后的可执行文件和库文件的输出目录|
| **PROJECT_SOURCE_DIR** |项目的源文件目录，即包含 `CMakeLists.txt` 文件的目录|
| **CMAKE_CXX_STANDARD** |CMAKE_CXX_STANDARD 是一个整数，表示要用的 C++ 标准。|

 项目名的另一大作用：会自动设置 **<项目名>_SOURCE_DIR**、**<项目名>_BINARY_DIR** 等变量。



#### [1.1  PROJECT_NAME](#)

这个变量记录了项目的名称，

```cmake
set(MUSE_PROJECT_NAME "muse")

project(${MUSE_PROJECT_NAME}
        VERSION         0.0.1.1
        DESCRIPTION     "cmake demo"
        HOMEPAGE_URL    "https://github.com/sorise"
        LANGUAGES       C CXX
)

message(STATUS "project name: ${PROJECT_NAME}")  #project name: muse

message(STATUS "major version: ${PROJECT_VERSION_MAJOR}")
message(STATUS "miner version: ${PROJECT_VERSION_MINOR}")
message(STATUS "patch version: ${PROJECT_VERSION_PATCH}")
message(STATUS "tweak version: ${CMAKE_PROJECT_VERSION_TWEAK}")
```



#### [1.2 PROJECT_VERSION_?](#)
在CMake中，通常使用三个数字的版本号（Major.Minor.Patch）来标识一个项目的版本。其中，CMAKE_PROJECT_VERSION_MAJOR表示主版本号，CMAKE_PROJECT_VERSION_MINOR表示次版本号，CMAKE_PROJECT_VERSION_PATCH表示修订号。而CMAKE_PROJECT_VERSION_TWEAK则是第四个数字，用于表示项目版本号的微调值。

|内置变量|名称|
|:----|:----|
|PROJECT_VERSION_MAJOR/CMAKE_PROJECT_VERSION_MAJOR|主版本号|
|PROJECT_VERSION_MINOR/CMAKE_PROJECT_VERSION_MINOR|次版本号|
|PROJECT_VERSION_PATCH/CMAKE_PROJECT_VERSION_PATCH|表示修订号|
|PROJECT_VERSION_TWEAK/CMAKE_PROJECT_VERSION_TWEAK|用于表示项目版本号的微调值。|
```cmake
message(STATUS "major version: ${PROJECT_VERSION_MAJOR}")  		#0
message(STATUS "miner version: ${PROJECT_VERSION_MINOR}")  		#0
message(STATUS "patch version: ${PROJECT_VERSION_PATCH}")  		#1
message(STATUS "tweak version: ${CMAKE_PROJECT_VERSION_TWEAK}")	#1

message(STATUS "major version: ${CMAKE_PROJECT_VERSION_MAJOR}") #0
message(STATUS "miner version: ${CMAKE_PROJECT_VERSION_MINOR}") #0
message(STATUS "patch version: ${CMAKE_PROJECT_VERSION_PATCH}") #1
message(STATUS "tweak version: ${CMAKE_PROJECT_VERSION_TWEAK}") #1
```



### [2. 项目构建有关的变量](#)
指定库的类型。

|内置变量|名称|
|:----|:----|
|BUILD_SHARED_LIBS|默认构建库的类型，静态还是动态库|
|CMAKE_COLOR_MAKEFILE|生成的 makefile 是否有颜色，默认是 ON。|
|CMAKE_BUILD_TYPE|确定构建的类型|

#### [2.1 BUILD_SHARED_LIBS](#)
用于指示是否构建共享库（Shared Library）。其默认值为OFF，表示构建静态库（Static Library）。

当将`BUILD_SHARED_LIBS`设置为ON时，CMake会将目标库编译为共享库，而不是静态库。这对于需要动态链接库文件的应用程序非常有用。通常情况下，我们可以在项目的顶层`CMakeLists.txt`文件中设置该变量：

```cmake
Copy Code# 设置构建共享库
set(BUILD_SHARED_LIBS ON)

# 添加子目录
add_subdirectory(src)
```

在上述例子中，我们将`BUILD_SHARED_LIBS`变量设置为ON，然后使用`add_subdirectory`命令来构建项目中的其他源文件。在这些源文件中，如果使用了`add_library`命令构建库目标，则可以根据`BUILD_SHARED_LIBS`的值生成共享库或静态库。

```cmake
Copy Code# 生成库目标
add_library(MyLib mylib.cpp)
```

如果`BUILD_SHARED_LIBS`的值为ON，则会生成共享库；如果该值为OFF，则会生成静态库。

使用`BUILD_SHARED_LIBS`变量可以让我们更加方便地切换目标库类型，提高代码的可重用性和可移植性。

#### [2.2 CMAKE_COLOR_MAKEFILE](#)
生成的 makefile 是否有颜色，默认是 ON。

#### [2.3 CMAKE_BUILD_TYPE](#)

CMAKE_BUILD_TYPE 是 CMake 中一个特殊的变量，用于控制构建类型，他的值可以是：

- `Debug`:调试模式，完全不优化，生成调试信息，方便调试程序
- `Release`:发布模式，优化程度最高，性能最佳，但是编译比 Debug 慢
- `MinSizeRel`:最小体积发布，生成的文件比 Release 更小，不完全优化，减少二进制体积
- `RelWithDebInfo`:带调试信息发布，生成的文件比 Release 更大，因为带有调试的符号信息
- - 这是为了让用户在使用程序出错时，能够反馈一些信息。

**默认**情况下 CMAKE_BUILD_TYPE 为空字符串，这时相当于 **Debug**。


### 3. 描述系统的变量 
如果环境匹配，对应值为ON，或者 值为 1

|内置变量|名称|
|:----|:----|
|MSVC|编译器是否是 MSVC|
|WIN32|操作系统是否是 windows|
|ANDROID|操作系统是否是 安卓|
|UNIX|操作系统是否是 unix|
|CMAKE_SYSTEM_NAME|目标系统的名称|



#### [3.1 CMAKE_SYSTEM_NAME](#)
在CMake中，`CMAKE_SYSTEM_NAME`是一个预定义的变量，用于表示目标系统的名称。该变量的取值通常是由 CMake 自动检测得出的，也可以手动设置。下面是一些常见的 `CMAKE_SYSTEM_NAME` 的取值：

- Linux：表示Linux操作系统。
- Darwin：表示苹果公司的操作系统，包括 macOS 和 iOS。
- Windows：表示 Windows 操作系统。
- FreeBSD：表示 FreeBSD 操作系统。
- Android：表示Android操作系统。
- iOS：表示苹果公司的iOS操作系统。
- watchOS：表示苹果公司的watchOS操作系统。
- tvOS：表示苹果公司的tvOS操作系统。

```cpp
message(STATUS "system name: " ${CMAKE_SYSTEM_NAME})  #Windows
```

### [4. 描述编译与输出的变量 ](#)

| 变量名称  | CMake变量          | 系统变量 |
| --------- | ------------------ | -------- |
| C编译器   | CMAKE_C_COMPILER   | CC       |
| C++编译器 | CMAKE_CXX_COMPILER | CXX      |
|C编译选项|	CMAKE_C_FLAGS|	CFLAGS|
|C++编译选项|	CMAKE_CXX_FLAGS|	CXXFLAGS|
|编译x64|	BUILD_USE_64BITS|	设置使用64位编译, ON 或者 OFF|
|输出目录设置|	CMAKE\_*\_OUTPUT_DIRECTORY|设置输出目录|
|设置文件后缀|CMAKE_\<CONFIG\>_POSTFIX |设置文件后缀|

#### [4.1 设置编译器的版本](#)

在实际的项目平台中可能安装有多个版本的编译器，可以通过如下两种方式设置编译器的版本。

直接在命令行中指定编译器
```shell
cmake .. -DCMAKE_CXX_COMPILER=/usr/local/gcc/bin/g++
```

在cmake中指定编译器

1.  **set (CMAKE_C_COMPILER "/usr/local/gcc/bin/gcc")**
2.  **set (CMAKE_CXX_COMPILER "/usr/local/gcc/bin/g++")**

#### [4.2 输出目录](#)
设置输出目录:

* CMAKE_ARCHIVE_OUTPUT_DIRECTORY是CMake中用于设定编译生成静态库文件（.a或.lib）的输出目录的变量。
* CMAKE_LIBRARY_OUTPUT_DIRECTORY是CMake中用于设定编译生成动态库态库文件（.so或.dll、.dylib）的输出目录的变量。
* CMAKE_RUNTIME_OUTPUT_DIRECTORY是CMake中用于设定编译生成可执行文件的输出目录的变量。

```cpp
# 全局设置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  #静态库存放地点
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  #动态库存放地点
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)	 #可执行文件存放目录

# 单个目标
set_target_properties( targets...
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
```

#### [4.3 CMAKE_\<CONFIG\>_POSTFIX](#)
设置文件后缀
```cpp
# 全局设置
set(CMAKE_DEBUG_POSTFIX "_debug")
set(CMAKE_RELEASE_POSTFIX "_release")

# 单个目标
set_target_properties( targets...
    PROPERTIES
    DEBUG_POSTFIX "_debug"
    RELEASE_POSTFIX "_release"
)
```