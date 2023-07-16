## [Coroutine 协程](#)

**Introduction**： 协程（Coroutine）是一个可以**暂停**和**恢复**的函数，线程（Thread）都是用于并发编程的概念，但它们的实现方式和工作原理有所不同。





### 一、背景

[**由于主流的操作系统都采取1对1的线程模型**](#)，用户态线程和内核态线程具有一对一关系，可以认为用户态线程的执行几乎完全受到操作系统调度器的管理。

可是随着计算机的发展，**应用程序也变得越来越复杂**，在这些复杂的应用程序中，每个线程各司其职，有的负责计算，有的负责网络通信，有的大量读写内存.与操作系统源度器相比，应用程序对线程的语义和执行状态更加了解。

因此可能做出更优的调度决策。此外，**用户态线程更加轻量级，比内核态线程的创建和切换的开销要低得多**，更多地使用**用户态线程**有利于提高整个系统的可扩展性。在这样的背景下，操作系统开始提供更多对用户态线程，即**纤程(fiber)**的支持， <span style="color:#2c73ff;font-weight:600;font-family:宋体" >**用户态线程和内核态线程的关系也由一对一向多对一扩多**</span>。

> 取自陈海波&夏虞斌老师的《现代操作系统原理与实现》











协程和线程是两种不同的并发编程模型，它们有以下几点区别： 

1. 调度方式：线程是由**操作系统内核**进行**调度**，而协程是由**程序自身控制**的，没有操作系统的干预。

2.  并发性：线程是并发执行的，多个线程可以同时运行在多个处理器上，而**协程是顺序执行的，同一时间只有一个协程在运行**。

3. 内存占用：一个进程可以包含多个线程，每个线程有自己的执行上下文和栈空间，线程需要一定的内存资源来维护线程栈和上下文切换的开销，而协程的内存占用相对较小，只需要保存协程的上下文信息。 

4. 切换开销：线程的切换需要保存和恢复线程的上下文信息，开销较大，而协程的切换只需要保存和恢复协程的上下文信息，开销较小。 

5. 编程模型：线程的并发编程模型是基于共享内存的，需要使用锁等机制来保证数据的一致性，而协程的并发编程模型是基于消息传递的，通过消息的发送和接收来实现协程之间的通信。 

   

> 总的来说，协程相对于线程来说更轻量级，因为协程的切换开销较小，不需要涉及操作系统的调度和上下文切换，更适合于高并发的场景，能够更高效地利用系统资源。但是协程需要程序员自己管理调度和通信，编程复杂度相对较高。 
>
> 线程的优势在于它可以利用多核处理器的并行能力，同时执行多个线程，提高程序的处理能力, 但是协程就不行。





参考文章：

> [什么是协程 ---- csdn 测试狗一枚](https://blog.csdn.net/sanmi8276/article/details/111375619)