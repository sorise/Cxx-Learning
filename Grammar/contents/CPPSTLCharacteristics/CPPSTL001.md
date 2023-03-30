### [C++ STL 泛型编程（Generic Pogramming）](#)
面向对象编程关注的是编程的数据方面，而泛型编程关注的是算法。它们之间的共同点是抽象和可重用代码，但它们的理念绝然不同。
泛型编程旨在编写独立于数据类型的代码。在C++中，完成通用程序的工具是模板。

-----

- [x] [1. 标准模板库 STL](#1-标准模板库-stl)
- [x] [2. 迭代器](#2-迭代器)
- [x] [3. 容器](#3-容器)
- [x] [4. 序列容器](#4-序列容器)
- [x] [5. 关联容器](#5-关联容器)
- [x] [6. 泛型算法](#6-泛型算法)

-----

### [1. 标准模板库 STL](#)
STL提供了一组表示容器、迭代器、函数对象和算法的模板！模板使得算法独立于存储的数据结构，而迭代器使算法独立于使用的容器类型，它们都是STL通用方法的重要组成成分。 **简而言之，就是Java C# 里面的集合板块！**
STL的代码从广义上讲分为三类：algorithm（算法）、container（容器）和iterator（迭代器）!

* **容器**: container, 一系列集合类型 STL库还有很多容器，比如：向量（vector）、栈（stack）、队列（queue）、优先队列（priority_queue）、链表（list）、集合（set）、映射（map）等容器
* **迭代器**:能够用来遍历容器的对象，与能够遍历数组的指针类似，是广义指针, 迭代器使算法独立于使用的容器类型 每个迭代器都要定义一个合适的迭代器，类型是  一个名为 iterator的 typedef，作用域位整个类！
* **算法**: 算法是完成特定任务的方法 包括 排序 for_each() random_shuhffle() sort()
* **函数对象**: 是类似于函数的对象，想想 lambda 


> STL（Standard Template Library，标准模板库)是惠普实验室开发的一系列软件的统称。它是由Alexander Stepanov、Meng Lee和David R Musser在惠普实验室工作时所开发出来！

以 vector为例！它提供了许多方法例如 **begin() end() erase() insert() push_back()**
```cpp
#include <iostream>
#include <string>
#include<vector>

int main(int argc, char const* argv[])
{   
   std::vector<std::string> names {"remix","kicker","yami","tome"} ;
   std::cout<< "name count:" << names.size() << "\n"; //name count:4

   std::vector<std::string>::iterator pointer_u16str = names.begin();
   //auto pointer_u16str = names.begin(); //可以简写为 auto
   while (pointer_u16str != names.end())
   {
      std::cout << "name: " << (*pointer_u16str) << "\n";
      pointer_u16str++;
   }
   //删掉两个
   names.erase(names.begin(), names.begin() + 2);
   //这样内存里面就清0了
   std::cout<< "name count:" << names.size() << "\n"; //name count:2
   names.insert(names.begin(),std::string("gross"));
   names.insert(names.begin() + 1,std::string("kiss"));
   names.push_back("cnm");

   std::cout << "again iterator: \n";

   for (auto pd = names.begin(); pd != names.end(); pd++)
   {
      std::cout << "name: " << (*pd) << "\n";
   }

   return 0;
}
```
使用 for_each 算法来完成迭代
```cpp
std::for_each(names.begin(), names.end(), [](const std::string &v){
    std::cout << "name: " << v << "\n";
});
```
基于范围的 for循环：
```cpp
for (auto &&nm : names)
{
    std::cout << "name: " << nm << "\n";
}
```

#### [1.1 分配器](#)
各种STL容器模板都接受一个可选的模板参数，该参数指定使用哪个分配器对象来管理内存。
```cpp
template<
    class T,
    class Allocator = std::allocator<T>
> class list{
   private:
      value_type	T;
      allocator_type	Allocator;
   /*...*/
};
```
如果模板省略 Allocator 参数，那么就是用默认值 std::allocator<T>！它使用new和 delete!

#### [1.2 泛型编程](#)
模板使得算法独立于存储的数据结构，迭代器使算法独立于使用的容器类型，面向对象编程关注的是编程的 **数据方面** 泛型编程关注的是 **算法**  泛型编程旨在编写独立于数据类型的代码, 而模板正是泛型得以实现的工具！

### [2. 迭代器](#)
**迭代器不是指针，是类模板，表现的像指针。他只是模拟了指针的一些功能，通过 重载了指针的一些操作符，->,*,++ --等封装了指针**，是一个 用于遍历STL（ Standard Template Library）容器内全部或部分元素的对象，
本质是封装了原生指针，是指针概念的一种提升（lift），提供了比指针更高级的行为，相当于一种智能指针，他可以根据不同类型的数据结构来实现不同的++，–等操作；迭代器在使用后就释放了，不能再继续使用，但是指针可以。
**模板使得算法独立于存储的数据结构**，而 **迭代器** 使 **算法** **独立于** 使用的 **容器类型**，它们都是STL通用方法的重要组成成分。


比如我们想要实现一个find()查找算法，即返回第一个符合要求的值的位置。但如果同时面对一个数组数据结构和一个链表数据结构，由于它们的遍历方式不同，所以find()的实现细节也会不同。
```cpp
//用于数组类型遍历
template<typename T>
T * array_find(T *ptr, int len, const T& val){
   for (int i = 0; i < len; i++)
   {
      if (ptr[i] == val)
      {
         return ptr + i;
      }
   }
   return nullptr;
}

template<typename T>
struct Node
{
   T data;
   Node<T> *next;
};

//用于链表类型遍历
template<typename T>
Node<T>* linklist_find(Node<T>* head, const T & val){
   while (head != nullptr)
   {
      if (val == head->data)
      {
         return head;
      }
      head = head->next;
   }
   return nullptr;
}
```
由此可见，虽然有了模板，但是对于一个查找操作任然需要不同的算法实现！

如果是为了实现find()函数，迭代器应该具备以下特征：

* 应能够对迭代器执行解除引用的操作，以便能够访问它引用的值。即如果p是一个迭代器，则应对*p进行定义。
* 应能够将一个迭代器赋给另一个。即如果p和q都是迭代器，则应对表达式p=q进行定义。
* 应能够将一个迭代器与另一个进行比较，看它们是否相等。即如果p和q都是迭代器，则应对p==q和p!=q进行定义。
* 应能够使用迭代器遍历容器中的所有元素，这可以通过为迭代器p定义++p和p++来实现。

```cpp
template<typename T, typename iterator = T *>
iterator array_find(iterator ptr, int len, const T& val){
   for (int i = 0; i < len; i++)
   {
      if (ptr[i] == val)
      {
         return ptr + i;
      }
   }
   return nullptr;
}
```

对于链表，我们可以如下处理 手写一个迭代器
```cpp
template<typename T>
struct Node
{
   T data;
   Node<T> *next;
};

template<typename T>
class iterator
{
private:
   Node<T> *pt;
public:
   iterator():pt(nullptr){};
   iterator(Node<T> *ot):pt(ot){};
   T operator*() {return pt->data;}
   //++it
   iterator& operator++(){
      if (pt->next == nullptr)
      {
         pt = nullptr;
      }else{
         pt = pt->next;
      }
      return *this;
   }

   //it++; 先返回 再++
   iterator operator++(int){
      iterator temp = *this;
      pt = pt->next;
      return temp;
   }

   //bool operator==();
   
   bool operator!=(iterator ptr){
      if (this->pt == ptr.pt)
      {
         return false;
      }else{
         return true;
      }
   }
   ~iterator(){};
};
```
我们基于迭代器实现find方法
```cpp
template<typename T, typename iterator = iterator<T> >
iterator linklist_find(iterator head, iterator end, const T & val){
   iterator start = head;
   for (; start != end; start++)
   {
      if ((*start) == val)
      {
         return start;
      }
   }
   return iterator(nullptr);
}
```
运行试一试
```cpp
int main(int argc, char const* argv[])
{   

   Node<int> head = {10, nullptr};
   Node<int> gk1 = {12, nullptr};
   head.next = &gk1;
   Node<int> gk2 = {30, nullptr};
   gk1.next = &gk2;
   Node<int> gk3 = {40, nullptr};
   gk2.next = &gk3;

   iterator<int> it(&head);
   iterator<int> end;
 
   iterator<int> pt_ite = linklist_find(it, end, 30);
   std::cout<< *pt_ite  << '\n';//30
   return 0;
}
```
成功运行！ 可以见得迭代器本身是一个类，它对指针进行了包装！ 并且重载的运算符，使之可以实现指针的解引用操作，++ -- 等等  如此，每个容器类（vector、list、deque等）都定义了相应的迭代器类型。不论实现方式如何，迭代器都将提供所需的操作。

使用容器类时候，无需直达其迭代器是如何实现的，也无需知道超尾是如何实现的，每个容器类都有一个超尾标记，当迭代器递增到超越容器的最后一个值后，这个值将被赋给迭代器。每个容器都有begin()和end()方法，它们分别返回一个指向容器的第一个元素和超尾位置的迭代器。每个容器类都使用++操作，让迭代器从指向第一个元素逐步指向超尾位置，从而遍历容器中的每一个元素。

> STL通过为每个类定义适当的迭代器，并以统一的风格设计类，能够对内部表示绝然不同的容器，编写相同的代码。

```cpp
std::list<double> scores {58.56, 98.54, 774.12, 95.23};
for (auto pt = scores.begin(); pt != scores.end(); pt++)
{
   std::cout << *pt <<  '\n';
}
```

#### [2.1 迭代器类型](#) 
不同的算法对迭代器的要求不同，查找算法要求重载++运算符，只能查看数据，不能写数据。排序算法要求能够交换不相邻的元素，并且实现随机访问！ 可以实现 iterator + n

STL定义了5种迭代器，并根据所需的迭代器类型对算法进行了描述，它们分别是 1. 输入迭代器  2. 输出迭代器  3. 正向迭代器  4. 双向迭代器 5. 随机访问迭代器


例如 find函数就需要输入迭代器：
```cpp
template< class InputIterator, class T >
InputIterator find( InputIterator first, InputIterator last, const T& value );
```
sort排序函数就需要随机访问迭代器
```cpp
template< class RandomIterator >
void sort( RandomIterator first, RandomIterator last );
```

五种迭代器都支持解引用操作 重载了*运算符！ 还支持比较操作 重载了 == 运算符！

<table>
	<thead>
		<tr>
			<th align="left">迭代器类型</th>
			<th align="left">能够进行的操作</th>
			<th align="center">备注</th>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td align="left">输入迭代器
				<br>
				<code>input iterator</code>
			</td>
			<td align="left">1. 比较两个迭代器是否相等（==、!=）。
				<br>2. 前置和后置递增运算（++）（意味着它是单向移动的）。
				<br>3. 读取元素的解引用运算符（*）。只能<strong>读元素</strong>，也就是解引用只能出现<strong>在赋值运算符的右边。</strong>
				<br>4. 箭头运算符（-&gt;），解引用迭代器，并提取对象的成员。
				<br>5. 只能用于单遍扫描算法。</td>
			<td align="center">无。</td>
		</tr>
		<tr>
			<td align="left">输出迭代器
				<br>
				<code>output iterator</code>
			</td>
			<td align="left">1. 比较两个迭代器是否相等（==、!=）。
				<br>2. 前置和后置递增运算（++）（意味着它是单向移动的）。
				<br>3. 读取元素的解引用运算符（*）。只能<strong>写元素</strong>，也就是解引用只能出现<strong>在赋值运算符的左边。</strong>
				<br>4. 箭头运算符（-&gt;），解引用迭代器，并提取对象的成员。
				<br>5. 只能用于单遍扫描算法。</td>
			<td align="center">与输入迭代器的操作相似，只不过输入变成了输出。</td>
		</tr>
		<tr>
			<td align="left">前向迭代器
				<br>
				<code>forward iterator</code>
			</td>
			<td align="left">与输入迭代器的操作相似，只不过输入变成了输出。
				<br>1. 比较两个迭代器是否相等（==、!=）。
				<br>2. 前置和后置递增运算（++）（意味着它是<strong>单向移动</strong>的）。
				<br>3. 读取元素的解引用运算符（*）。<strong>可写也可读</strong>。
				<br>4. 箭头运算符（-&gt;），解引用迭代器，并提取对象的成员。
				<br>5. 能用于多遍扫描算法。</td>
			<td align="center">输入迭代器与输出迭代器操作的结合。</td>
		</tr>
		<tr>
			<td align="left">双向迭代器
				<br>
				<code>bidirectional iterator</code>
			</td>
			<td align="left">1. 比较两个迭代器是否相等（==、!=）。
				<br>2. 前置和后置递增运算（++）。
				<br>
				<br>3. 前置和后置递减运算（–）（意味着它是<strong>双向移动</strong>的）。
				<br>3. 读取元素的解引用运算符（*）。<strong>可写也可读</strong>。
				<br>4. 箭头运算符（-&gt;），解引用迭代器，并提取对象的成员。
				<br>5. 能用于多遍扫描算法。</td>
			<td align="center">支持所有前向迭代器操作的基础上，支持递减运算符，也就是<strong>支持双向移动</strong>。</td>
		</tr>
		<tr>
			<td align="left">随机访问迭代器
				<br>
				<code>random-access iterator</code>
			</td>
			<td align="left">1. 比较两个迭代器是否相等（==、!=），以及比较两个迭代器相对位置的关系运算符（&lt;、&lt;=、&gt;和&gt;=）。
				<br>2. 前置和后置递增运算（++）。
				<br>3. 支持和一个整数值的加减运算（+、+=、-、-=）。
				<br>4. 两个迭代器上的减法运算符（-），得到两个迭代器的距离。
				<br>5. 前置和后置递减运算（–）（意味着它是<strong>双向移动</strong>的）。
				<br>6. 读取元素的解引用运算符（*）。<strong>可写也可读</strong>。
				<br>7. 箭头运算符（-&gt;），解引用迭代器，并提取对象的成员。
				<br>8. 支持下标运算符（iter[n]），与*(iter[n])等价，访问距离起始迭代器n个距离的迭代器指向的元素。
				<br>9. 能用于多遍扫描算法。</td>
			<td align="center">在支持双向移动的基础上，支持前后位置的比较、随机存取、直接移动n个距离。</td>
		</tr>
	</tbody>
</table>

#### [2.2 输入迭代器](#)
input iterator 输入”的意思即读取，输入迭代器可被程序用来读取容器中的信息。具体地说，对输入迭代器解除引用将使程序能够读取容器中的值，但不一定能让程序修改值。

> 需要输入迭代器的算法将不会修改容器中的值

* 输入迭代器是单向迭代器，可以递增，但不可以倒退！ 输入迭代器可以设置为指向第一个元素，依次递曾，知道超尾位置
* 基于输入迭代器的任何算法都应当是单通行（single-pass）的，不依赖于前一次遍历时的迭代器值，也不依赖于本次遍历中前面的迭代器值。

**一个输入迭代器必须支持的操作：**
1. 用于比较两个迭代器的相等和不相等运算符（==、!=）
2. 用于推进迭代器的前置和后置递增运算符（++）
3. 用于读取元素的解引用运算符（*）；解引用只会出现在赋值运算符的右侧（即val= *iter;）
4.箭头运算符（->）,等价于(*it).member，即解引用迭代器，并提取对象的成员

输入迭代器只用于顺序访问。对于一个输入迭代器，*it++保证是有效的，但递增它可能导致所有其他指向流的迭代器失效。即，不能保证输入迭代器的状态可以保存下来，也不能保证用这个输入迭代器来访问元素。因此，输入迭代器只能用于单遍扫描算法。

总结一句话：输入迭代器主要用于读取容器的值。

#### [2.3 输出迭代器](#)
output iterator “输出”的意思即写入。输出迭代器与输入迭代器相似，只是解除引用让程序能修改容器值，而不能读取。

> 对于单通行、只写算法，可以使用输出迭代器。

**一个输出迭代器必须支持：**
1. 用于推进迭代器的前置和后置递增运算符（++）
2. 解引用运算符（*），只出现在赋值运算符的左侧（向一个已经解引用的输出迭代器赋值，就是将值写入它所指向的元素（ *iter=val;））

我们只能向一个输出迭代器赋值一次。类似输入迭代器，输出迭代器只能用于单遍扫描算法。用作目的位置的迭代器通常是输出迭代器。ostream_iterator类型就是输出迭代器。

总结一句话：输出迭代器主要利用一个容器的输入迭代器，将值写入到输出迭代器所指向的另一个容器中去。

#### [2.4 正向迭代器](#)
forward iterator  正向迭代器也只能使用++来遍历容器，即每次沿容器向前移动一个元素。

但是，与输入、输出迭代器不同的是，它总是按相同的顺序遍历一系列值。另外，将正向迭代器递增后，仍然可以对前面的迭代器值解除引用（如果保存了它），并可以得到相同的值。

适合多次通行算法，正向迭代器既可以读取和修改数据，也可以使得只能读取数据（const）。

> 算法replace要求前向迭代器，forward_list上的迭代器也是前向迭代器。
#### [2.5 双向迭代器](#)
bidirectional iterator  双向迭代器具有正向迭代器的所有特性，同时支持两种（前缀和后缀）递减运算符。

#### [2.6 随机访问迭代器](#)
random-access iterator 随机访问迭代器具有双向迭代器的所有特性，同时添加了支持随机访问的操作（如指针增加运算）和用于对元素进行排序的关系运算符。


1.用于比较两个迭代器相对位置的关系运算符（<、<=、>和>=）
2.迭代器和一个整数值的加减运算（+，+=，-和-=），计算结果是迭代器在序列中前进或后退给定的整数个元素后的位置
3.用于两个迭代器上的减法运算符（-），得到两个迭代器的距离
4.下标运算符（iter[n]）与*（ *iter[n]）等价

> 提供在常数时间内访问任意元素的能力 算法sort 使用随机访问迭代器


#### [2.7 迭代器的层次结构](#)
迭代器类型形成了一个层次结构。

* 正向迭代器具有输入、输出迭代器的全部功能，同时还有自己的功能；
* 双向迭代器具有正向迭代器的全部功能，同时还有自己的功能；
* 随机访问迭代器具有双向迭代器的全部功能，同时还有自己的功能。

根据特定迭代器类型编写的算法可以使用该种迭代器，也可以使用具有所需功能的任何其他迭代器。例如具有随机访问迭代器的容器可以使用为输入迭代器编写的算法。

目的是为了在编写算法尽可能使用要求最低的迭代器，并让它适用于容器的最大区间。

#### [2.8 将指针用作迭代器](#)
**指针也是迭代器，支持 STL遍历** 迭代器是广义指针，而指针满足所有的迭代器要求。由于指针是迭代器，而算法是基于迭代器的，因此STL算法可以使用指针来对基于指针的非STL容器进行操作。例如，可将STL算法用于常规数组。

例如，可以使用sort()对一个数组进行排序：
```cpp
const int SIZE = 100;
double Receipts[SIZE];

std::sort(Receipts, Receipts + SIZE);
```

```cpp
double scores[6] = {89.56, 75.12,99.5,85.6,45.2,21.26};
std::sort(scores, scores + 6);
for (auto &&val : scores)
{
   std::cout << val << std::endl;
}
```
同样，可以将STL算法用于自己设计的数组形式，只要提供适当的迭代器（可以是指针，也可以是对象）和超尾指示器即可。

copy函数可以用来复制从一个容器到另一容器，前两个参数说明复制范围，后一个参数说明复制到另一个容器的哪一个位置
```cpp
double scores[6] = {89.56, 75.12,99.5,85.6,45.2,21.26};
std::list<double> mylist(6);
std::copy(scores, scores + 6, mylist.begin());

for (auto &&val : mylist)
{
   std::cout << val << std::endl;
}
```

**内置函数 begin(数组名) end(数组名)** 为了更加安全的获得尾后指针，C++11新标准引入了上述两个函数，与容器的 begin和end的功能类似。
```cpp
int scores[5] = {1,2,4,8,16};

//这样的写法有可能出错！
std::for_each(scores, scores + 5,  [](auto v){ std::cout<< v << "\n";});
```
**安全写法:**
```cpp
#include<iterator>
int scores[5] = {1,2,4,8,16};

std::for_each(std::begin(scores), std::end(scores),  [](auto v){ std::cout<< v << "\n";});
```

#### [2.9 ostream_iterator和 istream_iterator](#)
STL为这种迭代器提供了ostream_iterator模板，用STL的话说，该模板是输出迭代器概念的一个模型，它也是一个适配器（adapter）——一个类或模板，可以将一些其他接口转换为STL使用的接口。 
可以通过包含头文件iterator并作下面的声明来创建这种迭代器：

##### [2.9.1 ostream_iterator](#)
输出流迭代器 ostream_iterator属于I/O流STL适配器，用于获取一个元素，同时保存在缓冲器中，可以供Cout输出。如果把cout看做成一个对 象，那么在Cout对象当中存在一片用于数据存储的区域。ostream_iterator在STL中一般配合copy函数一起使用，如下代码；

```cpp
#include<iterator>
//...
std::ostream_iterator<int, char> out_iter(std::cout, " ");
// can use it like:
*out_iter++ = 15; // works like cout << 15 << " ";
```
**ostream_iterator**
* 第一个模板参数：说明输出类型
* 第二个模板参数：说明以什么字符类型输出 可以是 wchar_t
* 构造函数第一个参数:说明输出流，可以是cout,也可以是文件流
* 构造函数第二个参数:说明每个输出之间的分隔符！

```cpp
std::ostream_iterator<int, char> out_iter(std::cout, ",");
double scores[6] = {89.56, 75.12,99.5,85.6,45.2,21.26};
std::copy(scores, scores + 6, out_iter);
printf("\n");

//89.56,75.12,99.5,85.6,45.2,21.26,
```

##### [2.9.2 istream_iterator](#)
输入流迭代器

```cpp
istream_iterator<int> intie(cin);                //后面紧跟要输入一个数据
istream_iterator<int> intie_();                  //默认构造方法
```

```cpp
istream_iterator<int> getInt(cin); //从cin 中输入字符串
istream_iterator<int> eof;//尾后迭代器
vector<int> v(getInt,eof);//循环读取，直到输入不是int数据为止

//注意：不可利用这个方式读取字符串，否则无法结束输入
```

利用输入流迭代器istream_iterator进行文件操作：
```cpp
std::ifstream i_file("data.txt"); //打开文件
istream_iterator<string> getStr(i_file); //从i_file读取字符串
istream_iterator<string> eof;//尾后迭代器 
while (getStr != eof)//判断是否到达尾部
{
   std::cout << *getStr << std::endl;
   ++getStr;//迭代器移动
}
```

#### [2.10 reverse_iterator](#)
对reverse_iterator执行递增操作将导致它被递减，这主要是为了简化对已有函数的使用。

例如，想要反向打印容器的内容，可以使用下面的语句：
```cpp
copy(dice.rbegin(), dice.rend(), out_iter); // display in reverse order
```
* rbegin()和 end() 都返回相同的值(超尾)，但类型不同(reverse_iterator和iterator)。 同样rand()和begin()也返回相同的值，但是迭代器类型不一样！

```cpp
std::list<double> myList {89.56, 75.12,99.5,85.6,45.2,21.26};
std::for_each(myList.rbegin(), myList.rend(), [](auto v) { std::cout << v << ' '; });
printf("\n");
```

#### [2.11 insert_iterator](#)
一共优三种插入迭代器，能够解决空间不足的问题，因为会自动插入新的元素，而不会覆盖已有的数据，并使用自动内存分配来确保能够容纳新的信息。

* back_insert_iterator 将元素插入到容器尾部；只能用于在尾部快速插入的容器(快速插入是一个时间固定的算法) 适合 vector
* front_insert_iterator 将元素插入到容器的前端； 只能用于在头部快速插入的容器。  适合 queue
* insert_iterator 将元素插入到insert_iterator构造函数参数指定的位置前面。

这三个插入迭代器都是输出容器概念的模型。

这些迭代器将容器类型作为模板参数，将实际的容器标识符作为构造函数参数，也就是说，要为名为dice的vector<int>容器创建一个back_insert_iterator，可以这样做：

```cpp
int main(int argc, char const* argv[])
{   
   std::list<double> myList {89.56, 75.12,99.5,85.6,45.2,21.26};
   std::vector<double> container {100.0,90.10};
   
   std::back_insert_iterator<std::vector<double>> back_ist_container(container);
   copy(myList.begin(), myList.end(), back_ist_container);

   std::for_each(container.begin(), container.end(), [](auto v) { std::cout << v << ' '; });
   printf("\n");
   //100 90.1 89.56 75.12 99.5 85.6 45.2 21.26 
   return 0;
}
```

必须声明容器类型的原因是，迭代器必须使用合适的容器方法。譬如，back_insert_iterator的构造函数将假设传递给它的类型有一个push_back()方法，因为copy()是一个独立的函数，没有重新调整容器大小的权限，但前面的声明让back_iter能够使用方法vector<int>::push_back()，该方法有这样的权限。声明front_insert_iterator的方法与此相同。

对于insert_iterator声明，还需一个指示插入位置的构造函数参数：

```cpp
insert_iterator<vector<int>> insert_iterator(dice, dice.begin());
```

#### [2.12 back_inserter](#)
back_inserter接受一个指向容器的引用，返回一个与该容器绑定的插入迭代器。当我们通过迭代器赋值时，赋值运算符会调用push_back将一个具有给定值的元素添加到容器中。
```cpp
std::vector<int> voters_b1;
auto it = std::back_inserter(voters_b1);
std::fill_n(it, 10, 999);

for (auto&& v : voters_b1) {
	printf("%d ", v);
}
```
### [3. 容器](#) 
STL具有容器概念和容器类型。 概念是具有名称（如容器、序列容器、关联容器等）的通用类别； 容器类型是可用于创建具体容器对象的模板。

以前11个容器类型分别是
* deque、list、queue、priority_queue、stack、vector、map、multimap、set、multiset和bitset（这个是在比特级处理数据的容器）。

C++11新增了
* forward_list、unordered_map、unordered_multimap、unordered_set和unordered_multiset，且不将bitset视为容器，而将其视为一种独立的类别。

#### [3.1 容器概念](#)
没有与基本容器概念对应的类型，但概念描述了所有容器类都通用的元素。它是一个概念化的抽象基类，说它概念化是因为容器类并不真正使用继承机制。换句话说，容器概念指定了所有STL容器类都必须满足的一系列要求。

容器是存储其他对象的对象。被存储的对象必须是同一种类型的。存储在容器中的数据为容器所有，所以当容器过期时，存储在容器中的数据也将过期（然而，如果数据是指针的话，则它指向的数据并不一定过期）。

存储在容器中的对象，其类型需要满足一定的要求：
* **类型必须是可复制构造和可赋值的。** 基本类型满足这些要求；类定义只要没有将复制构造函数和赋值运算符声明为私有或保护的，则也满足这种要求。
* C++11改进了这些概念，添加了术语 **可复制插入（CopyInsertable）和可移动插入（MoveIsertable）。**

基本容器不能保证其元素都按特定的顺序存储，也不能保证元素的顺序不变，但对概念进行改进后，则可以增加这样的保证。所有的容器都提供某些特征和操作，下图为一些通用特征进行了总结。

|表达式|返回类型|说明|复杂度|
|:---|:---|:---|:---|
|X::iterator|指向T的迭代器类型|满足正向迭代器要求的任何迭代器|编译时间|
|X::const_iterator|T|T的 const 类型|编译时间|
|X::size_type|无符号整形|容器大小|编译时间|
|X::difference_type|带符号整数|保持两个迭代器之间的举例|编译时间|
|X::value_type|T|T的类型|编译时间|
|X::reference|T左值引用|元素的左值引用|编译时间|
|X::const_reference|T const 左值引用|元素的const 左值引用|编译时间|
|X u| |创建一个空容器|固定|
|X x(begin, end)| |用其他容器创建一个新容器|线性|
|X x{a,b,c,d,e...}| |使用列表初始化|线性|
|X()| |创建匿名容器|固定|
|X u(a)； X u = a| |复制构造函数|线性|
|u1 = u2|X&|赋值运算符 |线性|
|X = {a,b,c,d,e...}| |将容器里面的元素替换位列表中的元素|线性|
|(&a)->~X()|void|对容器里面每个元素应用析构函数 |线性|
|u.begin()|迭代器|返回开始迭代器|固定|
|u.end()|迭代器|返回超尾迭代器 | 固定|
|u.size()| |返回元素个数 end() - begin()|固定|
|u.max_size()| |返回容器可保持的最大元素数目|固定|
|u.empty()| |返回容器是否位空|固定|
|u.insert(args)| |把args中的元素拷贝进c|固定|
|u.emplace(init)| |使用init构造c中的一个元素|固定|
|u.erase(args)| |删除args指定的元素|固定|
|u.clear()|void|删除所有元素 返回void|固定|
|u.swap(b)| |交换u与b的内容|固定|
|swap(u，b)|void|交换u与b的内容|固定|
|u.assign(n，b)|void|将u替换为具有哪个元素大小的每个元素都是b的容器|固定|
|u.assign(it)|void|将u里面所有的元素都替换为it|固定|
|u.assign(begin，end)|void|将u里面所有的元素都替换为 begin - > end 的值|固定|
|u1 == u2|可转换为bool| |线性|
|u1 != u2|可转换为bool| |线性|


复杂度描述了执行操作所需的时间，这里有3种开那些，从快到慢依次为：

* 编译时间：操作在编译时执行，执行时间为0；
* 固定时间：操作发生在运行阶段，但独立于对象中的元素数。
* 线性时间：时间与元素数目成正比。

**C++新增的容器要求:**
|表达式|返回类型|说明|复杂度|
|:---|:---|:---|:---|
|X a(right_value)| |调用移动构造函数后u的值和rv的原始值相同|线性|
|X a = right_value| |调用移动构造函数后u的值和rv的原始值相同|线性|
|a = right_value| X&|调用赋值构造函数后，u的值与rv的原始值相同 |线性|
|u.cbegin()|const_iterator|返回开始 const 迭代器|固定|
|u.cend()|const_iterator|返回超尾 const 迭代器 | 固定|


复制构造和复制赋值以及移动构造和移动赋值之间的差别在于，复制操作保留源对象，而移动操作可修改源对象，还可能转让所有权，而不做任何复制。如果源对象是临时的，移动操作的效率将高于常规复制。

#### [3.2 容器适配器](#)
容器适配器是一个封装了序列容器的类模板，它在一般序列容器的基础上提供了一些不同的功能。之所以称作适配器类，是因为它可以通过适配容器现有的接口来提供不同的功能。

STL 提供了 3 种容器适配器，分别为 stack 栈适配器、queue 队列适配器以及 priority_queue 优先权队列适配器。其中，各适配器所使用的默认基础容器以及可供用户选择的基础容器。

* **stack<T>**：是一个封装了 deque<T> 容器的适配器类模板，默认实现的是一个后入先出（Last-In-First-Out，LIFO）的压入栈。stack<T> 模板定义在头文件 stack 中。
* **queue<T>**：是一个封装了 deque<T> 容器的适配器类模板，默认实现的是一个先入先出（First-In-First-Out，LIFO）的队列。可以为它指定一个符合确定条件的基础容器。queue<T> 模板定义在头文件 queue 中。
* **priority_queue<T>**：是一个封装了 vector<T> 容器的适配器类模板，默认实现的是一个会对元素排序，从而保证最大元素总在队列最前面的队列。priority_queue<T> 模板定义在头文件 queue 中。

<table>
  <tbody>
    <tr>
      <th>
        容器适配器</th>
      <th>
        基础容器筛选条件</th>
      <th>
        默认使用的基础容器</th>
    </tr>
    <tr>
      <td>
        stack&nbsp;</td>
      <td>
        基础容器需包含以下成员函数：
        <ul>
          <li>
            empty()</li>
          <li>
            size()</li>
          <li>
            back()</li>
          <li>
            push_back()</li>
          <li>
            pop_back()</li>
        </ul>
        满足条件的基础容器有 vector、deque、list。</td>
      <td>
        deque</td>
    </tr>
    <tr>
      <td>
        queue</td>
      <td>
        基础容器需包含以下成员函数：
        <ul>
          <li>
            empty()</li>
          <li>
            size()</li>
          <li>
            front()</li>
          <li>
            back()</li>
          <li>
            push_back()</li>
          <li>
            pop_front()</li>
        </ul>
        满足条件的基础容器有 deque、list。</td>
      <td>
        deque</td>
    </tr>
    <tr>
      <td>
        priority_queue</td>
      <td>
        基础容器需包含以下成员函数：
        <ul>
          <li>
            empty()</li>
          <li>
            size()</li>
          <li>
            front()</li>
          <li>
            push_back()</li>
          <li>
            pop_back()</li>
        </ul>
        满足条件的基础容器有vector、deque。</td>
      <td>
        vector</td>
    </tr>
  </tbody>
</table>

#### [3.3 指定适配器](#)
默认情况下stack是基于deque的，queue是基于deque序列容器的，priority_queue是基于vector序列容器的，但是你也可以指定适配器的序列容器。
我们可以在创建一个适配器的时候将一个命名的顺序容器作为第二个类型参数来重载默认容器类型。

```cpp
std::stack<std::string, std::list<std::string>> values;
std::stack<double, std::vector<double>> scores;
```
可以用一个基础容器来初始化 stack 适配器，只要该容器的类型和 stack 底层使用的基础容器类型相同即可
```cpp
std::list<int> values {1, 2, 3};
std::stack<int,std::list<int>> my_stack (values);
```

**注意事项：**
* [**适配器不能构建在 std::array<> 模板类之上。**](#)
* [**stack适配器不能构建在 std::array<>、forward_list 模板类之上。**](#)
* [**queue适配器只能构建在 std::vector<>、std::list<> 模板类之上。**](#)
* [**priority_queue适配器只能构建在 std::vector<>、std::deque<> 模板类之上。 不能构建在 list<>之上。**](#)


### [4. 序列容器](#) 
序列（sequence）是一种重要的改进，因为7种STL容器类型（deque、C++新增的forward_list、list、queue、priority_queue、stack和vector）都是序列。

* 双端队列（deque）表示允许在两端添加和删除元素。
* 数组（array）也被归类到序列容器，虽然它并不满足序列的所有要求。

序列概念增加了迭代器至少是正向迭代器这样的要求，这保证了元素将按特定顺序排列，不会在两次迭代之间发生变化。

> 序列还要求其元素按严格的线性顺序排列，即存在第一个元素、最后一个元素，除第一个元素和最后一个元素外，每个元素前后都分别有一个元素。数组和链表都是序列，但分支结构（其中每个节点都指向两个子节点）不是。

<table class="wikitable" style="font-size:0.8em; line-height:1em; text-align: center;">
	<tbody>
		<tr>
			<th rowspan="2"> 类别
			</th>
			<th rowspan="2"> 容器
			</th>
			<td colspan="2">
				<b>插入</b>后……
			</td>
			<td colspan="2">
				<b>擦除</b>后……
			</td>
			<th rowspan="2"> 条件
			</th>
		</tr>
		<tr>
			<td>
				<b>迭代器</b>有效？
			</td>
			<td>
				<b>引用</b>有效？
			</td>
			<td>
				<b>迭代器</b>有效？
			</td>
			<td>
				<b>引用</b>有效？
			</td>
		</tr>
		<tr>
			<th rowspan="8"> 顺序容器
			</th>
			<th>
				<a href="https://zh.cppreference.com/w/cpp/container/array" title="cpp/container/array">
					<tt>array</tt>
				</a>
			</th>
			<td colspan="2" style="background: #ececec; color: grey; vertical-align: middle; text-align: center;" class="table-na">
				<small>N/A</small>
			</td>
			<td colspan="2" style="background: #ececec; color: grey; vertical-align: middle; text-align: center;" class="table-na">
				<small>N/A</small>
			</td>
			<td>
			</td>
		</tr>
		<tr>
			<th rowspan="3">
				<a href="https://zh.cppreference.com/w/cpp/container/vector" title="cpp/container/vector">
					<tt>vector</tt>
				</a>
			</th>
			<td colspan="2" style="background:#ff9090; color:black; vertical-align: middle; text-align: center;" class="table-no"> 否
			</td>
			<td colspan="2" style="background: #ececec; color: grey; vertical-align: middle; text-align: center;" class="table-na">
				<small>N/A</small>
			</td>
			<td> 插入更改容量
			</td>
		</tr>
		<tr>
			<td colspan="2" style="background: #90ff90; color: black; vertical-align: middle; text-align: center;" class="table-yes">是
			</td>
			<td colspan="2" style="background: #90ff90; color: black; vertical-align: middle; text-align: center;" class="table-yes">是
			</td>
			<td> 在被修改元素前
			</td>
		</tr>
		<tr>
			<td colspan="2" style="background:#ff9090; color:black; vertical-align: middle; text-align: center;" class="table-no"> 否
			</td>
			<td colspan="2" style="background:#ff9090; color:black; vertical-align: middle; text-align: center;" class="table-no"> 否
			</td>
			<td> 在被修改元素处或元素后
			</td>
		</tr>
		<tr>
			<th rowspan="2">
				<a href="https://zh.cppreference.com/w/cpp/container/deque" title="cpp/container/deque">
					<tt>deque</tt>
				</a>
			</th>
			<td rowspan="2" style="background:#ff9090; color:black; vertical-align: middle; text-align: center;" class="table-no"> 否
			</td>
			<td style="background: #90ff90; color: black; vertical-align: middle; text-align: center;" class="table-yes">是
			</td>
			<td colspan="2" style="background: #ffff90; color: black; vertical-align: middle; text-align: center;" class="table-maybe">是，除了被擦除元素
			</td>
			<td> 修改首元素或尾元素
			</td>
		</tr>
		<tr>
			<td style="background:#ff9090; color:black; vertical-align: middle; text-align: center;" class="table-no"> 否
			</td>
			<td colspan="2" style="background:#ff9090; color:black; vertical-align: middle; text-align: center;" class="table-no"> 否
			</td>
			<td> 只修改中间元素
			</td>
		</tr>
		<tr>
			<th>
				<a href="https://zh.cppreference.com/w/cpp/container/list" title="cpp/container/list">
					<tt>list</tt>
				</a>
			</th>
			<td colspan="2" style="background: #90ff90; color: black; vertical-align: middle; text-align: center;" class="table-yes">是
			</td>
			<td colspan="2" style="background: #ffff90; color: black; vertical-align: middle; text-align: center;" class="table-maybe">是，除了被擦除元素
			</td>
			<td>
			</td>
		</tr>
		<tr>
			<th>
				<a href="https://zh.cppreference.com/w/cpp/container/forward_list" title="cpp/container/forward list">
					<tt>forward_list</tt>
				</a>
			</th>
			<td colspan="2" style="background: #90ff90; color: black; vertical-align: middle; text-align: center;" class="table-yes">是
			</td>
			<td colspan="2" style="background: #ffff90; color: black; vertical-align: middle; text-align: center;" class="table-maybe">是，除了被擦除元素
			</td>
			<td>
			</td>
		</tr>
		<tr>
			<th> 关联容器
			</th>
			<th>
				<a href="https://zh.cppreference.com/w/cpp/container/set" title="cpp/container/set">
					<tt>set</tt>
				</a>
				<p>
					<a href="https://zh.cppreference.com/w/cpp/container/multiset" title="cpp/container/multiset">
						<tt>multiset</tt>
					</a>
				</p>
				<p>
					<a href="https://zh.cppreference.com/w/cpp/container/map" title="cpp/container/map">
						<tt>map</tt>
					</a>
				</p>
				<p>
					<a href="https://zh.cppreference.com/w/cpp/container/multimap" title="cpp/container/multimap">
						<tt>multimap</tt>
					</a>
				</p>
			</th>
			<td colspan="2" style="background: #90ff90; color: black; vertical-align: middle; text-align: center;" class="table-yes">是
			</td>
			<td colspan="2" style="background: #ffff90; color: black; vertical-align: middle; text-align: center;" class="table-maybe">是，除了被擦除元素
			</td>
			<td rowspan="1">
			</td>
		</tr>
		<tr>
			<th rowspan="2"> 无序关联容器
			</th>
			<th rowspan="2">
				<a href="https://zh.cppreference.com/w/cpp/container/unordered_set" title="cpp/container/unordered set">
					<tt>unordered_set</tt>
				</a>
				<p>
					<a href="https://zh.cppreference.com/w/cpp/container/unordered_multiset" title="cpp/container/unordered multiset">
						<tt>unordered_multiset</tt>
					</a>
				</p>
				<p>
					<a href="https://zh.cppreference.com/w/cpp/container/unordered_map" title="cpp/container/unordered map">
						<tt>unordered_map</tt>
					</a>
				</p>
				<p>
					<a href="https://zh.cppreference.com/w/cpp/container/unordered_multimap" title="cpp/container/unordered multimap">
						<tt>unordered_multimap</tt>
					</a>
				</p>
			</th>
			<td style="background:#ff9090; color:black; vertical-align: middle; text-align: center;" class="table-no"> 否
			</td>
			<td rowspan="2" style="background: #90ff90; color: black; vertical-align: middle; text-align: center;" class="table-yes">是
			</td>
			<td colspan="2" style="background: #ececec; color: grey; vertical-align: middle; text-align: center;" class="table-na">
				<small>N/A</small>
			</td>
			<td> 插入导致重哈希
			</td>
		</tr>
		<tr>
			<td style="background: #90ff90; color: black; vertical-align: middle; text-align: center;" class="table-yes">是
			</td>
			<td colspan="2" style="background: #ffff90; color: black; vertical-align: middle; text-align: center;" class="table-maybe">是，除了被擦除元素
			</td>
			<td> 无重哈希
			</td>
		</tr>
	</tbody>
</table>


#### [4.1 序列概念模型的要求](#)
对于一个序列，我们在概念模型设计上应该有如下的操作被实现 t是容器里面的值类型对象 i，j 表示迭代器！

|表达式|返回类型|说明|
|:---|:---|:---|
|X a(n,t)| |声明一个名字为a的由n个t值组成的序列|
|X(n,t)| |创建匿名序列，其他和上面一样|
|X a(i,j)| |创建一个名字为a的序列，初始化为区间[i,j)的内容|
|X(i,j)| |创建匿名容器，初始化为区间[i,j)的内容|
|a.insert(p, t)|迭代器|将t插入到p前面；|
|a.insert(p,n, t)|void|将n个t插入到p前面；|
|a.insert(p,i,j)|void|将区间[i, j)中的元素插入到p的前面|
|a.erase(p)|迭代器|删除p指向的元素|
|a.erase(p, q)|迭代器|删除区间[p,q)中的元素|
|a.clear()|void|清空迭代器等于 a.erase(a.begin(), a.end())|

**C++ 序列的可选要求：**

|表达式|返回类型|含义|容器|
|:---|:---|:---|:---|
|a.front()|T&|*a.begin()  返回容器首个元素的引用|forward_list、vector、list、deque|
|a.back()|T&|*--a.end() 返回容器最后一个元素的引用 |vector、list、deque|
|a.push_front(t)| |头部插入 a.insert(a.begin(), t)|list、deque|
|a.push_back(t)| |尾部插入 a.insert(a.end(), t)|vector、list、deque|
|a.pop_front(T)| |a.erase(a.begin())|list、deque|
|a.pop_back(t)| |a.erase(--a.end())|vector、list、deque|
|a[n]|T&|*(a.begin() + n)|vector, deque|
|a.at[n]|T&|*(a.begin() + n)|vector, deque|


#### [4.2 序列实现](#)
* array<T,N> (数组容器) ：是一个长度固定的序列，有 N 个 T 类型的对象，不能增加或删除元素。
* vector (向量容器) ：是一个长度可变的序列，用来存放T类型的对象。是一个长度可变的序列容器，即在存储空间不足时，会自动申请更多的内存。使用此容器，在尾部增加或删除元素的效率最高（时间复杂度为 O(1) 常数阶），在其它位置插入或删除元素效率较差（时间复杂度为 O(n) 线性阶，其中 n 为容器中元素的个数）；
* deque (双向队列容器) ：和 vector 非常相似，区别在于使用该容器不仅尾部插入和删除元素高效，在头部插入或删除元素也同样高效，时间复杂度都是 O(1) 常数阶，但是在容器中某一位置处插入或删除元素，时间复杂度为 O(n) 线性阶；
* list (链表容器) 是一个长度可变的、由 T 类型对象组成的序列，它以双向链表的形式组织元素，在这个序列的任何地方都可以高效地增加或删除元素。访问容器中任意元素的速度要比前三种容器慢，这是因为 list 必须从第一个元素或最后一个元素开始访问，需要沿着链表移动，直到到达想要的元素。
* forward list (正向链表容器) ：是一个长度可变的、由 T 类型对象组成的序列，它以单链表的形式组织元素，是一类比链表容器快、更节省内存的容器，但是它内部的元素只能从第一个元素开始访问。


### [5. 关联容器](#) 
**关联容器（associative container）** 是容器概念的另一个改进。主要有集合和字典又分为两个大类，有序关联容器和无序关联容器！

C++ STL中标准关联容器set, multiset, map, multimap内部采用的就是一种非常高效的平衡检索二叉树：红黑树，也成为RB树(Red-Black Tree)。RB树的统计性能要好于一般平衡二叉树，所以被STL选择作为了关联容器的内部结构。

#### 5.1 有序关联容器
STL提供了4中关联容器：set、multiset、map和multimap。前两种定义在头文件<set>，后两种定义在头文件 <map>。 **这4个关联容器都是有序的，会自动进行排序。**
使用二叉树实现

* set是最简单的关联容器，其值类型于键相同，键是唯一的，这意味着集合中不会有多个相同的键。对于set来说，值就是键。
* multiset类似于set，只是可能有多个值的键相同。例如，如果键和值的类型为int，则multiset对象包含的内容可以是1、2、2、2、3、5、7、7。
* map中值与键的类型不同，键是唯一的，每个键只对应一个值。
* multimap与map相似，只是一个键可以与多个值关联。

#### 5.2 无序关联容器
有4中无序关联容器，它们是 unordered_set、unordered_multiset、unordered_map、unordered_multimap。 基于hash散列表实现。


无序关联容器提供能快速查找（均摊 O(1) ，最坏情况 O(n) 的复杂度）的无序（哈希）数据结构。

* unordered_set   唯一键的集合，按照键生成散列
* unordered_map   键值对的集合，按照键生成散列，键是唯一的
* unordered_multiset  键的集合，按照键生成散列
* unordered_multimap 键值对的集合，按照键生成散列


### [6. 泛型算法](#)
容器虽然定义了很多的操作，但是对于一些复制要求还是需要算法来实现。
STL算法函数设计，有两个主要的通用部分，都是要模板来提供泛型；其次，他们都是要迭代器来提供访问容器中数据的通用表示！ 算法主要在以下三个头文件中：

```cpp
#include<algorithm>  //大多数算法都在里面
#include<numberic>
#include<functional>
```
**标准库提供是算法是泛型的，并未给每个容器都定义成员函数来实现算法，而是一组泛型算法，实现一些经典算法的公共接口，它并不直接操作容器，而是遍历由两个迭代器指定的一个元素范围**。

算法库提供大量用途的函数（例如查找、排序、计数、操作），它们在元素范围上操作。 **注意范围定义为 [first, last)** ，**其中 last 指代要查询或修改的最后元素的后一个元素。**

[**算法独立于容器是依靠迭代器实现的**](#)， 虽然算法独立于特定类型，但也要求元素满足某些要求，比如重载了一些运算符 例如 比较运算符 == 、> 、 <等。

**算法组：**
* 非修改序列操作 algorithm 文件中
* 修改式序列操作 algorithm 文件中
* 排序和相关操作 algorithm 文件中
* 通用数字运算 numberic 文件中

#### [6.1 非修改序列操作](#)
主要用于判断，查找，遍历等等！

```cpp
template< class InputIt, class UnaryPredicate >
constexpr bool any_of( InputIt first, InputIt last, UnaryPredicate p );
```

* all_of 检查一元谓词 p 是否对范围 [first, last) 中所有元素返回 true 。
* any_of 检查一元谓词 p 是否对范围 [first, last) 中至少一个元素返回 true 。
* none_of 检查一元谓词 p 是否不对范围 [first, last) 中任何元素返回 true 。
* for_each(InputIt first, InputIt last, UnaryFunction f) 遍历操作
* count( InputIt first, InputIt last, const T &value ); 计数等于 value 的元素。
* count_if( InputIt first, InputIt last, Predict p) 计数谓词 p 对其返回 true 的元素。

查找操作！
```cpp
std::list<int> vals{98,75,84,65,91,32,15};
std::list<int>::iterator it = std::find(vals.begin(), vals.end(), 37);
if (it == vals.end())
{
	std::cout << "not find in list" << "\n";
}
else {
	std::cout << *it <<"\n";
}
```

找到最小值：
```cpp
std::initializer_list<int> vals{98,75,84,65,91,32,15};
std::cout << std::min(vals, [](const int t1, const int t2)-> bool {
	return t1 < t2;
}) << "\n"; //15
```


-----
最后修订时间: [2022年12月20日16:23:01] 