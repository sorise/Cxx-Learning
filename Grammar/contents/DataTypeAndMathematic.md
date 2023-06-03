### [C++ 基本数据类型](#)
**介绍**： C++类型分为两组： 基本类型、复合类型。 数据结构是什么，是数据类型和定义在数据类型上面的运算操作, 操作包括了， + - * / 类型转换....

-----
- [x] [1. C++ 数据类型概述](#1-c-数据类型概述)
- [x] [2. C++ 基本类型](#2-c-基本类型)
- [x] [3. C++ 类型转换](#3-c-类型转换)
- [x] [4. C++ 复合类型](#4-c-复合类型)
- [x] [5. C++ 三种管理数据内存的方式](#5-c-三种管理数据内存的方式)
- [x] [6. C++ 数组的替代品](#6-c-数组的替代品)

-----

### [1. C++ 数据类型概述](#)
内置的C++类型分为两组： **基本类型和复合类型**，大一点还可以分为内置类型和自定义类型。 数据是存放在计算机中的，我们要明白

1. 数据存放在哪里
2. 数据存储什么值
3. 类型是什么，占多少存储空间。

存储整数，四个字节大小，通过访问变量 `count` 访问，存储该数据的内存单元。`[C++ 中可以用 &运算符来检索count的内存地址。]`
```cpp
int count = 20;
```

#### [1.1 变量名称](#) 
**C++**  **字母、数字、下划线。字母，下划线可以做开头。** C++对于名称的长度没有限制，名称中所有的字符都有意义，但有些平台有长度限制。` `这与C语言不同， C99标准，只保证名称中的前63个字符有意义。即使第64个字符不同，但是前63个字符相同。也会被当做相同的变量名。 **[注意]**   

至于命名习惯、方案什么的。自己写程序建议使用 country_count 这的命名。或者大驼峰方案。 介绍下前缀方案例如nStudentWeight :整形数据类型，表示学生重量` `实际中嘛，就按照公司规范写就行了。

**C++ 变量存在的意义： 给内存空间起一个别名，方便管理和使用内存空间。**

#### [1.2 常量](#) 
例如一个小时60分钟，一周七天，一年十二个月，都是固定的数据，对于不可更改的数据可以使用常量来定义存储。 C++中有两种定义常量的方式

1. **#define**  宏常量 本质是是一个文本替换， 通常在文件上方定义 , 也就是说它的常量是全局的。
```cpp
#define 常量名  常量值 

#define Day 7;
```
2. **const**： 修饰的变量. 通常在变量定义前面加上关键字const , 一旦定义就必须进行初始化！` `他的常量是有作用于的。
```cpp
const int month = 24;
```

C++ 尝尝用 const来定义符号常量。而不是C语言的遗留物。 #define。 如果你设计的头文件既可以用于C，也可以用于C++。那定义符号常量还是需要用到 #define。

**const 确实简单，但是const 一旦和其他的东西结合就复杂了，比如 const type \*pointerName  和 type \* const pointerName 还有 const type & !**

**默认情况下，const对象仅在文件内有效！** 需要加上 extern 才能在多个文件生效

* **顶层 const**： 指针本身是常量。此时，指针在定义初始化之外，不能被赋值修改。称指针为指针常量。` `不能修改自己的值！指针指向不能修改！
* **底层 const**： 指针指向的变量是常量。此时，不能通过解引用指针的方式，修改变量的值。称指针为常量的指针。` `不能通过自己修改引用/指向的值。但是可以修改自己的值！

#### [1.3 一次定义多次声明](#)
变量能且只能被定义一次，但是可以多次声明！ C++支持多文件分离式编译机制！可以在多个文件声明，只在一个文件里面定义！

variablesShare.h 声明一个共享变量
```cpp
#ifndef __VARIABLE_SHARE_H__
#define __VARIABLE_SHARE_H__ 2
extern int MAX_CONNECTION_COUNT; //最大链接数量
extern int MIN_CONNECTION_COUNT; //最小维持链接线程数量
#endif
```
variablesShare.cpp` `定义
```cpp
#include"variablesShare.h"
int MAX_CONNECTION_COUNT = 512; //最大链接数量
int MIN_CONNECTION_COUNT = 2; //最小维持链接线程数量
```
main.cpp
```cpp
#include<iostream>
#include"variablesShare.h"

int main(int argc, char *argv[]){
    std::cout << MAX_CONNECTION_COUNT << "\n"; //512
    std::cout << MIN_CONNECTION_COUNT << "\n"; //2
   return 0;
}
```

### [2. C++ 基本类型](#) 
C++基本类型包括，整形 整形有 char short int long 和 long long。还有对应的无符号版本。 总共十个整形。不过char常用来表示字符。而不是数字。

C++ 为了应对不同计算机设计的需要，提供了一种灵活的标准，它确保了最小的长度，并不固定不同整形一定要具有的大小，例如int 它不一定是 32位的，它可以是16位的，甚至是64位的。C++标准只确保了 int 至少是和short一样长，short至少是16位的!` `而且你还必须要考虑到有符号和无符号的区别！

|类型|标准大小|说明|
|:----|:---|:---|
|bool|1位[bit] 1bit|true 非零值 / false 零值|
|char|8位[bit] 1字节|常用来表示字符 CHAR_BIT 返回位数，CHAR_MAX char的最大值。CHAR_MIN char的最小值！  |
|short|至少16位[bit] 2字节|SHORT_MAX SHORT_MIN USHORT_MAX（unsigned short 的最大值） |
|int|至少和short 一样长|INT_MAX INT_MIN UINT_MAX 意义和上面一样|
|long|至少32位[bit] 4字节 且至少和int一样长|LONG_MIN LONG_MAX ULONG_MAX ...|
|long long|至少64位，且至少和long一样长|LLONG_MAX LLONG_MIN ULLONG_MAX ...|
|unsigned 整形类型|和整形类型一样|声明无符号整数例如 unsigned int 无符号整形 只能表示 >= 0的整数|

int 很奇特啊。他可以是16位，24位，32位，在不同的系统中有所不同，在小内存系统例如IBM PC中就是16，在win7,win10就是32位。 

有些系统还允许选择如何处理int的宽度随实现而异, 实际上 short是 short int的简称。

而long是 long int的简称， 如果要知道系统中整数的最大长度，可以在程序中使用C++工具来检查类型的长度, sizeof 运算符返回类型和变量的长度。

单位是字节。 字节这玩意儿挺奇特的，一般来说是八位一字节。但是有些实现，有16位甚至是32位的字节。

你完全可以通过 sizeof  与最大值 相结合去判断整形的宽度。 sizeof 不是函数哦，他是一个运算符，类型名称要放到括号中 sizeof(int), 但是变量可以加括号也可以不加。

```cpp
int year = 18;

int year_bytes = sizeof year; //4
int year_bytes_ = sizeof(year);//4
int int_byte = sizeof(int);//4
```


`头文件 climits： #include <climits> ` `定义了符号常量来表示对类型的限制。例如: ` 
1. `INT_MAX 表示类型 int 能够存储的最大值！`  
2. `CHAR_BIT 表示 char的位数。 `
3. `UCHAR_MAX 表示unsigned char的最大值！`
4. `SHRT_MAX 表示 short的最小值`
5. `....需要的时候查询就可以了`

```cpp
#include <iostream>
#include <climits>

int main(int argc, char const* argv[])
{
    short max_short = SHRT_MAX;
    int max_int = INT_MAX;
    long max_long = LONG_MAX;
    long long max_llong = LLONG_MAX;

    cout << "short is " << sizeof(short) << "bytes. \n";
    cout << "int is " << sizeof(int) << "bytes. \n";
    cout << "long is " << sizeof(long) << "bytes. \n";
    cout << "Llong is " << sizeof max_llong << "bytes. \n";

    cout << "maximum values: " << endl;
    cout << "shot: " << max_short << endl;
    cout << "int: " << max_int << endl;
    cout << "long: " << max_long << endl;
    cout << "long long: " << max_llong << endl;
    return 0;
}
```

```cpp
#include <iostream>
#include <climits>

using namespace std;
int main()
{
    int int_bit = INT_WIDTH;
    int max = INT_MAX;
    cout << "int bits: "<< int_bit << endl; //32
    cout << "value of max int is: "<< max << endl;//2147483647
    return 0;
}
```


`无符号类型使用 关键字` `unsigned 来修改声明即可。`
```cpp
unsigned short change = 15;
unsigned int count = 38484;
unsigned long country_size_km = 9630405;
```
`C++并不保证符号整形超越限制时不出错。`

`最小值减一  变成最大值。 当然要考虑到类型转换问题。 ` `cout <<  change - 1 << endl;//-32769`  `想想为啥会是-32769` 

`因为发生了类型转换！ change - 1 的类型变成了int。因为默认1的类型是 int。`
```cpp
short change = SHRT_MIN;
cout << change  <<endl;//-32768  
change -= 1;
cout <<  change << endl;//32767
cout <<  change - 1 << endl;//-32769
```

`最大值 加1 变成最小值`
```cpp
unsigned int count = UINT_MAX;//4294967295
count += 1;
cout << count << endl;//0
```

`整形溢出分为几种情况，首先是整形是否有符号，其次是向上溢出 + 还是向下溢出 -`

`比如 有符号的 short 类型`: `最大值 (32767) + 1 会变成最小值 而 最小值(-32768) - 1 会变成最大值`

`无符号的 short 类型`: `最大值 (65535) + 1 会变成最小值 0 而 最小值(0) - 1 会变成最大值`


#### [2.1 整形类型的选择](#)
`C++ 提供了十种整形类型。通常int被设置为对目标计算机而言最为“自然”的长度。自然长度：计算机处理起来效率最高的长度。一般情况下使用int。如果变量不可能为负。
那么应该使用无符号整形。 ` `由于int宽度随系统的不同有所变化。例如将程序从16位的系统转移到int为32位的系统。那么而用于存储int数组的内存存量将加倍。但是short类型
就不会受影响。 如果只需要一个字节。可以使用Char。` `如果你使用其他语言，例如Java,.NET 当然可以无脑int。 但是你使用C++` `这种需要高性能的语言。当然是能省就要省，
保持这个好习惯吧。`


#### [2.2 初始化](#)
`C语言初始化：` `int year = 2022;` `C++提供了一种初始化语法！int count(32);` `C++ 11提供了另一种初始化方式。这种方式用于数组和结构。但在C++98中，也可以用于单值变量`
`如果大括号不包含任何东西。在这种情况下，变量被初始化为0.`  **初始化和赋值是两种操作，不要混为一谈！**

* **初始化是指创建变量的时候赋予其一个初始值！**
* **赋值的含义是把对象当前的值擦除掉，而以一个新的值来代替！**

```cpp
int year = 2019;
int age = { 18 };
int count(32);
int sum = {}; //sum = 0
cout << age << endl; //18
cout << count << endl; //32

//C++ 11 推荐使用
int years[5] = { 1945,1991,2008,2014,2022 };

for (int i = 0; i < 5; i++)
{
    cout << "years[" << i << "]" <<": " << years[i] << endl;
}
```

**定义在函数体内的内置类型的对象如果没有初始化，则其值未定义。类的对象如果没有显式地初始化，则其值由类确定！**

#### [2.3 整形字面量](#)
`整形字面量，即使进制问题。C++ 有三种不同的计数方式来表示整数。` `10, 8, 16 进制。` `93 就是十进制 ` `042 就是八进制`  `0x15, 0X74A 就是十六进制。`

```cpp
int value = 052;//42
int change = 0x1A4C; //6732
cout << value << endl;
cout <<  change << endl;
```

`如何帮整数输出为进制呢？ 利用 cout的控制符 std::hex , std::dec , std::oct`

```cpp
using namespace std;

int value = 64;
cout << hex <<"十六进制为：" << value << endl;//十六进制为：40
cout << dec << "十进制为：" << value << endl;//十进制为：64
cout << oct << "八进制为：" << value << endl;//八进制为：100        
```

`C++确定常量的类型， 默认情况下，C++将整形常量存储为int。` `例如： 22022 将被存储为 int` `但是如何指定常量的存储类型呢？` `通过后缀来实现。例如 22022L 将被存储为long类型  22022LL 或者 22022ll 被存储为 long long类型。`
`15U 15u被存储为 unsigned int. 所以后缀有 u,U, UL, ul,Ull, ULL...`


1. `ul 后缀 /UL` `表示无符号长整型  unsigned long`
2. `u 后缀 /U` `表示无符号 int`
3. `ll 后缀 /LL` `表示 long long`
4. `ull 后缀 /ULL` `unsigned long long类型！`

```cpp
unsigned int year = 2021u;

cout << hex <<"十六进制为：" << 2204525LL << endl;
```

#### [2.4 char](#)
`实际上，很多系统支持的字符不超过128个，例如ASCII字符集。 一个字节就可以表示所有的符号。也可以表示比short更小的整数。` **当然char表示的字符集范围有限，不适合 utf8, unicode字符集。** 
`C++支持的 wchar_t 宽字符类型、 char16_t char32_t 可以存储更多的值。`  **请先去了解一下计算机编码技术！**

`cout.put(char)`:`输出一个字符。`
```cpp
char c = 'A';
printf("%c  %d \n",c,c);
```

`\u00F6`:`以\u开头，表示Unicode码点。十六位两个字节表示一个字符！` 

`unicode字符集。可以表示很多语言，ASCII是它的子集。他可以表示拉丁字符，中文，日语，阿拉伯等等。总共可以表示109000多种符号和90多个手写符号。Unicode给每个字符指定一个编号----码点。例如 U-2228 u开头，表示Unicode码点。Unicode2228表示该字符的十六进制号。`

`char Ascii`  `里面有一些转移字符！ 这些字符无法通过键盘输入`  **只能用转义字符输出！**

-----
`\n`:`换行符  ascii:10 十六进制 oxA`  
`\t`:`水平制表符 ascii:9 十六进制 ox9`  
`\v`:`垂直制表符 ascii:11 十六进制 oxB`  
`\b`:`退格符  ascii:8 十六进制 ox8`  
`\r`:`回车符  ascii:13 十六进制 oxD`  
`\\`:`\  ascii:92 十六进制 ox5c`  
`\"`:`“  ascii:34 十六进制 ox22`  
`\'`:`’  ascii:39 十六进制 ox27`  

**也可以直接使用编码的方式**
```cpp
std::cout<< '\65' << std::endl; //78
std::cout<< "\65  \112" << std::endl; //112
```

**u8"hello"** `u8 前缀是utf-8字符串字面量`

#### [2.5 unsigned char 和 signed char](#)
`与int不同`。**char在默认情况下，既不是没有符号，也不是有符号的。它是否有符号由C++实现决定**。`这编译器开发人员可以最大限度地将这种类型与硬件属性匹配起来。如果你需要指定是否有符号。可以用unsigned char 和 signed char 来声明 char。 `

```cpp
signed char c = 'c';
unsigned char v = 200;
```

#### [2.6 wchar_t](#)
表示系统使用的最大扩展字符集。这种类型与另一种整形(underlying)的长度和符号属性相同。因此不同的系统下，它的大小可以是 unsigned short， 也可能在另一个系统中int。对于wchar_t的输入输出，不适合 cin,cout。

C++ 提供了 wcin, wcout 来处理。可以加上前缀L来指示宽字符常量和宽字符串， **wchar_t总是2个字节长 。**

```cpp
wchar_t first = L'P';
wchar_t second = L'D';
wcout << first << second;//PD
```
#### [2.7 char16_t char32_t](#)
`随着Unicode的普及。 wchar_t 已经不能满足需求。 C++提供了 char16_t和char32_t。都是无符号的。 C++使用前缀u和U 表示  char16_t和char32_t。他们和wchar_t也有底层类型。一种内置的整形。但底层类型可能随着系统而异。`

```cpp
char16_t c1 = u'蒋';
char32_t c2 = U'蒋';//33931
cout << c2;
```

#### [2.8 bool](#)
`C++ 新类型哦，预定义的字面值是 false和true。 如果你复制整数。那么非零值为true，零值为 false。`

```cpp
bool sex = true;
bool isPass = 60; // true
bool isZore = -20; //true
bool flag = 0; //false
```

#### [2.9 浮点数](#)
`C++ 支持三种浮点类型，float 和 double、long double。 他们的不同在于精度不一样！ 与整数相比。浮点数有两大优缺点，浮点数运算通常比整数运算慢，且精度将降低`

|`类型`|`标准大小`|`说明`|
|:----|:---|:---|
|`float`|`至少 32位有效位`|`  s符号位 M指数部分 E 小数部分。 单精度:N共32位，其中S占1位，E占8位，M占23位。`|
|`double`|`至少 48位有效位`|` 双精度:N共64位，其中S占1位，E占11位，M占52位。 1.0001*2^3 符号位 0 表示正数，小数部分是 0.0001，指数部分是127 +3（移码表示法）  整数部分必然是1 不用存储`|
|`long double`|`至少和double 一样多`|`  `|

`移码（又叫增码或偏置码）通常用于表示浮点数的阶码，其表示形式与补码相似，只是其符号位用“1”表示正数，用“0”表示负数，数值部分与补码相同。`

**浮点数的存储方式 准寻 IEEE 标准  所有浮点数都以 二进制科学计数法存储！ 例如 8.25 存储位二进制科学计数法(1.0001*2^3)**

**由于浮点数天生的不准确的问题，所以推荐使用高精度 double 的浮点数让运算更加准确**

```cpp
printf("%f\n", 1e7f/9.0f);//1111111.125000
printf("%lf\n", 1e7/9.0);//1111111.111111 
```

#### [2.10 浮点数字面量](#)
`浮点数可以表示为小数点模式 3.1598245，也可以表示为指数类型！ 2.345E+22f;`

`默认浮点数常量的类型是 double, 如果需要常量位float类型，需要使用f或F后缀！`
```cpp
double PI = 3.1415926;
float pi = 3.1415926F; //加上后缀不需要 double 转 float
```

### [3. C++ 类型转换](#) 
`C++ 类型转换，有隐式类型转换和强制类型转换。转换类型发生的地方！ ` `初始化和赋值进行的转换 例如： bool flag = 30; //true ` `以{}方式初始化时进行的转换` 
`表达式中的转换` `传递参数时的转换` `强制类型转换` `auto类型推断`

```cpp
char val = 31325; //93
int count = (int)val;
auto pi = 3.1415926; // double
short age = 15;
auto val = age + 15;//int
```
`强制类型转换的两种形式`
* `(typename) value` `或者` `typename (value)`

#### [3.1 数值转换问题的问题](#)
1. `较大的浮点数转换位较小的浮点数类型，double 变 float 会使得精度下降`
2. `浮点数转换位整数，会舍弃小数部分！`
3. `大整数转换为小整数会导致取值范围不够的问题 例如long类型最大值转换位 int 会舍弃高位`
4. `赋值给带符号类型一个超出它表示范围的值的时候，结果是` **未定义的** `程序可能继续执行、也可能崩溃`
#### [3.2 类型转换的地方](#)

`1. 初始化和赋值进行的转换`  
```cpp
bool sex = 256; // 转换位 true
float year = 98 / 5.0;
```
`2. 以{}方式初始化时进行的转换` 
```cpp
char c = {64} ; //int 转换位 char
```
`3.表达式中的转换`

`4. 传递参数时的转换：` **推荐函数希望的参数是什么类型就传递给什么类型**

`5. 强制类型转换`
```cpp
char val = 93;
int count = (int)val;

char v = 25;
int validate =  static_cast<int>(v);
```


#### [3.3 auto](#) 
`C++11` 新增了一个工具，让编译器能够根据初始值/表达式推断变量的类型。 **但是编译器推断出来的auto类型有时候和初始值的类型并不完全一样！**

```cpp
auto pi = 3.1415926; // double
auto val = age + 15;//int

//类型不一致  引用作为初始化
const double & score = 58.65
auto v = score;//v是 double 类型
```

1. 使用引用其实是使用引用的对象，特别是当引用被当做初始值时，真正参与初始化的其实是引用对象的值。
2. auto一般会忽略掉 **顶层 const**。同时保存 **底层const**
3. 如果希望推断出来的auto类型是 **顶层const。需要明确指出**

```cpp
const int val{15};
const int * const pt = &val; //指向常量的指针常量

auto t = pt; //const int * 类型 保存了 底层const
const auto t2 = pt; //const int * const 保存了顶层 const
```

#### [3.4 含有无符号类型的表达式](#) 
如果你的表达式里面具有无符号类型，那么你需要万分小心你的运算结果！


**无符号和有符号进行运算：** `如下所示 -42 + 10 的结果是不预测的！` `在32位系统中它的值是：` `4294967264`
```cpp
unsigned int val = 10;
int t = -42;

std::cout << val + t << "\n"; //4294967264
std::cout << val - t << "\n"; //52
std::cout << t - val  << "\n"; //4294967244
```
**上述表达式会先进行有符号数向无符号数的类型转换，再进行运算！** 
```cpp
unsigned int u1 = 42, u2 = 10;
std::cout << u2 - u1 << "\n"; //4294967264
std::cout << u1 - u2 << "\n"; //32
```
**如果两个无符号类型进行运算，请保证结果不为负数，不然运算结果会发生错误！**


### [4. C++ 复合类型](#) 
数组类型：例如 scores，它首先是一个数组类型 其次是一个 float数组。所以数组其实是两种类型的合成！ 还有指针类型、结构体、联合体、枚举、类
```cpp
float scores[60];
```

#### [4.1 数组](#)
数组 `Array` 是一种数据格式, 能够存储多个同类型的值！: size 必须是整形常量 `1. 使用 #define 定义` `2. 使用const定义` `3. 可以是常量表达式 3 + 5` 
**数组使用的是C++内置的下标运算符,所以不是一个无符号整数,而vector与string标准库类型的下标运算符的索引值就是无符号整数**

```cpp
typeName arrayName[size];
```
`数组初始化  C++ 允许在声明语句就初始化数组元素，也可以初始化部分，其余全部为0` **不能直接用一个数组给另一个数组赋值  int scores[10] = myScoresArray**

```cpp
string names[4] = {"kciker", "tome" , "remix", "redux"};

int scores[] = { 35,95,68,95,86,86 }; //编译器自动推断长度

float vals[5] = { 0.0 }; //可以初始化部分，其余全部为0
```
`C++ 11 初始化直接省略了 = `
```cpp
const short name_length = 4;
string names[name_length]  {"kciker", "tome" , "remix", "redux"};

int scores[]  { 35,95,68,95,86,86 }; //编译器自动推断长度

float vals[5]  { 0.0 };

for (int i = 0; i < name_length; i++) {
    cout << "name[" << i << "]:" << names[i]<<endl;
}
```
`列表初始化禁止缩窄转换(精度降低的类型转换)`

```cpp
long plifs[] {25, 92 ,3.152}; //不被允许 double 转换位 long 缩窄了
char slifs[] {'a',56,55555,54}; //不被允许 55555 超过char的最大值 缩窄了
```


`对数组名  使用sizeof会以字节为单位返回整个数组的长度。`

`数组声明使用数组名来标记存储位置。`
```cpp
void listArray(const int *vals, int len){
    printf("[");
    for (int i = 0; i < len - 1; i++)
    {
        printf("%d,", vals[i]);
    }
    printf("%d]", vals[len-1]);
    
}

int main(int argc, char *argv[]){
    const int len = 7;
    int scores[len] = {78 , 89, 75, 84, 75, 94, 92};
    std::thread task1(listArray, scores, len);
    
    task1.join();
    printf("\n");
    return 0;
}//[78,89,75,84,75,94,92]
```

**C++11 也支持数组使用 for-range 循环**
```cpp
int scores[5] = {1,2,4,8,16};

for (auto &&v : scores)
{
    std::cout<< v << "\n";
}//1 2 4 8 16
```

#### 4.1.1 二维数组
`你可以通过创建元素时数组的数组，二维数组很简单 C++没有提供二维数组类型，但用户可以创建每个元素本身都是数组的数组。` `使用方法和C语言差不多！`
```cpp
int matirx[5][5];
```
`matrix 是一个二位数组指令！不是一个二级指针，他是一个指向一个一维指针数组的指针。`

```cpp
int matrix[5][5] = {
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,0,1,0,0},
    {1,0,0,1,0},
    {1,0,0,0,1}
};

printf("3 row 3 column: %d\n", *(*(matirx + 2) + 2));
//3 row 3 column: 1
```

#### 4.1.2 多维数组
`C++语言并没有所谓的多维数组，本质上多维数组表示的是`：**数组的数组**  `根据由内到外的阅读顺序就可以理解了。`

```cpp
int matrix[3][4]; //matrix是一个大小为3的数组，每个元素都是一个含有四个整数的数组

int arr[10][15][20];
//arr 是一个长度为10的数组，每个数组元素又是一个长度为15的数组，长度15的数组每个元素又是一个长度为20的数组！
```

#### [4.2 C语言处理字符串](#)
`C++ 处理字符串的方式有两种，第一种来自C语言。是一个char数组，结尾必须存在 \0空字符。才能为单做一个字符串！` `前者字符串后者不是`

```cpp
char dog_name[] = { 'k', 'i', 'c','k', 'e','r', '\0' };
char dog_name_not_include_0[] = { 'k', 'i', 'c','k', 'e','r'};
```

`拼接字符串！`  `C++允许拼接字符串字面值。任何两个由空白[空格，制表符和换行符]分隔的字符串常量都将自动拼接成一个！`
```cpp
cout << "i'd like my right arm to be" "a greate vilinist.\n";
```
`sizeof 和 strlen`:`前者返回大小按照字节计算，后者返回字符串长度，不包含空串`

```cpp
#include<cstring>

char dog_name[] = { 'k', 'i', 'c','k', 'e','r', '\0' };
char dog_name_not_include_0[] = { 'k', 'i', 'c','k', 'e','r' };
cout << strlen(dog_name) << endl; //6
//cout << strlen(dog_name_not_include_0);//不是字符串 不合法使用
cout << sizeof(dog_name) << endl; //7
cout << sizeof(dog_name_not_include_0) << endl;//6
```

**字符串大小问题:**

`要知道 strlen 是通过 \0来判断字符串长度的，那么请看如下程序`
```cpp
#include<cstring>

char c_name[20];

int len = strlen(c_name);
```
`len 的大小是多少呢，你可能会说是 0 或者 19，但其实len的大小不确定，我在linux上面运行结果 len 等于 6。 为啥呢？` `因为 char c_name[20]; 只是开辟了一块内存空间，你还没有初始化呢！也就是说你开辟的内存空间还保存着原来的值！`

`这样写就稳定是 0了;`
```cpp
char c_name[20] = { "\0" };

int len = strlen(c_name);
cout << len << endl;
```

`如果将一个字符串直接赋值给一个字符串指针，需要加上const修改该字符指针，因为字面量字符串是常量！`
```cpp
const char *name =  "remix";
```

#### [4.3 获取字符串输入](#)
`C++ 提供了 cin.getline(name, len); 和 cin.get(name, strlen);` `两个函数都读取一行输入，前者将丢弃换行符，而get会将换行符保留在输入序列中。`

* `cin.getline(name, 20)`:`将读取的输入保存到name中并且，最多读取19个字符！`
* `cin.get() 只读取一个字符。`
* `cin.get(name, len)`:`将读取的输入保存到name中并且，最多读取18个字符！因为换行符也在里面。`

```cpp
const int name_len = 20;
char name[name_len] = {};
cout << "input your name:" << endl;
cin.getline(name, name_len);
cout <<"name:" << name << endl;
```

#### [4.4 string 类简介](#)
`C++98 提供了string 类。因此可以使用string类型的变量存储字符串,他可以自动扩充大小。使用string 需要包含头文件 string。 并且它位于std命名空间中。`

`string本质上可以看做是一种vector< char >,也就是元素为char的vector。所以有关 vector的基本操作都可以在string上进行。`

```sql
string name = "jxkicker";
name += " is my name";
cout << name;
```
`字符串初始化`： `1. C语言风格初始化` `2. cin 将输入存储到string对象中` `3. c++ 11初始化`  

```cpp
string userName{ "jxkicker" };
string message = { "this is a good message" };
```
`string 支持 复制，拼接和附加操作！`
* `size()`:`返回字符串的大小。`
* `c_str() `:`转换 string 类型到 char* 类型时，需要为char*添加 const 关键字；`


`可以将char \* 赋值给 string，也可以将string转换为 char\*`

```cpp
char user_name[] {"you can convert a char * to string"}; 
string store_user_name = user_name;
```

**string的函数**

* `str.find(‘A’)——查找 ‘A’`
* `str.find(“ABC”)——查找 “ABC”`
* `str.rfind(‘A’)——查找 ‘A’`
* `str.rfind(“ABC”)——查找 “ABC”`
* `to_string(val) 把val转换成string`
* `stoi(s,p,b) 把字符串s从p开始由b进制转化为十进制的int`
* `stol(s,p,b) 把字符串s从p开始由b进制转化为十进制的long`
* `stoul(s,p,b) 把字符串s从p开始由b进制转化为十进制的unsigned long`
* `stoll(s,p,b) 把字符串s从p开始由b进制转化为十进制的long long`
* `stoull(s,p,b) 把字符串s从p开始由b进制转化为十进制的unsigned long long`
* `stof(s,p) 把字符串s从p开始转换成float`
* `stod(s,p) 把字符串s从p开始转换成double`
* `stold(s,p) l把字符串s从p开始转换成long double`
* `str.assign(“ABC”)——清空字符串，并设置为 “ABC”`
* `str.assign(“ABC”,2)——清空字符串，并设置为"AB"，保留两个字符`
* `str.assign(“ABC”,1,1)——清空字符串，设置为 “ABC” 中的从 位置1 开始，保留 1个 字符`
* `str.assign(5，‘A’)——清空字符串，然后字符串设置为 5个 ‘A’`
* `str.length()——求字符串长度`
* `str.size()——和 length() 一样`
* `str.capacity()——获取容量，包含了不用增加内存就能使用的字符数`
* `str.reasize(10)——设置当前 str 的大小为10，若大小大与当前串的长度，\0 来填充`
* `str.reasize(10,char c)——设置当前 str 的大小为10，若大小大与当前串的长度，字0符c 来填充`
* `str.reserve(10)——设置str的容量 10，不会填充数据`
* `str.swap(str1)——交换 str1 和 str 的字符串`
* `str.push_back(‘A’)——在str末尾添加一个字符 ‘A’ ，参数必须是字符形式`
* `str.append(“ABC”)——在str末尾添加一个字符串 “ABC”，参数必须是字符串形式`
* `str.insert(2,3,‘A’)——在str下标为2的位置添加 3个 字符’A’`
* `str.insert(2,“ABC”)——在str下标为2的位置添加 字符串 “ABC”`
* `str.insert(2,“ABC”,1)——在str下标为2的位置添加 字符串 “ABC” 中 1个 字符`
* `str.insert(2,“ABC”,1,1)——在str下标为2的位置添加 字符串 “ABC” 中从位置 1 开始的 1 个字符`
* `str.insert( iterator pos, size_type count, CharT ch )——在 str 中，迭代器指向的 pos位置 插入 count个 字符 ch`
* `str.insert(++str1.begin(),2,‘a’); 结果：s4：ABCD -> AaaBCD`
* `str.insert( iterator pos, InputIt first, InputIt last )——在 str 中，pos位置 插入 str1 的 开始位置 到 结束为止`
* `str.insert(s4.begin(),str1.begin(),str1.end()); 结果：s4：ABCD str1：abc -> abcABCD`
* `str.erase(2)——删除 下标2 的位置开始，之后的全删除`
* `str.erase(2,1)——删除 下标2 的位置开始，之后的 1个 删除`
* `str.clear()——删除 str 所有`
* `str.replace(2,4,“abcd”)——从 下标2 的位置，替换 4个字节 ，为"abcd"`
* `str.empty()——判空（空返回1，非空返回0）`

`c++11标准增加了全局函数 用于类型转换`
* `string to_string (int val);`
* `string to_string (long val);`
* `string to_string (long long val);`
* `string to_string (unsigned val);`
* `string to_string (unsigned long val);`
* `string to_string (unsigned long long val);`
* `string to_string (float val);`
* `string to_string (double val);`
* `string to_string (long double val);`



#### [4.5 其他字符串](#)
`使用其他类型的字符串需要使用前缀哦！` `C++11 还支持Unicode字符编码UTF-8方案！ 需要使用前缀 u8 来提供字面值。根据编码的数字值，字符可能存储为1-4个八位组！` 
```cpp
wchar_t title[] = L"Linux is best";
char16_t name[] = u"Felonia Ripova";
char32_t carName[] = U"nanBoJiNi";
cout << carName;//010FFBA4
```
```cpp
string carName = u8"我在干嘛啊！";
cout << carName;//鎴戝湪骞插槢鍟婏紒
```

#### [4.6 结构 struct](#)
`C++ 允许声明结构变量时省略关键字struct。 与数组的不同在于结构体的成员可以是不同的类型！` **在C++中除了类中可以有构造函数和析构函数外，结构体中也可以包含构造函数和析构函数，这是因为结构体和类基本雷同，唯一区别是，类中成员变量默认为私有，而结构体中则为公有。**
```cpp
#include <iostream>
#include <string>
using namespace std;

struct Commodity {
    std::string name;
    int count;
    double price;
    std::string unit;
};

int main(int argc, char const* argv[])
{
    Commodity apple = {"apple", 1500, 8.50, "斤"};
    //Commodity apple {"apple", 1500, 8.50, "斤"};/* C++ 11 初始化 */
    cout << "仓库中存有" << apple.name << 
    " 一共 " << apple.count << apple.unit << 
    " 价值:" << apple.count * apple.price;
    return 0;
}
```
`可以在声明的时候定义一个变量`
```cpp
struct Commodity {
    std::string name;
    int count;
    double price;
    std::string unit;
}apple ;

apple = { "apple", 1200, 5.85, "斤" };
```

**结构体是可以直接使用赋值运算符 复制的**
```cpp
#include<iostream>

struct Commodity {
    std::string name;
    int count;
    double price;
    std::string unit;
};

int main(void){
    Commodity apple = {"apple", 1500, 8.50, "斤"};
    Commodity pear = apple;  /* 直接赋值哦 数组就不行  */
    pear.name = "pear";

    std::cout << apple.name << " price: " << apple.price << std::endl;
    std::cout << pear.name << " price: " << pear.price << std::endl;
    return 0;
}
```
**C++ 匿名结构体**

`如下所示，创建一个匿名结构体，并且获得一个实例`
```cpp
struct {
    std::string name;
    int count;
    double price;
    std::string unit;
}apple ;
```


#### [4.7 结构数组](#)
`注意下初始化就行了；`

```cpp
Commodity goods[] = {
    {"apple", 850, 8.52, "斤"},
    {"pear ", 1800, 6.53, "斤"}
};

for (int i = 0; i < 2; i++)
{
    cout << goods[i].name << " price: " << goods[i].price << endl;
}
```

#### [4.8 共用体](#)
`基本上，实践中就没用到过！` `不过其作用是用于硬件，节约内存。`

```cpp
union MyUnion
{
    int int_val;
    long long_val;
    double double_val;
};

int main(int argc, char const* argv[])
{
    MyUnion me;
    me.int_val = 95;
    me.double_val = 95.685;
    me.long_val = 95;
    return 0;
}
```

#### [4.9 枚举](#)
`C++的enum工具提供了另一种创建符号常量的方式，这种方式可以代替const`

```cpp
enum spectrum {red, orange, yellow, green, blue, violet, indigo, ultraviolet};

//spectrum 就是一个新的类型了
//red 0
//orange 1
int main(int argc, char const* argv[])
{
    spectrum color = blue;
    int val = color;
    //cant use spectrum.red is bad things;
    cout<< color << endl;
    cout<< val << endl;
}
```

`枚举类型虽然本质上是整数，但是不能用把枚举类型当做整数运算操作，例如`

```cpp
pectrum brand = yellow;
brand++; //语法错误
newColor = red + green; //非法操作
pectrum one = 5;//非法 int不能转换为枚举类型
```

`枚举量是整形，可以转化为int 类型，但是int类型不能自动转换为枚举类型。你可以认为int类型的级别要比枚举类型要高。可以升，但不可以降级`

`可以使用强制转换将整形转换为枚举类型！`
```cpp
spectrum kicker = spectrum(5);
```
`如果强制转换一个超过枚举取值范围，那么其值是不确定的！`
```cpp
spectrum kicker = spectrum(512123123);//结果不确定
```

`枚举默认是从 0 开始的，但是你也可以设置它的值`

`可以使用赋值运算符来显示地设置枚举量的值`
```cpp
enum TransactionType { CreditTransaction = 1, ElectricTransaction = 2, AllType = 4};
TransactionType type = ElectricTransaction;

cout<< type << endl;

TransactionType credit = TransactionType(5);

printf("credit value: %d\n",credit); //5
```
`只设置一些的后果`
```cpp
enum bits {zero, kicker = 400, yoke}
```
`zero 是 0 而 yoke 是 401`


#### [4.10 指针](#)
`声明和初始化指针, 指针不是整数，它是地址！` `C++ 和指针相关的运算符，修饰符有许多`

1. `&` `取地址运算符`
2. `*` `可以用来声明指针，也可以做乘法`
3. `*` `解除引用运算符 or 间接值运算符` `通过指针获取地址里面存储的值！`
4. `new` `用来分配内存`
5. `new DataType[]` `分配内存 数组类型`
6. `delete pointerName 释放内存`
7. `delete [] pointerName` `释放数组指针`
8. `const DataType * pointerName` `声明常量指针`
9. `DataType * const pointerName` `声明指针常量`

`声明一个指针`
```
DataType * pointerName;
```

```cpp
int number = 35;
int* pointer = &number;
cout << *pointer;//35

int age {56};
int *ptr_age =  &age;
cout << "age: " << *ptr_age << endl;
```

`使用new来分配内存` `c语言用 malloc 和 calloc 分配内容。C++ 提供了new 运算符` `同时使用 delete来释放内存， C语言使用 free！ 使用new分配的内存，只能用delete 释放，反之亦然！ 释放
只能一次。不要反复释放！`

```cpp
int* init = new int;
*init = 35;
cout << *init;//35
delete init;// 释放指针
```
`使用new创建动态数组并释放, 释放数组需要加上一个符号 [] 编程 delete [] pointer ;`

```cpp
int len = 3;
Commodity* goods = new Commodity[len];
goods[0] = {"apple", 1500,9.5,"斤"};
goods[1] = {"pear", 1500,9.5,"斤" };
goods[2] = {"huawei", 15000,5499,"件" };

for (int i = 0; i < len; i++)
{
    cout << goods[i].name << endl;
}
delete [] goods; //释放数组指针
```

`如果我们使用数组声明的方式创建数组，那么这样的内存分配方式为静态联编。如果我们使用new动态创建数组，这种方式称为动态联编。`

```cpp
#include <iostream>
#include <climits>

using namespace std;
int main()
{
    int length = 10;
    int* scores = new int[length];
    
    for (int i = 0; i < length; i++)
    {
        scores[i] = ((i +8 )*84654)%100;
    }

    for (int i = 0; i < length; i++)
    {
        printf("%d ", *(scores + i));
    }
    printf("\n"); //32 86 40 94 48 2 56 10 64 18 
    //free 
    delete [] scores;
    return 0;
}
```

**利用空指针初始化指针** **请记住，使用指针之前，一定要初始化，不然会发生不可预知的错误！**
```cpp
int* scores = nullptr;
if (scores == nullptr)
{
    scores = new int[length];
}
```

空指针：`nullptr` ` C++11 引入了全新的空指针。不推荐使用之前的 NULL`

`空指针不指向任何对象，在试图使用一个指针之前代码可以首先检查它是否为空。以下列出几个生成空指针的方法：`

1. `int* p1 = nullptr;` `这是c++11新引入的方法，nullptr是一个特殊的字面值，它可以被转换成任何其它的指针类型。`
2. `int* p2 = 0;`
3. `int* p3 = NULL` ,`这种方法必须首先包含头文件cstdlib。这种是使用预处理变量的一种方式，NULL会被替换为实际值，实际值是0.所以和方法（2）是一样的。`


`在C中，使用NULL表示空指针，实际上，NULL被定义为：`
```c
#define NULL ((void*) 0)
```
`其中((void *) 0)表示对’0’进行强制转换，转换为一个void类型的指针，用于表示空指针。即NULL表示一个指向地址’0’的void指针，将NULL赋予其他类型的指针时，会进行一个隐式转换，将void类型的指针转换为相应的类型。·

`在C++中（C++11之前） 禁止了C中指针的这种隐式转换，所以，在C++环境中，对NULL的定义为（在cstdlib中)：`
```cpp
#ifndef NULL
    #ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif
```
`也就是说，在C++中的NULL，直接被定义为了int类型的 0 `

**指针的大小，在一个OS中大小是一样的。32位系统指针大小就是32位，64位系统指针大小就是64位的！**

### [5. C++ 三种管理数据内存的方式](#) 
`根据用于分配内存的方式, C++ 三种管理数据内存的方式` `自动存储` `静态存储` `动态存储` `都是需要记住的基本概念！` `C++11 引入了线程存储！`

#### 5.1 自动存储
`在函数内部定义的常规变量使用自动存储空间，被称为自动变量，这意味着它们在所属的函数被调用时自动产生，在该函数结束时消亡` 
`自动变量通常存储在栈中，是一个局部变量，其作用域为包含他的代码块，这意味着执行代码块时，其中的变量将依次加入栈中，而离开代码块时，将按相反的顺序释放这些变量，这被称为“后进先出”。`

#### 5.2 静态存储
`静态存储是在整个程序执行期间都存在的存储方式。`  `使变量成为静态的方式有两种：一种是在函数外面定义它；另一种是在声明变量时使用关键字` `static`
```cpp
static double fee = 56.5;
```
`自动存储和静态存储的关键在于：这些方法严格限制了变量的寿命。变量可能存在于程序的整个生命周期（静态变量），也可能只是在特定函数被执行时存在（自动变量）。`

#### 5.3 动态存储
` new 和 delete运算符提供了一种比自动变量和静态变量更灵活的方法。它们管理了一> 个内存池，这在C++中被称为自由存储空间（free store）或堆（heap）。该内存池同> 用于静态变量和自动变量的内存是分开的`
`new和delete让您能够在一个函数中分配内存，又在另一个函数中释放他。因此数据的> 生命周期完全不受程旭或函数的生存时间的控制。`

`虽然使用动态存储方式使得程序员对内存使用有了更大的控制权。然而内存管理也更复杂了。怎么讲呢？在栈中，自动添加和删除机制使得占用的内存总是连续的，但
是new 和 delete 的相互影响可能导致占用的自由存储区不连续，这使得跟踪分配内存的位置更困难。`

### [6. C++ 数组的替代品](#)
`模板类vector和array是数组的替代品。 这些封装得非常好！`

#### 6.1 vector
`模板类vector类似于string类，也是一种动态数组。你可以在运行阶段设置 vector长度，可以在末尾附加新数据，还可以在中间插入数据。 它是使用new创建动态数组的替代品。他也是使用new和delete来管理内存。`
`不过内部实现已经帮你做了你应该做的事情。让你可以轻松使用。 他在std命名空间中...`  `会存储在堆中`

`向量（Vector）是一个封装了动态大小数组的顺序容器（Sequence Container）。跟任意其它类型容器一样，它能够存放各种类型的对象。可以简单的认为，向量是一个能够存放任意类型的动态数组。`

`vector：顺序容器（可变大小数组）。支持快速随机访问。在尾部之外的位置插入或删除元素可能很慢。既然vector是个容器，那么一定相关特性，如添加元素、删除元素和查询容器大小等操作。`

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

#### 6.1.2 vector 构造函数
* `vector():创建一个空vector`
* `vector(int nSize):创建一个vector,元素个数为nSize`
* `vector(int nSize,const t& t):创建一个vector，元素个数为nSize,且值均为t`
* `vector(const vector&):复制构造函数`
* `vector(begin,end):复制[begin,end)区间内另一个数组的元素到vector中`

#### 6.1.3 vector 插值API
* `void push_back(const T& x):向量尾部增加一个元素X`
* `iterator insert(iterator it,const T& x):向量中迭代器指向元素前增加一个元素x`
* `iterator insert(iterator it,int n,const T& x):向量中迭代器指向元素前增加n个相同的元素x`
* `iterator insert(iterator it,const_iterator first,const_iterator last):向量中迭代器指向元素前插入另一个相同类型向量的[first,last)间的数据`

#### 6.1.4 vector 删除函数
* `iterator erase(iterator it):删除向量中迭代器指向元素`
* `iterator erase(iterator first,iterator last):删除向量中[first,last)中元素`
* `void pop_back():删除向量中最后一个元素`
* `void clear():清空向量中所有元素`

#### 6.1.5 vector 判断函数
* `bool empty() const:判断向量是否为空，若为空，则向量中无元素`

#### 6.1.6 vector 大小函数
* `int size() const:返回向量中元素的个数`
* `int capacity() const:返回当前向量所能容纳的最大元素值`
* `int max_size() const:返回最大可允许的vector元素数量值`
* `reserve(n) 改变当前vecotr所分配空间的大小, 设置容量（capacity）`
* `resize()，设置大小（size）;`

`size()是分配容器的内存大小，而capacity()只是设置容器容量大小，但并没有真正分配内存。`

`打个比方：正在建造的一辆公交车，车里面可以设置40个座椅（reserve(40);），这是它的容量，但并不是说它里面就有了40个座椅，只能说明这部车`
`内部空间大小可以放得下40张座椅而已。而车里面安装了40个座椅(resize(40);)，这个时候车里面才真正有了40个座椅，这些座椅就可以使用了`

#### [C++ vector中的resize，reserve，size和capacity函数讲解](#)
* `capacity 指容器在分配新的存储空间之前能存储的元素总数。`
* `size 指当前容器所存储的元素个数`
* `resize 既分配了空间，也创建了对象。 这里空间就是capacity，对象就是容器中的元素。`
* `reserve()表示容器预留空间，但不是真正的创建对象，需要通过insert()或push_back()等操作创建对象 `

#### 6.1.7 vector 其他函数

* `void swap(vector&):交换两个同类型向量的数据`
* `void assign(int n,const T& x):设置向量中前n个元素的值为x`
* `void assign(const_iterator first,const_iterator last):向量中[first,last)中元素设置成当前向量元素`

#### 6.1.8 vector 遍历函数

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
#### 6.1.9 二维数组两种定义方法 

```cpp
unsigned char row = 5, column = 5;
    vector<vector<float>> twoWayFoil(row);
    
    for (int i = 0; i < twoWayFoil.size(); i++)
    {
        twoWayFoil[i].resize(column);
    }

    /* 这里可以初始化 设置值 */

    for (int i = 0; i < twoWayFoil.size(); i++)//输出二维动态数组 
    {
        for (int j = 0;j < twoWayFoil[i].size();j++)
        {
            cout << twoWayFoil[i][j] << " ";
        }
        cout << "\n";
    }

    twoWayFoil.clear();
    /* 默认输出全 0 */
```

#### [6.2 array](#)
`模板类 array C++11 提供的！` `std::array 相对于 std::vector 而言，提供了静态数组，编译时确定大小、更轻量、更效率，当然也比 std::vector 有更
多局限性。但是相对于 C array 而言，又更加安全，更加方便，提供了 STL 的各种迭代器、算法、操作方法等，用起来有现代语言的快感。`

`数组类是固定大小的序列容器，它们包含以严格线性序列排序的特定数量的元素。数组类具有固定大小，并且不通过分配器管理其元素的分配，它们是封装固定大小元素数组的聚合类型。`

`会存储在栈中`

`array模板类的声明`
```cpp
template <class T, size_t N> class array;
```
* `序列容器中的元素按严格的线性顺序排序。各个元素按其顺序访问它们的位置。`
* `元素存储在连续的存储器位置，允许对元素进行恒定时间随机访问。可以偏移元素的指针以访问其他元素。`
* `容器使用隐式构造函数和析构函数静态分配所需的空间。它的大小是编译时常量。没有内存或时间开销。`

```cpp
std::array<std::string, 5>  names = {"彼得大帝","叶卡捷琳娜大帝", "彼得二世", "亚历山大一世", "普京大帝"}; /* = 可以省略 */

for (auto it = names.begin(); it != names.end(); ++it)
{
    std::cout << *it << " \n";
}
```
`array数组容器的大小是固定的, 可以通过sizeof()、size()、max_size()、empty()等函数进行检测。`

`可以通过下标[ ]、at()、front()、back()、data()等函数访问array容器内的元素。`

`可以使用fill()、swap()等函数对array容器整体进行操作。`

-----
`时间`: `[2022-3-11 11:07]` 