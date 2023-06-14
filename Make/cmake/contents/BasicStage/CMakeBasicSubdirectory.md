### [CMake 子目录](#)

 **介绍**：学习 指令add_subdirectory.

-----

#### add_subdirectory

添加子项目目录，如果有该条语句，就先会跑去执行子项目的cmake代码，这样会导致一些需要执行后立马生效的语句作用不到，比如include_directories和link_directories如果执行在这条语句后面。

则他们添加的目录在子项目中无法生效。有些命令如 `target_include_directories` 和 `target_link_directories` 是根据目标target是否被链接使用来生效的，所以这些命令的作用范围与执行顺序无关，
且恰好同一个cmake项目中产生的库文件是可以直接通过名称链接的，无论链接对象是在子目录还是父目录。

```cmake
add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
```
* **source_dir** :指定源CMakeLists.txt和代码文件所在的目录。如果它是一个相对路径，它将根据当前目录（典型用法）进行评估，但它也可能是一个绝对路径。
* **binary_dir** :通常 binaryDir 不需要指定，不指定的情况下，CMake 会在构建目录中对应的位置创建和源码目录对应的目录，用于存放构建输出。但是当 sourceDir 是源外路径的话，binaryDir 需要明确指定。
* **EXCLUDE_FROM_ALL** : 当指定了该参数，则子目录下的目标不会被父目录下的目标文件包含进去，父目录的CMakeLists.txt不会构建子目录的目标文件，必须在子目录下显式去构建。
例外情况：当父目录的目标依赖于子目录的目标，则子目录的目标仍然会被构建出来以满足依赖关系（例如使用了target_link_libraries）。



#### 变量的影响

甚至可能有多个使用相同源代码树的构建树。因此，开发人员需要一些 CMake 的帮助来确定感兴趣的目录。为此，CMake 提供了一些变量来跟踪当前正在处理
的 CMakeLists.txt 文件的源和二进制目录。以下是一些只读变量，随着每个文件被 CMake 处理，这些变量会自动更新。它们始终包含绝对路径。

* **CMAKE_SOURCE_DIR**
    源代码的最顶级目录（即最顶级 CMakeLists.txt 文件所在的位置）,这个变量的值永远不会改变。
* **CMAKE_BINARY_DIR**
    构建目录的最顶级目录。这个变量的值永远不会改变。
* **CMAKE_CURRENT_SOURCE_DIR**
    当前正在被 CMake 处理的 CMakeLists.txt 文件所在的目录。每当由 add_subdirectory() 调用处理新文件时，它都会更新，当处理该目录完成时，它会被还原回原来的值。
* **CMAKE_CURRENT_BINARY_DIR**
    由 CMake 处理的当前 CMakeLists.txt 文件所对应的构建目录。每次调用 add_subdirectory() 时都会更改该目录，当 add_subdirectory() 返回时将其恢复。



```tree
├── CMakeLists.txt
├── doc
├── include
│   ├── prints.h
│   └── toolkits.hpp
├── levelDB
│   ├── CMakeLists.txt
│   └── leveldb.cpp
├── main.cpp
└── src
    └── prints.cpp
```

**重点：前后位置问题** 主要相对于其他指令的位置问题!
```shell
include_directories(include)
link_directories(lib)

#增加子目录 但是子目录可以使用  include 的头文件 和连接目录 lib
add_subdirectory(levelDB)

include_directories(include)

link_directories(lib)
```



#### 子目录相关的作用域详解

add_subdirectory() 命令引入一个新的子目录的同时，也引入了新的作用域，相对于调用 add_subdirectory() 命令的 CMakeLists.txt 所在的作用域来说，通过 add_subdirectory() 命令引入的新的作用域叫做子作用域。其行为类似于 C/C++ 语言中调用一个新的函数。

- 调用 add_subdirectory() 命令的时候，当前作用域内的变量均会复制一份到子作用域，子作用域中对这些复制的变量进行操作不会影响到当前作用域中这些变量的值。
- 在子作用域中定义的新的变量对父作用域是不可见的。

下面我们看一个例子，加深一下理解：
- CMakeLists.txt
  ```cmake
  set(myVar foo)
  
  message("Parent (before): myVar    = ${myVar}")
  message("Parent (before): childVar = ${childVar}")
  
  add_subdirectory(subdir)
  
  message("Parent (after):  myVar    = ${myVar}")
  message("Parent (after):  childVar = ${childVar}")
  ```
- subdir/CMakeLists.txt
  ```cmake
  message("Child  (before): myVar    = ${myVar}")
  message("Child  (before): childVar = ${childVar}")
  
  set(myVar bar)
  set(childVar fuzz)
  
  message("Child  (after):  myVar    = ${myVar}")
  message("Child  (after):  childVar = ${childVar}")
  ```

我们之前在讲 set() 命令的时候，有提到 PARENT_SCOPE 关键字，在 add_subdirectory() 命令使用的时候，PARENT_SCOPE 关键字通常就可以派上用场了，下面我们一起来看一个例子：
- CMakeLists.txt
  ```cmake
  set(myVar foo)
  message("Parent (before): myVar = ${myVar}")
  add_subdirectory(subdir)
  message("Parent (after):  myVar = ${myVar}")
  ```
- subdir/CMakeLists.txt
  ```cmake
  message("Child  (before): myVar = ${myVar}")
  set(myVar bar PARENT_SCOPE)
  message("Child  (after):  myVar = ${myVar}")
  ```