## [智能指针和STL](#)

**介绍**：智能指针是行为类似于指针的类对象，本质上是利用栈去管理堆！



**目录**：

- [x] [1. 动态内存概述](#1-动态内存概述)
- [x] [2. 智能指针概述](#2-智能指针概述)
- [x] [3. 几种智能指针](#3-几种智能指针)
- [x] [4. unique_ptr](#4-unique_ptr)
- [x] [5. share_ptr](#5-share_ptr)
- [x] [6. 智能指针妙用](#6-智能指针妙用)
- [x] [7. weak_ptr](#7-weak_ptr)
- [x] [8. allocator](#8-allocator)



-----

### [1. 动态内存概述](#)

使用`C++`编程的程序所使用的对象都有**严格定义的生存期**。`C++`还支持动态分配对象，但是动态分配的对象的生存期与他们在哪里创建无关，只有当程序员手动释放的时候，这些对象才会被销毁，内存才会被回收。

* **全局对象**，在程序启动的时候分配，在程序结束时销毁，
* **局部自动对象**，当我们进入其定义所在的程序块时创建(压入栈中)，在离开块(出栈)时销毁。
* **局部static对象**，在第一次使用前分配，在程序结束时销毁。
* **动态分配对象**，在申请完成后创建，在程序员**手动**显示释放的时候销毁。

动态分配例子： 正确是释放动态内存在编程中及其容易出错的地方。

```cpp
Student* remix = new Student("remix", 28, true); //new

printf("address: %p\n", remix);
std::cout << "name: " << remix->get_name() << "\n";
//new type[]
Student* students = new Student[10]{
    {"nike", 27, true},
    {"mike", 26, true},
    {"kicker", 25, true},
    {"unix", 29, true},
    {"quake", 24, false},
    {"oiner", 22, false},
    {"wnier", 23, true},
    {"juner", 20, true},
    {"you", 30, false},
    {"pike", 31, true},
};
std::for_each(students, students + 10, [](Student p) {
    printf("address: %p, ", &p);//每次都一样
    std::cout << "name: " << p.get_name() << "\n";
});

delete remix; //不能忘记
delete [] students; //不能忘记
```

经典的通过 **new** 动态分配的内存，需要手动通过**delete**释放内存，否则会造成内存泄漏。而要做到防止内存泄漏，需要时刻注意**别忘了**这一步骤，事实上，有时候经常没有做到。

```cpp
int i = 56;
auto p = new auto(i); 
delete p;
```

而且有时候傻傻分不清楚指针是指向栈区的不需要的**delete**的，还是**new**申请的堆区的指针！

[**但如果当指针占据的内存被释放，同时指针指向的内存也被释放，那该有多好啊**](#)，这就是智能指针的思想。

#### [1.1 动态分配的const对象](#)

用**new分配const对象是合法**的！毫无疑问类似于任何**const对象**，一个动态分配的const对象必须进行初始化。

```cpp
const int* ci = new const int(1024);
const std::string* cstr = new const std::string("caller");

printf("ci: %d  cstr: %s\n", *ci, cstr->c_str());
delete ci,cstr;
```

#### [1.2 正确释放动态内存](#)

当我们**delete**一个指针之后，指针值就变为无效了。虽然指针已经无效，但在很多机器上指针仍然保存着动态内存的地址。但在**delete**之后，指针就变成了人们所说的**空悬指针(dangling pointer)**，指向一块曾经保存数据对象但现在已经无效的内存的指针。

**准则：释放指针以后将nullptr赋予指针。**

```cpp
auto p = new int(56);
delete p;//p 是 空悬指针(dangling pointer)
p = nullptr; //
```

#### [1.3 内存耗尽问题](#)

如果计算机的内存不足以满足申请需要，即一个程序用光了它所有的可用的内存，**new**表达式就会失败。[**如果new不能分配所要求的内存空间，它就会抛出一个 bad_alloc的异常**！](#)

```cpp
try
{
    Student *std = new Student[10000000]();
    delete[] std;
    std = nullptr;
}
catch (const std::bad_alloc& error) {
    std::cout << error.what() << std::endl;
}
catch (const std::exception& error)
{
    std::cout << error.what() << std::endl;
}

```

**我们也可以改变使用new的方式来阻止它抛出异常！**

```cpp
int *ptr = new (nothrow) int(500); 
```

#### [1.4 new和delete的本质](#) 
`operator new` 等操作它们本质上是对c语言的 `malloc`、`calloc`、`free`的上层封装！ 底层还是最基本的c语言内存申请和释放！ 最底层没有任何特殊的设计！

```cpp
class user {
private:
    std::string _name;
    uint16_t _age;
public:
    user(): user("",0){
        std::cout << "Call default constructor" << std::endl;
    };
    explicit user(const std::string& name, const uint16_t& age):_name(name), _age(age){
        std::cout << "Call constructor" << std::endl;
    };
    std::string getName() const{ return _name; };
    uint16_t getAge() const{ return _age; };
    ~user() {
        std::cout << "Call destory" << std::endl;
    };
};

int main() {
    user* up = (user *)calloc(1, sizeof(user));
    *up = user("remix", 28);


    free(up);
    return 0;
}
/*
Call constructor
Call destory
*/
```

#### [1.5 直接使用c语言分配再使用](#)
看代码吧！

```cpp
class user{
private:
    string _name;
    int _age;
public:
    explicit user(string name = "", int age = 0)
    :_name(std::move(name)), _age(age){
        std::cout << "Call Constructor" << std::endl;
    }
    user(const user& other)= default;
    user& operator=(const user& other){
        if (this != &other){
            this->_name = other._name;
            this->_age = other._age;
        }
        return *this;
    }
    void print() const{
        std::cout << "name: " << _name << ", age: " << _age << std::endl;
    }
    ~user(){
        std::cout << "disappear over" << std::endl;
    }
};
int main() {
    user *names = static_cast<user*>(calloc(5, sizeof(user)));
    if(names != nullptr){
        names[0] = user("remix", 18);
        names[0].print();

        free(names);
    }
    return 0;
}

/**
Call Constructor
over
name: remix, age: 18
**/
```

### [2. 智能指针概述](#)

智能指针是行为类似于指针的类对象，但这种对象还有其他功能。 如果指针本身有一个**析构函数**，**该析构函数将在指针过期时释放它指向的内存**。而指针的问题在于，它只是一个常规指针，不是有析构函数的类对象。如果它是对象，则可以在对象过期时，让它的析构函数删除指向的内存。

> 这正是auto_ptr、unique_ptr、shared_ptr背后的思想。

> **注意**: 模板 `auto_ptr` 是 `C++98`提供的解决方法，`C++11` 已将其摒弃，并提供了另外两种解决方法。然而，虽然 `auto_ptr` 被摒弃，但它已使用了多年；同时，如果编译器不支持其他两种解决方法，`auto_ptr` 将是唯一的选择。**不推荐使用！**

```cpp
int* ptr = new int(58);
delete ptr;//忘了就内存泄露
```

**智能指针使用例子：**

```cpp
std::unique_ptr<Student> make_student_unique_ptr(const std::string &name,const int& val, const bool & sex){
    std::srand(val);
    std::unique_ptr<Student> ptr(new Student(name, val, sex));
    return ptr;
}

int main(int argc, char* argv[]) {    
    std::unique_ptr<Student> ptr = make_student_unique_ptr("kicker", 17, true);
    std::cout << ptr->toString() << "\n";
    return 0;
}
```

#### [2.1 使用智能指针](#)
三个**智能指针模板**都定义了类似指针的对象，可以将new获得（直接或间接）的地址赋给这种对象。当智能指针过期时，其析构函数将使用delete来释放内存。

* 要创建智能指针对象，必须包括头文件 `#include<memory>`，该文件模板定义。智能指针模板位于名称空间std中。
* 智能指针对象很多方面都类似于常规指针。例如可以执行解除引用操作 `（*ps）`、访问结构成员`（ps->puffIndex）`、将它赋给指向相同类型的常规指针，还可以将智能指针对象赋给另一个同类型的智能指针对象。

使用区别如下所示：
```cpp
double demo01(double val){
   double *d_ptr = new double(10);
   double result = val * (*d_ptr);
   delete d_ptr;
   return result;
}
```
假设 d_ptr 存在 内存地址 3000 处， 里面存储的值是 10000 `(0x00002710)` ！ 表示申请到的用于存储double 值为10.0的内存空间！

```
         内存地址    值
d_ptr --> 3000    [10000]
          ...
         10000    [ 10  ]
```

```cpp
double demo02(double val){
   std::auto_ptr<double> dp(new double(10.0));
   double result = val * (*dp);
   return result;
}
```
首先为 double值 和 `dp`分配内存空间

```
dp对象存储在    6000  [10086]

double的地址在  10086 [10.0]    
```
**由于编译器的废弃警告，推荐你使用 unique_ptr**

```cpp
int main(int argc, char const* argv[])
{   
   std::string name("kicker");
   std::string env("land");
   std::unique_ptr<animal> pAnimal(new animal(name, 3, env));
   pAnimal->viewAnimal();
   return 0;
}
/*
animal appear !!!
animal - name: kicker, age: 3, enviroment: land 
animal diappear !!!
*/
```

#### [2.2 只接受堆区地址](#)
不应该指向将栈区地址的指针赋值给智能指针，全部三种智能指针都应当避免以下操作。

```cpp
std::string vacation("I wandered lonely as a cloud.");
std::shared_ptr<std::string> pvac(&vacation); // Don't do it.

int age = 24;
std::unique_ptr<int> i_u_ptr(&age); //被禁止的
```
这种情况下，程序将把delete运算符用于非堆内存，这是错误的。


#### [2.3 所有权问题](#)
其中，如果`ps`和`vocation`是两个常规的指针,将指向同一个`string`对象。但是如果是智能指针，这是不能接受的，因为程序将试图删除同一个对象两次：一次是`ps`过期，另一次是`vocation`过期时。这回导致内存某个地方被释放两次，将会造成严重问题，要避免这种问题，方法有多种。

```cpp
using namespace std;
auto_ptr<string> ps (new string("I reigned lonely as a cloud."));
auto_ptr<string> vocation;
vocation = ps;
```
**解决方法:**

* 重载赋值运算符，使之执行深复制。这样两个指针将指向不同的对象，其中的一个对象是另一个对象的副本。
* 建立所有权（`ownership`）概念，对于特定的对象，只能有一个智能指针可拥有它，这样只有拥有对象的智能指针的析构函数会删除该对象。然后，让赋值操作转让所有权。这就是用于`auto_ptr`和`unique_ptr`的策略，但`unique_ptr`的策略更严格。
* 创建智能更高的指针，跟踪引用特定对象的智能指针数。这称为引用计数`（reference counting）`。例如，赋值时，该计数将加1，而指针过期时，计数将减1。仅当最后一个指针过期时，才调用`delete`。这是`shared_ptr`采用的策略。


```cpp
int main(int argc, char const* argv[])
{   
   std::string name("kicker");
   std::string env("land");
   std::unique_ptr<animal> pAnimal(new animal(name, 3, env));
   std::unique_ptr<animal> pOthrtAnimal;
   pOthrtAnimal = pAnimal; 
   //这个操作对于  unique_ptr 不被允许，写的时候就不被允许
   //如果是 auto_ptr 那么会在编译阶段报错
   pOthrtAnimal->viewAnimal();
   return 0;
}
```

**C++11 规定转移所有权需要用到 std::move(智能指针对象) 函数**

```cpp
int main(int argc, char const* argv[])
{   
   std::string name("kicker");
   std::string env("land");
   std::unique_ptr<animal> pAnimal(new animal(name, 3, env));
   std::unique_ptr<animal> tAnimal = std::move(pAnimal);
   if (pAnimal == nullptr)
   {
      printf("what happen?\n");
   }
   tAnimal->viewAnimal();
   return 0;
}
```

**指针智能作为函数参数**

```cpp
std::unique_ptr<Student> printStudent(std::unique_ptr<Student> ptr) {
    std::cout << ptr->toString() << "\n";
    return ptr;
}

int main(int argc, char* argv[]) {    
    std::unique_ptr<Student> ptr = make_student_unique_ptr("kicker", 17, true);
    printStudent(std::move(ptr));
    return 0;
}
```

#### [2.4 不适用于auto_ptr的地方](#)

不适合使用的 `auto_ptr` 的示例。

```cpp
#include<iostream>
#include<string>
#include<memory>

int main()
{
    using namespace std;
    auto_ptr<string> films[5] =
    {
        auto_ptr<string>(new string("Fowl Balls")),
        auto_ptr<string>(new string("Duck Walks")),
        auto_ptr<string>(new string("Chicken Runs")),
        auto_ptr<string>(new string("Turkey Errors")),
        auto_ptr<string>(new string("Goose Eggs"))
    };
    auto_ptr<string> pwin;
    pwin = films[2]; // films[2] loses ownership

    cout << "The nominees for best avian baseball film are\\n";
    for (int i = 0; i < 5; i++)
        cout << *films[i] << endl;
    cout << "The winner is " << *pwin << "!\\n";
    cin.get();
    return 0;
}
/*
Output:
The nominees for best avian baseball film are
Fowl Balls
Duck Walks
Segmentation fault (core dumped) 这里是出错
*/
```
如下显示的错误可能随系统和实现而异。这里的问题在于，下面的语句将所有权从`film[2]`转让给`pwin`：

```cpp
pwin = films[2]; // films[2] loses ownership
```
这导致`film[2]`不再引用该字符串。在`auto_ptr`放弃对象的所有权后，便可能使用它来访问该对象。当程序试图打印`films[2]`指向的字符串，却发现这是一个空指针，这是个令人讨厌的错误。

但如果使用`shared_ptr`代替`auto_ptr`，程序将正常运行。

此时`pwin`和`films[2]`指向同一个对象，而引用计数从1增加到2。程序末尾，后声明的pwin首先调用其析构函数，该析构函数将引用计数降低到1。然后，在films[2]调用析构函数时，将引用计数降低到0，并释放指向的分配空间。

如果改成`unique_ptr`，程序不会等到运行阶段崩溃，而在编译器因下述代码行出现错误：
```cpp
pwin = films[2]; // 编译器会指出错误
```


#### [2.5 shared_ptr 引用计数](#)
它遵循共享所有权的概念，即不同的 `shared_ptr` 对象可以与相同的指针相关联，并在内部使用引用计数机制来实现这一点。

```cpp
std::string name("kicker");
std::string env("land");
std::shared_ptr<animal> pAnimal(new animal(name, 3, env));
std::shared_ptr<animal> pOthrtAnimal;
pOthrtAnimal = pAnimal;

std::shared_ptr<animal> tAnimal;
tAnimal = pOthrtAnimal;

tAnimal->viewAnimal();
```

#### [2.6 unique_ptr为何优于auto_ptr](#)
因为`unique_ptr`会在编译阶段指出错误，而`auto_ptr`不能，会存在潜在的程序崩溃。

```cpp
// auto_ptr 会在运行时存在潜在崩溃
autp_ptr<string> p1(new string("auto"));
autp_ptr<string> p2;
p2 = p1; // 运行时报错
// unique_ptr 会在编译阶段报错
unique_ptr<string> p3(new string("auto"));
unique_ptr<string> p4;
p4 = p3; // 编译器报错
```

相比于`auto_ptr`，`unique_ptr`还有另一个优点。它有一个可用于数组的变体。因为必须将`delete`和`new`配对，将`delete[]`和`new[]`配对。模板`auto_ptr`使用`delete`而不是`delete[]`，因此只能与`new`一起使用。但`unique_ptr`有使用`new[]`和`delete[]`的版本：

```cpp
std::unique_ptr<double[]>pda(new double[5]); // will use delete []
```

**unique_ptr 用于返回值：**

```cpp
#include<memory>

std::unique_ptr<animal> getAnimal(){
   std::string name("kicker");
   std::string env("land");
   std::unique_ptr<animal>  temp(new animal(name, 3, env)) ; 
   return temp;
}

int main(int argc, char const* argv[])
{   
   std::unique_ptr<animal> ps;
   ps = getAnimal();
   return 0;
}
/*
animal appear !!!
animal diappear !!!
*/
```

### [3. 几种智能指针](#)
**C++11** 引入了三种智能： unique_ptr、shared_ptr、weak_ptr。 加上 auto_ptr 一共有四种！

* auto_ptr： 会有所有权机制不完善导致的空指针问题！
* unique_ptr： 是唯一的，不支持赋值运算符！物理上隔绝了所有权问题！一旦创建，指针的拥有者只有当前智能指针！
* shared_ptr ：采用引用计数的方式，可以相互赋值！

**如果我们不初始化一个智能指针，它就会被初始化为一个空指针！**

### [4. unique_ptr](#)
**std::unique_ptr** 是通过指针占有并管理另一对象，并在 **unique_ptr** 离开作用域时释放该对象的智能指针，**支持移动操作。**支持数组或者单个对象操作。

如果程序不需要多个指向同一个对象的指针，则可使 用**unique_ptr**。如果函数使用new分配内存，并返回指向该内存的指针，将其返回类型声明为**unique_ptr**是不错的选择。这样，所有权将转让给接受返回值的unique_ptr，而该智能指针将负责调用delete。

1. 默认情况存储成本和裸指针相同，无添加` `unique_ptr被设计成为一个零额外开销的智能指针，使用它，应该相比你手工写new和delete没有额外开销，不管是时间还是空间上。
2. 独占拥有权，不支持拷贝构造，只支持移动（所有权转移）
3. 可以转换成shared_ptr

**std::unique_ptr 有两个版本：**

管理单个对象（例如以 new 分配）
```cpp
template<
    class T,
    class Deleter = std::default_delete<T>
> class unique_ptr;
```
管理动态分配的对象数组（例如以 new[] 分配）
```cpp
template <
    class T,
    class Deleter
> class unique_ptr<T[], Deleter>;
```
使用例子:
```cpp
std::unique_ptr<Student> ptr(new Student("kicker", 17, true));
std::unique_ptr<Student[]> sptr(new Student[3]{
    {"kicker", 17, true},
    {"remix", 18, false},
    {"quake", 19, true}
});
for (int i = 0; i < 3; i++)
    std::cout << sptr[i].toString() << "\n";
```
智能指针的使用方式与普通指针类似，解引用一个智能指针返回它指向的对象！[**智能指针可以转换为bool类型，如果是空智能指针就会转换为true，否则为false.**](#) 

在下列两者之一发生时用关联的删除器释放对象：

- 销毁了管理的 `unique_ptr` 对象
- 通过 [operator=](https://zh.cppreference.com/w/cpp/memory/unique_ptr/operator%3D) 或 [reset()](https://zh.cppreference.com/w/cpp/memory/unique_ptr/reset) 赋值另一指针给管理的 `unique_ptr` 对象。

**在unique_ptr为右值时，可将其赋给shared_ptr，这与将一个unique_ptr赋给另一个需要满足的条件相同。**
```cpp
int main(int argc, char* argv[]) {    
    std::shared_ptr<Student> ptr = std::make_unique<Student>("kicker", 17, true);
    std::cout << ptr->toString() << "\n";
    return 0;
}
```

#### [4.1 构造函数](#)
unique_ptr有许多的构造函数，了解一些常用的构造函数即可！
```cpp
/* 主模板 unique_ptr<T> 的成员 */
constexpr unique_ptr() noexcept;
constexpr unique_ptr( std::nullptr_t ) noexcept;
explicit unique_ptr( pointer p ) noexcept; 
unique_ptr( unique_ptr&& u ) noexcept; //移动构造函数

/* 数组特化 unique_ptr<T[]> 的成员 */
constexpr unique_ptr() noexcept;
constexpr unique_ptr( std::nullptr_t ) noexcept;
unique_ptr( unique_ptr&& u ) noexcept;
```

```cpp
std::unique_ptr<Foo> up1;  // up1 为空
std::unique_ptr<Foo> up1b(nullptr);  // up1b 为空
std::unique_ptr<Foo[]> up(new Foo[3]);
```

#### [4.2 修改器函数](#)
| 接口名称   | 函数                                            | 说明                                                         |
| ---------- | ----------------------------------------------- | ------------------------------------------------------------ |
| release    | pointer release() noexcept;                     | 返回一个指向被管理对象的指针，并释放所有权，无管理对象返回nullptr。 |
| reset      | void reset( pointer ptr = pointer() ) noexcept; | 替换被管理对象。                                             |
| operator*  | ...                                             | 类似于指针的解引用操作                                       |
| operator-> | pointer operator->() const noexcept;            | 类似于指针访问操作                                           |

**release()** - - - - 除法特殊用法，否则别用!

```cpp
std::unique_ptr<Student> ptr = make_student_unique_ptr("kicker", 17, true);

Student* stp = ptr.release();
std::cout << stp->toString() << "\n";
delete stp;
```

借助**release**和**reset**和完成指针移动操作！

```cpp
std::unique_ptr<Student> ptr1(new Student("remix", 59, true));
std::unique_ptr<Student> ptr2(new Student("coke", 29, false));

std::unique_ptr<Student> ptr3(ptr1.release()); //转移控制权
ptr3.reset(ptr2.release());//转移控制权

std::cout << ptr3->toString() << std::endl;
//Name: coke Age: 29 Sex: 0
```

**特化版本：`unique_ptr<T[]>`**

| 接口名称   | 函数                                  | 说明                                                 |
| ---------- | ------------------------------------- | ---------------------------------------------------- |
| operator[] | T& operator[]( std::size_t i ) const; | 返回指向被管理对象的指针，无被管理对象，则为 nullptr |
| reset      | void reset( U ) noexcept;             | 替换被管理对象。                                     |

#### [4.3 观察器](#)

| 接口名称    | 函数                             | 说明                                   |
| ----------- | -------------------------------- | -------------------------------------- |
| get         | pointer get() const noexcept;    | 返回指向被管理对象的指针               |
| get_deleter | Deleter& get_deleter() noexcept; | 返回会用于析构被管理对象的删除器对象。 |

#### [4.4 非成员函数](#)

| 接口名称                     | 函数                                                         | 说明                                                         |
| ---------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| std::hash`<std::unique_ptr>` | template<class T, class Deleter> struct hash<unique_ptr<T, Deleter>>; | **std::unique_ptr** 的散列支持                               |
| **make_unique<T>**           | template< class T, class... Args ><br/>unique_ptr<T> make_unique( Args&&... args ); | 构造非数组类型 `T` 对象。传递参数 `args` 给 `T` 的构造函数。 |
| make_unique_for_overwrite<T> |                                                              | C++20 提供                                                   |
| swap(p1,p2)                  |                                                              | 交换两个指针                                                 |
| move                         |                                                              | 移动语义                                                     |

**使用例子：**

```cpp
std::unique_ptr<Student> ptr = std::make_unique<Student>("kicker", 17, true);
std::cout << ptr->toString() << "\n";
```

#### [4.5 函数返回 unique_ptr](#)

但有时候，将一个智能指针赋给另一个并不会留下危险的悬挂指针，这没有问题。

假设有如下函数定义：

```cpp
// Function Definition
unique_ptr<string> demo(const char * s)
{
    unique_ptr<string> temp(new string(s));
    return temp;
}
// main
unique_ptr<string> ps;
ps = demo("Uniquely special");
```

demo()返回一个临时 `unique_ptr`，然后ps接管了原本归返回的 `unique_ptr` 所有的对象，而返回的unique_ptr被销毁。这没有问题，因为ps拥有了string对象的所有权。这里好有个好处是，`demo()` 返回的临时unique_ptr很快被销毁，没有机会使用它来访问无效的数据。换句话说，没有理由禁止这种赋值。神奇的是，编译器确实允许这种赋值！**unique_ptr 支持移动操作！**

总之，程序试图将一个unique_ptr赋给另一个时，如果源unique_ptr是个临时右值，编译器允许这样做；一旦源unique_ptr将存在一段时间（即悬挂着的unique_ptr），编译器将禁止这样做。

如果真的想实现unique_ptr对象之间的赋值，仅当以非智能的方式使用遗弃的智能指针（如解除引用时），这种赋值才不安全。要安全地重用这种指针，可给它赋新值。C++有一个标准库函数 `std::move()`，能够将一个`unique_ptr`赋给另一个。

```cpp
// 下面的代码是可行的，使用到上一段中定义的demo()
using namespace std;
unique_ptr<string> ps1, ps2;
ps1 = demo("Uniquely special");
ps2 = move(ps1); 
// enable assignment

ps1 = demo(" and more");
cout << *ps2 << *ps1 << endl;
```

> unique_ptr之所以能够区分安全和不安全的用法，是因为它使用了C++11新增的移动构造函数和右值引用。

### [5 share_ptr](#)

如果程序要使用多个指向同一个对象的指针，应选择**shared_ptr**。这样的情况包括：

1. 有一个指针数组，并使用一些辅助指针来标识特定的元素，如最大和最小的元素；
2. 两个对象包含都指向第三个对象的指针；
3. STL容器包含指针。

```cpp
template< class T > class shared_ptr;
```

很多STL算法都支持复制和赋值操作，这些操作可用于**shared_ptr**，但不能用于**unique_ptr**（编译器发出警告）和**auto_ptr**（行为不确定）。

> 如果编译器没有提供shared_ptr，可使用Boost库提供的shared_ptr。

[**在unique_ptr为右值时**](#) **，可将其赋给shared_ptr，这与将一个unique_ptr赋给另一个需要满足的条件相同。**

```cpp
std::unique_ptr<Student> make_student_unique_ptr(const std::string &name,const int& val, const bool & sex){
    std::srand(val);
    std::unique_ptr<Student> ptr(new Student(name, val, sex));
    return ptr;
}

std::shared_ptr<Student> ptr = make_student_unique_ptr("kicker", 17, true);
std::cout << ptr->toString() << "\n";

// make_int()是一个返回类型为unique_ptr<int>的函数
unique_ptr<int> pup(make_int(rand() % 1000)); // ok
shared_ptr<int> spp(pup); // not allowed, pup is an lvalue
shared_ptr<int> spr(make_int(rand() % 1000)); // ok
```

**当进行拷贝和赋值操作的时候，每个shared_ptr 都会记录有多少个其他shared_ptr指向相同的对象！** [**可以将shared_ptr理解为GC算法中的引用计数法！**](#)

#### [5.1 构造函数](#)

从指代要管理的对象的各种指针类型构造新的 `shared_ptr` ，有许多的重载！

```cpp
constexpr shared_ptr() noexcept;
constexpr shared_ptr( std::nullptr_t ) noexcept;

shared_ptr( const shared_ptr& r ) noexcept;
shared_ptr( shared_ptr&& r ) noexcept;
// auto_ptr 移动构造函数
template< class Y >
shared_ptr( std::auto_ptr<Y>&& r );
// unique_ptr 移动构造函数
template< class Y, class Deleter >
shared_ptr( std::unique_ptr<Y,Deleter>&& r );
//构造函数 + 删除器
template< class Y, class Deleter >
shared_ptr( Y* ptr, Deleter d );
// opt
template< class Y, class Deleter, class Alloc >
shared_ptr( Y* ptr, Deleter d, Alloc alloc );
operator= //对 shared_ptr 赋值
```

在满足 `unique_ptr` 要求的条件时，也可使用`auto_ptr`，但`unique_ptr`是更好的选择。如果编译器没有提供`unique_ptr`，可考虑使用`Boost`库提供的`scoped_ptr`，它与`unique_ptr`类似。

```cpp
std::shared_ptr<Student> ptr(new Student("remix", 59, true));
std::cout << ptr->toString() << std::endl;
//Name: remix Age: 59 Sex: 1
```

**删除器 - 构造函数**

```cpp
std::shared_ptr<Student> ptr(new Student("remix", 59, true), [](Student* ptr) {
    delete ptr;
    ptr = nullptr;
    std::cout << "回收内存" << std::endl;
});
std::cout << ptr->toString() << std::endl;
//Name: remix Age: 59 Sex: 1
```

#### [5.2 函数接口](#)

| 接口名称 | 函数                                                         | 说明                                                         |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| reset    | void reset() noexcept;                                       | 释放被管理对象的所有权，若存在。                             |
| reset    | template< class Y ><br/>void reset( Y* ptr );                | 以 `ptr` 所指向对象替换被管理对象。 `Y` 必须是完整类型且可隐式转换为 `T` 。 |
| reset    | template< class Y, class Deleter ><br/>void reset( Y* ptr, Deleter d ); | 以指定的删除器 `d` 为删除器。 `Deleter` 必须对 `T` 类型可调用，即 d(ptr)必须为良构，拥有良好定义行为且不抛任何异常。 |
| **swap** | void swap( shared_ptr& r ) noexcept;                         | 交换 *this 与 `r` 的存储指针值与所有权。不调整引用计数，若它们存在。 |

**reset()** : 替换所管理的对象。

```cpp
int main()
{
    std::shared_ptr<Foo> sptr = std::make_shared<Foo>(1);
    std::cout << "The first Foo's bar is " << sptr->getBar() << "\n";
 
    // 重置，交与新的 Foo 实例
    // （此调用后将销毁旧实例）
    sptr.reset(new Foo);
    std::cout << "The second Foo's bar is " << sptr->getBar() << "\n";
}
```

#### [5.3 观察器](#)

| 接口名称        | 函数                                                  | 说明                                                         |
| --------------- | ----------------------------------------------------- | ------------------------------------------------------------ |
| get             | T* get() const noexcept;                              | 返回存储的指针                                               |
| operator*       | ...                                                   | 类似于指针的解引用操作                                       |
| operator->      | pointer operator->() const noexcept;                  | 类似于指针访问操作                                           |
| operator[]      | element_type& operator[]( std::ptrdiff_t idx ) const; | 到数组第 `idx` 元素的引用，即 get()[idx]                     |
| use_count()     | long use_count() const noexcept;                      | 返回 `shared_ptr` 所指对象的引用计数                         |
| unique()        | bool unique() const noexcept                          | 检查 *this 是否管理当前对象的仅有 `shared_ptr` 实例，即是否 use_count() == 1 。 |
| operator bool() | explicit operator bool() const noexcept;              | 若 *this 存储非空指针则为 true ，否则为 false 。             |

**operator[]**

```cpp
const std::size_t arr_size = 10;
std::shared_ptr<int[]> pis(new int[10]{0,1,2,3,4,5,6,7,8,9});
for (std::size_t i = 0; i < arr_size; i++){
    std::cout << pis[i] << ' ';
}
```

**use_count()**

```cpp
std::shared_ptr<Student> ptr = make_student_unique_ptr("kicker", 17, true);
std::cout << ptr->toString() << "\n";
std::cout << ptr.use_count() << "\n"; //1

std::shared_ptr<Student> p2 = ptr;
std::cout << p2.use_count() << "\n"; //2
```

#### [5.4 转换为bool类型](#)

[**智能指针可以转换为bool类型，如果是空智能指针就会转换为true，否则为false.**](#) 

```cpp
std::shared_ptr<Student> p2 = ptr;
if (bool(p2))
{
    std::cout << p2.use_count() << "\n"; //2
}
```

#### [5.5 非成员函数](#)

| 接口名称        | 函数                                                         | 说明                                                         |
| --------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **make_shared** | template< class T, class... Args ><br/>shared_ptr<T> make_shared( Args&&... args ); | 以 args 为 T 的构造函数参数列表，构造 T 类型对象并将它包装于 std::shared_ptr。**(T 非数组)** |
| **make_shared** | template<class T><br/>shared_ptr<T> make_shared( [std::size_t](http://zh.cppreference.com/w/cpp/types/size_t) N ); | 同上，但构造的对象是可能多维的数组，(T 为 U[])               |
| **operator<<**  | template <class T, class U, class V><br/>    std::basic_ostream<U, V>& operator<<(std::basic_ostream<U, V>& os, const std::shared_ptr<T>& ptr); | 将存储的指针的值输出到输出流                                 |
| **swap**        | template< class T ><br/>void swap( std::shared_ptr<T>& lhs, std::shared_ptr<T>& rhs ) noexcept; | lhs, rhs 要交换内容的智能指针                                |
| **hash**        | template<class T> struct hash<shared_ptr<T>>;                | **std::shared_ptr** 的散列支持                               |

**make_shared** 非数组用法

```cpp
std::shared_ptr<Student> ptr = std::make_shared<Student>("kicker", 17, true);
std::cout << ptr->toString() << "\n";
```

**make_shared** 数组用法实例

```cpp
std::shared_ptr<Student[5]> sptr = std::make_shared<Student[5]>();

for (size_t i = 0; i < 5; i++)
{
    printf("%s \n", sptr[i].toString().c_str());
}
```

#### [5.6  缺陷说明](#)

然而直到**c++17**前**std::shared_ptr都有一个严重的限制，那就是它并不支持动态数组。** 现在C++17 已经支持了。

```cpp
#include <memory>

std::shared_ptr<int[]> sp1(new int[10]()); // 错误，c++17前不能传递数组类型作为shared_ptr的模板参数
std::unique_ptr<int[]> up1(new int[10]()); // ok, unique_ptr对此做了特化

std::shared_ptr<int> sp2(new int[10]()); // 错误，可以编译，但会产生未定义行为，请不要这么做
```

std::shared_ptr 支持数组类型（从 C++17 起），但 std::make_shared 不支持。 boost::make_shared 支持此功能。

如果希望使用shared_ptr管理一个动态数组，那么必须提供一个**删除器**。

```cpp
std::shared_ptr<int> sp1(new int[10](), [](int * ptr){ delete [] p; }); 
```

### [6. 智能指针妙用](#)

智能指针和其他C++特性结合使用的小例子！

#### [6.1 智能指针和哑类](#)

包括所有标准库在内的很多`C++`类都定义了析构函数负责清理对象使用的资源。但是不是所有的类都是这么良好定义的，特别是为`C`和`C++`两种语言设计的类，通常都要求用户手动释放所用的任何资源。与管理动态内存类似，我们可以使用类似的技术来管理不具有良好定义的析构函数。例如我们正在使用一个`C`和`C++`都使用的网络库，我们通过`disconnect`来显示释放：

```cpp
struct destination;  // 表示我们正在连接什么
struct connection;   // 使用连接所需的信息
connection connect(destination *);  // 打开连接
void disconnect(connection);        // 关闭给定的连接
void f(destination &d /* 其他参数 */)
{
    // 获得一个连接, 注意在使用完后要关闭它
    connection c = connect(&d);
    // 使用连接
    // 如果我们再f退出时忘记使用disconnect, 就无法关闭c了
}
```

由于**connection** 没有析构函数，因此不能在`f`结束时由析构函数自动关闭连接。使用`shared_ptr`来管理这种哑类已经被证明是一种有效的方法。

```cpp
void end_connection(connection *p) { disconnect(*p); }
void f(destination &d /* 其他参数 */) 
{
    connection c = connect(&d);
    shared_ptr<connection> p(&c, end_connection);
    // 使用连接
    // 当f退出时（即使是异常退出）, connection也能被正确关闭
}
```

重写一下前面的连接程序，用`unique_ptr`来代替`shared_ptr`：

```cpp
void f(destination &d /* 其他需要的参数 */)
{
    connection c = connect(&d); // 打开连接
    // 当p被销毁时，连接将被关闭
    unique_ptr<connection, decltype(end_connection)*>
        p(&c, end_connection);
    // 使用连接
    // 当f退出时（即使是异常退出），connection也会被正确关闭
}
```

#### [6.2 智能指针使用注意事项](#)

为了正确使用智能指针，我们需要遵守一些规范：

- 不使用相同的内置指针值初始化或`reset`多个智能指针
- 不`delete get()`返回的指针
- 不使用`get()`初始化或`reset`另一个智能指针
- 如果你使用`get()`返回的指针，记住当最后一个对应的智能指针销毁后，那你的指针就无效了
- 如果你使用智能指针管理的资源不是`new`分配的内存，记住传递给它一个删除器

### [7. weak_ptr](#)

**weak_ptr是一种不控制所指向对象生存期的智能指针，它指向由一个shared_ptr管理的对象**。和shared_ptr以及unique_ptr类型指针一样，weak_ptr智能指针也是以模板类的方式实现的。将一个`weak_ptr`绑定到一个`shared_ptr`不会改变`shared_ptr`的引用计数，weak_ptr类型指针并不会影响所指堆内存空间的引用计数。

**除此之外，weak_ptr<T>模板类中没有重载*和->运算符，这也就意味着，weak_ptr类型指针只能访问所指的堆内存，而无法修改它**。

[**主要作用，解决share_ptr的循环引用问题！**](#)

一旦最后一个指向对象的`shared_ptr`被销毁，对象就会被释放。即使有`weak_ptr`指向对象，对象也还是会被释放。 

#### [7.1 构造函数](#)

```cpp
constexpr weak_ptr() noexcept;  //默认构造函数。构造空 weak_ptr 。
weak_ptr( const weak_ptr& r ) noexcept; // 构造新的 weak_ptr ，它共享 r 所管理的对象。
template< class Y >
weak_ptr( const weak_ptr<Y>& r ) noexcept;
template< class Y >
weak_ptr( const std::shared_ptr<Y>& r ) noexcept;
weak_ptr( weak_ptr&& r ) noexcept; // 移动构造函数。
template< class Y >
weak_ptr( weak_ptr<Y>&& r ) noexcept;
```

**weak_ptr指针创建一个weak_ptr指针**，有以下3种方式：

* 可以创建一个空的weak_ptr指针，例如：
```cpp
std::weak_ptr<int> wp1;
```
* 凭借已有的weak_ptr指针，可以创建一个新的weak_ptr指针，例如：
```cpp
std::weak_ptr<int> wp2(wp1);
```
若wp1为空指针，则wp2也为空指针；反之，如果wp1指向某一shared_ptr指针拥有的堆内存，则wp2也指向该块存储空间（可以访问，但无所有权）。

* weak_ptr指针更常用于指向某一shared_ptr指针拥有的堆内存，因为在构建weak_ptr指针对象时，可以利用已有的shared_ptr指针为其初始化。例如：
```cpp
std::shared_ptr<int> sp(new int);
std::weak_ptr<int> wp3(sp);
```
由此，wp3指针和sp指针有相同的指针。再次强调，weak_ptr类型指针不会导致堆内存空间的引用计数增加或减少。

#### [7. 2 weak_ptr模板类提供的成员方法](#)

和shared_ptr\<T\>以及unique_ptr\<T\>相比，weak_ptr\<T\>模板类提供的成员方法不多，下表罗列了常用的成员方法及各自的功能。

|`成员方法`|`功能`|
|:---|:---|
|operator=()|`重载=赋值运算符，weak_ptr指针可以直接被weak_ptr或者shared_ptr类型指针赋值。`|
|swap(x)|`其中x表示一个同类型的weak_ptr类型指针，该函数可以互换2个共同类型weak_ptr指针的内容。`|
|reset()|`将当前weak_ptr指针置为空指针。`|
|use_count()|`查看指向和当前weak_ptr指针相同的shared_ptr指针的数量。`|
|expired()|`判断当前weak_ptr指针是否过期（指针为空，或者指向的堆内存已经被释放）。`|
|lock()|`如果当前weak_ptr已经过期，则该函数会返回一个空的shared_ptr指针；反之，该函数返回一个和当前weak_ptr指针指向相同的shared_ptr指针。`|

再次强调，weak_ptr<T>模板类没有重载*和->运算符，因此weak_ptr类型指针只能访问某一shared_ptr指针指向的堆内存空间，无法对其进行修改。

下面的样例演示了weak_ptr指针以及上表中部分成员方法的基本用法：
```cpp
#include <iostream>
#include <memory>

using namespace std;

int main(int argc, char const* argv[])
{   
	std::shared_ptr<int> sp1(new int(10));
	std::shared_ptr<int> sp2(sp1);
	std::weak_ptr<int> wp(sp2);
	std::cout << wp.use_count() << std::endl;//2
   //输出和wp同指向的shared_ptr类型指针的数量
	sp2.reset();//释放sp2
	std::cout << wp.use_count() << std::endl;//1
	std::cout << *(wp.lock()) << std::endl; //借助lock()函数，返回一个和wp同指向的shared_ptr类型指针，获取其存储的数据
	return 0;
}
```

### [8. allocator](#)

**new有一些灵活性上的局限，其中一方面表现在它将内存分配和对象构造组合在了一起。类似的，delete将对象析构和内存释放组合在了一起。**

分配器是**负责封装堆内存管理的对象**，它们在整个标准库中使用，特别是STL容器使用它们来管理容器内部的所有内存分配。

标准库中包含一个名为allocator的类，允许我们将分配和初始化分离。使用allocator通常会提供更好的性能和更灵活的内存管理能力。

当分配一大块内存时，我们通常计划在这块内存上按需构造对象。在此情况下，我们希望将内存分配和对象构造分离。这意味着我们可以分配大块内存，但只在真正需要时才真正执行对象的创建操作(同时付出一定开销)

```cpp
template< class T >
struct allocator;
```

**标准库allocator类定义在头文件memory中，它帮助我们将内存分配和对象构造分离开来**。**它提供一种类型感知的内存分配方法，它分配的内存是原始的、未构造的**。

[**allocator 出现的原因：**](#)

> allocator是只分配内存不构造对象，一般的new支持不了这种能力，这是专门配合placement new用的。C里其实都是malloc/init/destruct/free四个步骤分开的，C++为了简单也为了完备把前两个合成new，后两个合成delete，但是容器的时候还是需要分开。

#### [8.1 构造函数](#)

**allocator**是一个模板。为了定义一个**allocator**对象，我们必须指明这个**allocator**可以分配的对象类型。当一个allocator对象分配内存时，它会根据给定的对象类型来确定恰当的内存大小和对齐位置。

```cpp
constexpr allocator() noexcept;
template< class U >
allocator( const allocator<U>& other ) throw();
template< class U >
constexpr allocator( const allocator<U>& other ) noexcept;
```

例子：

```cpp
std::allocator<int> alloc;
int * p = alloc.allocate(10); // int array len: 10
//如果是一个类，此地不会调用构造函数
```

```cpp
std::allocator<Student> alloc;
Student* stu = alloc.allocate(1); //构造函数未调用
alloc.deallocate(stu, 1); //析构函数未调用
```

#### [8.2 分配器 allocators的接口](#)

| 成员函数         | 函数接口                                                     | 返回值                                                       |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **a.allocate**   | T* allocate( std::size_t n );                                | `分配一段原始的、未构造的内存，保存n个类型为T的对象`         |
| **a.deallocate** | constexpr void deallocate( T* p, std::size_t n );            | `释放T*类型的指针p地址开始的内存，这块地址内存保存了n个类型为T的对象。` `p必须是 allocate方法返回的指针！ 在调用deallocate之前，需要多n个对象一一调用destory` |
| **a.construct**  | void construct( U* p, Args&&... args ); **[c++17 弃用 20 移除]** | `用全局的布置 new，在 p 指向的未初始化存储中构造 T类型对象。` |
| **a.destory**    | void destroy( U* p );**[c++17 弃用 20 移除]**                | 调用 p 指向的对象的析构函数。                                |

allocatro**分配的内存是未构造的(unconstructed)**。我们按需要在此内存中构造对象。在新标准库中，construct成员函数接受一个指针和零个或多个额外参数，在给定位置构造一个元素。额外参数用来初始化构造的对象。类似make_shared的参数，这些额外参数必须是与构造的对象的类型相匹配的合法的初始化器。

```cpp
#include<memory>  /* C++ 17 之前的版本 */

std::allocator<Student> alloc;
Student* stu = alloc.allocate(1); //构造函数未调用
alloc.construct(stu, "remix", 59, false); //调用构造函数
/* 使用 */
std::cout<< stu->toString() << std::endl;

alloc.destroy(stu); //调用析构函数
alloc.deallocate(stu, 1); 
```

在早期版本的标准库中，construct只接受两个参数：指向创建对象位置的指针和一个元素类型的值。因此，我们只能将一个元素拷贝到未构造空间中，而不能用元素类型的任何其它构造函数来构造一个元素。还未构造对象的情况下就使用原始内存是错误的。为了使用allocator返回的内存，我们必须用construct构造对象。使用未构造的内存，其行为是未定义的。

当我们用完对象后，必须对每个构造的元素调用destroy来销毁它们。函数destroy接受一个指针，对执行的对象执行析构函数。我们只能对真正构造了的元素进行destroy操作。一旦元素被销毁后，就可以重新使用这部分内存来保存其它string，也可以将其归还给系统。释放内存通过调用deallocate来完成。我们传递给deallocate的指针不能为空，它必须指向由allocate分配的内存。而且，传递给deallocate的大小参数必须与调用allocate分配内存时提供的大小参数具有一样的值。

**C++ 17 以后的代码！**

```cpp
std::allocator<Student> alloc;
Student* stu = alloc.allocate(1); //构造函数未调用
std::construct_at(stu, "remix", 59, false);
/* 使用 */
std::cout<< stu->toString() << std::endl;
std::destroy(stu, stu + 1); //析构函数调用
alloc.deallocate(stu, 1); 
```

在标头 `<memory>` 定义了两个方法代替对象方法 ，destory和construct。

```
template<class T, class... Args>
constexpr T* construct_at( T* p, Args&&... args );
template< class ForwardIt >
constexpr void destroy( ForwardIt first, ForwardIt last );
```

标准库还为**allocator**类定义了两个伴随算法，可以在未初始化内存中创建对象。它们都定义在头文件**memory**中，如下：










-----
[`时间`: `[2023/2/8 日第三次修改完成]` ](#)