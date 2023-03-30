### [C++ OOP 对象和类](#)
**介绍:**  面向对象编程 OOP是一种特殊的，设计程序的概念性方法，C++通过一些特性改进了C语言！抽象封装/数据隐藏多态继承代码的可重用性。

----

- [1. CPP 类的概述](#1-cpp-类的概述)
- [2. this指针](#2-this指针)
- [3. 对象数组](#3-对象数组)
- [4. 类作用域](#4-类作用域)
- [5. 将对象作为参数](#5-将对象作为参数)
- [6. 类数据成员的初始化](#6-类数据成员的初始化)
- [7. 字面值常量类](#7-字面值常量类) 

---
### [1. CPP 类的概述](#)
类 （Class） 是面向对象程序设计 （OOP，Object-Oriented Programming） 实现信息封装的基础。类是一种用户定义的抽象数据类型，也称类类型，基本思想是**数据抽象**和**封装**。

每个类包含数据说明和一组操作数据或传递消息的函数，类的实例称为对象。通常，我们会将类的定义放在头文件.h 当中，并将实现的代码放在源代码 .cpp 文件中。 **OOP**三大核心： 封装 多态 继承。

[需要常记的是 抽象和多态，要区分多态和重载！ OOP四大特点是: **封装** **继承** **抽象** **多态**。 ](#)

* **封装 -- 将数据和对数据操作进行整合**，就是将要访问的数据保护起来，不让外界直接访问类的属性和字段，而是对外提供有限的访问接口，授权外部仅能通过类提供的接口进行访问！  
* **继承 -- 实现代码的复用**: 继承在编程语言里最直接的体现就是父子类的关系，继承最大的好处就是能够实现代码的复用！  
* **抽象 -- 抽象更多的是能够让程序的设计和实现分离。** 抽象主要指的是隐藏方法的具体实现，让方法的调用者无需关心方法的具体实现，只需要关心类提供了哪些功能即可! 例如 Java的接口和抽象类。
* **多态 -- 多态指的是子类可以替换父类!**  对同一个完全相同的消息，所表现出来的动作是各不相同的！ 
  **重载** -- 函数名相同，但是函数的参数不同，调用时根据参数的不同决定调用哪一个函数。

首先是关键字 class，它表示我们声明的是一个类，一般类名看以后项目如何规定！例如定义一个用户持股类。
private 修饰符下的方法和数据是私有的，外界无法访问，只能在类定义中使用！ public修饰符下面的方法和数据是共有的，外界可以访问！ 类设计者和使用者之间的接口！

**类的定义描述的是数据格式及其用法！对象(实例化)是根据数据格式规范创建的实体！**
shareholding.h

```cpp
#ifndef ___SHAREHOLDING_H_JX__
#define ___SHAREHOLDING_H_JX__  '___SHAREHOLDING_H_JX__'
#include<string>

//表示股票持股信息
class Shareholding
{
private:
    /* data */
    std::string user_id;
    std::string company_name;
	long share_stotal_count; //股票总数
	double share_price; //平均持股 成本价格
    double total_val; //更新价值
    //保护的内联函数
protected://派生类函数及友元函数 可以访问
    inline void set_total(){ 
        total_val = share_price * share_stotal_count;
    };
public:
    Shareholding(const std::string &user,const std::string &name, long count, double price); //构造函数
    Shareholding(const Shareholding& other) = delete;  //没写 不允许调用
    Shareholding(const Shareholding&& other) = delete; //没写 不允许调用
    ~Shareholding(); //析构函数
    bool buy(long number, double price);
    bool sell(long number, double price);
    std::string toString();
    inline double market_value(double marketPrice) { return marketPrice * share_stotal_count; } ;
};

#endif  //___SHAREHOLDING_H_JX__
```

当我们完成了类定义之后， 还需要来实现类当中的函数。们通常会在同名的cpp文件当中实现，实现的时候，需要使用作用域解析运算符来表示函数所属的类：  Shareholding::
```cpp
bool Shareholding::buy(long number, double price) {
    //...
}
```
这样就表明了 buy 函数所属 Shareholding 这个类，这也就意味着我们可以创建属于其他类的同名函数, 在成员函数当中，可以访问 private 限定的成员变量

**说明**
1. 定义成员函数要是有作用域解析运算符  **::** 来表示函数所属的类 
2. 类方法可以访问类的private修饰的成分！

shareholding.cpp
```cpp
#include"shareholding.h"

Shareholding::Shareholding(const std::string &user,const std::string &name, long count, double price){
    user_id = user;
    company_name = name;
    share_stotal_count = count;
    share_price = price;
    set_total();
}

Shareholding::~Shareholding(){
    printf("%s that userid:%s have object has been disapper! \n", company_name.c_str(), user_id.c_str());
}

bool Shareholding::buy(long number, double price){
    double nowTotal = total_val + number * price;
    share_stotal_count += number;
    share_price = nowTotal / share_stotal_count;
    set_total();//更新价值
    return true;
}

bool Shareholding::sell(long number, double price){
    if (number <= 0 || number > share_stotal_count)
    {
        printf("卖出股票数量不正确\n");
        return false;
    }

    share_stotal_count -= number;
    set_total();
    return true;
}

std::string Shareholding::toString(){
    std::string str = "user: " + user_id + " have " + "company:" 
    + company_name + " " + std::to_string(share_stotal_count)+ " shares" +
       " cost price: " + std::to_string(share_price) + " total value: " + std::to_string(total_val);
    return str;
}
```

#### [1.1 访问修饰符](#)
类成员的访问限制是通过在类主体内部对各个区域标  public、private、protected 来指定的。 常见的访问修饰符， public（共有成员）private（私有成员）protected（保护成员）;

* public（共有成员）: 修饰成员在任意地方都可以访问。公有成员在程序中类的外部是可访问的。可以不使用任何成员函数来设置和获取公有变量的值。
* private（私有成员）: 修饰的成员只能够在类中或者友元函数中可以访问。私有成员变量或函数在类的外部是不可访问的，甚至是不可查看的。只有类和友元函数可以访问私有成员。默认情况下，类的所有成员都是私有的。 width 是一个私有成员。
* protected（保护成员）: 修饰的成员可以在类中的函数、子类函数及友元函数中访问。 保护成员变量或函数与私有成员类似，但有一点不同，保护成员在派生类（即子类）中是可访问的。

如果类中成员没有修饰，那么默认修饰符就是 private！ 结构体也有访问修饰符，只是默认是public的！ 结构体是纯粹的数据对象！

```cpp
class father
{
public:
	father():father(18){};
	father(int _id):count_id(_id),score(0.0) {};
	virtual ~father() = default;
protected:
	int count_id;
private:
	float score;
};

class son : public father
{
public:
	son():father(18){};
	virtual ~son() = default;
	int getID() { return count_id; } //访问父类的 protected
};
```





#### [1.2 内联函数](#)

我们再回过头来看一下Shareholding这个类的定义，在类的定义当中，有一个叫做set_total的函数。我们直接在类当中实现了逻辑。
虽然同样是成员函数，但是在类当中直接实现的函数是有所区别的。在类声明当中实现的函数，一般会被视为是内联函数。般我们会把一些比较简短的函数在类的声明当中直接实现，当然我们也可以使用关键字inline，手动指定某个函数是内联的。

(不疑惑就跳过这句话)你可能很疑惑，那么怎么实现私有函数呢！很简单，你在private中声明函数，但是不定义函数。而在 .cpp 文件中定义函数就可以了！

**如果你在 .h文件中声明并定义了函数，那么即使你没加 inline 修饰符。这个函数默认位 内联函数！**
#### [1.3 使用类](#)
类中定义的方法对所有对象共享，但是每个新对象都有自己的存储空间，用于存储内部变量和类成员。 创建类对象，可以直接声明类变量，也可以使用new为类对象分配存储空间！ new创建类对象(返回的是指针类型)，使用完后需使用delete删除，跟申请内存类似
可以将对象作为函数的参数和返回值，也可以将一个对象赋给另一个。

创建一个类可以显示调用某个构造函数，也可以使用C++ 11新语法

```cpp
Shareholding byd = Shareholding(uid, name, 0, 0);
//显示调用某个构造函数
```

```cpp
Shareholding byd(uid, name, 0, 0);
//C++新语法
Student remix{uid, name, age};
//大括号语法也支持
```

```cpp
#include <iostream>
#include <string.h>
#include <math.h>
#include"shareholding.h"

int main(int argc, char const* argv[])
{
    std::string  uid = "2016110418";
    std::string  name = "比亚迪";

    Shareholding byd = Shareholding(uid, name, 0, 0);
    
    std::cout << byd.toString() <<std::endl;

    byd.buy(1000, 345.12);

    std::cout << byd.toString() <<std::endl;
    return 0;
}
/*
user: 2016110418 have company:比亚迪 0 shares cost price: 0.000000 total value: 0.000000
user: 2016110418 have company:比亚迪 1000 shares cost price: 345.120000 total value: 345120.000000
比亚迪 that userid:2016110418 have object has been disapper! 
*/
```

一. new创建类对象与不new区别, 下面是自己总结的一些关于new创建类对象特点：  

1. new创建类对象需要指针接收，一处初始化，多处使用! 直接访问成员需要使用  -> 符
2. new创建类对象使用完需delete销毁
3. **new创建对象直接使用堆空间 需要手动删除，回收内存，而局部不用new定义类对象则使用栈空间 弹栈的时候会自动消亡**
4. new 创建的对象指针用途广泛，比如作为函数返回值、函数参数等
5. 频繁调用场合并不适合new，就像new申请和释放内存一样

```cpp
#include <iostream>
#include <string.h>
#include <math.h>
#include"shareholding.h"

int main(int argc, char const* argv[])
{
    std::string  uid = "2016110418";
    std::string  name = "比亚迪";

    Shareholding *byd = new Shareholding(uid, name, 0, 0);;
    
    std::cout << byd->toString() <<std::endl;

    byd->buy(1000, 345.12);

    std::cout << byd->toString() <<std::endl;

    delete byd; //千万别忘了
    return 0;
}
```

#### [1.4 类的构造函数](#)
C++ 中的构造函数是一种特殊的方法， **当创建类的对象时会自动调用该方法。 要创建构造函数，必须使用与类相同的名称作为构造函数的函数名，后跟括号()，构造函数可以重载。没有返回值！** 用来初始化内部变量。构造函数只能在创建对象的时候调用，不能在对象上调用！

1. **无参构造**：当没有编写构造函数时，编译器会自动生成一个什么都不做的无参构造函数 -- 默认构造函数
2. **有参构造**：程序员自己编写的构造函数，一旦编写后，无参构造就不再生成，此时创建对象时如果不提供参数就会出错，有两种解决方法，一种对有参构造的所有参数设置默认值，或者手动编写一个空的无参构造。构造函数的参数名称不要和类的字段同名！ 


一般来说，为了安全使用，你至少需要定义以下构造函数, 并重载 = 号运算符！ 以Student 为例：
为啥要自己定义并实现他们呢？ 因为为了防止编译器自动给你生成一堆默认的玩意儿！你不能控制它的行为! 导致一些bug！

* **无参构造函数**      Student()! 虽然你定义了有参构造函数就不用再定义无参构造函数，但是推荐自己定义一个！
* **有参构造函数**      (如果函数有数据成员) Student(参数列表)
* **左值复制构造函数**！ 用一个对象去初始化另一个新的对象 Student(const Student &)
* **右值引用与移动构造函数**! 为了应对你的构造函数参数是其他函数的返回值！ Student(const Student &&)
* **赋值运算符**  Student& operator = (const Student &student);

student.h
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
    std::string ToString();//转换为字符串方法
    bool setAge(unsigned short new_age);
    const unsigned short& getAge() const;
};
#endif
```

student.cpp
```cpp
#include"student.h"
//使用委托构造函数
Student::Student(/* args */):Student("", "", 0)
{
    printf("调用无参构造函数 \n");
}

Student::Student(std::string stu_id, std::string student_name, unsigned short student_age){
    printf("调用有参构造函数 \n");
    student_id = stu_id;
    name = student_name;
    age = student_age;
}

Student::Student(const Student & student){
    printf("调用复制构造函数 \n");
    student_id = student.student_id;
    name = student.name;
    age = student.age;
}

Student::Student(const Student && student){
    printf("调用移动构造函数 \n");
    student_id = student.student_id;
    name = student.name;
    age = student.age;
}

Student& Student::operator=(const Student & student){
    if(&student == this) return (*this);
    printf("调用赋值运算符重载函数 \n");
    student_id = student.student_id;
    name = student.name;
    age = student.age;
    return (*this); //this是指向当前对象的指针
}

std::string Student::ToString(){
    std::string val = "ID：" + student_id + " Name: " + name + " Age:" + std::to_string(age);
    return val;
}


Student::~Student()
{
    printf("属性 student_id为: %s 的 Student 对象消亡了！\n", student_id.c_str());
}


bool Student::setAge(unsigned short new_age){ 
    if (new_age >= 0 && new_age <= 150)
    {
        age = new_age;
        return true;
    }
    return false;
};

const unsigned short& Student::getAge() const{
    return age;
}
```

vs 2019的编译器会默认提供两个构造函数，即使在你已经写了一个构造函数之后。

```cpp
Student(Student &);  //复制构造函数
Student(Student &&); //右值引用与移动构造函数
```

这也太变态了，怎么不受控制啊

**使用构造函数**  

C++提供了两种使用构造函数来初始化对象的方式。 第一种是显示地调用构造函数：
```cpp
Student xiaohu = Student("2021220604880", "李元昊", 24);
```
第二种，是隐式地调用构造函数 **推荐使用**
```cpp
Student xiaohu("2021220604880", "李元昊", 24);

Student xiaohu{"2021220604880", "李元昊", 24};
```
第三种，是使用new一起使用
```cpp
Student *xiaohu = new Student("2021220604880", "李元昊", 24);
cout << (*xiaohu).ToString() << endl;

delete xiaohu;
```

调用默认构造函数(无参数)
```cpp
Student jx; //调用默认构造函数
Student me(); //调用默认构造函数
Student kicker = Student();//调用默认构造函数
```


```cpp
#include <iostream>
#include <string.h>
#include <math.h>
#include"student.h"

int main(int argc, char const* argv[])
{
    std::string  uid = "2016110418";
    std::string  name = "Remix";
    unsigned short age = 15;


    Student remix(uid, name, age); //调用有参构造函数
    Student lzm = remix;//调用复制构造函数 有新对象参数所以调用复制构造函数
    Student liu("2016110423", "liuyue", 18); //调用有参构造函数
    lzm.setAge(24);//调用赋值运算符重载函数 

    liu = lzm;
    Student liming = Student(lzm);//调用复制构造函数
    liming.setAge(25);
    std::cout << remix.ToString() << std::endl;
    std::cout << liu.ToString() << std::endl;
    std::cout << lzm.ToString() << std::endl;
    std::cout << liming.ToString() << std::endl;
    return 0;
}
/*
调用有参构造函数 
调用复制构造函数 
调用有参构造函数 
调用赋值运算符重载函数 
调用复制构造函数 
ID：2016110418 Name: Remix Age:15
ID：2016110418 Name: Remix Age:24
ID：2016110418 Name: Remix Age:24
ID：2016110418 Name: Remix Age:25
属性 student_id为: 2016110418 的 Student 对象消亡了！
属性 student_id为: 2016110418 的 Student 对象消亡了！
属性 student_id为: 2016110418 的 Student 对象消亡了！
属性 student_id为: 2016110418 的 Student 对象消亡了！
*/
```

注意： 构造函数参数名称，不能和类的私有成员变量参数名称一样!

问题：当你使用一个对象去初始化另一个对象时，编译器将自动生成复制构造函数(因为他创建对象的一个副本);


#### [1.5 类的析构函数](#)
用构造函数创建对象后，程序负责跟踪对象，直到过期为止。对象过期时，程序将自动调用一个特殊的成员函数 - 析构函数完成清理工作！
例如： 构造函数中使用 new 来申请内存，则析构函数将使用 delete来释放内存！声明一个析构函数只需要函数名和类名一致但是需要在类名前加上 ~

构造函数和析构函数一样，都没有返回值。但是析构函数比构造函数多一点 **没有参数**
```cpp
~Stock(){
	/**/
}

~Student(){
	/**/
}
```
如果你不定义一个析构函数，那么编译器会隐式地声明一个默认析构函数，并在发现导致对象被删除的代码后，提供默认析构函数的定义！ 析构函数将在程序结束的时自动被调用。

* 如果创建的是静态存储类对象，那么析构函数将在程序结束时被自动调用！
* 如果创建的是自动存储类对象。则析构函数将在程序执行完代码块时自动调用！
* 如果对象是new创建的时候，则它驻留在堆栈中，当时候delete释放内存的时候，析构函数会被自动调用！

看懂这个哦！
```cpp
#include <iostream>
#include <string.h>
#include <math.h>
#include"student.h"

int main(int argc, char const* argv[])
{
    std::string  uid = "2016110418";
    std::string  name = "Remix";
    unsigned short age = 15;

    Student remix(uid, name, age); //调用有参构造函数
    std::cout<< "remix address:" << &remix << remix.ToString() << std::endl;
    remix = Student("2016110417","yami", 24);
    std::cout<< "remix address:" << &remix << remix.ToString() << std::endl;
    return 0;
}

/*
调用有参构造函数 
remix address:0x7ffdf591cd60ID：2016110418 Name: Remix Age:15
调用有参构造函数 
调用赋值运算符重载函数 
属性 student_id为: 2016110417 的 Student 对象消亡了！
remix address:0x7ffdf591cd60ID：2016110417 Name: yami Age:24
属性 student_id为: 2016110417 的 Student 对象消亡了！
*/
```

#### [1.6 C++列表实例化](#)
C++11中，可以将列表初始化语法用于类。只要提供与某个构造函数的参数列表匹配的内容并用大括号

```cpp
Student xiaohu = {"2021220604880", "李元昊", 24};
Student lzm {"2021220604881", "李志明", 24};
```

#### [1.7 const 修饰类](#)
如果我们用 const 修饰类的对象，那么我们将无法修改。这样将导致无法调用对象方法！因为类的方法可以修改内部成员！所以对内部方法声明要有所改变！

```cpp
const Student xiaohu = Student("2021220604880", "李元昊", 24);
cout << xiaoming.ToString() << endl; //无法调用 编译器不通过
```

我们需要保证函数不会修改调用对象，C++的解决方法是将const关键字放在函数的括号后面

.h
```cpp
std::string ToString() const;
```
.cpp
```cpp
std::string Student::ToString() const {
	std::string val = "学号：" + student_id + " 姓名: " + name + " 年龄:" + std::to_string(age);
	return val;
}
```
这种方式声明和定义的函数被称为 const 成员函数。 规则:  **只要类的方法不修改调用对象，就应该将其声明为const。** **[你应该遵守这个规则]**

#### [1.8 C++ 11成员初始化列表语法](#)
只有构造函数可以使用这种初始化列表语法 const 数据成员，我们不能像常规的那样使用构造函数来初始化。

```cpp
class A {
    private:
        const int qsize;
    public:
        A(int qs = 10);
}
```
那么下面这种写法是错误的：
```cpp
A::A(int qs) {
    qsize = qs;
}
```
原因是因为，qsize是const的，只能被初始化，不能被赋值，这种情况下，应该怎么办呢？


从概念上说，在执行构造函数时，对象会在执行括号里的代码之前被创建，分配内存，然后流程进入到括号里进行常规赋值操作，所以对于const成员，应该在赋值前进行初始化，即使用成员初始化列表。


这是一种C++提供的特殊语法：写法如下：

```cpp
A::A(int qs) : qsize(qs) {
    //...
}
```

也可以用于初始化非const数据成员
```cpp
kick::Point3D::Point3D(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
	Point3DCount++;
}
```

可以改成如下
```cpp
kick::Point3D::Point3D(double _x, double _y, double _z): x(_x),y(_y),z(_z)
{
	Point3DCount++;
}
```

**何时必须用成员初始化列表**
* 该类的成员变量是个引用
* 该类的成员变量是const类型
* 该类是继承一个基类，并且基类中有构造函数，构造函数里有参数;
* 该类的成员变量类型是类类型，而该类的构造函数带参数时；

**成员初始化列表细节探究**
* 初始化列表中的代码可以看作是被编译器安插到构造函数体中的，只是这些代码有些特殊。
* 这些代码 是在任何用户自己的构造函数体代码之前被执行的。所以大家要区分开构造函数中的用户代码 和 编译器插入的 初始化所属的代码。
* 这些列表中变量的初始化顺序是 定义顺序，而不是在初始化列表中的顺序。不建议 在初始化列表中 进行 两个 都在初始化列表中出现的成员之间的初始化。

### [2. this指针](#)
this 是 C++ 中的一个关键字，**也是一个 const 指针，它指向当前对象，通过它可以访问当前对象的所有成员**。
 所谓当前对象，是指正在使用的对象。例如对于 stu.show(); stu 就是当前对象，this 就指向 stu 的指针。

1. this 只能用在类的内部，通过 this 可以访问类的所有成员，包括 private、protected、public 属性的。
2. 注意，this 是一个指针，要用->来访问成员变量或成员函数。
3. 不能在 static 成员函数中使用
4. this 是 const 指针，它的值是不能被修改的，一切企图修改该指针的操作，如赋值、递增、递减等都是不允许的。


```cpp
unsigned short Student::getAge() const {
	return age;
}

const Student& Student::MaxAge(const Student& stu) const {
	if (stu.getAge() > age)
	{
		return stu;
	}
	else {
		return (*this);
	}
}


Student xiaohu = Student("2021220604880", "李元昊", 20);
Student ming = Student("2021220604881", "李志明", 21);
cout << xiaohu.MaxAge(ming).ToString() << endl;
```


### [3. 对象数组](#)
如果你提供了不带参数的默认构造函数，或者没有写任何一个构造。那么你可以直接声明一个数组，在初始化每个对象的时候，那么会自动调用无参数的隐式构造函数！

如果没有无参数构造函数，那么下面这样写是无法通过编译的！
```cpp
Student friends[5];
```
但是很明显，Student 只有一个带参数的构造函数所以我们需要显式的调用构造函数！
```cpp
#include <iostream>
#include <string.h>
#include <math.h>
#include"student.h"

Student * findAgeMax(Student students[],unsigned int len){
    Student *ptr = nullptr;
    if (len == 0) return nullptr;
    
    int max = students[0].getAge();
    for (int i = 1; i < len; i++)
    {
        unsigned short nowAge = students[i].getAge();
        if (nowAge > max)
        {
            max = nowAge;
            ptr = students + i;
        }
    }
    return ptr;
}

int main(int argc, char const* argv[])
{
    Student friends[5] = {
        Student("2021220604880", "李元昊", 22),
        Student("2021220604881", "李志明", 21),
        Student("2021220604882", "李嘉坤", 24),
        Student("2021220604883", "李世明", 25),
        Student("2021220604884", "李天明", 20)
    };

    for (auto my_friend: friends)
    {
        std::cout << my_friend.ToString() << std::endl;
    }
    
    std::cout << findAgeMax(friends, 5)->getAge() << std::endl;//25


    return 0;
}
```

### [4. 类作用域](#)
一个类就是一个独立的作用域，C++为类引入了一种新的作用域：类作用域，在类中定义的名称(数据成员名和函数名)的作用域都属于类。所以外部不能直接访问类中私有的成员！
类中定义的成员作用域是整个类！ 调用公有成员只能通过类的实例(对象)！ 

#### 4.1 作用域内枚举
传统的枚举，其中两个枚举中定义的枚举量可能会发生冲突。假设有一个处理鸡蛋和T恤的项目，其中包含类似的代码。傻逼吧 这玩意儿。

```cpp
enum egg {small,medium,large,jumbo}
enum t_shirt{small,medium,large,jumbo}
```
这将无法通过编译，因为egg中的small 和 t_shirt中的small位于相同的作用域中，这会引发冲突。解决方案：创建枚举量作用域为类的枚举

```cpp
enum class egg {small,medium,large,jumbo}
enum class t_shirt{small,medium,large,jumbo}
```
也可以使用关键字 struct 代替 class。无论使用哪种方式，都需要使用枚举名来限定枚举量：

```cpp
egg choice = egg::Large;
t_shirt Floyd = t_shirt :: Large;
```
C++11 还提高了作用域内枚举的类型安全。在有些情况下，常规枚举将自动转换为整型，如将其赋给int变量或用于比较表达式时，但作用域内枚举不能隐式地转换为整型：

枚举用某种底层整型类型表示，在 C++98 中，如何选择取决于实现，因此包含枚举的结构的长度可能随系统而异。对于作用域内枚举，C++11消除了这种依赖性。默认情况下，C++11作用域内枚举的底层类型为int，另外，还提供了一种语法，可用于坐出不同的选择：
```cpp
enum class : short pizza { Small, Medium, Large, XLarge};
```

:short 将底层类型指定为 short。底层类型必须为整型。在 C++11 中，也可使用这种语法来指定常规枚举的底层类型，但如果没有指定，编译器选择的底层类型将随实现而异。

```cpp
enum egg {Small, Media, Large, Jumbo};
enum class T_Short: unsigned short{ Small, Media, Large, Jumbo };

egg one = Small;
T_Short Anta = T_Short::Small;
```

#### 4.2 类中枚举
如下所示：

```cpp
class machine{
    /* ... */
public:
    enum {ON, OFF, ERROR}
    /* ... */
}
```
访问方式：
```cpp
int state = machine::ON;
```

### [5. 将对象作为参数](#)
将类的对象作为参数，按照值传递，会发生一些可怕的事情！ 首先要构造一个对象，在传递给函数，函数还会生成一个临时对象，将这个临时对象赋值给函数里面的参数又会创建一个新对象.

下面的操作，总共创建了三个对象！但是本质上三个对象有着同样的内容！ 所以将对象传递给函数，最好传引用或者指针，千万不要按值传递，会非常耗费性能！

```cpp
#include <iostream>
#include <string.h>
#include <math.h>
#include"student.h"
#include <stack>  
#include <list>

void func(Student stu){
    Student me = stu;//调用复制构造函数
    //搞清楚为啥不调用赋值运算符？
    //因为这里有新的对象 me 生成
    //下面的方式才会调用赋值运算符 
    /*
      Student me;//创建me对象
      me = stu;//me已结创建了 没有新对象生成
    */
}

int main(int argc, char const* argv[])
{

    auto lzm = Student("2021220604880", "李元昊", 22);//调用有参构造函数
    func(lzm);//复制构造函数

    return 0;
}
/*
调用有参构造函数 
调用复制构造函数 
调用复制构造函数 
属性 student_id为: 2021220604880 的 Student 对象消亡了！
属性 student_id为: 2021220604880 的 Student 对象消亡了！
属性 student_id为: 2021220604880 的 Student 对象消亡了！
*/
```
### [6 类数据成员的初始化](#)
对内种的数据成员初始化，有几种方法。列表初始化初始化的原因：[**如果没有在构造函数的初始值列表中显式地初始化成员，则该成员将在构造函数体之前执行默认初始化。**](#)
1. 直接在类里面使用初始值初始化
2. 构造函数, 列表初始化

```cpp
class animal{
private:
    std::string typeName {""};
	int age = 10; //类里面初始化 10是初始值
	std::string name;
public:
    //使用构造函数初始化
	animal(std::string _name, int _age, std::string _typeName) {
		this->name = _name;
		this->age = _age;
		this->typeName = _typeName;
	}
    //使用构造函数 + 列表初始化
    animal(const char* _name, const int& _age, const char* _typeName) 
	:name(_name),age(_age), typeName(_typeName){}
}
```

#### [6.1 对类成员的列表初始化](#)
[**推荐一定要使用列表初始化, 如下所示animal没有无参构造函数，下面的代码无法通过！**](#) 
```cpp
class owner
{
private:
	animal pet;
public:
	owner(animal ot) {
		pet = ot; //无法通过 这里是赋值运算符 要求pet先初始化
                  //但是 animal没有无参构造函数 
	};
	~owner() = default;
};
```
[**如果使用列表初始化就可以通过了！**](#)
```cpp
class owner
{
private:
	animal pet;
public:
	owner(animal ot):pet(ot) {};
	~owner() = default;
};
```
```cpp
animal cat("rice", 2, "kitty");
owner me(cat);
//调用了两次 animal的复制构造函数
//第一次是 将cat复制到 owner 构造函数。
//第二次是 pet(ot)。调用一次复制构造函数
```

**加入 animal 类有默认构造函数，不适应列表初始化** **如果说animal有默认构造函数，那么此地将会调用三个不同的函数**
* animal  复制构造函数
* animal  无参构造函数
* animal  赋值运算符 
```cpp
owner(animal ot) {
    pet = ot;
};
```
首先是将cat 复制给 ot,然后是 owner的成员pet调用无参构造函数默认初始化自己，然后再在构造函数里面调用复制运算符。
[**为了防止默认初始化问题，一定要使用列表初始化。这样可以拥有更少的函数调用，更好的性能**](#)

#### [6.2 列表初始化的原因](#)
何时必须用成员初始化列表 **如果成员是const、引用，或者属于某种未提供默认构造函数的类类型，我们必须通过构造函数初始化列表为这些成员提供初值。
这样还可以防止构造内部赋值操作带来的开销。** **最好令构造函数初始值的顺序与成员声明的顺序保持一致。而且如果可能的话，尽力避免使用某些成员初始化其他成员！**

```cpp
//不要使用这种操作
class X{
    int i;
    int j;
public:
    //未定义，i应该在j之前被初始化
    //而不是用j初始化i
    X(int val):j(val),i(j){

    }
}
```

* 该类的成员变量是个**引用**
* 该类的成员变量是 **const类型**
* 该类是继承一个基类，并且**基类中有构造函数**，构造函数里有参数;
* 该类的成员**变量类型是类类型**，而该类的构造函数带参数时；

成员初始化列表细节探究
* 初始化列表中的代码可以看作是被编译器安插到构造函数体中的，只是这些代码有些特殊。
* 这些代码 是在任何用户自己的构造函数体代码之前被执行的。所以大家要区分开构造函数中的用户代码 和 编译器插入的 初始化所属的代码。
* 这些列表中变量的初始化顺序是 定义顺序，而不是在初始化列表中的顺序。不建议 在初始化列表中 进行 两个 都在初始化列表中出现的成员之间的初始化。

### [7. 字面值常量类](#)
在C++中，定义变量的时候可以指定常量属性，说明这个变量成为常量，无法直接改变；这个使用const限定符来限定，在C++中还有
另外一种声明方式，叫做常量表达式（constexpr),那就需要赋值的表达式为编译期间的常量

**字面值的常量类有两种定义：**
1. 数据成员都是字面值类型(算术类型，引用和指针，以及字面值常量类)的聚合类是字面值常量类。
2. 或者满足如下的定义：
    * 数据成员都必须是字面值类型(算术类型，引用和指针，以及字面值常量类)。
    * 类必须至少含有一个constexpr构造函数。
    * 如果一个数据成员含有类内初始值，则内置类型的初始值必须是一条常量表达式。或者如果成员属性某种类类型，则初始值必须使用成员自己的constexpr构造函数。
    * 类必须使用析构函数的默认定义，该成员负责销毁类的对象。

```cpp
#include <iostream>
using namespace std;

class CPoint
{
public:
        constexpr CPoint(int xx, int yy) : x(xx), y(yy){}
        void setx(int xx){x = xx;}
        void sety(int yy){y=yy;}
        constexpr int getx() const {return x;}
        constexpr int gety() const {return y;}
private:
        int x;
        int y;
};
int main(int args, char* argv[])
{
    constexpr CPoint point(100, 200);
    constexpr int data = point.getx() * point.gety();
    cout << data << endl;
    cout << point.getx() * point.gety() << endl;
    return 0;
}
```
尽管构造函数不能是 const 的，但是字面值常量类的构造函数可以是 constexpr函数。事实上，一个字面值常量类必须至少提供一个 constexpr 构造函数。

constexpr构造函数可以声明成=default的形式（或者是删除函数的形式）。

否则，constexpr构造函数就必须既符合构造函数的要求（意味着不能包含返回语句），又符合constexpr函数的要求（意味着它能拥有的唯一可执行语句就是返回语句）。

综合这两点可知，constexpr 构造函数体一般来说应该是空的。我们通过前置关键字 constexpr 就可以声明一个 constexpr 构造函数了：

```cpp
class Dubug{
public:
    constexpr Debug(bool b = true) : hw(b), is(b), other(b) { }
    constexpr Debug(bool h, bool i, bool o) : hw(h), io(i), other(o) { }
    
    constexpr bool any() { return hw || io || other; }
    void set_io(bool b) { io = b; }
    void set_hw(bool b) { hw = b; }
    void set_other(bool b) { hw = b; }
private:
    bool bw;            // 硬件错误，而非IO错误
    bool io;            // IO错误
    bool other;         // 其他错误
};
```
constexpr **构造函数必须初始化所有数据成员**，初始值或者使用constexpr构造函数，或者是一条常量表达式。  
constexpr 构造函数用于生成 constexpr 对象以及 constexpr 函数的参数或返回类型∶

```cpp
constexpr Debug io_sub(false, true, false);
if (io_sun.any()){
    cerr << "print appropriate error message" << endl;
}

constexpr Debug prod(false);
if (prod.any()){
    cerr << "print an error message" << endl;
}
```