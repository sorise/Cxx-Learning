### [函数模板 Template 初步](#)
函数模板是通用的函数描述，也就是说它们使用泛型来定义函数，其中的泛型可用具体的类型替换。 通过将类型作为参数传递给模板，可以使得编译器生成该类型的函数。

-----
- [x] [1. 创建一个模板](#1-创建一个模板)
- [x] [2. 显示具体化](#2-显示具体化)
- [x] [3. 重载的模板](#3-重载的模板)
- [x] [4. 函数的选择问题](#4-函数的选择问题)
- [x] [5. decltype](#5-decltype)
- [x] [6. 后置用法](#6-后置用法)
- [x] [7. 函数类型](#7-函数类型)

-----

### 实践经验
你很容易发现，使用模板函数，很难去操作一个类型的内部值，例如结构体的成员，类的成员。 泛型和函数模板天然和集合操作相关，你不可以修改内部，但是可以对外部的集合位置做修改。例如排序，比较之类的！
函数模板是一个生成函数定义的方案。 编译器使用模板为特定类型生成函数定义，得到函数实例。 **要区分清楚 函数类型和函数指针**

### [1. 创建一个模板](#)
C++98 添加 typename 关键字之后，使用 typename 来从创建模板。之前使用关键字class来创建模板。template<class T> 实际使用需要 使用 函数<T>(参数) 调用！

```cpp
template<typename T>
void MySwap(T& x, T& y) {
    T temp = x;
    x = y;
    y = temp;
}

int x = 10, y = 20;
float one = 50.25, two = 79.56;
MySwap<int>(x, y);
MySwap<float>(one, two);

cout << x << y << endl;
cout << one << two << endl;
```

### [2. 显示具体化](#) 
如果你对某个类型有特殊的需要。例如你只想交换Job类型中的 salary, floor。 你可以告诉编译器指定模板创建特定的实例。 模板需要实例化 还可以具体化

#### [3.1 第三代具体化 IOS/ANSI C++ 标准](#)
* 对于给定函数名，可以有非模板函数，模板函数和显示具体化模板函数以及它们的重载版本
* 显式具体化的原型和定义应以 template<> 打头，并通过名称来制定类型
* 具体化优先于常规模板，而非模板函数优先于具体化和常规模板的原型！  非模板函数 > 具体化模板 > 常规模板

```cpp
#include<iostream>

struct Job
{
    char name[40];
    double salary;
    int floor;
};

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
//显示具体化
template<> void swap<Job>(Job& x, Job& y) {
    auto tempSalary = x.salary;
    auto tempFloor = x.floor;

    x.salary = y.salary;
    x.floor = y.floor;
    y.salary = tempSalary;
    y.floor = tempFloor;
}
template void swap<char>(char &, char &); //显式实例化

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

    Job j1 = { "engineer", 15000.0, 5000 };
    Job j2 = { "worker", 3500, 2000 };

    swap<Job>(j1, j2); //MySwap(j1, j2); 也可以这样写

    std::cout << "job one:" << j1.name << " " << j1.salary << std::endl;
    std::cout << "job two:" << j2.name << " " << j2.salary << std::endl;
    return 0;
}
```

### [3. 重载的模板](#) 
模板的重载是可以实现的!

```cpp
template<typename T>
void MySwap(T& x, T& y) {
    auto temp = x;
    x = y;
    y = temp;
}

template<typename T>
void MySwap(T x[], T y[], int size) {
    T temp;
    for (int i = 0; i < size; i++)
    {
        temp = x[i];
        x[i] = y[i];
        y[i] = temp;
    }
}

int list1[] = {15,52,69,78,45};
int list2[] = { 75,84,95,71,65 };
int size = sizeof(list1) / sizeof(int);
MySwap(list1, list2, size);

for (int i = 0; i < size; i++)
{
    cout << i << ":" << "list1 - " << list1[i] << " " << "list2 - " << list2[i] << endl;
}
```


### [4. 模板函数](#) 

#### [4.1 模板函数实例化](#)
如果函数模板本身并不会生成函数定义(不被使用某个类型调用)， 那么他只是一个用于生成函数定义的方案。 编译器使用模板为特定类型生成函数定义时，得到的是模板实例。 
swap<int>(a, b); 是 void swap(T& x, T& y) 的一个实例。 

C++ 现在支持 隐式实例化 显示实例化 显式具体化

* 隐式实例化： swap(1,2)这个过程就是一个隐式实例化的过程，它实例化生成了一个T为int的函数。因此我们得知，隐式实例化实际上就是模板函数根据传入的实参类型实例化一个函数的过程。
* 显式实例化： template void print_typeof<double> (double const&); 显示实例化也称为外部实例化。在不发生函数调用的时候讲函数模板实例化，或者在不适用类模板的时候将类模板实例化称之为模板显示实例化。
* 显式具体化：  template<> void swap<Job>(Job& x, Job& y)

显式实例化:用于类库 支持： 函数模板的显示实例化， 类模板的显示实例化
```cpp
//template [函数返回类型] [函数模板名]<实际类型列表>（函数参数列表）
//函数模板的显示实例化
template void func<int>(const int&);
```

#### [4.1.1 函数模板的显示实例化](#)
对于函数模板而言，不管是否发生函数调用，都可以通过显示实例化声明将函数模板实例化，格式为：

```cpp
//template [函数返回类型] [函数模板名]<实际类型列表>（函数参数列表）

template void func<int>(const int&);
```

#### [4.1.2 类模板的显示实例化](#)
对于类模板而言，不管是否生成一个模板类的对象，都可以直接通过显示实例化声明将类模板实例化，格式为：

```cpp
//template class [类模板名]<实际类型列表>

template class theclass<int>;
```



#### [4.2 函数版本选择](#) 
现在我们有了函数参数默认值，函数参数的隐式类型转换. 函数重载， 函数模板，函数模板重载。 那么编译器在编译过程中是如何选择具体的函数的呢？ 实现函数匹配。
**那个函数最省事，转换工作最少，就优先用哪个！**

第一步：创建候选函数列表。其中包含与被调用函数的名称相同的函数和模板函数。

第二步：使用候选函数列表创建可行函数列表。这些都是参数数目正确的函数，为此有一个临时转换序列，其中包括实参类型和相应的形参类型完全匹配的情况。

第三步：确定是否有最佳的可行函数，如果有则使用它，否则该函数调用出错。


**不会有什么傻子程序员，对一个函数名又些 模板函数，又模板函数重载，又非模板函数。搞一大堆，这不是傻逼吗？ 这是编译器需要考虑的特殊情况，防止底层错误，程序员就不要乱搞了。**

#### [4.3 需要注意的地方](#)
如果你显示调用模板函数，那么及时存在匹配的非模板函数也会调用 模板函数 <> 告诉编译器去找模板函数
```cpp
#include<iostream>

template<typename T>
void swap(T& x, T& y) {
    std::cout << "tmplate call function:" << std::endl;
    auto temp = x;
    x = y;
    y = temp;
};


void swap(int& x, int& y) {
    std::cout << "normal call function:"<< std::endl;
    int temp = x;
    x = y;
    y = temp;
};


int main(int argc, char const* argv[])
{
    int a = 10, b =20;
    //显式实例化
    swap<int>(a, b); //tmplate call function:
    swap<>(a, b); //tmplate call function:
    swap(a, b); //normal call function:

    std::cout<< "a: " << a << " b: " << b << std::endl;

    return 0;
}
```
如果你不适应 <> 调用模板函数，但是又没有非模板函数，那么编译器就会去调用模板函数

```cpp
#include<iostream>


template<typename T>
void swap(T& x, T& y) {
    std::cout << "tmplate call function:" << std::endl;
    auto temp = x;
    x = y;
    y = temp;
};

int main(int argc, char const* argv[])
{
    int a = 10, b =20;
    //显式实例化
    swap<int>(a, b); //tmplate call function:
    swap(a, b); //tmplate call function:

    std::cout<< "a: " << a << " b: " << b << std::endl;

    return 0;
}
```

### [5. decltype](#)
decltype 检查实体的声明类型，或表达式的类型和值类别。

```cpp
decltype ( 实体 )	//(C++11 起)
decltype ( 表达式 )	//(C++11 起)
```

如果实参是其他类型为 T 的任何表达式，且
* 如果 表达式 的值类别是亡值，将会 decltype 产生 T&&；
* 如果 表达式 的值类别是左值，将会 decltype 产生 T&；
* 如果 表达式 的值类别是纯右值，将会 decltype 产生 T。


```cpp
template<typename T1, typename T2>
?type? Anonymous(T1 x, T2 y){
    auto xpt = x + y;
    return xpt;
}
```

c++ 11 提供了decltype来解决这个问题 decltype被称作类型说明符，它的作用是选择并返回操作数的数据类型。


工作原理 decltype 并不会实际计算表达式的值，编译器分析表达式并得到它的类型。

#### [5.1 decltype + 变量](#)
decltype(var)完美保留了变量的类型。

1. decltype : 获取表达式的类型。在编译时推导出一个表达式的类型，并且不会计算表达式的值

```cpp
int age = 25;
const float score = 25;
const float & reference = score;

decltype(age) y; //将y 设置位 x的类型 即  int
decltype(score) t1; // const float 类型
decltype(score) t2; // const float & 类型

int scroes[] = {56,75,48,58,55};

decltype(scroes) numbers; //int 数组 类型 

std::cout <<  sizeof(numbers) << std::endl; //20
```

两层括号变引用
```cpp
int x = 5;
decltype ((x)) t = x; // int &
```
**重点:** **对指针的解引用会返回引用类型**
```cpp
const int val{15};
const int * const pt = &val; //指向常量的指针常量
decltype(*pt) dcl_p; //const int & 类型 这里会报错 因为引用必须初始化！
```

#### [5.2 decltype + 表达式](#)
当使用decltype(expr)的形式时，decltype会返回表达式结果对应的类型。

```cpp
int x = 0;
decltype(x) y = 1; 			// y ->  int
decltype(x+y) z = x + y; 		// z - >  int	
```

#### [5.3 decltype + 函数返回值](#)
C++中通过函数的返回值和形参列表，定义了一种名为函数类型的东西。它的作用主要是为了定义函数指针。 

```cpp
int int_func();
int x = 0;
decltype(int_func()) a = 0; 					//a -> int
int& int_func_l();					
decltype(int_func_l()) b = x;					// b -> int&
int&& int_func_rr();
decltype(int_func_rr()) c = 0;					// c -> int&&
```

#### [5.4 作用于函数结果](#)
这个方法 使用auto
```cpp
template<typename T1, typename T2>
auto Anonymous(T1 x, T2 y){
    decltype(x + y) xpt = x + y;
    return xpt;
}
std::cout <<  Anonymous<float, int>(35.85, 10) << std::endl; //45.85
```

#### [5.5 decltype + 函数](#)
**decltype作用于函数的时候返回的是函数类型，而不是函数指针。** 如果你需要的函数指针而不是函数类型，你需要显示的加上 *！

```cpp
int add(int a, int b) {
	return a + b;
}

decltype(add) Sum; //Sum的类型是函数 int(int,int)
decltype(add) *SumPtr; //Sum的类型是 函数指针 int(*)(int,int)
```

### [6. 后置用法](#)
返回类型的后置语法使用 auto 和decltype

后置返回类型
```cpp
template<typename T1, typename T2>
auto Anonymous(T1 x, T2 y) -> decltype( x + y )
{
    decltype(x + y) xpt = x + y;
    return xpt;
}


std::cout <<  Anonymous<float, int>(35.85, 10) << std::endl; //45.85
```

```cpp
int& foo(int& i);
float& foo(float& f);

template <typename T>
auto func(T& val) ->decltype(foo(val))
{
    return foo(val);
}
```

### [7. 函数类型](#)
声明一个函数指针是简单的，声明一个函数类型也是简单的！只写返回值类型和参数类型即可！

```cpp
returnValueType(paraType,paraType2,paraType3...);
```

```cpp
using F = int(int, int); //函数类型
using Fptr = int(*)(int, int); //函数指针

int add(int a, int b) {
	return a + b;
}

template<typename Func=int(int, int)>
int useAdd(Func F, int a, int b) {
	return F(a, b);
}
```


-----
[2022/8/27 20:35:50 第二次修订]