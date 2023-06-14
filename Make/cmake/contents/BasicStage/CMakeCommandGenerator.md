### [CMake 命令实现程序的分布生成与调试信息](#)
 **介绍**：预处理、编译、汇编、链接、运行，分步骤进行，以便排查错误。

### 1. 先生成项目

有点小问题的感觉：

```cmake
cmake -S . -B Build
```

查看所有的目标：
```cmake
cmake --build . --target help
```

预处理
```cmake
cmake --build . --target first_cmake.i
```

编译
```cmake
cmake --build --target first_cmake.s
```

汇编
```cmake
cmake --build --target first_cmake.o
```

**cmake 程序分步生成、指定项目和清理**





#### 1.1 windows 下必须运行 vs 控制台

```cmake
cmake -S . -B nmake -G "NMake Makefile
```
cmake 命令构建指定项目和清理

```cmake
cmake --build . --target help
cmake --build . --target clean
```



### 2. 调试打印生成的具体指令

**cmake 调试打印生成的具体指令** ，需要设置变量 **CMAKE_VERBOSE_MAKEFILE**

```cmake
##显示详细的生成日志
set(CMAKE_VERBOSE_MAKEFILE ON) #默认是 OFF
```

也可以用命令行实现：

```cmake
cmake --build . -v  
```
第一次运行就要加-v，不然日志不完整，可以清理后重新生成