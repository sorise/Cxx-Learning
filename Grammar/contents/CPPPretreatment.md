### [C++ 预处理、宏定义](#)
`C++ 中预定义变量或者宏定义是经常使用！`

-----
- [x] [1. 断言](#1-)
- [x] [2. 宏定义](#2-)
- [x] [3. ](#3-)
- [x] [4. ](#4-)
- [x] [5. ](#5-)
-----

### [1. 断言](#)

#### [1.1 assert](#)
assert宏的原型定义在<assert.h>中，其作用是如果它的条件返回错误，则终止程序执行。
原型定义：
```cpp
#include <assert.h>
void assert( int expression );
```
`assert的作用是先计算表达式expression，如果其值为假（即为0），那么它先向std::error打印一条错误信息，然后通过调用` **abort()** `来终止程序运行。`

```cpp
fp = fopen( "noexitfile.txt", "r" );
//以只读的方式打开一个文件，如果不存在就打开文件失败

assert( fp );                       
//所以这里出错
assert(nNewSize >= 0);
```

```cpp
int main(int argc, char const* argv[])
{   
   int i = 100;
   assert(i++ < 100);
   //runner: index.cpp:19: int main(int, const char**): Assertion `i++ < 100' failed.
   return 0;
}
```
`频繁的调用assert函数会极大的影响程序的性能，增加额外的开销。`

#### [1.2 NDEBUG](#)
assert的行为依赖于一个名为NDEBUG的预处理变量的状态。如果定义了 NDEBUG，则assert什么也不做！默认情况下没有定义NDEBUG。

* assert是一种预处理宏。对于assert(expr);，当expr表达式为0时，assert输出信息并终止程序运行；当expr表达式非0时，什么也不做。assert定义在cassert头文件中。
* assert的行为依赖于另一个宏NDEBUG，如果定义了这个宏，assert(expr)就不会执行任何操作，可以避免判断expr的开销。

```cpp
#define NDEBUG //定义在 <assert.h> 之前关闭断言和调试
#include<iostream>
#include<memory>
#include<functional>
#include<thread>
#include<iterator>
#include <assert.h>

int main(int argc, char *argv[]){
    const int len = 7;
    assert(len == 6);
    #ifndef NDEBUG
        std::cerr << __func__ << " - 函数名" << endl;
        std::cerr << __FILE__ << " - 文件名" << endl;
        std::cerr << __LINE__ << " - 多少行" << endl;
        std::cerr << __TIME__ << " - 时间" << endl;
        std::cerr << __DATE__ << " - 日期" << endl;
    #endif
    return 0;
}
```

C++ 编译器预定义很多预处理变量
* `__func__`  `存放函数名的静态数组，默认输出当前所在的函数名`
* `__FILE__`  `存放文件名的字符串的字面值`
* `__LINE__`  `存放当前行号的整形字面值`
* `__TIME__`  `存放文件编译时间的字符串字面值`
* `__DATE__`  `存放文件编译日期的字符串字面值`


#### [1.3 static_assert](#)
assert是动态断言，运行期检查，影响性能，故debug版本检查，release关闭。
stastic_assert为了弥补assert和error的不足，可以作编译期的静态检查。
```cpp
static_assert(condition,tipString);
//(常量表达式，"提示字符串")
```

错误使用！
```cpp
int list[] = {78,98,99,45,75,56,85,45};
static_assert(list[2] > 70, "不合理");
```
正确使用
```cpp
static_assert(sizeof(char) == 2, "hello furong");
```


### [2. 宏定义](#) 

#### [2.1 pragma once](#)
**#pragma once** 是一个比较常用的C/C++预处理指令，只要在头文件的最开始加入这条预处理指令，就能够保证头文件只被编译一次。
### [3.](#) 

### [4.](#) 

### [5.](#) 

-----
`时间`: `[]` 