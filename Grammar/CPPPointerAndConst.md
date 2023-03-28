### [C++ 指针](#)
`指针，用于存储内存地址的变量，在C++中是极其危险的存在！理清它是非常需要的！ 对其进行整理规范！总结用法`

---

- [`1. 指针基础`](#一-指针基础)  
- [`2. const 修饰指针`](#二-const-修饰指针)

  

---
### [1. 指针基础](#)
创建一个指针是非常简单的，使用 `*` 表示指针类型。指针本身是一种复合类型，可以实现对其他对象的间接访问，本身也是一个对象，**允许对指针赋值和拷贝**！， 在指针的生命周期可以先后指向不同的对象。 指针无需在定义的时候就进行赋值。
如果不赋值，那么其内容就是一个不确定的值。**指针定义的时候不进行初始化是一个极其危险，甚至是杀全家的操作！** 指针可以指向数据域也可以指向代码域！ 

* **指针可以指向数据的存储区域** -- `数据指针`
* **同样可以指向代码(机器码)块的存储区域** -- `函数指针`。 


计算机中所有的数据都必须放在内存中，不同类型的数据占用的字节数不一样，例如 `int` 占用 4 个字节，`char` 占用 1 个字节。 为了正确地访问这些数据，必须为每个字节都编上号码，就像门牌号、身份证号一样，每个字节的编号是唯一的，根据编号可以准确地找到某个字节。
我们将存储内存中字节的编号的变量称为指针 `（Pointer）。`地址从 0 开始依次增加，对于 32 位环境，程序能够使用的内存为 `4GB`，最小的地址为 `0`，最大的地址为 `0XFFFFFFFF`


```cpp
struct Pointer2D
{
    float x;
    float y;
};

float distance(const Pointer2D & one,const Pointer2D & two) {
    return sqrt(PPow(abs(one.x - two.x)) + PPow(abs(one.y - two.y)));
}

Pointer2D p1 = { 6.3, 23.2 };
Pointer2D p2 = { 4.0, 15.2 };
//数据指针
Pointer2D* _p = &p2;

//函数指针
float (*pdistance)(const Pointer2D&, const Pointer2D&) = distance;
    
std::cout << distance(p1, *_p) << std::endl;
```

`指针解引用`: 通过指针去取得指针指向的内存中的数据  `*pointerName` 例如: `*_p` 就是解引用

指针的大小问题：由于指针表示的是内存地址，也就是 `0x0DAF542... `  指针大小和操作系统中地址编码大小有关！

1. 在32位操作系统中，占用四个字节空间
2. 64位操作系统中，占用八个字节空间！

可以用sizeof查看指针的大小
```cpp
std::cout <<"int * 类型的指针的大小: " << sizeof(int*) << std::endl; // x86为 4 X64 为 8
std::cout << "Pointer2D * 类型的指针的大小: " << sizeof(Pointer2D*) << std::endl;//4
```

#### [1.1 空指针](#)
指针变量指向内存中编号为0的空间， 可以初始化指针， 但你要记住内存编号为 0-255为系统占用内存，不允许用户访问！ c++表示空指针的方式为 `nullptr`。 你可能很好奇为啥不是 `NULL`。 
 引入nullptr的原因，这个要从NULL说起。对于C和C++程序员来说，一定不会对NULL感到陌生。但是C和C++中的NULL却不等价。NULL表示指针不指向任何对象，但是问题在于，NULL不是关键字，而只是一个宏定义(macro)。

在C中，习惯将NULL定义为void*指针值0：但同时，也允许将NULL定义为整常数0
```c
#define NULL (void*)0  
```
**在C++中，NULL却被明确定义为整常数0：**
```cpp
// lmcons.h中定义NULL的源码  
#ifndef NULL  
#ifdef __cplusplus  //C++ 就定义为 0
#define NULL    0  
#else  
#define NULL    ((void *)0)  
#endif  
#endif
```
根本原因和C++的重载函数有关。C++通过搜索匹配参数的机制，试图找到最佳匹配（best-match）的函数，而如果继续支持void*的隐式类型转换，则会带来语义二义性（syntax ambiguous）的问题。 nullptr在C++11中就是代表空指针

```cpp
int *p = NULL
```

#### [1.2 void*](#)
**void\*** 是一种特殊的指针类型，可以用于存放任意对象的地址，一个 `void*` 指针存放着地址，这一点和其他指针类似，只是我们不知道这个地址对应着什么类型， **所以不能直接操作 void\*所指向的对象！**。
所以 `void*` 指针能做的事情有限，

```cpp
double v = 10.15;
int r = 5;
void * ptr = &v; //指向 double类型
ptr = r; //指向 int类型
```
**总的来说 void\* 仅仅存放内存地址，而无法操作指向的对象！**

**想要啊访问指向的对象，就需要类型转换了**

```cpp
double v = 10.15;
void * ptr = &v; //指向 double类型
double * p = static_cast<double *>(ptr);

std::cout << *p << "\n"; //10.15
```

#### [1.3 野指针](#)
“野指针” 不是NULL指针，而是指向“垃圾”内存的指针!指针变量中的值是非法的内存地址 ，即它指向了不该指向的位置。

**野指针** 的成因主要有两种：
1. 指针变量创建后没有被初始化。任何指针变量刚被创建时不会自动成为NULL指针，它的默认值是随机的，它会乱指一气。
2. 指针p被free或者delete之后，没有置为NULL,让人误以为p是个合法的指针。
3. 指针变量作用范围区域超出，在范围区域外使用了变量。

```cpp
char* str = new char[100];  
delete[] str;  
// Do something  
strcpy( str, "Dangerous!!" );   //str是野指针
```

空指针和野指针都不应该去访问

#### [1.4 指针支持的运算](#)
指针支持许多的运算，例如： 解引用、加减法、比较
* **解引用**： **获得指针指向内存的存放的值。这里解引用返回的是值的一个引用，而不是值本身**
* **自增++， 自减 --**： 内存地址加一个类型单位或者减少一个类型单位
* **整数加法，整数减法**： 指针加法和数学运算加法是不一样的，它和类型有关！ **结果还是指针**
* **指针之间的加减**: **返回的是单位差距，例如数组首地址指针 减去数组尾后的地址，就是数组的长度** **结果是 有符号整数**
* **指针的比较** 需要指针指向的类型相同且指向相关才能进行比较！

```c
#include "stdio.h"

int main(int argc, char const *argv[])
{
    float v1 = 5.63, v2 = 9.63, v3 = 98.9;

    float *pv = &v1;
    float *pv2 = &v2;
    
    printf("%#x --( + 1)-> %#x \n", pv, pv + 1); // 指针 加了四个地址单位 一个类型长度
    printf("%#x --( + 1)-> %#x \n", pv2, pv2 + 1);

    int arr[] = { 99, 15, 100, 888, 252 };
    int len = sizeof(arr) / sizeof(int);  //求数组长度
    for(int i=0; i<len; i++){
        printf("Unit address--> %#X : %d \n", (arr + i), *(arr+i) );  //*(arr+i)等价于arr[i]
    } 
    return 0;
}
/*
0x60fef8 --( + 1)-> 0x60fefc 
0x60fef4 --( + 1)-> 0x60fef8 
Unit address--> 0X60FEE0 : 99 
Unit address--> 0X60FEE4 : 15 
Unit address--> 0X60FEE8 : 100 
Unit address--> 0X60FEEC : 888 
Unit address--> 0X60FEF0 : 252 
*/
```

#### [1.5 数组指针](#)
`数组（Array）是一系列具有相同类型的数据的集合，每一份数据叫做一个数组元素（Element）。数组中的所有元素在内存中是连续排列的，整个数组占用的是一块内存。以int arr[] = { 99, 15, 100, 888, 252 };为例，该数组在内存中的分布如下所示： `

|`arr[0]`|`arr[1]`|`arr[2]`|`arr[3]`|`arr[4]`|
|:---|:---|:---|:---|:---|
|`99`|`15`|`100`|`888`|`252`|

`定义数组时，要给出数组名和数组长度，数组名可以认为是一个指针，它指向数组的第 0 个元素。在C语言中，我们将第 0 个元素的地址称为数组的首地址。以上面的数组为例，下图是 arr 的指向：`

```shell
arr --> &arr[0]
```
`数组名的本意是表示整个数组，也就是表示多份数据的集合，但在使用过程中经常会转换为指向数组第 0 个元素的指针，所以上面使用了“认为”一词，表示数组名和数组首地址并不总是等价。初学者可以暂时忽略这个细节，把数组名当做指向第 0 个元素的指针使用即可` **`但是谨记数组名可以理解为指针但是他是一种有类型的指针是指向数组的指针`**

**数组名是常量，它的值不能改变，而数组指针是变量（除非特别指明它是常量），它的值可以任意改变。**

**数组名只能指向数组的开头，而数组指针可以先指向数组开头，再指向其他元素**

```cpp
#include <stdio.h>
int main(){
    int a[3][4] = { {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11} };
    int (*p)[4] = a;
    printf("%d\n", sizeof(*(p+1)));
    return 0;
}
```

**指针也是迭代器，支持 STL遍历**
```cpp
int scores[5] = {1,2,4,8,16};

std::for_each(scores, scores + 5,  [](auto v){ std::cout<< v << "\n";});
```

#### [1.6 二维数组指针](#)  
`二维数组在概念上是二维的，有行和列，但在内存中所有的数组元素都是连续排列的，它们之间没有“缝隙”。以下面的二维数组 a 为例：`

```
int a[3][4] = { {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11} };
```
`从概念上理解，a 的分布像一个矩阵：`
```
0   1   2   3
4   5   6   7
8   9  10  11
```
`但在内存中，a 的分布是一维线性的，整个数组占用一块连续的内存：a 指向 a[0]`
```shell
---------------------------------------------------
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
---------------------------------------------------
↑a[0]           ↑a[1]           ↑a[2]     
 ↑a
```
`a[0], a[1] a[2] 本质上也是指针`

`为了更好的理解指针和二维数组的关系，我们先来定义一个指向 a 的指针变量 p：`

```c
int (*p)[4] = a; //此时 p是一个二维数组指针
```
`括号中的*表明 p 是一个指针，它指向一个数组，数组的类型为int [4]，这正是 a 所包含的每个一维数组的类型`

`[ ]的优先级高于*，( )是必须要加的，如果赤裸裸地写作int *p[4]，那么应该理解为int *(p[4])，p 就成了一个指针数组，而不是二维数组指针`

`a[r][c] = *(*(a + r) + c);`
```c
#include "stdio.h"

int main(int argc, char const *argv[])
{
    int a[3][4] = { {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11} };
    int (*p)[4] = a;
    printf("a[1][2]:%d  p[1][2]:%d", a[1][2], *(*(p  + 1) + 2);
    return 0;
}
/*
a[1][2]:6  p[1][2]:6
*/
```



#### [1.7 指针数组和二维数组指针的区别](#)
`指针数组和二维数组指针在定义时非常相似，只是括号的位置不同：`

```
int *(p1[5]);  //指针数组，可以去掉括号直接写作 int *p1[5];
int (*p2)[5];  //二维数组指针，不能去掉括号
```
`指针数组和二维数组指针有着本质上的区别：指针数组是一个数组，只是每个元素保存的都是指针，以上面的 p1 为例，在32位环境下它占用 4×5 = 20 个字节的内存。二维数组指针是一个指针，它指向一个二维数组，以上面的 p2 为例，它占用 4 个字节的内存。`

#### [1.8 指向指针数组的指针](#)
`看懂就懂了！` `和二维数组指针对比一下吧！`

```cpp
int * list[3];  //长度为3的指针数组

int *(*list)[3]; //指向指针数组的指针
```
`实际演示`

```cpp
#include<iostream>
#include<array>

int main(int argc, char const *argv[])
{
    using namespace std;
    int a = 10, b =11, c =12;
    int *pa = &a;
    int *pb = &b;
    int *pc = &c;

    int *list[3] = {pa, pb, pc};

    for (int * pointer : list)
    {
        cout << "address: " << pointer << " value: " << *pointer << endl;
    }
    /*
    address: 0x7ffd41e05eac value: 10
    address: 0x7ffd41e05eb0 value: 11
    address: 0x7ffd41e05eb4 value: 12
    */

    int *(*dplist)[3] = &list;
    auto plist = &list;

    for (int * pointer : (*plist))
    {
        cout << "address: " << pointer << " value: " << *pointer << endl;
    }
    /*
    address: 0x7ffd41e05eac value: 10
    address: 0x7ffd41e05eb0 value: 11
    address: 0x7ffd41e05eb4 value: 12
    */

    cout << "address: " << (*dplist)[1] << " value: " << *((*dplist)[1]) << endl;
    //address: 0x7ffd41e05eb0 value: 11
    
    return 0;
}
```

#### [1.9 函数指针](#) 
`一个函数总是占用一段连续的内存区域，函数名在表达式中有时也会被转换为该函数所在内存区域的首地址，这和数组名非常类似。我们可以把函数的这个首地址（或称入口地址）赋予一个指针变量，使指针变量指向函数所在的内存区域，然后通过指针变量就可以找到并调用该函数。这种指针就是函数指针。`

`函数指针的定义形式为：`
```
returnType (*pointerName)(param list);
```
`returnType 为函数返回值类型，pointerNmae 为指针名称，param list 为函数参数列表。参数列表中可以同时给出参数的类型和名称，也可以只给出参数的类型，省略参数的名称，这一点和函数原型非常类似。注意( )的优先级高于*，第一个括号不能省略`

```c
/*
函数指针的形式: 你可以直接理解为 
   1. 复制函数声明[不要函数体]
   2. 修改函数名为你想要的指针名
   3. 在新名称前加一个 *符号 然后一起括起来 就是一个函数指针的声明形式  
*/
int add(int a, int b){
    return a + b;
}

int (*ADD)(int a,int b) = add;

int result = (*ADD)(a,b);
```

`调用方式`
```c
(*pointerName)(param list);
```

```c
#include <stdio.h>
//返回两个数中较大的一个
int max(int a, int b){
    return a>b ? a : b;
}

int main(){
    int x = 12, y = 35, maxval;
    //定义函数指针
    int (*pmax)(int, int) = max;  //也可以写作int (*pmax)(int a, int b)
    maxval = (*pmax)(x, y);
    printf("Max value: %d\n", maxval);
    return 0;
}
```

#### [1.10 C语言指针作为函数返回值](#) 
`C语言允许函数的返回值是一个指针（地址），我们将这样的函数称为指针函数。`

```c
#include <string.h>
char *strlong(char *str1, char *str2){
    if(strlen(str1) >= strlen(str2)){
        return str1;
    }else{
        return str2;
    }
}
```

`用指针作为函数返回值时需要注意的一点是，函数运行结束后会销毁在它内部定义的所有局部数据，包括局部变量、局部数组和形式参数，函数返回的指针请尽量不要指向这些数据，C语言没有任何机制来保证这些数据会一直有效，它们在后续使用过程中可能会引发运行时错误。`

```c
#include <stdio.h>
int *func(){
    int n = 100;
    return &n; //gcc 报错不允许返回本地变量的地址
    /* warning: function returns address of local variable [-Wreturn-local-addr]  */
}
int main(){
    int *p = func(), n;
    n = *p;
    printf("value = %d\n", n);
    return 0;
}
```

`前面我们说函数运行结束后会销毁所有的局部数据，这个观点并没错，大部分C语言教材也都强调了这一点。但是，这里所谓的销毁并不是将局部数据所占用的内存全部抹掉，而是程序放弃对它的使用权限，弃之不理，后面的代码可以随意使用这块内存。对于上面的两个例子，func() 运行结束后 n 的内存依然保持原样，值还是 100，如果使用及时也能够得到正确的数据，如果有其它函数被调用就会覆盖这块内存，得到的数据就失去了意义。 `



#### [1.11 总结](#)

|`定  义`|`含  义`|
|:---|:----|
|`int *p;`| `p 可以指向 int 类型的数据，也可以指向类似 int arr[n] 的数组。`|
|`int **p;`| `p 为二级指针，指向 int * 类型的数据。`|
|`int *p[n];`|`p 为指针数组。[ ] 的优先级高于 *，所以应该理解为 int *(p[n]);`|
|`int (*p)[n];` |`p 为二维数组指针。`|
|`int *p();` |`p 是一个函数，它的返回值类型为 int *。`|
|`int (*p)();` |`p 是一个函数指针，指向原型为 int func() 的函数。`|



### [2. const 修饰指针](#)
常量指针 `const int* p `  指针常量 `int * const p`  指向常量的常量指针 `const int * const p` 这需要分清楚他们！

1. 常量指针: 指针指向一个常量,无法通过指针去修改原始值。
2. 指针常量: 将指针式声明为常量，那么就不能修改指针的指向。
3. 指向常量的常量指针: 既无法通过指针去修改原始值，又不能修改指针的指向。

#### [2.1 指向常量的指针](#)
`要想让指针指向一个常量，就必须对指针的声明做一些修改！` **谨记： const DataType \* pointerName 类型的指针，无法通过指针去修改原始值 \*pointerName = value; 是被禁止的！**

`直接让指针变量指向一个常量是不会被编译通过的。`
```cpp
const int weekDays = 7; 
int* wdp = &weekDays; 
/* "const int *" 类型的值不能用于初始化 "int *" 类型的实体 */
```
`常量类型就要配指向常量的指针`: `const DataType * pointerName = &....`

```cpp
const int* wdp = &weekDays; 
```
`由于指向常量，所以无法通过指针修改原始值！`
```cpp
*wdp = 8; // 修改不被允许
```
`可以将常规变量的地址赋值给常规指针，还可以将常规变量的地址赋值给指向const的指针。` `同样会导致不可以通过指针修改原始值！`
```cpp
float PI = 3.1415;

const float* PIPointer = &PI;
*PIPointer = 3.141592; // 修改不被允许
```

`const 指针可以指向 const变量，也可以指向常量。一旦指针声明为const。就无法通过指针修改原始值。 但是常量指针不可以指向const 变量。`

`C++ Primer Plus 里面的一段代码分析`
```cpp
const int** pp2;
int* p1;
const int n = 13; 
pp2 = &p1; //不被允许
*pp2 = &n; // 由于上一行代码不被允许，所以不被执行，就算删除 pp2 = &p1; 也会报错 使用了未初始化的指针 pp2
*p1 = 10;
```

#### [2.2 尽量使用const ](#)
`可以避免由于无意间修改数据而导致编程错误。使用const使得函数能够处理const和非const实参。否则只能结束非const的数据`

```cpp
int sum(const int* list,const int size) {
    int total = 0;
    for (int i = 0; i < size; i++)
    {
        total += list[i];
    }
    return total;
}

const int len = 5;
int list[len] = { 5,15,25, 45,125 };
cout << sum(list, len);
```

#### [2.3 常量指针](#)
`将指针式声明为常量，那么就不能修改指针的引用，这个声明方式就是把 const 放后面就可以了; 虽然不能改变指针的指向，但是可以通过指针修改原始值。`

```cpp
int WeekDays = 7;
int timerCount = 3;
int* const wdp = &WeekDays;
//wdp = &timerCount; //不被允许修改
*wdp = 10;

cout << *wdp; //10
```

#### [2.4 指向常量的常量指针](#)
`不仅不能能改变指针的指向，而且不能通过指针修改原始值！` `const DataType * const pointerName = ....;`

```cpp
const int * const wdp = &WeekDays;
```

#### [2.5 字符串指针](#)
`c++ 禁止讲字符串常量赋值给一个字符串指针变量,这是为了防止你通过变量去修改常量！`
```cpp
char * name = "jxkicker";//不被允许
```
`正确做法`
```cpp
const char * name = "jxkicker";//允许
```