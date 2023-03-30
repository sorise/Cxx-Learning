### [C++ 无序关联容器](#)
无序关联容器提供能快速查找（均摊` O(1)` ，最坏情况 `O(n)` 的复杂度）的无序（哈希）数据结构。

-----
- [x] [1. 无序容器概述](#1-无序容器概述)
- [x] [2. hash集合](#2-hash集合)
- [x] [3. unordered_multiset](#3-unordered_multiset)
- [x] [4. hash 字典](#4-hash-字典)
- [x] [5. unordered_multimap](#4-unordered-multimap)

-----

### [1. 无序容器概述](#)

新标准定义了四个无序关联容器，它们**并不使用比较运算符**来组织运算符，而是使用一个**Hash函数** 和 **关键字类型的==运算符**。对于hash函数最终使用的应该是数据结构中的**拉链法**。

**使用例子：**

```cpp
using Key = std::string;
using Value = Student;
using Map = std::unordered_map<Key, Value>;

Map dics = {
    {"20212209016", {std::string("Cicer1"), 18, true}},
    {"20212209017", {std::string("Cicer2"), 17, true}},
    {"20212209018", {std::string("Cicer3"), 19, true}}
};

for (const std::pair<Key, Value> kv : dics )
{
    std::cout << "{key: " << kv.first << ", Value: {" << kv.second.toString() << "}}\n";
}
```

#### [1.1 Hash函数管理桶](#)

无序容器在存储上组织为一组桶，每个桶保存`0`或者`多个`元素。无序容器使用一个hash函数将元素映射到桶。**容器首先计算元素的hash函数**，找到它对应的桶，然后将具有一个特定hash值的所有元素都保存在相同的桶中。

**桶的容量和多少**

| 桶接口                              | 函数接口                                    | 说明                                             |
| ----------------------------------- | ------------------------------------------- | ------------------------------------------------ |
| bucket_count()                      | size_type bucket_count() `const`            | 返回桶数                                         |
| max_bucket_count()                  | size_type max_bucket_count() const          | 返回容器由于系统或库实现限制的能保有的最大桶数。 |
| bucket_size(size_type )             | size_type bucket_size( size_type n ) const; | 返回下标为 `n` 的桶中的元素数。                  |
| size_type  bucket( const Key& key ) | size_type bucket( const Key& key ) const;   | 返回关键 `key` 的桶的下标。                      |

```cpp
using Key = std::string;
using Value = Student;

std::unordered_map<Key, Value> dics = {
    {"20212209016", {std::string("Cicer1"), 18, true}},
    {"20212209017", {std::string("Cicer2"), 17, true}},
    {"20212209018", {std::string("Cicer3"), 19, true}},
    {"20212209019", {std::string("Cicer4"), 19, true}},
    {"20212209020", {std::string("Cicer5"), 19, true}},
    {"20212209021", {std::string("Cicer6"), 19, true}},
    {"20212209022", {std::string("Cicer7"), 19, true}},
    {"20212209023", {std::string("Cicer8"), 19, true}},
};

std::cout << "bucket_count: " << dics.bucket_count() << "\n";
std::cout << "max_bucket_count: " << dics.max_bucket_count() << "\n";
std::cout << "20212209017 bucket count: " << dics.bucket_size(dics.bucket(std::string("20212209017"))) << "\n";
//bucket_count: 8
//max_bucket_count: 1152921504606846975
//20212209017 bucket count: 2
```

**桶的迭代接口**

| 桶迭代               | 接口                                              | 说明                                                         |
| -------------------- | ------------------------------------------------- | ------------------------------------------------------------ |
| local_iterator       | 静态类型-非函数                                   | 类别、值、差、指针和引用类型都与 `iterator` 相同的迭代器类型。<br/>能用此迭代器在单个桶迭代，但不能跨桶。 |
| const_local_iterator | 静态类型-非函数                                   | 类别、值、差、指针和引用类型都与 `const_iterator` 相同的迭代器类型。<br/>能用此迭代器在单个桶迭代，但不能跨桶。 |
| begin(n)             | const_local_iterator begin( size_type n ) const;  | 指向某个桶的首元素的迭代器。                                 |
| cbegin(n)            | const_local_iterator cbegin( size_type n ) const; | 指向某个桶的首元素的迭代器。                                 |
| end(n)               | const_local_iterator end( size_type n ) const;    | 指向某个桶的首元素的迭代器。                                 |
| cend(n)              | const_local_iterator cend( size_type n ) const;   | 指向某个桶的首元素的迭代器。                                 |

```cpp
using Key = std::string;
using Value = Student;

std::unordered_map<Key, Value> dics = {
    {"20212209016", {std::string("Cicer1"), 18, true}},
    {"20212209017", {std::string("Cicer2"), 17, true}},
    {"20212209018", {std::string("Cicer3"), 19, true}},
    {"20212209019", {std::string("Cicer4"), 19, true}},
    {"20212209020", {std::string("Cicer5"), 19, true}},
    {"20212209021", {std::string("Cicer6"), 19, true}},
    {"20212209022", {std::string("Cicer7"), 19, true}},
    {"20212209023", {std::string("Cicer8"), 19, true}},
};

std::unordered_map<Key, Value>::size_type buk = dics.bucket(std::string("20212209017"));

std::for_each(dics.begin(buk), dics.end(buk), [](const std::pair<Key, Value> v){
    std::cout  << v.first <<":" << v.second.get_name() << "\n";
});
//20212209022:Cicer7
//20212209017:Cicer2
```

**哈希策略接口**

| 哈希策略        | 函数                              | 说明                                                         |
| --------------- | --------------------------------- | ------------------------------------------------------------ |
| load_factor     | float load_factor() const;        | 返回每个桶的平均元素数量                                     |
| max_load_factor | float max_load_factor() const;    | 返回最大加载因子。                                           |
| max_load_factor | void max_load_factor( float ml ); | 设置最大加载因子为 `ml` ，管理每个桶的平均元素数量的最大值。 |
| rehash          | void rehash( size_type count );   | 设置桶数为 `count` 并重置哈希容器，即考虑桶总数已改变，再把元素放到适当的桶中。若新的桶数使加载因子大于最大加载因子`（ count < size() / max_load_factor() ）`，则新桶数至少为` size() / max_load_factor() ` |
| reserve         | void reserve( size_type count );  | 设置桶数为适应至少 `count` 个元素，而不超出最大加载因子所需的数，并重哈希容器，即考虑桶数已更改后将元素放进适合的桶。等效地调用 `rehash(std::ceil(count / max_load_factor()))` |

```cpp
std::cout << "load_factor: " << dics.load_factor() << "\n";
std::cout << "max_load_factor: " << dics.max_load_factor() << "\n";
//load_factor: 1
//max_load_factor: 1
```

#### [1.2 观察器](#)

| 名称          | 函数                          | 说明                      |
| ------------- | ----------------------------- | ------------------------- |
| hash_function | hasher hash_function() const; | 返回用于对键散列的函数    |
| key_eq        | key_equal key_eq() const;     | 返回比较key相等性的函数。 |

####  [1.3 无序容器对关键字类型的要求](#)

默认情况下，无序容器使用**关键字类型的==**运算符来比较元素，它们还使用一个**hash<key_type>**类型的对象来生成每个元素的**哈希值**。

标准库为内置类型（包括指针)提供了**hash模板**。还为一些标准库类型，包括string等等。因此，我们可以**直接定义**关键字是内置类型（包括指针类型)、string还是智能指针类型的无序容器。

```cpp
template<> struct hash<bool>;
template<> struct hash<char>;
template<> struct hash<signed char>;
template<> struct hash<unsigned char>;
template<> struct hash<char8_t>;        // C++20
template<> struct hash<char16_t>;
template<> struct hash<char32_t>;
template<> struct hash<wchar_t>;
template<> struct hash<short>;
template<> struct hash<unsigned short>;
template<> struct hash<int>;
template<> struct hash<unsigned int>;
template<> struct hash<long>;
template<> struct hash<long long>;
template<> struct hash<unsigned long>;
template<> struct hash<unsigned long long>;
template<> struct hash<float>;
template<> struct hash<double>;
template<> struct hash<long double>;
template<> struct hash<std::nullptr_t>;
template< class T > struct hash<T*>;
std::hash<std::bitset>
std::hash<std::string>
std::hash<std::u8string>
std::hash<std::u16string>
std::hash<std::u32string>
std::hash<std::wstring>
std::hash<std::pmr::string>
std::hash<std::pmr::u8string>
std::hash<std::pmr::u16string>
std::hash<std::pmr::u32string>
std::hash<std::pmr::wstring>
std::hash<std::string_view>
std::hash<std::wstring_view>
std::hash<std::u8string_view>
std::hash<std::u16string_view>
std::hash<std::u32string_view>
std::hash<std::vector<bool>>
std::hash<std::filesystem::path>
std::hash<std::thread::id>
std::hash<std::shared_ptr>  
//......
```

但是，我们不能直接定义关键字类型为**自定义类类型**的无序容器。与容器不同，不能直接使用哈希模板，而必须提供我们自己的**hash模板**版本。

```cpp
//自定义hash函数
size_t hasher(const Student& student) {
    return std::hash<std::string>()(student.get_name());
}
//自定义 比较
bool equal(const Student& s1, const Student& s2) {
    return s1.get_name() == s2.get_name();
}

std::unordered_set<Student, decltype(hasher)*, decltype(equal)*> collections(10, hasher, equal);
collections.insert(Student("cicer", 18, true));
collections.insert(Student("remix", 17, true));
collections.insert(Student("king", 20, false));

for (const Student& s1 : collections) {
    std::cout << "{ " << s1.toString() << " }\n";
}
```

**hash函数实际使用例子：**

```cpp
std::string str { "0000000" };
std::size_t rel = std::hash<std::string>()(str);

std::cout << "hash: " << rel << "\n";
//hash: 17051979661405316375
```

### [2. hash集合](#)

**unordered_set **是含有 **Key** 类型唯一对象集合的关联容器。搜索、插入和移除拥有平均常数时间复杂度。在内部，元素并不以任何特别顺序排序，而是组织进桶中。元素被放进哪个桶完全依赖其值的哈希。这允许对单独元素的快速访问，因为哈希一旦确定，就准确指代元素被放入的桶。

```cpp
template<
    class Key,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator<Key>
> class unordered_set;
```

**部分内置类型：（set 类型的key_type和value_type是一样的，map不是）**

| 成员类型               | 字段                                   |
| ---------------------- | -------------------------------------- |
| key_type               | Key                                    |
| value_type             | Key                                    |
| size_type              | 无符号整数类型                         |
| difference_type        | 有符号整数类型                         |
| hasher                 | Hash                                   |
| const_iterator         | 指向 const value_type 的老式双向迭代器 |
| iterator               | 指向 value_type 的常老式双向迭代器     |
| reverse_iterator       | std::reverse_iterator<iterator>        |
| const_reverse_iterator | std::reverse_iterator<const_iterator>  |



#### [2.1 内置函数](#)

有许多的构造函数，相比于 `set`它增加了几个关于`桶`的构造函数。

```cpp
unordered_set() : //无参函数
unordered_set( const unordered_set& other ); //复制构造函数
unordered_set( unordered_set&& other ); //移动构造函数
//桶 + 哈希函数指针 + 分配器 
unordered_set( size_type bucket_count,
               const Hash& hash,
               const Allocator& alloc )
              : unordered_set(bucket_count, hash, key_equal(), alloc) {}
//列表构造函数
template< class InputIt >
unordered_set( InputIt first, InputIt last,
               size_type bucket_count = /*implementation-defined*/,
               const Hash& hash = Hash(),
               const key_equal& equal = key_equal(),
               const Allocator& alloc = Allocator() );
//列表 + 桶 + ...
template< class InputIt >
unordered_set( InputIt first, InputIt last,
               size_type bucket_count,
               const Hash& hash,
               const Allocator& alloc )
              : unordered_set(first, last,
                  bucket_count, hash, key_equal(), alloc) {}
```

**使用一个列表+桶的构造函数：**

```cpp
std::list<Student> students{
    {std::string("Cicer1"), 18, true},
    {std::string("Cicer2"), 17, true},
    {std::string("Cicer3"), 19, true},
    {std::string("Cicer4"), 19, true},
    {std::string("Cicer5"), 19, true},
    {std::string("Cicer6"), 19, true},
    {std::string("Cicer7"), 19, true},
    {std::string("Cicer8"), 19, true}
};
std::unordered_set<Student, decltype(hasher)*, decltype(equal)*> collections(
    students.begin(), students.end(), 10, hasher, equal);
collections.insert(Student("cicer", 18, true));
collections.insert(Student("remix", 17, true));
collections.insert(Student("king", 20, false));

for (const Student& s1 : collections) {
    std::cout << "{ " << s1.toString() << " }\n";
}
```

#### [2.2 方法函数](#)

hash集合的方法接口和二叉树集合的方法接口都差不多，只是内部实现有所不同。

| 函数    | 函数声明                                      | 说明                                                         |
| ------- | --------------------------------------------- | ------------------------------------------------------------ |
| find    | iterator find( const Key& key );              | 寻找带有特定键的元素                                         |
| extract | node_type extract( const key_type& k );       | 解链含 position 所指向元素的结点并返回占有它的结点柄。`node_type ` |
| extract | node_type extract( const_iterator position ); | 解链含 position 所指向元素的结点并返回占有它的结点柄。`node_type ` |

**查找算法：find**

```cpp
auto v = collections.find(Student(std::string("Cicer4"), 19, true));
if (v !=  collections.end())
{
    std::cout << v->toString() << "\n";
}
```

**释放查找：extract**

```cpp
int i_array[] = { 48,75,89,88,54,49,45 };
std::unordered_set<int> numbers(std::begin(i_array), std::end(i_array));
// 释出结点柄并更改键
int key = 48;
auto nh = numbers.extract(key);
if (!nh.empty())
{
    nh.value() = 199;
    std::for_each(numbers.begin(), numbers.end(), [](const int& v) {
        std::cout << v << " ";
    });
    //88 75 49 89 54 45
    // 往回插入结点柄
    std::cout << std::endl;
    numbers.insert(std::move(nh));
    std::for_each(numbers.begin(), numbers.end(), [](const int& v) {
        std::cout << v << " ";
    });
    //88 75 49 89 54 45 199
}
else {
    std::cout << "did'nt have the key: " << key << std::endl;
}
```



### [3. unordered_multiset](#) 

```cpp
template<
    class Key,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator<Key>
> class unordered_multiset;
```

### [4. hash 字典](#)

unordered_map 是关联容器，含有带唯一键的键-值 pair 。搜索、插入和元素移除拥有平均常数时间复杂度。元素在内部不以任何特定顺序排序，而是组织进桶中。元素放进哪个桶完全依赖于其键的哈希。这允许对单独元素的快速访问，因为一旦计算哈希，则它准确指代元素所放进的桶。

```cpp
template<
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator< std::pair<const Key, T> >
> class unordered_map;
```

**成员类型**

| 成员类型               | 字段                                   |
| ---------------------- | -------------------------------------- |
| key_type               | Key                                    |
| value_type             | Key                                    |
| size_type              | 无符号整数类型                         |
| difference_type        | 有符号整数类型                         |
| hasher                 | Hash                                   |
| const_iterator         | 指向 const value_type 的老式双向迭代器 |
| iterator               | 指向 value_type 的常老式双向迭代器     |
| reverse_iterator       | std::reverse_iterator<iterator>        |
| const_reverse_iterator | std::reverse_iterator<const_iterator>  |
| node_type              | 表示容器结点的结点把柄特化             |

#### [4.1 构造函数](#)

```cpp
//无参构造函数
unordered_map() : unordered_map( size_type(/*implementation-defined*/) ) {}
unordered_map( const unordered_map& other );
unordered_map( unordered_map&& other );
//桶 构造函数
explicit unordered_map( size_type bucket_count,
                        const Hash& hash = Hash(),
                        const key_equal& equal = key_equal(),
                        const Allocator& alloc = Allocator() );
//桶 构造函数
unordered_map( size_type bucket_count,
               const Allocator& alloc )
              : unordered_map(bucket_count, Hash(), key_equal(), alloc) {}
//序列容器初始化
template< class InputIt >
unordered_map( InputIt first, InputIt last,
               size_type bucket_count,
               const Hash& hash,
               const Allocator& alloc )
              : unordered_map(first, last,
                  bucket_count, hash, key_equal(), alloc) {}
//列表初始化构造函数
unordered_map( std::initializer_list<value_type> init,
               size_type bucket_count,
               const Hash& hash,
               const Allocator& alloc )
              : unordered_map(init, bucket_count,
                  hash, key_equal(), alloc) {}
```

**使用构造函数：**

```cpp
std::unordered_map<std::string, Student> dictionary(20);
dictionary["remix"] = Student("Remix", 18, true);
dictionary.insert(std::make_pair<std::string, Student>("unix", Student("Unix", 18, true)));
dictionary.insert(std::make_pair<std::string, Student>("kicker", Student("Kicker", 18, true)));
dictionary.emplace("linee",Student("Linee", 17, false));

for (const auto& pair : dictionary) {
    printf("{ %s }\n", pair.second.toString().c_str());
}
/*
{ Name: Remix ,Age: 18,Sex: 1 }
{ Name: Unix ,Age: 18,Sex: 1 }
{ Name: Kicker ,Age: 18,Sex: 1 }
{ Name: Linee ,Age: 17,Sex: 0 }
*/
```

#### [4.2 函数方法](#)

hash字典的方法接口和二叉树字典的方法接口都差不多，只是内部实现有所不同。

| 函数名           | 函数声明                                                     | 说明                                                         |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| insert_or_assign | template <class M><br/>std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj); | 插入或者修改字典元素，若插入发生则 bool 组分为 true ，若赋值发生则 bool 组分为 false 。 |
| insert_or_assign | template <class M><br/>std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj); | 插入或者修改字典元素若插入发生则 bool 组分为 true ，若赋值发生则 bool 组分为 false 。 |
| insert_or_assign | template <class M><br/>iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj); | 插入或者修改字典元素，返回指向被插入或更新的元素的迭代器。   |
| insert_or_assign | template <class M><br/>iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj); | 插入或者修改字典元素，返回指向被插入或更新的元素的迭代器。   |

