### [C++ OOP 类的深入](#)

`面向对象编程 OOP是一种特殊的，设计程序的概念性方法，C++通过一些特性改进了C语言！` `抽象` `封装/数据隐藏` `多态` `继承` `代码的可重用性`

----

- [1. 运算符重载](#1-运算符重载)
- [2. 友元函数](#2-友元函数)
- [3. 类的自动类型转换和强制类型转换](#3-类的自动类型转换和强制类型转换)
- [4. 静态数据成员](#4-静态数据成员)
- [5. 动态内存分配](#5-动态内存分配)
- [6. 各种构造函数](#6-各种构造函数)
- [7. 返回对象的问题](#7-返回对象的问题)
- [8. 对象的消亡](#8-对象的消亡)
- [9. 成员初始化列表](#9-成员初始化列表)
- [10. 类内成员初始化](#10-类内成员初始化)
- [11. C++ 11 =delete禁止调用和生产](#11-c-11-=delete禁止调用和生产)
- [12. C++ 11 =default](#12-c-11-default)
- [13. 可变数据成员](#13-可变数据成员)

---
### [1. 运算符重载](#)
运算符重载的实质就是函数重载或函数多态。运算符重载是一种形式的C++多态。目的在于让人能够用同名的函数来完成不同的基本操作。
要重载运算符，需要使用被称为运算符函数的特殊函数形式，运算符函数形式：`operator p（argument-list）` `operator` 后面的 `p` 为要重载的运算符符号。 

**将运算符理解位函数调用**: `c = a + b` `等价于` ` c = a.operator+(b) `

不能将运算符声明为 static :  **static Number operator+(const Number& n1);**

```
<返回类型说明符> operator <运算符符号>(<参数表>)
{
     <函数体>
}
```

`mytime.h`
```cpp
#ifndef __MY_TIME_H__
#define __MY_TIME_H__ __MY_TIME_H__
#include<string>
class MyTime
{
private:
    unsigned int hour;
	unsigned int minutes;
public:
    
    MyTime(/* args */);
    MyTime(const MyTime & mt);
    MyTime(const MyTime && mt);
    MyTime(const int& tm_hour,const int& tm_minutes);
    std::string ToString() const;
    MyTime operator + (const MyTime & t1) const;
    MyTime operator = (const MyTime & t2);
    ~MyTime();

};
#endif
```
运算符重载，其实很简单，就是需要使用关键operator

mytime.cpp
```cpp
#include"mytime.h"

MyTime::MyTime(){
    printf("调用无参构造函数\n");
    hour = 0;
    minutes = 0;
}

MyTime::MyTime(const MyTime & mt){
    printf("调用复制构造函数\n");
    hour = mt.hour;
    minutes = mt.minutes;
}

MyTime::MyTime(const MyTime && mt){
    printf("调用移动构造函数\n");
    hour = mt.hour;
    minutes = mt.minutes;
}

MyTime::MyTime(const int& tm_hour,const int& tm_minutes){
    printf("调用有参构造函数\n");
    hour = tm_hour + tm_minutes / 60;
    minutes = tm_minutes % 60;
}

MyTime::~MyTime(){
    printf("%s 对象消亡了 \n", ToString().c_str());
}

std::string MyTime::ToString() const{
    return std::to_string(hour) + ":" + std::to_string(minutes);
}

MyTime MyTime::operator+(const MyTime & t1) const{
    printf("调用重载 + 运算符\n");
    MyTime result;
    result.hour = hour + t1.hour + (minutes + t1.minutes) / 60;
    result.minutes = (minutes + t1.minutes) % 60;
    return result;
}

MyTime c::operator = (const MyTime & t2){
    if(&t2 == this) return (*this);
    printf("调用重载赋值运算符\n");
    hour = t2.hour;
    minutes = t2.minutes;
    return (*this);
}
```
你可以如此调用 `MyTime t3 = t1.operator+(t2);`  也可以 `MyTime t3 = t1 + t2` 后者只是前者的语法糖！
```cpp
#include <iostream>
#include <string.h>
#include <math.h>
#include"mytime.h"

int main(int argc, char const* argv[])
{
    MyTime now = MyTime(12, 90);//13:30
    MyTime t1(23, 12);

    MyTime total = now + t1;
    
    std::cout << now.ToString() << std::endl;
    return 0;
}

/*
调用有参构造函数
调用有参构造函数
调用重载 + 运算符
调用无参构造函数
13:30
36:42 对象消亡了 
23:12 对象消亡了 
13:30 对象消亡了 
*/
```

多数的C++的运算符都可以用这样的方式重载。重载的运算符不必是成员函数，但必须至少有一个操作数是用户定义的类型！

1. 为防止用户为标准类型重载运算符，重载后的运算符必须至少有一个是用户自定义类型的数据。、
2. 不能违反运算符原有的运算规则, 两个操作数的运算符 变成一个 操作数的运算符。
3. 不能重载不存在的运算符，即不能创建新的运算符
4. 以下运算符不可重载：
    * **sizeof** ： sizeof运算符
    * **.** ：成员运算符
    * `.*` ：成员指针运算符
    * **::** : 域解析运算符
    * **?:**： 条件运算符
    * typid ： RTTI运算符
    * const_cast 、dynamic_cast、reinterpret_cast、static_cast  ：强制类型转换

以下运算符用于都可以实现自定义运算符重载

<table align="center"cellspacing="0">
<tbody>
<tr>
<td><p style="text-indent:50px;"><span style="color:#000000;">+</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">-</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">*</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">/</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">%</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">^</span></p></td>
</tr>
<tr>
<td><p style="text-indent:50px;"><span style="color:#000000;">&amp;</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">|</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">~=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">!</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">&lt;</span></p></td>
</tr>
<tr><td><p style="text-indent:50px;"><span style="color:#000000;">&gt;</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">+=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">-+</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">*=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">/=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">%=</span></p></td>
</tr>
<tr><td><p style="text-indent:50px;"><span style="color:#000000;">^=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">&amp;=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">|=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">&lt;&lt;</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">&gt;&gt;</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">&gt;&gt;=</span></p></td>
</tr>
<tr>
<td><p style="text-indent:50px;"><span style="color:#000000;">&lt;&lt;=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">==</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">!=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">&lt;=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">&gt;=</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">&amp;&amp;</span></p></td>
</tr>
<tr><td><p style="text-indent:50px;"><span style="color:#000000;">||</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">++</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">--</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">,</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">-&gt;*</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">-&gt;</span></p></td>
</tr><tr><td><p style="text-indent:50px;"><span style="color:#000000;">()</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">[]</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">new</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">delete</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">new[]</span></p></td>
<td><p style="text-indent:50px;"><span style="color:#000000;">delete[]</span></p></td>
</tr>
</tbody>
</table>

#### [1.1 重载运算符说明](#)
大多数运算符都可以通过 成员或非成员函数 进行重载

但下面的运算符只能通过成员函数进行重载。 `=:` 赋值运算符 `()` 函数调用运算符 `[]` 下标运算符 `->` 通过指针访问类成员运算符

使用非成员函数实现运算符重载,这需要使用到友元函数！ 必须使用友元函数，不然非成员函数无法访问类中私有成员！

```cpp
#ifndef __NUM_h__JX__
#define __NUM_h__JX__ __NUM_h__JX__
class Number
{

friend Number operator +(const Number& n1, const Number& n2);
private:
    int number;
public:
    Number(/* args */);
    Number(int n);
    Number(const Number& o);
    Number(const Number&& o);
    int getNumber() const;
    //左侧的操作数必须是 Number 类型 有操作数 是 int类型
	Number operator=(const int& n);
    Number operator=(const Number& n);
    ~Number();
};

Number Number::operator=(const int& n){
    number = n;
    return (*this);
}

Number Number::operator=(const Number& n){
    if(&n == this) return (*this);
    number = n.number;
    return (*this);
}

Number::Number(/* args */)
{
    number = 0;
}

Number::Number(int n){
    number = n;
}

Number::Number(const Number& o){
    number = o.number;
}

Number::Number(const Number&& o){
    number = o.number;
}

Number operator+(const Number& n1, const Number& n2){
    Number n;
    n.number = n1.number + n2.number;
    return n;
}

int Number::getNumber() const{
    return number;
}

Number::~Number()
{
}
#endif
```
`使用`
```cpp
int main(int argc, char const* argv[])
{
    Number n1 = Number(35);
    Number n2 = 45;

    Number n3; 
    n3 = n1;

    Number t = n3 + n2;

    std::cout << t.getNumber() << std::endl; //80
    return 0;
}
```
#### [1.2 成员函数重载运算符的问题](#)
使用成员函数进行运算符重载会有一个小问题 以如下为例 它支持 `mytime * 1.5` 但是不支持 `1.5 * mytime` `他限制了操作数类型顺序，左侧必须是 MyTime 类型 右侧必须是 double`
```cpp
MyTime operator * (const double & t1) const;
```
但是如果你使用友元函数就不会出现这种情况！

当然也有 **解决方法**，为了可以 “左右互搏” 你可以这样做，添加一个外部函数！
```cpp
MyTime operator *(const double &t1, const MyTime& mytime){
	return mytime * t1;
}
```
你会发现，为了重载运算符，竟然要写两个函数！ 为了让你一个函数重载运算符，并且可以调换操作数顺序，你就需要使用外部函数，但是如果你的外部函数比较复杂需要访问类中成员，那么你就需要友元函数！

#### [1.3 左值和右值](#)
如果我们重载数组运算符，请考虑下面这种情况，两个重载运算符函数有什么区别， **带const的，是为const对象调用的，可以和非const的成员构成重载。**
```cpp
//类名： Student
double &operator[](int index); // [0]
double operator[](int index) const; // [1]
```
`[0]` `可以作左值`
`[1]` `可以作右值`
```cpp
double grades[5] = {95.5,87.5,96.5,89.5,90.12};

Student stu("remix", 24, grades, 5);

stu[2] = 99.5; //可以直接修改成员值 会调用 [0]

double yuwen = stu[3]; //调用 [1]
```
**返回引用和返回值是有所不同的，对于不同的运算符，要利用左右值多加思考辨别！**


### [2. 友元函数](#)
C++控制对类对象私有部分的访问，通常类成员函数提供唯一的访问途径。但是C++提供了另一种形式的访问权限: 友元：
友元有三种：
1. 友元函数： 不是类的成员函数，但是它和成员函数具有相同的访问权限，**声明在类中需要加上关键字！ friend！定义的时候不用加friend 也不需要 类名:: 限定符！**
2. 友元类
3. 友元成员函数

#### [2.1 定义友元函数](#)
通过让函数成为类的友元，可以赋予该函数与类的成员函数相同的访问权限。需要友元的原因： 不同类型进行运算符重载， 例如：`对象 * 实数` `实数 * 对象` 
友元函数的声明就是在函数原型前面加上friend。

```cpp
#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__ __VECTOR2D_H__
#include <string>

class Vector2D
{
friend Vector2D operator *(const double& , const Vector2D& );
friend Vector2D operator *(const Vector2D& , const double&);
private:
	double _x;
	double _y;
public:
	Vector2D();
	Vector2D(const double& );
	Vector2D(const double& , const double& );
    Vector2D(const double&& ,const double&& );
    Vector2D(const Vector2D& );
    Vector2D(const Vector2D&& );
    Vector2D operator =(const Vector2D &);
	std::string ToString() const;
};

Vector2D::Vector2D(){
    _x = 0;
    _y = 0;
}

Vector2D::Vector2D(const double& p){
    _x = p;
    _y = p;
}


Vector2D::Vector2D(const double& px,const double& py){
    _x = px;
    _y = py;
}

Vector2D::Vector2D(const double&& px,const double&& py){
    _x = px;
    _y = py;
}

Vector2D::Vector2D(const Vector2D& v1){
    _x = v1._x;
    _y = v1._y;
}

Vector2D::Vector2D(const Vector2D&& v1){
    _x = v1._x;
    _y = v1._y;
}

Vector2D Vector2D::operator=(const Vector2D & v){
    _x = v._x;
    _y = v._y;
    return (*this);
}

std::string Vector2D::ToString() const{
    return "x:" + std::to_string(_x) +" y:"+ std::to_string(_y);
}

Vector2D operator *(const double& mult, const Vector2D& v){
    Vector2D result = Vector2D();
    result._x = v._x * mult;
    result._y = v._y * mult;
    return result;
}

Vector2D operator *(const Vector2D& v, const double& mult){
    return mult * v;
}

#endif
```
通过两个友元函数！ 第一个操作数是 Vector2D 类型， 第二个操作数是 double。 支持 `Vector2D * double。` 反过来也行!
```cpp
#include <iostream>
#include <string.h>
#include"Vector2D.h"

int main(int argc, char const* argv[])
{
    Vector2D v(3.0,3.0);
    Vector2D v1 = v * 5.0;
    Vector2D v2 = 4.0 * v;
    std::cout << v1.ToString() << std::endl; //x:15.000000 y:15.000000
    std::cout << v2.ToString() << std::endl; //x:12.000000 y:12.000000
    return 0;
}

```
你会反思，其实外部函数只需要调用public的成员函数，其实友元函数重载无意义！

```cpp
class Vector2D
{
private:
	double _x;
	double _y;
public:
	Vector2D();
	/*
	 .... 省略
	*/
    Vector2D operator*(const double&) const;
	std::string ToString() const;
};

/*
省略
*/

Vector2D Vector2D::operator *(const double& mult) const{
    Vector2D result = Vector2D();
    result._x = _x * mult;
    result._y = _y * mult;
    return result;
}

//加一个外部函数
Vector2D operator *(const double& mult, const Vector2D& v){
    return  v * mult;
}
```

#### [2.2 重载 << 运算符](#)
对 `<< 运算符重载`，可以方便我们使用 cout进行输出！

```cpp
friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
	os << "x: "<< std::to_string(v._x) << " y:" + std::to_string(v._y);
	return os;
}

Vector2D v1 = Vector2D(5.1, 3.1);
Vector2D v2 = v1 * 3.0;

cout << v2 <<endl;
```

#### [2.3 友元函数总结](#)
可用也可以不用！ 了解一下就行！ 反正Java C#没有友元函数一样过日子！

1. 友元函数不是类的成员函数，但是仍可以访问该类的私有成员。友元函数可以是一个外部函数，也可以是另一个类的成员函数。
2. 当类A中的某个函数成员需要访问类B中的私有成员的时候，首先需要在类A的头文件中进行对类B的超前使用说明，这是必不可少的。其次需要在类B中将A的函数声明为友元函数，C++ 搞这么麻烦干嘛！
3. 友元函数的声明就是在函数原型前面加上friend。
4. 可以将一个类定义为另一个类的友元，但是这样的话，友元类中的所有函数成员都能访问另一个的类中的私有数据成员，这样是不安全的。最好的方法就是哪个需要访问，就设置为友元函数即可。

**友元函数破坏了类的封装,一般不建议使用! 也有人说 友元保护了封装性。**

### [3. 类的自动类型转换和强制类型转换](#)
类型转换就很常见了，其他类型转换为类类型可以使用构造函数实现，类类型转换为其他类型可以使用类的成员函数实现，只不过函数名是类型名称！

`重新定义一个 Vector2D！`

```cpp
class Vector2D
{
friend Vector2D operator *(double , const Vector2D& );
private:
	double _x;
	double _y;
public:
	Vector2D(double&& , double&& );
	Vector2D(double& , double& );
	Vector2D();
	Vector2D(double ); //类型转换也可以使用
	std::string ToString() const;
	Vector2D operator *(double );
	
};
```

#### [3.1 其他类型转换为类类型](#)
在C++中，允许将接受一个参数的构造函数作为基础，实现类型间的转换。如Cat类的Cat(string name)构造函数：

```cpp
Cat m = "HelloKitty"; // 以Cat(string name)为基础的隐式转换
cout << m << endl; // 输出m
```
以上面 Vector2D 类为例， 程序使用构造函数 Vector2D(double );  来创建一个临时的 Vector2D对象，并以5.0 为初始化值，然后采用逐成员赋值方式将临时对象的呢诶荣赋值给 v1，这一过程为隐式转换！
```cpp
Vector2D( double );

Vector2D v1 = 5.0; //成立
//

```
这种方式必须只接受一个参数，或者有多个参数同时除第一个都有默认值。 如果为了防止这种隐式转换带来问题，可以给这个构造函数的前面的加上 explicit 关键字。 如此就只能使用强制类型转换了!
```cpp
explicit Vector2D( double );

Vector2D v1 =(Vector2D)5;
```

#### [3.2 类类型转换为其他类型](#)
前面的转换使用的是构造函数。如果想要做到**类类型到其他类型的转换**，就必须使用c++运算符函数，转换函数。 
语法： 

* 必须是类方法
* 不能指定返回类型
* 不能有参数 ,  一般都会加上const修饰符, 因为类型转换运算符通常不应该改变带转换对象的内容。

```cpp
operator typename() const{
}
```

```cpp
operator std::string();

Vector2D::operator std::string() {
	return ToString();
}

int ki = v1;
cout << ki << endl;
```

同样为了防止隐式转换。可以使用explicit关键字。 那么就必须使用强制类型转换了

```cpp
explicit operator typename(){

}
```

```cpp
explicit operator int();
Vector2D::operator int() {
	return _x;
}

cout << (int)v1 << endl;
```

#### [3.3 二义性问题](#)
不论是第一种转换，还是第二种转换，都有可能造成二义性（ambiguous）。比如：int到long、double的转换都有可能造成精度的丢失。编译器不会承担选择哪个构造函数的责任，于是编译无法通过。

```cpp
class A{
    long x = 0;
    double y = 0.0;
public:
    operator long(){
        return x;
    }
    operator double(){
        return y;
    }
};

int main() {
    A a;
    int x = a; // erro 转换有二义性
    return 0;
}
```

### [4. 静态成员](#)
静态成员为所有对象共享！很简单哈！只需要在 数据或者函数成员前面加上 `static` 关键字就好了。当我们声明类的成员为静态时，这意味着无论创建多少个类的对象，静态成员都只有一个副本。

不要再类声明中初始化静态成员变量，这是因为声明描述了如何分配内存，但并不分配内存。初始化是在 类外实现而不是在类声明中进行

**C++语言规范并未包含类似静态构造函数的东西！ 那是C#的东西哦！**

**C++ 静态成员访问很特殊，你可以用对象来访问，也可以使用类名来访问** `类名::静态成员名 对象.静态成员名  都是被允许的`

静态成员也有访问修饰符，外界可以访问到共有的静态成员！
```cpp
//flag 是静态数据成员
Vector2D v(3.0,3.0);

std::cout << Vector2D::flag << std::endl; 
std::cout << v.flag << std::endl; 
```

静态函数成员只能访问静态成员，静态数据或者静态函数！ 不能访问非静态成员，但是非静态成员可以访问静态成员！

#### [4.1 静态数据成员](#)
**静态数据成员** 在类声明中声明，在包含类方法的文件中初始化。处说实话时使用作用域运算符来指出静态成员所属的类。但如果静态成员是 const 整数类型或枚举型，则可以在类声明中初始化;
C++ 可以使用类名加上作用域解析符一起使用直接调用公开静态成员！ 也可以通过对象调用！

* 非 const 静态成员只能在类外定义初始化！
* const 静态成员可以在类声明中初始化！ **推荐还是在外部初始化，不然即使在类内部初始化了，也应该在外部定义一下。**


##### [4.1.1 非const的static数据成员](#)
如果数据成员是非 const， 就很简单啦。

```cpp
#include <iostream>
 
using namespace std;
 
class Box
{
   public:
      static int objectCount;
      // 构造函数定义
      Box(double l=2.0, double b=2.0, double h=2.0)
      {
         cout <<"Constructor called." << endl;
         length = l;
         breadth = b;
         height = h;
         // 每次创建对象时增加 1
         objectCount++;
      }
      double Volume()
      {
         return length * breadth * height;
      }
      static int getCount()
      {
         return objectCount;
      }
   private:
      double length;     // 长度
      double breadth;    // 宽度
      double height;     // 高度
};
 
// 初始化类 Box 的静态成员
int Box::objectCount = 0;
 
int main(void)
{
  
   // 在创建对象之前输出对象的总数
   cout << "Inital Stage Count: " << Box::getCount() << endl;
 
   Box Box1(3.3, 1.2, 1.5);    // 声明 box1
   Box Box2(8.5, 6.0, 2.0);    // 声明 box2
 
   // 在创建对象之后输出对象的总数
   cout << "Final Stage Count: " << Box::getCount() << endl;
 
   return 0;
}
```

##### [4.1.2 const static 数据成员](#)
如果数据成员是 const static的！如果数据的类型是 int 那么可以直接初始化！ 但是如果数据的类型不是 整形，那么必须加上` `constexpr` `关键字。

初始化也需要 :: 限定符！
```cpp
class Vector2D
{
private:
    //静态数据
    const static int initialValue = 0;
    constexpr const static double originX = 0.0; 
    static unsigned long verctor2dObjectCount;
	double _x;
	double _y;
public:
    static bool flag;
	/*
	  省略
	*/
};

bool Vector2D::flag = true;
```

#### [4.2 静态函数成员](#)
静态函数成员只能访问静态成员，静态数据或者静态函数！ 不能访问非静态成员，但是非静态成员可以访问静态成员 **[谨记]**

StringBuilder.h

```cpp
#ifndef STRINGBUILDER_H_
#define STRINGBUILDER_H_ STRINGBUILDER_H_
#include<string>

class StringBuilder
{
private:
	//静态数据 
	static int builderCount;
	int len;
	char* str;
public:
	//静态方法 只能访问静态数据
	static int GetBuilderCount();
	StringBuilder(const char*);
	StringBuilder(const StringBuilder&);
	~StringBuilder();
};


//静态数据初始化
int StringBuilder::builderCount = 0;

//静态方法实现
int StringBuilder::GetBuilderCount()
{
	return builderCount;
}

StringBuilder::StringBuilder(const char* s) {
	len = std::strlen(s);
	str = new char[len + 1];
	std::strcpy(str, s);
	builderCount++;
}


StringBuilder::StringBuilder(const StringBuilder& sb) {
	len = std::strlen(sb.str);
	str = new char[len + 1];
	std::strcpy(str, sb.str);
	builderCount++;
}

StringBuilder::~StringBuilder() {
	delete [] str;
}
```

使用类名调用！
```cpp
cout<< StringBuilder::GetBuilderCount() << endl;
```

### [5. 动态内存分配](#)
动态内存就是使用 `new` 和 `delete`来创建类，或者在类里面使用 `new` `delete` 创建内部成员。 没啥可说的，你懂的！

当类有指针成员或有动态分配空间，都应实现自定义的拷贝构造函数。提供了拷贝构造函数，最后也实现赋值运算符。 这也是  `<<C++ Primer Plues>>` 12章的主题所在

在构造函数使用new时应注意的事项
1. 如果在构造函数中使用 `new` 来初始化指针成员，则应在析构函数中使用 `delete`来释放内存。
2. `new` 和 `delete` 必须兼容，`new` 和 `delete` 对应，`new name[]` 和 `delete [] name`相对应。
3. 如果有多个构造函数，应该以相同的方法使用 `new`，要么带 `[]`,要么不带（因为只有一个析构函数）。另外，`delete`，无论带不带[]都可以用于空指针。空指针表示法，传统的是 `NULL` 或 `0`（ `NULL`是0的符号表示），现在可以使用 `nullptr`，这是一种个更好的选择。
4. 应该定义一个复制构造函数，通过深度复制将一个对象初始化为另一个对象。
5. 应当定义一个赋值运算符（运算符重载），通过深度复制将一个对象赋值给另一个对象。具体需要完成的工作是：检查自我赋值的情况；释放成员指针以前指向的内存；复制数据而不仅仅是数据的地址；返回一个指向调用对象的引用。


如果你使用了 `定位 new` 那么你一定要显式地调用析构函数，然后再释放 `buffer` ！不然可能会出大问题！ 多复习 《C++ primer plus》 p372-p373

### [6. 各种构造函数](#)
由于特殊成员函数的问题，很可能会产生与你的类设计不符合的行为！ C++ 将自动提供下面这些成员函数

1. 默认构造函数 - 如果没有定义构造函数
2. 默认析构函数 - 如果没有定义
3. 拷贝(复制)构造函数 - 如果没有定义
4. 赋值运算符 - 如果没有定义 `ClassName& operator = (const ClassName& str);` 重载赋值操作符函数：对赋值操作符"="进行重载，解决了对象赋值的情况下，析构函数中过程中多次释放同一块内存的问题！
5. 地址运算符 - 如果没有定义

更准确说，编译器将会生成上诉最后三个函数的定义。 我们将这些又编译器生成的函数或者运算符称为: `隐式xxxx`; `C++11` 还提供了另外两个特殊成员函数 `move constructor`  `move assignment operator`

```cpp
//地址运算符
const ClassName* operator&() const
{
    return this;
}
```

#### [6.1 默认构造函数](#)
没有编写构造函数时，编译器会自动生成一个什么都不做的无参构造函数! 例如

```cpp
class Time
{
private:
	unsigned int hour;
	unsigned int minutes;
public:
	std::string ToString();
	Time operator +(const Time& t) const;

```
`C++` 提供一个无参数的构造函数 `Time::Time(){ }` 如果不想编译器自动生成，就自己手写一个构造函数！

#### [6.2 拷贝构造函数](#)
拷贝(复制)构造函数的参数可以是 `const 引用`，也可以是非 const 引用。 **一般使用前者，这样既能以常量对象（初始化后值不能改变的对象）作为参数，也能以非常量对象作为参数去初始化其他对象。** 一个类中写两个复制构造函数，一个的参数是 const 引用，另一个的参数是非 const 引用，也是可以的。

如果类的设计者不写复制构造函数，编译器就会自动生成复制构造函数。大多数情况下，其作用是实现从源对象到目标对象对 **非静态成员** 逐个字节的复制，即使得目标对象的每个成员变量都变得和源对象相等。编译器自动生成的复制构造函数称为**默认拷贝构造函数**。

注意，默认构造函数（即无参构造函数）不一定存在，但是复制构造函数总是会存在。

[**拷贝构造函数必须以引用的方式传递参数 --- 考察点**](#)。**这是因为，在值传递的方式传递给一个函数的时候，会调用拷贝构造函数生成函数的实参**。 **如果拷贝构造函数的参数仍然是以值的方式，就会无限循环的调用下去，直到函数的栈溢出**。

```cpp
#ifndef POINT3D_H_
#define POINT3D_H_
#include<iostream>
namespace kick {

	class Point3D
	{
	friend std::ostream& operator<<(std::ostream& os, const Point3D& p);
	private:
		double x;
		double y;
		double z;
	public:
		Point3D(double _x, double _y, double _z);
		Point3D(const Point3D&);////拷贝构造函数 
	};
}
#endif // !POINT3D_H_
```
`cpp` 实现

```cpp
#include "Point3D.h"
namespace kick {

	Point3D::Point3D(const Point3D& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
	}
    /* ... */
}
```
定义复制构造函数的原因就是为了防止 在使用动态内存分配的时候，编译器自动进行`浅拷贝` 带来问题。 从而让对象对内部成员使用 `深拷贝`！

```cpp
Point3D p(5.1,95.2,-62.315);
Point3D p1 = p; //等价于 Point3D p1 = Point3D(P); 调用复制构造函数
```

**被调用的情况:** 

1. 当用一个对象去初始化同类的另一个对象时，会引发复制构造函数被调用。 如下所示：
```cpp
using namespace kick;
Point3D p(5.1,95.2,-62.315);

Point3D p1 = p;
Point3D p2(p);
Point3D p3 = Point3D(p);
Point3D* pp4 = new Point3D(p);

```

2. 当将对象作为函数的参数的时候, Func 函数的形参 a 在初始化时调用了复制构造函数。
```cpp
void Func(Point3D p){ /** */ }

int main(){
    Point3D point(5.1,95.2,-62.315);
    Func(point);
    return 0;
}
```

3. 如果函数的返冋值是类 A 的对象，则函数返冋时，类 A 的复制构造函数被调用。
```cpp
Point3D Func() {
    Point3D a(4);
    return a;
}

cout << Func() << endl;
```
4. 编译器生成临时对象

**总结:** 

- 拷贝初始化（用=定义变量）
- 将一个对象作为实参传递给一个非引用类型的形参
- 从一个返回类型为非引用类型的函数返回一个对象
- 用花括号列表初始化一个数组中的元素或一个聚合类中的成员
- 某些类类型还会对它们所分配的对象使用拷贝初始化。*（初始化标准库容器或调用其insert/push操作时，容器会对其元素进行拷贝初始化）*



#### [6.3 赋值运算符](#)

**赋值运算符是将对象的值复制给一个已经存在的实例**  顾名思义，赋值运算符是当赋值的时候所使用。但是默认的赋值运算符所采用的也是浅复， 赋值运算符函数，定义一个赋值运算符时需要考虑以下四点： 

语法格式
```cpp
//声明
ClassName & operator=(const ClassName &);

//定义
ClassName & ClassName::operator=(const ClassName &){

}
```

1. 把返回值声明为该类型的引用，并在函数结束时前返回实例自身的引用（ `*this` ）。只有返回一个引用，才可以允许连续赋值。
2. 传入的参数需要声明为常量引用。 **如果传入的参数不是引用而是实例，则从形参到实参会调用一次复制构造函数，造成无谓消耗。**
3. **释放实例自身已有的内存。如果在分配新内存前忘记释放自己已有的内存，则会出现内存泄漏。**
4. 判断传入参数与当前实例是不是同一个实例。如果是同一个，则立即返回。如果事先不判断会导致严重问题：当 `*this` **和传入参数是同一个实例，一旦释放了自身的内存，传入的参数的内存便也就同时被释放了，后续再也找不到需要赋值的内容了。**

```cpp
#pragma once
#ifndef POINT3D_H_
#define POINT3D_H_
#include<iostream>
namespace kick {

	class Point3D
	{
	friend std::ostream& operator<<(std::ostream& os, const Point3D& p);
	private:
		static int Point3DCount;
		double x;
		double y;
		double z;
	public:
		inline static int getPoint3DCount() { return Point3DCount; };
		Point3D(double _x, double _y, double _z);
		Point3D(const Point3D&);////拷贝构造函数
		Point3D& operator = (const Point3D& p);  //赋值运算符
	};
}
#endif // !POINT3D_H_
```
实现

```cpp
#include "Point3D.h"
namespace kick {

	int Point3D::Point3DCount = 0;

	kick::Point3D::Point3D(double _x, double _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
		Point3DCount++;
	}

	Point3D::Point3D(const Point3D& p)
	{
		std::cout << "调用拷贝构造函数" << std::endl;
		x = p.x;
		y = p.y;
		z = p.z;
		Point3DCount++;
	}

	Point3D& Point3D::operator=(const Point3D& p)
	{
	    std::cout << "调用赋值运算符"<< std::endl;
		if (this != &p)
		{
			x = p.x;
			y = p.y;
			z = p.z;
		}
		return (*this);
	}

	std::ostream& operator<<(std::ostream& os, const Point3D& p)
	{
		os << "x:" << p.x << " y:" << p.y << " z:" << p.z;
		return os;
	}
}
```

执行
```cpp
#include <iostream>
#include <string>
#include"Point3D.h"
using namespace std;
int main(int argc, char const* argv[])
{
    using namespace kick;
    Point3D p(5.1,95.2,-62.315);
    Point3D p1 = p;
    Point3D p2(p);
    Point3D p3 = Point3D(p);
    Point3D* pp4 = new Point3D(p);

    Point3D p5(25.45, 35.0, 71.21);
    p5 = p;


    cout << p << endl;
    cout << p1 << endl;
    cout << p2 << endl;
    cout << p3 << endl;
    cout << *pp4 << endl;
    cout << p5 << endl;
    cout << p.getPoint3DCount() << endl;

    delete pp4;

    return 0;
}
/*
调用拷贝构造函数
调用拷贝构造函数
调用拷贝构造函数
调用拷贝构造函数
调用赋值运算符

x:5.1 y:95.2 z:-62.315
x:5.1 y:95.2 z:-62.315
x:5.1 y:95.2 z:-62.315
x:5.1 y:95.2 z:-62.315
x:5.1 y:95.2 z:-62.315
x:5.1 y:95.2 z:-62.315
*/
```

```cpp
friend std::ostream& operator<<(std::ostream& os, const Point3D& p);
```
请问传入的参数 `std::ostream&` 为啥是引用类型，而不是直接传入 `ostream` 因为 `ostream` 这玩意儿啊！他没有 `public` 的拷贝构造函数！

#### [6.4 拷贝构造函数和赋值运算符](#)
在默认情况下（用户没有定义，但是也没有显式的删除），编译器会自动的隐式生成一个拷贝构造函数和赋值运算符。但用户可以使用delete来指定不生成拷贝构造函数和赋值运算符，这样的对象就不能通过值传递，也不能进行赋值运算。


拷贝构造函数和赋值运算符的行为比较相似，都是将一个对象的值复制给另一个对象；但是其结果却有些不同。

**拷贝构造函数使用传入对象的值生成一个新的对象的实例，而赋值运算符是将对象的值复制给一个已经存在的实例。**

**调用的是拷贝构造函数还是赋值运算符，主要是看是否有新的对象实例产生。如果产生了新的对象实例，那调用的就是拷贝构造函数；如果没有，那就是对已有的对象赋值，调用的是赋值运算符。**

```cpp
Point3D p(5.1,95.2,-62.315); //自定义构造函数
Point3D p1 = p; //拷贝构造函数
Point3D p5(25.45, 35.0, 71.21);//自定义构造函数
p5 = p;//调用赋值运算符
```

**赋值运算符 同样可以接受右值参数哦~~~~**

```cpp
Point3D& Point3D::operator=(Point3D&& p)
{
    std::cout << "调用赋值运算符"<< std::endl;
    if (this != &p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
    }
    return (*this);
}
```

#### [6.5 移动构造函数](#)
移动构造是C++11标准中提供的一种新的构造方法。移动构造可以减少不必要的复制，带来性能上的提升。有些复制构造是必要的，我们确实需要另外一个副本；而有些复制构造是不必要的，我们可能只是希望这个对象换个地方，移动一下而已。

在C++11之前，如果要将源对象的状态转移到目标对象只能通过复制。使用复制构造函数就可以了！

移动构造函数定义形式： **不能加上 const 参数，因为移动构造函数可能需要修改内部参数以转交对数据的控制权**
```cpp
class_name(class_name && )
```

如果编译器优化了，你是看不到效果的！
```cpp
#include <iostream>
#include <string.h>
#include <math.h>

using namespace std;

class IntNum{  
public:  
     IntNum(int x = 0):xptr(new int(x)){//构造函数  
        cout<<"Calling constructor....  x: "<< x << endl;  
	}  
    IntNum(const IntNum &n){//复制构造函数  
        cout<<"Calling copy constructor... n.x: " << *n.xptr <<  " this.x:" <<*xptr <<endl;  
        xptr = new int(*n.xptr);
	}
    IntNum(IntNum && n):xptr(n.xptr){//移动构造函数
        n.xptr = nullptr;
        cout<<"Calling move constructor... && n.x: " << *n.xptr <<endl;
	}
    IntNum operator=(const IntNum &);
   ~IntNum(){//析构函数  
        int num = *xptr; 
        delete xptr;  
        cout<<"Destructing... x: " << num <<endl;
	} 
    int getInt(){return *xptr;}//返回指针所指向的值，而不是返回指针本身  
private:  
         int *xptr;  
};  

IntNum IntNum::operator=(const IntNum & nm){
    cout<<"Calling operator = ... nm.x:" << *(nm.xptr) << " this.x:" << *xptr <<endl;
    *xptr = *(nm.xptr);
    return (*this);
}  

//返回值为IntNum类对象  
IntNum getNum(const int& val){  
     //定义了一个局部对象，然后将局部对象作为结果返回  
      IntNum a(val);  
    //返回值是IntNum类型  
      int i = 1600;
      while (i!=0)
      {
        i--;
      }
      return a;  
}  

int main(int argc, char const* argv[])
{
   //getNum()函数返回了一个IntNum类型的对象（临时无名对象），之后调用类的函数  
    IntNum b(111);
    b = getNum(105); //b.operator=(getNum(105));

    cout<<b.getInt()<<endl;
    cout<<getNum(105).getInt()<<endl;
    return 0;  
}

/* g++ 优化后 */
/*
Calling constructor....  x: 111
Calling constructor....  x: 105
Calling operator = ... nm.x:105 this.x:111
Calling copy constructor... n.x: 105 this.x:-1154862024
Destructing... x: 105
Destructing... x: 105
105
Calling constructor....  x: 105
105
Destructing... x: 105
Destructing... x: 105
*/
```

**强制移动**:

强行调用移动构造函数或者移动赋值运算符， 有两种方式！

* 使用 static_cast<ClassName &&>
* 使用 std::move(obj)

#### [6.6 移动赋值运算符](#)
参数是右值引用的意思， 属于C++11新加的，常用于资源管理类！
```cpp
class_name operator=(class_name && ) noexcept;
    
    
Student& Student::operator=(Student&& user) noexcept {
    std::swap(*this, user);
    return *this;
}
```
**必背箴言:** 

**与拷贝操作不同，编译器根本不会为某些类合成移动操作。特别是，如果一个类定义了自己的拷贝构造函数、拷贝赋值运算符或者析构函数，编译器就不会为它合成移动构造函数和移动赋值运算符了。**

[**如果一个类没有移动操作，通过正常的函数机制，类会使用对应的拷贝操作代替移动操作。**](#)

**只有当一个类没有定义任何自己版本的拷贝控制成员，并且类的每一个非static数据成员都是可以移动的时候，编译器才会为它合成移动构造函数和移动赋值运算符。**

#### [6.7 委托构造函数](#)

为了让编码更简单、更可靠，`C++` **允许在一个构造函数的定义中使用另一个构造函数，这被称为委托**，因为构造函数暂时将创建对象的工作委托给另一个构造函数。
**下面的默认构造函数使用第一个构造函数初始化数据成员并执行其函数体，然后再执行自己的函数体。**
```cpp
class Notes
{
    int k;
    double x;
    std::string st;
public:
    Notes();
    Notes(int);
    Notes(int, double);
    Notes(int, double, std::string);
};
Notes::Notes(int kk, double xx, std::string stt) :k(kk),
            x(xx), st(stt) {/*do stuff*/}
Notes::Notes() : Notes(0, 0.01, "Oh") {/*do other stuff*/}
Notes::Notes(int kk) : Notes(kk, 0.01, "Ah") {/*do yet other stuff*/ }
Notes::Notes(int kk, double xx) : Notes(kk, xx, "Uh") {/*ditto*/ }
```

#### [6.8 深拷贝和浅拷贝](#)
默认生成的拷贝构造函数和赋值运算符，只是简单的进行值的复制。Person类，字段只有int和string两种类型，这在拷贝或者赋值时进行值复制创建的出来的对
象和源对象也是没有任何关联，对源对象的任何操作都不会影响到拷贝出来的对象。反之，假如Person有一个对象为int \*，这时在拷贝时还只是进行值复制，那
么创建出来的Person对象的int \*的值就和源对象的int \*指向的是同一个位置。任何一个对象对该值的修改都会影响到另一个对象，这种情况就是浅拷贝。


深拷贝和浅拷贝主要是针对类中的指针和动态分配的空间来说的，因为对于指针只是简单的值复制并不能分割开两个对象的关联，任何一个对象对该指针的操作都会影响到另一个对象。这时候就需要提供自定义的深拷贝的拷贝构造函数，消除这种影响。通常的原则是：

1. 含有指针类型的成员或者有动态分配内存的成员都应该提供自定义的拷贝构造函数
2. 在提供拷贝构造函数的同时，还应该考虑实现自定义的赋值运算符

对于拷贝构造函数的实现要确保以下几点：
1. 对于值类型的成员进行值复制
2. 对于指针和动态分配的空间，在拷贝中应重新分配分配空间
3. 对于基类，要调用基类合适的拷贝方法，完成基类的拷贝

拷贝构造函数和赋值运算符的行为比较相似，却产生不同的结果；拷贝构造函数使用已有的对象创建一个新的对象，赋值运算符是将一个对象的值复制给另一个已存在的对象。区分是调用拷贝构造函数还是赋值运算符，主要是否有新的对象产生。
关于深拷贝和浅拷贝。**当类有指针成员或有动态分配空间，都应实现自定义的拷贝构造函数。提供了拷贝构造函数，最后也实现赋值运算符。**

#### [6.9 使用原则](#)

**一个类可以具有默认构造函数、拷贝构造函数、赋值运算符、移动构造函数、移动赋值运算符、析构函数** ，**C++** **语言并不要求我们定义所有的操作: 可以只定义其中一个或两个，而不必定义所有。**

**通常定义所有操作的情况是很少见的！**

* [**如果这个类需要一个析构函数，我们几乎可以肯定它也需要一个拷贝构造函数和一个赋值运算符，反之不成立！**](#) `可能需要潜在的delete 操作，防止内存泄露！`
* [**拷贝构造函数和赋值运算符如果需要其中一个，那么就一定两个都需要！**](#)

### [7. 返回对象的问题](#)

让我们看看如下两个函数有什么区别
```cpp
Point3D Max(const Point3D& p1, const Point3D& p2) {
    if (p1.distance() > p2.distance())
        return p1;
    else
        return p2;
}

const Point3D & Maxx(const Point3D& p1, const Point3D& p2) {
    if (p1.distance() > p2.distance())
        return p1;
    else
        return p2;
}
```
前者返回的是一个对象实例，后者返回一个引用！这有啥区别呢？前者会调用拷贝构造函数，而后者返回引用则不会调用！因此第二个版本所做的工作更少，效率更高。

### [8. 对象的消亡](#)
对象何时会消亡，何时会被调用析构函数！

```cpp
kick::Point3D origin(0.0, 0.0, 0.0); //静态成员

int main(int argc, char const* argv[])
{

    Point3D p(5.1,95.2,-62.315); 
    
    Point3D* pp4 = new Point3D(p); //对象使用new创建的

    {
        Point3D p3 = Point3D(p);//动态变量
        cout << p3 << endl;
    }

    cout << origin << endl;

    delete pp4;
    /* ... */
    return 0;
}
```

1. 如果对象时动态变量，则当执行完定义该对象的程序块时，将调用该对象的析构函数！
2. 如果对象时静态变量(外部，静态，静态外部或来自名称空间)，则在程序结束时将调用对象的析构函数！
3. 如果7 对象时用 `new` 创建的，则仅当你显式使用delete删除对象时，析构函数才会被调用！

### [9. 成员初始化列表](#)
首先如果一个类有一个成员是这样的 `const int size` 我们应该如何初始化它呢！并且我们希望这个值最好不要写死，能够根据构造函数指定！

你可能想到如此写：
```cpp
class Self
{
private:
	const int _size;
public:
	Self(int size);
};
```
如此实现
```cpp
#include "Self.h"
Self::Self(int size)
{
	_size = size;
}
```
可是很明显， `_size = size;` 这个语句是通不过的，那么该怎么办呢？ 只能这样办。使用成员初始化列表！
```cpp
Self::Self(int size):_size(size){ }
```
你可能问，为啥不能通过构造函数内部去初始化呢！让我们来解释一下吧！

#### [9.1 原理](#)
从概念上来说，调用构造函数时，对象将在构造函数括号中的代码被执行之前被创建。因此，调用构造函数将导致程序先给成员变量分配内存，然后，程序流程进入到括号中，使用常规的赋值方式将值存储到内存中！我们将构造函数执行分为
两个先后过程：
1. 初始化阶段： 创建对象，给成员变量分配内存。调用构造函数时，创建对象，隐式/显式地初始化各数据成员(给其成员变量分配内存);
2. 赋值计算阶段: 进入构造函数中执行一般赋值与计算操作！

下列四种情况下，必须使用成员初始化列表：
1. 当初始化一个引用成员时
2. 当初始化一个 `const` 成员时
3. 当调用一个基类的构造函数，而它拥有一组参数时
4. 当调用一个成员类的构造函数，而它拥有一组参数时

**例如 [引用成员]:**  
我们给Self增加一个引用对象 `Point3D& point`！我们该如何初始化它呢？

```cpp
#include "Point3D.h";
using namespace kick;
class Self
{
private:
	Point3D& point;
	const int _size;
public:
	Self(int size, Point3D &);
};
```
你可能想这样初始化！
```cpp
Self::Self(int size, Point3D& p): _size(size)
{
	point = p;
}
```
然后代码无法通过！为啥通不过呢？ 回忆一下引用类型，他要求一旦声明引用类型就必须初始化，一旦初始化就不能改变！ 他其实等同于 const 类型! 

可以如此初始化：
```cpp
Self::Self(const int size, Point3D& p): _size(size),point(p){}
```

综上所述，如果是普通数据成员呢？我也希望你使用成员初始化列表。不然还要进行赋值计算阶段！

**例如 [类对象成员]:**  
我们可以
```cpp
#ifndef SELF_H_
#define SELF_H_
#include "Point3D.h";
using namespace kick;
class Self
{
private:
	Point3D point;
	const int _size;
public:
	Self(int size, Point3D &);
};
```

```cpp
Self::Self(const int size, Point3D& p): _size(size),point(p){
	point = p;
}
```

```cpp
int main(int argc, char const* argv[])
{
    Point3D p = Point3D(5.1, 5.2, 5.3); // 1
    Self s = Self(5, p); // 2 3
    return 0;
}
```

分析一下它的调用顺序：

1. 调用 Point3D 普通构造函数 Point3D(double _x, double _y, double _z);
2. 调用 Point3D 拷贝构造函数 Point3D(const Point3D& p)
3. 调用 Point3D 赋值运算符 Point3D& operator = (const Point3D& p)

也就是说 `point(p)` 调用的是 `Point3D(const Point3D& p)` 这个构造函数哦！ 联系一下，你是不是可以手动觉得调用哪个构造函数啊！当然如果 `Point3D` 每个这个拷贝构造函数的话！ 它调用什么？当然是 默认拷贝构造函数啊！
如果对象有动态内存分配, 而没有拷贝构造函数, 这里就要出大问题！ 也就是说初始化函数列表到底干了啥？
就是在第一阶段 --- 初始化阶段，指定了构造函数！

就是说你可以写一个这样的构造函数 
```cpp
Self(int size, int x, int y, int z);
```
如此实现:
```cpp
Self::Self(const int size, int x, int y, int z): _size(size),point(x, y, z){}
```

### [10. 类内成员初始化](#)
C++ 11之后支持直接在类定义中初始化成员，而不知只能用构造函数的方式， 语法如下所示。 通过使用类内初始化，可以避免在构造函数中编写同样的代码，降低工作量。
```cpp
class Point3D
{
friend std::ostream& operator<<(std::ostream& os, const Point3D& p);
private:
    static int Point3DCount = 0;  //类内部初始化
    double x = 0;
    double y = {0}; //大括号也可以的
    double z = 0;
public:
    inline static int getPoint3DCount() { return Point3DCount; };
    Point3D(double _x, double _y, double _z);
    Point3D(const Point3D&);////拷贝构造函数
    Point3D& operator = (const Point3D& p);  //赋值运算符
};
```


### [11. C++ 11 =delete禁止调用和生产](#)
如果你的类不需要拷贝/移动操作，请显式的通过在public域中（private本身就是不能显式的直接调用的）使用 =delete 或其他手段禁用之， 如果你喜欢某种自动类型转换不会发生, 请使用=delete。

* 禁止类使用默认生成的成员函数，最好设置为private，同时设置为=delete
* 禁止类使用其他类成员函数


```cpp
// MyClass is neither copyable nor movable.
MyClass(const MyClass&) = delete;
MyClass& operator=(const MyClass&) = delete;
```
****
```cpp
MyClass my;
MyClass me;
my = me; //无法被调用
MyClass other(me); //错误 无法调用
```

#### [11.1 禁止隐私类型转换](#)
如下所示: 

```cpp
#include <cstdio>

class TestClass
{
public:
    void func(int data) { printf("data: %d\n", data); }
};

int main(void)
{
    TestClass obj;
    obj.func(100);
    obj.func(100.0);
    return 0;
}
```
上面的示例的输入是：
```
data: 100
data: 100
```

```cpp
#include <cstdio>

class TestClass
{
public:
    void func(int data) { printf("data: %d\n", data); }
    void func(double data)=delete; //禁止
};
int main(void)
{
    TestClass obj;
    obj.func(100);
    obj.func(100.0); //直接编译不过

    return 0;
}
```

毫无疑问，还可以禁止拷贝和赋值！**但是析构函数是不能被 delete的！**

### [12. C++ 11 =default](#)

可以通过 `=default` 关键来声明构造函数，告诉编译器为该类生成一个默认的版本。主要作用在构造函数，拷贝构造，搬移构造，`=delete` 就是不要它，`=deafult` 是要编译器默认那个；

* 析构函数可以 `=default` 使用默认，但是不可以 `=delete`；
* `=delete` 可以用到任何函数（非析构函数）上，`=0` 只能用于虚函数；变成纯虚函数；


```cpp
Shareholding() = default; //要求强制生成默认构造函数
Shareholding(const Shareholding& other) = default;  //生成默认的复制构造函数
Shareholding(const Shareholding&& other) = default; //生成默认移动构造函数
```

#### [12.1 default 移动操作](#)

与拷贝操作不同，移动操作永远不会隐式定义为删除的函数。但是，如果我们显式地要求编译器生成`=default`的移动操作，且编译器不能移动所有成员，则编译器会将移动操作定义为删除的函数。

- 与拷贝构造函数不同，移动构造函数被定义为删除的函数的条件是：有类成员定义了自己的拷贝构造函数且未定义移动构造函数，或者是有类成员未定义自己的拷贝构造函数且编译器不能为其合成移动构造函数。移动赋值运算符的情况类似。
- 如果有类成员的移动构造函数或移动赋值运算符被定义为删除的或是不可访问的，则类的移动构造函数或移动赋值运算符被定义为删除的。
- 类似拷贝构造函数，如果类的析构函数被定义为删除的或不可访问的，则类的移动构造函数被定义为删除的。
- 类似拷贝赋值运算符，如果有类成员是`const`的或是引用，则类的移动赋值运算符被定义为删除的。

**如果类定义了一个移动构造函数和/或一个移动赋值运算符，则该类的合成拷贝构造函数和拷贝赋值运算符会被定义为删除的。因此，定义了一个移动构造函数或移动赋值运算符的类必须也定义自己的拷贝操作。否则，这些成员默认地被定义为删除的。**

### [13. 可变数据成员](#)

**mutable** 用于结构体/类，即使结构体/类变量被声明为const， 那么其内部某个成员也可以被修改

```cpp
class Screen{
public:
    void some_number() const;
private:
    mutable size_t access_ctr; //即使在const对象内也可以修改
}

void Screen::some_number() const{
    ++access_ctr; //可以修改哦
}
```



### 参考资料

1. 成员初始化列表 [https://blog.csdn.net/zhizhengguan/article/details/116405457](https://blog.csdn.net/zhizhengguan/article/details/116405457)
2. 成员初始化列表 [https://zhuanlan.zhihu.com/p/386604081](https://zhuanlan.zhihu.com/p/386604081)