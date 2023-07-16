## [C++ 内存管理和智能指针](#)

智能指针是一种RAII（Resource Acquisition Is Initialization）技术的应用,  本质是利用栈来管理堆空间， C++ 17 已经提供了标准的内存池。

[synchronized_pool_resource](https://zh.cppreference.com/w/cpp/memory/synchronized_pool_resource)、[unsynchronized_pool_resource](https://zh.cppreference.com/w/cpp/memory/unsynchronized_pool_resource)。

#### 目录

- [前提01：内存区域划分](../../Grammar/contents/CPPCodeCompileMemory.md)
- [前提02：智能指针和STL](../../Grammar/contents/CPPModernCharacteristics/CPPSmartPointer.md)
- [C++ 11 share_ptr 循环引用问题](./contents/shareptr_re.md)
- [C/C++ 动态内存操作接口](./contents/operators.md)
- [C/C++ 内存分配器](./contents/allocate.md)
- [C/C++ 17 标准内存池](./contents/C17memorypool.md)

