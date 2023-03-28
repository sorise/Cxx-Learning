### [C++ 标准现代特性](#)
 **介绍**：内容包括元祖、位集合、还有随机数...

-----
- [x] [1. 元祖 tuple](#1-元祖-tuple)
- [x] [2. 位集 bitset](#2-位集-bitset)
- [x] [3. 随机数](#3-随机数)
- [x] [4. 生成随机数](#4-生成随机数)

-----

### [1. 元祖 tuple](#)
其他语言都有的东西，类模板 **std::tuple** 是固定大小的异类值汇集。

```cpp
template< class... Types > //可变参数模板
class tuple;
```

使用案例：

```cpp
std::tuple<std::string, float, int> user(std::string("king zong"), 3.1415, 108);
std::cout << std::get<0>(user) << std::endl;//king zong
std::cout << std::get<1>(user) << std::endl;//3.1415
std::cout << std::get<2>(user) << std::endl;//108
```

#### [1.1 定义和初始化tuple](#)
可以直接创建模板类，或者使用[make_tuple](https://zh.cppreference.com/w/cpp/utility/tuple/make_tuple)方法创建一个 `tuple` 对象，其类型根据各实参类型定义。

```cpp
std::tuple<int, int, float> numbers{ 52,35,35.12 };


std::tuple<std::string, std::array<float, 5>> grade{ 
    std::string("remix"), 
    {89.5,68.5,95.5,98.5,99.0} 
};
//使用 make_tuple
auto tpl = std::make_tuple<std::string, std::array<float, 5>>(
    std::string("remix"), 
    { 89.5,68.5,95.5,98.5,99.0 }
);

std::cout << std::get<1>(tpl)[2] << std::endl; //95.5
std::cout << std::get<1>(grade)[2] << std::endl; //95.5
```

#### [1.2 访问tuple的成员](#)
使用API [std::get(std::tuple)](https://zh.cppreference.com/w/cpp/utility/tuple/get)返回t的第i个数据成员的**引用**：如果t是一个左值，结果是一个左值引用；否则，结果是一个右值引用。此外tuple的所有成员都是pulic的。

```cpp
get<i>(t)
```

通过引用修改元祖内容：

```cpp
auto tpl = std::make_tuple<std::string, std::array<float, 5>>(
    std::string("remix"), 
    { 89.5,68.5,95.5,98.5,99.0 }
);

std::get<1>(tpl)[1] = 100; //返回的是引用，可以修改
std::cout << std::get<1>(tpl)[1] << std::endl; //100
```

#### [1.3 判断元祖是否相等](#)
对元祖的比较操作C++20以后就支持判断两个元祖是否相等或者 不相等了！也就是说只支持 **==** 和 **!=** 运算符，而且还要求必须两个元祖的类型是一致的！

#### [1.4 获得tuple成员的类型信息](#)
首先我们获取一个元祖的大小, 使用 tuple_size 赋值：

```cpp
//获取元祖的大小
std::cout << std::tuple_size<decltype(tpl)>::value << std::endl; //2
```

然后获得元祖某位置的类型， 使用 tuple_element赋值:

```cpp
//获得类型
std::tuple_element<1, decltype(tpl)>::type number = std::get<1>(tpl);
//number 的类型就是 std::array<float, 5>
std::for_each(number.begin(), number.end(), [](const float& v) {
    std::cout << v << std::endl;
});
```

### [2. 位集 bitset](#) 
类模板 **bitset** 表示一个 **N** 位的固定大小序列。可以用标准逻辑运算符操作**bitset**，并将它与字符串和整数相互转换。头文件所在：**#include\<bitset\>**!

```cpp
template< std::size_t N >
class bitset;
```

#### [2.1 构造函数](#)
bitset有许多的构造函数，可以直接初始化，也可以用整数、string、或者自定义的参数用于构造位集！

```cpp
constexpr bitset() noexcept;
bitset( unsigned long val );
constexpr bitset( unsigned long long val ) noexcept;
```

默认构造函数。构造所有位都设为零的 bitset。

构造 bitset，初始化首（最右、最低）`M` 位位置为对应 val 的位值，其中 `M` 是以下两者中的较小者, 如果 `M` 小于 `N`，那么剩余位位置被初始化为零。

```cpp
// 空构造函数
std::bitset<8> b1;      		// [0,0,0,0,0,0,0,0]

// unsigned long long 构造函数
std::bitset<8> b2(42);  		// [0,0,1,0,1,0,1,0]
std::bitset<70> bl(ULLONG_MAX); // [0,0,0,0,0,0,1,1,1,...,1,1,1]
std::bitset<8> bs(0xfff0);      // [1,1,1,1,0,0,0,0]

// string 构造函数
std::string bit_string = "110010";
std::bitset<8> b3(bit_string); 
std::bitset<8> b4(bit_string, 2); 
std::bitset<8> b5(bit_string, 2, 3);

// 使用自定义零/一数字的 string 构造函数
std::string alpha_bit_string = "aBaaBBaB";
std::bitset<8> b6(alpha_bit_string, 0, alpha_bit_string.size(),
                  'a', 'B');  // [0,1,0,0,1,1,0,1]

//使用自定义数字的 char* 构造函数
std::bitset<8> b7("XXXXYYYY", 8, 'X', 'Y'); // [0,0,0,0,1,1,1,1]
```

#### [2.2 std::bitset\<N\>::reference](#)
表示到一个位的引用的代理类。作为可公开访问的嵌套类。此类用作允许用户与 bitset 的单个位交互的代理对象，因为标准 C++ 类型（如引用和指针）没有内建指定单个位的足够精度。

**std::bitset::reference** 的基本用途是提供能从 **operator[ ]** 返回的左值。

```cpp
std::bitset<8> bs(0b00001110);
std::cout << bs.to_string() << "\n"; // 00001110

std::bitset<8>::reference ref = bs[2];

ref = false;
std::cout << bs.to_string() << "\n"; // 00001010
```

#### [2.3 元素访问与修改](#)
**operator[]** , 访问指定的位。[test()](https://zh.cppreference.com/w/cpp/utility/bitset/test)也可以访问特定位！

```cpp
const int len = 8;
std::bitset<len> bs(0b00001110);
for (size_t i = 0; i < len; i++)
{
    std::cout << bs[i] << " ";
}
printf("\n");
for (size_t i = 0; i < len; i++)
{
    std::cout << bs.test(i) << " ";
}
/*
0 1 1 1 0 0 0 0
0 1 1 1 0 0 0 0
*/
```

* **bitset& set(pos, value)** 函数： 

1. 将所有位设置为 true。
2. 将在 pos 的位设置为 value。

* **bitset& set()** ;  设置所有位为 true 或设置某一位为指定值。
* **bitset& reset()** ;  设置位为 false。
* **bitset& reset( std::size_t pos )** ;  设置在 pos 的位为 false。
```cpp
std::bitset<8> b;
std::cout << b << '\n'; 		//00000000
std::cout << b.set() << '\n';	//11111111
```
* **bitset&  flip()** 等价于在 bitset 一部分或全体上的逻辑非
* **bitset& flip( std::size_t pos );**  翻转在 pos 的位。

#### [2.4 判断操作](#)
* **bool all() const noexcept;**   若全部位被设为 true 则为 true ，否则为 false。
* **bool any() const noexcept;** 若任何一位被设为 true 则为 true ，否则为 false。
* **bool none() const noexcept;** 若无位被设为 true 则为 true ，否则为 false。
* **std::size_t count() const;**  返回设为 true 的位数。

```cpp
std::bitset<8> bs(0b00001110);
std::cout << bs.any() << "\n"; //1
std::cout << bs.all() << "\n"; //0
std::cout << bs.none() << "\n"; //0
std::cout << bs.count() << "\n"; //3
```

#### [2.5 转换](#)
* [to_string()](https://zh.cppreference.com/w/cpp/utility/bitset/to_string)  返回数据的字符串表示!
* [to_ulong()](https://zh.cppreference.com/w/cpp/utility/bitset/to_ulong) 返回数据的 unsigned long 整数表示!
* [to_ullong()](https://zh.cppreference.com/w/cpp/utility/bitset/to_ullong) 返回数据的 unsigned long long 整数表示！

#### [2.6 辅助类](#)
**std::hash\<std::bitset\>** 获得对象的哈希。

```cpp
#include <iostream>
#include <bitset>
#include <functional>
 
int main()
{
    std::bitset<4> b1(1);
    std::bitset<4> b2(2);
    std::bitset<4> b3(b2);
 
    std::hash<std::bitset<4>> hash_fn;
 
    std::cout << hash_fn(b1) << '\n';
    std::cout << hash_fn(b2) << '\n';
    std::cout << hash_fn(b3) << '\n';
}
/*
67918732
118251589
118251589
*/
```

#### [2.7 集合操作](#)
operator&=  **进行二进制与、或、异或及非。**
operator|=
operator^=
operator~
operator<<=  **进行二进制左移和右移。**
operator>>=
operator<<
operator>>

```cpp
#include <bitset>
#include <iostream>
 
int main()
{
    std::bitset<4> b1("0110");
    std::bitset<4> b2("0011");
    std::cout << "b1 & b2: " << (b1 & b2) << '\n';
    std::cout << "b1 | b2: " << (b1 | b2) << '\n';
    std::cout << "b1 ^ b2: " << (b1 ^ b2) << '\n';
}
/*
b1 & b2: 0010
b1 | b2: 0111
b1 ^ b2: 0101
*/
```

### [3. 随机数](#)
随机数基本是程序需要，早期C和C++都使用经典 **线性同余算法** （linear congruential method）来获得随机数，此函数生成均匀分布的伪随机
整数，每个随机数的范围在0和一个系统相关的最大值 **RAND_MAX (宏常量)** 之间。

**C语言随机数:**

```cpp
#include<random>
#include <ctime>

srand(time(0));
std::cout << rand() << std::endl;
```

有时候我们需要浮点数随机数而不是整数，一些程序需要非均匀分布的随机数，C++在随机数库(头文件**random**)中通过定义一组协作的类来解决这些问题：

* **随机数引擎类(均匀随机位生成器 (URBG))**：一个引擎类可以生成unsigned随机数序列。
* **随机数分布类**：一个分布类使用一个引擎类生成指定类型的、在给定范围内的、服从特定概率分布的随机数。

如何形容他们的关系呢？ **可以理解为：概率统计的概率密度函数 y= f(x)**，随机数引擎类就是生成x，随机数分布类就是f。f可以使均匀分布、正态分布、指数分布。y就是我们最后的随机数！

#### [3.1 伪随机数生成器](#)
生成真随机数往往需要硬件的结合是一件非常困难的事情，所以使用伪随机代替真随机，伪随机数生成算法有如下：

1. **线性同余算法**（linear congruential method）线性同余引擎一般地快，并对状态的存储要求非常小。
2. **梅森旋转算法** 梅森缠绕器较慢且拥有较大的状态存储要求，但只要有正确的参数，就会有最长的的不可重复序列，且拥有最想要的谱特性（对于给定的想要的定义）
3. **平方区中法**
4. **蒙特卡洛算法**
5. **带进位减（一种延迟斐波那契）算法** 延迟斐波那契生成器在无先进算术指令集的处理器上非常快，但状态存储较为庞大，有时有不太想要的谱特性

#### [3.2 随机数引擎](#)
随机数引擎就是依照上诉或者其他随机数生成算法实现的！

随机数引擎以种子数据为熵源生成伪随机数。数种不同类的伪随机数生成算法实现为能定制的模板。

| 类模板                                                       | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [linear_congruential_engine](https://zh.cppreference.com/w/cpp/numeric/random/linear_congruential_engine)(C++11) | 实现[线性同余](https://en.wikipedia.org/wiki/Linear_congruential_generator)算法 |
| [mersenne_twister_engine](https://zh.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine)(C++11) | 实现[梅森缠绕器](https://en.wikipedia.org/wiki/Mersenne_twister)算法 |
| [subtract_with_carry_engine](https://zh.cppreference.com/w/cpp/numeric/random/subtract_with_carry_engine)(C++11) | 实现带进位减（一种[延迟斐波那契](https://en.wikipedia.org/wiki/Lagged_Fibonacci_generator)）算法 |

这些模板类C++标准库都已经预定义的许多的实现，这些模板类的显式类被称为：**预定义随机数生成器**

#### [3.3 default_random_engine](#)
每个编译器都会预定义多个随机数生成器，区别在于性能和随机性质量不同，其中有一个default_random_engine **函数对象类**，定义的随机无符号整数的通用源，它由编译器自己指定是其中的哪个！

```cpp
std::default_random_engine e(time(0) ); //传入一个随机数种子
for (size_t i = 0; i < 10; i++)
{
    std::cout <<i <<":" << e() << std::endl;
}
```

**创建随机数分布对象**

```cpp
default_random_engine e(time(0) ); //传入一个随机数种子
uniform_int_distribution<unsigned> u1(0, 9);//得到0到9之间的数
cout << u1(e) << endl;
cout << u1(e) << endl;
cout << u1(e) << endl;
```

**uniform_int_distribution**，生成均匀分布的unsigned值。

**随机数引擎操作:**

| 操作                            | 说明                               |
| ------------------------------- | ---------------------------------- |
| Engine  e                       | 默认构造函数，使用该引擎类默认种子 |
| Engine  e(s)                    | 使用整数 s 作为种子                |
| e.seed(s)                       | 使用种子s重置引擎的状态            |
| e.min()                         | 该引擎可以生成的最小值             |
| e.max()                         | 该引擎可以生成的最大值             |
| Engine::result_type             | 改引擎生成的 unsigend整数类型      |
| e.discard(unsigned long long z) | 将引擎推进z步                      |
| **operator()**                  | 获得随机数                         |

#### [3.4 预定义随机数生成器](#)
定义了数个特别的流行算法。

| **类型**              | **定义**                                                     |
| --------------------- | ------------------------------------------------------------ |
| mt19937               | 32 位梅森缠绕器, 使用随机数引擎 **mersenne_twister_engine** 定义实现。 |
| mt19937_64            | 64 位梅森缠绕器，使用随机数引擎 **mersenne_twister_engine** 定义实现。 |
| default_random_engine | 默认引擎                                                     |
| minstd_rand0          | [std::linear_congruential_engine](http://zh.cppreference.com/w/cpp/numeric/random/linear_congruential_engine)<[std::uint_fast32_t](http://zh.cppreference.com/w/cpp/types/integer), 16807, 0, 2147483647> |
| minstd_rand           | [std::linear_congruential_engine](http://zh.cppreference.com/w/cpp/numeric/random/linear_congruential_engine)<[std::uint_fast32_t](http://zh.cppreference.com/w/cpp/types/integer), 48271, 0, 2147483647> |
| ranlux24_base         | [ std::subtract_with_carry_engine](http://zh.cppreference.com/w/cpp/numeric/random/subtract_with_carry_engine)<[std::uint_fast32_t](http://zh.cppreference.com/w/cpp/types/integer), 24, 10, 24> |
| ranlux48_base         | [std::subtract_with_carry_engine](http://zh.cppreference.com/w/cpp/numeric/random/subtract_with_carry_engine)<[std::uint_fast64_t](http://zh.cppreference.com/w/cpp/types/integer), 48, 5, 12> |
| ....                  | ...                                                          |

#### [3.5 随机数分布类](#)
有很多分布： 均匀分布、伯努利分布、泊松分布、正态分布(T分布，F分布, X^2分布)、采样分布。每一种分布有数种实现，查阅相关API文档！

### [4. 生成随机数](#) 
接来下我们给出使用例子：

#### [4.1 生成随机实数](#)
生成十个随机数，然后进行输出！

```cpp
std::mt19937_64 engine(time(0));
std::uniform_real_distribution<double> ureal(0, 2); //生成0 - 2的实数

std::ostream_iterator<double, char> out_iter(std::cout, " ");
for (size_t i = 0; i < 10; i++)
{
    *out_iter++ = ureal(engine);
}//0.454155 1.46436 1.6466 0.648223 1.37363 1.97065 1.64297 1.45391 0.692035 0.735622
```

#### [4.2 normal_distribution](#)
使用标准高斯正太分布：

```cpp
std::mt19937_64 engine(10);
std::normal_distribution<double> d(2,4);
//2是 均值/期望 也就是对称轴 ， 4是方差
std::ostream_iterator<double, char> out_iter(std::cout, "\n");

for (size_t i = 0; i < 20; i++)
{
    *out_iter++ = d(engine);
}
```

#### [4.3 二选一随机 bernoulli_distribution](#)
根据离散概率函数产生[伯努利分布](https://en.wikipedia.org/wiki/Bernoulli_distribution)上的 bool 值：

```cpp
std::random_device rd;
std::mt19937 gen(rd());
std::bernoulli_distribution d(0.25); //true 的概率为 0.25

std::cout << d(gen) << std::endl; //返回true或者false
```



-----
时间: [2023年3月11日17:16:26 大修] 