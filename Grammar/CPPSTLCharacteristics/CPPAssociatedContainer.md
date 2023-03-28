### [C++ 有序关联容器](#)
无非是集合和字典类型！vector封装数组，list封装了链表，map和set封装了二叉树、哈希表等！ 提供了对元素快速访问！ 

-----
- [x] [1. set 集合](#1-set-集合)
- [x] [2. multiset 可重复集合](#2-multiset)
- [x] [3. set algorithm 集合操作](#3-set-algorithm)
- [x] [4. pair 类型](#6-pair-类型)
- [x] [5. map 字典](#5-map-字典)
- [x] [6. multimap 可重复字典](#6-multimap-可重复字典)
-----

### [1. set 集合](#)
自动排序的不重复集合！ 不存在相同的键！ 头文件 `#include <set>` 需要使用 std 命名空间 `using namespace std;` **set** 最主要的作用就是自动去重并按升序排序，适用于需要去重但是又不方便直接开数组的情况。

```cpp
template<
    class Key, class Compare = std::less<Key>,
    class Allocator = std::allocator<Key>
> class set;
```

**部分内置类型：（set 类型的key_type和value_type是一样的，map不是）**

| 成员类型               | 字段                                   |
| ---------------------- | -------------------------------------- |
| key_type               | Key                                    |
| value_type             | Key                                    |
| size_type              | 无符号整数类型                         |
| difference_type        | 有符号整数类型                         |
| const_iterator         | 指向 const value_type 的老式双向迭代器 |
| iterator               | 指向 value_type 的常老式双向迭代器     |
| reverse_iterator       | std::reverse_iterator<iterator>        |
| const_reverse_iterator | std::reverse_iterator<const_iterator>  |



#### [1.1 构造函数](#)

set的构造函数分为以下几个关键部分

* **alloc**:  用于此容器所有内存分配的分配器
* **comp**: 用于所有关键比较的比较函数对象， true 在前，false在后。
* **first, last**: 复制元素的来源范围
* **other**: 将用作初始化容器元素所用源的另一容器
* **init**: 初始化容器元素所用的 initializer_list

主要关键如下集合参数！
```cpp
std::set<Key,Compare,Allocator>::set
set(); //无参构造函数
set( const set& other ); //复制构造函数
set( set&& other ); //移动构造函数
set( set&& other, const Allocator& alloc ); //移动构造函数 重载
//构造函数
template< class InputIt >
set( InputIt first, InputIt last, const Compare& comp = Compare(),const Allocator& alloc = Allocator() );
//构造函数
template< class InputIt >
set( InputIt first, InputIt last, const Allocator& alloc): set(first, last, Compare(), alloc) {}
//构造函数
set( std::initializer_list<value_type> init, const Compare& comp = Compare(),
   const Allocator& alloc = Allocator() );
```

##### 1.1.1 自定义比较器 Compare
```cpp
#include <iostream>
#include <string>
#include<list>
#include<set>
#include<math.h>

class IntCmp
{
public:
   bool operator()(const int& t1, const int& t2) const{
      return abs(t1 -10) < abs(t2 -10);
   }
};

int main(int argc, char const* argv[])
{   
   std::list<int> list1 = {9,5,15,23,16,1,48};
   std::set<int, IntCmp> scores(list1.begin(), list1.end());
   //插入
   scores.insert(98);
   scores.insert(scores.begin(), 18);
   //大小 8
   std::cout << scores.size() << "\n";
   //查找
   auto tg = scores.find(16);

   std::cout << *tg << "\n";//16
   for (auto &&element : scores)
   {
      std::cout<<  element << "\n";
   }
   return 0;
}
```


#### [1.2 迭代器](#)
返回的迭代器！

* **s.begin() 和 s.end()** 返回 iterator 类型的迭代器。
* **s.rbegin() 和 s.rend()** 返回 reverse_iterator 类型的迭代器。
* **s.cbegin() 和 s.cend()** 返回 const_iterator 类型的迭代器。
* **s.crbegin() 和 s.crend()** 返回 const_reverse_iterator 类型的迭代器。
* **get_allocator**: 返回相关的分配器。

#### [1.3  student 实验类](#)

```cpp
class Student
{
public:
    Student();
	Student(const std::string& name, const int & age, const bool & sex);
    Student(const Student & user);
    Student(const Student&& user) noexcept;
	~Student();
    Student& operator=(const Student& user);
    bool operator==(const Student& user) noexcept {
        return user._name == _name;
    };
    Student& operator=(const Student&& user) noexcept;
    std::string toString() const;
    std::string get_name() const{
        return _name;
    }
private:
    std::string _name;
    int _age;
    bool _sex;
};


std::string Student::toString() const {
    std::string str = "Name: " + this->_name  + " Age: " 
        + std::to_string(this->_age) + " Sex: " + std::to_string(this->_sex);
    return str;
}

Student::Student() 
:_name(""), _age(0), _sex(true)
{
    printf("默认构造函数 call over \n");
}

Student::Student(const std::string& name, const int& age, const bool& sex)
:_name(name), _age(age), _sex(sex)
{
    printf("全参数构造函数 call over \n");
}

Student::Student(const Student& user) 
:_name(user._name), _age(user._age), _sex(user._sex)
{
    printf("复制构造函数 call over \n");
}

Student::Student(const Student&& user) noexcept
:_name(user._name), _age(user._age), _sex(user._sex)
{
    printf("移动构造函数 call over \n");
}

Student::~Student()
{
    printf("析构函数 call over \n");
}

Student& Student::operator=(const Student& user) {
    if (&user == this) return *this;
    this->_name = user._name;
    this->_age = user._age;
    this->_sex = user._sex;
    return *this;
}

Student& Student::operator=(const Student&& user) noexcept {
    if (&user == this) return *this;
    this->_name = user._name;
    this->_age = user._age;
    this->_sex = user._sex;
    return *this;
}
```

#### [1.4 查找](#)

| **操作**                | **函数**                                                    | **说明**                                                     |
| ----------------------- | ----------------------------------------------------------- | ------------------------------------------------------------ |
| find(key)               | const_iterator find( const Key& key ) const;                | 寻找带有特定键的元素                                         |
| find(key)               | iterator find( const Key& key );                            | 寻找键等于 key 的的元素。                                    |
| count( const Key& key ) | size_type count( const Key& key ) const;                    | 返回匹配特定键的元素数量 , 为 1 或 0。                       |
| contains(key)           | bool contains( const Key& key ) const;                      | (C++ 20)检查容器中是否有键等价于 `key` 的元素。              |
| lower_bound             | iterator lower_bound( const Key& key );                     | 返回指向首个*不小于* `key` 的元素的迭代器。                  |
| upper_bound             | iterator upper_bound( const Key& key );                     | 返回指向首个*大于* `key` 的元素的迭代器。                    |
| equal_range             | std::pair<iterator,iterator> equal_range( const Key& key ); | 返回容器中所有拥有给定关键的元素范围。                       |
| swap                    | void swap( set& other );                                    | 将内容与 `other` 的交换。不在单独的元素上调用任何移动、复制或交换操作。 |
| **std::swap(std::set)** | std::swap(set1, set2)                                       | 特化 std::swap 算法                                          |
| **erase_if(std::map)**  |                                                             | (C++ 20)擦除所有满足特定判别标准的元素(函数模板)             |

**find操作**

```cpp
std::set<Student, SCmp> students;
students.emplace(std::string("Cicer1"), 18, true);
students.emplace(std::string("Cicer2"), 17, true);
students.emplace(std::string("Cicer111"), 19, false);

Student stu(std::string("Cicer1"), 18, true);
std::set<Student, SCmp>::iterator it =  students.find(stu);
if (it == students.end())
{
    printf("not found!\n");
}
else {
    printf("found it!\n");
    std::cout << "found the name is: " << (*it).get_name() << std::endl;
}
```

#### [1.5 容量](#)

| **操作** | **函数**                             | **说明**                                                     |
| -------- | ------------------------------------ | ------------------------------------------------------------ |
| empty    | bool empty() const noexcept;         | 若容器为空则为 true ，否则为 false                           |
| size     | size_type size() const noexcept;     | 容器中的元素数量。返回容器中的元素数，即 `std::distance(begin(), end()) `。 |
| max_size | size_type max_size() const noexcept; | 返回可容纳的最大元素数                                       |

**capacity 操作**

```cpp
template<typename T>
class Cmp
{
public:
    bool operator()(const T& t1, const T& t2) const {
        return abs(t1) < abs(t2);
    }
};

std::set<double, Cmp<double>> scores = { 91.8,50.15,15.12,23.1,16.15,1.5,48.17 };
printf("size: %zd, max_size: %zd\n", scores.size(), scores.max_size());
//size: 7, max_size: 461168601842738790
```



#### [1.6 修改器](#)

| **操作** | **函数**                                                     | **说明**                                                     |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| clear    | void clear() noexcept;                                       | 清除内容                                                     |
| insert   | void insert( std::initializer_list<value_type> ilist );      | 插入来自 initializer_list ilist 的元素                       |
| insert   | template< class InputIt ><br/>void insert( InputIt first, InputIt last ); | 插入来自范围 [first, last) 的元素。                          |
| insert   | std::pair<iterator, bool> insert( value_type&& value );      | 插入 value，返回迭代器，和是否插入成功                       |
| insert   | std::pair<iterator, bool> insert( const value_type& value ); | 插入 value，返回迭代器，和是否插入成功                       |
| emplace  | template< class... Args ><br/>std::pair<iterator,bool> emplace( Args&&... args ); | 原位构造元素(公开成员函数)                                   |
| erase    | iterator erase( iterator pos );                              | 从容器移除指定的元素。                                       |
| erase    | iterator erase( iterator first, iterator last );             | 移除范围 `[first; last)` 中的元素，它必须是 *this 中的合法范围。 |
| merge    | template<class C2><br/>void merge( [std::set](http://zh.cppreference.com/w/cpp/container/set)<Key, C2, Allocator>& source ); | (C++17)从另一容器接合结点                                    |
| extract  | node_type extract( const_iterator position );                | (C++17)从另一容器释出结点                                    |



**insert 操作**

```cpp
template<typename T>
class Cmp
{
public:
    bool operator()(const T& t1, const T& t2) const {
        return abs(t1) < abs(t2);
    }
};

std::set<double, Cmp<double>> scores = { 91.8,50.15,15.12,23.1,16.15,1.5,48.17 };
scores.insert({ 75.15, 14.12, 71.1, 71.1 });

std::for_each(scores.begin(), scores.end(), [](double v) { std::cout << v << " "; });
//1.5 14.12 15.12 16.15 23.1 48.17 50.15 71.1 75.15 91.8
```

**emplace 操作**

```cpp
class SCmp {
public:
    bool operator()(const Student& t1, const Student& t2) const {
        return t1.get_name() > t2.get_name();
    }
};

std::set<Student, SCmp> students;
students.emplace(std::string("jxkicker"), 18, true);
students.emplace(std::string("miker"), 17, true);
students.emplace(std::string("cocer"), 19, false);

std::for_each(students.begin(), students.end(), [](const Student& v) { std::cout << v.toString() << "\n"; });
```




### [2. multiset](#) 
**std::multiset** 是含有 Key 类型对象有序集的容器。与**set**不同，它允许多个**Key** 拥有等价的值。用关键比较函数**Compare** 进行排序。搜索、插入和移除操作拥有对数复杂度。

```cpp
template<
    class Key,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<Key>
> class multiset;
```



#### [2.1 构造函数](#)

构造函数和set差不多，看看就懂啦。

```cpp
//无参构造函数
multiset();
//复制构造函数
multiset( const multiset& other );
//移动构造函数
multiset( multiset&& other );
//禁止隐私转换的构造函数
explicit multiset( const Compare& comp, const Allocator& alloc = Allocator() );
//列表初始化构造函数
multiset( std::initializer_list<value_type> init, const Allocator& );
//使用其他序列来初始化的构造函数
template< class InputIt >
multiset( InputIt first, InputIt last, const Compare& comp = Compare(),
          const Allocator& alloc = Allocator() );
//使用其他序列来初始化的构造函数
template< class InputIt >
multiset( InputIt first, InputIt last, const Allocator& alloc );
```



**列表初始化:**

```cpp
std::multiset<int> scores = {1,2,3,4,5,2,3,4,5,6,5,6,7,2 };
int count = scores.count(2);
std::cout << "count: " << count << "\n";
//count:3
```

**序列初始化**

```cpp
std::list<int> numbers = { 1,2,3,4,5,2,3,4,5,6,5,6,7,2 };
std::multiset<int> scores (numbers.begin(), numbers.end());
int count = scores.count(2);
std::cout << "count: " << count << "\n";
//count:3
```

#### [2.1 一些操作](#)

| **操作** | **函数**                                                     | **说明**                                                     |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| erase    | iterator erase( iterator pos );                              | 移除位于 pos 的元素。                                        |
| erase    | iterator erase( const_iterator pos );                        | 如果 `iterator` 与 `const_iterator` 的类型相同就只会提供一个重载。 |
| erase    | iterator erase( iterator first, iterator last );             | 移除范围 `[first; last)` 中的元素，它必须是 *this 中的合法范围。 |
| erase    | iterator erase( const_iterator first, const_iterator last ); | 移除范围 `[first; last)` 中的元素，它必须是 *this 中的合法范围。 |
| erase    | size_type erase( const key_type& key );                      | 移除键比较*等价*于值 x 的所有元素。                          |
| erase    | template< class K ><br/>size_type erase( K&& x );            | 移除键比较*等价*于值 x 的所有元素。                          |

**size() 操作**
```cpp
#include<iostream>
#include<string>
#include<algorithm>
#include<set>
#include<math.h>

int main(int argc, char const* argv[])
{   
   std::multiset<std::string> names = {"lime", "remix", "coking","kicer", "dime", "coking"};
   std::cout<< names.size() << "\n"; //6
   std::for_each(names.begin(), names.end(), [](std::string t) {
      std::cout<< t<<"\n";
   });
   return 0;
}
```

**erase() 操作**
```cpp
std::list<int> numbers = { 1,2,3,4,5,2,3,4,5,6,5,6,7,2 };
std::multiset<int> scores (numbers.begin(), numbers.end());

scores.erase(2); //删除值为 2 的元素

std::for_each(scores.begin(), scores.end(), [idx = 0](auto v) mutable {
std::cout << "[" << idx++ << "] = " << v << "\n";
});
```



### [3. set algorithm](#) 
定义于头文件 <algorithm>

* **set_union** ：计算两个集合的并集
* **set_intersection**： 计算两个集合的交集
* **set_difference**： 计算两个集合的差集
* **includes**： 若一个序列是另一个的子列则返回 true
* **set_symmeric_difference**： 计算两个集合的对称差

```cpp
#include <iostream>
#include <string>
#include<algorithm>
#include<set>
#include<math.h>

int main(int argc, char const* argv[])
{   
   std::set<int> voters_b1 = {12,15,98,56,47,87,89,45};
   std::set<int> voters_b2 = {22,15,98,56,49,87,89,41};
   std::set<int> voters;
    //计算两个集合的并集
   std::set_union(voters_b1.begin(),voters_b1.end(),
                      voters_b2.begin(),voters_b2.end(), 
                      std::insert_iterator<std::set<int>>(voters, voters.begin()));

   std::cout <<"total:"<< voters.size() <<"\n";
   std::for_each(voters.begin(), voters.end(), [](int t) {
      std::cout<< t<<"\n";
   });
   return 0;
}
```

### [4. pair 类型](#)
在标头 `<utility>` 定义， 这是map类型存储的值类型。

```cpp
template<
    class T1,
    class T2
> struct pair;
//T1, T2	-	pair 所存储的元素类型。
```

#### [4.1 构造函数](#)
构造函数只需要记住几个经典的就可以了。

```cpp
pair();
constexpr pair();
pair( const T1& x, const T2& y );
pair( const T1& x, const T2& y );
constexpr pair( const T1& x, const T2& y );
template< class U1, class U2 >
pair( U1&& x, U2&& y );
```

**使用例子**

```cpp
std::list<std::pair<std::string, int>> numbers = {
    { std::string("remix"), 95},
    { std::string("cicer"), 90},
    { std::string("nike"), 97}
};
std::map<std::string, int> scores (numbers.begin(), numbers.end());

std::for_each(scores.begin(), scores.end(), [](const std::pair<std::string, int> &v) mutable {
    std::cout << "[" << v.first << "] = " << v.second << "\n";
});
/*
[cicer] = 90
[nike] = 97
[remix] = 95
*/
```

#### [4.2 非成员函数](#)

| **操作**                           | **函数**                                                     | **说明**                                                     |
| ---------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **std::make_pair**                 | template< class T1, class T2 ><br/>[std::pair](http://zh.cppreference.com/w/cpp/utility/pair)<T1, T2> make_pair( T1 t, T2 u ); | 构造 [std::pair](https://zh.cppreference.com/w/cpp/utility/pair) 对象，从参数类型推导目标类型。 |
| **std::swap(std::pair,std::pair)** |                                                              |                                                              |
| **std::get()**                     |                                                              | 访问 `pair` 的其中一个元素                                   |

**make_pair 操作**

```cpp
int n = 1;
int a[5] = {1, 2, 3, 4, 5};
auto p1 = std::make_pair(n, a[1]);
std::cout << "p1 的值是 "
    << "(" << p1.first << ", " << p1.second << ")\n";
/*
p1 的值是 (1, 2)
*/

std::list<std::pair<std::string, int>> numbers = {
    { std::string("remix"), 95},
    { std::string("cicer"), 90},
    { std::string("nike"), 97}
};

auto kv = std::make_pair<std::string, int>("coke", 91);
numbers.push_back(kv);
std::for_each(numbers.begin(), numbers.end(), [](const std::pair<std::string, int> &v) mutable {
    std::cout << "[" << v.first << "] = " << v.second << "\n";
});
```

**get操作**

```cpp
#include <iostream>
#include <utility>
 
int main()
{
    auto p = std::make_pair(1, 3.14);
    std::cout << '(' << std::get<0>(p) << ", " << std::get<1>(p) << ")\n";
    std::cout << '(' << std::get<int>(p) << ", " << std::get<double>(p) << ")\n";
}
```



### [5. map 字典](#) 
std::map 是有序键值对容器，它的元素的键是唯一的。用比较函数 Compare 排序键。搜索、移除和插入操作拥有对数复杂度。map 通常实现为红黑树!

存储的值的类型为：**std::pair<const Key, T>**

```cpp
template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class map;
```
标准库使用比较 (Compare) 概念时用等价关系确定唯一性。不精确地说，如果两个对象 a 与 b 相互不比较小于对方：!comp(a, b) && !comp(b, a)，那么认为它们等价。

**部分内置类型：**

| 成员类型               | 字段                                      |
| ---------------------- | ----------------------------------------- |
| key_type               | Key                                       |
| mapped_type            | value类型                                 |
| value_type             | **std::pair<const Key, T>**               |
| size_type              | 无符号整数类型                            |
| difference_type        | 有符号整数类型                            |
| const_iterator         | 指向 const value_type 的老式双向迭代器    |
| iterator               | 指向 value_type 的常老式双向迭代器        |
| reverse_iterator       | **std::reverse_iterator<iterator>**       |
| const_reverse_iterator | **std::reverse_iterator<const_iterator>** |

```python
using std::map;
using std::string;
map<string, double> grade;
map<string, double>::key_type name("remix");
map<string, double>::mapped_type score{ 95.55 };

grade.emplace(name, score);
name = "cicer";
score = 96.054;
grade.emplace(name, score);
for (std::pair<string, double> &&v : grade) {
    std::cout << v.first << ": " << v.second << "\n";
}
```

#### [5.1 构造函数](#)
关键的参数
* **alloc**:  用于此容器所有内存分配的分配器
* **comp**:用于所有关键比较的比较函数对象
* **first, last**:  复制元素的来源范围
* **other**:将用作初始化容器元素所用源的另一容器
* **init**: 初始化容器元素所用的 initializer_list

需要关注的构造函数！

```cpp
template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class map;
//无参构造函数
map(); 
//复制构造函数
map( const map& other );
//列表
template< class InputIt >
map( InputIt first, InputIt last,const Compare& comp = Compare(),const Allocator& alloc = Allocator() );
map( std::initializer_list<value_type> init,const Allocator& );
```
构造函数和操作与set差不多！
```cpp
// (1) 默认构造函数
std::map<std::string, int> map1;
map1["something"] = 69;
map1["anything"] = 199;
map1["that thing"] = 50;
std::cout << "map1 = "; print_map(map1);

// (2) 范围构造函数
std::map<std::string, int> iter(map1.find("anything"), map1.end());
std::cout << "\niter = "; print_map(iter);
std::cout << "map1 = "; print_map(map1);

// (3) 复制构造函数
std::map<std::string, int> copied(map1);
std::cout << "\ncopied = "; print_map(copied);
std::cout << "map1 = "; print_map(map1);

// (4) 移动构造函数
std::map<std::string, int> moved(std::move(map1));
std::cout << "\nmoved = "; print_map(moved);
std::cout << "map1 = "; print_map(map1);

// (5) initializer_list 构造函数
const std::map<std::string, int> init {
   {"this", 100},
   {"can", 100},
   {"be", 100},
   {"const", 100},
};
std::cout << "\ninit = "; print_map(init);
```

#### [5.2 初始化](#)
可以使用大括号的方式初始化
```cpp
struct Point { double x, y; };
struct PointCmp {
    bool operator()(const Point& lhs, const Point& rhs) const { 
        return lhs.x < rhs.x; // NB 。有意忽略 y
    }
};
std::map<Point, double, PointCmp> mag = {
   { {5, -12}, 13 },
   { {3, 4},   5 },
   { {-8, -15}, 17 }
};
```

#### [5.3 迭代器](#)
返回的迭代器！

* **s.begin() 和 s.end()** 返回 iterator 类型的迭代器。
* **s.rbegin() 和 s.rend()** 返回 reverse_iterator 类型的迭代器。
* **s.cbegin() 和 s.cend()** 返回 const_iterator 类型的迭代器。
* **s.crbegin() 和 s.crend()** 返回 const_reverse_iterator 类型的迭代器。
* **get_allocator**: 返回相关的分配器。


#### [5.4 元素访问](#)

| **操作**   | **函数**                             | **说明**                                                     |
| ---------- | ------------------------------------ | ------------------------------------------------------------ |
| at         | const T& at( const Key& key ) const; | 访问指定的元素，同时进行越界检查                             |
| at         | T& at( const Key& key );             | 若无这种元素，则抛出 std::out_of_range 类型异常。            |
| operator[] | T& operator[]( const Key& key );     | 返回到映射到等于 `key` 的键的值的引用，若这种键不存在则进行插入。 |
| operator[] | T& operator[]( Key&& key );          | 返回到映射到等于 `key` 的键的值的引用，若这种键不存在则进行插入。 |

**operator 操作**
```cpp
std::map<char, int> letter_counts {{'a', 27}, {'b', 3}, {'c', 1}};
std::cout << "initially:\n";
for (const auto &pair : letter_counts) {
    std::cout << pair.first << ": " << pair.second << '\n';
}
letter_counts['b'] = 42;  // 更新既存值
letter_counts['x'] = 9;  // 插入新值
```

**at**
```cpp
std::map<std::string, double> scores{ 
    {std::string("remix"), 95.15},
    {std::string("umix"), 87.5},
    {std::string("nike"), 59.3},
    {std::string("cicer"), 48.6},
    {std::string("Quake"), 79.7}
};
std::cout <<"cicer 分数为: " << scores.at(std::string("cicer")) << std::endl; 
//cicer 分数为: 48.6
```

**统计次数：**
```cpp
std::list<std::string> names { "remix", "cicier", "remix", "unix", "mike", "unix", "mike", "coc", "remix"};
std::map<std::string, double> scores;

for (const auto& name : names)
{
    ++scores[name];
}

std::for_each(scores.begin(), scores.end(), [](const std::pair<std::string, double>& c) {
    std::cout << "[" << c.first << "] occur time: " << c.second << std::endl;
});
/*
[cicier] occur time: 1
[coc] occur time: 1
[mike] occur time: 2
[remix] occur time: 3
[unix] occur time: 2
*/
```

#### [5.5 容量](#)
非常简单啊，看看就懂，就这几个函数呢！

| **操作** | **函数**                             | **说明**                                                     |
| -------- | ------------------------------------ | ------------------------------------------------------------ |
| empty    | bool empty() const noexcept;         | 检查容器是否无元素，即是否 begin() == end() 。               |
| size     | size_type size() const noexcept;     | 容器中的元素数量。返回容器中的元素数，即 `std::distance(begin(), end()) `。 |
| max_size | size_type max_size() const noexcept; | 返回可容纳的最大元素数                                       |


#### [5.6 修改器](#)
| **操作**             | **函数**                                                     | **说明**                                                     |
| -------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| clear                | void clear() noexcept;                                       | 清除内容                                                     |
| insert               | void insert( std::initializer_list<value_type> ilist );      | 插入来自 initializer_list ilist 的元素                       |
| insert               | template< class InputIt ><br/>void insert( InputIt first, InputIt last ); | 插入来自范围 [first, last) 的元素。                          |
| insert               | [std::pair](http://zh.cppreference.com/w/cpp/utility/pair)<iterator, bool> insert( const value_type& value ); | 插入 （key ,value），返回迭代器，和是否插入成功              |
| insert               | std::pair<iterator, bool> insert( const value_type& value ); | 插入 value，返回迭代器，和是否插入成功                       |
| **insert_or_assign** | template <class M><br/>[std::pair](http://zh.cppreference.com/w/cpp/utility/pair)<iterator, bool> insert_or_assign(const key_type& k, M&& obj); | 若等价于 `k` 的键已存在于容器中，则赋值 ,键不存在，则如同用 insert 插入 |
| emplace              | template< class... Args ><br/>std::pair<iterator,bool> emplace( Args&&... args ); | 原位构造元素(公开成员函数)                                   |
| erase                | iterator erase( iterator pos );                              | 从容器移除指定的元素。返回被移除的元素个数                   |
| erase                | size_type erase( const key_type& key );                      | 返回被移除的元素个数                                         |
| erase                | iterator erase( iterator first, iterator last );             | 移除范围 `[first; last)` 中的元素，它必须是 *this 中的合法范围。返回被移除的元素个数。 |
| erase                | template< class K ><br/>size_type erase( K&& x );            | 返回被移除的元素个数。                                       |
| merge                | template<class C2><br/>void merge( [std::set](http://zh.cppreference.com/w/cpp/container/set)<Key, C2, Allocator>& source ); | (C++17)从另一容器接合结点                                    |
| extract              | node_type extract( const_iterator position );                | (C++17)从另一容器释出结点                                    |

**insert_or_assign 操作**

```cpp
std::map<std::string, double> scores{ 
    {std::string("remix"), 95.15},
    {std::string("umix"), 87.5},
    {std::string("nike"), 59.3}
};
scores.insert_or_assign(std::string("mike"), 98.56); //插入
scores.insert_or_assign(std::string("mike"), 97.56); //赋值

std::for_each(scores.begin(), scores.end(), [](const std::pair<std::string, double>& c) {
    std::cout << c.first << " 分数为: " << c.second << std::endl;
});
/*
mike 分数为: 97.56
nike 分数为: 59.3
remix 分数为: 95.15
umix 分数为: 87.5
*/
```

**insert**

```cpp
scores.insert(std::pair<std::string, double>(std::string("mike"), 98.56));
```

**emplace**

```cpp
std::map<std::string, std::string> m;
// 使用 pair 的移动构造函数
m.emplace(std::make_pair(std::string("a"), std::string("a")));
// 使用 pair 的转换移动构造函数
m.emplace(std::make_pair("b", "abcd"));
// 使用 pair 的模板构造函数
m.emplace("d", "ddd");
// 使用 pair 的逐片构造函数
m.emplace(std::piecewise_construct,
          std::forward_as_tuple("c"),
          std::forward_as_tuple(10, 'c'));
// C++17 起，能使用 m.try_emplace("c", 10, 'c');
for (const auto &p : m) {
    std::cout << p.first << " => " << p.second << '\n';
}
```

#### [5.7 遍历](#)

map的迭代器有两个值，first，second！

```cpp
std::map<std::string, int> dictionary = {
   {"xiaohu", 1},
   {"ming", 1},
   {"wei", 1},
   {"gala", 1},
   {"breath", 1}
};

for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
   std::cout << it->first << ":" << it->second << '\n';
}
```

```cpp
std::map<std::string, double> scores{ 
    {std::string("remix"), 95.15},
    {std::string("umix"), 87.5},
    {std::string("nike"), 59.3},
    {std::string("cicer"), 48.6},
    {std::string("Quake"), 79.7}
};

std::for_each(scores.begin(), scores.end(), [](const std::pair<std::string, double>& c) {
    std::cout << c.first << " 分数为: " << c.second << std::endl;
});
```

### [6. multimap 可重复字典](#) 
**std::multimap**是关联容器，含有键值对的已排序列表，同时容许多个元素拥有同一键。按照应用到键的比较函数 Compare 排序。搜索、插入和移除操作拥有对数复杂度。当向里面插入元素的时候，总是插入一个新的key。

```cpp
template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class multimap;
```

#### [6.1 构造函数](#)
关键的参数

* **alloc**:  用于此容器所有内存分配的分配器
* **comp**:用于所有关键比较的比较函数对象
* **first, last**:  复制元素的来源范围
* **other**:将用作初始化容器元素所用源的另一容器
* **init**: 初始化容器元素所用的 initializer_list

```cpp
//无参参数
multimap();
//复制构造函数
multimap( const multimap& other );
//移动构造函数
multimap( multimap&& other );
multimap( multimap&& other, const Allocator& alloc );
//有参无法隐式转换构造函数
explicit multimap( const Compare& comp,const Allocator& alloc = Allocator() );
//你懂的
explicit multimap( const Allocator& alloc );
//列表初始化
template< class InputIt >
multimap( InputIt first, InputIt last, const Compare& comp = Compare(),
          const Allocator& alloc = Allocator() );
//列表初始化
multimap( std::initializer_list<value_type> init,const Compare& comp = Compare(),
          const Allocator& alloc = Allocator() );
```


#### [6.2 操作](#)
* **insert_or_assign** 插入元素，或若键已存在则赋值给当前元素
* **emplace**: 原位构造元素
* **emplace_hint (C++20)**: 使用提示原位构造元素



#### 6.3 insert

**multimap** 的**insert** 操作每次都只是增加一个新 pair, 其本质不应该是 map，而是**list< std::pair<T1,T2>>**。

```cpp
using std::multimap;
using std::string;
multimap<string, double> grade;
multimap<string, double>::key_type name("remix");
multimap<string, double>::mapped_type score{ 95.55 };

grade.emplace(name, score);
name = "cicer";
score = 96.054;
grade.emplace(name, score);
grade.insert(std::make_pair<std::string, double>("cicer", 97.054));
grade.insert({"cicer", 94.56});

for (std::pair<string, double> &&v : grade) {
    std::cout << v.first << ": " << v.second << "\n";
}
/*
cicer: 96.054
cicer: 97.054
cicer: 94.56
remix: 95.55
*/
```



-----
时间: [ 2023年1月5日16:24:25 ]

