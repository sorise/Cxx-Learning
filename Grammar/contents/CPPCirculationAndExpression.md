### [C++ 循环和关系表达式](#)

**介绍**：列举C++的程序流程控制、基本运算符！

-----

- [ ] [1. for 循环](#1-for-循环)
- [ ] [2. while 循环](#2-while-循环)
- [ ] [3. if else](#3-if-else)
- [ ] [4. switch](#4-switch-表达式)
- [ ] [5. 运算符](#5-运算符)
- [ ] [6. cctype 字符函数库](#6-cctype-字符函数库)
- [ ] [7. eof fail](#7-eof-fail)
- [ ] [8. typedef 类型别名](#8-typedef-类型别名)
- [ ] [9. constexpr和常量表达式](#9-constexpr和常量表达式)
- [ ] [10. 简单文件输入输出](#10-简单文件输入输出)

---

### [1. for 循环](#)
很简单的， 记得三个事物，初始化，测试条件，更新测试条件。
```cpp
for (initialzation;test-expression; update-expression)
{
    body
}

float scores[5] = { 68.5, 56,94,86,88 };
for (int i = 0; i < 5; i++)
{
    cout << scores[i] << endl;
}
```

* **break**: 直接结束整个循环
* **continue**: 直接结束本轮循环，然后指向 update-expression


#### [1.1 for-c++11](#)
C++ 新增了一种循环： 基于范围的for循环。这简化了一种常见的循环任务，对数组，容器类执行操作。C++ 11 也支持类似于 foreach 的for循环用于遍历数组，如果你不需要改变起值，你可以使用如下形式。

```cpp
float scores[5] = { 68.5, 56,94,86,88 };

for (float number: scores)
{
    if (number > 90)
    {
        std::cout<<"exist high score more than 90! the value is " << number << " ." << std::endl;
        break; //找到就结束 for 循环
    }
}
```

#### [1.2 for-引用方式](#)
如果你需要修改值，那么你可以获得值的引用。

```cpp
float scores[5] = { 68.5, 56.5,94.0,86,88 };

for (float  &x: scores)
{
    x = x * 0.9;
    cout << "value: "<< x <<endl;
}
/*
value: 61.65
value: 50.85
value: 84.6
value: 77.4
value: 79.2
*/
```
也可以使用auto 进行简写
```cpp
int my_array[5] = {1, 2, 3, 4, 5};
// auto 类型也是 C++11 新标准中的，用来自动获取变量的类型
for (auto &x : my_array) {
    x *= 2;
    cout << x << endl;  
}
```

#### [1.3 for循环遍历字符串](#)
C++ 老版本for循环
```cpp
string message = "this is a message!";
for (int i = 0; i <= message.size(); i++)
{
    printf("%c", message[i]);
}
printf("\n");
```

C++ 11 for循环
```cpp
short idx = 0;
string message = "this is a message!";
for (auto c : message)
{
    printf("[%d]:%c \n", idx++, c);
}
```

#### [1.4 break continue](#)
* break: 直接结束整个循环
* continue: 直接结束本轮循环，然后指向 update-expression

可以用于 for  while switch


### [2. while 循环](#)
while 循环是没有初始化和更新部分的for循环，他只有测试条件和循环体
```cpp
while(test-condition){
    body
}

float scores[5] = { 68.5, 56.5,94.0,86,88 };

int i = 0;
while (i < 5)
{
    cout << scores[i++] << endl;
}
```

#### [2.1 do while](#)
和前面一样嘛, 先执行循环体，再执行判断条件
```cpp
float scores[5] = { 68.5, 56.5,94.0,86,88 };
int i = 0;
do {
    cout << scores[i++] << endl;
} while (i < 5);
```

#### [2.2 while实现延时循环](#)
ANSI C 和 C++ 中的clock函数返回程序开始执行后所用的系统时间，clock 函数返回的时间不一定是秒，且返回类型不定 可能是 long 也可能是 long long 也可能是 unsigned long.... 所以
头文件ctime提供了解决方案，定义了一个符号常量 CLOCKS_PER_SEC。等于每秒钟包含的系统时间单位数(时间节拍)，将系统时间乘以这个值就可以得到秒数。其次 clock_t作为clock返回类型的别名！这意味
可以将变量声明位 clock_t 类型！

```cpp
#include <iostream>
#include <ctime>

using namespace std;
int main()
{
    clock_t delay = 5 * CLOCKS_PER_SEC; //延迟多少秒
    clock_t start = clock();
    while (clock() - start <delay);
    
    cout <<"来自程序运行五秒之后的消息"<< endl;
    return 0;
}

```

### [3. if else](#)
单分支多分支，分支逻辑运算 ，很简答

```cpp
int main(int argc, char const* argv[])
{
    bool flag = true;
    if (flag) {
        cout << "条件为真" << endl;
    }
    else {
        cout << "条件为假" << endl;
    }

    int value = 25;
    if (value < 18)
    {
        cout << "少年人" <<endl;
    }
    else if (value >= 18 && value < 40)
    {
        cout << "壮年" << endl;
    }
    else {
        cout << "优化年龄" << endl;
    }
    return 0;
}
```

### [4. switch 表达式](#)
C++ 有一个常用来替代if else 的运算符就是 Switch。他 **只支持结果为一个整数的表达式**。

```cpp
switch(integet-expression){
    case label1:
      statement(s);
      break;
    case label2:
      statement(s);
      break;
    default: statement(s);
}
```

### [5. 运算符](#)
和各类语音大同小异，都很简答 在C++种运算符具有两种性质：优先级和结合性，优先级是一种运算符相比于其他运算符的运算优先级，结合性是运算符对操作数的处理流程！ 当然还有些其他情况，例如隐式类型转换之类的问题！ 例如 整数 = 整数/整数；  浮点数 = 浮点数 / 整数;

+ 就是有输入两个操作数然后返回一个操作数  并且运算符是可以使用 operator 进行运算符重载的！


#### [5.1 一元运算符](#)
一元运算符主要是 ++ -- 运算符。性质和C语言 JS语言，C#， Java 差不多！ 放在变量前后有所差别！ 关键在于区别其对于数值变量和指针变量的运算结果不同！

|运算符|说明|
|:---|:---|
|--|--age 在前 先减一 在返回其值  age-- 在后 先返回其值 再减一|
|++ --|++age 在前 先加一 在返回其值  age++ 在后 先返回其值 再加一|

++ -- 可以操作整数，也可以进行浮点数运算，还可以操作指针，他可以使得指针移动一个指向类型的存储单位！ 需要区分 指针的几种情况，如下所示！ 但是实际应该避免如此使用！
```cpp
#include <iostream>
#include <climits>

using namespace std;
int main()
{
    float val = 56.52f;
    val++;
    int age = 28;
    age++; //age : 29
    float scores[5] = { 68.5, 56.5,94.0,86,88 };

    float * pf = scores; //point at scores[0]

    pf++; //
    std::cout << *pf << std::endl;///56.5  point at scores[1]
    pf++; //
    std::cout << *pf << std::endl;///94.0 point at scores[2]
    ++*pf;
    std::cout << *pf << std::endl;///95.0 
    *pf++; //get 95 and chang pointer
    std::cout << *pf << std::endl;///86
    (*pf)++;
    std::cout << *pf << std::endl;///87

    std::cout << val << std::endl;//57.52
    std::cout << age << std::endl;//29
    std::cout << *pf << std::endl;//56.5
    return 0;
}
```

#### [5.2 算数和组合运算符](#)
+ - * / % 这类很简单的运算符，还有他们的简写方式！ += /= ...

|运算符|说明|
|:---|:---|
|+ - * / %|加减乘除 取余操作|
|+=| age += 2|
|-=| age -= 2 |
|*=| age *= 2|
|/=| age /= 2|
|%=| age %= 2|
|=|赋值运算符|
|&=|按位与且赋值运算符	C &= 2 等同于 C = C & 2|
|^=|按位异或且赋值运算符	C ^= 2 等同于 C = C ^ 2|
|\|=|按位或且赋值运算符	C \|= 2 等同于 C = C \| 2|

#### [5.3 关系表达式](#)
用于逻辑判断，如果你需要比较字符串，那么可以用 strcmp 函数来比较，但是需要引入 **#include<cstring>** ！

|运算符|说明|
|:---|:---|
|<|小于|
|<=|小于等于|
|>|大于|
|>=|大于等于|
|==|等于|
|!=|不等于|


#### [5.4 逻辑与位运算符](#)
|| &&  !  :? 

|运算符|说明|
|:---|:---|
||||表示 或 or的意思|
|&&|表示 和 and的意思|
|!|取反操作|
|:?|三元表达式，条件运算符|

:?为条件运算符， easy！
```cpp
bool sex = true;
unsigned short age = 24u;

if (sex && age >= 18)
{
    cout << "成年男性!" << endl;//成年男性
}
```
使用条件运算符完成上述代码！
```cpp
cout << ((sex&&age>=18)?"成年男性!":"不符合要求") << endl;//成年男性
```

#### [5.5 位运算](#)
位运算符作用于位，并逐位执行操作。&、 | 和 ^ 的真值表如下所示： << 左移运算符 >> 右移运算符！

|运算符|说明|
|:---|:---|
|||位或运算符|
|&|位与运算符|
|~|位取反运算符|
|^|位异或运算符|
|>>|位右移运算符！|
|<<|位左移运算符|

假设如果 A = 60，且 B = 13，现在以二进制格式表示，它们如下所示：

```cpp
#include <iostream>
using namespace std;
 
int main()
{
   unsigned int a = 60;      // 60 = 0011 1100  
   unsigned int b = 13;      // 13 = 0000 1101
   int c = 0;           
 
   c = a & b;             // 12 = 0000 1100
   cout << "Line 1 - c 的值是 " << c << endl ;
 
   c = a | b;             // 61 = 0011 1101
   cout << "Line 2 - c 的值是 " << c << endl ;
 
   c = a ^ b;             // 49 = 0011 0001
   cout << "Line 3 - c 的值是 " << c << endl ;
 
   c = ~a;                // -61 = 1100 0011
   cout << "Line 4 - c 的值是 " << c << endl ;
 
   c = a << 2;            // 240 = 1111 0000
   cout << "Line 5 - c 的值是 " << c << endl ;
 
   c = a >> 2;            // 15 = 0000 1111
   cout << "Line 6 - c 的值是 " << c << endl ;
 
   return 0;
}
```

#### [5.6 成员运算符](#)
主要用于访问类或者结构体的成员！ **ptr->member 等价于 (\*ptr).member**

|运算符|说明|
|:---|:---|
|.|点运算符获取类对象或者结构体对象的成员|
|->|和指针有关，通过指向类或者结构体的指针访问他们的成员|

```cpp
string message { "this is a message from jx" };

string *ptr = &message;
auto len = message.size();//获得字符串长度
auto len_t = ptr->size();//获得字符串长度
// ptr->size() 等价于 (*ptr).size();
```

#### [5.7 sizeof运算符](#)
sizeof运算符返回一条表达式或者一个类型名字所占的字节数！满足右结合律，返回的值类型为： size_t

```cpp
sizeof(type)
sizeof expr
```

#### [5.8 逗号运算符](#)
使用逗号运算符的为了把几个表达式串在一起。整个逗号表达式的值是以逗号分隔的列表中的最后一个表达式的值。从本质上讲，逗号的作用是导致一系列运算被顺序执行。

```cpp
auto v = (count=19, incr=10, count+1);
```



#### 5.9 三向比较运算符

两个比较的操作数可能产生三种结果！三向比较运算符 `<=>` 通常也被称为 spaceship 运算符，它能够确定两个值 A 和 B 之间的大小和相等关系，C++20使用三向比较运算符能够大大提高编程效率。

使用单个表达式，它可以告诉你一个值是否等于、小于或大于另一个值。因为它必须返回的不仅是true或者false，所以它不能范围布尔类型。相反，他返回类枚举（enumeration-like）类型(不是真正的[枚举类型](https://so.csdn.net/so/search?q=枚举类型&spm=1001.2101.3001.7020)，不能应用于switch)，定义在和std名称空间中。



**如果操作数是整形**，结果是所谓的强排序，并可以是一下之一。
* strong_ordering::less 第一个操作数小于第二个操作数
* strong_ordering::greater 第一个操作数大于第二个操作数
* strong_ordering::equal 第一个操作数等于第二个操作数, 任何情况下都可以相互替换！

```cpp
int i{11};
strong_ordering result{i <=> 0};
if(result == strong_ordering::less){ cout<< "less" << endl; }
if(result == strong_ordering::greater){ cout<< "greater" << endl; }
if(result == strong_ordering::equal){ cout<< "equal" << endl; }
```

**浮点型操作数**: 如果操作数是浮点型，结果是一个偏序。
* partial_ordering::less 第一个操作数小于第二个操作数
* partial_ordering::greater 第一个操作数大于第二个操作数
* partial_ordering::equivalent第一个操作数等于第二个操作数，不能相互替换
* partial_ordering::unordered 两个操作数中有非数字

```cpp
int i{5.5};
partial_ordering result{i <=> 1.1};
if(result == partial_ordering::less){ cout<< "less" << endl; }
if(result == partial_ordering::greater){ cout<< "greater" << endl; }
if(result == partial_ordering::equivalent){ cout<< "equivalent" << endl; }
```

**自定义类型**：还有一种弱排序，针对自己定义的类型
* weak_ordering::less 第一个操作数小于第二个操作数
* weak_ordering::greater 第一个操作数大于第二个操作数
* weak_ordering::equivalent第一个操作数等于第二个操作数，比较虽然相等，但是一般不能相互替换，例如字符串比较、两个对象比较。

```cpp
class ClassA
{
public:
	ClassA() {
		data = 0;
	};
	weak_ordering operator<=>(const ClassA& classA) {
		if (this->data < classA.data) {
			return weak_ordering::less;
		}
		if (this->data > classA.data) {
			return weak_ordering::greater;
		}
		if (this->data = classA.data) {
			return weak_ordering::equivalent;
		}
	}
public:
	int data;
};
int main()
{
	ClassA a;
	a.data = 3;
	ClassA b;
	b.data = 2;
	auto result = a <=> b;
	if (result == weak_ordering::less) { cout << "less" << endl; }
	if (result == weak_ordering::greater) { cout << "greater" << endl; }
	if (result == weak_ordering::equivalent) { cout << "equal" << endl; }
}

```

对于原始类型，与仅使用==、<和>运算符进行单个比较相比，使用三向比较运算符不会带来太多收益。但是，它对于比较昂贵的对象很有用。
提供命名的比较函数来解释排序结果，这些函数是std::is_eq()、is_neq()、is_lt()、is_gt()以及is_gteq()。函数返回类型为布尔。

**不支持数组比较**：

```cpp
char arr1[5];
char arr2[5];

auto result = arr1 <=> arr2;
```


### [6. cctype 字符函数库](#)

C++ 从C语言继承了一个与字符相关的，非常方便的函数软件包

* isalnum()  如果参数是字母数字，即字母或者数字，函数返回true
* isalpha()  如果参数是字母，函数返回true
* iscntrl()  如果参数是控制字符，函数返回true
* isdigit()  如果参数是数字（0－9），函数返回true
* isgraph()  如果参数是除空格之外的打印字符，函数返回true
* islower()  如果参数是小写字母，函数返回true
* isprint()  如果参数是打印字符（包括空格），函数返回true
* ispunct()  如果参数是标点符号，函数返回true
* isspace()  如果参数是标准空白字符，如空格、换行符、水平或垂直制表符，函数返回true
* isupper()  如果参数是大写字母，函数返回true
* isxdigit() 如果参数是十六进制数字，即0－9、a－f、A－F，函数返回true
* tolower()  如果参数是大写字符，返回其小写，否则返回该参数
* toupper()  如果参数是小写字符，返回其大写，否则返回该参数


```cpp
unsigned char ch = 'a';
unsigned char ni = '1';
if (isalnum(ch))
{
    cout << "是一个字母或数字"<<endl;
}

if (isdigit(ni))
{
    cout << "是一个数字" << endl;
}
```


### [7. eof fail](#)
文件结束符问题... 百度吧 不想记录

EOF可在getchar()中用于结束读取，防止while出现死循环。
```cpp
#include<stdio.h>

int main()
{

    int ch=0;
    while((ch=getchar())!=EOF)
    //按住CTRL和z结束读取，退出循环。EOF=end of file无文件可读取。右图中第四次输出回车，输入的时候要回车，该回车也是一个字符。
    {
      putchar(ch);
      printf("读取成功\n");
    }
    return 0;
}
```
cin.fail()/cin.eof() 检测是否输入了 EOF 
```cpp
char ch;
int count = 0;
cin.get(ch);
while (cin.fail() == false)
{
    cout << ch;
    ++count;
    cin.get(ch);
}//输入 ctrl +Z + enter 结束
cout << endl << "输入字符数量：" << count;
```

总结：
* 1.getchar()遇到文件结束符（即EOF），c将被赋值为0 （注意：!= 运算符优先级大于赋值号=），终止循环
* 2.在Windows10操作系统中，EOF被定义为-1

注意：
* 1. win7 ctrl+z 确实是模拟EOF。但是在linux Unix中 CTRL+Z 是将程序挂起，可以使用fg 命名恢复程序的执行， ubuntu 是输入 CTRL+D 是给出入发出一个EOF  jobs 查看挂起程序 fg % number 恢复程序 number是第几个挂起程序！
```shell
remix@remix-A34S:$ g++ main.cpp -o runner; ./runner
jxkicker is your father
jxkicker is your father
^Z
[1]+  已停止               ./runner
remix@remix-A34S$ 
remix@remix-A34S:$ g++ main.cpp -o runner; ./runner
asdjhh
asdjhh
^Z
[2]+  已停止               ./runner
remix@remix-A34S:$ 
remix@remix-A34S:$ jobs
[1]-  已停止               ./runner
[2]+  已停止               ./runner
remix@remix-A34S:$ fg %1
./runner
njkasnd
njkasnd
^C
remix@remix-A34S:$ jobs
[2]+  已停止               ./runner
remix@remix-A34S:$ fg %2
./runner
sdasd
sdasd
^C
remix@remix-A34S:$ 
```

### [8. typedef 类型别名](#)
C++ 为类型建立别名有两张方式，第一种是预处理器
```cpp
#define Byte char //Byte 就是 char 类型
```
第二种就是使用关键字 typedef ：typedef typeName aliasName;
```cpp
typedef char Byte; //给 char 建立一个别名
typedef char * BytePointer;
typedef int array42[42]; //array42 表示42个int的数组类型
```

#### [8.1 using 别名](#)
C++11引入的别名声明很好的替代了typedef，并拥有很多优点，最重要的一点就是**模板中的应用**。

简单一点的
```cpp
typedef const int CInt;
CInt val = 56;
CInt & refVal = val;

using C_Ref_Double = const double &;    
C_Ref_Double score = 58.65;

std::cout << val << "," << refVal <<"\n"; //56,56
std::cout << score <<"\n"; //58.65
```

复杂一点的
```cpp
//使用typedef：
typedef std::unqiue_ptr<std::unordered_map<std::string,std::string>>  UPtrMapSS;

//使用别名声明：
using UPtrMapSS = std::unqiue_ptr<std::unordered_map<std::string,std::string>>;
```

**模板中的应用**: 别名声明可以模板化，typedef就不行。C++98时代我们必须要嵌套在模板化的struct里的typedef才能硬搞出来的东西，C++11可以直截了当的表达。比如我们想要定义一个链表的同义词，它使用了一个自定义的分配器MyAlloc:

```cpp
//使用别名声明：MyAllocList<T>是std::list<T,MyAlloc<T>>同义词
template<typename T>
using MyAllocList = std::list<T,MyAlloc<T>>;

MyAllocList<Widget> lw;



//使用typedef：MyAllocList<T>::type是std::list<T,MyAlloc<T>>同义词
template<typename T>
struct MyAllocList
{
	typedef std::list<T,MyAlloc<T>> type;
};

MyAllocList<Widget>::type lw;
```

### [9. constexpr和常量表达式](#)
**常量表达式**：指值不会改变并且在编译过程就能得到计算结果的表达式或者函数。 定义 constexpr函数只需要将 constexpr放置在函数最前面！

```cpp
constexpr int new_size(){ return 35; }
constexpr int len = new_size();
```

```cpp
const int max_files = 20;            //max_files是常量表达式
const int limit = max_files + 1;     //limit是常量表达式
int staff_size = 27;                 
//staff_size不是常量表达式，因为它的数据类型不是const int
const int sz = get_size();           
//sz不是常量表达式，因为get_size()只有到运行的时候才知道结果，除非它是constexpr修饰的(C++ 11标准）
```

**constexpr变量**：C++ 11新标准规定，允许将变量声明为 constexpr 类型以便由编译器来验证变量的值是否是一个常量表达式。若不是，则编译报错。同时，声明为 constexpr 的变量一定是一个常量，而且必须用常量表达式初始化。 
```cpp
constexpr int mf = 20;              //正确，20是常量表达式
constexpr int limit = mf + 1;       //正确，mf + 1是常量表达式
constexpr int sz = size();          
//未知，若size()函数是一个constexpr函数时即正确，反之错误。
int i = 10;
constexpr int t = i;                //错误，i不是常量
```
* **尽管指针和引用都能被定义成constexpr，但它们的初始值却受到严格限制。一个constexpr指针的初始值必须是nullptr或者0，或者是存储于某个固定地址中的对象。**
* **函数体内定义的变量一般来说并非存在固定地址中（除了static等修饰，且const也不行，static之前不能加限定词volatile，否则出错），相反，定义于所有函数体之外的对象其地址固定不变，能用来初始化constexpr指针，同时能用constexpr引用绑定到这样的变量上。**
* **对于指针而言，constexpr仅对指针有效，与指针所指的对象无关。**

```cpp
const int *p = nullptr;            //正确，p是一个指向整型常量的指针
constexpr int *q = nullptr;        //正确，但q是一个指向整数的常量指针
//二式等同于  int * const q = nullptr ! q永远都是一个 nullptr
```

#### [9.1 constexpr函数](#)
我们允许 constexpr 函数的返回值并非一个常量，constexpr函数可以被隐式地指定为内联函数！

```cpp
constexpr int new_size(){ return 35; }

constexpr size_t scale(size_t cint){ return new_size() * cint; }

int arr[scale(2)]; //正确 是常量表达式
int v = 10;
int arr[scale(v)]; //不正确 v 是变量
```


### [10. 简单文件输入输出](#)
iostream类似，要进行文件需要引入fstream fstream有两个子类：ifstream(input file stream)和ofstream(outpu file stream)，ifstream默认以输入方式打开文件，而ofstream默认以输出方式打开文件

ofstream是从内存到硬盘，ifstream是从硬盘到内存，其实所谓的流缓冲就是内存空间;

在C++中，有一个stream这个类，所有的I/O都以这个“流”类为基础的，包括我们要认识的文件I/O，stream这个类有两个重要的运算符：

* 1、插入器(<<): 向流输出数据。比如说系统有一个默认的标准输出流(cout)，一般情况下就是指的显示器，所以，cout<<“Write Stdout”<<’\n’;就表示把字符串"Write Stdout"和换行字符(’\n’)输出到标准输出流。
* 2、析取器(>>):从流中输入数据。比如说系统有一个默认的标准输入流(cin)，一般情况下就是指的键盘，所以，cin>>x;就表示从标准输入流中读取一个指定类型(即变量x的类型)的数据。
在C++中，对文件的操作是通过stream的子类fstream(file stream)来实现的，所以，要用这种方式操作文件，就必须加入头文件fstream

void open(const char* filename,int mode,int access); 在fstream类中，有一个成员函数open()，就是用来打开文件的，其原型是：
参数：
* filename：要打开的文件名
* mode：要打开文件的方式
* access：打开文件的属性

打开文件的方式在类ios(是所有流式I/O类的基类)中定义，常用的值如下： [可以用“或”把以上属性连接起来，如ios::out|ios::binary]

|mode 打开方式|说明|
|:----|:----|
|ios::app|以追加的方式打开文件|
|ios::ate|文件打开后定位到文件尾，ios:app就包含有此属性|
|ios::binary|以二进制方式打开文件，缺省的方式是文本方式。两种方式的区别见前文|
|ios::in|文件以输入方式打开（文件数据输入到内存）|
|ios::out|文件以输出方式打开（内存数据输出到文件）|
|ios::nocreate|不建立文件，所以文件不存在时打开失败|
|ios::noreplace|不覆盖文件，所以打开文件时如果文件存在失败|
|ios::trunc|如果文件存在，把文件长度设为0|


access 打开文件的属性取值是：
* 0：普通文件，打开访问
* 1：只读文件
* 2：隐含文件
* 4：系统文件

```cpp
fstream file1;
file1.open("c:\config.sys",ios::binary|ios::in,0);

file1.close();
```

打开的文件使用完成后一定要关闭，fstream提供了成员函数close()来完成此操作，如：file1.close();就把file1相连的文件关闭。

#### 10.1 写入到文本文件中
必须包含头文件 fstream。 里面包含一个或多个ofstream对象 fstream 用来打开一个文件，然后将程序内容输出到文件中！
* 指定名称空间 stl
* 使用open()方法将ofstream对象与文件关联起来
* 使用完后，需要使用 close()将其关闭！
* 可结合ofstream对象和运算符 << 来输出各种类型的数据！  

void open( const char *filename, ios_base::openmode mode = ios_base::out );

打开一个 txt 文件然后将键盘输入输出到文件中！
```cpp
#include <iostream>
#include <fstream>

using namespace std;
int main()
{
    std::ofstream jsFile; //创建输出对象
    jsFile.open("./static/outFile.txt", std::ios::app|std::ios::out); //关键文件
    //不指定 std::ios::app 那么默认是重写文件，而不是追加
    const short bufferSize = 51;
    char content[bufferSize];

    printf("please input your information: ");
    cin.getline(content, bufferSize - 1);

    
    jsFile << content; //输出到文件
    

    jsFile.close();//关闭文件
}
```

#### 10.2 读取文本文件
必须包含头文件 fstream。 使用对象 ifstream 打开文件 将文件内如输入到内存！
* 指定名称空间 stl
* 使用open()方法将ifstream对象与文件关联起来
* 使用完后，需要使用 close()将其关闭！
* 可结合 ifstream 对象和运算符 >> 来输出各种类型的数据！  

void open( const char *filename, ios_base::openmode mode = ios_base::in );

ifstream 使用 fail方法检测是否读取失败， eof()方法检测是否读取到末尾，可以使用 >>  get getline 方法读取数据！

利用 eof检测 尾部！ 统计有效字符数，行数！
```cpp
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;
int main()
{
    const short bufferSize = 256;
    std::ifstream myFIle; //创建输出对象

    string filePath = "./static/index.js"; 

    myFIle.open(filePath , ios::in);
    unsigned long total = 0;
    unsigned short lines = 0;
    while (!myFIle.eof()&&myFIle.good())
    {
        char readBuffer[bufferSize];
        myFIle.getline(readBuffer, bufferSize -1);

        printf("%s\n", readBuffer);

        total += (unsigned long)strlen(readBuffer);    
        lines++;
    }
    cout<< "index.js 文件 "<< "有效字符数: " << total << " 行数：" << lines << endl ;
    myFIle.close();//关闭文件
}

```