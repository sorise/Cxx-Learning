### [C++ 引用变量](#)
***介绍**：C++ 新增了一种复合类型 --- 引用变量。可以理解为已定义变量的别名！

----



### [1. 引用变量概述](#)

变量本身是给内存中的一块地址取一个程序员可以语义识别的名字， 引用变量通过原来的变量找到那块内存地址，再给他取个名字，例如学号也是指你，身份证号也是指你，工号还是指你通过引用可以直接操作原始值！

**引用变量，实际上是给另一个变量取一个别名。** 例如下面的 value 和 time 是同一块内存地址的不同名字。常常用作函数的形参，用引用变量用作参数，函数将使用原始数据，而不是副本。这样除了指针之外，引用也为函数处理大型结构提供了一种非常方便的途径。

**引用类型一旦创建就要初始化，一旦初始化后就不更更改！ 常用语函数传参！**

地址一样
```cpp
int value = 35;
int& time = value;

time = 33;

cout << value << endl;
cout << "value 的 内存地址:" << &value <<endl;//004CF740
cout << "time 的 内存地址:" << &time << endl;//004CF740
```
值一样
```cpp
int score = 95;
int& score_ = score;

score_ = 98;

cout << score_ << endl; //98
cout << score << endl; //98
```

引用更接近于const指针： 必须在创建的时候初始化使得和某个变量相关联，一旦与某个变量关联起来, 将一直效忠它。无法更改！
```cpp
int rats = 25;
int & mouses = rats;

int * const pm = &rats;

mouses = 75;

*pm = 75
```

**通过引用定义引用！**

```cpp
int v  = 15;
int &refV = v;
int &refRefV = refV;

std::cout << v << "\n";  //15
std::cout << refV << "\n"; //15
std::cout << refRefV << "\n"; //15
```
#### [1.1 引用作为函数参数](#)
这种将引用作为函数参数传递的方式叫引用传递。 C语言只能按值传递。我们以交换两个值为例子.

```cpp
/* 使用指针交换其值 */
void inline swapByPointer(float* x, float* y) {
    auto temp = *x;
    *x = *y;
    *y = temp;
}

/* 使用指针交换其值 */
void inline swapByReferenceType(float& x, float& y) {
    auto temp = x;
    x = y;
    y = temp;
}


int main(int argc, char const *argv[])
{
    using namespace std;
    float a = 56.12f, b = 75.23f;
    float x = 56.12f, y = 75.23f;
    swapByReferenceType(a, b);
    swapByPointer(&x, &y);
    cout << "a: "<< a << " b: "<< b << endl; //a: 75.23 b: 56.12
    cout << "x: "<< x << " y: "<< y << endl; //x: 75.23 y: 56.12
}

```
可以看到，只有了解函数定义或者原型的情况下才知道值按引用传递，实际调用的时候和按值传递没有啥区别！

将复合类型引用 传递给函数
```cpp
#include<iostream>
#include<array>


double total(std::array<double, 5> & list){
    double all = 0.0;
    for (int i = 0; i < 5; i++)
    {
        all += list[i];
    }
    
    return all;
}


int main(int argc, char const *argv[])
{
    using namespace std;
    std::array<double, 5> list = {89.12,98.12,99.12,87.5,87.25};
    
    cout<<"total: " << total(list) << endl;//total: 461.11
}
```


#### [1.2 const 引用](#)
你也可以使用const 引用，防止对值的修改 const dataType &; 最好加上const，不然你给函数传递字面值是通不过的 **const 引用还可以引用右值！**

```cpp
const int count = 256;
int &refCount = count; //编译错误
```

* 不经过引用修改原始值，就应该加上 const。这样可以传递字面值 [526. 0, true] 参数 如果参数没有 const 修饰符，那么就无法传递字面值，因为字面值是常量值！
* 如果加上 const 修饰引用，那么就无法修改原始值了。
* 不加 const 不支持隐式类型转换，例如 double & 就无法传递 int 类型的参数。加上const 就可以，const 可以生成临时变量的方式支持隐式转换！
* 加上 const 只有右值传递！

