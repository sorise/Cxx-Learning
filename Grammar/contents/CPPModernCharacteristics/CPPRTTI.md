### [C++ RTTI 运行阶段类型识别 和 cast类型转换](#)
Runtime Type Identification 运行阶段类型识别

-----
- [x] [1. RTTI的用途](#1-rtti的用途)
- [x] [2. 类型转换运算符](#2-类型转换运算符)

-----

### [1. RTTI的用途](#)
RTTI是运行阶段类型识别（Runtime Type Identification）的简称。这是新添加到C++中的特性之一，很多老式实现不支持。另一些实现可能包含开关RTTI的编译器设置。RTTI旨在为程序再运行阶段确定对象的类型提供一种标准方式。
很多类库已经为其类对象提供了实现这种功能的方式，但由于C++内部并不支持，因此各个厂商的机制通常互不兼容，创建一种RTTI语言标准将使得未来的库能够彼此兼容。

假设有一个类层次结构，其中的类都是从一个基类派生而来的，则可以让基类指针指向其中任何一个类的对象。

有时候我们会想要知道指针具体指向的是哪个类的对象。因为：

可能希望调用类方法的正确版本，而有时候派生对象可能包含不是继承而来的方法，此时，只有某些类的对象可以使用这种方法。
也可能是出于调试目的，想跟踪生成的对象的类型。

#### [1.1 RTTI的工作原理](#)
C++有3个支持RTTI的元素：

* 如果可能的话，dynamic_cast运算符将使用一个指向基类的指针来生成一个指向派生类的指针；否则，该运算符返回0（空指针）；
* **typeid** 运算符返回一个指出对象的类型的值；
* **type_info**结构存储了有关特定类型的信息。

> 注意️：️RTTI只适用于包含虚函数的类。因为只有对于这种类层次结构，才应该将派生类的地址赋给基类指针。

```cpp
int age = 27;
int *ptr = &age;

muse::BinarySerializer serializer;

std::cout <<  typeid(age).name() << std::endl; //i
std::cout <<  typeid(ptr).name() << std::endl; //Pi
std::cout <<  typeid(serializer).name() << std::endl; //N4muse16BinarySerializerE
```


#### [1.2 dynamic_cast](#)
这是最常用的RTTI组件，它不能回答“指针指向的是哪类对象”这样的问题，但能够回答“是否可以安全地将对象的地址赋给特定类型的指针”这样的问题。 否则，结果为0，即空指针。
**说白了，就是看看这个对象指针能不能转换为目标指针。**

**dynamic_cast是将一个基类对象指针（或引用）转换到继承类指针，dynamic_cast会根据基类指针是否真正指向继承类指针来做相应处理。**

**至于“先上转型”（即派生类指针或引用类型转换为其基类类型），本身就是安全的，尽管可以使用dynamic_cast进行转换，但这是没必要的， 普通的转换已经可以达到目的，毕竟使用dynamic_cast是需要开销的。**
三个父子类
```cpp
class Grand
{
private:
   int num;
public:
   Grand(int meony):num(meony){};
   virtual std::string speak() const{ return "grand cnm"; }
   virtual ~Grand(){};
};

class Father: public Grand
{
private:
   /* data */
public:
   Father(int meony):Grand(meony){};
   virtual std::string speak() const{ return "father cnm"; }
   void cnm() const{ printf("father is me\n"); };
   virtual ~Father(){};
};

class Son:public Father
{
private:
   int son_id;
public:
   Son(int meony, int sid):Father(meony), son_id(sid){};
   virtual std::string speak() const{ return "Son cnm"; }
   int getNumber() const { return son_id; }
   virtual ~Son(){};
};

```
使用方法：
```cpp
int main(int argc, char const* argv[])
{   
   
   Grand* grd_ptr = new Grand(10);
   Grand* ftr_ptr = new Father(10);
   Grand* son_ptr = new Son(20, 115);

   Father *ptr_father = dynamic_cast<Father *>(ftr_ptr); 
   ptr_father->cnm();//father is me

   std::cout << ptr_father->speak() << "\n" ; //father cnm
   std::cout << dynamic_cast<Son *>(son_ptr)->getNumber() << "\n" ; //115
   
   delete grd_ptr, ftr_ptr, son_ptr;
   return 0;
}
```

也可以将dynamic_cast用于引用，用法稍微有点不同：
没有与空指针对应的引用值，因此无法使用特殊的引用值来指示失败。当请求不正确时，dynamic_cast将引发类型为bad_cast的异常，这种异常是从exception类派生而来的，它是在头文件typeinfo中定义的。
```cpp
#include <typeinfo> // for bad_cast
/*...*/
try {
	Basic & rs = dynamic_cast<Basic &>(rt);
	/*...*/
}
catch(bad_cast &){
	/*...*/
};
```

尽量少使用转型操作，尤其是dynamic_cast，耗时较高，会导致性能的下降，尽量使用其他方法替代。

#### [1.3 typeid 运算符](#)
typeid 运算符能够用于确定两个对象是否为同种类型。它与 sizeof 有些想象，可以接受两种参数

* **类名**
* **结果为对象的表达式。**

```cpp
// 判断pg指向的是否是ClassName类的对象
typeid(ClassName) == typeid(*pg)
```
如果pg是一个空指针，程序将引发bad_typeid异常，该异常是从exception类派生而来的，它是在头文件typeinfo中声明的。

type_info类的实现随厂商而异，但包含一个name()成员，该函数返回一个随实现而异的字符串，通常（但并非一定）是类的名称。可以这样显示：

```cpp
int main(int argc, char const* argv[])
{   
   
   Grand* grd_ptr = new Grand(10);
   Father ft(10);
   

   std::cout<< "typeName:" << typeid(*grd_ptr).name() << "\n";
   //typeName:5Grand
   std::cout<< "typeName:" << typeid(ft).name() << "\n";
   //typeName:6Father
   std::cout<< "typeName:" << typeid(Father).name() << "\n";
   //typeName:6Father

   delete grd_ptr;
   
   return 0;
}
```

#### 1.4 误用RTTI的例子
有些人对RTTI口诛笔伐，认为它是多余的，会导致程序效率低下和糟糕的编程方式。这里有一个需要尽量避免的例子。

在判断是否能调用某个方法时，尽量不要使用if-else和typeid的形式，因为这会使得代码冗长。

如果在扩展的if else语句系列中使用了typeid，则应该考虑是否应该使用虚函数和dynamic_cast。
### [2. 类型转换运算符](#) 
C++的创始人Bjarne Stroustrup认为，C语言中的类型转换运算符太过松散，比如，在C语言中，可将指向一个对象的指针转换为完全不相关的类型，编译器不会报错（而C++中使用这种C风格类型转换也不会报错）

```c
// Example #1
int integar = 0;
char * pch = (char *) (&integer); // 不会报错
// Example #2
char str[4] = { 1,1,1,1 };
int * pint = (int*)str; 
// pint = 0000 0001 0000 0001 0000 0001 0000 0001
// pint = 16843009
```
对于这种松散的情况，Stroustrup采取的措施是，更严格地限制允许地类型转换，并添加4个类型转换运算符，使转换过程更规范：

* **dynamic_cast**
* **const_cast**
* **static_cast**
* **reinterpret_cast**

可以根据目的选择一个合适地运算符，而不是使用通用地类型转换。这指出了进行类型转换的原因，并让编译器能够检查程序的行为是否与设计者想法吻合。

它们的用法都相似：
```
dynamic_cast <type_name> (expression)
static_cast <type_name> (expresson)
const_cast <type_name> (expresson)
reinterpret_cast <type_name> (expresson)
```

|关键字|说明|
|:----|:----|
|static_cast|用于良性转换，一般不会导致意外发生，风险很低。|
|const_cast|用于 const 与非 const、volatile 与非 volatile 之间的转换。|
|reinterpret_cast|高度危险的转换，这种转换仅仅是对二进制位的重新解释，不会借助已有的转换规则对数据进行调整，但是可以实现最灵活的 C++ 类型转换。|
|dynamic_cast|借助 RTTI，用于类型安全的向下转型（Downcasting）。|

#### [2.1 dynamic_cast](#)
前面以及讲过了，**dynamic_cast** 用于类继承层次间的指针或引用转换。主要还是用于执行 **安全的向下转型（safe downcasting）**， 如本节 1.2所述！

作用对象的是： **指针或引用**  


#### [2.2 static_cast\<type\>(expr)](#)
当需要把一个较大的算数类型转换为较小的类型时，static_cast非常有用！这将告诉编译器，我们并不在乎潜在的精度损失，不然一般编译器会给出警告信息。可以用于编译器无法自动类型转换的时候！

用于在相关类型的指针之间进行转换，还可显式地执行标准数据类型之间类型转换（这种类型原本将自动或隐形地进行）！ 

用于指针时，static_cast实现了基本的编译阶段检查，确保指针被转换为相关类型。

```cpp
type v = static_cast<type>(expr);
//将 expr 转换为 type类型
```

* 用于基本数据类型之间的转换，如把 int 转换成 char，把int转换成enum。
* 把空指针void *转换成目标类型的指针 type * 。
* 把任何类型的表达式转换成 void类型。

注意：static_cast不能转换掉expression的const、volatile、或者__unaligned属性。

```cpp
char a = 'a';
int b = static_cast<int>(a);//正确，将char型数据转换成int型数据

double *c = new double;
void *d = static_cast<void*>(c);//正确，将double指针转换成void指针
double *ic = static_cast<double *>(d); //再转换回去

int e = 10;
const int f = static_cast<const int>(e);//正确，将int型数据转换成const int型数据

const int g = 20;
int *h = static_cast<int*>(&g);//编译错误，static_cast不能转换掉g的const属性
```


#### [2.3 const_cast\<type\>(expr)](#)
**const_cast** 运算符用于执行只有一种用途的类型转换，即改变值为const或volatile。 从而强制消除对象的常量性。使用const_cast去除const限定的目的并不是为了修改它的内容。
变量本身的const属性是不能去除的，要想修改变量的值，一般是去除指针（或引用）的const属性，再进行间接修改。**如果本身内容是const，那么后果自负！**

如果原来数据不是const, **以下程序能够正常运行是建立在，对象本身是一个非const的，如果对象 a本身也是一个 const的，那么将产生未定义的后果！**
```cpp
int a = 10;
const int *pa = &a;

int *p_pa = const_cast<int *>(pa);

*p_pa = 20;

std::cout<< "a = " << a << "\n";
//a = 20
```

如果原来数据是const，**结果未定义， 程序会闹鬼！**
```cpp
#include <iostream>
using namespace std;

int main(){
    const int n = 100;
    int *p = const_cast<int*>(&n);
    *p = 234;
    cout<<"n = "<<n<<endl; //n = 100
    cout<<"*p = "<<*p<<endl; //*p = 234

    return 0;
}
```


#### 2.4 reinterpret_cast
**危险的转换，如果不了解所涉及的类型和编译器实现转换的过程，别用！ 可能根本也用不上，别折磨自己！** 
**良好的程序设计能力，可以减少学习一些不必要的知识！**

reinterpret_cast是为了映射到一个完全不同类型的意思，这个关键词在我们需要把类型映射回原有类型时用到它。我们映射到的类型仅仅是为了故弄玄虚和其他目的，这是所有映射中最危险的。
使用reinterpret_cast的场合不多，仅在非常必要的情形下，其他类型的强制转换不能满足要求时才使用。



-----
时间: [] 