### [C++ 日期和时间工具](#)
 **介绍**：C++ 包含对二类时间操作的支持, 首先是C语言风格日期和时间工具，其次是[**chrono**](https://zh.cppreference.com/w/cpp/header/chrono) 库，以各种精度跟踪时间的类型的灵活汇集，当然重点是后者！[C语言时间库](https://zh.cppreference.com/w/cpp/chrono/c)

-----
- [x] [1. chrono概念](#1-chrono概念)
- [x] [2. 时间字面量](#2-时间字面量)
- [x] [3. C语言时间](#3-c语言时间)
- [x] [4. ](#4-)
- [x] [5. ](#5-)
-----

* [C++ chrono  此头文件是C++日期和时间库的一部分](https://zh.cppreference.com/w/cpp/header/chrono)
* [C ctime.h  此头文件是 C 风格日期和时间库的一部分](https://zh.cppreference.com/w/cpp/header/ctime)



**GMT（Greenwich Mean Time）- 格林尼治标准时**：依靠对太阳的观察，也就是星球自转来计时，GMT 12:00 就是指的是英国伦敦郊区的皇家格林尼治天文台当地的中午12:00，而GMT+8 12:00，则是指的东八区的北京当地时间的12:00。

**UTC（Coordinated Universal Time） - 协调世界时**：国际原子时 （International Atomic Time）， 1967 年国际度量衡大会把秒的定义改成**铯原子**进行固定震荡次数的时间后，时间的测量就可以与星球的自转脱节了，**只利用原子钟计算时间与日期的系统**。<span style="color:#5e8f65;font-weight:600" >这也就是 UTC 为什么与 GMT 几乎一样的关係。由于 UTC 直接与国际度量衡标准相联繫，所以目前所有的国际通讯系统，像是卫星、航空、GPS 等等，全部都协议採用 UTC 时间。</span>

**UTC + 时区差 = 本地时间**

### [1. chrono概念](#)

C++11 则通过 chrono 库提供了更加灵活的时间操作，要想在 C++11 中完成同样的操作，首先需要引入 3 个概念, **时钟 clock**、<span style="color:red">**时长uration**</span> 和 **时间点time_point**。

clock 表示计时的时钟，C++11 提供了 3 种 clock

- [**steady_clock**](https://zh.cppreference.com/w/cpp/chrono/steady_clock)，稳定时钟，保证每时每刻以稳定速率增长
- [**system_clock**](https://zh.cppreference.com/w/cpp/chrono/system_clock)，系统时钟，可能不稳定，因为人或其它程序可以修改
- [**high_resolution_clock**](https://zh.cppreference.com/w/cpp/chrono/high_resolution_clock)，高精度钟，拥有最小系统可提供的时间单位 (PS. 在我本机 clang11 的标准库中，**high_resolution_clock**  就是精度更高的 **steady_clock**。

#### 1.1 duration

**duration** 表示时间间隔，我们平时经常说做某件事“用了2个小时”，这里“2个小时”就是一个时间间隔，它由两部分组成，数量 2 和单位小时，C++11 中，duration 也拥有类似的定义：

```cpp
template<
    class Rep,
    class Period = std::ratio<1>
> class duration;

template<std::intmax_t Num, std::intmax_t Denom = 1> 
class ratio{}

/* Num代表分子，Denom代表分母。 */
```

其中，**Rep** 存储时间间隔的基础类型，C++11 通常使用 **long**, **long long** 这些整型表示;  **Period** 则用一个有理数表示了该时间间隔和秒之间的换算关系：

```cpp
typedef duration<long,  ratio<3600>> hours;
//意味着，小时的数量内部用长整型表示，且一小时等于 3600 秒
```

**C++11** 预定义了常用的时间间隔类型，包括 `hours`, `minutes`, `seconds`, `milliseconds`, `microseconds`, `nanoseconds`

```cpp
typedef duration <Rep, ratio<3600,1>> hours; 
typedef duration <Rep, ratio<60,1>> minutes;
typedef duration <Rep, ratio<1,1>> seconds;
typedef duration <Rep, ratio<1,1000>> milliseconds;          //毫秒
typedef duration <Rep, ratio<1,1000000>> microseconds;       //微秒
typedef duration <Rep, ratio<1,1000000000>> nanoseconds;     //纳秒
```

duration 之间是可以互相换算且可加减的: 
```cpp
microseconds mi(2745);

// 2745000, 向高精度隐式类型转换
nanoseconds na = mi; 

// 2, 向低精度必须显式类型转换，且会被取整
milliseconds mill =  duration_cast<milliseconds>(mi); 

// 4745，不同精度也可相加，转换为较高精度
cout << (mill + mi).count() << endl; 
```

#### 1.2 time_point

类模板 **std::chrono::time_point** 表示时间中的一个点。它被实现成如同存储一个 `Duration` 类型的自 `Clock` 的纪元起始开始的时间间隔的值，原型是：

```cpp
template<
    class Clock,
    class Duration = typename Clock::duration
> class time_point;
```
* Clock ，此时间点在此时钟上计量
* Duration ，用于计量从纪元起时间的 std::chrono::duration 类型

[**time_since_epoch**](https://zh.cppreference.com/w/cpp/chrono/time_point/time_since_epoch)  函数用来获取1970.1.1以来的时间:

```cpp
#include <iostream>
#include <chrono>
using namespace std;
 
int main()
{
    using namespace std::chrono;
    time_point<system_clock, hours> h = time_point_cast<hours>(system_clock::now());
    cout << h.time_since_epoch().count() << " hours since epoch" << endl;
    getchar();
    return 0;
}
```

### [2. 时间字面量](#) 

在内联命名空间 `std::literals::chrono_literals` 定义，方便直接定义时长。

|方法|返回值|
|:---|:---|
| [operator""h](https://zh.cppreference.com/w/cpp/chrono/operator""h)(C++14) | 表示小时的 [std::chrono::duration](https://zh.cppreference.com/w/cpp/chrono/duration) 字面量 (函数) |
| [operator""min](https://zh.cppreference.com/w/cpp/chrono/operator""min)(C++14) | 表示分钟的 [std::chrono::duration](https://zh.cppreference.com/w/cpp/chrono/duration) 字面量 (函数) |
| [operator""s](https://zh.cppreference.com/w/cpp/chrono/operator""s)(C++14) | 表示秒的 [std::chrono::duration](https://zh.cppreference.com/w/cpp/chrono/duration) 字面量 (函数) |
| [operator""ms](https://zh.cppreference.com/w/cpp/chrono/operator""ms)(C++14) | 表示毫秒的 [std::chrono::duration](https://zh.cppreference.com/w/cpp/chrono/duration) 字面量 (函数) |
| [operator""us](https://zh.cppreference.com/w/cpp/chrono/operator""us)(C++14) | 表示微秒的 [std::chrono::duration](https://zh.cppreference.com/w/cpp/chrono/duration) 字面量 (函数) |
| [operator""ns](https://zh.cppreference.com/w/cpp/chrono/operator""ns)(C++14) | 表示纳秒的 [std::chrono::duration](https://zh.cppreference.com/w/cpp/chrono/duration) 字面量 (函数) |
| [operator""d](https://zh.cppreference.com/w/cpp/chrono/operator""d)(C++20) | 表示月内日期的 std::chrono::day 字面量 (函数)                |
| [operator""y](https://zh.cppreference.com/w/cpp/chrono/operator""y)(C++20) | 表示特定年的 std::chrono::year 字面量 (函数)                 |

**字面量利用:**

```cpp
#include <chrono>
using namespace std::chrono_literals;

auto tickGap = 20ms; // 等同于 std::chrono::milliseconds tickGap(20);
```

### [3. C语言时间](#) 

**time.h/ctime.h** 是C/C++中的日期和时间头文件。用于需要时间方面的函数

**宏常量**:   **CLOCKS_PER_SEC** 表示每秒的处理器始时钟嘀嗒数。

#### [3.1 类型信息](#)

|类型|说明|
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [clock_t](https://zh.cppreference.com/w/cpp/chrono/c/clock_t) | 表示**进程**运行时间 (typedef long)， 足以表示实现定义范围和精度的进程运行时间的算术类型。 |
| [time_t](https://zh.cppreference.com/w/cpp/chrono/c/time_t)  | 从纪元起的时间类型 (typedef)                                 |
| [tm](https://zh.cppreference.com/w/cpp/chrono/c/tm)          | 日历时间类型 (类)                                            |
| [timespec](https://zh.cppreference.com/w/cpp/chrono/c/timespec)(C++17) | 以秒和纳秒表示的时间 (结构体)                                |

虽然标准中没有给出定义，但是该类型几乎总是整数类型，表示自 1970 年 1 月 1 日 00:00 UTC 以来所经过的秒数（不计闰秒）

```cpp
int j = 10;
for (int i = 0; i < 10000000; ++i) {
    j++;
}
clock_t systemTime = clock();
std::cout <<"cpu run time: "<< systemTime << std::endl; //16
```

**tm**:

```cpp
struct tm {
    int tm_sec; //分后之秒 – [0, 61] (C++11 前)[0, 60] (C++11 起)
    int tm_min; //时后之分 – [0, 59]
    int tm_hour; //自午夜起之时 – [0, 23]
    int tm_mday; //月之日 – [1, 31]
    int tm_mon;  //自一月起之月 – [0, 11]
    int tm_year; //自 1900 起之年
    int tm_wday; //自星期日起之日 – [0, 6]
    int tm_yday; //自一月 1 日起之日 – [0, 365]
    int tm_isdst;//夏令时标志。值若夏令时有效则为正，若无效则为零，若无可用信息则为负
};
```

夏令时又称“日光节约时制”或“夏时制”，**是指一种为节约能源而人为规定地方时间的制度**，**夏令时比标准时早一个小时**，在这一时间的制度实行期间所采用的统一时间称为“夏令时间”。法定时的一种。**在夏季，由于昼长夜短，为充分利用日光，把时钟适当拨快；到秋季，再拨回**。

#### [3.2 时间函数](#)

|函数|说明|
|:---|:---|
| [clock](https://zh.cppreference.com/w/cpp/chrono/c/clock)    | 返回自程序启动时起的原始处理器时钟时间 (函数) |
| [std::time_t time( std::time_t* arg );](https://zh.cppreference.com/w/cpp/chrono/c/time) | 返回自纪元起计的系统当前时间 (函数)，基本单位是秒。   |
| [difftime](https://zh.cppreference.com/w/cpp/chrono/c/difftime) | 计算时间之间的差 (函数)                       |
| [timespec_get](https://zh.cppreference.com/w/cpp/chrono/c/timespec_get)(C++17) | 返回基于给定时间基底的日历时间 (函数)         |



**std::time_t  std::time_t time( std::time_t* arg )** 

* arg	-	指向要存储时间的 std::time_t 对象的指针，或空指针

```cpp
time_t now;
time(&now);
std::cout << now << std::endl;
```

**double difftime( std::time_t time_end, std::time_t time_beg );**

* 以秒表示的二个时间的差。



#### [3.3 格式转换](#)

将日期转换为字符串类型。

|函数|说明|
|:---|:---|
| [char* ctime( const std::time_t* time );](https://zh.cppreference.com/w/cpp/chrono/c/ctime) | 转换 `time_t` 对象为文本表示 (函数)               |
| [asctime](https://zh.cppreference.com/w/cpp/chrono/c/asctime) | 转换 `tm` 对象为文本表示 (函数)                   |
| [strftime](https://zh.cppreference.com/w/cpp/chrono/c/strftime) | 转换 `tm` 对象到自定义的文本表示 (函数)           |
| [wcsftime](https://zh.cppreference.com/w/cpp/chrono/c/wcsftime) | 转换 `tm` 对象为定制的宽字符串文本表示 (函数)     |
| [gmtime](https://zh.cppreference.com/w/cpp/chrono/c/gmtime)  | 转换纪元起时间为以协调世界时表示的日历时间 (函数) |
| [localtime](https://zh.cppreference.com/w/cpp/chrono/c/localtime) | 转换纪元起时间为以本地时间 time_t 表示的日历时间 std::tm (函数) |
| [mktime](https://zh.cppreference.com/w/cpp/chrono/c/mktime)  | 转换日历时间为纪元起的时间 (函数)                 |

##### 1. char* ctime( const [std::time_t](https://zh.cppreference.com/w/cpp/chrono/c/time_t)* time )

产生的字符串拥有如下格式： **Www Mmm dd hh:mm:ss yyyy**

- `Www` - 星期（ `Mon` 、 `Tue` 、 `Wed` 、 `Thu` 、 `Fri` 、 `Sat` 、 `Sun` 之一）。
- `Mmm` - 月份（ `Jan` 、 `Feb` 、 `Mar` 、 `Apr` 、 `May` 、 `Jun` 、 `Jul` 、 `Aug` 、 `Sep` 、 `Oct` 、 `Nov` 、 `Dec` 之一）。
- `dd` - 月之日
- `hh` - 时
- `mm` - 分
- `ss` - 秒
- `yyyy` - 年

**返回值**：指向空终止字符串的指针，字符串保有日期和时间的文本表示。字符串可能在 `std::asctime` 与 [std::ctime](https://zh.cppreference.com/w/cpp/chrono/c/ctime) 之间共享，而且可能在每次调用任何这些函数时被重写。

函数不支持本地化。

```cpp
time_t now;
time(&now);
std::cout << ctime(&now) << std::endl;
//Tue Jun 27 20:34:36 2023
```

##### 2.  char* asctime( const [std::tm](https://zh.cppreference.com/w/cpp/chrono/c/tm)* time_ptr )  已经废弃

转换给定日历时间 [std::tm](https://zh.cppreference.com/w/cpp/chrono/c/tm) 为拥有下列固定 25 字符形式的文本表示： **Www Mmm dd hh:mm:ss yyyy**

**返回值**：指向空终止字符串的指针，字符串保有日期和时间的文本表示。字符串可能在 `std::asctime` 与 [std::ctime](https://zh.cppreference.com/w/cpp/chrono/c/ctime) 之间共享，而且可能在每次调用任何这些函数时被重写。

此函数返回指向静态数据的指针，且非线程安全。 POSIX 标记此函数为过时，并推荐使用 [std::strftime](https://zh.cppreference.com/w/cpp/chrono/c/strftime) 替代。

```cpp
std::time_t result = std::time(nullptr);
std::cout << std::asctime(std::localtime(&result));
```

##### 3. [std::size_t](https://zh.cppreference.com/w/cpp/types/size_t) strftime( char* str, [std::size_t](https://zh.cppreference.com/w/cpp/types/size_t) count, const char* format, const [std::tm](https://zh.cppreference.com/w/cpp/chrono/c/tm)* time );

按照格式字符串 `format` ，转换来自给定的日历时间 `time` 的日期和时间信息，为空终止多字节字符串 `str` 。最多写入 `count` 字节。

```cpp
std::time_t t = std::time(nullptr);
char mbstr[100];
if (std::strftime(mbstr, sizeof(mbstr), "%Y/%m/%d %H:%M:%S", std::localtime(&t))) {
    std::cout << mbstr << '\n';
}//2023/06/27 20:47:10
```

##### 4. [std::size_t](https://zh.cppreference.com/w/cpp/types/size_t) wcsftime( wchar_t* str, [std::size_t](https://zh.cppreference.com/w/cpp/types/size_t) count, const wchar_t* format, const [std::tm](https://zh.cppreference.com/w/cpp/chrono/c/tm)* time );

按照格式字符串 `format` ，转换自给定的日历时间 `time` 的日期和时间信息，为空终止宽字符串 `str` 。最多写入 `count` 个宽字符。

```cpp
std::time_t t = std::time(NULL);
wchar_t wstr[100];
if(std::wcsftime(wstr, 100, L"%Y/%m/%d %H:%M:%S", std::localtime(&t))) {
    std::wcout << wstr << '\n';
}
```

##### 5. std::tm* gmtime( const std::time_t* time );

*gmtime*是把日期和时间转换为格林威治(GMT)时间的函数

```cpp
std::time_t t = std::time(nullptr);
std::cout << "GMT:" << std::put_time(std::gmtime(&t), "%Y/%m/%d %H:%M:%S") << '\n';
```



##### 6. [std::tm](https://zh.cppreference.com/w/cpp/chrono/c/tm)* localtime( const [std::time_t](https://zh.cppreference.com/w/cpp/chrono/c/time_t) *time );

转换作为 [std::time_t](https://zh.cppreference.com/w/cpp/chrono/c/time_t) 值的从纪元起时间到以本地时间表达的日历时。



##### 7. [std::time_t](https://zh.cppreference.com/w/cpp/chrono/c/time_t) mktime( [std::tm](https://zh.cppreference.com/w/cpp/chrono/c/tm)* time );

转换本地日历时间为从纪元起的时间，作为 time_t 对象。忽略 `time->tm_wday` 与 `time->tm_yday` 。容许 `time` 中的值在其正常范围外。





### [4.](#) 

### [5.](#) 

-----
时间: [] 