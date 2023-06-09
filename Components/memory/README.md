### [C++ 内存管理 - 八股文](#)

**介绍**:  C++ 八股取士、经典题目！

-----

#### [1. 什么叫内存泄漏](#)

*内存泄漏*（Memory Leak）是指程序中已动态分配的**堆内存**由于某种原因程序**未释放或无法释放**，造成系统内存的浪费，导致程序运行速度减慢甚至系统崩溃等严重后果。



#### [2. 内存溢出和内存泄露的区别](#)

内存溢出是指程序在申请内存时，没有足够的内存空间供其使用，出现out of memory。内存泄露是指程序在申请内存后，无法释放已申请的内存空间，一次内存泄露危害可以忽略，但内存泄露堆积后果很严重。




#### [3. C++系统运行时发生内存泄露，如何解决](#)

一般通过**top、htop指令**可以查看进程的内存是否发生泄漏问题，如果是自己写的代码发生了内存泄露，应该引入**内存池**来管理内存，多进程系统中每个进程配一个内存池，**内存池中打开日志文件**，查看哪个地方出了问题。第三方库内存泄露，就没法了，要么换库要么自己实现一个要么报告其官方Github。

同时可以使用内存泄露检测工具来判断是那部分代码出了问题：
* **valgrind** 是一款用于内存调试、内存泄漏检测以及性能分析的软件开发工具。
* **mtrace**

使用GC算法自动回收，lua就是在C语言上面用万行代码实现了一个虚拟机，实现垃圾回收，管理内存、语法解析。