```cpp
const int count = 256;
const int &refCount = count; 
const int &refValue = 78; //指向右值！

std::cout << refCount << "," << refValue <<"\n"; 
//256, 78
```

```cpp
double rate = 6.894;

float inline dollar_to_rmb(double& doller) {
    return doller * rate;
}

int main(int argc, char const *argv[])
{
    using namespace std;
    double salary =  9511.12;
    double subsidy = 2000.0;
    double overtime_pay = 3581.12;
    
    cout<< salary <<" doller equal to " << dollar_to_rmb(salary) << "rmb" << endl;//通过
    cout << dollar_to_rmb(8552.12) << endl;//编译失败
    cout << dollar_to_rmb(11000) << endl;//编译失败 类型不同
    cout<< salary <<" doller equal to " << dollar_to_rmb(salary + subsidy + overtime_pay) << endl;//编译失败
    cout<< salary <<" doller equal to " << dollar_to_rmb(salary + 5000.12) << endl;//编译失败
}
```

使用常量引用
```cpp
double rate = 6.894;

float inline dollar_to_rmb(const double& doller) {
    return doller * rate;
}


int main(int argc, char const* argv[])
{
    using namespace std;
    double salary = 6788.5;

    cout << dollar_to_rmb(8552.12) << endl;//58958.3  可以通过
    cout<< salary <<" doller equal to " << dollar_to_rmb(salary) << "rmb" << endl;//通过
    cout << dollar_to_rmb(11000) << endl;// 类型不同 但是可以使用临时变量的方式
    salary =  9511.12;
    cout << dollar_to_rmb(salary + 8552.12) << endl;//124528
    return 0;
}
```

常量引用可以允许如下操作： 右值引用传递
```cpp
int main(int argc, char const *argv[])
{
    using namespace std;
    double salary =  9511.12;
    double subsidy = 2000.0;
    double overtime_pay = 3581.12;

    cout<< salary + subsidy + overtime_pay <<" doller equal to " 
        << dollar_to_rmb(salary + subsidy + overtime_pay) << endl;//可以通过
    cout<< salary + 5000.12 <<" doller equal to " << dollar_to_rmb(salary + 5000.12) << endl;//可以通过
}
```

* 能用const就用const，避免无意识修改数据的编程错误！
* 使得函数可以结束 const 和 非const参数， 不然只能接受非const参数
* 使用 const 可以使函数正确生成并使用临时变量

#### [1.3 C++ 11 右值引用](#)
一个最简单判断左值、右值的方式是：等号左边的值即左值，等号右边的值即右值；但为了更加严谨，我们这样定义：能够取地址的，有名字的就是左值；反之，不能取地址，没有名字的就是右值。


* 左值： 永久对象，可被取地址，可以出现在 operator= 左侧。 典型的 lvalue：有名称的变量、函数形参（栈中的对象）等。
* 右值: 临时对象（即将销毁），不可取地址，只能出现在 operator= 右侧
```cpp
int a = 75; //a 是 左值
int b = 75 + a;; // 75 + a 会生成一个临时变量，存放在寄存器或者某个类型中，一旦完成赋值就删除 这就是右值
```

右值引用简单理解，就是绑定到左值的引用，右值引用的特点是：它将让其所绑定的右值重获新生，即使该右值本身是一个临时变量，但是它本身却不能绑定任何左值。比如：

```cpp
int &like = 6;//不能引用字面常量值
const int &number = 15; // ok 可以将一个临时对象绑定到一个const的左值引用

TypeName &val =  A(); //error
const TypeName &val = A(); //ok 可以将一个临时对象绑定到一个const左值引用

int &&ref = 6;//ok
TypeName &&val = A(); //ok
```

小例子
```cpp
#include <iostream>
using namespace std;
 
int main()
{
	int num = 10;
	//int && a = num;  //右值引用不能初始化为左值
	int && a = 10;
	a = 100;
	cout << a << endl;//输出100
 
	system("pause");
	return 0;
}
```


