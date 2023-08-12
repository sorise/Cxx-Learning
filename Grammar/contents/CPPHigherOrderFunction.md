### [C++ 函数高阶用法](#)
**介绍**：C++实现函数功能不仅仅只有一种方法，还可以有函数对象，lambda表达式，它们被统称为可调用对象。

-----
- [x] [1. 函数对象](#1-函数对象)
- [x] [2. 内置函数对象](#2-内置函数对象)
- [x] [3. Lambda 引入](#3-lambda-引入)
- [x] [4. Lambda](#4-lambda)
- [x] [5. 包装器](#5-包装器)

-----

### [1. 函数对象](#)
很多STL算法都使用 **函数对象**——也叫做 **函数符（functor）、仿函数**。**函数符** 是可以 以函数方式与()结合使用的任意对象，这包含函数名、指向函数的指针和重载了()运算符的类对象（即定义了函数operator()()的类）！

* 定义函数对象的类，需要 **重载()运算符**！ 
* 函数对象在使用的时候可以像普通函数那样使用，可以有参数、有返回值
* 和普通函数不一样，函数对象可以有自己的状态！
* 可以作为参数被传递

```cpp
class AddBasicPoint
{
private:
  int bp;
public:
  AddBasicPoint(int _bp):bp(_bp){};

  int operator()(int val){
    return bp * val;
  }
};

AddBasicPoint abp(12);
int total = abp(45);

int total_ = AddBasicPoint(12)(45);
std::cout << "total: " << total << " total_:"<< total_ << "\n";
//total: 540 total_:540
```

#### [1.1 算法使用的函数参数](#)
算法 for_each(books.begin(), books.end(), ShowReview()); 的第3个参数可以是常规函数，也可以是 **函数符**。

```cpp
template<class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function f);
```

**遍历的时候给出索引的方法！**
```cpp
//声明一个函数符
class ShowScore
{
private:
  int start;
public:
  ShowScore(int idx):start(idx){};

  void operator()(float val){
     printf("[%d]: %.2f\n", start++, val);
  }
};
```
**传递给 for_each 函数**
```cpp
int main(int argc, char *argv[]){
    std::list<float> scores = {98.56, 98.5, 97.125, 96.3, 95.2, 98.234, 89.56};

    std::for_each(scores.begin(), scores.end(), ShowScore(0));
    return 0;
}
/*
[0]: 98.56
[1]: 98.50
[2]: 97.12
...
*/
```

#### [1.2 函数符概念](#)
正如STL定义了容器和迭代器概念一样，它也定义了一些关于函数符的概念：

* 生成器（generator） 是不用参数就可以调用的函数符；
* 一元函数（unary function）  是用一个参数可以调用的函数符；
* 二元函数（binary function） 是用两个参数可以调用的函数符；
* 返回bool值的函数是 **谓词（predicate）**；
* 返回bool值的一元函数是 **一元谓词（predicate）**； 
* 返回bool值的二元函数是 **二元谓词（binary predicate）。**

一些STL函数需要谓词参数或二元谓词参数。例如算法 sort() 函数的一个版本需要 **二元谓词** 来决定排序规则；list模板 有一个将谓词作为参数的 remove_if()成员，来判断是否要删除区间中满足条件的元素。
提供给 for_each() 的函数符应当是 **一元函数**，因为它每次用于一个容器元素。类使得我们能够自定义各种函数符，也可以在现有函数的基础上定义合适的类作为适配器函数符，即使得函数能够满足不同的接口。

```cpp
//一元谓词
bool predicate(const float & v){
  return v > 95.0?true:false;
}

int main(int argc, char *argv[]){
    std::list<float> scores = {78.56, 91.5, 87.125, 96.3, 95.2, 98.234, 89.56};

    int count = std::count_if(scores.begin(), scores.end(), predicate);
    printf("The count(score > 95.0): %d\n", count);
    //The count(score > 95.0): 3
    return 0;
}
```

#### [1.3 拓展设计](#)
拓展设计:  假设已经有了一个接受两个参数的模板函数:

```cpp
template <class T>
bool tooBig (const T & val, const T & lim)
{ 
    return val > lim; 
}
```
则可以使用类将它转换为单个参数的函数对象:
```cpp
template <class T>
class TooBig2
{ 
private:
    T cutoff;
public:
    tooBig (const T & t): cutoff(t) {}
    bool operator() (const T & v) { return tooBig<T> (v,cutoff); }
};
```
即可以这样做:
```cpp
TooBig2<int> tB100(100);
int x;
cin >> x;
if (tB100(x))          // same as if ( tooBig(x,100) )
```
类函数符TooBig2是一个函数适配器，使函数能够满足不同的接口。


#### [1.4 实现函数式编程](#)
函数式编程的意思就是，传入的参数是函数，函数返回的参数也是函数。

```cpp
class Func
{
private:
    int base;
public:
    Func(int _base):base(_base){};
    int operator()(int val){
        base += val;
        return base;
    }
    Func(const Func & other) = delete;
    Func &operator=(const Func & other) = delete;
    ~Func() { printf("over\n"); };
};


std::unique_ptr<Func> getBaseFunction(std::function<int(int)> f,int base){
   return std::unique_ptr<Func> (new Func(f(base))); 
};

int main(int argc, char *argv[]){
    std::unique_ptr<Func> f = getBaseFunction([](auto v){ return v * 10; } ,10);

    cout<< (*f)(20) << endl;
    cout<< (*f)(20) << endl;
    cout<< (*f)(20) << endl;

    return 0;
}
/*
120
140
160
over
*/
```

### [2. 内置函数对象](#) 
STL定义了多个基本函数符，它们执行诸如将两个值相加、比较两个值是否相等操作, 对于所有内置的算术运算符、关系运算符和逻辑运算符，STL都提供了等价的函数符。 
这些仿函数所产生的对象，用法和一般函数完全相同, 使用内建函数对象，需要引入头文件 #include\<functional\>  **感觉有点傻逼的样子**

分类如下所示：
* 算术仿函数
* 关系仿函数
* 逻辑仿函数

#### [2.1 算术仿函数](#)
实现四则运算， 除了negate是一元运算，其他的都是二元运算

* template`<class T> T plus<T>`  加法仿函数
* template`<class T> T minux<T>`  减法仿函数
* template`<class T> T multiplies<T>` 乘法仿函数
* template`<class T> T divides<T>` 除法仿函数
* template`<class T> T modulus<T>` 取模仿函数
* template`<class T> T negate<T>`  取反仿函数

```cpp
int total = std::plus<int>()(12, 45);
std::cout<< "total: " << total;
```

#### [2.2 关系仿函数](#)
功能描述：实现关系对比

* template`<class T> bool equal_to<T>`  等于
* template`<class T> bool not_equal_to<T>`  不等于
* template`<class T> bool greater<T>` 大于
* template`<class T> bool greater_equal<T>`  大于等于
* template`<class T> bool less<T>`  小于
* template`<class T> bool less_equal<T>`  小于等于

```cpp
int main(int argc, char const* argv[])
{   
  std::list<double> scores {85.5, 95.5, 78.5, 86.5, 99.5, 89.6, 91.10,79.5};
  scores.sort(std::greater<double>());//排个序

  std::for_each(scores.begin(), scores.end(), [](const double& score){
    std::cout << score << "\n";
  });
  return 0;
}
```
#### [2.3 逻辑仿函数](#)
功能描述：实现逻辑运算

* template`<class T> bool logical_and<T>`  逻辑与
* template`<class T> bool logical_or<T>`  逻辑或
* template`<class T> bool logical_not<T>` 逻辑非

```cpp
transform(v.begin(), v.end(), v2.begin(), logical_not<bool>());
```


### [3. Lambda 引入](#) 
**lambda函数**，可以使用三种方法给STL算法传递信息：函数指针、函数符和Lambda函数。这三种形式通称为函数对象。为了进行比较，可以看以下示例。现在我们想生成一个随机整数列表，并判断其中多少个整数可被3整除，多少个整数可被13整除，可以像下面一样使用STL算法 generate() 在其中填充随机数：

```cpp
#include <vector>
#include <algorithm>
#include <cmath>

std::vector<int> numbers(1000);
std::generate(vector.begin(), vector.end(), std::rand);
// 第三个参数是一个不接受任何参数的函数对象
// 这里该函数对象是一个指向标准函数rand()的指针
```
通过使用算法count_if()，很容易计算出有多少个元素可被3整除，其第三个参数应是一个返回 true 或false的函数对象。

#### [3.1 使用函数指针实现](#)
可使用下面的函数定义：

```cpp
bool f3(int x) { return x % 3 == 0; }
// 同样，判断元素能否被13整除，可使用如下函数定义
bool f13(int x) { return x % 13 == 0; }

// 然后旧可以使用如下语句调用：
int count3 = std::count_if(numbers.begin(), numbers.end(), f3);
int count13 = std::count_if(numbers.begin(), numbers.end(), f13);
```

#### [3.2 使用函数符实现](#)
函数符是一个类对象，可以通过定义类方法 operator()() 来向函数名那样使用它。就这个示例而言，函数符的优点之一是，可使用同一个函数符来完成两项计数任务：
```cpp
class f_mod
{
private:
    int dv;
public:
    f_mod(int d = 1) : dv(d) {}
    bool operator()(int x) { return x % dv == 0; }
};

// 然后像如下这样调用：
count3 = std::count_if(numbers.begin(), numbers.end(), f_mod(3));
count13 = std::count_if(numbers.begin(), numbers.end(), f_mode(13));
```

#### [3.3 lambda实现](#)
很简单的=就可以完成！

```cpp
count3 = std::count_if(numbers.begin(), numbers.end(), [](int x){ return x % 3 == 0; });
```

### [4. Lambda](#) 
lambda 匿名函数，也是一个类类型，重载了 operator( ) 默认情况下是const函数。使用[]替代了名称（这就是匿名的由来）；没有声明返回类型。返回类型相当于使用decltype根据返回值推断得到的，这里为bool。 [https://zh.cppreference.com/w/cpp/language/lambda](https://zh.cppreference.com/w/cpp/language/lambda)

**仅当lambda表达式完全由一条返回语句组成时，自动类型推断才管用；**

**需要使用新增的返回类型后置语法：**

```
[捕获参数列表](函数参数列表) mutable throw(类型)->返回值类型 {函数语句};
```


```cpp
[](double x)->double{ int y = x; return x - y; } // return type is double
```

**lambda 默认情况下是不能访问外界变量的！**  想要访问 外界变量需要一些额外的操作！

```cpp
int start = 0;
std::for_each(scores.begin(), scores.end(), [](float val){
  printf("[%d]: %.2f\n", start++, val);  //错误 无法访问 start
});
```

#### [4.1 lambda的捕获功能：[&]和[=]](#)
具体地说，lambda可访问作用域内的任何动态变量；要捕获要使用的变量，可 **将其名称放在中括号[]内**，，有些时候 lambda 表达式在使用它前不需要先捕获：**该变量是非局部变量，或具有静态或线程局部存储期（此时无法捕获该变量），或者该变量是以常量表达式初始化的引用**。

**指定单个变量**

* 如果指定了变量名，如 [z]， 将 **按值访问变量**；
* 如果在名称前加上&，如[&count]，将 **按引用访问变量**。

```cpp
std::list<float> scores = {78.56, 91.5, 87.125, 96.3, 95.2, 98.234, 89.56};

int start = 0;
std::for_each(scores.begin(), scores.end(), [&start](float val){
  printf("[%d]: %.2f\n", start++, val);
});
```

**还有指定多个的：**
* [&] 让我们能够 **按引用访问所有动态变量**；
* [=] 让我们能够 **按值访问所有动态变量**。

```cpp
std::list<float> scores = {78.56, 91.5, 87.125, 96.3, 95.2, 98.234, 89.56};

int start = 0;
std::for_each(scores.begin(), scores.end(), [&](float val){
  printf("[%d]: %.2f\n", start++, val);
});
```

还可混合使用这两种方式，例如
* [ted, &ed] 让我们能够按值访问ted以及按引用访问ed；
* [&, ted]让我们能够按值访问 ted以及按引用访问其他所有动态变量；
* [=, &ed]让我们能够按引用访问ed以及按值访问其他所有动态变量。

**不捕获也能访问的情况**，a不用捕获也能访问，其实 `[=]` 里面的等号可以不要。

```cpp
static int a = 42;

auto p = [=] {++a; };
std::cout << sizeof p << "\n";
p();
return a;
```






#### [4.2 有名称的lambda](#)
lambda表达式是匿名函数，实际上也可以给lambda表达式指定一个名称，如下所示：

```cpp
auto f = [](int x){ return x % 3 == 0; };
```

#### [4.3 有状态的lambda](#)
lambda 里面可以设计一些参数哦

```cpp
std::list<double> numbers(10);
//是否所有人都及格了
std::generate_n(numbers.begin(), numbers.size(), [n = 0] () mutable { n *= 2 ;return n++; });
```
start 并不是外部参数
```cpp
auto f = [start = 10](int t){
  return start + t;
};

std::cout<< f(34) << std::endl; //44
```
如果需要状态可以变化 需要加关键字 mutable
```cpp
auto f = [start = 10](int t) mutable {
  start += t;
  return start + t;
};

std::cout<< f(34) << std::endl; //78
std::cout<< f(34) << std::endl; //112
```

#### [4.4  泛型lambda](#)

使用auto自动推断类型，C++14 开始支持。

```cpp
auto func = [](auto value) { std::cout << value << std::endl; };
auto Tfunc = []<typename T>(T value) { std::cout << value << std::endl; };

func(20);
Tfunc(20);

func(15.21);
Tfunc(15.21);

func("hello world");
Tfunc("hello world");
```

#### [4.5  constexpr 泛型 lambda](#)

constexpr也可以修改lambda，比较lambda本质也是类，然后重载operator 函数。

```cpp
auto p2 = []() constexpr { return 5; };
constexpr int v =  p2();
```

#### [4.6 lambda 转函数指针](#)

这是支持的，最后是生成一个静态函数然后返回静态函数指针，然后静态函数里面创建lambda对象调用operator函数。

```cpp
void (*p)(const std::string&) = [](const std::string& name) {
    std::cout << "call me by " << name << std::endl;
};

p("remix");
```

C++ 翻译以后:

```cpp
class __lambda_5_35
{
public: 
    inline /*constexpr */ void operator()(const std::basic_string<char> & name) const
    {
      std::operator<<(std::operator<<(std::cout, "call me by "), name).operator<<(std::endl);
    }

    using retType_5_35 = void (*)(const std::string &);
    inline constexpr operator retType_5_35 () const noexcept
    {
      return __invoke;
    }

private: 
    static inline /*constexpr */ void __invoke(const std::basic_string<char> & name)
    {
      __lambda_5_35{}.operator()(name);
    }


};

using FuncPtr_5 = void (*)(const std::string &);
FuncPtr_5 p = static_cast<void (*)(const std::basic_string<char> &)>(__lambda_5_35{}.operator __lambda_5_35::retType_5_35());
p(std::basic_string<char>("remix", std::allocator<char>()));
```

### [5. 包装器](#)

包装器(wrapper)又名适配器(adapter),这些对象用于给其他编程接口提供更一致或更合适的接口。目前，我们的知识深度已知的可调用对象类型有：

* 函数指针
* 仿函数 / 函数对象
* lambda表达式

现在我们有一个函数模板,对于函数模板，编译器会根据实参，按照模板定义出一份特定的函数。
```cpp
template<class F, class T>
T useMod(F f, T x) //让T作为参数传递给F
{
  static int count = 0;
  count++; //看看调用了多少次
  std::cout << "count address: " << &count << " value:" << count << std::endl;
  return f(x);
}
```
接来下我们以函数指针、函数对象、lambda实现f功能！
```cpp
//函数指针
int mod_23_(int val){ return val % 23; }

//函数对象
class modValue
{
private:
  int mod;
public:
  modValue(int val = 23):mod(val){};
  int operator()(int v){
    return v % mod;
  }
  ~modValue(){};
};

int main(int argc, char *argv[]){
     //调用了两次  只产生了一个 static int count
     useMod(mod_23_ , 79);
     useMod(mod_23_ , 79);
     //函数符调用了三次  只产生了一个 static int count
     useMod(modValue(23) , 79);
     useMod(modValue(23) , 79);
     useMod(modValue(23) , 79);
     //lambda 每次都重写生成一个 static int count
     useMod([](int val){ return val % 23; }, 79);
     useMod([](int val){ return val % 23; }, 79);
     useMod([](int val){ return val % 23; }, 79);

    /**
     * @brief 
      count address: 0x55b236cbf164 value:1
      count address: 0x55b236cbf164 value:2
      count address: 0x55b236cbf168 value:1
      count address: 0x55b236cbf168 value:2
      count address: 0x55b236cbf168 value:3
      count address: 0x55b236cbf158 value:1
      count address: 0x55b236cbf15c value:1
      count address: 0x55b236cbf160 value:1
     */
    return 0;
}
```
我们在函数模板内部实现打印静态成员变量，发现三个类型调用打印的cout地址不一样。然而静态成员变量是属于函数的，一个函数的静态成员变量无论调用多少次都只有一份。这说明是三个不同的函数调用！
**这三个函数功能一样，但是仅仅是实现的方式不同，却产生了多个函数模板，这显然不是一个很高效的处理方案！**

**一个lambda表达式语句就生成一个自定义类型（仿函数），那么多次调用会根据模板产生非常多的函数。**

#### [5.1 C++ 提供的包装器](#)
C++11 提供了包装器，**包装器可以将可调用对象统一包装成一个类型**。function就是一个包装器，也可称为适配器 function!

```cpp
#include <functional>

template <class Ret, class... Args> 
class function<Ret(Args...)>;
//Ret(Args…)：第一个模板参数类型（参数包）
//Ret是返回类型
```

**function<Ret(Args...)> 对象名 = 函数指针**
```cpp
//例如
int Sub(int x, int y)
{
    return x - y;
}

int main(void)
{
    //包装函数指针
    function<int(int, int)> f1 = Sub;
    cout << f1(12, 8) << endl;
    return 0;
}
```

**静态类成员函数指针:  function<Ret(Args……) 对象名 = &类域::函数指针** & 可以不加，不影响结果，但是加上要更优一些
```cpp
class Sub
{
public:
	static int SubStatic(int x, int y)
	{
		return (x - y);
	}
};
int main(void)
{
	function<int(int, int)> f2 = &Sub::SubStatic;
	cout << f2(10, 3) << endl;
	function<int(int, int)> f3 = Sub::SubStatic;
	cout << f2(10, 3) << endl;
	return 0;
}
```

**非静态类成员函数指针： function<Ret(类域名， Args……) 对象名 = &类域::函数指针**
```cpp
class Sub
{
public:
	int SubMember(int x, int y)
	{
		return (x - y);
	}
};
int main(void)
{
	function<int(Sub, int, int)> f4 = Sub::SubMember;
	cout << f4(Sub(), 3, 1) << endl;
	return 0;
}
```

**function<Ret(Args……) 对象名 = 仿函数类()**
```cpp
class Sub
{
public:
    int operator()(int x, int y)
    {
        return (x - y);
    }
};
int main(void)
{
    function<int(int, int)> f2 = Sub();
    return 0;
}
```


**function<Ret(Args……) 对象名 = lambda表达式**
```cpp
function<double(double, double)> f5 = [](double x, double y)mutable->double {return x - y; };
cout << f5(2.23, 1.11) << endl;
```


#### [5.2 用包装器重写 useMod](#)
首先我们改造 **useMod**

```cpp
template<class T>
T useMod(std::function<T(T)> f, T x)
{
  static int count = 0;
  count++;
  std::cout << "count address: " << &count << " value:" << count << std::endl;
  return f(x);
}
```

```cpp
#include<iostream>
#include<functional>

int mod_23_(int val){ return val % 23; }

class modValue
{
private:
  int mod;
public:
  modValue(int val = 23):mod(val){};
  int operator()(int v){
    return v % mod;
  }
  ~modValue(){};
};

int main(int argc, char *argv[]){

     std::function<int(int)> f1 = mod_23_;

     useMod<int>(mod_23_, 79);
     useMod<int>(modValue(23) , 79);
     useMod<int>(modValue(23) , 79);
     useMod<int>(modValue(23) , 79);
     useMod<int>([](int val){ return val % 23; }, 79);
     useMod<int>([](int val){ return val % 23; }, 79);
     useMod<int>([](int val){ return val % 23; }, 79);

    /**
     * @brief 
      count address: 0x560bf6dd6154 value:1
      count address: 0x560bf6dd6154 value:2
      count address: 0x560bf6dd6154 value:3
      count address: 0x560bf6dd6154 value:4
      count address: 0x560bf6dd6154 value:5
      count address: 0x560bf6dd6154 value:6
      count address: 0x560bf6dd6154 value:7
     */
    return 0;
}
```

**使用包装器以后 我们会发现，只产生了一个 static const int**

#### [5.3 function的解释](#)
我们查看 **function的定义** 会发现很牛逼的一个写法 **function<_Res(_ArgTypes...)>** 这个写法叫做： **模板偏特化** 
模板参数为函数类型 _Res(_ArgTypes...)），_Res为可调用目标的返回值类型，可调用目标的入参为 ... _ArgTypes。

```cpp
template<typename _Res, typename... _ArgTypes>
class function<_Res(_ArgTypes...)>
  : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>
  , private _Function_base
{
private:
    using _Invoker_type = _Res (*)(const _Any_data&, _ArgTypes&&...);
    _Invoker_type _M_invoker;
// ... ...
// 还有很多成员函数
};
```


-----
时间: [] 