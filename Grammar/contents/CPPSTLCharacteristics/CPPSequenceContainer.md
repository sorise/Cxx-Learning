### [C++ STL序列容器/集合类型](#)
`序列（sequence）容器，7种STL容器类型（deque、C++新增的forward_list、list、queue、priority_queue、stack和vector）都是序列。`

-----
- [x] [1. stack](#1-stack)
- [x] [2. queue](#2-queue)
- [x] [3. valarray](#3-valarray)
- [x] [4. vector](#4-vector)
- [x] [5. deque](#5-deque)
- [x] [6. list](#6-list)
- [x] [7. forward_list](#7-forward_list)
- [x] [8. priority_queue](#8-priority_queue)
- [x] [9. 确定使用哪种顺序容器](#9-确定使用哪种顺序容器)

-----

为了建立数据结构和算法的一套标准，并且降低他们之间的耦合关系，以提升各自的独立性、弹性、交互操作性(相互合作性, `interoperability`),诞生了 `STL`。
`STL(Standard Template Library,标准模板库)`，是惠普实验室开发的一系列软件的统称。现在主要出现在 c++中，但是在引入 c++之前该技术已经存在很长时间了。
**stack、queue、priority_queue 本质上是顺序容器的适配器。**
### [1. stack](#)
stack 类型没有迭代器，不能用迭代器遍历队列stack(堆栈）是一个容器的改编，它实现了一个先进后出的数据结构（FILO）  使用该容器时需要包含#include<stack>头文件；

#### [1.1 创建stack](#)
stack 有三个构造方法
* `Stack()  使用初始容量创建 Stack 的对象`
* `Stack(ICollection col) 创建 Stack 的实例，该实例包含从指定实例中复制的元素，并且初始容量与复制的元素个数、增长因子相同`
* `Stack(int capacity) 创建 Stack 的实例，并设置其初始容量`

```cpp
// 声明一个空的 stack
stack<int> s; 

// 声名并赋值
stack<int> s;
for(int i=0; i<10; i++) s.push(i); 

//声明 初始化
std::list<double> values {1.414, 3.14159265, 2.71828};
```

##### [1.1.1 利用容器初始化](#)
stack 容器适配器的模板有两个参数。第一个参数是存储对象的类型，第二个参数是底层容器的类型。stack<T> 的底层容器默认是 deque<T> 容器，因此模板类型其实是 `stack<typename T, typename Container=deque<T>>`。

通过指定第二个模板类型参数，可以使用任意类型的底层容器，只要它们支持 `back()、push_back()、pop_back()、empty()、size()` 这些操作。下面展示了如何定义一个使用 list<T> 的堆栈：

```cpp
#include <iostream>
#include <string.h>
#include <stack>  
#include <list>
int main(int argc, char const* argv[])
{

    std::list<double> values {1.9564, 3.1415, 2.71828, 7.521, 9454.21};
    std::stack<double,std::list<double>> my_stack (values);
    while (!my_stack.empty())
    {
        auto val = my_stack.top();//调用复制构造函数
        my_stack.pop();
        std::cout << val << std::endl;
    }
    return 0;
}
```

#### [1.2 操作方法](#)
和其他序列容器相比，stack 是一类存储机制简单、所提供操作较少的容器。下面是 stack 容器可以提供的一套完整操作：

* `top()：返回一个栈顶元素的引用，类型为 T&。如果栈为空，返回值未定义。`
* `push(const T& obj)：可以将对象副本压入栈顶。这是通过调用底层容器的 push_back() 函数完成的。`
* `push(T&& obj)：以移动对象的方式将对象压入栈顶。这是通过调用底层容器的有右值引用参数的 push_back() 函数完成的。`
* `pop()：弹出栈顶元素。`
* `size()：返回栈中元素的个数。`
* `empty()：在栈中没有元素的情况下返回 true。`
* `emplace()：用传入的参数调用构造函数，在栈顶生成对象。`
* `swap(stack<T> & other_stack)：将当前栈中的元素和参数中的元素交换。参数所包含元素的类型必须和当前栈的相同。对于 stack 对象有一个特例化的全局函数 swap() 可以使用。`

```cpp
#include <iostream>
#include <string.h>
#include"student.h"
#include <stack>  

int main(int argc, char const* argv[])
{

    auto lzm = Student("2021220604880", "李元昊", 22);//调用有参构造函数
    auto ljk = Student("2021220604882", "李嘉坤", 24);//调用有参构造函数
    auto lsm = Student("2021220604883", "李世明", 25);//调用有参构造函数
    auto ltm = Student("2021220604884", "李天明", 20);//调用有参构造函数

    std::stack<Student> stu_stack;
    std::string isEmpty = stu_stack.empty()?"stack is empty":"stack is not empty";
    std::cout  << isEmpty << std::endl;

    stu_stack.push(lzm);//调用复制构造函数 
    stu_stack.push(ltm);//调用复制构造函数 
    stu_stack.push(lsm);//调用复制构造函数 
    stu_stack.push(ljk);//调用复制构造函数 
    
    while (!stu_stack.empty())
    {
        Student stu = stu_stack.top();//调用复制构造函数
        stu_stack.pop();
        std::cout << stu.ToString() << std::endl;
    }
    // 12次 对象消亡

    return 0;
}

```
### [2. queue](#) 
queue是一种容器转换器模板，调用#include< queue>即可使用队列类。 `queue<typename> name` `queue<Type, Container> (<数据类型，容器类型>）`


#### [2.1 创建一个 queue](#)
始化时必须要有数据类型，容器可省略，省略时则默认为deque 类型。

`queue 的生成方式和 stack 相同，下面展示如何创建一个保存字符串对象的 queue:`
```cpp
std::queue<std::string> names;
names.push("kicker");
names.push("mirror");
```
`也可以使用拷贝构造函数：`
```cpp
std::queue<std::string> copy_words {words}; // A duplicate of words
```
`stack<T>、queue<T> 这类适配器类都默认封装了一个 deque<T> 容器，也可以通过指定第二个模板类型参数来使用其他类型的容器：`
`std::queue<std::string, std::list<std::string>>words;`
```cpp
#include <iostream>
#include <string>
#include <queue>
#include <list>

int main(int argc, char const* argv[])
{
    std::list<double> values {1.9564, 3.1415, 2.71828, 7.521, 9454.21};

    std::queue<double, std::list<double>> queue_list(values);

    while (!queue_list.empty())
    {
        std::cout << queue_list.front() << std::endl;
        queue_list.pop();
    }
    
    std::cout << "size: " << queue_list.size() << std::endl;
    return 0;
}
```

#### [2.2 queue 方法](#)
queue 和 stack 有一些成员函数相似，但在一些情况下，工作方式有些不同：

* `front()：返回 queue 中第一个元素的引用。如果 queue 是常量，就返回一个常引用；如果 queue 为空，返回值是未定义的。`
* `back()：返回 queue 中最后一个元素的引用。如果 queue 是常量，就返回一个常引用；如果 queue 为空，返回值是未定义的。`
* `push(const T& obj)：在 queue 的尾部添加一个元素的副本。这是通过调用底层容器的成员函数 push_back() 来完成的。`
* `push(T&& obj)：以移动的方式在 queue 的尾部添加元素。这是通过调用底层容器的具有右值引用参数的成员函数 push_back() 来完成的。`
* `pop()：删除 queue 中的第一个元素。`
* `size()：返回 queue 中元素的个数。`
* `empty()：如果 queue 中没有元素的话，返回 true。`
* `emplace()：用传给 emplace() 的参数调用 T 的构造函数，在 queue 的尾部生成对象。`
* `swap(queue<T> &other_q)：将当前 queue 中的元素和参数 queue 中的元素交换。它们需要包含相同类型的元素。也可以调用全局函数模板 swap() 来完成同样的操作。`


```cpp
#include <iostream>
#include <string>
#include <queue>

int main(int argc, char const* argv[])
{
    std::queue<std::string> names;
    names.push("kicker");
    names.push("mirror");
    names.push("umix");
    names.push("remix");
    names.push("nike");

    while (!names.empty())
    {
        std::cout << names.front() << std::endl;
        names.pop();
    }
    
    std::cout << "size: " << names.size() << std::endl;
    return 0;
}
```

### [3. valarray](#) 
valarray对象被设计用来保存一个值数组，并且可以轻松地对它们执行数学运算。它还允许特殊机制引用数组中元素的子集。
大多数数学运算可以直接应用于valarray对象，包括算术和比较运算符，影响其所有元素。` `是表示并操作值数组的类。它支持逐元素数学运算与多种形式的广义下标运算符、切片及间接访问。

1. `使用场景是向量的快速计算；`
2. `有一堆辅助的工具类；`
3. `C++之父Bjarne Stroustrup的HOPL4论文《Thriving in a Crowded and Changing World: C++ 2006–2020》中提到；`

#### [3.1 使用构造函数创建](#)

```cpp
#include <iostream>
#include <string>
#include"animal.h"
#include<valarray>

int main(int argc, char const* argv[])
{
    std::valarray<double> scores(5);  //[2]
    //默认值 {0 0 0 0 0}

    std::valarray<double> scores(10，4);  //[3]
    //{10 10 10 10}

    std::valarray<double> ant_scores{58.6, 98.5, 66.4, 56.5 ,94.23};//[5]
    // std::valarray<int> data = {0,1,2,3,4,5,6,7,8,9};//[5]

    int dim[] = { 1,2,3,4,5,6,7,8,9 };
    std::valarray<int> dim_array(dim, sizeof(dim)/sizeof(int)); //[4]
    
    return 0;
}
```

**常用构造函数**

* `[1] valarray();`
* `[2] explicit valarray( std::size_t count );` 
* `[3] valarray( const T& val, std::size_t count );`
* `[4] valarray( const T* vals, std::size_t count );`
* `[5] valarray( std::initializer_list<T> il );` `C++ 11新增，可以直接使用数组初始化的方式！`

#### [3.2 非成员函数](#)

```cpp
std::valarray<double> ant_scores{58.6, 98.5, 66.4, 56.5 ,94.23};//[5]

for (double *  itl = std::begin(ant_scores); itl != std::end(ant_scores); ++itl)
{
    std::cout << *itl << std::endl;
}
```

* `std::swap(std::valarray) (C++11)`
* `std::begin(std::valarray) (C++11)`
* `std::end(std::valarray) (C++11)`

#### [3.3 基本方法](#)
* `size() 返回valarray的大小 (公开成员函数)`
* `sum() 计算所有元素的和(公开成员函数)`
* `max() 返回最大元素`
* `min() 返回最小元素`
* `apply() 将函数应用到 valarray 的每个元素(公开成员函数)` `valarray apply (T func(T)) const;valarray apply (T func(const T&)) const;`
* `valarray<T> shift( int count ) const; 以填入零的方式移动 valarray 的元素 (公开成员函数)`
* `cshift 循环移动 valarray 的元素`
```cpp
#include <iostream>
#include<valarray>

int main(int argc, char const* argv[])
{

    std::valarray<double> scores(5);  //[2]
    //默认值 {0 0 0 0 0}
    //填值 
    scores[0] = 55.12;
    scores[1] = 95.12;
    scores[2] = 58.12;
    scores[3] = 65.12;
    scores[4] = 75.12;
    
    std::valarray<double> ant_scores{58.6, 98.5, 66.4, 56.5 ,94.23};//[5]
    
    int dim[] = { 1,2,3,4,5,6,7,8,9 };
    std::valarray<int> dim_array(dim, sizeof(dim)/sizeof(int)); //[5]

    dim_array = dim_array.apply([](int v) -> int {
        return v * 3;
    });

    for (auto &val : dim_array)
    {
        printf("value: %d\n", val);    
    }
    
    double max = ant_scores.max();
    double mix = scores.min();

    printf("max value: %.3lf\n", max);
    printf("mix value: %.3lf\n", max);
 
    return 0;
}
```

`移动例子`
```cpp
std::valarray<int> v{1, 2, 3, 4, 5, 6, 7, 8};

std::valarray<int> v2 = v.shift(2);
//3 4 5 6 7 8 0 0

std::valarray<int> v3 = v.cshift(2);
//3 4 5 6 7 8 1 2
```

#### [3.4 运算符操作](#)
它支持很多线性运算， 需要想到这个运算都是向量运算！

* `当左侧参数和右侧参数都是valarray对象时，将在每个对象中的相应元素之间执行操作（左参数的第一个元素与右参数的第一个元素，第二个元素与第二个元素，依此类推…）。`
* `当其中一个参数是值时，该操作将应用于valarray中针对该值的所有元素。`


`基本运算`
```cpp
std::valarray<double> scores = {55.12,95.12,58.12,65.12,75.12 };  

std::valarray<double> ant_scores{58.6, 98.5, 66.4, 56.5 ,94.23};

std::valarray<double>  li_score = scores * 10 + ant_scores;
//609.800 1049.700 647.600 707.700 845.430 
for (auto &&i : li_score)
{
    printf("%.3lf ", i);
}

std::valarray<double> result = scores * ant_scores;
//3230.032 9369.320 3859.168 3679.280 7078.558

for (auto &&i : result)
{
    printf("%.3lf ", i);
}
printf("\n");
```

`稍有难度`
```cpp
#include <cstddef>
#include <valarray>
#include <iostream>
 
int main()
{
    std::valarray<double> a(1, 8);
    std::valarray<double> b{1, 2, 3, 4, 5, 6, 7, 8};
    std::valarray<double> c = -b;
    // LWG3074 前字面量亦必须拥有 T 类型（此情况下为 double ）
    std::valarray<double> d = std::sqrt(b * b - 4.0 * a * c);
    std::valarray<double> x1 = (-b - d) / (2.0 * a);
    std::valarray<double> x2 = (-b + d) / (2.0 * a);
    std::cout << "quadratic equation    root 1,  root 2" << "\n";
    for (std::size_t i = 0; i < a.size(); ++i) {
        std::cout << a[i] << "x\u00B2 + " << b[i] << "x + " << c[i] << " = 0   ";
        std::cout << std::fixed << x1[i] << ", " << x2[i] << std::defaultfloat << "\n";
    }
}

/*
quadratic equation    root 1,  root 2
1x² + 1x + -1 = 0   -1.618034, 0.618034
1x² + 2x + -2 = 0   -2.732051, 0.732051
1x² + 3x + -3 = 0   -3.791288, 0.791288
1x² + 4x + -4 = 0   -4.828427, 0.828427
1x² + 5x + -5 = 0   -5.854102, 0.854102
1x² + 6x + -6 = 0   -6.872983, 0.872983
1x² + 7x + -7 = 0   -7.887482, 0.887482
1x² + 8x + -8 = 0   -8.898979, 0.898979
*/

```

#### [3.5 数学运算](#)
valarray 支持三角函数，幂函数，指数函数

```cpp
std::valarray<int> lite{2, 3, 4, 5 ,6};
// a * b 向量相乘
std::valarray<int> result_pow = pow(lite, 2);
//4 9 16 25 36

for (auto &&i : result_pow)
{
    printf("%d ", i);
}
```

#### [3.6 长度不匹配问题](#)
如果两个对象做运算，但是长度不相匹配，就有问题了, 结果是以第一个对象长度为准

```cpp
std::valarray<int> gloom{2, 3};
std::valarray<int> lite{2, 3, 4, 5 ,6};
// a * b 向量相乘
std::valarray<int> result1 = gloom * lite;//[2, 3] * [2, 3, 4, 5 ,6]
// 4 9

std::valarray<int> result2 = lite * gloom;//[2, 3, 4, 5 ,6] * [2, 3]
//4 9 0 0 0
```

### [4. vector](#) 
模板类vector和array是数组的替代品, 也可以说是一种动态数组。它是一个泛型类，为STL容器类型之一！ [Vector API](https://zh.cppreference.com/w/cpp/container/vector)

**vector**：顺序容器（可变大小数组）。支持快速随机访问。在尾部之外的位置插入或删除元素可能很慢。既然vector是个容器，那么一定相关特性，如添加元素、删除元素和查询容器大小等操作。

向量 **（Vector）**是一个封装了动态大小数组的**顺序容器（Sequence Container）**。跟任意其它类型容器一样，它能够存放各种类型的对象。可以简单的认为，向量是一个能够存放任意类型的动态数组。

* **std::vector**  **是封装动态数组的顺序容器。**
* **std::pmr::vector** **是使用多态分配器的模板别名。**

```cpp
// c++17 之前只有如下定义
template < class T, class Alloc = allocator<T> > 
class vector; // generic template

// c++17 新增
namespace pmr {
    template <class T>
    using vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;
}
```

* `T`：`T 必须满足可复制赋值 (CopyAssignable) 和可复制构造 (CopyConstructible) 的要求。`
* `Allocator`:`用于获取/释放内存及构造/析构内存中元素的分配器。类型必须满足分配器 (Allocator) 的要求`

`标准库提供了std::vector类型bool，对空间效率进行了优化。`
```cpp
vector<bool>
```

`vector 的存储是自动管理的，按需扩张收缩。 vector 通常占用多于静态数组的空间，因为要分配更多内存以管理将来的增长。 vector 所用的方式不在每次插入元素时，而只在额外内存耗尽时重分配。分配的内存总量可用 capacity() 函数查询。可通过调用 shrink_to_fit() 返回多出的内存给系统。 (C++11 起)`
`重分配通常是性能上有开销的操作。若元素数量已知，则 reserve() 函数可用于消除重分配。`

`vector 上的常见操作复杂度（效率）如下：`
* `随机访问——常数 𝓞(1)`
* `在末尾插入或移除元素——均摊常数 𝓞(1)`
* `插入或移除元素——与到 vector 结尾的距离成线性 𝓞(n)`

`std::vector （对于 bool 以外的 T ）满足容器 (Container) 、知分配器容器 (AllocatorAwareContainer) 、序列容器 (SequenceContainer) 、连续容器 (ContiguousContainer) (C++17 起)及可逆容器 (ReversibleContainer) 的要求。`

```cpp
#include < vector> 
using namespace std;

vector<int>obj;//创建一个向量存储容器 int
for(int i=0;i<10;i++) // push_back(elem)在数组最后添加数据 
{
    obj.push_back(i);
    cout<<obj[i]<<",";    
}

for(int i=0;i<5;i++)//去掉数组最后一个数据 
{
    obj.pop_back();
}

cout<<"\n"<<endl;

for(int i=0;i<obj.size();i++)//size()容器中实际数据个数 
{
    cout<<obj[i]<<",";
}
```

#### [4.1 构造函数](#)
* `vector():创建一个空vector`
* `vector(int nSize):创建一个vector,元素个数为nSize`
* `vector(int nSize,const t& t):创建一个vector，元素个数为nSize,且值均为t`
* `vector(const vector&):复制构造函数`
* `vector(begin,end):复制[begin,end]区间内另一个数组的元素到vector中`

```cpp
#include <vector>
#include <string>
#include <iostream>
 
template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v)
{
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v)
    {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}
 
int main() 
{
    // C++11 初始化器列表语法：
    std::vector<std::string> words1 {"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words1: " << words1 << '\n';
 
    // words2 == words1
    std::vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: " << words2 << '\n';
 
    // words3 == words1
    std::vector<std::string> words3(words1);
    std::cout << "words3: " << words3 << '\n';
 
    // words4 是 {"Mo", "Mo", "Mo", "Mo", "Mo"}
    std::vector<std::string> words4(5, "Mo");
    std::cout << "words4: " << words4 << '\n';
}
```

#### [4.2 插值API](#)
* `void push_back(const T& x):向量尾部增加一个元素X`
* `iterator insert(iterator it,const T& x):向量中迭代器指向元素前增加一个元素x`
* `iterator insert(iterator it,int n,const T& x):向量中迭代器指向元素前增加n个相同的元素x`
* `iterator insert(iterator it,const_iterator first,const_iterator last):向量中迭代器指向元素前插入另一个相同类型向量的[first,last)间的数据`

```cpp
std::list<std::string> names {"remix", "unix", "kicer", "ciur"};

names.insert(names.begin(), "himix");
names.insert(names.end(),3 , "cutse");

std::for_each(names.begin(), names.end(), [n = 0](std::string name) mutable {
    printf("[%d]: %s\n", n++, name.c_str());
});
/*
[0]: himix
[1]: remix
[2]: unix
[3]: kicer
[4]: ciur
[5]: cutse
[6]: cutse
[7]: cutse
*/
```

#### [4.3 删除函数](#)
* `iterator erase(iterator it):删除向量中迭代器指向元素`
* `iterator erase(iterator first,iterator last):删除向量中[first,last)中元素`
* `void pop_back():删除向量中最后一个元素`
* `void clear():清空向量中所有元素`

```cpp
std::vector<double> scores {89.15, 97.15, 78.1, 74.1, 95.12, 94.10, 85.15 };
scores.erase(scores.begin(), scores.begin() + 2); 
std::for_each(scores.begin(), scores.end(), [](double v){
    printf("%.2lf \n", v);
});
//78.10 74.10 95.12 94.10 85.15 
```

#### [4.4 判断函数](#)
* `bool empty() const:判断向量是否为空，若为空，则向量中无元素`

#### [4.5 大小函数](#)
* `int size() const:返回向量中元素的个数`
* `int capacity() const:返回当前向量所能容纳的最大元素值`
* `int max_size() const:返回最大可允许的vector元素数量值`
* `reserve(n) 改变当前vecotr所分配空间的大小, 设置容量（capacity）`
* `resize()，设置大小（size）;`
* `shrink_to_fit()`  ` (C++11)通过释放未使用的内存减少内存的使用`
`size()是分配容器的内存大小，而capacity()只是设置容器容量大小，但并没有真正分配内存。`

`打个比方：正在建造的一辆公交车，车里面可以设置40个座椅（reserve(40);），这是它的容量，但并不是说它里面就有了40个座椅，只能说明这部车`
`内部空间大小可以放得下40张座椅而已。而车里面安装了40个座椅(resize(40);)，这个时候车里面才真正有了40个座椅，这些座椅就可以使用了`

#### [4.6 C++ vector中的resize，reserve，size和capacity函数讲解](#)
* `capacity 指容器在分配新的存储空间之前能存储的元素总数。`
* `size 指当前容器所存储的元素个数`
* `resize 既分配了空间，也创建了对象。 这里空间就是capacity，对象就是容器中的元素。`
* `reserve() 表示容器预留空间，但不是真正的创建对象，需要通过insert()或push_back()等操作创建对象 `

#### [4.7 其他函数](#)
用于内容替代或者交换

* `void swap(vector&):交换两个同类型向量的数据`
* `void assign(int n,const T& x):设置向量中前n个元素的值为x`
* `void assign(const_iterator first,const_iterator last):向量中[first,last)中元素设置成当前向量元素`
```cpp
std::list<std::string> names_t1 {"remix", "unix", "kicer", "ciur"};
std::list<std::string> names_t2 {"shower", "maxs", "uu", "kiuer" ,"cni", "ui"};
std::swap<std::list<std::string>>(names_t1, names_t2);

std::for_each(names_t1.begin(), names_t1.end(), [n = 0](std::string name) mutable {
    printf("[%d]: %s\n", n++, name.c_str());
});
/*
[0]: shower
[1]: maxs
[2]: uu
[3]: kiuer
[4]: cni
[5]: ui
*/
```

#### [4.8 遍历函数](#)

* `reference at(int pos):返回pos位置元素的引用`
* `reference front():返回首元素的引用`
* `reference back():返回尾元素的引用`
* `iterator begin():返回向量头指针，指向第一个元素`
* `iterator end():返回向量尾指针，指向向量最后一个元素的下一个位置`
* `reverse_iterator rbegin():反向迭代器，指向最后一个元素`
* `reverse_iterator rend():反向迭代器，指向第一个元素之前的位置`

```cpp
unsigned char len = 10;
vector<int> scores(len);
for (int i = 0; i < len; i++)
{
    scores[i] = (i * 458 + 15 + i) % 35;
}

for (int i = 0; i < len; i++)
{
    cout << scores[i] <<" ";
}
cout << endl;
scores.push_back(1);
cout <<"容器大小：" << scores.size()<<endl;

scores.clear();
cout << "清除数据后容器大小：" << scores.size() << endl;
```

### [5. deque](#) 
双端队列, 在标头 <deque> 定义 定义如下，只要学了双端队列就很好理解！ `std::deque （ double-ended queue ，双端队列）`是有下标顺序容器，它允许在其首尾两端快速插入及删除。另外，在 deque 任一端插入或删除不会非法化指向其余元素的指针或引用。

```cpp
template<
    class T,
    class Allocator = std::allocator<T>
> class deque;
```

`deque 上常见操作的复杂度（效率）如下：`
* `随机访问——常数 O(1)`
* `在结尾或起始插入或移除元素——常数 O(1)`
* `插入或移除元素——线性 O(n)`

> `std::deque 满足容器 (Container) 、知分配器容器 (AllocatorAwareContainer) 、序列容器 (SequenceContainer) 和可逆容器 (ReversibleContainer) 的要求。`

#### [5.1 构造函数](#)
顺序容器，构造函数大同小异。

* `deque( std::initializer_list<T> init, const Allocator& alloc = Allocator() );(C++11 起)`
* `template< class InputIt > deque( InputIt first, InputIt last,const Allocator& alloc = Allocator() );`

```cpp
std::list<double> container {10.1, 100.05, 900.1, 84.1};

std::deque<double>  xiaohu(container.begin(), container.end(),std::allocator<double>()); 
xiaohu.push_back(25.14);
xiaohu.push_front(15.14);

std::for_each(xiaohu.begin(), xiaohu.end(), [](auto v) { std::cout << v << ' '; });
//15.14 10.1 100.05 900.1 84.1 25.14 
std::cout << "\n";
```

#### [5.2 元素访问](#)
`主要关注 front() 和 back()`

* `at(index)  访问指定的元素，同时进行越界检查 (公开成员函数)`
* `operator[] 访问指定的元素 (公开成员函数)`
* `front()  访问第一个元素 (公开成员函数)`
* `back() 访问最后一个元素 (公开成员函数)`

#### [5.3 容量函数](#)
`顺序容器都差不多！`

* `empty 检查容器是否为空`
* `size 返回容纳的元素数`
* `max_size 返回可容纳的最大元素数`
* `shrink_to_fit(C++11) 通过释放未使用的内存减少内存的使用 (公开成员函数)`

#### [5.4 操作方法](#)

* `emplace_front (C++11)在容器头部原位构造元素(公开成员函数)`
* `emplace_back (C++11)在容器末尾就地构造元素(公开成员函数)`
* `支持C++ 序列的可选要求：`


### [6. list](#)
在标头 <list> 定义` `双向链表，除了第一个和最后一个元素，每个元素都与前后的元素相互链接。在 list 内或在数个 list 间添加、移除和移动元素不会是迭代器或引用失效。迭代器只有在对应元素被删除时才会失效。
```cpp
template<
    class T,
    class Allocator = std::allocator<T>
> class list;
```

`支持 序列概念模型的要求和，部分C++ 序列的可选要求：`


#### [6.1 一些自己的操作](#)

* `merge` `合并二个已排序列表 (公开成员函数)`
* `splice` `从另一个list中移动元素(公开成员函数)`
* `remove/remove_if`  `移除满足特定标准的元素(公开成员函数)`
* `reverse` `将该链表的所有元素的顺序反转(公开成员函数)`
* `unique` `删除连续的重复元素(公开成员函数)`
* `sort` `对元素进行排序` 

##### 6.1.1 splice
`从一个 list 转移元素给另一个。不复制或移动元素，仅重指向链表结点的内部指针。`
```cpp
void splice( const_iterator pos, list& other, const_iterator first, const_iterator last);
```

* `pos	-	将插入内容到它之前的元素`
* `other	-	要从它转移内容的另一容器`
* `it	-	要从 other 转移到 *this 的元素`
* `first, last	-	要从 other 转移到 *this 的元素范围`

```cpp
std::list<int> list1 = {1, 2, 3, 4, 5};
std::list<int> list2 = {10, 20, 30, 40, 50};

list1.splice(list1.end(), list2, ++list2.cbegin(), list2.end() );
std::for_each(list1.begin(), list1.end(), [](auto v) { std::cout << v << ' '; });
//1 2 3 4 5 10 20 30 40 50 
std::cout << "\n";
```
##### 6.1.2 remove/remove_if
`remove/remove_if`

```cpp
#include <list>
#include <iostream>
 
int main()
{
    std::list<int> l = { 1,100,2,3,10,1,11,-1,12 };
 
    l.remove(1); // 移除两个等于 1 的元素
    l.remove_if([](int n){ return n > 10; }); // 移除全部大于 10 的元素
 
    for (int n : l) {
        std::cout << n << ' '; 
    }
    std::cout << '\n';
}
```

##### 6.1.3 sort
排个序罢了！ 降序排序。
```cpp
bool Compare(const Type1 &a, const Type2 &b);
template< class Compare >
void sort( Compare comp );


std::list<double> container {10.1, 100.05, 900.1, 84.1, 52.12};
container.sort();
std::for_each(container.begin(), container.end(), [](auto v) { std::cout << v << ' '; });
//10.1 52.12 84.1 100.05 900.1
std::cout << "\n";
```

##### 6.1.4 assign
assign仅仅适用于顺序容器！

```cpp
std::list<double> scores{29.25, 98.15};

scores.assign(10, 99.56);

for (auto && v : scores)
{
	printf("%.2lf ", v);
}
printf("\n");
//99.56 99.56 99.56 99.56 99.56 99.56 99.56 99.56 99.56 99.56

```

### [7. forward_list](#)
就是一个单向链表，std::forward_list 是支持从容器中的任何位置快速插入和移除元素的容器。不支持快速随机访问。它实现为单链表，且实质上与其在 C 中实现相比无任何开销。与 std::list 相比，此容器在不需要双向迭代时提供更有效地利用空间的存储。在标头 <forward_list> 定义
```cpp
template<
    class T,
    class Allocator = std::allocator<T>
> class forward_list;
```

* `front()`  `(C++11)访问第一个元素(公开成员函数)`
* `before_begin()`  `(C++11)返回指向第一个元素之前迭代器`
* `begin()` `返回指向起始的迭代器` 
* `cbegin()`  `返回指向逆序起始的迭代器`
* `end()` `返回指向末尾的迭代器`
* `cend()` `返回指向逆序末尾的迭代器`


#### [7.1 forward_list的限制](#)
大小的成员函数只具有 max_size  和 empty。不具备 size()函数。

* `u.max_size()`	`返回容器可保持的最大元素数目	固定`
* `u.empty()`		`返回容器是否位空	固定`
* `u.push_back(args)`	**不支持 forward_list**
* `u.emplace_back(args)` **不支持 forward_list**
* `u.emplace()`  **forward_list 专有版本**
* `u.insert()`  **forward_list 专有版本**


#### [7.2 插入元素](#)
单向链表，只能在前一个元素的后面插入元素。

* iterator emplace_after( const_iterator pos, Args&&... args ); 在元素后原位构造元素
* insert_after(it, value) 在某个元素后插入新元素
```cpp
std::forward_list<std::string> names{ "shower", "maxs", "uu", "kiuer" ,"cni", "ui" };
std::forward_list<std::string>::iterator it = names.before_begin();
names.insert_after(it, "remix");


for (auto&& v : names) {
    printf("%s ", v.c_str());
}//remix shower maxs uu kiuer cni ui
```

### [8. priority_queue](#)
在标头 <queue> 定义priority_queue 是容器适配器，它提供常数时间的（默认）最大元素查找，对数代价的插入与释出。可用用户提供的 Compare 更改顺序，例如，用 std::greater<T> 
将导致最小元素作为 top() 出现。用 priority_queue 工作类似管理某些随机访问容器中的堆，优势是不可能突然把堆非法化。

**less就是让前一个比后一个更小；greater就是让前一个比后一个更大。**

**推荐使用自定义函数：**

用 priority_queue 工作类似管理某些随机访问容器中的堆，优势是不可能突然把堆非法化。

* **迭代器不支持 -- 运算符**
* **不支持属性 reverse_iterator、const_reverse_iterator**
* **适配一个容器以提供优先级队列**

```cpp
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;
```

```cpp
int main(int argc, char const* argv[])
{   
   std::list<int> list1 = {1, 2, 3, 4, 5};

   std::priority_queue<int> p_q(list1.begin(), list1.end());

   std::cout << p_q.top() << '\n'; //5
   return 0;
}
```

```cpp
void common_sort()
{
    int source_data[10] = {3, 5, 8, 1, 10, 2, 9, 15, 13, 16};

    // 默认大根堆，实现由大到小排序
    priority_queue<int> q;
    for (auto n : source_data) q.push(n);

    while (!q.empty()) {
        cout << q.top() << endl;
        q.pop();
    }
}
```

#### [8.1 构造函数](#)
**总共19个重载构造函数** priority_queue()，默认按照从小到大排列。所以top()返回的是最大值而不是最小值！

使用greater<>后，数据从大到小排列，top()返回的就是最小值而不是最大值！

如果使用了第三个参数，那第二个参数不能省，用作保存数据的容器！！！！

```cpp
priority_queue<int,vector<int> , greater<>> pq;//这是对的
```

```cpp
#include <stdio.h>
#include <queue>
using namespace std;
int main()
{
    priority_queue<int> q;
    q.push(3);
    q.push(4);
    q.push(1);
    printf("%d\n",q.top()); //4
    return 0;
}
```

#### [8.2 小顶堆 自定义函数](#)

```cpp
bool minHeap(int x1, int x2){
    return x1 > x2;
}

std::priority_queue<int, std::vector<int>, decltype(bigHeap)*> squeue(bigHeap);
squeue.push(10);
squeue.push(17);
squeue.push(12);
squeue.push(17);

while (!squeue.empty()){
    std::cout << squeue.top() << " ";
    squeue.pop();
}
```
#### [8.3 大顶堆 自定义函数](#)

```cpp
bool bigHeap(int x1, int x2){
    return x1 < x2;
}
```




### [9. 确定使用哪种顺序容器](#)
* 通常，使用vector时最好的选择，除非你有很好的理由选择其他容器。
* 如果是你的程序有**很多小的元素，而空间的额外开销很重要**，否则不需要使用list或forward_list。
* 如果程序要求在容器的**中间插入或删除元素**，应使用list或forward_list。
* 如果程序要写**随机访问元素**，应使用vector或deque。
* 如果程序需要在**头尾位置插入或删除元素**，但不会在中间位置进行插入或删除操作，则使用deque。
* 如果程序只有在读取输入时才需要在容器中间位置插入元素，随后需要随机访问元素，则
   * 首先，确定是否真的需要在容器中间位置添加元素。当处理输入数据时，通常可以很容易地向vector追加数据，然后在 调用标准库的sort函数来重排容器中的元素，从而避免在中间位置添加元素。
   * 如果必须在中间位置插入元素，考虑在输入阶段使用list，一旦输入完成，将list中的内容拷贝到一个vector。





-----
`时间`: `[]` 