在C++11之前为了能够将右值（或临时对象）作为引用参数传递给函数，C++标准故意设置了这一个特例：将函数参数声明为 const Type &即可（其中 Type为具体类型）。

使用右值引用，可以如下操作了！
```cpp
double rate = 6.894;

float inline dollar_to_rmb(double&& doller) {
    return doller * rate;
}


int main(int argc, char const* argv[])
{
    using namespace std;
    double salary = 6788.5;

    cout << dollar_to_rmb(8552.12) << endl;//58958.3  可以通过
    cout << dollar_to_rmb(11000) << endl;// 类型不同 但是可以使用临时变量的方式
    cout << dollar_to_rmb(salary + 8552.12) << endl;//124528
    return 0;
}
```
但是直接给变量是不可以的
```cpp
cout << dollar_to_rmb(salary) << endl;//无法变异通过
```

#### [1.4 返回引用](#)
传统按值返回机制和按值传递函数参数类型，计算关键字return 后面的表达式，并将结果返回给调用函数，从概念上讲，这个值被复制到一个临时位置。而调用程序将使用这个值。
说明一下函数返回时，如果不是返回一个变量的引用，则一定会生成一个临时变量

```cpp
double m = sqrt(16.0);
cout << sqrt(25.0);
```
首先值 4.0被复制到一个临时位置，然后复制给m。 而第二条语句 5.0被复制到一个临时位置然后传递给cout。 也就是说传统的返回机制，是先复制到临时位置，再复制给接收值的变量/函数。

**返回引用的函数实际上返回的是被引用变量的别名！**  **要始终避免返回函数终止时不再存在的内存单元的引用。如下所示：**

```cpp
const float& changeDollar(const float& rmb) {
    float exchangeRate = 6.3147;
    float result = rmb * exchangeRate;
    return result;
}
```
result作为临时变量，已经不存在了。不应该如此返回！此处不适合返回引用。

避免方法:
* 1. 返回作为参数传递给函数的引用。
```cpp
Pointer2D& expandPoint(Pointer2D& p) {
    p.x *= 10;
    p.y *= 10;
    return p;
}

Pointer2D p = { 5.0,3.0 };
p = expandPoint(p);
cout << "x:" << p.x; //50
```
* 2. 使用new 来分配新的存储空间


用引用作函数的返回值的最大的好处是在内存中不产生返回值的副本

#### [1.5 引用占用内存问题](#)
引用是什么？ 大家都知道它就是个别名！引用的实现方式类似于常量指针(类型\* const 指针)， **每次对引用的使用实际上就是对该指针进行解引用操作(由编译器完成)。多数时候编译器能对引用进行优化，使得在运行时，引用不占额外的存储空间！**

但是，如果需要分配存储空间以实现所需要的语义，则编译器还是会给引用分配空间的即，引用类型的非静态数据成员，会增加类对象的大小


#### [1.6 一级简单指针的引用](#)
指针也是数据对象，也可以被引用！所以我们可以创建对指针的引用！ **声明对一级简单指针的引用只需要加一个&！**

```cpp
double *my_score = new double(98.56);
double *(&ref_score) = my_score;
//double * &ref_score = my_score; // 也可以

std::cout << *ref_score << "\n"; //98.56
delete my_score;
```

#### [1.7 一维数组的引用](#)
**需要使用到括号**  int & val[10] 这种是错误的，因为不能声明引用数组！ 

```cpp
int s1[] = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
int (&val)[10] = s1;
// int & val[10] = s1; 错误
for (auto &&i : val)
{
    printf("%d ", i);
}
printf("\n"); //5 7 4 2 6 1 9 0 3 3 
```

#### [1.8 一维指针数组的引用](#)
指向一维指针数组！ **和 1.7 相比只需要加一个 \*号**

```cpp
int a = 10, b = 20,c = 30, d= 40;
int *val[4] = {&a, &b, &c, &d};

int * (&refVal)[4] = val;

for (auto &&i : refVal)
{
    std::cout  << *i << "\n";
}//10 20 30 40
```

