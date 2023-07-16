### [C/C++: 内存分配器 allocator](#)

**介绍**:  分配器（配置器，allocator）是STL中六大组件（容器、算法、迭代器、仿函数、适配器、分配器）之一，用于分配管理内存空间。allocator的**特点**是将**内存分配(allocate)与对象构造(construct)分离**，这样使得对内存的管理更加灵活，性能较高



### [1. allocator](#)

分配器是**负责封装堆内存管理的对象**，它们在整个标准库中使用，特别是STL容器使用它们来管理容器内部的所有内存分配,大部份情况下,程序员不用理会,标准容器使用默认的分配器称为std :: allocator。

new 将内存分配和对象构造组合到一起，delete将析构和内存释放组合在一起，如果我们需要分开这个操作，可以使用STL的分配器。

```cpp
template< class T >
struct allocator;
```

创建一个分配器

```cpp
std::allocator<XData> alloc;
```

**C++ 17以后还支持的方法接口**：

| 成员函数                                                     | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [allocate(size_t n)](https://zh.cppreference.com/w/cpp/memory/allocator/allocate) | 分配未初始化的存储, 要分配存储的对象数                       |
| [deallocate(T* p, std::size_t n)](https://zh.cppreference.com/w/cpp/memory/allocator/deallocate) | 从指针 `p` 所引用的存储解分配,先前传递给 `allocate()` 的对象数 |
| max_size()                                                   | 返回最大的受支持分配大小 (C++17 中弃用)( **C++20 中移除**  ) |
| construct(p, args)                                           | 在分配的存储构造对象 (C++17 中弃用)(**C++20 中移除**)        |
| destroy(p)                                                   | 析构在已分配存储中的对象 (C++17 中弃用)(**C++20 中移除** )   |

由于有些方法已经废弃，析构和构造需要借助 **allocator_traits**的静态方法。

```cpp
class XData
{
public:
	XData()
	{
		cout << "Create XData" << endl;
	}
	~XData()
	{
		cout << "Drop XData" << endl;
	}
};

int main()
{	
	std::allocator<XData> alloc;
	auto start = alloc.allocate(3);
	//分配三个对象

	//构造
	std::allocator_traits<std::allocator<XData>>::construct(alloc, start);

	//析构
	std::allocator_traits<std::allocator<XData>>::destroy(alloc, start);

	alloc.deallocate(start, 3);
	//释放内存
}
```



### [2. allocator_traits](#)

**allocator_traits** 类模板提供访问[*分配器* *(Allocator)* ](https://zh.cppreference.com/w/cpp/named_req/Allocator)各种属性的标准化方式。标准容器和其他标准库组件通过此模板访问分配器，这使得能以任何类类型为分配器，只要用户提供的 **allocator_traits** 特化实现所有要求的功能。



|接口|说明|
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [static pointer allocate( Alloc& a, size_type n );](https://zh.cppreference.com/w/cpp/memory/allocator_traits/allocate)[静态] | 用分配器分配未初始化的存储 (公开静态成员函数)                |
| [allocate_at_least](https://zh.cppreference.com/w/cpp/memory/allocator_traits/allocate_at_least) [静态] | 经由分配器分配至少与请求的大小一样大的存储 (公开静态成员函数) |
| [static void deallocate( Alloc& a, pointer p, size_type n ); ](https://zh.cppreference.com/w/cpp/memory/allocator_traits/deallocate) [静态] | 用分配器解分配存储 (公开静态成员函数)                        |
| [static void construct( Alloc& a, T* p, Args&&... args );](https://zh.cppreference.com/w/cpp/memory/allocator_traits/construct) [静态] | 在分配的存储构造对象 (函数模板)                              |
| [static void destroy( Alloc& a, T* p );](https://zh.cppreference.com/w/cpp/memory/allocator_traits/destroy)[静态] | 析构存储于已分配存储的对象 (函数模板)                        |
| [static size_type max_size( const Alloc& a ) noexcept;](https://zh.cppreference.com/w/cpp/memory/allocator_traits/max_size)[静态] | 返回分配器所支持的最大对象大小 (公开静态成员函数)            |
| [select_on_container_copy_construction ](https://zh.cppreference.com/w/cpp/memory/allocator_traits/select_on_container_copy_construction)[静态] | 获得复制标准容器后使用的分配器 (公开静态成员函数)            |

使用例子：

```cpp
std::allocator<XData> alloc;

//分配三个对象
auto start = std::allocator_traits<std::allocator<XData>>::allocate(alloc, 2);

for (size_t i = 0; i < 3; i++)
{
    //构造
    std::allocator_traits<std::allocator<XData>>::construct(alloc, start);
}

for (size_t i = 0; i < 3; i++)
{
    //析构
    std::allocator_traits<std::allocator<XData>>::destroy(alloc, start);
}

//释放内存
std::allocator_traits<std::allocator<XData>>::deallocate(alloc, start, 3);
```



### [3. 自定义 allocator](#)

我们下面从0实现一个符合定义的allocator，首先我们要在调用层代码,在声明一个vector实例，显式传入我们自定义的MyAllocator，并使用他来分配内存

```cpp
int main(void){
    std::vector<int,MyAllocator<int>> v;
}
```

实现：

```cpp
template <typename Ty>
class MyAllocator 
{
public:
    //下面是个类型别名，实现中的可选项
	using value_type = Ty;
    using pointer = Ty *;
    using const_pointer = const Ty *;

    using void_pointer = void *;
    using const_void_pointer = const void *;

    using size_type = size_t;
    using difference = std::ptrdiff_t;
    
    MyAllocator()=default;
    ~MyAllocator()=default;

    template<class Other>
    MyAllocator(const MyAllocator<Other>&) {}

    pointer allocate(size_type numObjs){
        return static_cast<pointer>(operator new(sizeof(T)*numObjs));
    }
      
    pointer allocate(size_type numObjs,const_void_pointer hit){
        return allocate(numObjs);
    }
    void deallocate(pointer p,size_type numObjs){
        operator delete(p);
    }
    
    //构造函数调用
    template<class U,class...Args>
    void construct(U *p,Args &&...){
          new(p) U(std::forward<Args>(args)...);
    }
    
    //析构函数调用
    template<class U,class...Args>
    void destroy(U *p){
        p->~U();
     }
    
    size_type max_size() const{
        return std::numeric_limits<size_type>::max();
    }
};
```

**内置绑定机制**

上面实现的必选项和可选项的allocator的类成员，都是标准库已经规定的接口名称，没什么好说的。而allocator的难点就是rebind这个内部类成员，而且它也是一个类模板。

**rebind内部类成员**
根据标准库的定义，rebind被定义为std::allocator类的结构成员； 此结构定义了other成员，该成员定义为专门用于不同参数类型的分配器的实例（other成员定义了可以创建不同类型的对象的分配器类）

```cpp
class MyAllocator{
public:
    .....
     template<class U>
     struct rebind{using other=MyAllocator<U>;}
}
```



### [4. 未初始化内存算法](#)

对未初始化的内存进行连续操作的方法

| 算法函数                                                     | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [uninitialized_copy](https://zh.cppreference.com/w/cpp/memory/uninitialized_copy) | 将范围内的对象复制到未初始化的内存区域 (函数模板)            |
| [uninitialized_copy_n](https://zh.cppreference.com/w/cpp/memory/uninitialized_copy_n)(C++11) | 将指定数量的对象复制到未初始化的内存区域 (函数模板)          |
| [uninitialized_fill](https://zh.cppreference.com/w/cpp/memory/uninitialized_fill) | 复制一个对象到以范围定义的未初始化内存区域 (函数模板)        |
| [uninitialized_fill_n](https://zh.cppreference.com/w/cpp/memory/uninitialized_fill_n) | 复制一个对象到以起点和计数定义的未初始化内存区域 (函数模板)  |
| [uninitialized_move](https://zh.cppreference.com/w/cpp/memory/uninitialized_move)(C++17) | 移动一个范围的对象到未初始化的内存区域 (函数模板)            |
| [uninitialized_move_n](https://zh.cppreference.com/w/cpp/memory/uninitialized_move_n)(C++17) | 移动一定数量对象到未初始化内存区域 (函数模板)                |
| [uninitialized_default_construct](https://zh.cppreference.com/w/cpp/memory/uninitialized_default_construct)(C++17) | 在范围所定义的未初始化的内存区域以[默认初始化](https://zh.cppreference.com/w/cpp/language/default_initialization)构造对象 (函数模板) |
| [uninitialized_default_construct_n](https://zh.cppreference.com/w/cpp/memory/uninitialized_default_construct_n)(C++17) | 在起始和计数所定义的未初始化内存区域用[默认初始化](https://zh.cppreference.com/w/cpp/language/default_initialization)构造对象 (函数模板) |
| [uninitialized_value_construct](https://zh.cppreference.com/w/cpp/memory/uninitialized_value_construct)(C++17) | 在范围所定义的未初始化内存中用[值初始化](https://zh.cppreference.com/w/cpp/language/value_initialization)构造对象 (函数模板) |
| [uninitialized_value_construct_n](https://zh.cppreference.com/w/cpp/memory/uninitialized_value_construct_n)(C++17) | 在起始和计数所定义的未初始化内存区域以[值初始化](https://zh.cppreference.com/w/cpp/language/value_initialization)构造对象 (函数模板) |
| [destroy_at](https://zh.cppreference.com/w/cpp/memory/destroy_at)(C++17) | 销毁在给定地址的对象 (函数模板)                              |
| [destroy](https://zh.cppreference.com/w/cpp/memory/destroy)(C++17) | 销毁一个范围中的对象 (函数模板)                              |
| [destroy_n](https://zh.cppreference.com/w/cpp/memory/destroy_n)(C++17) | 销毁范围中一定数量的对象 (函数模板)                          |
| [construct_at](https://zh.cppreference.com/w/cpp/memory/construct_at)(C++20) | 在给定地址创建对象 (函数模板)                                |



#### 4.1 uninitialized_copy、uninitialized_copy_n

将范围内的对象复制到未初始化的内存区域， 复制来自范围 `[first, last)` 的元素到始于 `d_first` 的未初始化内存，如同用循环复制。

```cpp
template< class InputIt, class NoThrowForwardIt >
NoThrowForwardIt uninitialized_copy( InputIt first, InputIt last, NoThrowForwardIt d_first );
```

*为什么不用* *memcpy* *或者* *std::copy? uninitialized_copy* 会对对象进行复制（拷贝构造）。



#### 4.2 uninitialized_fill、uninitialized_fill_n

以指定值value填充。

```cpp
template< class ForwardIt, class T >
void uninitialized_fill( ForwardIt first, ForwardIt last, const T& value );
```



```cpp
std::allocator<XData> alloc;

std::vector<XData> numbers {56, 78, 98, 78, 45};


//分配两倍的内存
auto p = alloc.allocate(numbers.size() * 2);

//初始化 调用构造函数
auto q = uninitialized_copy(numbers.begin(), numbers.end(), p);

//剩余元素初始化
uninitialized_fill_n(q, numbers.size(), XData(14));

//释放
alloc.deallocate(p ,numbers.size() * 2);
```





#### 4.3 uninitialized_move、uninitialized_move_n





#### 4.4 uninitialized_default_construct、uninitialized_default_construct_n





#### 4.5 uninitialized_value_construct、uninitialized_value_construct





#### 4.6 destroy_at





#### 4.7 destroy





#### 4.8 destroy_n





#### 4.9 construct_at



### [5. polymorphic_allocator](#)

C++的`polymorphic_allocator`是C++17标准中引入的一个类模板，用于支持多态内存分配。它是以C++ Allocator接口为基础的扩展，允许使用多态的内存分配器来为对象分配内存。

`polymorphic_allocator`类模板定义如下：



```cpp
template <typename T>
class polymorphic_allocator {
public:
    using value_type = T;

    polymorphic_allocator() noexcept;
    polymorphic_allocator(const polymorphic_allocator& other) noexcept;
    template <class U> polymorphic_allocator(const polymorphic_allocator<U>& other) noexcept;
    ~polymorphic_allocator();

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n) noexcept;

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args);

    template <typename U>
    void destroy(U* p);
};

```

**polymorphic_allocator** 提供了一些成员函数，包括：

- `allocate`：分配指定数量的内存，并返回指向分配内存的指针。
- `deallocate`：释放先前分配的内存。
- `construct`：在给定的内存地址上构造`T`类型的对象。
- `destroy`：销毁在给定的内存地址上构造的对象。

`polymorphic_allocator`的设计允许通过多态分配器实现自定义内存管理策略，例如使用不同的内存池或内存分配算法。

以下是一个简单的示例，演示如何使用`polymorphic_allocator`：

```cpp
#include <iostream>
#include <vector>
#include <memory_resource>

int main() {
    std::pmr::monotonic_buffer_resource pool;
    std::pmr::polymorphic_allocator<int> allocator(&pool);

    std::vector<int, std::pmr::polymorphic_allocator<int>> vec(allocator);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

在上述示例中，我们首先创建了一个`monotonic_buffer_resource`对象`pool`，它是一个多态内存资源，用于管理内存分配。然后使用`pool`作为构造函数的参数，创建了一个`polymorphic_allocator<int>`对象`allocator`。最后，我们通过`allocator`来分配一个支持多态内存分配的`vector`对象`vec`，并向其添加几个整数元素。

请注意，示例中使用了`std::pmr`命名空间，这是C++17中提供的一个用于多态内存资源和分配器的命名空间。为了使用`polymorphic_allocator`，需要包含头文件`<memory_resource>`。



### [6. 限制在栈中创建对象](#)

我们可以使用私有构造函数的来实现，如此就只能在堆上面创建对象了。

```cpp
#include <iostream>
#include <memory>

using namespace std;

class XData
{
public:
	static XData* Create(int i = 0) {
		return new XData(i);
	}

	static XData* Create(const XData* other) {
		return new XData(*other);
	}

	static XData* Create(const XData other) {
		return new XData(other);
	}

private:
	XData(int val = 0): i(val)
	{
		cout << "Create XData" << endl;
	}

	XData(const XData& x):i(x.i)
	{
		cout << "copy XData" << endl;
	}
public:
	~XData()
	{
		cout << "Drop XData" << endl;
	}
	
	int i;
};

int main()
{	
	//XData x(10); //不被允许

	XData* xd = XData::Create(100);
	XData* xd2 = XData::Create(xd);

	std::cout << "xd.i = " << xd->i << std::endl;
	std::cout << "xd2.i = " << xd2->i << std::endl;

	delete xd, xd2;
	
}
```

