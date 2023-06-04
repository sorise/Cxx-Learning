### [C++ Template Class](#)
C++ 模板类！模板天生和集合，容器操作相关！ 模板是泛型编程的基础，泛型编程即以一种独立于任何特定类型的方式编写代码。

-----
- [x] [1. 模板类基础](#1-模板类基础)
- [x] [2. 模板类型是指针](#2-模板类型是指针)
- [x] [3. 非类型参数](#3-非类型参数)
- [x] [4. 模板具体化](#4-模板具体化)
- [x] [5. 将模板作为参数](#5-将模板作为参数)
- [x] [6. 模板类与友元](#6-模板类与友元)
- [x] [7. C++11 可变模板参数](#7-c11-可变模板参数)
- [x] [8. C++11 std::forward](#8-c11-stdforward)

-----

### [1. 模板类基础](#)
模板是创建泛型类或函数的蓝图或公式。库容器，比如迭代器和算法，都是泛型编程的例子，它们都使用了模板的概念。 定义一个类模板要使用到 template关键字！

```cpp
template <class T>
class class_Name{
    /*成员函数和成员变量*/
};
```
也可以多类型

```cpp
template <class T1, class T2>
class class_Name{
    /*成员函数和成员变量*/
};
```

* template声明创建模板
* typename表明其后面的符号是一种数据类型，可以用class代替
* T是通用的数据类型，名称可以替换，通常为大写字母

**例子**
```cpp
template<typename T>
class Stack
{
private:
    T *container; //容器指针
    int len;
    int capacity;
public:
    Stack(/* args */);
    Stack(const int& capty);
    void push(T const&);  // 入栈
    void pop();           // 出栈
    T top() const;        // 返回栈顶元素
    bool empty() const;   // 如果为空则返回真。
    ~Stack();
};
```

#### [1.1 typename和class](#)
在 C++ Template 中很多地方都用到了 typename 与 class 这两个关键字，而且好像可以替换, class 用于定义类，在模板引入 c++ 后，最初定义模板的方法为：

```cpp
template<class T> //......
```
这里 class 关键字表明T是一个类型，后来为了避免 class 在这两个地方的使用可能给人带来混淆，所以引入了 typename 这个关键字，它的作用同 class 一样表明后面的符号为一个类型，这样在定义模板的时候就可以使用下面的方式了：
```cpp
template<typename T>......
```
在模板定义语法中关键字 class 与 typename 的作用完全一样。typename 难道仅仅在模板定义中起作用吗？其实不是这样，typename 另外一个作用为：使用嵌套依赖类型(nested depended name)，如下所示：
```cpp
class TArray 
{ 
    public:
    typedef int LengthType;
    //.....
}

template<class T>
void MyMethod( T myarr ) 
{ 
    typedef typename T::LengthType LengthType; 
    LengthType length = myarr.GetLength; 
}
```
这个时候 typename 的作用就是告诉 c++ 编译器，typename 后面的字符串为一个类型名称，而不是成员函数或者成员变量，这个时候如果前面没有 typename，编译器没有任何办法知道 T::LengthType 是一个类型还是一个成员名称(静态数据成员或者静态函数)，所以编译不能够通过。

#### [1.2 成员方法](#)
模板类的方法定义部分和模板函数一样，也需要模板类型说明，同时类名限定需要改成 class_Name<T>


构造函数
```cpp
template<typename T>
Stack<T>::Stack(const int& capty){
    capacity = capty;
    container = new T[capacity]; 
    len = 0;
}
```

成员方法
```cpp
template<typename T>
bool Stack<T>::empty() const{
    return len == 0;
}
```

#### [1.3 模板类的位置](#)
模板类不是具体的类的定义，他是一组告诉编译器如何生成类的指令模板！ 所以对于模板类，应该整体放到 .h文件中！而不需要 分开防止到 .h和cpp文件中！

编译器何时利用模板来生成独立的类声明和独立的类方法呢？ 如下所示的时候！

```cpp
stack<int> istack;
stack<double> dstack;
```

#### [1.4 模板的多功能性](#)
可以将用于常规类的技术用于模板类。模板类 可用作基类，也可以用作组件类，还可用作其他模板的类型参数

```cpp
template<typename T>
class LinkedStack: public Stack<T>{
    /* ... */
}

//模板为类型
Array<Stack<double>, 10> myArray;
```

##### [1.4.1 成员模板](#)
模板作为成员是非常需要的，STL中经常使用！

```cpp
template<typename T, unsigned int N>
class Array final
{
private:
    T *container;
    std::stack<T> _t_stack;
public:
    Array(std::initializer_list<T> list);
    Array(const Array<T,N>& other);
    Array(const Array<T,N>&& other);
    Array<T,N>& operator=(Array<T,N>& other);
    template<typename U>
    std::string getTypeName(U typeVariable);
    int size() const;
    T &operator[](int idx);
    ~Array();
};

template<typename T, unsigned int N>
template<typename U>
std::string Array<T,N>::getTypeName(U typeVariable){
    std::string typeName(typeid(typeVariable).name());
    return typeName;
}
```

使用方法
```cpp
const int len = 5;
Array<double, len> scores = {95.5,85.0,96.25,94.45,90.5};
std::cout <<  scores.getTypeName<int>(len) << std::endl;//i
std::cout <<  scores.getTypeName(scores) << std::endl;//5ArrayIdLj5EE
```

#### [1.5 默认模板参数](#)
模板默认参数，默认参数是也是模板。
```cpp
template <typename T, typename F = less<T>>
int compare(const T &before, const T &after, F f = F()){
    if(f(before, after)) return -1;
    if(f(after, before)) return 1;
    return 0;
}
```

#### [1.6 模板默认实参](#)

模板默认参数，和函数参数差不多。

```cpp
template<typename T1, typename T2 = int>
class keyValue{
    /*  */
}

keyValue<double, double> k1;
keyValue<double> k2; // T2使用默认类型 int
```



#### [1.7 stack 例子](#)

自己使用 delete new 实现的一个栈！

```cpp
#ifndef _STACK_H_REMIX_
#define _STACK_H_REMIX_
//我虽无意逐鹿，却只苍生苦楚
template<typename T>
class Stack final //不让继承
{
private:
    static const int SIZE = 8;
    T *container; //容器指针
    int len;
    int capacity;
public:
    Stack(/* args */);
    Stack(const Stack<T> & other);
    Stack(const int& capty);
    void push(T const&);  // 入栈
    bool pop();           // 出栈
    T top() const;        // 返回栈顶元素
    bool empty() const;   // 如果为空则返回真。
    Stack<T> &operator=(const Stack<T>& other);
    ~Stack();
};

template<typename T>
Stack<T>::Stack(/* args */)
{
    capacity = SIZE;
    container = new T[SIZE]; 
    len = 0;
}

template<typename T>
Stack<T>::Stack(const Stack<T> & other){
    if (other.len = 0)
    {
        capacity = SIZE;
    }else{
        capacity = other.capacity;
    }
    len = 0;
    container = new T[capacity]; 
    for(len = 0; len < other.len; len++)
    {
        container[len] = other.container[len];
    }
}
template<typename T>
Stack<T> & Stack<T>::operator=(const Stack<T>& other){
    if (&other == this); return (*this);
    capacity = other.capacity;
    len = 0;
    delete [] container;//把自己的内容进行释放
    container = new T[capacity]; //重写申请
    for(len = 0; len < other.len; len++)
    {
        //赋值内容
        container[len] = other.container[len];
    }
    return *this;
}

 

template<typename T>
Stack<T>::Stack(const int& capty){
    capacity = capty;
    container = new T[capacity]; 
    len = 0;
}

/* 入栈 */
template<typename T>
void Stack<T>::push(T const& val){
    if (len == capacity)
    {
        /* code  增加一倍容量 */ 
        capacity *= 2;
        //完成复制
        T* container_tem_pointer = new T[capacity]; 
        for (int i = 0; i < len; i++)
        {
            container_tem_pointer[i] = container[i];
        }
        //释放掉
        delete [] container;
        container = container_tem_pointer;
    }
    //加入栈顶
    container[len++] = val;
}

// 返回栈顶元素
template<typename T>
T Stack<T>::top() const{
    if (len == 0)
    {
        throw "stack is empty!";
    }else{

    }
    return container[len - 1];
}        


// 出栈
template<typename T>
bool Stack<T>::pop(){
    if(len > 0) {
        len--;
        return true;
    };
    return false;

}

template<typename T>
bool Stack<T>::empty() const{
    return len == 0;
}

template<typename T>
Stack<T>::~Stack()
{
    delete [] container;
}
#endif
```

### [2. 模板类型是指针](#) 
如果模板类型是指针！ 这个问题，值得思考！

```cpp
Stack<int *> myStack(8);
```
* 首先你必须手动实现复制运算符！
* 必须有析构函数，如果不会再被继承一定要加上 final 关键字，如果会被继承，析构函数必须变成虚析构函数！
* 必须重载赋值运算符，访问浅拷贝问题！


```cpp
//mian dui ren ming qun zhong jiu yao bei dui shang ji ling dao!
int main(int argc, char const* argv[])
{
    char t1[] = {"i am your father!"};
    char t2[] = {"i wanna fuck the world"};
    char t3[] = {"save chinese! kill!"};
    Stack<char *> my_stack;

    my_stack.push(t1);
    my_stack.push(t2);
    my_stack.push(t3);

    while (!my_stack.empty())
    {
        std::cout << my_stack.top() << std::endl;
        my_stack.pop();
    }
    return 0;
}
/*
save chinese! kill!
i wanna fuck the world
i am your father!
*/
```
#### [2.1 为指针提供通用的模板](#)
如果提供的T是指针那么会使用下面的第二个模板来，如果不是指针会直接使用第一个模板类！

```cpp
template<typename T> class FeedBack { /* ... */ };

template<typename T *> class FeedBack { /* ... */ };
```

### [3. 非类型参数](#) 
对于模板类，模板参数不仅仅能够抽象一个类型，还支持让一个常量作为模板参数，在类模板中可以将该参数当作一个常量进行使用

非类型模板参数，我们尝试着将一个size_t 类型的常量作为模板参数

```cpp
typedef int size_t;
template<class T, size_t N>
class array
{
    /* ... */
}
```

#### [3.1 非类型参数的范围](#) 
非类型形参：用一个常量作为类(函数)模板的一个参数，在类(函数)模板中可将该参数当成常量来使用

非类型参数，也可以称为表达式参数，只能是常量表达式,常量 支持的类型有： 整型 枚举 引用 指针。

```cpp
Array<double, 2+3> scores = {95.5,85.0,96.25,94.45,90.5};

const int len = 5;
Array<double, len> _scores2 = {95.5,85.0,96.25,94.45,90.5};
```


* **浮点型数值或者class类型的对象不能作为非类型模板参数使用**
```cpp
//animal 是一个类
template<typename T, animal taidi> // 禁止

template<typename T, float number> //浮点数 不行
```

* **就算是指针，你会发现，只能输入指针常量，其他的一概不行** **只能是指向外部链接对象的指针**
```cpp
//animal.h
char animal_char = 'i';

//main.cpp
extern char animal_char; //向外部链接对象的指针

template<typename T, const char * cptr>
void giveStack(std::initializer_list<T> list){
    for (auto &&i : list)
    {
        std::cout<< i << std::endl;   
    }
}


char clist[] = {"wcnmd song jiang!"};
const char * ptr = nullptr;
char o = 'c';
const char *address = &o;
char * const cnt_ptr = &o;

giveStack<double, address>({95.2,85.2,65.2,95.2,84.2}); //错误 参数不匹配
giveStack<double, cnt_ptr>({95.2,85.2,65.2,95.2,84.2});//错误 参数不匹配
giveStack<double, clist>({95.2,85.2,65.2,95.2,84.2});//错误 参数不匹配
giveStack<double, ptr>({95.2,85.2,65.2,95.2,84.2});//错误 参数不匹配
giveStack<double, &o>({95.2,85.2,65.2,95.2,84.2});//错误 参数不匹配
giveStack<double, nullptr>({95.2,85.2,65.2,95.2,84.2}); //正确

giveStack<double, &animal_char>({95.2,85.2,65.2,95.2,84.2}); //正确
```


#### [3.2 例子 Array 模板类](#) 

```cpp
#ifndef _ARRAY_H_REMIX_
#define _ARRAY_H_REMIX_ 2
#include <initializer_list>
#include <string>

template<typename T, unsigned int N>
class Array final
{
private:
    T *container;
public:
    Array(std::initializer_list<T> list);
    Array(const Array<T,N>& other);
    Array(const Array<T,N>&& other);
    Array<T,N>& operator=(Array<T,N>& other);
    int size() const;
    T &operator[](int idx);
    ~Array();
};

template<typename T, unsigned int N>
Array<T,N>::Array(const Array<T,N>& other){
    if (N <= 0)
    {
        container = nullptr;
    }else{
        container = new T[N]; //再申请
        for (int i = 0; i < N; i++)
        {   //复制内容
            container[i] = other.container[i];
        }
    }
}

template<typename T, unsigned int N>
Array<T,N>::Array(const Array<T,N>&& other){
    if (N <= 0)
    {
        container = nullptr;
    }else{
        container = new T[N]; //再申请
        for (int i = 0; i < N; i++)
        {   //复制内容
            container[i] = other.container[i];
        }
    }
}

template<typename T, unsigned int N>
Array<T,N>& Array<T,N>::operator=(Array<T,N>& other){
    if (&other == this) return *this;
    delete [] container; //释放
    container = new T[N]; //再申请
    for (int i = 0; i < N; i++)
    {   //复制内容
        container[i] = other.container[i];
    }
    return *this;
}

template<typename T, unsigned int N>
int Array<T,N>::size() const{
    return (int)N;
}

template<typename T, unsigned int N>
Array<T,N>::Array(std::initializer_list<T> list)
{
    if (N <= 0)
    {
        container = nullptr;
    }else{
        container = new T[N];
        for (int i = 0; i < list.size(); i++)
        {
            container[i] = *(list.begin() + i);
        }
    }
}

template<typename T, unsigned int N>
T& Array<T,N>::operator[](int idx){
    if (idx >= N || idx < 0)
    {
        throw "index out of the array bound!";
    }
    return container[idx];
}

template<typename T, unsigned int N>
Array<T,N>::~Array()
{
    delete [] container;
}
#endif
```

使用:

```cpp
int main(int argc, char const* argv[])
{
    Array<double, 5> scores = {95.5,85.0,96.25,94.45,90.5};

    std::string rlt = "{";
    for (int i = 0; i < scores.size(); i++)
    {
        rlt += std::to_string(scores[i]) + " ";
    }
    rlt += "}";

    std::cout <<  rlt << std::endl;
    return 0;
}
```

### [4. 模板具体化](#)
类模板和函数模板类似，也有隐式实例化，显式实例化，显式具体化！ 具体化 就是 实例化的意思！

#### [4.1 隐式实例化](#)
最为推荐，也是最常用的！

```cpp
Stack<double> theshys;

Array<double, 10> *ary_ptr; //不会生成具体类
ary_ptr = new Array<double, 10>({95.5,85.0,96.25,94.45,90.5}); //会生成具体类 实例化
```
#### [4.2 显式实例化](#)
使用关键字 template并指出所需要的类型来声明类

```cpp
template class Array<animal, 10>; // 生成一个 Array<animal, 10> 类
```

#### [4.3 显式具体化](#)
为特定类型定义一个类, 这合适吗？函数模板还可以这样！类的话这样做也太麻烦了吧！

显式具体化例子：
```cpp
template <> class Array<int, 10>
{
private:
    int *container;
public:
    Array(std::initializer_list<int> list);
    Array(const Array& other);
    Array(const Array&& other);
    Array& operator=(const Array& other);
    int size() const;
    int &operator[](int idx);
    ~Array();
};

Array<int, 10>::Array(std::initializer_list<int> list){
    container = new int[10];
    for (int i = 0; i < list.size() && i < 10; i++)
    {
        container[i] = *(list.begin() + i);
    }
}

int Array<int, 10>::size() const{
    return 10;
}
/* ... */
```

#### [4.4 部分显式具体化](#)
顾名思义，如果模板类用到了两个以上的类型参数

```cpp
template<class T1, class T2> 
class keyValue{
    /* ... */
}

//部分显式具体化
template <class T1> class keyValue<T1, int>{
    /* 你懂的！ */
}
```


### [5. 将模板作为类型参数](#) 
模板可以使用类型参数(typename T)和非类型参数(int n)。模板还可以包括本身就是模板的参数，这种参数是模板新增的特性，用于实现STL;

tk:是参数，要求tk必须是一个模板类！ tk 是一个模板类，不是一个真实具体的类哦！
```cpp
//创建一个双栈类
template< template <typename T> class tk, typename U>
class StackDouble
{
private:
    tk<U> stack_first;
    tk<U> stack_second;
public:
    StackDouble()= default;
    tk<U>& getFirstStack() { return this->stack_first;};
    tk<U>& getSecondStack() { return this->stack_second;};
};
```
使用模板类型参数
```cpp
StackDouble<std::stack, int> dl;

dl.getFirstStack().push(25);
dl.getFirstStack().push(28);

dl.getSecondStack().push(21);

StackDouble<Stack, double> stkdl; //或者用我们自己的模板类 Stack
```

理解以下这种情况, 非常有意思！ 将未实例化的模板类作为模板参数！

**作为函数参数， 你要记住，将模板做类型参数和把模板做函数参数是不一样的！别混了**
```cpp
template<typename T>
void giveStack(std::initializer_list<T> list){
    for (auto &&i : list)
    {
        std::cout<< i << std::endl;   
    }
}
```

### [6. 模板类与友元](#)
模板类也可以有友元，模板的友元有三类

* 非模板友元！
* 约束模板友元，即友元的类型取决于类被实例化的类型！
* 非约束模板友元，即友元的所有具体化都是类的每一个具体化的友元！

#### [6.1 非模板友元](#)
**在模板类中将一个常规的函数声明为友元！**

counts() 函数将称为所有模板实例化的友元。例如它将会是类 Array<int, 10> 和 Array<double, 10> 的友元！ counts函数不能通过
对象调用，那么怎么友好的访问 Array的实例类中的成员呢？ 这就看你自己的设计了，比如定义全局对象，全局指针！让当前函数可以访问！ 还可以访问模板类对象的静态成员!

```cpp
template<typename T, unsigned int N>
class Array final
{
private:
    T *container;
public:
    friend void counts(); //非模板友元
    friend void report(Array<T,N> & tmp);//带模板类参数
    Array(std::initializer_list<T> list);
    Array(const Array<T,N>& other);
    Array(const Array<T,N>&& other);
    Array<T,N>& operator=(Array<T,N>& other);
    int size() const;
    T &operator[](int idx);
    ~Array();
};
```
friend void report(Array<T,N> & tmp) report 函数和上面就不一样了！要记住 report不能声明为： friend void report(Array & tmp)！ 得有类型参数！
实例化后 friend void report(Array<int,10> & tmp) 就是 Array<int,10>的友元函数！ friend void report(Array<double,10> & tmp) 是 Array<double,10>的友元！

report()本身并不是模板函数，而只是使用一个模板作参数。这意味着必须为要使用的友元定义显式具体化: 也就说你得自己写
* void report(Array<int, 10> &tmp); 显式具体化
* void report(Array<double, 10> &tmp); 显式具体化

```cpp
void report(Array<int, 10> &temp){
    /* 函数体 */
}

void report(Array<double, 10> &temp){
    /* 函数体 */
}
```
#### [6.2 约束模板友元函数](#)
使友元函数本身成为模板,来使得类的每一个具体化多度获得一个与友元匹配的具体化！

```cpp
template<typeName T, unsigned int N>
void counts(){
    /* ... */
}

template<typeName T>
void report(T &tmp){
    /* ... */
}
//这里的T 可以使 Array<T,N> 的任意实例化 例如 Array<double, 10>
// T还可以是 int  double  其他class 

template<typename T, unsigned int N>
class Array final
{
public:
    friend void counts<T,N>(); //约束模板友元函数
    friend void report<>(Array<T,N> & tmp);//约束模板友元函数
    /* ... */
};
```
声明中<>指出这是模板具体化，注意模板具体化和函数具体化有点不一样。对于report，<>可以为空,因为可以从函数参数推断出模板类型参数。在声明的例子中是：HasfriendT<TT>。然而，也可以使用report<HasFriendT<TT>> 来代替 report<> 。
由于counts()函数没有参数，所以必须使用模板参数 counts<T,N>() 来制定生成具体的友元函数！

实例化一个 Array<double, 10> 生成下面的类定义！

```cpp
class Array<double, 10> final
{
public:
    friend void counts<double,10>(); //约束模板友元函数
    friend void report<>(Array<double,10> & tmp);//约束模板友元函数
    /* ... */
};
```
由于模板类具体化了，再调动友元模板函数也具体化！

#### [6.3 非约束模板友元函数](#)
上面的是将友元函数声明在类外面，这个的意思就是将模板函数声明在类里面！

```cpp
template<typename T, unsigned int N>
class Array final
{
public:
    template<typename T1, unsigned int N>
    friend void counts(); //约束模板友元函数
    template<typename C>
    friend void report(C &);//约束模板友元函数
    /* ... */
};
```

### [7. C++11 可变模板参数](#)
**可变参数模板（variadic template）** 为了解决强类型语言的严格性与灵活性的冲突而产生，让我们能够创建这样的模板函数和模板类：即可接受可变数量的参数。
它对参数进行了高度的泛化，可表示0到任意个数、任意类型的参数。 提供了一个用 **省略号(...)** 表示的元运算符（meta-operator）

* 在C++11之前，类模板和函数模板中只能包含固定数量的模板参数，可变模板参数无疑是一个巨大的改进，但由于可变参数模板比较抽象，因此使用起来需要一定的技巧。
* 在C++11之前其实也有可变参数的概念，比如printf函数就能够接收任意多个参数，但这是函数参数的可变参数，并不是模板的可变参数。


省略号的作用
* 位于参数左边： **打包** T... args
* 位于参数右边：**解包** args...

要创建可变参数模板，需要理解几个要点：

* 模板参数包（parameter pack）；
* 函数参数包；
* 展开（unpack）参数包；
* 递归。

**可变参数模板的定义方式**
函数的可变参数模板定义方式如下：

```
template<typename …Args>
返回类型 函数名(Args… args)
{
  //函数体
}
```

```cpp
//按照值传递
template<typename... Args>    // Args is a template parameter pack
void ShowList(Args... args) // args is a function parameter pack
{
   std::cout << sizeof...(args) << std::endl; //获取参数包中参数的个数
}

//按照引用传递
template<typename... Args>    // Args is a template parameter pack
void ShowList(const Args&... args) // args is a function parameter pack
{
   std::cout << sizeof...(args) << std::endl; //获取参数包中参数的个数
}

```

* 模板参数Args前面有省略号，代表它是一个可变模板参数，我们把带省略号的参数称为参数包，参数包里面可以包含0到 **N ( N ≥ 0 )** 个模板参数，而args则是一个函数形参参数包。
* 模板参数包Args和函数形参参数包args的名字可以任意指定，并不是说必须叫做 Args 和 args。

**现在调用ShowList函数时就可以传入任意多个参数了，并且这些参数可以是不同类型的。比如：**

```cpp
int main(int argc, char *argv[]){

    ShowList(); //0
	ShowList(1); //1
	ShowList(1, 'A'); //2
	ShowList(1, 'A', std::string("hello")); //3
    return 0;
}
```

#### [7.1 展开参数包](#)
但是我们无法直接获取参数包中的每个参数，只能通过展开参数包的方式来获取，这是使用可变参数模板的一个主要特点，也是最大的难点。

**错误示例：** 
```cpp
template<class ...Args>
void ShowList(Args... args)
{
	//错误示例：
	for (int i = 0; i < sizeof...(args); i++)
	{
		cout << args[i] << " "; //打印参数包中的每个参数
	}
	cout << endl;
}
```

**可变模板参数的展开**
* 通过递归函数来展开参数包
* 是通过逗号表达式来展开参数包。

#### [7.2 递归展开参数包](#)
递归展开参数包的方式如下：
* 给函数模板增加一个模板参数，这样就可以从接收到的参数包中分离出一个参数出来。
* 在函数模板中递归调用该函数模板，调用时传入剩下的参数包。
* 如此递归下去，每次分离出参数包中的一个参数，直到参数包中的所有参数都被取出来。


比如我们要打印调用函数时传入的各个参数，那么函数模板可以这样编写：
```cpp
template<typename T,typename... Args>    // Args is a template parameter pack
void ShowList(T value, Args... args) // args is a function parameter pack
{
   std::cout << value << " ";
   //std::cout << sizeof...(args) << std::endl; //获取参数包中参数的个数
   printf("\n");
   ShowList(args...); //递归调用
}
```

**这时我们面临的问题就是，如何终止函数的递归调用?** **编写无参的递归终止函数**

```cpp
void ShowList(){ printf("\n");  }

template<typename T,typename... Args>    // Args is a template parameter pack
void ShowList(T value, Args... args) // args is a function parameter pack
{
   std::cout << value << " ";
   //std::cout << sizeof...(args) << std::endl; //获取参数包中参数的个数
   ShowList(args...);
}

int main(int argc, char *argv[]){
	ShowList("cnm",78, 'A', std::string("hello")); //3
    return 0;
}
```
这样一来，当递归调用 ShowList 函数模板时，如果传入的参数包中参数的个数为0，那么就会匹配到这个无参的递归终止函数，这样就结束了递归。

#### [7.3 逗号表达式来展开参数包](#)
这种方式是通过列表获取参数包中的参数。如果参数包中各个参数的类型都是整型，那么也可以把这个参数包放到列表当中初始化这个整型数组，此时参数包中参数就放到数组中了。比如：

数组可以通过列表进行初始化，比如：
```cpp
int a[] = {1,2,3,4}
```
除此之外，如果参数包中各个参数的类型都是整型，那么也可以把这个参数包放到列表当中初始化这个整型数组，此时参数包中参数就放到数组中了。比如：
```cpp
//展开函数
template<class ...Args>
void ShowList(Args... args)
{
	int arr[] = { args... }; //列表初始化
	//打印参数包中的各个参数
	for (auto e : arr)
	{
		cout << e << " ";
	}
	cout << endl;
```

这时调用ShowList函数时就可以传入多个整型参数了。比如：

```cpp
int main()
{
	ShowList(1);
	ShowList(1, 2);
	ShowList(1, 2, 3);
	return 0;
}
```

但C++并不像Python这样的语言，**C++规定一个容器中存储的数据类型必须是相同**的，因此如果这样写的话，那么调用ShowList函数时传入的参数只能是整型的，
并且还不能传入0个参数，因为数组的大小不能为0，因此我们还需要在此基础上借助逗号表达式来展开参数包。

虽然我们不能用不同类型的参数去初始化一个整型数组，但我们可以借助逗号表达式。
* 逗号表达式会从左到右依次计算各个表达式，并且将最后一个表达式的值作为返回值进行返回。
* 将逗号表达式的最后一个表达式设置为一个整型值，确保逗号表达式返回的是一个整型值。
* 将处理参数包中参数的动作封装成一个函数，将该函数的调用作为逗号表达式的第一个表达式。

```cpp
//处理参数包中的每个参数
template<class T>
void PrintArg(const T& t)
{
	cout << t << " ";
}
//展开函数
template<class ...Args>
void ShowList(Args... args)
{
	int arr[] = { (PrintArg(args), 0)... }; //列表初始化+逗号表达式
	cout << endl;
}
```

这样一来，在执行逗号表达式时就会先调用处理函数处理对应的参数，然后再将逗号表达式中的最后一个整型值作为返回值来初始化整型数组。比如：

```cpp
//处理参数包中的每个参数
template<class T>
void PrintArg(const T& t)
{
	cout << t << " ";
}
//展开函数
template<class ...Args>
void ShowList(Args... args)
{
	int arr[] = { (PrintArg(args), 0)... }; //列表初始化+逗号表达式
	cout << endl;
}
```

* 我们这里要做的就是打印参数包中的各个参数，因此处理函数当中要做的就是将传入的参数进行打印即可。
* 可变参数的省略号需要加在逗号表达式外面，表示需要将逗号表达式展开，如果将省略号加在args的后面，那么参数包将会被展开后全部传入PrintArg函数，代码中的 {(PrintArg(args), 0)...} 将会展开成 {(PrintArg(arg1), 0), (PrintArg(arg2), 0), (PrintArg(arg3), 0), etc...}。

这时调用ShowList函数时就可以传入多个不同类型的参数了，但调用时仍然不能传入0个参数，因为数组的大小不能为0，如果想要支持传入0个参数，也可以写一个无参的ShowList函数。比如：
```cpp
//支持无参调用
void ShowList()
{
	cout << endl;
}
//处理函数
template<class T>
void PrintArg(const T& t)
{
	cout << t << " ";
}
//展开函数
template<class ...Args>
void ShowList(Args... args)
{
	int arr[] = { (PrintArg(args), 0)... }; //列表初始化+逗号表达式
	cout << endl;
}
```
实际上我们也可以不用逗号表达式，因为这里的问题就是初始化整型数组时必须用整数，那我们可以将处理函数的返回值设置为整型，然后用这个返回值去初始化整型数组也是可以的。比如：
```cpp
//支持无参调用
void ShowList()
{
	cout << endl;
}
//处理函数
template<class T>
int PrintArg(const T& t)
{
	cout << t << " ";
	return 0;
}
//展开函数
template<class ...Args>
void ShowList(Args... args)
{
	int arr[] = { PrintArg(args)... }; //列表初始化
	cout << endl;
}
```

#### [7.4 emplace相关接口函数](#)
C++11标准给STL中的容器增加emplace版本的插入接口,增加了对应的emplace_front、emplace_back和emplace函数。

emplace系列接口的可变模板参数类型都带有 **“&&”**，这个表示的是 **万能引用**，而不是右值引用。 emplace系列接口**最大的特点就是支持传入参数包，用这些参数包直接构造出对象，这样就能减少一次拷贝**，这就是为什么有人说emplace系列接口更高效的原因。
```cpp
template< class... Args >
iterator emplace( const_iterator pos, Args&&... args ) //原位构造元素

template< class... Args >
void emplace_back( Args&&... args ); //在容器末尾就地构造元素

template< class... Args >
void emplace_front( Args&&... args ); //在容器头部原位构造元素
```
**这个接口，可以传递左值，右值，列表初始化**
```cpp
int main()
{
	list<pair<int, string>> mylist;
	pair<int, string> kv(10, "111");
	mylist.push_back(kv);                              //传左值
	mylist.push_back(pair<int, string>(20, "222"));    //传右值
	mylist.push_back({ 30, "333" });                   //列表初始化

	mylist.emplace_back(kv);                           //传左值
	mylist.emplace_back(pair<int, string>(40, "444")); //传右值
	mylist.emplace_back(50, "555");                    //传参数包
	return 0;
}
```

* 传入左值对象，需要调用构造函数+拷贝构造函数。
* 传入右值对象，需要调用构造函数+移动构造函数。
* 传入参数包，只需要调用构造函数。

#### [7.5 通过参数调用构造函数](#)
我们自己也可以实现一个emplace

```cpp
#ifndef __Student_H__JX
#define __Student_H__JX __Student_H__JX
#include<string>

class Student
{
private:
    /* data */
    std::string student_id;
	std::string name;
	unsigned short age;
public:
    Student(/* args */); //无参构造函数
    Student(std::string id, std::string name, unsigned short age); //有参构造函数
    Student(const Student & student); //拷贝构造函数
    Student(const Student && student); //移动构造函数
    Student& operator = (const Student &student); //赋值运算符
    ~Student();//默认析构函数
    std::string ToString() const;//转换为字符串方法
    bool setAge(unsigned short new_age);
    const unsigned short& getAge() const;
};
#endif
```

```cpp
template<typename... Args>    
void ShowList(Args&&... args) 
{ 
   Student stu(args...);
   std::cout << stu.ToString() << std::endl;
}

int main(int argc, char *argv[]){
	ShowList("2016110418", "remix", 24); //3
    return 0;
}
```

### [8. C++11 std::forward](#)
**std::forward通常是用于完美转发的**，**它会将输入的参数原封不动地传递到下一个函数中**，这个“原封不动”指的是，如果输入的参数是左值，那么传递给下一个函数的参数的也是左值；如果输入的参数是右值，那么传递给下一个函数的参数的也是右值。一个经典的完美转发的场景是：

```cpp
template <class... Args>
void forward(Args&&... args) {
    f(std::forward<Args>(args)...);
}
```
1. 输入参数的类型是 Args&&... ， && 的作用是引用折叠，其规则是：

```
&& && -> &&
& && -> &
& & -> &
&& & -> &
```
由于我们需要保留输入参数的右值属性，因此 Args 后面需要跟上 &&；

2. std::forward的模板参数必须是<Args>，而不能是<Args...>，这是由于我们不能对 Args 进行解包之后传递给 std::forward，而解包的过程必须在调用 std::forward 之后.

```cpp
int add(int a, int b) {
	return a + b;
}

template<typename Func =int(int, int), typename ...Args  >
int useAdd(Func F, Args && ...args) {
	return F(std::forward<Args>(args)...);
}

std::cout <<"sum: " << useAdd(add, 50, 95) << "\n";
//sum: 145
```

-----
时间: [2022/9/17 12:45 第二次修订完成！] 