#### [1.9 二维数组引用](#)
**使用括号 加一个 & 就可以，很简单的！**

```cpp
int matirx[5][5] = {
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,0,1,0,0},
    {1,0,0,1,0},
    {1,0,0,0,1}
};

int (&refMatirx)[5][5] = matirx;

for (int i = 0; i < 5; i++)
{
    for (int j = 0; j < 5; j++)
    {
        printf("%d ", refMatirx[i][j]);
    }
    printf("\n");
}
```

#### [1.10 引用指向二维数组指针](#)
**引用一个二维数组指针** **括号 加上 & ！**

```cpp
int matirx[5][5] = {
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,0,1,0,0},
    {1,0,0,1,0},
    {1,0,0,0,1}
};

int (*matirx_pointer)[5] = matirx;

int (*(&ref_matirx_pointer))[5] = matirx_pointer;

for (int i = 0; i < 5; i++)
{
    for (int j = 0; j < 5; j++)
    {
        printf("%d ", *(*(ref_matirx_pointer + i) + j));
    }
    printf("\n");
}
```


### [2. C++ 11 右值引用](#)
传统的C++引用（现在称为左值引用）使得标识符关联到左值）。左值是一个表示数据的表达式（如变量名或解除引用的指针），程序可获取其地址。
**C++11新增了右值引用，使用&&表示，右值引用可关联到右值。** 右值即可出现在赋值表达式右边，但不能对其应用地址运算符的值。

```cpp
int &&val = 1.01;

std::cout << val << "\n"; //1
val = 12;
std::cout << val << "\n"; //12
```

右值包括字面常量（C-风格字符串除外，它表示地址）、诸如x+y等表达式以及返回值的函数（条件是该函数返回的不是引用）。
```cpp
int x = 10;
int y = 23;
int && r1 = 13;
int && r2 = x + y;
double && r3 = std::sqrt(2.0);
```
有趣的是，将右值关联到右值引用导致该右值被存储到特定的位置，且可以获取该位置的地址。

即虽然不能将运算符&用于13，但可将其用于r1，通过将数据与特定的地址关联，使得可以通过右值引用来访问该数据。

引入右值引用的主要目的之一是 **实现移动语义** 。

#### [2.1 为何需要移动语义](#)
下面看看没有移动语义的情况，假设有一个包含20000个string的vector，且string都包含1000个字符，当想要复制这个vector的时候，工作量势必会很大，此时如果有这样一个函数：

```cpp
vector<string> allcaps(const vector<string> & vs)
{
    vector<string> temp(vs);
    return temp;
}
// 进行复制
vector<string> vstr; // a vector of 20000 strings, each of 1000 characters
/* .. */

vector<string> vster_copy1(vstr);          // #1
vector<string> vster_copy2(allcaps(vstr)); // #2
```
第二个复制中，allcaps()函数在创建完temp后，会先将temp复制给一个临时返回对象，删除temp，将临时返回对象复制给vstr_copy2后，删除临时返回对象，这相当于调用了3次复制构造函数。

> 如果编译器会进行优化，可能不会产生临时对象，这种技术叫作RVO优化，可看下一节说明。此时就是调用了２次，即没有temp复制给临时变量这一步，或者说temp本身就被当作临时变量了

如果能将临时对象的所有权直接转让给vstr_copy2，可能会更好。即不删除临时对象，其内容还在原来的地方，而直接将vstr_copy2与之关联。

这类似于在计算机中移动文件的情形：实际文件还留在原来的地方，而只修改记录。这种方法被称为移动语义（move semantics）。有点悖论的是，**移动语义实际上避免了移动原始数据，而只是修改了记录。** 移交了控制权

#### [2.2 返回对象不一定都会调用复制构造函数](#)
为避免对临时对象进行不必要的拷贝，C++编译器常使用一种名为Copy Ellision（拷贝去除）的优化技术,同时，还需要知道一个技术：RVO（Return Value Optimization）。RVO是一种编译器优化技术，可以把通过函数返回创建的临时对象给”去掉”，然后可以达到少调用拷贝构造的操作目的，