### [函数 C++的编程模块](#)
函数可以是说是任何程序设计需要都必有的语法成分了，也是C++的基本编程模块，提高编程效率。学习函数后还可以深入学习STL和BOOST C++..

-----
- [x] [1. C++ 函数概述](#1-c-函数概述)
- [x] [2. 函数和二维指针](#2-函数和二维指针)
- [x] [3. 复合类型参数](#3-复合类型参数)
- [x] [4. 函数指针](#4-函数指针)
- [x] [5. 内联函数](#5-内联函数)
- [x] [6. 默认参数](#6-默认参数)
- [x] [7. 函数重载](#7-函数重载)
- [x] [8. 变长参数](#8-变长参数)
- [x] [9. 参数使用](#参数使用)
- [x] [10. 函数返回值](#函数返回值)

-----

### [1. C++ 函数概述](#)
C++ 中, 函数三件事，提供函数定义，提供函数原型 ，调用函数。 库函数是指，已结定义和编译好的函数！ **同时要区分清楚，函数类型和函数指针，学会如何声明函数类型和函数指针**
* 一般来说，一个函数定义由四个部分组成，参数 返回值 函数体 函数名 C++中函数定义可以单独声明的！ 
* 按照C语言惯性，我们知道声明的参数名称是可以省略的，所以下面的声明部分也可以换成
* 函数也可以分为，有返回值函数和无返回值函数，如果函数有返回值，那么函数通过将返回值复制到指定的CPU寄存器或者内存单位中将其返回！
* 函数参数是按值传递的！
* C++ 不允许对main进行递归调用！
```cpp
int add(int, int);
```

```cpp
int add(int a, int b);


int main(int argc, char const* argv[])
{
    cout << add(55, 65);
    return 0;
}

int add(int a, int b) {
    return a + b;
};
```
C++对返回值的类型有一定的限制，不能是数组，但可以是其他任何类型。 通常，函数通过将返回值复制到指定的CPU寄存器或内存单元中将其返回。

**重点：**
C++ 函数可以利用是否有返回值将其分为两类 对于有返回值的函数 它将会生成一个值，也就是说函数调用后会在内存中开辟一个空间用来存储这个结果值！ 这个值可以赋值
给其他变量或在其他表达式中使用....

#### [1.1 函数原型](#)
也可以称之为函数声明！ 如下所示，原型描述了函数到编译器的接口，也就是说，它将函数返回值的类型，以及参数的类型和数量提供给编译器，应该多使用原型，提前告诉编译器函数信息。
```cpp
int add(int, int);
```
如果你不使用函数原型，那么你需要在调用它之前定义它

```cpp
int add(int a, int b) {
    return a + b;
};

int main(int argc, char const* argv[])
{
    cout << add(55, 65);
    return 0;
}
```
果在程序中提供函数原型呢？有两种方法
1. 直接在源代码中输入函数原型，如上所示!
2. 包含在头文件中例如，#include<cmath>  [推荐如此使用]

要区分函数的定义和函数原型！

#### [1.2 一维数组参数问题](#)
C++ 通常按照值传递参数。 我们主要关注参数是数组的问题。sum里面的 arr 参数是一个指针。指向数组的第一个参数。也就是数组名称。这还是很简单的。
**数组以指针的形式传递给函数** [**由于数组不能拷贝，所以无法按值传递方式使用数组参数。因为数组会被转换位指针，当我们为函数传递一个数组时，实际上传递的是指向数组首元素的指针。**](#)

```cpp
//以下三个数组是等价的
void sum(const int *vals, int len);
void sum(const int vals[], int len);
void sum(const int vals[10], int len);
```

理解下面的 sizeof，上面的sizeof 计算的数组大小，下面的 sizeof 计算的 int *的大小也就是一个指针的大小！ 这就是我们为什么要传递 len 参数的原因！
```cpp
int sum(int arr[], int len);

int main(int argc, char const* argv[])
{
	const int len = 5;
	int array[len]{ 58,56,98,75,44 };

	int size = sizeof(array); // 20 字节

	cout << sum(array, len); //331
    return 0;
}

int sum(int arr[], int len) {
	int total = 0;

	int size = sizeof(arr); // 8 字节

	for (int i = 0; i < len; i++)
	{
		total += *(arr + i);
	}
	return total;
}
```
int arr[] 说明可以将任何长度的int类型数组传递给sum函数。实际是一个指针。指向数组的第一个元素， 所以我们也可以这样写！ C++和C语言一样，也将数组名视为指针！ **参数是数组推荐使用数组表示法 int arr[] 而不是 int *arr 更加直观！**
```cpp
int sum(int *arr, int len);

int main(int argc, char const* argv[])
{
	const int len = 5;
	int array[len]{ 58,56,98,75,44 };
	cout << sum(array, len); //331
    return 0;
}

int sum(int *arr, int len) {
	int total = 0;
	for (int i = 0; i < len; i++)
	{
		total += *(arr + i);
	}
	return total;
}
```

#### [1.3 函数占位参数](#)
c++中函数的形参列表里可以有占位参数，用来占位，调用函数时必须填补位置
下方参数列表中  **第二个 int 既是占位参数 **
```cpp
//语法： 返回值类型 函数名（数据类型） { }

void test(int c, int)
{
    //pass
}

test(10,10);
```
占位参数的：**​在调用的时候必须传入该类型的参数函数才能被调用​；**

**优点**：
1. 占位参数与默认参数结合起来使用，可以做到兼容旧版本代码，提高代码的规范性。
2. 可以以小的改动，进行代码移植等。

### [2. 函数和二维指针](#)
一维数组的数组名是其第一个元素位置的地址别名！其是一个指针。指针指向的类型是数组成员的类型！ 二维数组名 是指向一个一维指针数组的第一个位置地址的别名！这个指针数组里面的每个一个值都代表一个一维数组指针！

二维数组：定义的一般形式是：

```cpp
dataType arrayName[length1][length2];

//例子
int a[3][4];
```

按行分段赋值应该写作
```cpp
int a[5][3]={ {80,75,92}, {61,65,71}, {59,63,70}, {85,87,90}, {76,77,85} };
```

按行连续赋值应该写作
```cpp
int a[5][3]={80, 75, 92, 61, 65, 71, 59, 63, 70, 85, 87, 90, 76, 77, 85};
```

int a[2][2] 等价于 *(*(a + 2) + 2)

```cpp
int a[5][3] = { {80,75,92}, {61,65,71}, {59,63,70}, {85,87,90}, {76,77,85} };
cout << *(*(a + 2) + 2) << " " << a[2][2]; // 70 70
```
#### 2.1 数组定义式 
当传递函数参数的时候，什么样的类型表示一个二维数组呢？ 第一种数组定义式

```cpp
void sum(int list[][3], int size);


int main(int argc, char const* argv[])
{
	int a[5][3] = { {80,75,92}, {61,65,71}, {59,63,70}, {85,87,90}, {76,77,85} };
	sum(a, 5);
    return 0;
}

void sum(int list[][3], int rwo) {
	for (int i = 0; i < rwo; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << list[i][j] << " ";
		}
		cout << endl;
	}
}
```

#### 2.2 二维指针式
还有一种这样写: int (*list)[3] 其中括号是必不可少的。如果不加括号就变成指针数组了 c++ [] 优先级 高于 \*

* int *array[4]: 一维指针数组 [] 优先级更高，所以先是数组 ，然后根据 int * 说明是指针数据。
* int *array[5][5]:二维指针数组
* int (*array)[4]:二维数组指针 列数为4。 (*array) 此时 * 先作用于 array 然后才是 []。所以是二位数组指针！ 没法子 C++ 就是这样设计的，贼他妈奇怪！ 

```cpp
void sum(int (*list)[3], int size);


int main(int argc, char const* argv[])
{
	int a[5][3] = { {80,75,92}, {61,65,71}, {59,63,70}, {85,87,90}, {76,77,85} };
	sum(a, 5);
    return 0;
}

void sum(int (*list)[3], int rwo) {
	for (int i = 0; i < rwo; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << list[i][j] << " ";
		}
		cout << endl;
	}
}

```

反正你需要指出了列数！推荐使用2.1 指针数组和二维数组名和二级指针是三个完全不同的概念哦！ 二维数组指针是指向一个一维指针数组第一个位置的指针，但是这个一维指针数组的元素，是指向具有特定长度的数组的第一个位置的指针。

### [3. 复合类型参数](#)
将结构体，集合类作为参数

#### [3.1 结构体参数](#)
将结构体作为参数传递给函数，需要记住结构体是值类型

第一种：直接将结构体当参数传递给函数： 按值传递
```cpp
struct Pointer2D
{
    float x;
    float y;
};

float distance(Pointer2D one, Pointer2D two) {
    return sqrt(pow(abs(one.x - two.x), 2) + pow(abs(one.y - two.y), 2));
}

Pointer2D p1 = { 1.0,5.0 };
Pointer2D p2 = { 3.0,7.0 };

cout << distance(p1, p2);//2.82843
```

第二种: 给函数传递结构体指针：按引用传值。通过指针访问结构体需要使用 -> 符号
```cpp
void output(const Pointer2D *o) {
    cout << "x: " << o->x << endl;
    cout << "y: " << o->y << endl;
}
Pointer2D p1 = { 1.0,5.0 };

output(&p1);
```

结构体引用做参数
```cpp
#include<iostream>
#include<array>
#include<cmath>
struct Pointer2D
{
    float x;
    float y;
};

float distance(const Pointer2D & one, const Pointer2D & two) {
    return sqrt(pow(abs(one.x - two.x), 2) + pow(abs(one.y - two.y), 2));
}


int main(int argc, char const* argv[])
{
    using namespace std;
    Pointer2D p1 = { 1.0,5.0 };
    Pointer2D p2 = { 3.0,7.0 };

    cout << distance(p1, p2) << endl;;//2.82843
    return 0;
}
```

#### [3.2 array参数](#)
array 是 stl提供的数组集合类！

```cpp
#include<iostream>
#include<array>

double total(std::array<double, 5> *list){
    double all = 0.0;
    for (int i = 0; i < 5; i++)
    {
        all += list->at(i);
    }
    
    return all;
}

double totalp(std::array<double, 5> *list){
    double all = 0.0;
    for (int i = 0; i < 5; i++)
    {
        all += (*list)[i];
    }
    
    return all;
}


int main(int argc, char const *argv[])
{
    using namespace std;

    std::array<double, 5> scores = {95.16, 85.12 ,56.3,75.36, 99.21};

    cout << total(&scores) << endl;
    /* code */
    return 0;
}
```

#### [3.3 string 做引用参数](#)
将一个字符串逆序

```cpp
#include<iostream>

std::string reverse(const std::string &str){

    std::string result = "";
    for (auto &&c : str)
    {
        result.insert(0,1, c);
    }
    return result;
}

int main(int argc, char const* argv[])
{
    std::string val = {"123456789"};

    std::cout << reverse(val) <<std::endl; 
    std::cout << reverse("java is the best languge in the word!") <<std::endl; 
    return 0;
}
```

### [4. 函数指针](#) 
函数也有地址，函数的地址是存储其及其语言代码的开始地址。通过函数指针，你可以对不同的事件使用不同的函数。同时函数名本来就是一个函数指针！[] 括号优先级比 * 所以定义函数指针的时候要用括号。

* 存在一个函数
* 声明一个函数指针
* 获取一个已经存在的函数的值：函数名就是函数地址
* 将函数地址赋值给函数指针
* 使用函数指针来调用函数

语法
```
return_DataType (*pointerName)(dataType1, dataType2,....);
```

```cpp
int redure(int a, int b) {
    return a - b;
}

int add(int a, int b) {
    return a + b;
}

int multi(int a, int b) {
    return a * b;
}


int call(int (*op)(int, int), int v1, int v2) {
    return (*op)(v1, v2);
}

cout << call(add, 20, 30) << endl;
cout << call(redure, 50, 20) << endl;
cout << call(multi, 10, 20) << endl;
```

使用函数指针，再c语言里面是 (*pointerName)(参数) 来使用函数指针的。 例如
```cpp
int (*op)(int, int) = multi;
cout << (*multi)(25, 15) <<endl;
```
但是c++ 也允许像使用函数名那样使用函数指针 multi(25,15);  前者虽然不好看，但是它给出了强有力的提示，代码正在使用函数指针。
```cpp
int (*op)(int, int) = multi;
int (*opt)(int, int) = &multi; //可以有取地址符 也可以没有
cout << op(25, 15) <<endl;
```
使用C++ 11 auto来声明函数指针

```cpp
auto op = multi;
cout << op(25, 15) <<endl;
```
给出三个函数原型，为他们编写一个函数指针 通过auto 你会发现很简单哦！。但是在声明函数参数为函数指针的时候显然不能这样做！
```cpp
const double* f1(const double ar[], int n);
const double* f2(const double[], int n);
const double* f3(const double*, int n);
//函数实现省略 本质是是数组加和

//函数指针
const double* (*op)(const double*, int) = f1;
auto p = f2;
```

再看一个例子

```cpp
#include<iostream>
#include<array>

double total(std::array<double, 5> *list){
    double all = 0.0;
    for (int i = 0; i < 5; i++)
    {
        all += list->at(i);
    }
    return all;
}

double totalp(std::array<double, 5> *list){
    double all = 0.0;
    for (int i = 0; i < 5; i++)
    {
        all += (*list)[i];
    }
    return all;
}

int main(int argc, char const *argv[])
{
    using namespace std;

    std::array<double, 5> scores = {95.16, 85.12 ,56.3,75.36, 99.21};

    double (*calculate)(std::array<double, 5> *list) = totalp;

    cout << calculate(&scores) << endl;
    /* code */
    return 0;
}
```


#### [4.1 重载函数的指针](#) 
如果你的函数指针指向的是一个重载函数，那么编译器会根据指针类型决定选用哪个函数。没有与之匹配的重载函数就会报错！

```cpp
int find(int begin, int end);
int find(int start, double value);
int find(double value);

int (*fp)(double value) = find;//绑定第三个函数
```

#### [4.2 函数指针数组](#) 
关键是吧这个 [] 加在哪里？
```cpp
const double *(*pa[3])(const double *, int) = {f1, f2, f3};

//利用C++ 11 的自动推断
auto pa = { f1, f2, f3 };

//如何使用呢
pa[1](list, 5);
```

#### [4.3 指向函数指针的指针](#) 
最简答是使用 auto
```cpp
auto pc = &pa;

const double* *(*op)(const double*, int) = &f1;
```

#### [4.4 指向函数指针数组的指针](#) 
注意写法 *pd 表示函数指针 (*pd)[3] 表示函数指针数组 (*(*pd)[3]) 表示指向函数指针数组的指针！
```cpp
const double *(*pa[3])(const double *, int) = {f1, f2, f3};

const double* (*(*pd)[3])(const double *, int) = &pa;
```

* (*pd[2])(avlist, 3) 表示调用 f3 函数得到返回值是一个 pointer-to-double 等价：f3(avlist, 3)
* *pd[2](avlist, 3) 表示调用 f3 函数得到返回值 pointer-to-double， 然后再对这个返回的指针解引用 获得它的实际值！ 等价： *f3(avlist, 3)

#### [4.5 typedef 给函数指针区别名](#)
和其他的有所区别， 只有两部分 ，前面是关键字，后面是函数指针，指针名就是类型别名！

```cpp
typedef double real; //给double一个别名

typedef const double* calculate_total(const double*, int n); //给函数指针去别名的方式

calculate_total cal = f2;
```

**和decltype() 配合** ,decltype(函数) 返回的是函数类型。不会将函数类型自动转换为指针类型，因此decltype的结果是函数类型，所
以只有在结果前面加上*才能得到指针，但是两种使用起来也没啥区别。
```cpp
int findByValue(int start, double value);

typedef decltype(findByValue) Find_ptr;  // Find_ptr 函数类型
typedef decltype(findByValue) *Find_ptr2; //与上面等价的类型 函数指针
```

### [5. 内联函数](#)
C++ 内联函数是通常与类一起使用。 **如果一个函数是内联的，那么在编译时，编译器会把该函数的代码副本放置在每个调用该函数的地方。对内联函数进行任何修改，都需要重新编译函数的所有客户端，因为编译器需要重新更换一次所有的代码，否则将会继续使用旧的函数。**
如果想把一个函数定义为内联函数，则需要在函数名前面放置关键字 inline，在调用函数之前需要对函数进行定义。如果已定义的函数多于一行，编译器会忽略 inline 限定符。在类定义中的定义的函数都是内联函数，即使没有使用 inline 说明符。

```cpp
inline int Max(int x, int y)
{
   return (x > y)? x : y;
}
```

引入内联函数的目的是为了解决程序中函数调用的效率问题，这么说吧，程序在编译器编译的时候，编译器将程序中出现的内联函数的调用表达式用内联函数的函数体进行替换，而对于其他的函数，都是在运行时候才被替代。这其实就是个空间代价换时间的i节省。所以内联函数一般都是1-5行的小函数。在使用内联函数时要留神：  

1.在内联函数内不允许使用循环语句和开关语句；  
2.内联函数的定义必须出现在内联函数第一次调用之前；  
3.类结构中所在的类说明内部定义的函数是内联函数。  

**对于小块的函数，使用内联可以减少堆栈开销！不需要再跳转内存，存储变量，出栈入栈。 代价是：占用更多的内存空间！**

执行函数代码的时间比处理函数调用机制的时间长, inline作用不大，如果代码执行时间短但是调用机制时间长，可以使用！

对函数声明和定义一起使用inline

```cpp
inline float distance(Pointer2D one, Pointer2D two);

inline float distance(Pointer2D one, Pointer2D two) {
    return sqrt(pow(abs(one.x - two.x), 2) + pow(abs(one.y - two.y), 2));
}
```
**内联函数和常规函数之间的主要区别在于C++编译器如何将它们组合到程序中！**

**好处是运行更快，不需要CPU执行多余的堆栈，内存跳转，缺点是需要更多的内存**

内联和宏 函数宏 都可以替换为内联函数

* 宏的本质是 **文本替换** 问题，纯纯的文本替换,无法按值传递！

```cpp
#define SQUARE(X) X*X;

int a = SQUARE(5);
float b = SQUARE(5.2);
cout << a << endl;
cout << b << endl;
```

你会发现 SQUARE(5+7) 就不管用了。 5+7*5+7 需要修改宏为

```cpp
#define SQUARE(X) ((X)*(X));
```

如果使用C语言的宏执行了类似函数的功能，就应该将它们转换为C++内联函数！

### [6. 默认参数](#)
你懂的，简单。基本上每个语言都有的概念！ **重点：必须从右向左添加默认值**

```cpp
int func1(int a, int b =10, int c =5);//合法
int func2(int a, int b = 10, int c); //非法 不能 fun2(10, _ , 1); 调用
int func3(int k = 1, int m =2, int n = 3);//合法

func1(1);  //func1(1, 10 , 5);

inline float changeDollar(const float& rmb, const float rate = 6.3147) {
    return rmb * rate;
}

float rmb = 68.5;

cout << "rmb   :" << rmb << endl; // 68.5
cout << "dollar:" << changeDollar(rmb) << endl;  // 432.557
```

### [7. 函数重载](#)
函数重载又称为函数多态，可以让i能够使用多个同名的函数，使得一个名字的函数具有多种形态！ 相同函数名和返回值，不同的参数个数，参数类型。 参数列表为函数的 ----- 函数特征标 function signature。

编译器根据你传递的参数决定调用那个函数，调用过程中也会有隐式类型转换在作用！
```cpp
void staff(double & r1);
void staff(const double & r2);
void staff(const double && r3);
```

```cpp
inline void swap(double& x, double& y) {
    auto temp = x;
    x = y;
    y = temp;
};

inline void swap(float& x, float& y) {
    auto temp = x;
    x = y;
    y = temp;
}

inline void swap(int& x, int& y) {
    auto temp = x;
    x = y;
    y = temp;
}

int x = 12,y = 65;
swap(x, y);

cout << "x:" << x << endl; 
cout << "y:" << y << endl;  
```

匹配函数的时候， const 和 非const变量是要区分的

```cpp
int func_(char * str);
int func_(const char * str);
```

**C++ 使用名称修饰或名称矫正来跟踪每一个重载函数， 是一种在编译过程中，将函数、变量的名称重新改编的机制，简单来说就是编译器为了区分各个函数，将函数通过一定算法，重新修饰为一个全局唯一的名称。**

#### [7.1 函数特征标](#)
函数重载的关键在于函数的列表参数 --- 也称为 函数特征标(function signature) 。如果两个函数参数数目和类型相同，同时参数的排列顺序相同，则它们的特征标也相同！ 一些看起来彼此不同的特征标是不能共存的！

```cpp
double cube(double x);
double cube(double & x);
```
以上两个函数，对于参数x 与 double x 和 double &x 都匹配，编译器就不知道应该调用那个原型！

**谨记: 编译器在检查凸函数特征标时，将类型引用和类型本身视为同一个特征标！**

**利用泛型来解决部分重载问题**

```cpp
#include<iostream>

template<typename T>
void swap(T& x, T& y) {
    auto temp = x;
    x = y;
    y = temp;
};

template<typename T>
void swap(T x[], T y[], int size) {
    T temp;
    for (int i = 0; i < size; i++)
    {
        temp = x[i];
        x[i] = y[i];
        y[i] = temp;
    }
}


int main(int argc, char const* argv[])
{
    int a = 10, b =20;
    swap<int>(a, b);

    std::cout<< "a: " << a << " b: " << b << std::endl;
    //a: 20 b: 10
    int list1[] = {15,52,69,78,45};
    int list2[] = { 75,84,95,71,65 };

    swap<int>(list1, list2, 5);

    std::cout<< "list1[0]: " << list1[0] << " list2[0]: " << list2[0] << std::endl;
    //list1[0]: 75 list2[0]: 15
    return 0;
}
```

#### [7.2 重载和const形参](#)
一个拥有顶层const的形参无法和另一个没有顶层const的形参区分开。

```cpp
int lookup(Phone phone);
int lookup(const Phone phone);    //顶层 const 无法与上面的函数构成重载 重复声明 

int lookup(Phone phone*);
int lookup(Phone * const phone);  //顶层 const 无法与上面的函数构成重载 重复声明
```

如果形参是某种类型的指针或引用，则通过区分其指向的是常量对象还是非常量对象可以实现函数重载，此时的const是底层的

```cpp
Record FindSlot(Account &);  //函数作用域 Account 引用
Record FindSlot(const Account &);  //新函数 作用于常量引用

Record FindSlot(Account *);  //函数作用域 Account 指针
Record FindSlot(const Account *);  //新函数 作用于常量指针
```

**总结： 顶层const不能重载，底层const可以！**

```cpp
void sink(double & v); //左值引用
void sink(const double & v); //常量左值
void sink(double && v); //右值引用参数
```

#### [7.3 特征标浅析](#)
C++中，普通函数重载需要满足一定的条件，总之，普通函数重载取决于形参的个数和类型，与函数的返回值类型无关。

* 形参的个数不同
* 形参的类型不同
* 形参的类型和个数都不同
* 返回值必须一样

**但是，在类中，函数将区分常量函数和非常量函数的特征标。也就是说，常量函数和非常量函数如果形参一样，函数名一样也是可以实现函数重载的。**

```cpp
class CTest
{
private:
	int m_num;
public:
	int test(int &);	//ok
	int test(int &) const;	//ok
};
```
**在普通函数中则不能通过const当做函数的特征。如下面两个普通函数在一起编译时重定义的。**

```cpp
const int test(int &);	
int test(int &);	//重定义，编译报错
```

### [8. 变长参数](#)
c语言 中对变长参数的实现在C++中是有更modern的实现的

**使用initializer_list**  
对于可变参数是同一类型，可以使用C++11新增的一个类型initialize_list。举例如下：

```cpp
#include <initializer_list>

int sum_initializer_list(std::initializer_list<int> list) {
	auto beg = list.begin();
	auto sum = 0;
	for (; beg != list.end(); ++beg) {
		sum += *beg;
	}
	return sum;
}

void test_sum_initilizer_list() {
	sum_initializer_list({1,2,3});
	sum_initializer_list({1,2,3,4});
}
```

**使用可变参数模板template<typename… Args>**  
在C++中可变参数被称为参数包(parameter package), 存在两种：模板参数包，表示零个或多个模板参数；函数参数包，表示零个或多个函数参数。举例如下：

```cpp
//Args 是模板参数包，args是函数参数包
template<typename T, typename... Args>
void foo(const T& t, const Args&... args);
```

### [9. 参数使用](#)
给函数传递参数有三种方式，引用，指针，按值传递。

* 如果需要通过参数修改原始值，可以使用引用，传递引用而不是整个数据对象，可以提高程序的运行速度！
* 对于使用传递的值而不作修改的函数， 小型数据类型，可以传值！
* 传递的参数值数组，一定要使用指针
* 如果数据对象是较大的结构，使用const指针 或 引用
* 类对象，使用const 引用
* 若需要修改调用函数中数据的函数，如果数据对象是内置数据类型，建议使用指针

### [10. 函数返回值](#)
**如果函数返回的是值，而不是引用，那么函数返回值只能用于右边值，如果函数返回返回引用说明函数返回可以作为左值！**

常用于面向对象中

```cpp
class resident final
{
private:
    animal pet;
    std::string resident_name;//姓名
    unsigned short age;//年龄
    std::valarray<double> scores; //成绩
public:
    /* 省略 */
    double &operator[](int index); // 返回引用 rresident[1] = 95.5; 可以当左值
    double operator[](int index) const; //返回值 double val = rresident[1]; 可以当右值
    ~resident();
};
```

#### [10.1 返回类](#)
如果你返回一个类，C++ 11提供移动构造函数以后， 默认会调佣移动构造函数，如下所示:。
```cpp
class animal{
    animal(const animal&& other) = delete;//禁止调用
}

animal getAnimal(std::string _name, int _age, std::string _typeName) {
	return animal(_name, _age, _typeName); 
}

int main(int args, char* argv[]) {
	animal kitty = getAnimal("cni", 18, "cat");//animal::animal(const animal&&)
	std::cout << kitty << "\n";
	return 0;
}
```