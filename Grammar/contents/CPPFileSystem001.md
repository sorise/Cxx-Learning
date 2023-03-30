### [C++ 文件输入输出](#)
每个程序的基本操作了！ 接受吧!

-----
- [x] [1. C++ 11 文件输入输出](#1-c-11-文件输入输出)
- [x] [2. fstream](#2-fstream)
- [x] [3. C++ 17 文件系统库](#3-c-17-文件系统库)
- [x] [4. ](#4-)
- [x] [5. ](#5-)
-----

### [1. C++ 11 文件输入输出](#)
文件 I/O 实现，定义于头文件 **<fstream>** , 头文件定义了四个模板类，总共实例化了八个类 filebuf ifstream ofstream fstream wfilebuf wifstream wofstream wfstream 分别位 char 和 wchar_t 实例化了！

* **basic_filebuf**: 抽象原生文件设备。
* **basic_fstream**: 实现高层文件流输入/输出操作。
* **basic_ifstream**: 实现高层文件流输入操作 **复制构造函数被删除：此类不可复制。**
* **basic_ofstream**: 实现高层文件流输出操作 **复制构造函数被删除：此类不可复制。**

iostream对象都有一个缓冲区，我们称之为流缓冲区，iostreams中将该流缓冲区抽象为一个类，即streambuf类。每个iostream的类都会包含一个指向streambuf对象的指针，这也就意味着我们可以直接的去访问到这个指针，并向该streambuf对象发送消息等

**刷新缓存区** 使用 flush() 方法,使用 endl 换行 刷新缓存区。

* stream << unitbuf 设置流，每次操作后都刷新缓存区。
* stream << nounitbuf 设置流，回到缺省状态。

```cpp
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char const* argv[])
{ 
  char bufName[256] = {"\0"};  
  printf("please input your new file name:");
  std::cin.getline(bufName, 256);

  //打开一个新的文件
  std::ofstream fout(bufName);
  printf("input your secret: ");
  char bufFIle[256];
  std::cin.getline(bufFIle, 256);

  //将输入输出到 文件中
  fout << bufFIle;
  fout.close();
  return 0;
}
```

#### [1.1 文件打开模式](#)
文件传输模式, ios_base定义了openmode类型表示模式，类似于fmtflags和iostate类型，是bitmask类型。

**文件模式常量**
|Constant|Meaning|
|:----|:----|
|ios_base::in|读取文件|
|ios_base::out|写入文件|
|ios_base::ate|打开文件时文件指针指向文件尾|
|ios_base::app|只能在在文件尾添加内容|
|ios_base::trunc|如果文件存在则截断它|
|ios_base::binary|二进制文件|

**文件模式组合** （|符号表示联合两个二进制为一个值，该值可以表达两个二进制模式）

|C++ 模式|C模式|含义|
|:----|:----|:----|
|ios_base::in|“r”|读文件模式|
|ios_base::out|“w”|与ios_base::out | ios_base::trunc一样|
|ios_base::out \| ios_base::trunc|“w”|写文件模式，并且在打开文件时截断文件(将文件置为空)|
|ios_base::out \| ios_base::app|“a”|追加模式，将文件指针指向文件尾，向末尾添加内容；原始数据不可修改|
|ios_base::in \| ios_base::out|“r+”|打开以读写，在文件允许的位置写入|
|ios_base::in\| ios_base::out \| ios_base::trunc|“w+”|打开以读写，如果文件已经存在，则首先截断文件|
|其他 \| ios_base::binary|“cmodeb”|在C++模式和二进制模式打开|
|其他 \| ios_base::ate|“cmode”|以指定的模式打开，并将文件指针移到文件尾。|


#### [1.2 ofstream](#)
文件输出流，将程序中的数据输出到其他地方！ 同样的 他也继承了 ios_base的流状态！

**构造函数**
* basic_ofstream();
* explicit basic_ofstream( const char* filename,std::ios_base::openmode mode = ios_base::out );
* basic_ofstream( basic_ofstream&& other );

**参数：**
* filename	-	要打开的文件名
* mode	-	指定打开模式。可使用下列常量或它们之间的逐位或：
    * app	追加模式。所有写入都追加到文件末尾。
    * binary	以二进制模式打开
    * in	为读打开
    * out	为写打开
    * trunc	在打开时舍弃流的内容
    * ate	打开后立即寻位到流结尾
* other  用作源的另一文件流

要让程序写入文件，需要以下几个步骤： 1. 创建一个ofstream对象来管理输出流 2. 将该对象与特定的文件关联起来  3. 以使用cout的方式使用该对象

> 以默认的方式打开文件，这相当于删除已有的内容！

如果调用无参构造函数，那么打开文件需要调用open函数并且传入文件路径，打开文件！  其他有参数构造函数，会自动打开文件！

* is_open()： 判断文件是否以及被打开了！
* close()： 关闭文件！
* open(s，mode)： 代开文件s！
* <<: 将数据输出到文件中！

```cpp
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char const* argv[])
{ 
  //向文件尾部添加内容
  std::ofstream out_to_file("./kick.txt", std::ios_base::app | std::ios_base::ate );
  if (out_to_file.is_open())
  {
    printf("open wait to add text! \n");
    int i = 0;
    while(i++ < 11){
      out_to_file << "number:" <<i << "\n";
    }
  }
    
  //关闭内容
  out_to_file.close();
  return 0;
}
```

#### [1.2 ifstream](#)
该数据类型表示输入文件流，用于从文件读取信息。

**构造函数**
* basic_ifstream();
* explicit basic_ifstream( const char* filename,std::ios_base::openmode mode = ios_base::in );
* basic_ifstream( basic_ifstream&& other );

**参数：**
* filename	-	要打开的文件名
* mode	-	指定打开模式。可使用下列常量或它们之间的逐位或：
    * app	追加模式。所有写入都追加到文件末尾。
    * binary	以二进制模式打开
    * in	为读打开
    * out	为写打开
    * trunc	在打开时舍弃流的内容
    * ate	打开后立即寻位到流结尾
* other  用作源的另一文件流

```cpp
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char const* argv[])
{ 
  //读取
  std::ifstream in_file("./kick.txt", std::ios_base::in);
  const int len_buf = 512;
  char buf[len_buf];
  int lines = 0;
  //结束
  while (!in_file.eof())
  {
     in_file.getline(buf, len_buf);
     std::cout << "line "<< lines++ << ": " << buf << "\n";
  }
  
  //关闭内容
  in_file.close();
  return 0;
}
```

#### [1.3 二进制文件](#)
向文件中写入二进制数据的方式也非常简单，首先你要以二进制的方式打开文件， 然后需要将对象的地址转换成 字节指针 char * 如果你使用的是 wchar_t 那么应该转换为 wchar_t *

**可以将那些数据写入二进制文件，并且可以正常读取呢？**
* 内置数据类型 int  float double 数组 ....
* 结构体
* **不使用虚函数的类** 如果有虚函数存在，那么类中会有一个指向虚函数表的指针！这个指针数据重新加载到内存后，任然指向原来的位置，而原来的位置很有可能早就不是原来的数据了！


char_type: char、 wchar_t
* ifstream.read 读取二进制数据 basic_istream& read( char_type* s, std::streamsize count );
* ofstream.write 写出二进制数据 basic_ostream& write( const char_type* s, std::streamsize count );

**输出数据到文件**
```cpp
#include<iostream>
#include<list>
#include<algorithm>
#include<fstream>

struct Student
{
    char studentID[32];
    char name[256];
    double scores[3];
    unsigned int grade; //年级
};


int main(int argc, char *argv[]){
    Student lzm = { "2022110419", "lizhiming", {85,98,85}, 2022 };
    
    std::ofstream fout("student.data", std::ios_base::out|
                                       std::ios_base::binary|
                                       std::ios_base::app);
    fout.write((char *)&lzm, sizeof(lzm));

    fout.close();
    return 0;
}
```

**输入数据到程序**
```cpp
#include<iostream>
#include<list>
#include<algorithm>
#include<fstream>

struct Student
{
    char studentID[32];
    char name[256];
    double scores[3];
    unsigned int grade; //年级
};


int main(int argc, char *argv[]){
    try{
      std::ifstream fin("student.data", std::ios_base::in|std::ios_base::binary);

      if (fin.is_open())
      {
          Student lzm;
          fin.read((char*)&lzm, sizeof(Student));
          std::cout << lzm.name <<" 's information has been read!" << "\n";
          fin.close();
      }else{
        throw "error happend when open student.data!";
      }
    }catch(const char * errorinfo){
        std::cout << errorinfo << "\n";
    }catch(...){
        std::cout << "no catch to match the exception!" << "\n";
    }
    return 0;
}
```

#### [1.4 随机存取](#)
指的是直接移动到文件的任意位置。常用于数据库文件，程序维护一个独立的索引文件，该文件指出数据在主数据文件中的位置，这样程序就可以直接跳转到存放数据的位置了。 

前一个参数必须有，后一个参数可有可无
* basic_istream& seekg( pos_type pos ); 
* basic_istream& seekg(off_type off, std::ios_base::seekdir dir)：将输入指针移动至给定文件位置 **(由于fstream对象使用缓冲区存取中间数据，因此指针指向的实际上是缓冲区的位置，而不是实际的文件)**
* basic_ostream& seekp( pos_type pos );
* basic_ostream& seekp(off_type off, std::ios_base::seekdir dir)：将输出指针移动至给定文件位置 **(由于fstream对象使用缓冲区存取中间数据，因此指针指向的实际上是缓冲区的位置，而不是实际的文件)。**
* pos_type tellg(); basic_istream: 返回值为streampos，表示输入指针的当前文件位置(单位为字节)，相对于文件开始的偏移量。
* pos_type tellp(); basic_ostream 返回当前关联的 streambuf 对象的输出位置指示器。


参数说明：
* pos	-	设置输入位置指示器到的绝对位置。
* off	-	设置输入位置指示器到的相对位置。
* dir	-	定义应用相对偏移到的基位置。它可以是下列常量之一：
  * beg	流的开始
  * end	流的结尾
  * cur	流位置指示器的当前位置

首先我们在文件中存放 3000 个整数！
```cpp
int main(int argc, char *argv[]){
    const int len = 3000;
    int numbers[len];
    for (int i=0; i<= len ; ++i)
    {
        numbers[i] = i;
    }
    
    std::ofstream fout("number.dat", std::ios_base::out|
                                       std::ios_base::binary|
                                       std::ios_base::app);

    fout.write((char *)numbers, sizeof(int) * len);
    fout.close();
    return 0;
}
```
我们利用 seekp 读取第 n 个数子
```cpp
int main(int argc, char *argv[]){
    try{
      std::ifstream fin("number.dat", std::ios_base::in|std::ios_base::binary);

      if (fin.is_open())
      {
          int n = 200; // n = 200
          int value = -1;
          fin.seekg(n* sizeof(int)); 
          fin.read((char*)&value, sizeof(int));
          std::cout << "position " << n +1 << ": " << value << "\n";
          fin.close();
      }else{
        throw "error happend when open student.data!";
      }
    }catch(const char * errorinfo){
        std::cout << errorinfo << "\n";
    }catch(...){
        std::cout << "no catch to match the exception!" << "\n";
    }
    return 0;
}
```
**从结尾倒过来读取**
```cpp
int main(int argc, char *argv[]){
    try{
      std::ifstream fin("number.dat", std::ios_base::in|std::ios_base::binary);

      if (fin.is_open())
      {
          int n = -200;
          int value = -1;
          fin.seekg(n* sizeof(int), std::ios_base::end); //到末尾 相对末尾200位置 也就是第1301 的位置
          fin.read((char*)&value, sizeof(int));
          std::cout << "position " << n +1 << ": " << value << "\n";
          fin.close();
      }else{
        throw "error happend when open student.data!";
      }
    }catch(const char * errorinfo){
        std::cout << errorinfo << "\n";
    }catch(...){
        std::cout << "no catch to match the exception!" << "\n";
    }
    return 0;
}
```

#### [1.5 临时文件](#)
软件开发过程中生成临时文件是比较常用的。此时为临时文件指定一个明明方案，确保每个文件都被指定独一无二的文件名。cstdio库函数中声明的tmpnam()标准函数可实现该功能！

```cpp
char*tmpnam(char*pszname);
```
mpname()函数创建了一个临时文件名，并将它放到pszname指向的C风格字符串中，常量L_tmpnam限制了文件名包含的字符串，常量TMP_MAX限制了确保当前目录中不生成崇明文件夹的情况下，tmpnam()可被调用的最多次数

```cpp
#include <iostream>
#include <cstdio>
using namespace std;
int main()
{
  cout << "This system can generate up tp" << TMP_MAX << 
      " temporary names of up to " << L_tmpnam << "characters.\n";
  char pszName[L_tmpnam] = { '\0' };
  cout << "Here are ten names: \n";
  for (int i = 0; i < 10; i++)
  {
    tmpnam_s(pszName);
    cout << pszName << endl;
  }
  return 0;
}
```

### [2. fstream](#) 
读写操作，对打开的文件可进行读写操作，可以把它理解成一个父类，包含的子类有ifstream和ofstream等！ 

**常用构造函数:**
```cpp
fstream();
fstream( fstream&& other );
fstream( const basic_ifstream& rhs) = delete;
explicit fstream( const char* filename, std::ios_base::openmode mode = ios_base::in );
explicit fstream( const std::filesystem::path::value_type* filename, 
        std::ios_base::openmode mode = ios_base::in );
explicit fstream( const std::string& filename, std::ios_base::openmode mode = ios_base::in );
```
**具有的方法:**
* **open(s)** 打开名为s的文件，并将其与文件与fstream绑定。
* **is_open()** 判断文件是否以及被打开了！
* **close()**： 关闭文件！

将输入的数字输出到文件中！
```cpp
#include<iostream>
#include<fstream>
using std::cout;
using std::endl;
using std::cin;

int main(int argc, char *argv[]){
    int number;
    using std::ios_base;
    std::fstream fs("./user.txt", ios_base::out|ios_base::app);
    
    while(cin >> number && fs.is_open()){
        fs << number << " ";
        printf("number: %d \n", number);
    }
    fs.close();
    return 0;
}
```
**当一个fstream对象被销毁时(离开其作用域时)，close会自动调用！**
### [3. C++17 文件系统库](#) 
**#include\<filesystem\>** 是C++17 将文件系统的操作封装了进来，从而可以跨平台的操作文件系统。

```cpp
#include<filesystem>
using namespace std::filesystem;

//or
using fs=std::filesystem;
```

[**主要我们使用几个类：**](#) 当然还有很多函数。
* **path 类**：说白了该类只是对字符串（路径）进行一些处理，这也是文件系统的基石。
* **directory_entry 类**：功如其名，文件入口，这个类才真正接触文件。 
* **directory_iterator 类**：获取文件系统目录中文件的迭代器容器，其元素为 directory_entry对象（可用于遍历目录）
* **file_status 类**：用于获取和修改文件（或目录）的属性（需要了解C++11的强枚举类型（即枚举类））

[**常用库函数**](#)
* **void copy(const path& from, const path& to)** ：目录复制
* **path absolute(const path& pval, const path& base = current_path())** ：获取相对于base的绝对路径
* **bool create_directory(const path& pval)** ：当目录不存在时创建目录
* **bool create_directories(const path& pval)** ：形如/a/b/c这样的，如果都不存在，创建目录结构
* **bool exists(const path& pval)**：用于判断path是否存在
* **uintmax_t file_size(const path& pval)**：返回目录的大小
* **file_time_type last_write_time(const path& pval)**：返回目录最后修改日期的file_time_type对象
* **bool remove(const path& pval)**：删除目录
* **uintmax_t remove_all(const path& pval)**：递归删除目录下所有文件，返回被成功删除的文件个数
* **void rename(const path& from, const path& to)**：移动文件或者重命名

### [4 path类](#)
类型 path 的对象表示文件系统上的路径。

```cpp
#include<iostream>
#include<fstream>
#include<filesystem>
using std::cout;
using std::endl;
using std::cin;

int main(int argc, char *argv[]){
    std::filesystem::path p1("./src/main.cpp");

    if (!std::filesystem::exists(p1)){
        printf("the file not exist!\n");
    }else{
        std::cout << "p1的父目录:" << p1.parent_path()<<std::endl;
        std::cout << "p1的文件名:" << p1.filename() << std::endl; 
        //p1的文件名(带拓展名字)
        std::cout << "p1的文件名:" << p1.stem() << std::endl; 
        //p1的文件名(不带拓展名字)
        std::cout << "p1的拓展名:" << p1.extension() << std::endl;
        std::cout << "p1的根目录:" << p1.root_name() << std::endl;
    } 
    //创建一个目录
    std::filesystem::create_directories("./data");
    return 0;
}
```


#### [4.1 构造函数](#)
```cpp
path() noexcept;
path( const path& p );
path( path&& p ) noexcept;
template< class Source >
path( const Source& source, format fmt = auto_format );
template< class InputIt >
path( InputIt first, InputIt last, format fmt = auto_format );
```


#### [4.2 路径函数](#)
* **operator=** 赋值运算符
* **stem()** 返回主干路径组分(公开成员函数)
* **empty()** 检查路径是否为空(公开成员函数)
* **root_directory**  若存在则返回路径的根目录
* **root_path** 若存在则返回路径的根路径
* **filename** 返回文件名路径组分
* **extension()** 返回文件扩展名路径组分(公开成员函数)
* **relative_path()** 返回相对根路径的路径(公开成员函数)

```cpp
fs::path p = "C:/users/abcdef/AppData/Local";
```

### [4.](#) 

### [5.](#) 

-----
时间: [] 