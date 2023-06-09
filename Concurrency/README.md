### [C++ Concurrency](#)
**介绍**: C++ 11标准最重要的新特性之一，就是在**原生语言层面支持多线程**，这使得C++程序无须依赖于平台专属扩展实现程序并发多线程！
因而我们可以写出**可移植、行为确定的** 多线程代码。

----


**C++ 98** 标准并没有采纳线程！于是使用C语言的多线程 **API**(符合POSIX标准的C语言多线程接口/Windows系统的多线程API！)，**C++ 11** 以 **Boost线程库** 为基础固定了内存模型，
区分不同线程，扩增了新类，**C++14**、**C++17**对此进一步为多线程编程提供了支持！

**RAII(Resource Acquisition Is Initialization)**，资源获取就是初始化，是C++等编程语言常用的管理资源、避免内存泄露的方法。它保证在任何情况下，使用对象时先构造对象，最后析构对象，是线程管理的常用手法！



**笔记目录:**

* **00.** C++ 并发相关概念和原理 [->](./Theory)
* **01.** C++ 线程初探 [->](./contents/CPPThreadControl.md)
* **02.** C++ 在线程间共享数据 [->](./contents/CPPThreadShareData.md)
* **03.** C++ 并发操作的同步 [->](./contents/CPPFutureAsync.md)
* **04.** C++ 内存模型与原子操作 [->](./contents/CPPAtomicAndMemoryModel.md)
* **05.** C++ 线程池初探 [->](./contents/CPPThreadPoolEnterDoor.md)
* **06.** C++ 无锁数据结构 [->](./contents/CPPThreadlockFreeDT.md) 



**八股内容**：

* **01**. [八股基本问题索引](./Theory/index.md)
* **02**. [协程的基本理论](./coroutine)
* **03**. [信号量和锁的理论问题 ](./Theory/BasicQuestion.md)


----
last later: 2023-04-03 14:28 
