### [CMake 自动查找所有源代码和头文件](#)

 **介绍**：介绍命令 aux_source_directory 和 file 命令， 自动将一个文件中的所有文件合成一个列表，而不是手动一个个添加。

-----

- [x] [1. aux_source_directory ](#1-aux_source_directory )
- [x] [2. file 命令](#2-file-命令)

-----

### [1. aux_source_directory ](#)

aux_source_directory是CMake中用于查找指定目录下所有符合特定模式的**源文件**，并将其存储到一个变量中的函数， 不包括头文件哦！

发现一个目录下所有的源代码文件并将列表存储在一个变量中，这个指令临时被用来自动构建源文件列表。

```cmake
aux_source_directory(<dir> <variable>)
```

使用例子：

```cmake
# 定义SRC变量，其值为当前目录下所有的源代码文件
aux_source_directory(. SRC_CPPS)
# 编译SRC变量所代表的源代码文件，生成main可执行文件
add_executable(main ${SRC_CPPS})
```



#### [1.1 使用步骤](#)

使用该函数需要遵循以下步骤：

1. 在CMakeLists.txt文件中调用aux_source_directory函数，并指定需要查找源文件的目录和符合特定模式的文件类型，例如：

```cmake
aux_source_directory(src SRCS)
```

1. CMake会在src目录下查找所有符合模式的源文件，并将它们的路径存储到SRCS变量中，例如：

```cmake
aux_source_directory(src SRCS)
```

将src目录下所有的.cpp和.c文件的路径存储到SRCS变量中。

1. 最后使用add_executable或add_library添加需要编译的源文件，例如：

```cmake
add_executable(myapp ${SRCS})
```

示例代码：

```cmake
# CMakeLists.txt文件
cmake_minimum_required(VERSION 3.5)
project(myproject)

# 查找所有源文件
aux_source_directory(src SRCS)

# 添加可执行文件
add_executable(myapp ${SRCS})
```

在上面的示例中，aux_source_directory函数会在src目录下查找所有源文件，并将它们的路径存储到SRCS变量中，然后使用add_executable函数将这些源文件编译生成myapp可执行文件。

##### 

### [2. file 命令](#) 

这个命令实在是太强大了，你如果翻一翻这个官方文档就会发现它具备几乎文件系统的所有功能，什么读写文件啊，什么从网上下载文件，本地上传文件之类的它都有，计算文件的相对路径，路径转化等等!    官方 API： [http://cmake.org.cn/command/file.html](http://cmake.org.cn/command/file.html)



#### [2.1 常用操作](#)

常用的file命令包括：

* file(GLOB files pattern)：查找某个目录下符合特定模式的文件，并将结果列表存储到files变量中。例如：
```cmake
file(GLOB sources src/*.cpp)
file(GLOB H_FILE "${INCLUDE_PATH}/serilazer/*.h*") 
```
将src目录下所有的.cpp文件的路径存储到sources变量中。

* file(COPY src DESTINATION dst)：复制源目录(src)下的所有文件到目标目录(dst)。例如：

```cmake
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
```
将data目录下的文件复制到编译目录（CMAKE_BINARY_DIR）中。

* file(MAKE_DIRECTORY dir)：创建指定目录，如果该目录已经存在，则什么也不做。例如：

```cmake
file(MAKE_DIRECTORY output)
```
在当前目录下创建output目录。

* file(REMOVE_RECURSE dir)：删除指定目录，如果该目录不存在，则什么也不做。例如：

```cmake
file(REMOVE_RECURSE build)
```
删除build目录及其所有内容。

* file(STRINGS filename variable)：读取指定文件中的每一行，并将结果存储在变量中。例如：

```cmake
file(STRINGS config.txt CONFIG_CONTENT)
```
读取config.txt文件中的每一行，并将结果存储到CONFIG_CONTENT变量中。



**示例代码**：

```cmake
# CMakeLists.txt文件
cmake_minimum_required(VERSION 3.5)
project(myproject)

# 查找所有源文件
file(GLOB sources src/*.cpp)

# 复制data目录到编译目录中
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

# 创建output目录
file(MAKE_DIRECTORY output)

# 删除build目录
file(REMOVE_RECURSE build)

# 读取config.txt文件
file(STRINGS config.txt CONFIG_CONTENT)

# 添加可执行文件
add_executable(myapp ${sources})
```

在上面的示例中，通过使用file命令操作文件和目录，可以方便地完成复制、创建、删除、读取等一系列操作，并且将这些操作集成到构建过程中



#### 2.2 file  DOWNLOAD

**DOWNLOAD**: file命令中也可以使用DOWNLOAD选项来下载网络文件。使用该选项需要遵循以下步骤：

1. 在CMakeLists.txt中使用file(DOWNLOAD)命令，指定需要下载的文件URL和保存位置，例如：

```cmake
file(DOWNLOAD https://example.com/file.txt ${CMAKE_BINARY_DIR}/file.txt)
```

将从https://example.com/file.txt下载文件，并将其保存到${CMAKE_BINARY_DIR}/file.txt中。

2. 如果需要验证下载文件的完整性，可以使用SHA256或MD5哈希值验证。首先将哈希值存储到一个文件中，然后使用file(DOWNLOAD)命令时指定该文件并启用验证选项，例如：

```cmake
file(DOWNLOAD https://example.com/file.txt ${CMAKE_BINARY_DIR}/file.txt
     EXPECTED_HASH SHA256=abc123def456ghijk789)
```

将从https://example.com/file.txt下载文件，并将其保存到${CMAKE_BINARY_DIR}/file.txt中，同时使用SHA256哈希值abc123def456ghijk789验证下载文件的完整性。

**示例代码**：

```cmake
# CMakeLists.txt文件
cmake_minimum_required(VERSION 3.5)
project(myproject)

# 下载文件并验证完整性
file(DOWNLOAD https://example.com/data.zip ${CMAKE_BINARY_DIR}/data.zip
     EXPECTED_HASH SHA256=abc123def456ghijk789)

# 解压缩文件
execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/data.zip
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# 添加可执行文件
add_executable(myapp main.cpp)
```