### [C++ 异常与运行阶段类型识别](#)
终止、断言、异常处理！

-----
- [x] [1. 异常处理](#1-异常处理)
- [x] [2. 使用异常](#2-使用异常)
- [x] [3. 异常规范](#3-异常规范)
- [x] [4. 栈解退](#4-栈解退)
- [x] [5. catch引用与继承族](#5-catch引用与继承族)
- [x] [6. exception类](#6-exception类)
- [x] [7. 迷失方向的异常](#7-迷失方向的异常)

-----

### [1. 异常处理](#)
在C语言中处理错误的方式和缺陷有： **返回错误码** 和 **终止程序** 比如：发生越界，除0，内存错误等，会直接终止程序。 错误码不好设置，比如：除0操作，就不好返回错误码。需要程序员去查找错误码对应的含义。

C++针对上面的不足，引入了异常的概念，不会终止程序，异常是面向对象语言处理错误的一种方式。当一个函数出现自己无法处理的错误时，可以抛出异常，然后由直接或者间接调用者处理这个错误。

异常提供了一种转移程序控制权的方式。C++ 异常处理涉及到三个关键字：try、catch、throw。

* throw: 当问题出现时，程序会抛出一个异常。这是通过使用 throw 关键字来完成的。
* catch: 在您想要处理问题的地方，通过异常处理程序捕获异常。catch 关键字用于捕获异常。
* try: try 块中的代码标识将被激活的特定异常。它后面通常跟着一个或多个 catch 块。

```cpp
double division(int a, int b)
{
   if( b == 0 )
   {
      throw "Division by zero condition!";
   }
   return (a/b);
}

int main(int argc, char const* argv[])
{   
   int x = 50, y = 0;;
   double z = 0;
   try {
        z = division(x, y);
        std::cout << z << std::endl;
   }catch (const char* msg) {
        std::cerr << msg << std::endl;
   }
    return 0;
}
```

#### [1.1 内置终止函数](#)
C++ 内置了一些用于终止程序的函数， 情况如下所示！

##### [1.1.1 abort 和 exit函数](#)
exit和abort都是用来终止程序的函数，他们的不同如下：

* abort() : 立即结束，不做任何操作.
* exit(): 释放所有静态全局的对象、缓存，关掉所有的I/O通道，然后终止程序。

##### [1.1.2 abort函数](#)
abort()函数的原型位于头文件cstdlib（或stdlib.h）中，其典型实现是向标准错误流（即cerr使用的错误流）发送abnormal program termination（程序异常终止），然后终止程序。 调用abort()时，不进行任何清理工作，直接终止程序。

```cpp
double hmean(double a, double b){
	if (a == -b){
		cout << "unenable arguments to hmean()" << endl;
		abort();
	}
	return 2.0*a*b / (a + b);
}
```

##### [1.1.3 assert断言](#)
assert 宏的原型定义在<assert.h>中，其作用是如果它的条件返回错误，则终止程序执行。
原型定义：
```cpp
#include <assert.h>
void assert( int expression );
```
assert的作用是先计算表达式expression，如果其值为假（即为0），那么它先向std::error打印一条错误信息，然后通过调用 **abort()** 来终止程序运行。

```cpp
fp = fopen( "noexitfile.txt", "r" );
//以只读的方式打开一个文件，如果不存在就打开文件失败

assert( fp );                       
//所以这里出错
assert(nNewSize >= 0);
```

```cpp
int main(int argc, char const* argv[])
{   
   int i = 100;
   assert(i++ < 100);
   //runner: index.cpp:19: int main(int, const char**): Assertion i++ < 100' failed.
   return 0;
}
```
频繁的调用assert函数会极大的影响程序的性能，增加额外的开销。


##### [1.1.4 NDEBUG](#)
assert的行为依赖于一个名为NDEBUG的预处理变量的状态。如果定义了NDEBUG，则assert什么也不做！默认情况下没有定义NDEBUG。

* assert是一种预处理宏。对于assert(expr);，当expr表达式为0时，assert输出信息并终止程序运行；当expr表达式非0时，什么也不做。assert定义在cassert头文件中。
* assert的行为依赖于另一个宏NDEBUG，如果定义了这个宏，assert(expr)就不会执行任何操作，可以避免判断expr的开销。

```cpp
#define NDEBUG 

int main(int argc, char *argv[]){
    const int len = 7;
    #ifndef NDEBUG
        std::cerr << __func__ << " - 函数名" << endl;
        std::cerr << __FILE__ << " - 文件名" << endl;
        std::cerr << __LINE__ << " - 多少行" << endl;
        std::cerr << __TIME__ << " - 时间" << endl;
        std::cerr << __DATE__ << " - 日期" << endl;
    #endif
    return 0;
}
```

C++ 编译器预定义很多预处理变量
* __func__  存放函数名的静态数组，默认输出当前所在的函数名
* __FILE__  存放文件名的字符串的字面值
* __LINE__  存放当前行号的整形字面值
* __TIME__  存放文件编译时间的字符串字面值
* __DATE__  存放文件编译日期的字符串字面值




### [2. 使用异常](#) 
C++ 异常是对程序运行过程中发生的异常情况的一种相应。异常提供了将控制权从程序的一个部分传递到另一部分的途径。对异常的处理有三个组成部分！

* **引发异常 / 抛出异常** thorw 关键字表示引发异常，紧随其后的值指出了异常的特征！ 
* **将可能产生异常的代码放进try块中** try 表示其块中的代码块可能会引发异常！
* **在catch中使用处理程序捕获异常** 异常处理程序也被称为catch块，以关键字catch开头，随后是位于括号中的类型声明。它指出了异常处理程序要响应的异常类型；然后是一个用花括号扩起的代码块， try后面可以用跟随多个catch块！

```cpp
double division(int a, int b)
{
   if( b == 0 )
   {
      throw "Division by zero condition!";
   }
   return (a/b);
}

int main(int argc, char const* argv[])
{   
   try
   {
        int numbers[3] = {95,85,76};
        int index = 5;
        if (index < 0 || index > 2)
        {
            throw std::out_of_range("数组访问越界");
        }else{
            int a = numbers[index]; //这里没有异常，确实没有，编译器不检查越界错误
        }        
        division(10, 0);
   }
   catch(const std::exception& e)
   {
        std::cerr << e.what() << '\n';
   }
   catch(const char *error){
        std::cerr<< error << '\n';
   }
   return 0;
}
```
catch 捕获的异常类型可以是字符串，也可以是C++其他类型！通常来说位类类型！

#### [2.1 throw 关键字](#)
首先是引发异常，throw语句实际上是跳转，即命令程序跳到另一条语句。throw关键字表示引发异常，紧随其后的值（例如字符串或对象）指出了异常的特征。

执行throw语句类似于执行返回语句，因为它也将终止函数的执行；但throw不是将控制权返回给调用程序，而是导致程序沿函数调用序列后退，直到找到try块的函数。

#### [2.2 catch 关键字](#)
程序使用异常处理程序 （exception handler） 来捕获异常，异常处理程序位于要处理问题的程序中。catch关键字表示捕获异常。

处理程序以关键字catch开头，随后是位于括号中的类型声明，它指出了异常处理程序要响应的异常类型；然后是一个用花括号括起的代码块，指出要采取的措施。
catch关键字和异常类型用作标签，指出当异常被引发时，程序应该跳到这个位置执行。

异常处理程序也被称为catch块。
catch块点类似于函数定义，但并不是函数定义。关键字catch表明这是一个处理程序。另外，当异常于该处理程序匹配时，程序将执行括号中的代码。


捕获异常时的注意事项：
1. catch子句中的异常说明符必须是完全类型，不可以为前置声明，因为你的异常处理中常常要访问异常类的成员。例外：只有你的catch子句使用指针或者引用接收参数，并且在catch子句内你不访问异常类的成员，那么你的catch子句的异常说明符才可以是前置声明的类型。
2. catch的匹配过程是找最先匹配的，不是最佳匹配。
3. catch的匹配过程中，对类型的要求比较严格。不允许标准算术转换和类类型的转换。（类类型的转化包括两种：通过构造函数的隐式类型转化和通过转化操作符的类型转化）。
4. 和函数参数相同的地方有： 
     1. 如果catch中使用基类对象接收子类对象，那么会造成子类对象分隔（slice）为父类子对象（通过调用父类的复制构造函数）； 
     2. 如果catch中使用基类对象的引用接受子类对象，那么对虚成员的访问时，会发生动态绑定，即会多态调用。 
     3. 如果catch中使用基类对象的指针，那么一定要保证throw语句也要抛出指针类型，并且该指针所指向的对象，在catch语句执行是还存在（通常是动态分配的对象指针）。
5. 和函数参数不同的地方有：
     1. 如果throw中抛出一个对象，那么无论是catch中使用什么接收（基类对象、引用、指针或者子类对象、引用、指针），在传递到catch之前，编译器都会另外构造一个对象的副本。也就是说，如果你以一个throw语句中抛出一个对象类型，在catch处通过也是通过一个对象接收，那么该对象经历了两次复制，即调用了两次复制构造函数。一次是在throw时，将“抛出到对象”复制到一个“临时对象”（这一步是必须的），然后是因为catch处使用对象接收，那么需要再从“临时对象”复制到“catch的形参变量”中； 如果你在catch中使用“引用”来接收参数，那么不需要第二次复制，即形参的引用指向临时变量。 
     2. 该对象的类型与throw语句中体现的静态类型相同。也就是说，如果你在throw语句中抛出一个指向子类对象的父类引用，那么会发生分割现象，即只有子类对象中的父类部分会被抛出，抛出对象的类型也是父类类型。（从实现上讲，是因为复制到“临时对象”的时候，使用的是throw语句中类型的（这里是父类的）复制构造函数）。 
     3. 不可以进行标准算术转换和类的自定义转换：在函数参数匹配的过程中，可以进行很多的类型转换。但是在异常匹配的过程中，转换的规则要严厉。
     4. 异常处理机制的匹配过程是寻找最先匹配（first fit），函数调用的过程是寻找最佳匹配（best fit）。

#### [2.3 默认catch语句](#)
有时候可能不知道会发生哪些异常。例如编写了一个调用另一个函数的函数，而并不知道被调用的函数可能引发哪些异常。为了捕获这些未知类型的异常，可以使用省略号来表示异常类型，从而捕获任何异常：

#### 2.4 异常默认处理方式
如果函数引发了异常，而没有try块或没有匹配的处理程序时，将会调用abort()函数。

在默认情况下，程序最终将调用abort()函数，但可以修改这种行为（迷失方向的异常 ）。

```cpp
catch (...) { // statements }
// catch any type exception
```
使用例子:

```cpp
try
{
   throw "cnm";
}
catch(const std::exception& e)
{
   std::cerr << e.what() << '\n';
}catch(...){
   std::cerr << "捕获了未处理异常！" << '\n';
}
```
这有点类似于switch语句中的default。

#### 2.5 将对象用作异常类型
通常，引发异常的函数将传递一个对象。这样做的重要优点之一是，可以使用不同的异常类型来区分不同的函数在不同情况下引发的异常。另外，对象可以携带信息，程序员可以根据这些信息来确定引发异常的原因。同时，catch块可以根据这些信息来决定采取什么样的措施。

空指针异常类
```cpp
class NullPointerException
{
private:
   std::string message;
public:
   NullPointerException(std::string error_msg): message(error_msg){};
   NullPointerException(const NullPointerException & other);
   NullPointerException(const NullPointerException && other);
   NullPointerException& operator=(const NullPointerException & other);
   inline std::string getMessage() const { return message; };
   virtual ~NullPointerException(){};
};
NullPointerException::NullPointerException(const NullPointerException & other){
   message = other.message;
}
NullPointerException::NullPointerException(const NullPointerException && other){
   message = other.message;
}
NullPointerException& NullPointerException::operator=(const NullPointerException & other){
   if (&other == this) return *this;
   message = other.message;
   return *this;
};
```

```cpp
#include <iostream>
#include <string>
#include<assert.h>
int sum(int *ary = nullptr, int len = 0){
   if(ary == nullptr) throw NullPointerException("the pointer you passed in is null");
   int total = 0;
   for (int i = 0; i < len; i++)
   {
      total += ary[i];
   }
   return total;
}

int main(int argc, char const* argv[])
{   
   int *all = nullptr;

   try
   {
      sum(all, 10);
   }
   catch(const NullPointerException& e) //捕获引用
   {
      //the pointer you passed in is null
      std::cerr << e.getMessage() << '\n';
   }
   return 0;
}
```
#### 2.6 将字符串作异常类型
看看就知道了，你可能疑惑error需不需要 delete [] error! 我只能说我也不知道啊！
```cpp
try
{
   throw "cnm";
}
catch(const char * error){
   std::cerr << error << '\n';
}
```

### [3. 异常规范](#) 
异常规则说明说明的目的是为了让函数使用者知道该函数可能抛出什么异常，在函数后面接throw，列出这个函数可能抛出的所有异常类型； **c++98新增， c++11 废弃！**

```cpp
void func() throw(string, char, char*);//可抛出三种类型的异常
void* operator new(size_t) throw(bad_alloc);//只会抛bad_alloc异常
```
函数后面接throw()，表示不会抛出异常；
```cpp
void func() throw();//不抛异常
void* operator new(size_t) throw();//不抛异常
```
如果没有异常接口声明，则可以抛任意类型的异常。

#### [3.1 noexcept修饰符和noexcept操作符](#)
C++确实支持一种特殊的异常规范： noexcept 表示函数不会引发异常，在CPP11中，如果noexcept修饰的函数抛出了异常，那么编译器会调用std::terminate()直接终止运行了，

```cpp
void may_throw(); // 可能抛出异常
void no_throw() noexcept; //不可能抛出异常
```
为true表明函数不会抛出异常，false则会抛出异常
```cpp
void fun() noexcept(/* 常量表达式 */) {... }
```
虽然noexcept关键字修饰的函数，只是调用了std::terminate()函数，简单暴力的结束了程序的运行，带来了很多问题，比如析构函数没有被调用，栈没有被释放啊等等，但是他被广泛的用在C++11的标准库中，用来提高标准库的性能，以及满足一些阻止异常扩散的需求

nexcept做操作符时，通常用于模板：

```cpp
template<class T>
void func() noexcept(noexcept(T())){}
```
这里，func函数是否时一个noexcept函数，将由T()表达式是否会抛出异常所决定。如果参数是一个可能抛出异常的就返回false，否则返回true。

noexcept更大的作用是保护应用程序的安全，比如一个类的析构函数，就不应该抛出异常，那么对于常被析构调用的 delete函数来说，它也不应该抛出异常，所以C++11默认将delete函数设置为noexcept，提高应用程序的安全。

```cpp
void operator delete(void*) noexcept;
void operator delete[](void*) noexcept;
```

#### [3.2 析构于构造异常说明](#)
**1、保证不在构造函数中抛出异常，因为构造函数的作用是构造对象并初始化，一旦抛出异常，有可能导致对象不完整或没有完全初始化。**

**2、保证不在析构函数中抛出异常，因为析构函数的作用是完成资源的清理，一旦抛出异常，有可能导致资源泄漏（内存泄漏等等）**

#### [3.3 函数try语句块与构造函数](#)

要想处理构造函数初始值抛出的异常，我们必须将构造函数写成**函数try语句块（function try block）**的形式。

**处理构造函数初始值异常的唯一方法是将构造函数写成函数try语句块。**

```cpp
template <typename T>
Blob<T>::Blob(std::initializer_list il) try : 
data(std::make_shared<std::vector<T>>(il))
{}
catch (const std::bad_alloc& e)
{
    hand_out_of_memory(e);
}
```

### [4. 栈解退](#) 
try块可能没有直接调用引发异常的函数，而是调用了对引发异常的函数进行调用的函数，此时，需要栈解退（unwinnding the stack）步骤，即当函数由于出现异常而终止，则程序会释放从try块到throw之间整个函数调用序列放在栈中的对象。

因为函数返回而终止的函数，仅仅会处理该函数放在栈中的对象。

总之，程序进行栈解退以回到能够捕获异常的地方时，将释放栈中的自动存储型变量。如果变量是类对象，将为该对象调用析构函数。

#### [4.1 其他异常特性](#)
1. **控制权返回**：其中之一就是函数fun()中的返回语句将控制权返回到调用到fun()的函数，但throw语句将控制权向上返回到第一个这样的函数：包含能够捕获相应异常的try-catch组合。

2. **临时拷贝**：另一个不同之处就是，引发异常时编译器总是创建一个临时拷贝，即使异常规范和catch块中指定的是引用。

这主要是因为，很有可能throw后，该处函数创建的自动变量会被释放（要throw的变量也被释放），所以每次直接生成一个副本最为妥当。并且建议将引发异常和创建对象组合在一起，这样更简单：

```cpp
throw problem();  // construct and throw default problem object
```
所以为什么要用引用呢？毕竟将引用作为返回值的通常原因是避免创建副本以提高效率。

这是因为，引用还有另一个重要特征：**基类引用可以执行派生类对象。**

### [5.  catch引用与继承族](#) 
假设有一组通过继承关联起来的异常类型，则在异常规范中只需列出一个基类引用，它将与任何派生类对象匹配。

假设有一个异常类层次结构，并要分别处理不同的异常类型，则使用基类引用将能够捕获任何异常对象；而使用派生类对象只能捕获它所属类及从这个类派生而来的类的对象。

引发的异常对象将被和第一个与之匹配的catch块捕获。这意味着catch块的排列顺序应该与派生顺序相反：


```cpp
class bad_1 { /*...*/};
class bad_2 : public bad_1 { /*...*/};
class bad_3 : public bad_2 { /*...*/};
...
void duper()
{
    ...
    if (oh_no)
        throw bad_1();
    if (rats)
        throw bad_2();
    if (drat)
        throw bad_3();
}
...
try {
    duper();
}
catch(bad_3 &be)
{ // statements }
catch(bad_2 &be)
{ // statements }
catch(bad_1 &be)
{ // statements }
/*
如果将bad_1 &处理程序放在最前面，它将捕获bad_1、bad_2和bad_3；
通过按相反的顺序排列，bad_3异常将被bad_3 &处理程序所捕获。
*/
```

所以， **如果有一个异常类继承层次结构，应该这样排列catch块：将捕获位于层次结构最下面的异常类的catch语句放在最前面，将捕获基类异常的catch语句放在最后面。**

可以创建捕获对象而不是引用的处理程序。在catch语句中使用基类对象时，将捕获所有的派生类对象，但派生特性将被剥去，因此将使用虚方法的基类版本。

### [6. exception类](#)
它是 **C++** 的内置异常类型，也是其他许多的异常类型的积累性，**C++** 库定义了很多基于 **exception** 的异常类型。**C++**语言本身或者标准库抛出的异常都是 **exception** 的子类，称为**标准异常（Standard Exception）**

有一个名为what()的虚拟成员函数，它返回一个字符串，该字符串的特征随实现而异。可以这样定义自己的类：

* [**被 throw的异常是一个类的话，这个类必须具有一个可访问析构函数、可访问拷贝构造函数或者移动构造函数**](#)

```cpp
class exception
{
public:
   exception() _GLIBCXX_NOTHROW { }
   virtual ~exception() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
#if __cplusplus >= 201103L
   exception(const exception&) = default;
   exception& operator=(const exception&) = default;
   exception(exception&&) = default;
   exception& operator=(exception&&) = default;
#endif

   /** Returns a C-style character string describing the general cause
   *  of the current error.  */
   virtual const char*
   what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
};
```

基于 exception 继承一个自己的类

```cpp
#include <exception>
class bad : public std::exception
{
public:
	const char * what() { return "bad arguments"; }
...
};
```

#### [6.1 标准异常](#)
头文件stdexcept定义了其他几个异常类。首先，该文件定义了logic_error和runtime_error类，它们都是以公有方式从exception派!

```cpp
class logic_error : public exception {
public:
	explicit logic_error(const string& what_arg);
   /*...*/
};
class domain_error : public logic_error {
public:
	explicit domain_error(const string& what_arg);
   /*...*/
}
```
这些类的构造函数接受一个string对象作为参数，该参数提供了方法what()以C-风格字符串方式返回的字符数据。 这两个新类被用作两个派生类系类的基类。


#### [6.2 logic_error 派系](#)
异常类系列 logic_error 描述了典型的逻辑错误。
总体而言，通过合理的编程可以避免这种错误，但实际上这些错误还是可能发生的。每个类的名称指出了它用于报告的错误类型。

* **domain_error**：数学函数有定义域（domain）和值域（range），可以让参数不在定义域时引发domain_error。
* **invalid_argument**：指出给函数传递了一个意料外的值。例如，函数可能希望接受一个这样的字符串：其中每个字符要么是’0’要么是’1’，则当传递的字符串中包含其他字符时，该函数将引发invalid_argument异常。
* **length_error**：用于指出没有足的空间来执行所需的操作。例如，string类的append()方法在合并得到的字符串长度超过最大允许长度时，将引发length_error异常。
* **out_of_bounds**：通常用于指示索引错误。

#### [6.3 runtime_error 派系](#)
异常系列 runtime_error 描述了可能在运行期间发生但难以预计和防范的错误。

* **range_error**：计算结果可能不在函数允许的范围之内，但没有发生上溢或下溢错误，在这种情况下，用这个。
* **overflow_error**：上溢错误。整型和浮点型都可能发生上溢错误，当计算结果超过了某种类型能够表示的最大数量级时，将发生上溢错误。
* **underflow_error**：下溢错误。存在于浮点数计算中。一般而言，存在浮点类型可以表示的最小非零值，计算结果比这个值还小时将导致下溢错误。

一般而言，logic_error系列异常表明存在可以通过编程（或通过修改输入）修复的问题，而runtime_error系列异常表明存在无法避免的问题。


#### [6.4 bad_alloc异常和new](#)
对于使用new导致的内存分配问题，C++最新的处理方法是让new引发bad_alloc异常。头文件new包含bad_alloc类的声明，它是从exception类公有派生而来的。在以前，当无法分配请求的内存量时，new返回一个空指针。 
**定义在 new头文件**

```cpp
int main(int argc, char const* argv[])
{   
   try
   {
      double * scores = new double[1000000000000000];
   }
   catch(const std::bad_alloc& e)
   {
      std::cerr << e.what() << '\n';
      //std::bad_alloc
   }
   
   return 0;
}
```

#### [6.5 空指针和new（开启返回空指针）](#)
很多代码都是在new失败时返回空指针编写的。为处理new的变化，有些编译器提供了一个标记（开关），让用户选择所需的行为。当前，C++标准提供了一种在失败时返回空指针的new，其用法如下：

```cpp
int * pi = new (std::nothrow) int;       // 错误变成空指针
int * pa = new (std::nowthrow) int[500]; // 不会返回空指针，要用bad_alloc

double * scores = new (std::nothrow) double[1000000000000000];
if (scores == nullptr)
{
   std::cout<< "Failed to apply for allocating memory!" << std::endl;
}
delete [] scores;
```

#### [6.6 bad_cast异常](#)
由于强制转换为引用类型失败，dynamic_cast 运算符引发 bad_cast 异常。定义在 **type_info 头文件中**

### [7. 迷失方向的异常](#)
异常被引发后，在两种情况下，会导致问题。首先，如果它时在带异常规范的函数中引发的，则必须与规范列表中的某种异常匹配（在继承层次结构中，类类型与这个类及其派生类的对象匹配），否则称为意外异常（unexpected exception）。在默认情况下，这将导致程序异常终止。
**虽然C++11摒弃了异常规范，但仍支持它，且有些现有的代码使用了它。** 如果异常不是在函数中引发的（或者函数没有异常规范），则必须捕获它。如果没被捕获（没有try块或没有匹配的catch块时），则异常被称为未捕获异常（uncaught exception）。在默认情况下，这将导致程序异常终止。
然而，可以修改程序对意外异常和未捕获异常的反应。

#### [7.1 修改对未捕获异常的反应](#)
**未捕获异常不会导致程序立刻异常终止。程序首先调用函数terminate()。默认情况下，terminate()调用abort()函数，可以指定terminate()应调用的函数来修改这种行为。** 为此，可以调用set_terminate()函数。

**set_terminate()和terminate()都是在头文件exception中声明的。**

可以这样使用：

```cpp
#include <iostream>
#include <exception>

// 希望terminate()调用的自定义函数
// 时间
void myQuit() noexcept
{
	std::cout << "Terminating due to uncaught exception\\n";
	exit(5);
}

set_terminate(myQuit);
// 现在如果引发了一个异常且没有被捕获
// 程序将调用terminate()，后者将调用myQuit()
```

#### [7.2 修改对意外异常的反应](#)
如果发生意外异常，程序将调用unexpected()函数，这个函数将调用terminate()，后者在默认情况下将调用abort()。 也有一个可用于修改unexpected()的行为的函数set_unexpected()。

**他们也是在exception头文件中声明的。**

提供给set_unexpected()的函数的行为收到更严格的限制。具体地说，这个函数可以：

通过调用terminate()（默认行为）、abort()或exit()来终止程序；引发（想要的）异常。
可以引发一个异常规范中的异常，也可以引发exception头文件中的bad_exception类，然后再在异常规范中添加这个类。

如果你需要捕获所有的异常，你可以这么做，设计一个替代函数，将意外异常转换为 bad_exception 异常！ bad_exception

```cpp
void unexpected_hander_(){
   throw std::bad_exception();
}

try{
   /**/
}
catch(/**/){

}
catch(bad_exception &ex){
   /* ... */
}
```
标准 C++ 包含多个 内置 异常类，exception::bad_exception 是其中之一。这是意外处理程序抛出的异常。

#### [7.3 有关异常的注意事项](#)
使用异常时，应在设计程序时就加入异常处理功能，而不是以后再添加。虽然异常处理对于某些项目极为重要，，但它也会增加编程的工作量、增大程序、降低降序的速度。另一方面，不进行错误检查的代价可能非常高。

异常规范不适用于模板，以为模板函数引发的异常可能随特定的具体化而异。

异常和动态内存分配并非总能协同工作，比如下面这段代码：

```cpp
void test(int n)
{
	std::string mesg("I'm trapped in an endless loop");
	double * ar = new double[n];
	...
	if(oh_no)
		throw exception()
	...
	delete [] ar;
}
```
抛出异常后，函数中断，多亏于栈解退，string类对象的内存会被正确释放，但是delete语句没有被使用，这将导致内存泄漏。但这种内存泄漏是可以避免的，比如可以在引发异常的函数中捕获该异常，在catch块中包含一些清理代码，然后重新引发异常：

```cpp
void test(int n)
{
	std::string mesg("I'm trapped in an endless loop");
	double * ar = new double[n];
	...
	try {
		if(oh_no)
			throw exception()
	}
	catch(exception & ex)
	{
		delete [] ar;
		throw;
	}
	...
	delete [] ar;
}
```


-----
时间: [2022/9/22 22:53] 