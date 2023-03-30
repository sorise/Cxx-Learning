## [C++ Inheritance](#)
**介绍：** 继承可以说是OOP的基本操作了，他可以提供可重用的代码，但是C++有些些变态的地方就是，它支持多继承，并且很可悲的事情是C++并不支持接口类型！

----

- [1. C++ 继承概述](#1-c继承概述)
- [2. 派生类的实现](#2-派生类的实现)
- [3. OOP三原则之多态](#3-oop三原则之-多态)
- [4. 动态联编与静态联编](#4-动态联编与静态联编)
- [5. C++ 11 final override](#5-c11-final-override)
- [6. 抽象类](#6-抽象类)
- [7. has-a关系](#7-has-a关系)
- [8. 多重继承](#8-多重继承)

---
### [1. C++ 继承概述](#)
面向对象编程的主要目的是提供可重用的代码，重用经过测试的可靠代码比重新编写代码好得多，让代码基于可靠的基础上继续前进，继承允许我们依据另一个类来定义一个类，这使得创建和维护一个应用程序变得更容易。

**C++ 继承代表了**  **is a** 关系，从一个类派生出另一个类，前者为**基类**！后者为**派生类**！

派生类不能直接访问基类的私有成员，而必须通过基类(公开)方法进行访问。 我们可以将基类和派生类放在一个文件中！

C++继承牛逼一点的是，它可以分为三种继承 **public 继承** 、 **protected 继承**、  **private 继承** 。

```
class<派生类名>:<继承方式><基类名>
{
    <派生类新定义成员>
};
```

C++  支持多继承，也就是说他可以继承两个及其以上父类！ 比较奇特！其他语言一般都是单继承！

```cpp
// 基类
class Animal
{
    virtual void eat();
    virtual void sleep();
    virtual ~Animal() = default;
};

//派生类
class Dog : public Animal
{
    virtual void eat() override; //重写父类方法
    std::string bark() const override;
    virtual ~Dog() = default;
};
```

派生类可以父类的基础上增加数据，操作方法！还可以修改/重写父类的方法！

#### [1.1 访问权限](#)
公有继承(public)、私有继承(private)、保护继承(protected)是常用的三种继承方式。**基类成员在派生类中的访问权限不得高于继承方式中指定的权限** 
1. **公有继承(public)** 公有继承的特点是基类的公有成员和保护成员作为派生类的成员时，它们都保持原有的状态，而基类的私有成员仍然是私有的，不能被这个派生类的子类所访问。
   1. 当继承方式为 public 时，那么基类成员在派生类中的访问权限将保持不变。也就是说，继承方式中的 public、protected、private 是用来指明基类成员在派生类中的最高访问权限的。

2. **私有继承(private)** 私有继承的特点是基类的公有成员和保护成员都作为派生类的私有成员，并且不能被这个派生类的子类所访问。
3. **保护继承(protected)** 保护继承的特点是基类的所有公有成员和保护成员都成为派生类的保护成员，并且只能被它的派生类成员函数或友元访问，基类的私有成员仍然是私有的。
   1. 当继承方式为 protected 时，那么基类成员在派生类中的访问权限最高也为 protected，高于protected 的会降级为 protected，但低于 protected 不会升级。


* 不管继承方式如何，基类中的 private 成员在派生类中始终不能使用（不能在派生类的成员函数中访问或调用）。
* 如果希望基类的成员能够被派生类继承并且毫无障碍地使用，那么这些成员只能声明为 public 或 protected；只有那些不希望在派生类中使用的成员才声明为 private。
* 如果希望基类的成员既不向外暴露（不能通过对象访问），还能在派生类中使用，那么只能声明为 protected。


#### [1.2 基类: human](#)
我们写一个父类，人类,有姓名，年龄!
```cpp
#include<string>

class human
{
private:
    std::string _first_name;
    std::string _last_name;
    int _age;
public:
    human(/* args */);
    human(const std::string& fisrt_name, const std::string& last_name,const int& age = 0);
    human(const human & _human);
    human(const human && _human);
    human& operator=(const human& _human);
    virtual std::string toString() const;
    ~human();
};

human::human(/* args */):_first_name(""),_last_name(""),_age(0)
{
    printf("call human no paramters constructor... \n");
}

human::human(const std::string& first_name, const std::string& last_name, const int& age)
:_first_name(first_name), _last_name(last_name),_age(age)
{
    printf("call human have paramters constructor... \n");
}

human::human(const human & _human)
:_first_name(_human._first_name), _last_name(_human._last_name),_age(_human._age)
{   
    
    printf("call human copy constructor... \n");
}

human::human(const human && _human)
:_first_name(_human._first_name), _last_name(_human._last_name),_age(_human._age)
{   
    
    printf("call human move constructor... \n");
}

human& human::operator=(const human& _human){
    if (&_human == this) return (*this);
    _first_name = _human._first_name;
    _last_name = _human._last_name;
    _age = _human._age;

    printf("call human = operator... \n");
    return (*this);
}

std::string human::toString() const{
    std::string result = "name: " + _first_name + "." + _last_name + " age: " + std::to_string(_age);
    return result;
}

human::~human()
{
    printf("call human destructor... \n");
}
```
#### [1.3 派生类: worker](#)
我们再实现一个派生类，使用public 继承！ 这个派生类多了新的数据成员：性别,工龄,工号！ 我们同样实现各个构造函数和赋值运算符！

```cpp
class worker:public human
{
private:
    bool _sex;
    int _seniority;//工龄
    const std::string _worker_id;
public:
    worker(/* args */);
    worker(const std::string& fisrt_name, const std::string& last_name,
        const std::string& worker_id, const int& age,
        const bool& sex, const int& seniority);
    worker(const worker & _worker);
    worker(worker && _worker);
    worker& operator=(const worker& _worker);
    std::string toString() const;
    ~worker();
};

std::string worker::toString() const{
    std::string pstr = human::toString() + " workID: " + _worker_id + " seniority:" 
        + std::to_string(_seniority) + " sex:" + std::to_string(_sex);
    return pstr;
}

worker::worker(/* args */):human(),
_sex(false), _seniority(0),_worker_id("")
{
    printf("call worker no paramters constructor... \n");
}

worker::worker(
    const std::string& fisrt_name,
    const std::string& last_name,
    const std::string& worker_id, 
    const int& age,
    const bool& sex, 
    const int& seniority)
:human(fisrt_name, last_name, age), _worker_id(worker_id)
{
    _sex = sex;
    _seniority = seniority;
    printf("call worker have paramters constructor... \n");
}


worker::worker(const worker & _worker)
:human(_worker),_worker_id(_worker._worker_id)
{
    _sex = _worker._sex;
    _seniority = _worker._seniority;
}

worker::worker(worker && _worker)
:human(_worker),_worker_id(_worker._worker_id)
{
    _sex = _worker._sex;
    _seniority = _worker._seniority;
}

worker& worker::operator=(const worker& _worker)
{
    if(this == &_worker) return *this; //自己等于自己的情况
    human::operator=(_worker);
    _sex = _worker._sex;
    _seniority = _worker._seniority;
    return (*this);
}


worker::~worker()
{
    printf("call worker destructor... \n");
} 
```

```cpp
#include <iostream>
#include <string>
#include"human.h"

int main(int argc, char const* argv[])
{
    human remix("simith","mix", 24);

    worker lzm("Li", "ZhiMing", "2016110421", 24, true, 1);
    worker liming("Li", "Ming", "2016110422", 24, true, 0);
    
    human Lm=  liming;

    std::cout <<remix.toString() << std::endl;
    std::cout <<Lm.toString() << std::endl;
    std::cout <<lzm.toString() << std::endl;
    return 0;
}

/*
call human have paramters constructor... 
call human have paramters constructor... 
call worker have paramters constructor... 
call human have paramters constructor... 
call worker have paramters constructor... 
call human copy constructor... 
name: simith.mixage: 24
name: Li.Mingage: 24
name: Li.ZhiMingage: 24 workID: 2016110421 seniority:1 sex:1
call human destructor... 
call worker destructor... 
call human destructor... 
call worker destructor... 
call human destructor... 
call human destructor... 
*/
```

总结: 
1. 使用子类构造器使用 : 调用父类构造器
2. 子类可以直接访问父类中的 public 成员, 可以在内部函数和中访问 protected 成员！不能通过对象直接访问 protected 成员！

#### [1.4 存在继承关系的类型之间的转换规则](#)

存在继承关系的类型之间的转换规则, **不存在子类、父类对象之间的类型转换** ，必须记住的规则！

- 从派生类向基类的类型转换只对指针或引用类型有效
- 基类向派生类不存在隐式类型转换
- 和任何其他成员一样，派生类向基类的类型转换也可能会由于访问受限而变得不可行。

```cpp
worker son; //子类
human father(son); // 调用 human::human(const human & _human)
father = son //调用 human& human::operator=(const human& _human)

human * ptr_f = &son; //派生类指针 转换为 父类指针
human & ptr_f = &son; //派生类引用 转换为 父类引用
```

### [2. 派生类的实现](#)

继承一个基类，派生类需要做许多工作，例如构造函数问题，如何调用父类方法，如何重写父类方法，派生类方法和父类方法重名，复制构造函数问题，赋值运算符问题！

#### [2.1 派生类构造函数](#)
子类可以直接使用基类名称调用基类的构造函数, 他可以通过参数传递决定调用那个基类构造函数！
派生类使用初始化成员列表机制将参数传递给基类的构造函数！ 

**派生类构造函数应该初始化派生类新增的成员同时使用成员初始化列表调用父类构造函数！**

* 创建对象的时候，首先调用基类的构造函数，然后才调用子类的构造函数
* 释放对象的时候，首先调用子类的析构函数，然后才调用父类的析构函数

```
派生类名::派生类名(参数):基类类名(参数) { 
    /*构造函数细节*/
}
```

```cpp
worker::worker(/* args */):human(),
_sex(false), _seniority(0),_worker_id("")
{
    printf("call worker no paramters constructor... \n");
}

worker::worker(
    const std::string& fisrt_name,
    const std::string& last_name,
    const std::string& worker_id, 
    const int& age,
    const bool& sex, 
    const int& seniority
)
:human(fisrt_name, last_name, age), _worker_id(worker_id)
{
    _sex = sex;
    _seniority = seniority;
    printf("call worker have paramters constructor... \n");
}
```

#### [2.2 子类赋值给基类](#)
基类指针可以在不进行显式类型转换的情况下指向派生类，基类引用可以在不进行显式类型转换的情况下引用派生类。但是这样的指针或者引用只能调用基类的方法！ 反之则不行 

**记住：这里会有问题，因为将子类赋值给父类，这个指引/引用 调用的析构函数就不是子类的析构函数了！ 而是父类的析构函数，导致在子类析构函数中该释放的内存没有被释放掉，会有内存泄露的危险**

也可以将派生类传递给基类的复制构造函数
```cpp
#include <iostream>
#include <string>
#include"human.h"

int main(int argc, char const* argv[])
{

    worker lzm("Li", "ZhiMing", "2016110421", 24, true, 1);
    human * lzm_ptr = &lzm;
    human& lzm_ref = lzm;

    worker remix("J", "X", "2016110418", 24, true, 1);
    //将派生类传递给基类的复制构造函数
    human jx(remix);

    std::cout << lzm_ptr->toString() <<std::endl;
    //name: Li.ZhiMing age: 24 workID: 2016110421 seniority:1 sex:1
    std::cout << lzm_ref.toString() <<std::endl;
    ////name: Li.ZhiMing age: 24 workID: 2016110421 seniority:1 sex:1
    std::cout << jx.toString() <<std::endl;
    //name: J.X age: 24
    return 0;
}
```

#### [2.3 派生类复制构造函数](#)
在派生类中实现构造函数，同理也需要调用基类的复制构造函数，只需要将参数传递给基类复制构造函数就可以了

由于子类可以赋值给父类所以这种操作是合理的！
```cpp
worker::worker(const worker & _worker)
:human(_worker)  /* 调用基类复制构造函数 */
,_worker_id(_worker._worker_id)
{
    _sex = _worker._sex;
    _seniority = _worker._seniority;
}
```

#### [2.4 移动构造函数](#)
和复制构造函数差不多的

```cpp
worker::worker(worker && _worker) //移动构造函数
:human(std::move(_worker)), //父类也要移动
_worker_id(_worker._worker_id)
{
    _sex = _worker._sex;
    _seniority = _worker._seniority;
}
```

#### [2.5 调用父类方法](#)
在派生类中调用父类方法，需要用到父类名称和作用域限定符
```cpp
human::operator=(_worker);

//调用父类方法
human::toString();
```

#### [2.6 赋值运算符](#)
这里，需要调用父类方法！

```cpp
worker& worker::operator=(const worker& _worker)
{
    if(this == &_worker) return *this; //自己等于自己的情况
    human::operator=(_worker); //调用父类赋值运算符
    _sex = _worker._sex;
    _seniority = _worker._seniority;
    return (*this);
}
```

#### [2.7 静态成员](#)
C++的静态成员只能继承，不能重写，更不能声明为virtual！ **C++ 规定：方法不能同时是static和virtual。**


### [3. OOP三原则之多态](#)
同样的行为作用于不同的个体产生不同的效果。 关于这个OOP原则，就涉及到另外一个核心原则——“推迟作出决定的时机”。

* 定义接口就是假装作出了这个决定。
* 而实现接口则是真正地作出这个决定。
* 上面两个行为是有时间差的。而且这个时间差越大，你的程序就越灵活。
* 熟悉Java的同学应该都知道多态属于一种 动态绑定 吧。所谓的动态绑定不就是到真正需要的时候才去找实现者吗？

同样的行为对于父类和子类其行为是不同的！ 意味着子类要重写/修改父类的方法！ **方法的行为应取决于调用该方法的对象！** C++ 有一种重要的机制可以用于实现多态公有继承。

* 在派生类中重写定义基类的方法
* 使用虚方法

#### [3.1 虚函数 virtual](#)
在进行类的继承的时候所有被virtual关键字修饰父类的函数都会被子类继承，在子类中也可以对该函数进行重写。对继承函数的重写也是C++多态的体现之一

虚函数是一种特殊的函数，当我们使用virtual修饰一个类方法时，他就成为了一个虚函数，虚函数允许子类对其进行重写以实现不同的函数效果  我们常使用virtual修饰析构函数以防止内存问题。 主要是防止将子类传递给父类指针或者引用，导致没有调用子类的析构函数！

**C++中基类采用virtual虚析构函数是为了防止内存泄漏。 但不能用于构造函数 p413、友元**

纯虚函数 纯虚函数与虚函数的区别在于纯虚函数没有函数体而是用=0进行代替，纯虚函数会强制子类对其进行重写。

virtual，望文生义。虚拟的意思。它是用来修饰一个方法的，当编译器看到这个关键字，就懂了。哦，这个方法调用的时候，要看实际对象的真正类型，而非它的指针或者引用的类型。简而言之，virtual，实现动态绑定。

#### [3.2 父子类方法重名不用virtual](#)
我们可以试试如果不使用virtual看看，程序将会如何运行！ 

eat, viewAnimal 在父类和子类中都存在都没有virtual！，并且析构函数没有加上 virtual！ 程序会按照指针/引用的类型选择方法！
```cpp
class animal
{
protected:
    std::string _name;
    std::string _environment_type;
    int _age;    
public:
    animal(std::string name, int age, std::string env);
    std::string eat() const;
    animal& operator=(const animal & other);
    void viewAnimal() const;
    ~animal();

};

animal& animal::operator=(const animal & other){
    
    if (&other == this) return *this;
    _name = other._name;
    _age = other._age;
    _environment_type = other._environment_type;
    return *this;
}



animal::animal(std::string name, int age, std::string env)
{
    printf("animal appear !!!\n");
    _name = name;
    _age = age;
    _environment_type = env;
}

std::string animal::eat() const{
    printf("animal eat food \n");
    std::string rlt = "animal shit";
    return rlt;
}

void animal::viewAnimal() const {
    printf("animal - name: %s, age: %d, enviroment: %s \n", _name.c_str(), _age, _environment_type.c_str());
}


animal::~animal()
{
    printf("animal diappear !!!\n");
}
```

子类
```cpp
//哺乳动物
class mammal: public animal
{
private:
    float _suitable_temperature; //适宜温度
    std::string _sort;
public:
    mammal(std::string name, int age, std::string env, float temperature, std::string sort);
    std::string eat() const;
    void viewAnimal() const;
    ~mammal();
};

mammal::mammal(std::string name, int age, std::string env, float temperature, std::string sort)
:animal(name, age, env)
{
    printf("manmal appear !!!\n");
    _suitable_temperature = temperature;
    _sort = sort;
}

std::string mammal::eat() const{
    printf("mammal eat good food \n");
    std::string rlt = "mammal shit";
    return rlt;
}

void mammal::viewAnimal() const {
    printf("mammal - name: %s, age: %d, enviroment: %s suitable temperature: %f ℃ ,sort: %s \n", 
    _name.c_str(), _age, _environment_type.c_str(), 
    _suitable_temperature, _sort.c_str());
}

mammal::~mammal()
{
    printf("mammal diappear !!!\n");
}
#endif
```

引用调用结果 堆栈结束后才调用 mammal的析构函数
```cpp
#include <iostream>
#include <string>
#include"animal.h"

int main(int argc, char const* argv[])
{
    //nike 是一只五岁的老虎, 它是生活在森林 forest 的猫科动物 feline 
    mammal tiger("nike",5, "forest", 38.5, "feline");

    animal& t = tiger;

    t.eat(); //调用 animal::eat
    t.viewAnimal(); //animal::viewAnimal

    return 0;
}
```

**指针调用**：你会发现子类的构造函数在 delete的时候根本没有被调用！如果类中进行的内存分配操作，那么这会导致严重的内存泄露！ **谨记：C++中基类采用virtual虚析构函数是为了防止内存泄漏。**

```cpp
#include <iostream>
#include <string>
#include"animal.h"

int main(int argc, char const* argv[])
{
    //nike 是一只五岁的老虎, 它是生活在森林 forest 的猫科动物 feline 
    mammal *tiger = new mammal("nike",5, "forest", 38.5, "feline");

    animal* t = tiger;

    t->eat(); //调用 animal::eat
    t->viewAnimal(); //animal::viewAnimal

    delete t;
    return 0;
}

/*
animal appear !!!
manmal appear !!!
animal eat food 
animal - name: nike, age: 5, enviroment: forest 
animal diappear !!!
*/
```

#### [3.3 父子类方法重名用virtual](#)
我们加上父类和子类的重名方法都加上virtual！

virtual，望文生义。虚拟的意思。它是用来修饰一个方法的，当编译器看到这个关键字，就懂了。哦，这个方法调用的时候，要看实际对象的真正类型，而非它的指针或者引用的类型。简而言之，virtual，实现动态绑定。

**在基类方法的声明中使用的virtual关键字，可以使得该方法在基类以及所有派生类中都是虚方法，也就是基类方法加上了virtual，子类重写的时候不加virtual，他也是virtual的！ 当然最好加上！增加可读性！**
```cpp
class animal
{
protected:
    std::string _name;
    std::string _environment_type;
    int _age;    
public:
    animal(std::string name, int age, std::string env);
    virtual std::string eat() const;
    virtual void viewAnimal() const;
    animal& operator=(const animal & other);
    virtual ~animal();

};


//哺乳动物
class mammal: public animal
{
private:
    float _suitable_temperature; //适宜温度
    std::string _sort;
public:
    mammal(std::string name, int age, std::string env, float temperature, std::string sort);
    virtual std::string eat() const;
    virtual void viewAnimal() const;
    virtual ~mammal();
};
```

再看调用结果
```cpp
int main(int argc, char const* argv[])
{
    //nike 是一只五岁的老虎, 它是生活在森林 forest 的猫科动物 feline 
    mammal *tiger = new mammal("nike",5, "forest", 38.5, "feline");

    animal* t = tiger;

    t->eat(); //调用 mammal::eat
    t->viewAnimal(); //mammal::viewAnimal

    delete t; //调用 mammal:~mammal() 然后调用 animal:~animal()
    return 0;
}

/*
animal appear !!!
manmal appear !!!
mammal eat good food 
mammal - name: nike, age: 5, enviroment: forest suitable temperature: 38.500000 ℃ ,sort: feline 
mammal diappear !!!
animal diappear !!!
*/
```

**黄金准则：将需要在派生类中重新定义的方法声明为虚方法！并且对析构函数一定要加上 virtual！ 这样程序将根据对象实际类型而不是引用或指针的类型来选择方法！为基类声明一个虚析构函数也是一个惯例！**


### [4. 动态联编与静态联编](#)
程序调用函数的时候，将使用哪个可执行代码块呢？将源代码中的函数调用解释位执行特定的函数代码块被称为函数名联编 (binding)!由于函数重载和模板的缘故，编译器必须查看函数参数以及函数名 才能确定使用哪个参数！
C/C++编译器可以在编译过程完成这种联编。在编译过程中进行联编被称为静态联编 (static binding)/早期联编(early binding)!  

编译程序在编译阶段并不能确切地指导将要调用的函数，只有在程序执行时才能确定将要调用的函数，为此要确切地指导将要调用的函数，要求联编工作在程序运行时进行，这种在 程序运行时进行的 联编工作被称为动态联编。C++中，动态联编是在虚函数的支持下实现的 。

动态联编(dynamic binding)需要虚函数的支持，这是因为虚函数的工作原理决定的，而正是因为使用了虚函数来实现动态联编，也让动态联编的效率略低于静态联编。

**总之，C++对非虚方法使用静态联编，对于虚方法使用动态联编！动态联编效率更低，因为要采取方法来跟踪基类指针或引用指向的对象类型！增加了额外开销！**

动态联编与通过指针和引用调用方法相关并且和继承相关，是因为派生类可以不仅过显示类型转换直接赋值给基类导致的！ 实现动态联编需要同时满足以下三个条件：

* 必须把动态联编的行为定义为类的虚函数。
* 类之间应满足子类型关系，通常表现为一个类从另一个类公有派生而来。
* 必须先使用基类指针指向子类型的对象，然后直接或者间接使用基类指针调用虚函数。

虚函数的实现依赖于虚函数表！每个对象有将具有一个指向虚函数表的指针，加上虚函数表，这会使得每个对象都增大，同时需要额外的去虚函数表查找地址的操作！虚函数表本身也会占用存储空间！

**如果你的类不会或者你不想要它产生派生类，那么推荐你使用 C++ 的 final 关键字！禁止继承该类！**

### [5. C++11 final override](#)
虚方法对实现多态类层次结构很重要，让基类引用或指针能够根据指向的对象类型调用相应的方法，但虚方法也带来了一些编程陷阱。

#### [5.1 override](#)
例如，假设基类声明了一个虚方法，而我们决定在派生类中提供不同的版本，这将 **覆盖旧版本**。即如果 特征标 不匹配，将 **隐藏** 而不是 **覆盖**旧版本：

```cpp
//父类
class Action
{
	int a;
public:
	Action(int i = 0) : a(i) {}
	int val() const { return a; }
	virtual void f(char ch) const { std::cout << val() << ch << '\\n'; }
};

//子类
class Bingo :public Action
{
public:
	Bingo(int i = 0) :Action(i) {}
    //将会隐藏父类的 f(char ch)
	virtual void f(char* ch)const { std::cout << val() << ch << "!\\n"; } 
};
```
由于 Bingo 定义的是 f(char *) 而不是 f(char)，这将对 Bingo 对象隐藏 f(char)。

在C++11中，可使用 **虚说明符override** 指出您要覆盖一个虚函数：将其放在参数列表后面。如果声明与基类方法不匹配，编译器将视为错误。因此，下面的Binfo::f()版本将生成一条编译错误消息：

```cpp
virtual void f(char * ch) const ovrride
{
    std::cout << val() << ch << "!\\n";
}
// Microsoft Visual C++ 2010 报错如下：
// method with overried specifier 'override' did not overried any base class methods
```

#### [5.2 final](#)
C++11的关键字final有两个用途：(1)、禁止函数被重写；(2)、禁止基类被继承。

用来修饰函数，使得该函数在子类中，不能被重写！
```cpp
class Super
{
  public:
    Supe();
    virtual void SomeMethod() final;
};
```

禁用继承: 用来修饰类，让该类不能被继承.
```cpp
class Super final
{
  //......
};

class XiaoMi {
public:
    XiaoMi(){}
};

class XiaoMi2 final : public XiaoMi  {
    XiaoMi2(){}
};

class XiaoMi3 : public XiaoMi2 { //不能把XiaoMi2作为基类

}; 
```



### [6. 抽象类](#)
如果你学过C#或者Java，知道定义抽象类需要使用 abstract关键字，但是C++不一样！ C++很特殊，他不使用关键字，而是使用纯虚函数
在C++中，含有纯虚函数的类(即：包含至少一个纯虚函数的类)称为抽象类，它不能生成对象。派生自抽象类的类必须实现纯虚函数或者它们必须也是抽象类。

纯虚函数是在基类中声明的虚函数，它在基类中没有定义，但要求任何派生类都要定义自己的实现方法。在基类中实现纯虚函数的方法是在函数原型后加"=0"
```cpp
virtual void funtion1()=0
```

**抽象类特点**
* 抽象类不能直接实例化，并且对抽象类使用 new 运算符会导致编译时错误；
* 抽象方法只能声明于抽象类中，且不包含任何实现，派生类必须覆盖它们；
* 重要的是抽象类可以包括抽象方法，这是普通类所不能的，但同时也能包括普通的方法。

```cpp
#include<string>
class object
{

private:
    /* data */
public:
    object(/* args */);
    virtual std::string to_string() const = 0; //纯虚函数
    virtual bool equals(const object& obj1, const object & obj2) const = 0;  //纯虚函数
    ~object();
};
```
抽象类是不完整的，它只能用作基类。在面向对象方法中，抽象类主要用来进行类型隐藏和充当全局变量的角色。

可以为抽象类定义纯虚函数，但是只能通过使用以下语法直接调用：
```cpp
//抽象类名称::函数名称（）
object::to_string()

```
#### [6.1 接口概念](#)
接口是一个概念。它在C++中用抽象类来实现，在C#和Java中用interface来实现。

**与抽象类的关系**
* 包含未实现的方法声明；
* 不能实例化；
* 派生类必须实现未实现的方法，抽象类是抽象方法，接口则是所有成员（不仅是方法包括其他成员）。

**与抽象类的区别**
* **抽象类当中可以存在非抽象的方法，可接口不能，且它里面的方法只是一个声明必须用public来修饰没有具体实现的方法。抽象类是对象的抽象，然而接口是一种行为规范。**

抽象类中的成员变量可以被不同的修饰符来修饰，可接口中的成员变量默认的都是静态常量（static final）。

接口基本上不具备继承的任何具体特点，它仅仅承诺了能够调用的方法; 而类可以实现无限个接口，但仅能从一个抽象（或任何其他类型）类继承，从抽象类派生的类仍可实现接口，从而得出接口是用来解决多重继承问题的。

C++ 定义一个接口
```cpp
#define Interface class

//interface string 
//表示对象有公开的 toString 方法
Interface IString
{
private:
    /* data */
public:
    virtual std::string toString() const = 0;
};
```

继承并且实现它

```cpp
class animal: public IString
{
protected:
    std::string _name;
    std::string _environment_type;
    int _age;    
public:
    animal(std::string name, int age, std::string env);
    virtual std::string eat() const;
    virtual void viewAnimal() const;
    virtual std::string toString() const; //实现接口方法
    animal& operator=(const animal & other);
    virtual ~animal();

};


std::string animal::toString() const{
    std::string result = "animal - name:" + _name + "age:" +
         std::to_string(_age) + ", enviroment: " + _environment_type ;
    return result;
}
/*
 ... 
*/
```

### [7. has-a关系](#)
一个类作为其他类的组成部分！

#### [7.1 默认构造函数问题](#)
当你将一个类的对象作为另一个类的成员的时候，要注意下这个作为成员的类是否有默认构造函数，如果没有的话，你可能需要成员列表初始化的方式来给他初始化例如

animal类，没有默认构造函数！只能用成员列表初始化的方式来赋值初始化！

```cpp
class resident
{
private:
    animal pet;
    std::string resident_name;//姓名
    unsigned short age;//年龄
    std::valarray<double> scores; //成绩
public:
    resident(std::string name , unsigned short _age ,animal yourPet, double * _scores, int len);
    resident(const resident& other);
    resident(const resident && other); //移动构造函数
    resident& operator = (const resident &other); //赋值运算符
    ~resident();
};

//有参构造函数
resident::resident(std::string name , unsigned short _age,animal yourPet, double * _scores, int len)
:pet(yourPet), scores(_scores, len)
{
    printf("call 有参构造函数\n ");
    resident_name = name;
    age = _age;
}

/* ... */
```
如果animal 有无参构造函数呢？ 那么不使用列表初始化也可以 ，不过会先调用对象的无参数构造函数！ 如果在当前构造函数中对pet进行赋值。那么为了初始化pet！ 总共调用了两次函数，一次默认构造函数，一次赋值运算符重载

```cpp
resident::resident(std::string name , unsigned short _age,animal yourPet, double * _scores, int len)
:scores(_scores, len)
{
    printf("call 有参构造函数\n ");
    pet = yourPet; // 调用赋值运算符
    resident_name = name;
    age = _age;
}
```
也就是说


#### [7.2 小例子](#)
居民，有一个宠物，还有个人基本信息

```cpp
#ifndef __RESIDENT_H__
#define __RESIDENT_H__
#include"animal.h"
#include<valarray>

class resident final
{
private:
    animal pet;
    std::string resident_name;//姓名
    unsigned short age;//年龄
    std::valarray<double> scores; //成绩
public:
    resident(std::string name , unsigned short _age ,animal yourPet, double * _scores, int len);
    resident(const resident& other);
    resident(const resident && other); //移动构造函数
    resident& operator = (const resident &other); //赋值运算符
    std::string toString() const;
    animal getPet() const;
    std::valarray<double> getScores() const;
    double &operator[](int index);
    double operator[](int index) const;
    ~resident();
};


double& resident::operator[](int index){
    if (index >= 0 && index < scores.size())
    {
        return scores[index];
    }else{
        throw "The index of the access array is out of bounds";
    }
}

double resident::operator[](int index) const{
    if (index >= 0 && index < scores.size())
    {
        return scores[index];
    }else{
        throw "The index of the access array is out of bounds";
    }
}

animal resident::getPet() const{
    return pet;
}

std::valarray<double> resident::getScores() const{
    return scores;
}

std::string resident::toString() const{
    std::string result = resident_name + " " + std::to_string(age);
    return result;
}

//有参构造函数
resident::resident(std::string name , unsigned short _age,animal yourPet, double * _scores, int len)
:pet(yourPet), scores(_scores, len)
{
    printf("resident call 有参构造函数\n");
    resident_name = name;
    age = _age;
}

//赋值构造函数
resident::resident(const resident& other)
:pet(other.pet),
scores(std::begin(other.scores), other.scores.size()),
resident_name(other.resident_name),
age(other.age)
{
    printf("resident call 赋值构造函数\n");
}

//赋值运算符
resident& resident::operator =(const resident &other){

    printf("resident call 赋值运算符\n");

    if(&other == this) return (*this);
    resident_name = other.resident_name;
    scores = other.scores;
    age = other.age;
    pet = other.pet;
    return (*this);
} 

resident::~resident()
{
    printf("resident call 析构函数\n");
}
#endif
```

使用
```cpp
nt main(int argc, char const* argv[])
{
    double grades[5] = {95.5,87.5,96.5,89.5,90.12};

    resident remix("remix", 24, animal("tidi", 2,"land") ,grades, 5);

    remix[2] = 100.0;

    std::cout << remix.toString() << " pet [" << remix.getPet().toString() << "]" <<std::endl;

    int index = 0;
    for (auto &&val : remix.getScores())
    {
        std::cout<< "the " << ++index << " discipline \'s grade: " << val << std::endl;
    }

    std::cout << "total score:" << remix.getScores().sum() << std::endl;
    std::cout << "[2] score:" << remix[2] << std::endl;//[2] [2] score:100
    //total score:459.12
    return 0;
}

/*animal appear !!!
resident call 有参构造函数
animal diappear !!!
remix 24 pet [animal - name:tidiage:2, enviroment: land]
animal diappear !!!
the 1 discipline 's grade: 95.5
the 2 discipline 's grade: 87.5
the 3 discipline 's grade: 100
the 4 discipline 's grade: 89.5
the 5 discipline 's grade: 90.12
total score:462.62
[2] score:100
resident call 析构函数
animal diappear !!!*/
```

#### [7.3 私有继承](#)
私有继承：使用私有继承，基类的公有成员和保护成员都将成为派生类的私有成员，只可以在派生类的成员函数中使用 派生类的新增成员可以访问基类的公有成员和保护成员，但是访问不了基类的私有成员。派生类的对象不能访问派生类继承基类的公有成员，保护成员和私有成员。

私有继承也可以用来实现 has-a关系！ 类将继承实现，基类的公共方法将成为派生类的私有方法！

将三个未命名的对象加入到 类中！ 例如我们利用私有继承，完成组合关系实现，如下所示！
要使用类名和作用域解析符来调用基类方法！

需要获得私用继承的对象，就需要使用强制类型转换
```cpp
#ifndef __RESIDENT_H__
#define __RESIDENT_H__
#include"animal.h"
#include<valarray>

class resident :private std::string, private std::valarray<double>, private animal
{
private:
    unsigned short age;//年龄
public:
    resident(std::string name , unsigned short _age ,animal yourPet, double * _scores, int len);
    resident(const resident& other);
    resident(const resident && other); //移动构造函数
    resident& operator = (const resident &other); //赋值运算符
    std::string toString() const;
    animal getPet() const;
    std::valarray<double> getScores() const;
    double &operator[](int index);
    double operator[](int index) const;
    ~resident();
};

double& resident::operator[](int index){
    if (index >= 0 && index < std::valarray<double>::size())
    {
        return std::valarray<double>::operator[](index);
    }else{
        throw "The index of the access array is out of bounds";
    }
}

double resident::operator[](int index) const{
    if (index >= 0 && index < std::valarray<double>::size())
    {
        return std::valarray<double>::operator[](index);
    }else{
        throw "The index of the access array is out of bounds";
    }
}

animal resident::getPet() const{
    //直接类型转换
    return animal(*this);
}

std::valarray<double> resident::getScores() const{
    return std::valarray<double>(*this);
}

std::string resident::toString() const{
    std::string result = (std::string)*this + " " + std::to_string(age);
    return result;
}

//有参构造函数
resident::resident(std::string name , unsigned short _age,animal yourPet, double * _scores, int len)
:animal(yourPet), std::valarray<double>(_scores, len),std::string(name)
{
    printf("resident call 有参构造函数\n");
    age = _age;
}

//赋值构造函数
resident::resident(const resident& other)
:animal(other),
std::valarray<double>(other),
std::string(other),
age(other.age)
{
    printf("resident call 赋值构造函数\n");
}

//赋值运算符
resident& resident::operator =(const resident &other){
    printf("resident call 赋值运算符\n");
    if(&other == this) return (*this);
    std::string::operator=(other);
    animal::operator=(other);
    std::valarray<double>(std::begin(other), std::valarray<double>::size());
    age = other.age;
    return (*this);
} 

resident::~resident()
{
    printf("resident call 析构函数\n");
}
#endif
```

resident 对象内存模型 里面通过私有继承，完成了组合关系，三个对象未命名而隐式存在！
```
[-----------------]
|                 |
|  string 对象     |
|                 |
|  animal 对象     |
|                 | 
| valarray<double>|
|                 |
|  age            | 
|  .....          |
|-----------------|
```

**实现方式比较抽象，最好别用私有继承来实现组合关系！ 继承会引起很多问题，尤其是多继承！而且包含可以包括多个同类的子对象例如三个string对象，但是私有继承却不可以！他只能有一个这样的对象！继承可以访问到包含对象的保护方法，重写虚函数，但是包含组合不可以！**

#### [7.4 通过using重写定义访问权限](#)
在保护继承和私有继承，对于继承过来的方法只能在定义中使用，派生类对象无法访问！ 那么你其实也可以使用一个方法来让外部可以访问，如下所示
```cpp
double resident::sum() const{
    return std::valarray<double>::sum();
}
```
这样写太麻烦了！可以试试 using

```cpp
class resident :private std::string, private std::valarray<double>, private animal
{
private:
    unsigned short age;//年龄
public:
    /*省略...*/
    using std::valarray<double>::sum;
    using std::valarray<double>::min;
    using std::valarray<double>::max;
    ~resident();
};

//使用

resident remix("remix", 24, animal("tidi", 2,"land") ,grades, 5);

std::cout << "total score:" << remix.sum() << std::endl;
```

### [8. 多重继承](#)
如果你没什么大病，千万别用什么多重继承！ 多重继承有一些大毛病！ 例如 A继承了 B,C,D ! 但是B,C继承自 E。 E继承自F，D也继承与F

```
变态的继承关系！

              F
            /   \    
          E      |
         / \     |
        B   C    D
          \ |  / 
            A
```

因为这种变态关系的存在，导致了一系列问题，并且伴随着解决这种问题的各种手段！

**多继承必然存在的 菱形问题** 使用多继承一定要避免菱形问题。保持继承链条上面一定没有共同的祖先！
```
          worker
         /     \
    singer      waiter
         \     /    
       singerWaiter
```

#### [8.1 虚基类](#)
**虚基类** 就是用来解决 singerWaiter 有两个 相同基类产生的二义性问题！ 由于singer,和 waiter 有共同的父类，导致 singerWaiter 对象里面具有两个 worker 基类对象！ 导致了二义性问题！

我们希望继承间接共同基类时只保留一份成员, 所以虚基类就诞生了. 当基类通过多条派生路径被一个派生类继承时, 该派生类只继承该基类一次.

虚基类 (virtual base class) 是用关键字 virtual 声明继承的父类.

```cpp
class singer:virtual public worker{
    /* ... */
}

class waiter:virtual public worker{
    /* ... */
}
```
这样继承的 singer 和 worker 共享一个 worker子对象！

* 不提倡在程序中使用多重继承
* 只有在比较简单和不易出现二义性的情况或实在必要时才使用多重继承
* 能用单一继承解决的问题就不要使用多重继承 **其他语言都是单继承，说明不用多继承，都能解决所有问题！**


#### [8.2 重名方法问题](#)
如果singer和waiter 都有一个 public profession() 方法！ 而子类没有！ 那么子类如何决定调用哪个方法，避免二义性呢

```cpp
singerWaiter tom("tom", 2020, 5, "singer - waiter");

tom.singer::profession();//调用 singer的方法
tom.waiter::profession();//调用 waiter的方法
```

当然最好的方式是在 singerwaiter里面 定义一个 show方法！避免二义性！

#### [8.3 构造函数问题](#)
调用哪个父类的构造函数去调用最上面的构造函数 singer 构造函数也可以调用 worker构造函数，waiter构造函数也可以调用worker构造函数，那么怎么办呢？

使用虚基类继承，手动调用共同基类的构造函数
```cpp
singerWaiter(/*参数*/)
:worker(/*参数*/), singer(/*参数*/),waiter(/*参数*/)
{

}
```

#### [8.4 使用多继承的时机和要求](#)

1. 保证你的类设计层次没有共同祖先的情况
2. 利用多继承达到 接口功能！
