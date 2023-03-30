### [C++ IO处理框架](#)
I/O概念框架：流、缓冲区、文件！

-----
- [x] [1. 输入输出概述](#1-输入输出概述)
- [x] [2. cout进行输出](#2-cout进行输出)
- [x] [3. cin进行输入](#3-cin进行输入)

-----

### [1. 输入输出概述](#)
对于一个程序而言，输入的来源可以是屏幕，文件，网络，其他程序，输出的地方也可以是屏幕，文件，网络，其他程序，甚至是内存单元！
**C++不关心源头和去处，而把输入和输出看作字节流。**

* 输入时，程序从输入流中抽取字节；输入流中的字节可能来自键盘、存储设备（如硬盘）或其他程序。 **将输入源与流链接，将流与程序链接**
* 输出时，程序将字节插入到输出流中；输出流中的字节可以流向屏幕、打印机、存储设备或其他程序。 **将输出源与程序链接，将一个输出目标与流链接**

**流充当了程序和流源或流目标之间的桥梁。** 这使得C++程序可以以相同的方式对地来自键盘的输入和来自文件的输入，C++程序只是检查字节流，而不需要知道字节来自何方；C++处理输出的方式也将独立于其去向。


> **你可以将流类/对象 理解为水管，I/O的过程就是用水管接水，送水的过程！**

#### [1.1 缓冲区](#)
通常，通过使用缓冲区可以更高效地处理输入和输出。缓冲区是用作中介地内存块，它是将信息从设备传输到程序或从程序传输给设备地临时存储工具。

> **你可以将缓冲区理解为水库！**

**导致缓冲刷新 (即，数据真正写到输出设备或文件) 的原因有很多：**
* 程序正常结束，作为 main 函数的 return 操作的一部分，缓冲刷新被执行。
* 缓冲区满时，需要刷新缓冲，而后新的数据才能继续写入缓冲区
* 可以使用操纵符如 endl 或者 flush 来显示刷新缓冲区
* 在每个输出操作之后，可以用操纵符 unitbuf 设置流的内部状态，来清空缓冲区。默认情况下，对 cerr 是设置 unitbuf 的，因此写到 cerr 的内容都是立即刷新的
* 一个输出流可能被关联到另一个流。在这种情况下，当读写被关联的流时，关联到的流的缓冲区会被刷新。例如，默认情况下，cin 和 cerr 都关联到 cout。因此，读 cin 或写 cerr 都会导致 cout 的缓冲区被刷新。

```cpp
using std::cout; 
using std::endl; 
using std::flush; 

cout << "hello world" << endl;
cout << "hello world" << flush;
cout << "hello world" << ends; //ends 为追加一个空字符，然后刷新缓冲区！
```

[**如果程序崩溃，输出缓冲区不会刷新。当一个程序崩溃后，它所输出的数据很可能停留在输出缓冲区中等待打印！**](#)

##### [1.1.1 从磁盘文件读写的情况](#)
通常，像磁盘驱动器这样的设备以512字节（或更多）的块为单位来传输的块为单位来传输信息，而程序通常每次只能处理一个字节的信息。缓冲区帮助匹配这两种不同的信息传输速率。
假设程序要计算记录在硬盘文件中的金额。如果程序直接从文件中一个一个读取字符，处理一个字符后，再从文件中读取下一个字符，再处理。以此类推。

但是从磁盘文件中每次读取一个字符需要大量的硬件活动，速度非常慢。

缓冲方法则从磁盘上读取大量信息，将这些信息存储在缓冲区中，然后程序每次从缓冲区里读取一个字节。因为从内存中读取单个字节的速度比硬盘上读取快很多，所以这种方式更快，也更方便。
当然，到达缓冲区尾部后，程序将继续从磁盘上读取另一块数据。

**输出**时，程序首先填满缓冲区，然后把整块数据传输到硬盘，并清空缓冲区，以备下一批输出使用。 **这被称为刷新缓冲区（flushing the buffer）。**

##### [1.1.2 从键盘读的情况](#)
键盘输入每次提供一个字符，因此在这种情况下，程序无需缓冲区来帮助匹配不同的数据传输速率。

然而，对键盘输入进行缓冲可以让用户在将输入传输给程序之前返回并更正。
* C++程序通常在用户按下 **回车键** 时候刷新输入缓冲区。
* 对于屏幕输出，C++程序通常在用户发送**换行符** 时刷新输出缓冲区。

程序也可能会在其他情况下刷新输入，例如输入即将到来时，这取决于实现。也就是说，当程序到达输入语句时，它将刷新输出缓冲区中当前所有的输出（与ANSI C一致的C++实现是这样工作的）。


#### [1.2 基于流的 I/O](#)
基于流的输入/输出库围绕抽象的输入/输出设备组织。这些抽象设备允许相同代码处理对文件、内存流或随即进行任意操作（例如压缩）的自定义适配器设备的输入/输出。
大多数已经被类模板化，故它们能被适配到任何标准字符类型

<img src="./imgs/std-io-complete-inheritance.svg">

C++98 C++版本定义了一些模板类，以支持char和wchar_t 后来C++11 添加了char16_t和char32_t具体化！ 通过typedef工具，使得模板具体化能够模仿传统的非模板I/O实现！
* istream、ostream都是char具体化的 typedef
* wistream、wostream都是wchar_t具体化的 typedef **wcout对象用于输出宽字符流！**

**管理流和缓冲区的工作有点复杂，但iostream文件中包含一些专门设计用来实现、管理流和缓冲区的类。**

**流对象不能拷贝或赋值**

* streambuf类为缓冲区提供了内存，并提供了用于填充缓冲区、访问缓冲区的内容、刷新缓冲区和管理缓冲区内存的类方法；
* ios_base类表示流的一般特征，如是否可读取、是二进制流还是文本流等；
* ios类基于ios_base，其中包括了一个指向streambuf对象的指针成员； 一般流属性！
* ostream类是从ios类派生而来的，提供了输出方法；
* istream类也是从ios类派生而来的，提供了输入方法；
* iostream类是基于istream和ostream类的，因此继承了输入方法和输出方法。

|头文件|拥有类型|
|:---|:---|
|[**iostream：**](#)|istream，wistream (从流读取数据)；ostream，wostream (向流写入数据)；iostream，wiostream (读写流) |
|[**fstream：**](#)|ifstream，wiftream (向文件读取数据)；ofstream，wofstream (向文件写入数据)；fstream，wfstream (读写文件)|
|[**sstream：**](#)|istringstream，wistringstream (从 string 读取数据)；ostringstream，wostringstream (向 string 写入数据)；stringstream，wstringstream (读写 string)|



#### [1.3 内置的8个流对象](#)
C++ 的 iostream 类库管理了很多细节。例如，在程序中包含 iostream 文件将自动创建8个流对象（4个用于窄字符流，4个用于宽字符流）。

* **cin** 对象对应于标准输入流。在默认情况下，这个流被关联到标准输入设备（通常为键盘）。wcin对象与此类似，但处理的是wchar_t类型。
* **cout** 对象与标准输出流相对应。在默认情况下，这个流被关联到标准输出设备（通常为显示器）。wcout对象与此类似，但处理的是wchar_t类型。
* **cerr** 对象与标准错误流相对应，可用于显示错误消息。在默认情况下，这个流被关联到标准输出设备（通常为显示器）。这个流没有被缓冲，这意味着信息将被直接发送给屏幕，而不会等到缓冲区填满或新的换行符。werr对象与此相似，但处理的是wchar_t类型。
* **clog** 对象也对应着标准错误流。在默认情况下，这个流被关联到标准输出设备（通常为显示器）。这个流被缓冲。wclog对象与此相似，但处理的是wchar_t类型。
* **wcin,wcout,wcerr,wclog** 和上面一样，不过处理的是宽字符！


#### [1.4 IO流的条件状态（iostate）](#)
IO操作可能发生错误。一些错误可恢复，而其他的则发生在系统深处，已经超出了应用程序可修正的范围。IO 库的条件状态：(注：strm 表示一种 IO 类型)!

|属性、方法|说明|
|:---|:---|
|strm::iostate|iostate 是一种机器相关的类型，提供了表达条件状态的完整功能|
|strm::badbit|用来指出流已崩溃，系统级不可恢复错误！|
|strm::failbit|用来指出一个 IO 操作失败了，如希望读取数字读取了字母，或者已到文件尾部|
|strm::eofbit|用来指出流已经到达文件结束，已到文件尾部|
|strm::goodbit|用来指出流未处于错误状态。此值保证为 0|
|s.eof()|若流 s 的 eofbit 置位，返回 true|
|s.fail()|若流 s 的 failbit 或 badbit 置位，返回 true|
|s.bad()|与上同理|
|s.good()|与上同理|
|s.clear()|将流 s 中所有条件状态复位，将流的状态设置为有效。返回 void|
|s.clear(flags)|根据给定的 flags 标志位，将流 s 中对应条件状态位复位。flags 类型为// strm::iostate。返回 void|
|s.setstate(flags);|根据给定的 flags 标志位，将流 s 中对应条件状态位置位。flags 类型为// strm::iostate。返回 void|
|s.rdstate();|返回流 s 的当前条件状态，返回类型为 strm::iostate|


* eofbit：001
* failbit：010
* badbit：100
* goodbit：000

[**IO流对象支持转换为bool值，如果流处于错误状态，返回false，否则返回true。**](#)

```cpp
int number;
while(std::cin >> number){
    // ... 读取成功
    printf("number: %d \n", number);
}
```

**管理流状态**
```cpp
auto old_state = cin.rdstate();		// 记住 cin 的当前状态
cin.clear();						// 使 cin 有效
process_input(cin);					// 使用 cin
cin.setstate(old_state);			// 将 cin 置位原有状态
```

#### [1.5 关联输入和输出流](#)
当一个输入流被关联到输出流时，任何试图从输入流读取数据的操作都会先刷新关联的输出流。标准库将 cin 和 cout 关联在一起，因此下面语句。
```cpp
cin >> val;
```
导致 cout 的缓冲区刷新。交互式系统通常应该关联输入流和输出流。这意味着所有输出，包括用户提示信息，都会在读操作之前被打印出来。IO流有一个 **cin.tie** 方法

* **cin.tie()** 返回指向输出流的指针、若无关联则返回空指针
* **cin.tie(&ostream)** 接受一个指向 ostream的指针，将自己关联到次ostream。
```cpp
cin.tie(&cout);				// 仅仅用来展示：将 cin 和 cout 关联在一起
                            // old_tie 指向当前关联到 cin 的流 (如果有的话)
ostream *old_tie = cin.tie(nullptr);		// cin 不再与其他流关联
cin.tie(&cerr);				// 将 cin 与 cerr 关联；cin 应该关联到 cout，这不是一个好主意
cin.tie(old_tie);			// 重建 cin 与 cout 的关联
```
**感觉功能很鸡肋！**


### [2. cout进行输出](#) 
C++ 将输入看做字节流(根据实现和OS的不同，有可能四8位、16位、32位！)， 程序中很多数据本身是比字节更大的单位。例如 int类型 16~32位 double类型64位！
**ostream类最重要的任务之一是将数值类型转换为以文本形式表示的字符流。**  为执行这些转换任务，ostream提供了多个类方法! cout 就是一个ostream对象！

#### [2.1 重载的<<运算符](#)
在cout中，<<运算符被称为插入（insertion）运算符，而它在C++中默认含义是按位左移运算符，主要是因为其外观（像向左流动的信息流）而或者这种新角色。

插入运算符被重载，使之能够识别C++中所有的基本类型：unsigned char、signed char、char、short、unsigned short、int、unsigned int、long、unsigned long、long long(C++11)、unsigned long long(C++11)、float、double、long double。

对于这些类型，重载运算符的声明为： 在类中 我们一般使用友元函数重载 << 
```cpp
ostream & operator<<(int){

};
//or
friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
	os << "x: "<< std::to_string(v._x) << " y:" + std::to_string(v._y);
	return os;
}
```
其中返回引用值能够使得插入运算符连续使用。


#### [2.2 输出指针](#)
还需要注意几种指针的输出，ostream类还为下面的指针类型定义了插入运算符函数：

* const signed char*；

  const unsigned char*；

* const char*；

* void*

前3个表明，cout会将指向char的指针默认认定为字符串，因为C++用指向字符串存储位置的指针来表示字符串，指针的形式可以是char数组名、显式char指针或用引号括起的字符串。下面的语句都将显式字符串

```cpp
char name[20] = "Dudly Diddlemore";
char * pn = "Violet D'Amore";
cout << "Hello!";
cout << name;
cout << pn;
```
方法使用字符串中的终止空字符来确定何时停止显示字符串

对于其他类型的指针， **C++将其对应于void*，并打印地址的数值表示。** 所以，如果要获得字符串的地址，则必须先将其强制转换为其他类型：

```cpp
char * amount = "dozen";
cout << amount;          // prints the string "dozen"
cout << (void *) amount; // prints the address of the "dozen" string
```

#### [2.3 put()方法和write()方法](#)
put() 方法用于显示字符，和e运算符一样，该函数也返回一个指向调用对象的引用，因此可以用它进行拼接输出：

```cpp
cout.put('I').put('t');
```
put()会将数值型参数自动转换为正确的char值，如put(int)、put(double)，并显示对应的字符。

> C++2.0之前，put()可很好地派上用场，因为那些版本中，C++用int值表示字符常量，即cout << ‘W’;这个语句会输出整数87。 write()方法显示整个字符串，其模板原型如下：
```cpp
basic_ostream<charT, traits>& write(const char_type* s, streamsize n);
// 第一个参数提供了要显示的字符串的地址
// 第二个参数指出要显示多少个字符
// 使用cout调用write()时，将调用char具体化，因此返回类型为ostream &
```
需要注意的是，write()方法并不会在遇到空字符时自动停止打印字符，而只是打印指定数目的字符，即使超出了字符串的变界。

write()为将数值数据存储在文件中提供了一种简洁、准确的方法。

#### [2.4 刷新输出缓冲区](#)
ostream类对cout对象处理的输出进行缓冲，所以输出不会立即发送到目标地址，而是被存储在缓冲区中，直到缓冲区填满。然后，程序将 **刷新（flush）缓冲区** ，把内容发送出去，并清空缓冲区，以存储新的数据。

> **通常缓冲区为512字节或其整数倍。**

**当标准输出连接的是硬盘上的文件时** 缓冲可以节省大量的时间，毕竟不希望程序为发送512个字节，而存取磁盘512次。而是将512个字节收集到缓冲区中，然后一次性将它们写入硬盘，这样效率要高很多。

**但对于屏幕输出来说，填充缓冲区的重要性要低很多** 。所幸的是，在屏幕输出时，程序不必等到缓冲区被填满。例如，将换行符发送到缓冲区后，将刷新缓冲区。另外，多数C++实现都会在输入即将发生时刷新缓冲区，即下面这段代码：

```cpp
cout << "Enter a number: ";
float num;
cin >> num;
```
即使这里的输出字符串中没有换行符，程序也会立刻显示cout消息。如果没有这种特性，程序将等待输入，而无法通过cout消息来提示用户。

如果实现不能在所希望时刷新输出，可以使用两个控制符中的一个来强行进行刷新：
* 控制符 **flush** 刷新缓冲区；
* 控制符 **endl** 刷新缓冲区，并插入一个换行符。
* 控制符 **ends** 刷新缓冲区，并插入一个空白符。

```cpp
cout << "Hello, good-looking!" << flush;
cout << "Wait just a moment, please." << endl;
// 控制符也是函数，可以直接调用flush()来刷新cout缓冲区
flush(cout);
// ostream类对<<插入运算符进行了重载，cout << flush;将被替换为函数调用flush(cout);
```

```cpp
using std::cout; 
using std::endl; 
using std::flush; 

cout << "hello world" << endl;
cout << "hello world" << flush;
cout << "hello world" << ends; //ends 为追加一个空字符，然后刷新缓冲区！
```

##### [2.4.1 unitbuf 操纵符](#)
如果想在每次输出后都刷新缓冲区，我们可以使用 **unitbuf** 操纵符。它告诉流在接下来的每次写操作之后都进行一次 flush 操作。而 **nounitbuf** 操纵符则重置流，使其恢复使用正常的系统管理的缓冲区刷新机制。

```cpp
cout << unitbuf;		// 所有输出操作后都会立即刷新缓冲区
                        // 任何输出都立即刷新，无缓冲
cout << nounitbuf;		// 回到正常的缓冲方式
```
#### [2.5 用cout进行格式化](#)
**默认情况下的显示** ,ostream插入运算符 **将值转换为文本格式** 。在默认情况下，格式化值的方式如下：

* 对于char值，如果它代表的是可打印字符，则将被作为一个字符显示在宽度为一个字符的字段中。
* 对于数值整型，将以十进制方式下显示在一个刚好容纳该数字及负号（如果有的话）的字段中。
* 字符串被显示在宽度等于该字符串长度的字段中。
* 浮点类型显示为6位，末尾的0不显示（注意，显示的数字位数与数字被存储时精度没有关系）。数字以定点表示法显示还是以科学计数法表示取决于它的值，当指数大于等于6或小于等于-5时，将使用科学计数法表示。另外，字段宽度恰好容纳数字和负号（如果有的话）。

ostream 类的成员方法
|成员函数|说明|
|:----|:-----|
|flags(fmtfl)|当前格式状态全部替换为 fmtfl。注意，fmtfl 可以表示一种格式，也可以表示多种格式。|
|precision(n)|设置输出浮点数的精度为 n。|
|width(w)|指定输出宽度为 w 个字符。|
|fill(c)|在指定输出宽度的情况下，输出的宽度不足时用字符 c 填充（默认情况是用空格填充）。|
|setf(fmtfl, mask)|在当前格式的基础上，追加 fmtfl 格式，并删除 mask 格式。其中，mask 参数可以省略。|
|unsetf(mask)|在当前格式的基础上，删除 mask 格式。|

##### 2.5.1 调整进制
ostream 类是ios类派生而来的，而ios类是从ios_base类派生而来的。ios_base类存储了描述格式状态的信息。

通过使用 **控制符（manipulator）** 可以控制显示整数时使用的计数系统。通过使用ios_base的成员函数，可以控制字段宽度和小数位数。

可以使用dec、hex和oct控制符，控制整数以十进制（decimal）、十六进制（hexadecimal）还是八进制（octal）显示
```cpp
int val = 5689;
std::cout << std::hex << val << std::flush;
//1639
printf("\n");
return 0;
```
也可以直接调用函数

```cpp
int val = 5689;
std::hex(std::cout);
std::cout << val << std::flush;
//1639
printf("\n");
```

##### 2.5.2 调整字段宽度
可以使用width成员函数将长度不同的数字放到宽度相同的字段中，该方法的原型为：

```cpp
int width(); // 返回字段宽度当前设置
int width(int i); // 将字段宽度设置为i个空格，并返回以前的字段宽度值
                  // 这使得能够保存以前的值，以便以后回复宽度值时使用
```
**width()** 方法只影响将显示的下一个项目，然后字段宽度将恢复为默认值。如下所示：

```cpp
cout.width(5);
cout << "N" <<':';
cout.width(8);
cout << "N * N" << ":\\n";
// Output:
    N:   N * N:
```
这里需要注意：
* 输出的值在字段中右对齐（默认的）；
* cout通过加入空格来填满整个字段，用来填充的字符叫做填充字符（fill character）。

##### 2.5.3 填充字符
默认情况下，cout用空格填充字段中未被使用的部分，可以用fill()成员函数来改变填充字符。如下所示：
```cpp
cout.fill('*');
```
这对于检查打印结果，防止接收方添加数字很有用。

>️️ ️**注意，与字段宽度不同的是，新的填充字符将一直有效，直到更改它为止。**


```cpp
int main(int argc, char const* argv[])
{   
  int val = 5689;
  std::hex(std::cout); //修改进制
  std::cout.width(10); //显示宽度
  std::cout.fill('*'); //填充字符
  std::cout << val << std::flush;
  //******1639
  printf("\n");
  return 0;
}
```
##### 2.5.4 设置浮点数的显示精度
浮点数精度的含义取决于输出模式。在默认模式下，它指的是显示的总位数。在定点模式和科学模式下，精度指的是小数点后面的位数。

C++的默认精度为6位（但末尾的0将不显示）。 **precision()** 成员函数使得能够选择其他值。调用方式如下：
```cpp
cout.precision(2);
```
与fill()相似，新的精度设置将一直有效，直到被重新设置

##### 2.5.5 打印末尾的0和小数点
对于有些输出（如价格），保留末尾的0将更美观。可以使用如下调用：
```cpp
cout.setf(ios_base::showpoint);
```
其中，showpoint是ios_base类声明中定义的类级静态常量，所以在类外使用时，需要加上作用域运算符（::）。


```cpp
int main(int argc, char const* argv[])
{   
  int val = 5689; double score = 98.5614;

  std::hex(std::cout); //修改进制
  std::cout.width(10); //显示宽度
  std::cout.fill('*'); //填充字符
  std::cout << val << std::endl;
  //******1639
  std::cout.width(10); //显示宽度
  std::cout.precision(9);
  std::cout.setf(std::ios_base::showpoint);
  std::cout << score << std::endl;
  //98.5614000

  return 0;
}
```

##### 2.5.6 控制布尔值的格式

默认输出布尔值打印是0和1。通过控制符**boolalpha**操作符返回字符串形式 false和true。**noboolalpha**操作符可以取消这个设置！

```cpp
bool sex = true;
std::cout << std::boolalpha << sex << std::endl;//true
```

##### 2.5.7 科学计数法

```cpp
double meony_i_will_get = 17500000.1425;
unsigned int eyes_left_healthy_year = 35;
std::cout.setf(std::ios::scientific);

std::cout << meony_i_will_get << std::endl;
//1.750000e+07
```



#### [2.6 使用setf()进行格式化](#)

修改将一直有效，直到被覆盖为止。 用于格式化输出！所有操作符：[https://zh.cppreference.com/w/cpp/io/manip](https://zh.cppreference.com/w/cpp/io/manip)

这个版本的setf()用来设置单个位控制的格式信息，参数是一个fmtflags值，指出要设置哪一位。返回值是类型为fmtflags的数字，指出所有标记以前的设置，如果打算以后恢复原始设置，则可以保存这个值。
```cpp
fmtflags setf(fmtflags);
fmtflags setf(fmtflags, fmtflags); //接受两个参数，并返回以前的设置 
```
第二个参数  ios_base::basefield ios_base::floatfield ios_base::adjustfield

|标志|作用|
|:----|:-----|
|ios::boolapha|把 true 和 false 输出为字符串|
|ios::noboolalpha|取消 boolapha效果|
|ios::left 第二参数: adjustfield|输出数据在本域宽范围内向左对齐|
|ios::right 第二参数: adjustfield|输出数据在本域宽范围内向右对齐|
|ios::internal 第二参数: adjustfield|数值的符号位在域宽内左对齐，数值右对齐，中间由填充字符填充|
|ios::dec 第二参数 ios_base::basefield|设置整数的基数为 10|
|ios::oct 第二参数 ios_base::basefield|设置整数的基数为 8|
|ios::hex 第二参数 ios_base::basefield|设置整数的基数为 16|
|ios::showbase|强制输出整数的基数（八进制数以 0 开头，十六进制数以 0x 打头）|
|ios::noshowbase|取消showbase效果|
|ios::showpoint|强制输出浮点数的小点和尾数 0|
|ios::noshowpoint|取消showpoint效果|
|ios::uppercase|在以科学记数法格式 E 和以十六进制输出字母时以大写表示|
|ios::showpos|对正数显示“+”号|
|ios::noshowpos|取消noshowpos效果|
|ios::skipws|控制是否跳过输入上的前导空白符|
|ios::noskipws|取消noskipws效果|
|ios::scientific 第二参数 ios_base::floatfield|浮点数以科学记数法格式输出|
|ios::fixed 第二参数 ios_base::floatfield|浮点数以定点格式（小数形式）输出|
|ios::unitbuf|每次输出之后刷新所有的流|
|std::fixed|浮点数为定点十进制|
|std::scientific|科学计数法|
|std::hexfloat|浮点数也这启用十六进制浮点格式化。|
|std::defaultfloat|重置为十进制浮点格式化。|


其中，fmtflags是bitmask类型的typedef名，用于存储格式标记。该名称定义在ios_base类。

```cpp
int main(int argc, char const* argv[])
{   
  int val = 5689; double score = 98.5614;

  std::cout.setf(std::ios_base::hex, std::ios_base::basefield);
  std::cout.setf(std::ios_base::left, std::ios_base::adjustfield);
  std::cout.setf(std::ios_base::left, std::ios_base::adjustfield);
  std::cout.setf(std::ios_base::showpoint);

  std::cout.width(10);
  std::cout.fill('*');
  std::cout.precision(10);
  std::cout << val << std::endl;  //1639******
  std::cout << score << std::endl;  //98.56140000

  return 0;
}
```

#### [2.7 unsetf()函数](#)
调用 setf() 的效果可以通过 unsetf() 消除，其原型为 void unsetf(fmtflags maslk); 其中 mask 是位模式。 setf() 将位设置为1，unsetf()将位恢复为0。

可以使用 unsetf() 切换默认模式，因为当没有位被设置或两位都被设置时，将使用默认模式，所以启用默认模式有两种方法：

```cpp
cout.setf(0, ios_base::floatfield);
cout.unsetf(ios_base::floatfield);
```

#### [2.8 头文件<iomanip>](#)
使用iostream工具来设置一些格式值（如字段宽度）不太方便。为了简化工作，C++在头文件iomnip中提供了其他一些控制符，它们能够提供一样的服务，但表示起来更方便。

3个最常用的控制符分别是setprecision()、setfill()、setw()，分别用来设置精度、填充字符和字段宽度，它们都带参数。

* setprecision()控制符接受一个指定精度的整数参数；
* setfill()控制符接受一个指定填充字符的char参数；
* setw()控制符结构一个指定字符宽度的整数参数。

由于它们都是控制符，因此可以用cout语句连接起来。这样，setw()控制符在现实多列值时尤其方便。

```cpp
#include<iostream>
#include<iomanip>
#include<cmath>

int main()
{
    using namespace std;
    // use new standard maniplulators
    cout << fixed << right;

    // use iomanip maniplulators
    cout << setw(6) << "N" << setw(14) << "square root"
        << setw(15) << "fourtj root\\n";
    double root;
    for (int n = 10; n <= 100; n += 10)
    {
        root = sqrt(double(n));
        cout << setw(6) << setfill('.') << n << setfill(' ')
            << setw(12) << setprecision(3) << root
            << setw(14) << setprecision(4) << sqrt(root)
            << endl;
    }

    return 0;
}
// Output:
N   square root   fourtj root
....10       3.162        1.7783
....20       4.472        2.1147
....30       5.477        2.3403
....40       6.325        2.5149
....50       7.071        2.6591
....60       7.746        2.7832
....70       8.367        2.8925
....80       8.944        2.9907
....90       9.487        3.0801
...100      10.000        3.1623
```


### [3. cin进行输入](#) 
C++ 中的cin是一个 istream对象，从标准输入中读取数据 输入即给程序提供数据，cin对象将标准输入表示为字节流。通常情况下，通过键盘来生成这种字节流。
cin对象会从输入流中抽取字符，输入可以是各种类型，cin会根据接受值的变量的类型，使用其方法将字符序列转换为所需的类型。

```cpp
cin >> value_holder;
```
#### [3.1 输入流状态](#)
cin或cout对象包含一个描述流状态（stream state）的数据成员  **条件状态** IO流有四种条件状态，分别用位来控制。流状态被定义为 **iostate类型（这是一种bitmask类型）**
* cin.badbit : 001 1 表示系统级错误，一旦被置位，流就无法再使用了
* cin.eofbit : 010 2 表示流已经读完，到达尾部了
* cin.failbit: 100 4 可恢复错误，如期望读取数值却读出一个字符等错误，或者已经到达流的尾部
* cin.goodbit: 000 0 可用状态

当一个流遇到数据类型不一致的错误而不可用时，我们可以使其恢复为有效状态（置换eof和fail位）

也可通过调用成员函数查看流的状态，如：cin.eof() cin.fail() cin.bad() cin.good() //相应状态为真则返回true，反则为false **eof() 只有输入流有**
* cin.rdstate() 返回流状态
* clear() 方法将状态设置位它的参数。默认参数为0，这将清除全部3个状态位（eofbit、badbit、failbit）。
* setstate() 方法只影响其参数中已设置的位置，而不会影响其他位。主要用途是为输入和输出函数提供一种修改状态的途径。

```cpp
// 1. 清除全部3个状态位
clear();
// 2. 设置eofbit，其他两个状态位被清除
clear(eofbit);
// 3. 只设置eofbit，不会影响其他两个位
setstate(eofbit);
```

#### [3.2 I/O和异常](#)
默认情况下，假设某个输入函数设置了eofbit，不会导致异常被引发。但可以使用exceptions()方法来控制异常如何被处理。

* exceptions()方法返回一个位字段，它包含3位，分别对应于eofbit、failbit、badbit。
* 修改流状态涉及clear()或setstate()，这都将使用clear()
* 修改流状态后，clear()方法将当前流状态与expections()返回的值进行比较。如果在返回值中某一位被设置，同时当前状态中的对应位也被设置，则clear()将引发ios_base::failure异常。
* 所以如果expections()返回的是goodbit，则不会引发任何异常。而expections()的默认设置为goodbit，即不会引发异常。
* 重载的expections(iostate)函数能够控制其返回值

> ios_base::failure 异常类是从std::expection类派生而来的，也包含一个what()方法。

```cpp
// setting badbit causes exception to be thrown
cin.exception(badbit);
// 使用位运算符OR能指定多位
cin.exception(badbit | eofbit);
```

#### [3.3 重载>>](#)
istream类重载了抽取运算符>>，使之能够识别这些基本类型： 

signed char &、unsigned char &、char &、short &、unsigned short & 、int &、unsigned int &、long &、unsigned long &、long long &(C++11)、unsigned long long &(C++11)、float &、double &、long double &。

istream类还未下列字符指针类型重载了>>抽取运算符：
```cpp
signed char*
unsigned char*
char*
```

顺便一说，可以将hex、oct、dec控制符与cin一起使用，来指定将整数输入解释为十六进制、八进制还是十进制格式。

#### [3.4 cin从缓冲区读取数据](#)
cin从缓冲区读取数据，有多种方式，如操作符 >> 、函数getline()、get()等

**使用 >> 输入** 不同版本的抽取运算符查看输入流的方法是相同的。 **它们跳过空白（空格、换行符和制表符），直到遇到非空白字符。**

* 根据后面的变量的类型读取数据
* 读取时结束条件：enter, space, tab
* 对结束符的处理：丢掉
* 当遇到类型不一致时，流处于不可用状态，若需继续使用这个流，需恢复流的有效状态。

```cpp
int main(int argc, char const* argv[])
{   
  std::cout <<"input you age:"<< std::flush;
  int i = 0;
  std::cin>> i;  
  std::cout<<"age: "<< i <<std::endl;  
  return 0;
}
/*
input you age:10
age: 10
*/

/*
input you age:asdadad
age: 0
*/
```

##### 3.4.1 控制输入格式

默认输入运算符会忽略空白字符（空格、回车符、回车符、换行符、和制表符）。可以设置它不忽略，使用noskipws控制符！

```cpp
char ch;
while(cin >> ch) cout << ch;
/*
a  b                  c
d
*/
//输出为：abcd
```

**使用noskipws**

```cpp
char ch;
cin >> noskipws;
while(cin >> ch) cout << ch;
/*
a  b                  c
d
*/
//输出为
/*
a  b                  c
d
*/
```

#### [3.5 get()和getline()](#)

它们提供下面的输入功能：

* 方法get(char&)和get(void) **提供不跳过空白的单字符输入功能**
* 函数get(char*, int, char)和getline(char*, int, char)在默认情况下读取整行而不是一个单词。

它们被称为非格式化输入函数（unformatted input functions），因为它们只是读取字符输入，而不会跳过空白，也不进行数据转换。

##### [3.5.1 get() 读取单字符](#)
在使用char参数或没有参数的情况下，get()方法读取下一个输入字符，即使该字符是空格、制表符或换行符。 成员函数get(char &)可以这样使用：

```cpp
// utility of get(char &)
int ct = 0;
char ch;
while (cin.get(ch))
{
    cout << ch;
    ct++;
}
```
get(char &) 成员函数返回一个指向用于调用它的istream对象的引用，所以上面可以用于while的条件，因为会转换位good()的值来判断流状态是否出错。同时，这使得能够拼接其他抽取：

```cpp
char c1, c2, c3;
cin.get(c1).get(c2) >> c3;
```

如果cin.get(char &)到达文件尾，无论是真正的文件尾，还是通过键盘仿真的文件尾，它都不会给其参数赋值，因为到达文件尾，就没有值可供赋给参数了。另外，还会调用setstate(failbit)，这会导致cin的测试结果为false。

> 键盘仿真的文件尾：对于DOS和Windows命令提示符模式，为按下Ctrl+Z；对于UNIX，是在行首按下Ctrl+D。

get(void)成员函数也能读取空白，这个重载版本需要使用返回值来将输入传递给程序。其返回类型为int（或某种更大的整型，这取决于字符集和区域）。所以不可以拼接。

到达文件尾后，cin.get(void)都将返回值EOF（头文件<iostream>提供的一个符号常量）。这种设计特性使得可以这样来读取输入：

```cpp
int ch; // 不能是char，因为值EOF可能无法使用char类型来表示
while ((ch = cin.get()) != EOF)
{
    // process input
}
```
##### [3.5.2 读取字符串](#)
getline() 成员函数和 get() 的字符串读取版本都读取字符串，它们的函数特征标相同：

```cpp
istream & get(char *, int, char);
istream & get(char *, int);
istream & getline(char *, int, char);
istream & getline(char *, int);
```

* 第1个参数是用于放置输入字符串的内存单元的地址；
* 第2个参数比要读取的最大字符数大1（额外的一个字符用于存储结尾的空字符）；
* 第3个参数指定用作分界符的字符 **只有两个参数的版本将换行符用作分界符。**

上述函数都在读取到最大数目的字符或遇到换行符后为止。

**二者之间的主要区别在于：**
* get()将换行符留在输入流中，这样接下来的输入操作首先看到的将是换行符；
* getline()会抽取并丢弃输入流中的换行符。

遇到分界符后，输入将停止，即使还未读取最大数目的字符。类似地，get()将分界字符留在输入队列中，而getline()不保留。

**ignore()** 成员函数会使输入读取并丢弃接下来指定的字符串，其原型如下：
```cpp
istream & ignore(int = 1, int = EOF);
```
* 第1个参数指定要读取的最大字符数；
* 第2个事字符，用作输入分界符。

#### [3.6 意外字符串输入](#)
上述的字符串输入也可能影响流状态。与其他输入函数一样，get()和getline()在遇到文件尾时将设置eofbit，遇到流被破坏（如设备故障）时将设置badbit。

另外两种特殊情况是 **无输入** 以及 **输入到达或超过函数调用指定的最大字符数** 

如果不能抽取字符（无输入），它们将把空值字符放置到输入字符串中，并使用setstate()设置failbit。方法在下面情况下可能无法抽取字符：

一种可能的情况是输入方法立刻到达了文件尾；

对于get(char *, int)来说，另一种可能是输入了一个空行。对于getline()来说不会设置failbit，因为它仍将抽取换行符。

**如果输入队列中的字符数等于或超过了输入方法指定的最大字符数：**
```cpp
char tmp[30];
cin.getline(tmp, 30);
```
getline()方法读取直至到达文件尾、直到换行符或存储到29个字符为止。

* 如果遇到文件尾，则设置eofbit；
* 如果将要读取的字符是换行符，则该字符将被读取且丢弃；
* 如果读取了29个字符，并且下一个字符不是换行符，则设置failbit。

因此，包含30个或更多字符的输入行将终止输入。

但对于get(char *, int)方法来说，它首先测试字符数，然后测试是否为文件尾以及下一个字符是否为换行符。如果它读取了最大数目的字符，不会设置failbit标记。

由此可以终止读取是否是由于输入字符过多，也可以通过peek()查看下一个输入字符

* 如果下一个字符是换行符，则说明get()已经读取了整行；
* 如果下一个字符不是换行符，则说明get()是在到达行尾前停止的。


* getline(char *, int)	如果没有读取任何字符，则设置failbit；如果读取了最大数目的字符，且行中还有其他字符，则设置failbit。
* get(char *, int)	如果没有读取任何字符，则设置failbit

#### [3.7 其他istream方法](#)
还有一些方法：

* read()函数读取指定数目的字节，并将它们存储在指定的位置中。和getline()与get()不同的是，不会在输入后加上空值字符’\\0’。它不是专为键盘输入设计的，最常与osream write()函数结合使用，来完成文件的输入输出。
* peak()函数返回输入中的下一个字符，但不抽取输入流中的字符。
* gcount()方法返回最后一个非格式化抽取方式读取的字符数。即字符是由get()、getline()、ignore()或redd()方法读取的，不是由抽取运算符（>>）读取的。但使用strlen()函数计算字符数可能更快。
* putback()函数将一个字符插入到输入字符串中，被插入的字符将是下一条输入语句读取的第一个字符
* ignore(size, delim) 读取并且忽略最多size字符，包括delim。



-----
