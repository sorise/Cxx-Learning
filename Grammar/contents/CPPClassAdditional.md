### [类的其他内容](#)
`包括友元类，友元类方法，嵌套类！`

-----
- [x] [1. 友元类](#1-友元类)
- [x] [2. 嵌套类](#2-嵌套类)

-----

### [1. 友元](#)
在一个C++程序里，我们在构造一个类的时候，有Public属性，也有Private属性，正常情况下，在主程序以及函数中，我们是只能调用Public（公开属性）里面的参数和函数。
但是在某些情况下，我们既想保留Private属性，又想修改或者调用其中的参数，我们该怎么办呢？这个时候需要引出我们的友元这一概念。

友元：
* **友元函数**
* **友元类**

友元类虽然好用，但是破坏了面向对象的封装特性。说明封装得不好，才需要这种东西，友元的功能完全可以通过设计模式中的，观察者模式，消息订阅与发布，事件来解决！《C++ Primer plus》 里面举例的
电视机和遥控器的例子，应该通过事件型设计模式，或者消息订阅发布机制完成，根本就不需要友元这个东西！

实践证明，就算Java，C#没有友元这个东西，也完全可以支持大型软件系统的开发！
#### [1.1 友元类](#)
`友元类的所有成员函数都是另一个类的友元函数，都可以访问另一个类中的隐藏信息（包括私有成员和保护成员）。当希望一个类可以存取另一个类的私有成员时，可以将该类声明为另一类的友元类, 还可以只将特定的成员函数指定为
另一个类的友元！`

`定义友元类的语句格式如下： ` `B是A的友元！`

```cpp
class A
{
    /* … */
public:
    friend class B;
    /* … */
};
```

`经过以上说明后，类B的所有成员函数都是类A的友元函数，能存取类A的私有成员和保护成员。`

`使用友元类时注意：`

* 友元关系不能被继承。
* `友元关系是单向的，不具有交换性。若类B是类A的友元，类A不一定是类B的友元，要看在类中是否有相应的声明。`
* `友元关系不具有传递性。若类B是类A的友元，类C是B的友元，类C不一定是类A的友元，同样要看类中是否有相应的申明`


#### [1.2 部分友元类](#)
`让类中的成员函数称为其他类的友元函数` `通过如下方法就可以了！`

```cpp
class A{
    friend void B::function_i(A & a, int b);
    friend A B::function_ii(A & a, int b);
    friend bool B::function_iii(A & a, int b);
    friend A& B::function_4(A & a);
}
```
`很脑残哈，B中的方法已修改，如果方法是A类中的友元，还要找到A类的文件进行同样的修改！` 


### [2. 嵌套类](#) 
在一个类中定义的类称为嵌套类，定义嵌套类的类称为外围类。定义嵌套类的目的在于隐藏类名，减少全局的标识符，从而限制用户能否使用该类建立对象。 这样可以提高类的抽象能力，并且强调了两个类（外围类和嵌套类）之间的主从关系。

如果一个嵌套类在另一个类的私有部分声明的，嵌套类对于这个类是可见的，对于外界是不可见的。如果一个嵌套类在另一个类的公有部分声明的，则允许在这个类和外界使用这个嵌套类。

* `从作用域的角度上看，嵌套类被隐藏在外围类之中，该类名只能在外围类中使用。如果在外围类的作用域内使用该类名时，需要加名字限定。`
* `从访问权限的角度来看，嵌套类名与它的外围类的对象成员名具有相同的访问权限规则。不能访问嵌套类的对象中的私有成员函数，也不能对外围类的私有部分中的嵌套类建立对象。`
* `嵌套类中的成员函数可以在它的类体外定义。`
* `嵌套类中说明的成员不是外围类中对象的成员，反之亦然。嵌套类的成员函数对外围类的成员没有访问权，反之亦然。因此，在分析嵌套类与外围类的成员访问关系时，往往把嵌套类看作非嵌套类来处理。`
* `在嵌套类中说明的友元对外围类的成员没有访问权。`
* `如果嵌套类比较复杂，可以只在外围类中对嵌套类进行说明，关于嵌套类的详细的内容可在外围类体外的文件域中进行定义。``

#### [2.1 private 嵌套类](#)
`所有的方法，构造函数，或者其他都只能写在类定义中，不能分开，因为它是类私有的，外界无法访问！`

```cpp
#ifndef ___CAR_H___
#define ___CAR_H___ 1
#include<string>
enum class EngineType{NaturallyAspirated, Turbocharged, Supercharged , DoubleSupercharged};

class car
{
private:
    class engine final
    {
    //进气系统的工作方式可分为自然吸气、涡轮增压、机械增压和双增压
    private:
        EngineType _type;
        unsigned int strepitus; //噪音 分贝
        std::string brand;
        double power;
    public:
        EngineType getType(){ return _type; };
        engine(EngineType type, unsigned int zaoyin, std::string brd, double _power)
        :_type(type), strepitus(zaoyin), brand(brd), power(_power){};
        ~engine(){};
    };
    engine kernel;
public:
    car();
    ~car();
};

car::car():kernel(EngineType::Supercharged, 85, "Chevrolet", 2.5){}
car::~car(){}
#endif
```

`使用方法！`
```cpp
int main(int argc, char const* argv[])
{   
    car chevrolet;
    //car::engine bydkernel(EngineType::Turbocharged, 75, "BYD", 3.5);
    
    return 0;
}
```

#### [2.2 public 嵌套类](#)
`可以实现嵌套类的声明定义分开， 外界可以通过外围类使用作用域限定符访问`

```cpp
#ifndef ___CAR_H___
#define ___CAR_H___ 1
#include<string>
enum class EngineType{NaturallyAspirated, Turbocharged, Supercharged , DoubleSupercharged};

class car
{

public:
    class engine final
    {
    //进气系统的工作方式可分为自然吸气、涡轮增压、机械增压和双增压
    private:
        EngineType _type;
        unsigned int strepitus; //噪音 分贝
        std::string brand;
        double power;
    public:
        EngineType getType();
        engine(EngineType type, unsigned int zaoyin, std::string brd, double _power);
        ~engine();
    };
    car();
    ~car();
private:
    engine kernel;
};

EngineType car::engine::getType(){
    return _type;
}


car::engine::engine(EngineType type, unsigned int zaoyin, std::string brd, double _power)
:_type(type), strepitus(zaoyin), brand(brd), power(_power){
    printf("engine 构造调用!\n");
};

car::engine::~engine(){
    printf("engine 析构调用!\n");
}

car::car():kernel(EngineType::Supercharged, 85, "Chevrolet", 2.5)
{
    printf("car 构造调用!\n");
}

car::~car()
{
    printf("car 析构调用!\n");
}
#endif
```

`使用方法！`
```cpp
int main(int argc, char const* argv[])
{   
    car chevrolet;
    car::engine bydkernel(EngineType::Turbocharged, 75, "BYD", 3.5);
    return 0;
}
/*
engine 构造调用!
car 构造调用!
engine 构造调用!
engine 析构调用!
car 析构调用!
engine 析构调用!
*/

```


-----
`时间`: `[]` 