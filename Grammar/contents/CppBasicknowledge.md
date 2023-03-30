### [C++ 基础知识](#)
`在正式开始语法学习之前，让我们来了解一些基础知识！`

---
- [ ] [1. main 函数说明](#1-main-函数说明)
- [ ] [2. 注释问题](#2-注释问题)
- [ ] [3. 头文件和命名空间](#3-头文件和命名空间)
- [ ] [4. 赋值声明语句](#4-赋值声明语句)
- [ ] [5. cin cout](#5-cin-cout)
- [ ] [6. cpp 程序创建过程](#6-cpp-程序创建过程)

---

[C语言基本内容：https://gitee.com/cicer/PL.Learning/tree/master/c](https://gitee.com/cicer/PL.Learning/tree/master/c)

### [1. main 函数说明](#)
**CPP**语言的 **main**，可以有几种写法，带参数，不带参数，带返回值，不带返回。**C++** 的运行都是从 **main** 函数开始的，但是对于动态库 **DLL** 可能不需要 **main** 函数！ 有些单片机环境也不需要 **main** 函数！

```cpp
int main(int argc, char *argv[]){
    return 0;
}

//如果函数不带参数，那么推荐加上 void
void main(void){
    
}

int mian(){
    return 0;
}
```

**标准格式:** `在C++，为了防止一些系统差异导致的不能工作, 不推荐` `void main()` `void main(void).` `除非是在自己写的玩具程序上面，我们推荐使用如下格式`
```cpp
int main(int argc, char *argv[]){
    /*
        其他语法！
    */
    return 0;
}
```


#### [1.1 返回值问题](#)
在大多数系统中，**main**的返回值都被用来指示状态。返回 `0` 表示成功，非 `0` 的返回值的含义由系统定义，通常用来支持错误类型!

```cpp
int mian(){
    
    return 0; //一切正常
}
```

#### [1.2 调用Cpp语言的参数问题](#)
`int args, char * argv[]` 我们可以理解下这两个参数的意思是什么，前者表示输入参数的个数，后者表述输入的字符串数组！
```cpp
#include<iostream>
  
int main(int args, char * argv[]){
  std::cout<<"hello ubuntu"<<std::endl;
  std::cout<<args<<std::endl;

  for(int i =0; i < args; i++){
      printf("%s \n", argv[i]);
  }
  return 0;
};
```
`编译它`：`g++ remix.cpp -o remix`

`我们如此调用程序` : `./remix 65 asdasd 7454` `输出如下`
```cpp
hello ubuntu
4
./remix 
65 
asdasd 
7454 
```


### [2. 注释问题](#)
参与的注释方法和C语言一样，单行注释用 `//` 多行注释用 `/**/`
```cpp
// 单行注释

/* 多行注释 */
```

### [3. 头文件和命名空间](#)
**C++** 比较奇特哈，又有头文件又有命名空间。在 `C++`中，引入头文件约定为不带扩展名 `.h`的方式` 
`#include` 该编译指令，会使得预处理器将iostream文件的内容添加到程序中。这是一种典型的预处理操作，在源代码被编译之前，替换或添加文本！

```cpp
#include <iostream> //CPP输入输出的库
using namespace std;
```

|头文件类型|约定|实例|说明|
|:---|:---|:---|:---|
|C++旧风格|.h结尾|iostream.h|C++可以使用|
|C++新风格|无扩展名|iostream|推荐使用，C++支持，使用 namespace std|
|转换后的c|加上前缀c,无扩展名|cmath|C++可以使用，可以使用不是c的特性，如namespace std|

#### [3.1 命名空间](#)
真实奇葩的存在，有的编程语言只有头文件 **[Node,Python]**，有的只有命名空间 **[.NET]**, C++为了兼容C语言, 让程序显得更加复杂，两个都有,真棒。 如果使用 **iostream**，而
不是 **iostream.h**，则应该使用下面的名称空间编译指令来使 **iostream** 中的定义对程序可用。

请不要有怨言，你只需要懂一件事情就可以了，**那就是不能靠头文件区分同名同参同返回的不同的方法**，如果 一个方法名称为 **int kicker()** 在头文件 **remix.h** 中。但是另一个头文件
**link.h** 中也有一个方法 **int kicker()** 。

如果你同时 **include** 这两个头文件，那么系统就会报错！为什么？你可能觉得不是在不同的头文件中吗？ 你要知道头文件只是预处理指令，它只是告诉预处理器在源代码被编译器编译前，实现文本替换修改！
它不区分不同的作用域！他只是让预处理器将 **#include<头文件>** 这句文本替换成 这个头文件中的源代码文本！

```cpp
using namespace std;
```
这叫做using编译指令，命名空间支持，是为了防止多个厂商现有的代码组合起来的程序时更容易。例如，两个厂商都写了work函数

```cpp
MicroSoft::work();
HuaWei::work();
```
按照这种方式，类、函数和变量便是C++编译器的标准组件，他们现在都被放置在命名空间std中。仅当头文件没有扩展h时，情况才是如此。这意味着在iostream中定义的用于输出的cout变量实际上是 std:cout,而endl实际上是std:endl。
因此，可以省略编译指令using，如此执行。
```cpp
std::cout<< "Hello Wrold"<<std::endl;
```

如果只想使用单个命名，也可以如此使用， 当有多个命名空间的时候，只推荐这样使用哦。
```cpp
using std::cout;
using std::endl;
using std::cin;

cout<< "Hello Wrold" <<endl;
```
**<<**:是一个重载了的运算符。将输出当做流来操作。 
endl是一个特殊的C++符号，表示一个重要的概念，重启一行。在输出流中插入endl将导致屏幕光标移动到下一行开头。打印的时候cout不会自动移动到下一行，下面两者是等价的。
```cpp
cout<< "Hello Wrold"<<endl;
cout<< "Hello Wrold \n";
```

#### [3.2 hpp和h](#)
**.hpp**，本质就是将**.cpp**的实现代码混入.h头文件当中，定义与实现都包含在同一文件，则该类的调用者只需要 **include该.hpp** 文件即可，无需再将cpp加入到project中进行编译。而实现代码将直接编译到调用者的obj文件中，不再生成单独的obj，
采用hpp将大幅度减少调用`project`中的cpp文件数与编译次数，也不用再发布lib与dll文件，因此非常适合用来编写公用的开源库。

hpp的优点不少，但是编写中有以下几点要注意：
1. 是`Header Plus Plus`的简写。（.h和.hpp就如同.c和.cpp似的）
2. 与`.h`类似，`.hpp`是`C++`程序头文件格式。
3. 是VCL专用的头文件,已预编译。
4. **是一般模板类的头文件。**
5. 一般来说，`.h`里面只有声明，没有实现，而`.hpp`里声明实现都有，后者可以减少`.cpp`的数量。
6. **.h里面可以有using namespace std，而.hpp里则无。**
7. **不可包含全局对象和全局函数。**

由于.hpp本质上是作为.h被调用者include的，所以当hpp文件中存在全局对象或者全局函数，而该hpp被多个调用者include时，将在链接时导致符号重定义错误。要避免这种情况，需要去除全局对象，将全局函数封装为类的静态方法。


### [4. 赋值声明语句](#)
和C语言一样的。 可以连续赋值，常量一旦定义就必须赋值初始化，并且无法重复赋值！变量应该在定义的时候就初始化！赋值有几种情况。

1. 声明后就赋值叫初始化： int age = 18;
2. 字面量赋值: int year = 1990; 调用类型的构造函数。
3. 使用其他变量来赋值: int era = year;` `当用一个对象去初始化同类的另一个对象时，会引发复制构造函数被调用。
4. 重复赋值: era = age; 将会调用对象的赋值运算符
5. 表达式赋值: int val = era + age + 10;
6. 圆括号赋值: int count(20);` `暂时没搞清楚它调用什么构造函数 还是 运算符 轻易别使用
7. 大括号赋值: int kicker = {312};  int jx {312}; 暂时没搞清楚它调用什么构造函数 还是 运算符 轻易别使用

赋值的时候是值类型就不出触发什么构造函数，复制构造函数,赋值运算符，一旦类型是class 就要多家考虑了！

```cpp
#include <iostream>
using namespace std;

int main(int argc, char const* argv[])
{
    int year(18);

    int year_bytes = sizeof year;
    int year_bytes_ = sizeof(year);
    int int_byte = sizeof(int);
    cout<< year_bytes << " " << year_bytes_ << " " << int_byte << endl;

    int val(year);
    int kicker = {312};
    int jx {312};
    cout << "val:" << val << endl;
    cout << "kicker:" << kicker << endl;
    cout << "jx:" << jx << endl;
    return 0;
}
```

```cpp
#include <iostream>
using namespace std;

int main(int argc, char const* argv[])
{
    int age = 32;
    int less = 10;
    age = age - less;
    string name = "jxkicker";
    cout << "Hello World! I am " << name;
}
```

声明时直接大括号初始化：
```cpp
vector<int> p={1,2,3};
```
这直接调用相应的构造函数。

声明之后再赋值：
```cpp
vector<int> p;
p={1,2,3};
```
先调用默认构造函数，再调用拷贝构造函数（赋值运算重载符）。

### [5. cin cout](#)
`cout` 实际上是一个 `iostream` 类的对象，每次调用的时候就会输出对应的字符串，调用的实际上就是成员运算符函数 `operator<<`

`cin.get()` 语句。可以等待一个字符的输入.输入后按enter 读取输入。cin是一个输入流，使用 >> 运算符从输入流中抽取字符，它是一个很只能的对象，他可以将通过键盘输入的一系列字符转换为接收信息的变量能够接收的形式。

endl实际上是一个操纵符，不仅仅实现了换行的操作，而且还对输出缓冲区进行刷新。 `std::endl` 这个东西, 可以说等价于 `<< '\n' << flush`

`还是推荐使用printf来输出，因为它是线程安全的`
```cpp
#include <iostream>
using namespace std;

int main(int argc, char const* argv[])
{
    int age = 0;

    string name;

    cout << "please input your age: " << endl;
    cin >> age;
    cout << "age: " << age << endl;
    cout << "please input your name: " << endl;
    cin >> name;
    cout << "name: " << name << endl;

}
```

**std::cout** 会把输出放到一个叫缓冲区(stream buffer) 里, 直到缓冲区满了才会清空缓冲区并把字串输出到 stdout 之类的输出流, 这也就是为什么关闭了缓冲区可能会出现错误。

那么说到这里也就整明白了, 如果频繁使用 std::endl 来做换行输出, 那便是破坏了 std::cout 本身的类似于优化的操作, 也就导致了通常认为 std::cout 的速度比 printf 慢。

添加了**ios::sync_with_stdio(false)**; 关闭同步之后，cout输出的速度相对而言更加快速。

```cpp
#include "cstdio"
#include "ctime"
#include "iostream"
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    clock_t start, end;
    start = clock();

    for (long long i = 1e+15; i < (1e+15) + 10000; i++) {
        // printf("%lld\n", i);  // 2875
        // printf("%lld", i);  // 790
        //  cout<<i; //147
        // cout<<i<<endl; //2832
        cout << i << "\n";  // 745
    }

    end = clock();
    cout << endl;
    cout << end - start << endl;
}
```

### [6. cpp 程序创建过程](#)
用C++ 编成一个程序需要以下一个流程

```shell
 [ 源代码 ]
    |
    ↓ 
 [ 编译器 ]
    |
    ↓
[ 目标代码 ]
    |            
    ↓         ↙--  [ 启动代码  ]   
[ 链接程序 ]  ←----  [ 代码库 ]
    |
    ↓
[ 可执行代码 ] 
```


`源代码` `--- 进过编译器 编译 --->`  `目标代码`  `---链接程序 将启动代码和库代码连接起来 --->`  `可执行代码`

`C++ 的编译器有很多种，并且在win系统和 linux 系统中也有所差别！`

#### [6.1 编译过程](#)
以linux系统下的g++编译过错为例：

1. **预处理-Pre-Processing**  生成 `.i` 文件
```shell
# -E 选项指示编译器仅对输入文件进行预处理
g++ -E test.cpp -o test.i
```
2. **编译-Compiling**  生成 `.s` 文件
```shell
# -S 编译选项告诉 g++ 在为 C++ 代码产生了汇编语言文件后停止编译
# g++ 产生的汇编语言文件的缺省扩展名是 .s
g++ -S test.i -o test.s
```
3. **汇编-Assembling** 生成`.o`文件
```shell
# -c 选项告诉 g++ 仅把源代码编译为机器语言的目标代码
# 缺省时 g++ 建立的目标代码文件有一个 .o 的扩展名。
g++ -c test.s -o test.o
```
4. **链接-Linking** 生成可执行文件
```shell
# -o 编译选项来为将产生的可执行文件用指定的文件名
g++ test.o -o test
```