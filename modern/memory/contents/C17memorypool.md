### C++ 17 标准内存池

**介绍**： C++17 引入了boost库中的内存池作为标准内存池，接口在头文件 [#include\<memory_resource\>](https://zh.cppreference.com/w/cpp/header/memory_resource) 中。

----



### [1. memory_resource](#)

C++17引入了**std::pmr::memory_resource**类，它是一个抽象基类，用于支持多态内存资源和内存分配器，针对封装内存资源类的无界集。 具有**默认**和**复制**构造函数、复制运算符、虚析构函数。

**memory_resource**类定义了一组虚拟成员函数，用于在派生类中实现特定的内存管理行为。

| 公开成员函数                                                 | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [allocate](https://zh.cppreference.com/w/cpp/memory/memory_resource/allocate) | 分配指定大小的内存，并返回指向分配内存的指针。还可以指定对齐方式，默认为`alignof(std::max_align_t)` |
| [deallocate](https://zh.cppreference.com/w/cpp/memory/memory_resource/deallocate) | 释放先前分配的内存。需要传入要释放的内存指针、大小和对齐方式，默认为`alignof(std::max_align_t)`。 |
| [is_equal](https://zh.cppreference.com/w/cpp/memory/memory_resource/is_equal) | 比较两个内存资源对象是否相等。                               |



**allocate**: 字节数参数bytes必须是2的次方！

```cpp
void* allocate(std::size_t bytes, std::size_t alignment = alignof(std::max_align_t));
```

| 私有成员函数                                                 | 说明                                               |
| ------------------------------------------------------------ | -------------------------------------------------- |
| [do_allocate](https://zh.cppreference.com/w/cpp/memory/memory_resource/do_allocate)[虚] | 分配内存 (虚私有成员函数)                          |
| [do_deallocate](https://zh.cppreference.com/w/cpp/memory/memory_resource/do_deallocate)[虚] | 解分配内存 (虚私有成员函数)                        |
| [do_is_equal](https://zh.cppreference.com/w/cpp/memory/memory_resource/do_is_equal)[虚] | 与另一 `memory_resource` 比较相等 (虚私有成员函数) |



<img src="./assets/image-20230715162336102.png" alt="image-20230715162336102" width="800px" />





### [2. pool_options](#)

C++标准库中的**std::pmr::pool_options**是一个结构体，用于配置和控制内存池的行为。它在C++17中引入，可以用于自定义内存资源类（例如std::pmr::unsynchronized_pool_resource、std::synchronized_pool_resource）的参数。

`std::pmr::pool_options`结构体具有以下成员变量：

```cpp
struct std::pmr::pool_options {
    size_t max_blocks_per_chunk = std::pmr::default_memory_resource(); //普通数据库的大小
    size_t largest_required_pool_block = 0; //大数据块的字节数量
}; 

```

* **max_blocks_per_chunk**: 每个内部分配块的最大数量。默认值为std::pmr::default_memory_resource()。若 `max_blocks_per_chunk` 的值为零或大于一个实现定义极限，则使用极限代替。实现可以选择使用小于指定于此域的值，并可对不同池使用不同值
* **largest_required_pool_block** : 要求使用分池机制实现的最大分配大小。分配大于此门槛的单个块的尝试会直接从上游 [std::pmr::memory_resource](https://zh.cppreference.com/w/cpp/memory/memory_resource) 分配。若 `largest_required_pool_block` 为零或大于实现定义的极限，则使用极限代替。



```cpp
std::pmr::pool_options option;
option.largest_required_pool_block = 1024 * 1024 * 10;  //大数据块的大小 10M
option.max_blocks_per_chunk = 1024 * 1024 * 100; //普通数据块的大小 10M
```

largest_required_pool_block 表示： **超过多大的数据会放到大数据块中！**

max_blocks_per_chunk： 表示普通数据存放空间的大小，以上是100M，数据超过100M，就再申请100M，每次增加100M。



### [3. unsynchronized_pool_resource](#)

线程不安全的 [std::pmr::memory_resource](https://zh.cppreference.com/w/cpp/memory/memory_resource)，**std::pmr::unsynchronized_pool_resource**是 **std::pmr::memory_resource**的一个 **派生类**，它使用对象池的方式进行内存分配，避免频繁的内存分配和释放带来的开销，从而提高性能。

一般而言，`std::pmr::unsynchronized_pool_resource`会维护一个或多个对象池，根据不同大小的内存块需求，从相应的对象池中分配内存，如果无法满足需求，会使用全局内存分配器。

**构造函数**：

```cpp
unsynchronized_pool_resource();

explicit unsynchronized_pool_resource(std::pmr::memory_resource* upstream);

explicit unsynchronized_pool_resource(const std::pmr::pool_options& opts);

unsynchronized_pool_resource(const std::pmr::pool_options& opts, std::pmr::memory_resource* upstream);

```

私有方法：实现了父类 **memory_resource** 的 do_allocate、do_deallocate、do_is_equal方法。



| 函数                                                         | 说明                                               |
| ------------------------------------------------------------ | -------------------------------------------------- |
| allocate(size)                                               | 分配内存                                           |
| deallocate(void *p, size)                                    | 将内存放回内存池                                   |
| [release()](https://zh.cppreference.com/w/cpp/memory/unsynchronized_pool_resource/release) | 释放所有分配的内存 (公开成员函数)                  |
| [std::pmr::memory_resource*  upstream_resource()](https://zh.cppreference.com/w/cpp/memory/unsynchronized_pool_resource/upstream_resource) | 返回指向上游 memory_resource 的指针 (公开成员函数) |
| [std::pmr::pool_options options() const;](https://zh.cppreference.com/w/cpp/memory/unsynchronized_pool_resource/options) | 返回控制此资源分池行为的选项                       |





### [4. synchronized_pool_resource](#)

线程安全的 std::pmr::memory_resource，用于管理具有不同块大小的池中的分配。



```cpp
std::pmr::pool_options option;
option.largest_required_pool_block = 1024 * 1024 * 10;  //10M
option.max_blocks_per_chunk = 1024 * 1024; //1M


std::pmr::synchronized_pool_resource pool(option);

void* ar[1000];
for (size_t i = 0; i < 1000; i++)
{
    try
    {
        void* data = pool.allocate(1024 * 1024);
        ar[i] = data;
        printf("+ 1M ");
        this_thread::sleep_for(15ms);
    }
    catch (const std::exception& ex)
    {
        printf("memory not enough!\n");
    }
}


for (size_t i = 0; i < 1000; i++)
{
    try
    {
        pool.deallocate(ar[i], 1024 * 1024);
        this_thread::sleep_for(15ms);
    }
    catch (const std::exception& ex)
    {
        printf("release error!\n");
    }
}


pool.release();
```



### [5. monotonic_buffer_resource](#)

它是通过继承`std::pmr::memory_resource`来实现的, 是一种单调递增的内存资源, `std::pmr::monotonic_buffer_resource`通过将内存分配请求从底层内存资源（例如堆）中获取，并按照顺序逐个进行分配，以减少内存重复分配的开销。



monotonic buffer resource是线程不安全的哦，所以分配的速度会很快。

```cpp
char buffer[32] = {};
std::fill_n(std::begin(buffer), std::size(buffer)-1, '_');

std::pmr::monotonic_buffer_resource pool{
   std::data(buffer), std::size(buffer)
};
```

pool背后实际有两个后盾，第一个后盾很明显，就是刚才创建的那个char buffer[32]数组。第二个后盾不是很明显，叫做upstream。这个upstream就是另一个memory_resource对象，当char buffer[32]不够用时，对内存的请求就会转向upstream。是的，32字节的数组，很可能不够用。