**使用例子：**

```cpp
std::unordered_map<std::string, std::string> myMap;

print_result( myMap.insert_or_assign("a", "apple"     ) );
print_result( myMap.insert_or_assign("b", "banana"    ) );
print_result( myMap.insert_or_assign("c", "cherry"    ) );
print_result( myMap.insert_or_assign("c", "clementine") );

for (const auto &node : myMap) { print_node(node); }
auto print_node = [](const auto &node) {
    std::cout << "[" << node.first << "] = " << node.second << '\n';
};
```

| 函数名      | 函数声明                                                     | 说明                                                         |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| try_emplace | template <class... Args><br/>pair<iterator, bool> try_emplace(const key_type& k, Args&&... args); | 若容器中已存在等价于 k 的关键，则不做任何事。否则行为类似 emplace, 返回类似! |
| try_emplace | template <class... Args><br/>pair<iterator, bool> try_emplace(key_type&& k, Args&&... args); | 若容器中已存在等价于 k 的关键，则不做任何事。否则行为类似 emplace, 返回类似! |
| try_emplace | template <class... Args><br/>iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args); | 若容器中已存在等价于 k 的关键，则不做任何事。否则行为类似 emplace_hint , 返回类似!。 |
| try_emplace | template <class... Args><br/>iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args); | 若容器中已存在等价于 k 的关键，则不做任何事。否则行为类似 emplace_hint, 返回类似!。 |

```cpp
std::unordered_map<std::string, std::string> m;
m.try_emplace("a", "a"s);
m.try_emplace("b", "abcd");
m.try_emplace("c", 10, 'c');
m.try_emplace("c", "Won't be inserted");

for (const auto &p : m) {
    std::cout << p.first << " => " << p.second << '\n';
}
```



### [5. unordered_multimap](#) 

```cpp
template<
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator< std::pair<const Key, T> >
> class unordered_multimap;
```



-----
`时间`: `[]` 
