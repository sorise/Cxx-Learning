# [CMake Tool](https://cmake.org/)

**CMake**是一个开源、跨平台的**编译**、**测试**和**打包**工具，它使用比较简单的语言描述编译、安装的过程，输出**Makefile** 或者 **project** 文件，再去执行构建。
**GCC（GNU Compiler Collection）**将源文件**编译（Compile）** 成可执行文件或者库文件， 而当需要编译的东西很多时，需要说明先编译什么，后编译什么，这个过程称为构建 **（Build）**。
常用的工具是 **make**，对应的定义构建过程的文件为 **Makefile**，而编写 **Makefile** 对于大型项目又比较复杂，通过 **CMake** 就可以使用更加简洁的语法定义构建的流程，
**CMake**定义构建过程的文件为 **CMakeLists.txt**。

### 1. CMake 的基础知识
---

0. [CMake 初体验](./contents/CMakeStarting.md)
1. [CMake 简单使用](./contents/CMakeUsingSImple.md)
2. [CMake 变量](./contents/CMakeVariables.md)
3. [CMake 内置变量](./contents/CMakeInternalVariable.md)
4. [CMake 指令](./contents/CMakeInstructions.md)
5. [CMake 结构化指令](./contents/CMakeStructuredInstruction.md)














----

#### 参考网站

- cmake.org.cn [http://cmake.org.cn/](http://cmake.org.cn/)
- cmake 菜谱 [https://www.bookstack.cn/books/CMake-Cookbook](https://www.bookstack.cn/books/CMake-Cookbook)
