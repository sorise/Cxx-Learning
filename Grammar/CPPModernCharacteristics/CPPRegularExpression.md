### [C++ 正则表达式](#)

 **介绍**：字符串处理能力是所有程序员的基本功, C++11开始，引入对正则表达式的支持，使得C++具备了更多的现代语言特性，但是仅支持**char**和**wchar_t**。

-----
- [x] [1. 概述](#1-概述)
- [x] [2. basic_regex](#2-basic_regex)
- [x] [3. regex_error](#3-regex_error)
- [x] [4. match_results](#4-match_results)
- [x] [5. regex_iterator](#5-regex_iterator)
- [x] [6. 子表达式 sub_match](#6-子表达式-sub_match)
- [x] [7. 正则表达式算法](#7-正则表达式算法)
-----

### [1. 概述](#)
正则表达式于C++11加入，它是 **class std::basic_regex\<\>** 针对**char**类型的一个特化，还有一个针对**wchar_t**类型的特化为**std::wregex**。

- **目标序列**。为模式而搜索到的字符序列。这可以是二个迭代器所指定的范围、空终止字符串或一个 [std::string](https://zh.cppreference.com/w/cpp/string/basic_string) 。

- **模式**。这是正则表达式自身。它确定构成匹配者。它是从带特定语法的字符串构成的 [std::basic_regex](https://zh.cppreference.com/w/cpp/regex/basic_regex) 类型对象。受支持的语法变体的描述见 [syntax_option_type](https://zh.cppreference.com/w/cpp/regex/syntax_option_type) 。

- **匹配的数组**。关于匹配的信息可作为 [std::match_results](https://zh.cppreference.com/w/cpp/regex/match_results) 类型对象获取。

- **替换字符串**。这是确定如何替换匹配的字符串，受支持的语法变体的描述见 [match_flag_type](https://zh.cppreference.com/w/cpp/regex/match_flag_type) 。

**std::regex默认使用是ECMAScript文法**，这种文法比较好用，且威力强大，常用符号的意义如下：

| 符号  | 意义                                             |
| ----- | ------------------------------------------------ |
| ^     | 匹配行的开头  **类别：限定符**                   |
| $     | 匹配行的结尾   **类别：限定符**                  |
| .     | 匹配任意单个字符  **类别：限定符**               |
| […]   | 匹配[]中的任意一个字符                           |
| (…)   | 设定分组 **类别：限定符**                        |
| \     | 转义字符                                         |
| \d    | 匹配数字[0-9]                                    |
| \D    | \d 取反                                          |
| \w    | 匹配字母[a-z]，数字，下划线                      |
| \W    | \w 取反                                          |
| \s    | 匹配空格                                         |
| \S    | \s 取反                                          |
| +     | 前面的元素重复1次或多次  **类别：限定符**        |
| *     | 前面的元素重复任意次  **类别：限定符**           |
| ?     | 前面的元素重复0次或1次  **类别：限定符**         |
| {n}   | 前面的元素重复n次  **类别：限定符**              |
| {n,}  | 前面的元素重复至少n次  **类别：限定符**          |
| {n,m} | 前面的元素重复至少n次，至多m次  **类别：限定符** |
| \|    | 逻辑或  **类别：限定符**                         |
| \b    | 匹配一个单词边界，即字与空格间的位置。           |
| \B    | 非单词边界匹配。                                 |

**更多支持可以查看ECMAScript正则表达式的支持**

#### [1.1 修饰符](#)
标记也称为修饰符，正则表达式的标记用于指定额外的匹配策略。

| 修饰符 | 含义                                   | 描述                                                         |
| :----- | :------------------------------------- | :----------------------------------------------------------- |
| i      | ignore - 不区分大小写                  | 将匹配设置为不区分大小写，搜索时不区分大小写: A 和 a 没有区别。 |
| g      | global - 全局匹配                      | 查找所有的匹配项。                                           |
| m      | multi line - 多行匹配                  | 使边界字符 **^** 和 **$** 匹配每一行的开头和结尾，记住是多行，而不是整个字符串的开头和结尾。 |
| s      | 特殊字符圆点 **.** 中包含换行符 **\n** | 默认情况下的圆点 **.** 是匹配除换行符 **\n** 之外的任何字符，加上 **s** 修饰符之后, **.** 中包含换行符 \n。 |

#### [1.2 非打印字符](#)
非打印字符也可以是正则表达式的组成部分。下表列出了表示非打印字符的转义序列：

| 字符 | 描述                                                         |
| :--- | :----------------------------------------------------------- |
| \cx  | 匹配由x指明的控制字符。例如， \cM 匹配一个 Control-M 或回车符。x 的值必须为 A-Z 或 a-z 之一。否则，将 c 视为一个原义的 'c' 字符。 |
| \f   | 匹配一个换页符。等价于 \x0c 和 \cL。                         |
| \n   | 匹配一个换行符。等价于 \x0a 和 \cJ。                         |
| \r   | 匹配一个回车符。等价于 \x0d 和 \cM。                         |
| \s   | 匹配任何空白字符，包括空格、制表符、换页符等等。等价于 [ \f\n\r\t\v]。注意 Unicode 正则表达式会匹配全角空格符。 |
| \S   | 匹配任何非空白字符。等价于 [^ \f\n\r\t\v]。                  |
| \t   | 匹配一个制表符。等价于 \x09 和 \cI。                         |
| \v   | 匹配一个垂直制表符。等价于 \x0b 和 \cK。                     |

#### [1.3 字符簇](#)
在 INTERNET 的程序中，正则表达式通常用来验证用户的输入。当用户提交一个 FORM 以后，要判断输入的电话号码、地址、EMAIL 地址、信用卡号码等是否有效，用普通的基于字面的字符是不够的。

| 字符簇       | 描述                                |
| :----------- | :---------------------------------- |
| [[:alpha:]]  | 任何字母                            |
| [[:digit:]]  | 任何数字                            |
| [[:alnum:]]  | 任何字母和数字                      |
| [[:space:]]  | 任何空白字符                        |
| [[:upper:]]  | 任何大写字母                        |
| [[:lower:]]  | 任何小写字母                        |
| [[:punct:]]  | 任何标点符号                        |
| [[:xdigit:]] | 任何16进制的数字，相当于[0-9a-fA-F] |

#### [1.4 C++ 正则表达式组件](#)
在 C++正则表达式库(RE库)中。RE库，定义在头文件 "regex"内，命名空间:std;

| class           | 说明                                                         |
| --------------- | ------------------------------------------------------------ |
| regex           | 表示有一个正则表达式的类;                                    |
| regex_match     | 将一个字符序列与一个正则表达式匹配; **算法**！               |
| regex_search    | 寻找第一个与正则表达式匹配的子序列; **算法**！               |
| regex_replace   | 使用给定格式替换一个正则表达式; **算法**！                   |
| sregex_iterator | 迭代器适配器，调用regex_search 来遍历一个string中所有匹配的子串; |
| smatch          | 容器类，保存在string中搜索的结果;                            |
| ssub_match      | string中匹配的子表达式的结果。                               |

#### [1.5 占位符的使用](#)

$1，$2表达的是小括号里面的内容，$1是第一个小括号里的内容，$2是第二个小括号里面的内容，依此类推

比如(\d{4})(\d{2})(\d{2}) 匹配"20190919"

$1是第一个括号里匹配的2019
$2是第二个括号里匹配的09
$3是第三个括号里匹配的19
$& 表示匹配本身 20190919

#### [1.6 简单使用](#)

简单的匹配一个 **friend**。

```cpp
std::string pattern{ "[[:alpha:]]*[^c]ei[[:alpha:]]*" };
std::smatch result; //存储匹配结果
std::regex regular(pattern, std::regex::icase); 
//构造一个用于查找的regx, 忽略大小写

std::string test_str = "receipt freind theif receive";
if (std::regex_search(test_str, result, regular)) //如果匹配成功
{
    std::cout << result.str() << std::endl;
}//freind
```

### [2.  basic_regex](#) 

类模板 **basic_regex** 提供保有正则表达式的通用框架。

```cpp
template <
    class CharT,
    class Traits = std::regex_traits<CharT>
> class basic_regex;
```

C++11，提供数个对于常用字符类型的 **typedef**，两者只有字符的不同，操作都是一样的！

| 类型            | 定义                        | 字符    | 说明                                      |
| :-------------- | :-------------------------- | :------ | :---------------------------------------- |
| **std::regex**  | std::basic_regex\<char\>    | char    | 一字节八位一个字符                        |
| **std::wregex** | std::basic_regex\<wchar_t\> | wchar_t | 宽字符，wchar_t数据类型一般为16位或32位。 |

#### [2.1 构造与重设](#)

| 函数                | 说明                                                         |
| ------------------- | ------------------------------------------------------------ |
| **regex(re)**       | **re**表示一个正则表达式，有字符串表示。                     |
| **regex(re, f)**    | **re**表示一个正则表达式，有字符串表示。**f** 是标志位，可以设置 |
| **r.assign(re, f)** | 重新设置**re**,和**f**。                                     |

```cpp
std::regex regular("[a-z]*[^c]ei[a-z]*", std::regex::icase);
```

**f: 标志位**定义在regex和std::regex_constants::syntax_option_type中！

| 标志              | 说明                                                         |
| ------------------- | ------------------------------------------------------------ |
| icase             | 应当以不考虑大小写进行字符匹配。                             |
| nosubs          | 不保存匹配的子表达式 |
| optimize          | 指示正则表达式引擎进行更快的匹配，带有令构造变慢的潜在开销。 |
| collate           | 形如 *"[a-b]"* 的字符范围将对本地环境敏感。                  |
| multiline (C++17) | 若选择 ECMAScript 引擎，则指定 ^ 应该匹配行首，而 $ 应该匹配行尾。 |
| ECMAScript        | 使用[改 ECMAScript 正则表达式文法](https://zh.cppreference.com/w/cpp/regex/ecmascript)  ，**默认设置** |
| basic             | 使用基本 POSIX 正则表达式文法（[文法文档](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap09.html#tag_09_03)）。 |
| extended          | 使用扩展 POSIX 正则表达式文法（[文法文档](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap09.html#tag_09_04)）。 |
| awk               | 使用 POSIX 中 *awk* 工具所用的正则表达式文法（[文法文档](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/awk.html#tag_20_06_13_04)）。 |
| grep              | 使用 POSIX 中 *grep* 工具所用的正则表达式文法。这等效于 basic 选项附带作为另一种分隔符的换行符 '\n' 。 |
| egrep             | 使用 POSIX 中 *grep* 工具带 *-E* 选项所用的正则表达式文法。这等效于 extended 附带 '\|' 之外的作为另一种分隔符的换行符 '\n' 。 |

#### [2.2 观察器](#)

展示正则表达式本身的信息：

| 方法                                                         | 说明                                                |
| ------------------------------------------------------------ | --------------------------------------------------- |
| [mark_count()](https://zh.cppreference.com/w/cpp/regex/basic_regex/mark_count) | 返回正则表达式中有标记的子表达式数量 (公开成员函数) |
| [flags()](https://zh.cppreference.com/w/cpp/regex/basic_regex/flags) | 返回语法标志                                        |

#### [2.3 特殊字符的处理](#)

例如，如果需要匹配在字符串中出现的 **.** 的时候，需要两个下划线！

```cpp
std::string file_name;
std::smatch result;
printf("please input file name:");
std::regex regular("[a-z]+\\.(cpp|cxx|cc|hpp|h)", std::regex::icase);
while (std::cin >> file_name)
{
    if (std::regex_search(file_name, result, regular)) {
        std::cout << "belong to c++: " << result.str() << std::endl;
    }
    else {
        std::cout << "not c++: " << file_name << std::endl;
    }

    printf("please input file name:");
}
```

### [3. regex_error](#) 

如果我们编写的正则表达式存在错误，则在运行时标准库会抛出一个类型为 **regex_error** 的异常！它继承自**runtime_error**l，而 **runtime_error** 继承自**exception**。

定义抛出的异常对象类型，以报告正则表达式库中的错误。

多了一个[**code()**](https://zh.cppreference.com/w/cpp/regex/regex_error/code)方法，描述不同类型的匹配错误！这些类型定义在 [std::regex_constants::error_type](https://zh.cppreference.com/w/cpp/regex/error_type)中！

| 常量               | 解释                                       |
| ------------------ | ------------------------------------------ |
| error_collate      | 表达式含有非法对照字符名                   |
| error_ctyp         | 表达式含有非法字符类名                     |
| error_escape     | 表达式含有非法转义字符或尾随转义           |
| error_backref    | 表达式含有非法回溯引用                     |
| error_brack      | 表达式含有不匹配的方括号对（ '[' 与 ']' ） |
| error_paren      | 表达式含有不匹配的括号对（ '(' 与 ')' ）   |
| error_brace      | 表达式含有不匹配的花括号对（ '{' 与 '}' ） |
| error_badbrace   | 表达式在 {} 表达式中含有非法范围           |
| error_range      | 表达式含有非法字符范围（例如 [b-a] ）      |
| error_space      | 没有将表达式转换成有限状态机的足够内存     |
| error_badrepeat  | *?+{ 之一不后继一个合法正则表达式          |
| error_complexity | 尝试的匹配的复杂度超过了预定义的等级       |
| error_stack      | 没有进行匹配的足够内存                     |

下面的正则表达式前面少了一个 **`]`** 符号！

```cpp
std::regex regular;
try
{
	regular.assign("[a-z+\\.(cpp|cxx|cc|hpp|h)$", std::regex::icase);
}
catch (const std::regex_error& e)
{
	std::cout << "\nerror ------------- error\n" << e.what() << std::endl;
	//regex_error(error_brack): The expression contained mismatched [ and ].
	std::cout << e.code() << std::endl; //4
}
```

### [4. match_results](#) 

类模板 **std::match_results** 保有表示正则表达式匹配结果的字符序列汇集。

```cpp
template<
    class BidirIt,
    class Alloc = std::allocator<std::sub_match<BidirIt>>
> class match_results;
```

**pmr 命名空间**

```cpp
namespace pmr {
    template <class BidirIt>
    using match_results = std::match_results<BidirIt,
                              std::pmr::polymorphic_allocator<
                                  std::sub_match<BidirIt>>>;
}
```

提供几个对常用字符类型的特化：

| 类型                      | 定义                                                         |
| ------------------------- | ------------------------------------------------------------ |
| std::cmatch               | std::match_results<const char*>  **字符数组**                |
| std::wcmatch              | std::match_results<const wchar_t*> **宽字符数组**            |
| std::smatch               | std::match_results\<std::string::const_iterator\>  **表示输入是字符串** |
| std::wsmatch              | std::match_results\<std::wstring::const_iterator\> **表示输入是宽字符串** |
| std::pmr::cmatch (C++17)  | std::pmr::match_results<const char*>                         |
| std::pmr::wcmatch (C++17) | std::pmr::match_results<const wchar_t*>                      |
| std::pmr::smatch (C++17)  | std::pmr::match_results\<std::string::const_iterator\>       |
| std::pmr::wsmatch(C++17)  | std::pmr::match_results\<std::wstring::const_iterator\>      |

```cpp
std::cmatch result;
if (std::regex_search("receipt freind theif receive", result , regular)) //如果匹配成功
{
    std::cout << result.str() << std::endl;
}//freind
```

#### [4.1 配对](#)

| 输入序列        | 定义                                           |
| --------------- | ---------------------------------------------- |
| string          | regex、smatch、ssub_match、sregex_iterator     |
| const char *    | regex、cmatch、csub_match、cregex_iterator     |
| wstring         | wregex、wsmatch、wssub_match、wsregex_iterator |
| const wchar_t * | wregex、wcmatch、wcsub_match、wcregex_iterator |

#### [4.2 上下文](#)

有时候我们会需要匹配的上下文内容，即匹配的前面部分和后面部分用于展示定义！

| 方法                                                         | 说明                                                        | 返回类型                                                     |
| ------------------------------------------------------------ | ----------------------------------------------------------- | ------------------------------------------------------------ |
| [prefix()](https://zh.cppreference.com/w/cpp/regex/match_results/prefix) | 返回目标序列起始和完整匹配起始之间的子序列。 (公开成员函数) | 获得到 [std::sub_match](https://zh.cppreference.com/w/cpp/regex/sub_match) 对象的引用 |
| [suffix()](https://zh.cppreference.com/w/cpp/regex/match_results/suffix) | 返回完整匹配结尾和目标序列结尾之间的子序列 (公开成员函数)   | 获得到 [std::sub_match](https://zh.cppreference.com/w/cpp/regex/sub_match) 对象的引用 |

```cpp
std::regex re("a(a)*b");
std::string target("qqqbaaabyyyy");
std::smatch sm;

std::regex_search(target, sm, re);
std::cout << sm.prefix().str() << '\n'; //qqqb
std::cout << sm.suffix().str() << '\n'; //yyyy
```

#### [4.3 状态](#)

| 方法                                                         | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [ready()](https://zh.cppreference.com/w/cpp/regex/match_results/ready) | 若 match_results 就绪则为 true ，否则为 false ，就是是否接收到了匹配结果！ |

```cpp
std::string target("pattern");
std::smatch sm;
std::cout << "default constructed smatch is "
          << (sm.ready() ? " ready\n" : " not ready\n");

std::regex re1("tte");
std::regex_search(target, sm, re1);

std::cout << "after search, smatch is "
          << (sm.ready() ? " ready\n" : " not ready\n");
```

#### [4.4 大小](#)

| 方法                                                         | 说明                                            |
| ------------------------------------------------------------ | ----------------------------------------------- |
| [ empty](https://zh.cppreference.com/w/cpp/regex/match_results/empty) | 检查匹配是否成功 (公开成员函数)                 |
| [size](https://zh.cppreference.com/w/cpp/regex/match_results/size) | 返回完全建立的结果状态中的匹配数 (公开成员函数) |
| [max_size](https://zh.cppreference.com/w/cpp/regex/match_results/max_size) | 返回子匹配的最大可能数量 (公开成员函数)         |

#### [4.5 元素访问](#)

| 方法                                                         | 说明                                      |
| ------------------------------------------------------------ | ----------------------------------------- |
| [length](https://zh.cppreference.com/w/cpp/regex/match_results/length) | 返回特定子匹配的长度 (公开成员函数)       |
| [position](https://zh.cppreference.com/w/cpp/regex/match_results/position) | 返回特定子匹配首字符的位置 (公开成员函数) |
| [str(idx)](https://zh.cppreference.com/w/cpp/regex/match_results/str) | 返回特定子匹配的字符序列 (公开成员函数)   |
| operator[]                                                   | 返回指定的子匹配 (公开成员函数)           |

#### [4.6 迭代器](#)

| 方法                                                         | 说明                                          |
| ------------------------------------------------------------ | --------------------------------------------- |
| [begincbegin](https://zh.cppreference.com/w/cpp/regex/match_results/begin) | 返回指向子匹配列表起始的迭代器 (公开成员函数) |
| [endcend](https://zh.cppreference.com/w/cpp/regex/match_results/end) | 返回指向子匹配列表末尾的迭代器 (公开成员函数) |

#### [4.7 format](#)

`format` 输出格式字符串，它将字符串中的任何格式指定符或转义序列替换为来自 *this 的数据。

```cpp
#include <iostream>
#include <string>
#include <regex>
 
int main()
{
    std::string s = "for a good time, call 867-5309";
    std::regex phone_regex("\\d{3}-\\d{4}");
    std::smatch phone_match;
 
    if (std::regex_search(s, phone_match, phone_regex)) {
        std::string fmt_s = phone_match.format(
            "$`"    // $` 意味着匹配之前的字符
            "[$&]"  // $& 意味着匹配的字符
            "$'");  // $' 意味着后随匹配的字符
        std::cout << fmt_s << '\n';
    }   
}
```

### [5. regex_iterator](#) 

**regex_iterator** 用于遍历在序列中找到的匹配正则表达式的整个集合。

```cpp
template<
    class BidirIt,
    class CharT = typename std::iterator_traits<BidirIt>::value_type,
    class Traits = std::regex_traits<CharT>
> class regex_iterator
```

提供对于常用字符序列类型的数个特化：

| 类型             |                                                |
| ---------------- | ---------------------------------------------- |
| cregex_iterator  | regex_iterator<const char*>                    |
| wcregex_iterator | regex_iterator<const wchar_t*>                 |
| sregex_iterator  | regex_iterator\<std::string::const_iterator\>  |
| wsregex_iterator | regex_iterator\<std::wstring::const_iterator\> |

#### [5.1 简单使用](#)

输出全部匹配到的选项！

```cpp
const std::string s = "Quick brown fox.";

std::regex words_regex("[^\\s]+");
std::sregex_iterator words_begin(s.begin(), s.end(), words_regex);
//words_begin  被初始化为 匹配到的第一个位置
auto words_end = std::sregex_iterator();

std::cout << "Found " 
          << std::distance(words_begin, words_end) 
          << " words:\n";

for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;                                                 
    std::string match_str = match.str(); //match类型为match_results
    std::cout << match_str << '\n';
}  
```

#### [5.2 支持操作](#)

| 操作                                                         | 说明                                     |
| ------------------------------------------------------------ | ---------------------------------------- |
| [operator=](https://zh.cppreference.com/w/cpp/regex/regex_iterator/operator%3D) | 赋值内容 (公开成员函数)                  |
| [operator==、operator!=](https://zh.cppreference.com/w/cpp/regex/regex_iterator/operator_cmp)(C++20 中移除) | 比较两个 `regex_iterator` (公开成员函数) |
| [operator*、operator->](https://zh.cppreference.com/w/cpp/regex/regex_iterator/operator*) | 访问当前匹配 (公开成员函数)              |
| [operator++、operator++(int)](https://zh.cppreference.com/w/cpp/regex/regex_iterator/operator_arith) | 推进迭代器到下一个匹配 (公开成员函数)    |

### [6. 子表达式 sub_match](#)

正则表达式中的模式通常包含一个或多个子表达式，一个子表达式就是模式的一部分！

```cpp
template<class BidirIt>
class sub_match;
```

为常见字符类型提供数个特化：

| 类型        | 定义                                      |
| ----------- | ----------------------------------------- |
| csub_match  | sub_match<const char*>                    |
| wcsub_match | sub_match<const wchar_t*>                 |
| ssub_match  | sub_match\<std::string::const_iterator\>  |
| wssub_match | sub_match\<std::wstring::const_iterator\> |

#### [6.1 子表达式](#)

就是正则表达式中用括号括起来的部分！

```cpp
//匹配浮点数，然后将小数点 前后部分交换位置
std::regex re("(-?\\d+)\\.(\\d+)?", std::regex::icase);
std::string number{ "1911.5684 1111.5684 2911.5684" };
std::string replace_str = std::regex_replace(number, re, "$2.$1");

std::cout << replace_str << std::endl;
//5684.1911 5684.1111 5684.2911
```

**输出子表达式匹配结果**

```cpp
std::regex re("(-?\\d+)\\.(\\d+)?", std::regex::icase);
std::string number{ "1911.5684 1111.5684 2911.5684" };
std::smatch result;
std::regex_search(number, result, re);

std::cout << result.str() << std::endl;//1911.5684
std::cout << result.str(0) << std::endl;//1911.5684
std::cout << result.str(1) << std::endl;//1911
std::cout << result.str(2) << std::endl;//1911
```

**可以使用索引 返回第idx个子匹配**

```cpp
std::cout << result[0].str() << std::endl;//1911.5684
std::cout << result[1].str() << std::endl;//1911
std::cout << result[1].length() << std::endl;//4
std::cout << result[1].matched << std::endl;//1
```

### [7. 正则表达式算法](#)

C++ 正则表达式提供了三个算法！

| class                                        | 说明                                           | 返回值 |
| -------------------------------------------- | ---------------------------------------------- | ------ |
| regex_match(sequence, result , regex, flag)  | 将一个字符序列与一个正则表达式匹配; **算法**！ | bool   |
| regex_search(sequence, result , regex, flag) | 寻找第一个与正则表达式匹配的子序列; **算法**！ | bool   |
| regex_replace                                | 使用给定格式替换一个正则表达式; **算法**！     |        |

将一个字符序列与一个正则表达式匹配;: 

```cpp
std::regex re("^\\d{5,6}$", std::regex::icase);
std::string number{ "1912314" };
std::smatch result;
bool isSuccess = std::regex_match(number, result, re);
std::cout << isSuccess << std::endl; //0
std::cout << result.str() << std::endl;// 
```

寻找第一个匹配的:

```cpp
std::regex re("\\d{5,6}$", std::regex::icase);
std::string number{ "1912314" };
std::smatch result;
bool isSuccess =  std::regex_search(number, result, re);
std::cout << isSuccess << std::endl; //1
std::cout << result.str() << std::endl; //912314
```

#### [7.1 regex_replace](#)

正则表达式不仅用在我们希望查找一个给定序列的时候，还可以用在**替换内容上面**， C++ 提供了**regex_replace** 算法！

```cpp
template< class OutputIt, class BidirIt,
          class Traits, class CharT,
          class STraits, class SAlloc >
OutputIt regex_replace( OutputIt out, BidirIt first, BidirIt last,
                        const std::basic_regex<CharT,Traits>& re,
                        const std::basic_string<CharT,STraits,SAlloc>& fmt,
                        std::regex_constants::match_flag_type flags =
                            std::regex_constants::match_default );

template< class Traits, class CharT,
          class STraits, class SAlloc >
std::basic_string<CharT,STraits,SAlloc>
    regex_replace( const std::basic_string<CharT,STraits,SAlloc>& s,
                   const std::basic_regex<CharT,Traits>& re,
                   const CharT* fmt,
                   std::regex_constants::match_flag_type flags =
                       std::regex_constants::match_default );
```

**使用例子：**

```cpp

std::regex re("\\d{5,6}", std::regex::icase);
std::string number{ "1912314 a5465756 78244 48634" };

std::cout << std::regex_replace(number, re, "[over]") << std::endl; 
//[over]4 a[over]6 [over] [over]
```



#### [7.2 match_flag_type](#)

[ std::regex_constants::match_flag_type](https://zh.cppreference.com/w/cpp/regex/match_flag_type) , 是指定附加正则表达式匹配选项的位掩码类型 (BitmaskType) 

| 常量              | 解释                                                         |
| ----------------- | ------------------------------------------------------------ |
| match_default     | 等价于format_default！                                       |
| match_not_bol     | *[first,last)* 中的首个字符将被处理成如同它**不**在行首（即 *^* 将不匹配 *[first,first)* ） |
| match_not_eol     | *[first,last)* 中的最末字符将被处理成如同它**不**在行尾（即 *$* 将不匹配 *[last,last)* ） |
| match_not_bow     | *"\b"* 将不匹配 *[first,first)*                              |
| match_not_eow     | *"\b"* 将不匹配 *[last,last)*                                |
| match_any         | 若多于一个匹配可行，则任何匹配都是可接受的结果               |
| match_not_null    | 不匹配空字符序列                                             |
| match_continuous  | 仅匹配始于 *first* 的子串                                    |
| match_prev_avail  | *--first* 是合法的迭代位置。设置时导致 *match_not_bol* 和 *match_not_bow* 被忽略 |
| format_default    | 使用 ECMAScript 规则于 [std::regex_replace](https://zh.cppreference.com/w/cpp/regex/regex_replace) 构造字符串（[语法文档](http://ecma-international.org/ecma-262/5.1/#sec-15.5.4.11)） |
| format_sed        | 于 [std::regex_replace](https://zh.cppreference.com/w/cpp/regex/regex_replace) 使用 POSIX *sed* 工具规则。（[语法文档](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/sed.html#tag_20_116_13_03)） |
| format_no_copy    | 不复制不匹配的字符串到 [std::regex_replace](https://zh.cppreference.com/w/cpp/regex/regex_replace) 中的输出 |
| format_first_only | 仅替换 [std::regex_replace](https://zh.cppreference.com/w/cpp/regex/regex_replace) 中的首个匹配 |

```cpp
std::regex re("\\d{5,6}", std::regex::icase);
std::string number{ "1912314 a5465756 78244 48634" };

std::cout << std::regex_replace(number, re, "$& ",  std::regex_constants::format_no_copy) << std::endl;
//191231 546575 78244 48634
```








-----
时间: [] 
