### [C++ 字符串处理](#)
字符串处理是程序几乎必需的日常过程！ 在C++中C语言提供了 cstring 库用于字符串处理，C++提供了 string库！

-----
- [x] [1. C++ 字符串环境](#1-c-字符串环境)
- [x] [2. 字符串库](#2-字符串库)
- [x] [3. string类](#3-string类)
- [x] [4. 字符串检测和转换](#4-字符串检测和转换)

-----

> * [彻底弄懂 Unicode 编码](https://blog.csdn.net/hezh1994/article/details/78899683)
>
> * [C++ 字符编码转换之UTF-8/UTF-16/UTF-32](https://blog.csdn.net/S676951/article/details/123165202)

字符串编码规则如下：

|Unicode 十六进制码点范围|	UTF-8 二进制|
|:---|:---|
|0000 0000 - 0000 007F|	0xxxxxxx|
|0000 0080 - 0000 07FF|	110xxxxx 10xxxxxx|
|0000 0800 - 0000 FFFF|	1110xxxx 10xxxxxx 10xxxxxx|
|0001 0000 - 0010 FFFF|	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx|

> “汉”的 Unicode 码点是 0x6c49（110 1100 0100 1001），通过上面的对照表可以发现，`0x0000 6c49` 位于第三行的范围,
>
>  那么得出其格式为 `1110xxxx 10xxxxxx 10xxxxxx`。
>
> 接着，从“汉”的二进制数最后一位开始，从后向前依次填充对应格式中的 x，多出的 x 用 0 补上。
>
> “汉”的 UTF-8 编码为 `11100110 10110001 10001001`，转换成十六进制就是 `0xE6 0xB7 0x89`。

### [1. C++ 字符串环境](#)

C 风格的字符串起源于 C 语言，并在 C++ 中继续得到支持。字符串实际上是使用 \0 字符为终止的一维字符数组。因此，字符串一个以 \0 结尾的字符串，包含了组成字符串的字符。
C语言提供了 cstring 库用于字符串处理，里面有strlen(s1) strcmp(s1,s2) strcat() strcpy....等等API， C++ 标准提供了 string 类统一处理字符串！使用 string 类需要包含头文件。

```cpp
#include <string>
```
很多开源C++库会自己实现一套字符串库，例如  fbstring  QString CString StringPiece FBString。
真实世界的程序里的字符串，有这些问题

* 多种编码。
* 变长编码。
* 一些特定关键字可能非常高频。 

然而C++标准库的string，对这些事情全都没有处理。

* 没有编码的概念。
* 没有变长的概念。 几乎就是个char vector或者wchar vector。
* 常亮引用什么的放弃治疗，完全依赖C++的引用语义。
* std::string 是「字节串」而非「字符串」! std string说到头只是一个轻量级的字节数组容器！

如下所示，四个字符，长度是12。每个字符长度为3个字节！
```cpp
std::string tm("我的天啦");
std::cout<< tm.length() << '\n'; //12

std::u16string u16tm(u"我的天啦");
std::cout<< u16tm.length()<< '\n'; //4
```

C++ 字符串库支持三种通用字符串类型：

#### [1.1 FBString](#)
FBString 是 facebook 开源 C++ lib folly 的一个 std::string 替换着，如果你想更好的性能，又不想重写，那么 FBString 是一个不错的选择，Facebook 对其做了大量的性能优化，其核心特点如下

* 100% 兼容 std::string
* 高性能的内存管理，对不同大小的字符串， 分 3 种情况分配和管理内存：（1）string length <= 23，使用栈上内存；（2）23< string length <=255，使用 malloc 分配堆上内存；（3）string lenght > 255，堆上内存，且使用引用计数 + COW 计数，实现在真正需要的时候才拷贝修改，避免大字符串不必要的拷贝
* ind 操作使用了 Boyer Moore [3] 算法，相比 O(n) 的 KMP 实际性能好很多，Facebook 官方宣称性能：Casual tests indicate a 30x speed improvement overstring::find()for successful searches and a 1.5x speed improvement for failed searches

### [2 字符串库](#)
C++ 字符串库支持三种通用字符串类型：

|类型|说明|支持性|
|:---|:---|:---|
|td::basic_string|为操作任何字符类型的字符串设计的模板类|
|std::basic_string_view|对于字符串子序列的轻量无所有权的只读视图	C++17|
|空终止字符串|以特殊的空字符终止的字符数组|

模板类 std::basic_string 通用化字符序列如何操作及存储。字符串创建、操作及析构全部为便利的类方法集合与相关函数所处

#### [2.1 C++ 支持的字符类型](#)
std::basic_string 标准提供 std::basic_string 对常用类型的数种特化, 定义于头文件 <string> 
首先你要明白C++不仅支持 unsigned char 和 signed char， 还支持宽字符类型wchar_t，还支持char16_t char32_t。 所有对于C++而言，处理字符串如下类:

|类型|说明|
|:---|:---|
|std::string|typedef std::basic_string<char>|
|std::wstring|typedef std::basic_string<wchar_t>|
|std::u8string (C++20 起)|typedef std::basic_string<char8_t>|
|std::u16string (C++11 起)|typedef std::basic_string<char16_t>|
|std::u32string (C++11 起)|typedef std::basic_string<char32_t>|


#### [2.2 std::basic_string_view](#)
模板类 std::basic_string_view 提供一个轻量对象，它使用类似 std::basic_string 接口的接口，提供对字符串一部分的只读访问。
标准提供 std::basic_string_view 对常用类型的数种特化：定义于头文件 <string_view>

|类型|说明|
|:---|:---|
|std::string_view (C++17 起)|std::basic_string_view<char>|
|std::wstring_view (C++17 起)|std::basic_string_view<wchar_t>|
|std::u8string_view (C++20 起)|std::basic_string_view<char8_t>|
|std::u16string_view (C++17 起)|std::basic_string_view<char16_t>|
|std::u32string_view (C++17 起)|std::basic_string_view<char32_t>|

#### [2.3 空终止字符串](#)
空终止字符串是以特殊的空字符终止的字符数组。C++提供创建、检查及修改空终止字符串的函数。  空终止字符串有三种类型：

* 空终止字节字符串
* 空终止多字节字符串
* 空终止宽字符串

空终止字节字符串（ null-terminated byte string, NTBS ）是后随拥有零值的字节（空终止字符）的非零字节序列。字节字符串的每个字节编码某字符集的一个字符。例如，字符数组 {'\x63', '\x61', '\x74', '\0'} 是持有以 ASCII 编码的字符串 "cat" 的 NTBS 


### [3. string类](#) 
学会 string 类，其他的 wstring u8string u16string u32string 也就懂了，本质上差不多！
string类本质上是 basic_string<char> 的一个 typedef！ size_type 是一个依赖于实现的整形，在头文件 string.h中定义！

* npos[静态] 特殊值。准确含义依赖语境(公开静态成员常量)

#### [3.1 string 构造函数](#)
常用的构造函数， 复制构造函数、移动构造函数就不谈了！

|构造函数|说明|
|:---|:---|
|string()|创建一个长度位0的空字符串，默认构造函数|
|string(const char* s)|将string对象初始化位以空终止字符串结尾的字符串！(NBTS)|
|string(size_type n, char c)|包含n个元素的string对象，其中每个元素都被初始化位字符 c|
|string(const char * s, size_type n)|取字符串s的前n个字符构成 以空终止字符串结尾的字符串！(NBTS)|
|string(initializer_lists<char> il)|这是C++ 11新增！|
|template<class Iter> string(Iter begin, Iter end)|将string对象初始化为区间 [begin, end]内的字符串， 他们本质上是指针|


```cpp
//这里必须加 const 哦！
const char *name =  "remix" ;
std::string remix(name);
std::cout << remix << '\n';


std::string thk(5, 'k');
std::cout << thk << '\n'; //kkkkk

char lname[] = {"cnm-you!"};
std::string ln(lname, 3);
std::cout <<  ln << "\n";

const char *ben = "I wanna kill your families!";
std::string gime(ben +2, ben +8);
std::cout <<  gime << "\n";//wanna
```

#### [3.2 重载运算符](#)
C++ 重载了许多运算符

|重载函数|说明|
|:---|:---|
|operator=|为字符串赋值(公开成员函数)|
|operator+|连接两个字符串或者一个字符串和一个字符(函数模板)|
|operator[]|访问指定字符(公开成员函数)！|
|operator==|比较啊两个字符串是否相同！|

```cpp
std::string message_header("Title:");
std::string message_content("Contract Information!");

std::string message = message_header + message_content;

printf("%s\n", message.c_str());
printf("message[0] = %c\n", message[0]);
```

#### [3.3 容量相关](#)
字符串判断是否为空，长度，最大长度，容量！

* empty() 检查字符串是否为空 (公开成员函数) ——判空（空返回1，非空返回0）
* size() 返回字符数(公开成员函数)
* length()  返回字符数(公开成员函数)
* max_size()  返回字符数的最大值 (公开成员函数)
* reserve()  保留存储(公开成员函数)
* capacity() 返回当前对象分配的存储空间能保存的字符数量 (公开成员函数)
* shrink_to_fit() (C++11) 通过释放不使用内存减少内存使用
* str.clear()——删除 str 所有

#### [3.4 其他函数](#)

* str.find(‘A’)——查找 ‘A’
* str.find(“ABC”)——查找 “ABC”
* str.rfind(‘A’)——查找 ‘A’
* str.rfind(“ABC”)——查找 “ABC”
* str.assign(“ABC”)——清空字符串，并设置为 “ABC”
* str.assign(“ABC”,2)——清空字符串，并设置为"AB"，保留两个字符
* str.assign(“ABC”,1,1)——清空字符串，设置为 “ABC” 中的从 位置1 开始，保留 1个 字符
* str.assign(5，‘A’)——清空字符串，然后字符串设置为 5个 ‘A’
* str.length()——求字符串长度
* str.size()——和 length() 一样
* str.capacity()——获取容量，包含了不用增加内存就能使用的字符数
* str.reasize(10)——设置当前 str 的大小为10，若大小大于当前串的长度，\\0 来填充
* str.reasize(10,char c)——设置当前 str 的大小为10，若大小大与当前串的长度，字0符c 来填充
* str.reserve(10)——设置str的容量 10，不会填充数据
* str.swap(str1)——交换 str1 和 str 的字符串
* str.push_back(‘A’)——在str末尾添加一个字符 ‘A’ ，参数必须是字符形式
* str.erase(2)——删除 下标2 的位置开始，之后的全删除
* str.erase(2,1)——删除 下标2 的位置开始，之后的 1个 删除
* **compare** 比较二个字符串

#### [3.5 c_str](#)
c_str() 函数可以将 const string* 类型 转化为 cons char* 类型。 因为在c语言中没有string类型，必须通过string类对象的成员函数 c_str() 把 string 转换成c中的字符串样式。

**c_str() 这个函数转换后返回的是一个临时指针，不能对其进行操作**，所以因为这个数据是临时的，所以当有一个改变这些数据的成员函数被调用后，该数据就会改变失效；

```cpp
#include<iostream>
#include<cstring>
using namespace std;

int main() {
	const char *ptr;
	string s = "12345";
    ptr = s.c_str();
    cout << "s改变前ptr为：" << ptr << endl;
    s = "66666";
    cout << "s改变后ptr为：" << ptr << endl;
    return 0;
}
```
**因此要么直接将这个数据应用或输出，要么把它的数据用 strcpy() 函数复制到自己可以管理的内存中；**

```cpp
#include<iostream>
#include<cstring>
using namespace std;

int main() {
	char ptr[5];
	string s = "12345";
	strcpy(ptr, s.c_str());
	cout << "s改变前ptr为：" << ptr << endl; //"12345"
	s = "66666";
    cout << "s改变后ptr为：" << ptr << endl; //"12345"
	return 0;
} 
//这就不会再出现上面的问题了；
```

#### [3.6 未定义行为](#)
如果讲一个没有以空终止字符串结尾的字符串赋值给字符串类型就导致未定义行为。

```cpp
char notEmpty[] = {'A', 'L'};

std::string s(notEmpty, 2); //正确 字符串为 "AL"
std::string s(notEmpty); //错误 未定义行为
```

#### [3.7 substr](#)
返回字符串的子串，第一个参数是位置，第二个参数是长度。
```cpp
constexpr basic_string substr( size_type pos = 0, size_type count = npos ) const;
```

```cpp
std::string message = { "fuck-the-world!" };
std::string s = message.substr(0,5);

std::cout << s << "\n"; //fuck-
```

#### [3.8 insert](#)
用于字符串插值，有多个重载版本！

* str.insert(2,3,‘A’)——在str下标为2的位置添加 3个 字符’A’
* str.insert(2,"ABC")——在str下标为2的位置添加 字符串 “ABC”
* str.insert(2,"ABC",1)——在str下标为2的位置添加 字符串 “ABC” 中 1个 字符
* str.insert(2,"ABC",1,1)——在str下标为2的位置添加 字符串 “ABC” 中从位置 1 开始的 1 个字符
* str.insert( iterator pos, size_type count, CharT ch )——在 str 中，迭代器指向的 pos位置 插入 count个 字符 ch
* str.insert(++str1.begin(),2,‘a’); 结果：s4：ABCD -> AaaBCD
* str.insert( iterator pos, InputIt first, InputIt last )——在 str 中，pos位置 插入 str1 的 开始位置 到 结束为止
* str.insert(s4.begin(),str1.begin(),str1.end()); 结果：s4：ABCD str1：abc -> abcABCD

```cpp
std::string str{ "雷神值得补二命嘛？" };
std::string in{ "她说:" };
str.insert(0,in, 0, in.size());

std::cout << str << "\n";
//她说:雷神值得补二命嘛？
```

#### [3.9 append](#)
后附字符到结尾，高达9个重载版本。

* str.append("ABC")——在str末尾添加一个字符串 "ABC"，参数必须是字符串形式

```cpp
std::string str{ "雷神值得补二命嘛？" };
std::string in{ "她说:" };
std::cout << in.size() << "\n"; //5
in.append(str);

std::cout << str << "\n";
//她说:雷神值得补二命嘛？
```

#### [3.10 replace](#)
用于内容替换，高达9个重载大版本。

* str.replace(2,4,“abcd”)——从下标2 的位置，替换 4个字节 ，为"abcd"

```cpp
std::string str{ "C++ Primer Plus 6th Ed." };

str.replace(11, 6, "5");

std::cout << str << "\n";//C++ Primer 5th Ed.
```

#### [3.11 字符串转数值](#)
* stoi(s,p,b) 把字符串s从p开始由b进制转化为十进制的int
* stol(s,p,b) 把字符串s从p开始由b进制转化为十进制的long
* stoul(s,p,b) 把字符串s从p开始由b进制转化为十进制的unsigned long
* stoll(s,p,b) 把字符串s从p开始由b进制转化为十进制的long long
* stoull(s,p,b) 把字符串s从p开始由b进制转化为十进制的unsigned long long
* stof(s,p) 把字符串s从p开始转换成float
* stod(s,p) 把字符串s从p开始转换成double
* stold(s,p) l把字符串s从p开始转换成long double

```cpp
std::string str{ "3.1415" };
double pi = std::stod(str);

std::cout << pi * 100 << "\n";//314.15
```

### [4. 字符串检测和转换](#)
C语言有一个头文件，**cctype** 在C++中被保留了下来，C++还内置了许多类型换行和检测的函数API！

#### [4.1 cctype](#)
C++从C语言继承了一个与字符相关的、非常方便的函数软件包，它可以简化诸如确定字符是否为大写字母、数字、标点符号等工作，这些函数的原型是在头文件cctype(老式的风格中为ctype.h)中定义!

* isalnum(）	如果参数是字母数字,即字母或数字,该函数返回true
* isalpha()	如果参数是字母,该函数返回true
* iscntrl()	如果参数是控制字符,该函数返回true
* isdigit0	如果参数是数字(0～9），该函数返回 true
* isgraph()	如果参数是除空格之外的打印字符,该函数返回 true
* islower()	如果参数是小写字母,该函数返回 true
* isprint()	如果参数是打印字符（包括空格）,该函数返回 true
* ispunct()	如果参数是标点符号,该函数返回true
* isspace(）	如果参数是标准空白字符,如空格、进纸、换行符、回车、水平制表符或者垂直制表符，该函数返回true
* isupper()	如果参数是大写字母,该函数返回true
* isxdigit()	如果参数是十六进制数字，即0~9、a～f或A～F，该函数返回 true
* tolower()	如果参数是大写字符,则返回其小写,否则返回该参数
* toupperO	如果参数是小写字符,则返回其大写,否则返回该参数

#### [4.2 C++ 11提供的类型转换](#)
c++11标准增加了全局函数 用于类型转换

* string to_string (int val);
* string to_string (long val);
* string to_string (long long val);
* string to_string (unsigned val);
* string to_string (unsigned long val);
* string to_string (unsigned long long val);
* string to_string (float val);
* string to_string (double val);
* string to_string (long double val);


#### [4.3 大小写转换](#)

```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;


int main(){
    string str = "helloWORLD";
    cout << "转换前：" << str << endl;
    
    //全部转换为大写
    transform(str.begin(), str.end(), str.begin(), ::toupper);    
    cout << "转换为大写：" << str << endl;    
    
    //全部转换为小写
    transform(str.begin(), str.end(), str.begin(), ::tolower);    
    cout << "转换为小写：" << str << endl; 
    
    //前五个字符转换为大写
    transform(str.begin(), str.begin()+5, str.begin(), ::toupper);
    cout << "前五个字符转换为大写：" << str << endl; 
    
    //后五个字符转换为大写
    transform(str.begin()+5, str.end(), str.begin()+5, ::toupper);
    cout << "前五个字符转换为大写：" << str << endl; 
    
    return 0;
}
```


-----
时间: [] 