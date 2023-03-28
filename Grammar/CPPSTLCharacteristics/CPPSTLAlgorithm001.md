### [C++ STL 算法](#)
`算法库提供大量用途的函数（例如查找、排序、计数、操作），它们在元素范围上操作。注意范围定义为 [first, last) ，其中 last 指代要查询或修改的最后元素的后一个元素。 这里我们介绍一些常用的算法，具体需要可以查询官方文档！ `

-----

- [x] [1. 常用遍历算法](#1-常用遍历算法)
- [x] [2. 常用查找算法](#2-常用查找算法)
- [x] [3. 生成值算法](#3-生成值算法)
- [x] [4. 常用排序算法](#4-常用排序算法)
- [x] [5. 常用拷贝和替代算法](#5-常用拷贝和替代算法)
- [x] [6. 删除算法](#6-删除算法)
- [x] [7. 集合算法](#7-集合算法)
- [x] [8. 数值算法](#8-最值算法)
- [x] [9. 配套算法](#9-配套算法)


-----

### [1. 常用遍历算法](#)
用于遍历容器里面的元素，需要迭代器支持 `++` 操作符。 有 for_each, for_each_n

#### [1.1 for_each](#)
按顺序应用给定的函数对象 f 到解引用范围 [first, last) 中每个迭代器的结果, UnaryFunction, 可以按值传递，也可以按引用传递。

```cpp
template< class InputIt, class UnaryFunction >
constexpr UnaryFunction for_each( InputIt first, InputIt last, UnaryFunction f );
```
`实验：`
```cpp
std::vector<double> scores{31.56, 40.2, 82.2, 88.95, 75.1, 67.89}; 
std::for_each(scores.begin(), scores.end(), [](double & v){v = int(v);});

for(auto v :  scores){
    printf("%.2lf ", v);
}
printf("\n");
//31.00 40.00 82.00 88.00 75.00 67.00 
```

#### [1.2 for_each_n 按数量遍历](#)
按顺序应用给定的函数对象 `f` 到解引用范围 `[first, first + n)` 中每个迭代器的结果

```cpp
template< class InputIt, class Size, class UnaryFunction >
InputIt for_each_n( InputIt first, Size n, UnaryFunction f );
```

`实验：`
```cpp
std::vector<double> scores{31.56, 40.2, 82.2, 88.95, 75.1, 67.89}; 
std::for_each_n(scores.begin(), 3, [](double& n){ n /= 10.0; });

for(auto v :  scores){
    printf("%.2lf ", v);
}
printf("\n");
//3.16 4.02 8.22 88.95 75.10 67.89 
```

#### [1.3 transform 遍历插入](#)
将一个函数应用于某一范围的各个元素，并在目标容器的范围存储结果！  **目标容器需要提前开辟好空间！**
```cpp
template< class InputIt, class OutputIt, class UnaryOperation >
OutputIt transform( InputIt first1, InputIt last1,
                    OutputIt d_first, UnaryOperation unary_op );
```

`实验:`
```cpp
//原始数据
std::vector<std::string> names {"kciker", "weriy", "cicer", "youmi", "jiangxing" };

//提前开辟空间
std::vector<int> lengthOfnames( names.size());
//转移数据
std::transform(names.begin(), names.end(), lengthOfnames.begin(), [](std::string name){
    return name.size();
});
//打印以下结果！
for (auto len : lengthOfnames)
{   
    std::cout << len << " ";
}
std::cout << std::endl;
//6 5 5 5 9
```

### [2. 常用查找算法](#) 
C++ STL 标准库提供了较多的查找算法，有 **find、** **find_if、** **find_if_not、**  **adjacent_find、**  **binary_search、** **count、count_if** **...** 

#### [2.1 find 按值查找](#)
返回指向范围 **[first, last)** 中满足特定判别标准的首个元素的迭代器, **如果容器里面存储的是对象，那么这个类必须重载运算符 operator== 不然将会报错！**

```cpp
template< class InputIt, class T >
InputIt find( InputIt first, InputIt last, const T& value );
```

**重载==运算符**
```cpp
bool MyTime::operator==(const MyTime & t){
    if (hour == t.hour && minutes == t.minutes)
    {
        return true;
    }
    return false;
}
```
**查找对象**
```cpp
int main(int argc, char *argv[]){
   std::vector<MyTime> Timers {MyTime(12,12), MyTime(10,12), MyTime(10,35), MyTime(22,10)};

   std::vector<MyTime>::iterator it = std::find(Timers.begin(), Timers.end(), MyTime(10, 12));

   if (it ==  Timers.end())
   {
      printf("not found 404!\n");
   }else{
      std::cout << "i found it: " << (*it).ToString() << "\n";
   }
   //i found it:10:12
   return 0;
}
```

**如果是其他类型，那么就很简答了**
```cpp
std::vector<double> scores{31.56, 40.2, 82.2, 88.95, 75.1, 67.89}; 

auto it = std::find(scores.begin(), scores.end(), 40.2);

if (it == scores.end())
{
    printf("not find");
}else {
    printf("i find it %.3lf\n", *it);
}
//i find it 40.200
```

#### [2.2 find_if](#)
返回指向范围 `[first, last)` 中满足一元谓词特定判别标准的首个元素的迭代器! `一元谓词`参数可以是 lambda、函数对象、函数指针

```cpp
std::vector<double> scores{31.56, 40.2, 82.2, 88.95, 75.1, 67.89}; 

auto it = std::find_if(scores.begin(), scores.end(), [](double &v){
    return v >= 85.0;
});

if (it == scores.end())
{
    printf("not find");
}else {
    printf("i find it %.3lf\n", *it);
}
//i find it 88.950
```

#### [2.3 find_if_not](#)
`find_if_not` 搜索谓词 `q` 对其返回 `false` 的元素! **有点反直觉，有了 find_if 感觉没必要 再弄一个 find_if_not**

```cpp
template< class InputIt, class UnaryPredicate >
constexpr InputIt find_if_not( InputIt first, InputIt last,
                               UnaryPredicate q );
```

#### [2.4 adjacent_find 相邻重复](#)
在范围 [first, last) 中搜索两个连续的相等元素。 并且返回相邻元素的第一个位置的迭代器

**用 operator== 比较元素** `如果是类类型需要重载 operator==`
```cpp
template< class ForwardIt >
ForwardIt adjacent_find( ForwardIt first, ForwardIt last );
```
**用给定的二元谓词 p 比较元素。**
```cpp
template< class ForwardIt, class BinaryPredicate>
ForwardIt adjacent_find( ForwardIt first, ForwardIt last, BinaryPredicate p );
```

`== 比较元素：`
```cpp
std::list<int> vals = {89,87,56,56,89,45,7,25};

auto it =  std::adjacent_find(vals.begin(), vals.end());

std::cout << *it << "\n";
//56
```

`二元谓词：`
```cpp
std::list<int> vals = {89,87,46,-46,89,45,7,25};

//绝对值相等也相邻
auto it =  std::adjacent_find(vals.begin(), vals.end(), [](auto &v, auto &t){
    if(abs(v) == abs(t)) return true;
    return false;
});

std::cout << *it << "\n";
//46
```

#### [2.5 binary_search 对有序序列二分查找](#)
检查等价于 value 的元素是否出现在范围 [first, last) 中出现。**返回布尔值** 找到等于 value 的元素时返回 true，否则返回 false。
**必须在有序序列中使用这个算法！**

```cpp
//使用 operator== 判断
template< class ForwardIt, class T >
bool binary_search( ForwardIt first, ForwardIt last, const T& value );
```

`operator== 判断`
```cpp
std::vector<int> numbers{1, 3, 4, 5, 9};
std::vector<int> search{7, 2, 3};

for (auto n : search)
{
    std::cout << "正在搜索 " << n << '\n';

    if (std::binary_search(numbers.begin(), numbers.end(), n)){
        std::cout << "找到了 " << n << '\n';
    }
    else{
        std::cout << "没找到！\n";
    }
}
```

#### [2.6 count 按值统计](#)
返回范围 [first, last) 中与值相等的元素数。

```cpp
//按值统计
template< class InputIt, class T >
typename iterator_traits<InputIt>::difference_type
    count( InputIt first, InputIt last, const T &value );
```

```cpp
std::list<int> vals = {89,87,56,56,89,45,7,25,56,78,56};

int count_56 =  std::count(vals.begin(), vals.end(), 56);

printf("共有: %d 个 56 存在 list中!\n", count_56);
//共有: 4 个 56 存在 list中!
```


#### [2.7 count_if 条件统计](#)
返回范围 [first, last) 中满足特定谓词判别标准的元素数。

```cpp
//按照一元谓词统计
template< class InputIt, class UnaryPredicate >
typename iterator_traits<InputIt>::difference_type
    count_if( InputIt first, InputIt last, UnaryPredicate p );
```

`实验 count_if`
```cpp
std::vector<MyTime> Timers(0); 
Timers.emplace_back(7,12);
Timers.emplace_back(8,41);
Timers.emplace_back(9,23);
Timers.emplace_back(11,11);

int count_8 =  std::count_if(Timers.begin(), Timers.end(),  [](MyTime &t){
    if(t.getHour() > 8){
        return true;
    }
    return false;
});

printf("大于八点的时间有: %d 个\n", count_8);
#大于八点的时间有: 2 个
```

#### [2.8 search 查找子序列](#)
搜索范围 `[first, last - (s_last - s_first))` 中元素子序列 `[s_first, s_last)` 的首次出现位置。
**返回值**：指向范围中 [first, last - (s_last - s_first)) ，首个子序列 [s_first, s_last) 起始的迭代器。若找不到这种子序列，则返回 last 。

**两个函数重载**  `元素用 operator== 比较`
```cpp
template< class ForwardIt1, class ForwardIt2 >
constexpr ForwardIt1 search( ForwardIt1 first, ForwardIt1 last,
                             ForwardIt2 s_first, ForwardIt2 s_last );
```

**元素用给定的二元谓词 p 比较。**
```cpp
template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
ForwardIt1 search( ForwardIt1 first, ForwardIt1 last,
                   ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p );
```

`实验 - operator == `
```cpp
std::list<int> vals = {1,2,3,4,5,6,7,8,9,10};
std::list<int> vals_son = {5,6,7,8};

auto it =  std::search(vals.begin(), vals.end(),  vals_son.begin(), vals_son.end());
if (it == vals.end())
{
    printf("not found the son sequence\n");
}else{
    printf("we find the son sequence, which start with: %d\n", *it);
}
//we find the son sequence, which start with: 5
```

#### [2.9 all_of 全部满足谓词集合](#)
检查一元谓词 p 是否对范围 [first, last) 中所有元素返回 true 。 **返回值**  若一元谓词对范围中所有元素返回 true 则为 true ，否则为 false 。若范围为空则返回 true 。

```cpp
std::list<double> scores {31.56, 40.2, 82.2, 88.95, 95.1, 67.89};
//是否所有人都及格了
bool isAllPass = std::all_of(scores.begin(), scores.end(), [](auto &v)->bool{return v >= 60.0;});
if (!isAllPass)
{
    printf("that is not all student pass the examine!\n");
}
```

#### [2.10 any_of 只要满足谓词集合](#)
检查一元谓词 p 是否对范围 [first, last) 中至少一个元素返回 true 。 **返回值** 若一元谓词对范围中至少一个元素返回 true 则为 true ，否则为 false 。若范围为空则返回 false 。

```cpp
std::list<double> scores {31.56, 40.2, 82.2, 88.95, 95.1, 67.89};
//是否所有人都及格了
bool moreThan95 = std::any_of(scores.begin(), scores.end(), [](auto &v)->bool{return v < 60.0;});
if (moreThan95)
{
    printf("the score > 95.0 is exists!\n");
}
//the score > 95.0 is exists!
```

#### [2.11 none_of 无满足谓词集合](#)
检查一元谓词 p 是否不对范围 [first, last) 中任何元素返回 true 。 **返回值** 若一元谓词不对范围中任何元素返回 true 则为 true ，否则为 false 。若范围为空则返回 true

**检测是否有分数低于60的！**
```cpp
std::list<double> scores {31.56, 40.2, 82.2, 88.95, 95.1, 67.89};
//是否所有人都及格了
bool isAllPass = std::none_of(scores.begin(), scores.end(), [](auto &v)->bool{return v < 60.0;});
if (!isAllPass)
{
    printf("that is not all student pass the examine!\n");
}
```

### [3. 生成值算法](#) 
就是对集合的值进行赋值填充， **generate、** **generate_n、**  **fill、** **fill_n、**

#### [3.1 generate](#)
以给定函数对象 g 所生成的值赋值范围 [first, last) 中的每个元素。 **无返回值！**

```cpp
template< class ForwardIt, class Generator >
void generate( ForwardIt first, ForwardIt last, Generator g );
```

将要调用的生成器函数。签名应等价于如下：`Ret fun()`;类型 `Ret` 必须使得 `ForwardIt` 类型对象能被解引用并能被赋 `Ret` 类型值。​

```cpp
std::list<double> numbers(10);
//是否所有人都及格了
std::generate(numbers.begin(), numbers.end(), [n = 0] () mutable { n *= 2 ;return n++; });

std::for_each(numbers.begin(), numbers.end(), [](auto &v){  std::cout<< v << "\n";  });
/* 0 2 6 14 30 62 126 254 510 1022 */
```


#### [3.2 generate_n](#)
等价于generate，只是第二个参数不一样而已！

```cpp
template< class OutputIt, class Size, class Generator >
OutputIt generate_n( OutputIt first, Size count, Generator g );
```

```cpp
std::list<double> numbers(10);
//是否所有人都及格了
std::generate_n(numbers.begin(), numbers.size(), [n = 0] () mutable { n *= 2 ;return n++; });

std::for_each(numbers.begin(), numbers.end(), [](auto &v){  std::cout<< v << "\n";  });
/* 0 2 6 14 30 62 126 254 510 1022 */
```

#### [3.3 fill](#)
赋值给定的 `value` 给 `[first, last)` 中的元素。
```cpp
template< class ForwardIt, class T >
void fill( ForwardIt first, ForwardIt last, const T& value ); 
//c++ 20 前面多了一个 constexpr 修饰符
```

`实验：`
```cpp
std::list<double> heights(6)  ; //{ 173.5, 177.5,185.2,169.15,175.2,180.2 } 
std::fill(heights.begin(), heights.end(), 173.12);

std::for_each_n(heights.begin(), heights.size(), [n=1](auto & h) mutable {
    printf("%d: %.2f\n",n++, h);
});
/*
1: 173.12
2: 173.12
3: 173.12
4: 173.12
5: 173.12
6: 173.12
*/
```
#### [3.4 fill_n](#)
若 `count > 0` ，则赋值给定的 `value` 给始于 的范围的首 `count` 个元素。否则不做任何事。

```cpp
template< class OutputIt, class Size, class T >
void fill_n( OutputIt first, Size count, const T& value );
```

`实验：`
```cpp
std::list<double> heights(6)  ; //{ 173.5, 177.5,185.2,169.15,175.2,180.2 } 
std::fill_n(heights.begin(), heights.size(), 173.12);

std::for_each_n(heights.begin(), heights.size(), [n=1](auto & h) mutable {
    printf("%d: %.2f\n",n++, h);
});
```

#### [3.5 iota 累加范围生成](#)
在头文件 `<numeric>` 定义，以始于 `value` 并重复地求值 `++value` 的顺序递增值填充范围 `[first, last)` 。
```cpp
template< class ForwardIt, class T >
void iota( ForwardIt first, ForwardIt last, T value );
```

```cpp
std::list<int> l(10);
std::iota(l.begin(), l.end(), 1);

for (auto&& v : l) {
    printf("%d ", v);
}//1 2 3 4 5 6 7 8 9 10
printf("\n");
```

### [4. 常用排序算法](#) 
C++ STL 标准库提供了一些排序算法: **sort、** **random_shuffle、** **merge、** **reverse、** 

#### [4.1 sort](#)
排序算法，以不降序排序范围 [first, last) 中的元素。不保证维持相等元素的顺序。 **非稳定算法**
**list容器 是不支持 sort操作的！**  **顺序容器 需要支持，必须符合值可交换 (ValueSwappable) 和 老式随机访问迭代器 (LegacyRandomAccessIterator) 的要求**
```cpp
template< class RandomIt >
void sort( RandomIt first, RandomIt last ); //用 operator< 比较元素。

template< class RandomIt, class Compare >
void sort( RandomIt first, RandomIt last, Compare comp ); //用给定的二元比较函数 comp 比较元素
```

```cpp
std::vector<double> heights{ 173.5, 177.5,185.2,169.15,175.2,180.2 }; 

std::sort(heights.begin(), heights.end());

std::for_each(heights.begin(), heights.end(), [](auto &v){ std::cout << v << "\n"; });
/*
169.15
173.5
175.2
177.5
180.2
185.2
*/
```
**实现降序效果： 前大为 true**
```cpp
std::vector<double> heights{ 173.5, 177.5,185.2,169.15,175.2,180.2 }; 

std::sort(heights.begin(), heights.end(), [](auto v1, auto v2){
    if (v1 > v2)
    {
        return true;
    }
    return false;
});

std::for_each(heights.begin(), heights.end(), [](auto &v){ std::cout << v << "\n"; });
/*
185.2
180.2
177.5
175.2
173.5
169.15
*/
```

#### [4.2 random_shuffle](#)
重排序给定范围 [first, last) 中的元素，使得这些元素的每个排列拥有相等的出现概率。 **洗牌算法** **感觉这个API进入了弃用阶段！**
```cpp
template< class RandomIt >
void random_shuffle( RandomIt first, RandomIt last );  //(C++14 中弃用) (C++17 中移除)
```

`如果不设置随机种子，每次运行此算法，结果都是一样的！`
```cpp
std::vector<int> cards{1,2,3,4,5,6,7,8,9}; 

using std::random_shuffle;
//设置以下随机种子，防止伪随机每次结果都是一样的！
srand((unsigned int)time(NULL));
random_shuffle(cards.begin(), cards.end());

std::for_each(cards.begin(), cards.end(), [n = 0, &cards](auto &v) mutable { 
    std::cout << v << " "; 
    if(++n == cards.size()) printf("\n");
});
//5 4 8 9 1 6 3 2 7
```

#### [4.3 shuffle](#)
**C++ 11提供了新的API** `g` **是随机数生成器为函数对象!**
```cpp
template< class RandomIt, class URBG >
void shuffle( RandomIt first, RandomIt last, URBG&& g );
```

```cpp
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
 
int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 
    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(v.begin(), v.end(), g);
 
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
    //8 6 10 4 2 3 7 1 9 5
}
```

#### [4.4 merge](#)
归并二个 **已排序**范围 `[first1, last1)` 和 `[first2, last2)` 到始于 `d_first` 的一个已排序范围中！

```cpp
//用 operator< 比较元素。
template< class InputIt1, class InputIt2, class OutputIt >
OutputIt merge( InputIt1 first1, InputIt1 last1,
                InputIt2 first2, InputIt2 last2,
                OutputIt d_first );

//用给定的二元比较函数 comp 比较元素。
template< class InputIt1, class InputIt2, class OutputIt, class Compare >
OutputIt merge( InputIt1 first1, InputIt1 last1,
                InputIt2 first2, InputIt2 last2,
                OutputIt d_first, Compare comp );
```

**合并两个序列，然后输出到屏幕！**
```cpp
std::vector<int> vcount{1,3,4,5,7,8,9,15}; 
std::vector<int> tcount{2,3,5,6,7,9,10,13}; 

std::merge(vcount.begin(), vcount.end(),
    tcount.begin(), tcount.end(), 
    std::ostream_iterator<int, char>(std::cout, " "));

printf("\n");
//1 2 3 3 4 5 5 6 7 7 8 9 9 10 13 15 
```

#### [4.5 reverse](#)
反转 `[first, last)` 范围中的元素顺序! **容器迭代器必须支持双向迭代器**

```cpp
std::vector<int> vcount{1,3,4,5,7,8,9,15}; 
std::reverse(vcount.begin(), vcount.end());

std::copy(vcount.begin(), vcount.end(),std::ostream_iterator<int, char>(std::cout, " "));

printf("\n");
//15 9 8 7 5 4 3 1
```


### [5. 常用拷贝和替代算法](#) 
**swap, replace, replace_if, move(C++11),copy, copy_if**

#### [5.1 copy](#)
复制 `[first, last)` 所定义的范围中的元素到始于 d_first 的另一范围。 **目标容器需要提前开辟好空间！**

```cpp
template< class InputIt, class OutputIt >
OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
```

`结果拷贝到屏幕用于输出`
```cpp
std::vector<int> vcount{1,3,4,5,7,8,9,15}; 
std::reverse(vcount.begin(), vcount.end());

std::copy(vcount.begin(), vcount.end(),std::ostream_iterator<int, char>(std::cout, " "));
```

`结果拷贝到其他容器`
```cpp
std::vector<int> vcount{1,3,4,5,7,8,9,15}; 
std::vector<int> count(vcount.size()); 

std::copy(vcount.begin(), vcount.end(), count.begin());

std::for_each(count.begin(), count.end(), [](auto v){ std::cout << v << " "; });

printf("\n");
//1 3 4 5 7 8 9 15 
```


#### [5.2  copy_if](#)
复制 [first, last) 所定义的范围中的元素到始于 d_first 的另一范围。 **仅复制谓词 pred 对其返回 true 的元素。保持被复制元素的相对顺序。** 若源与目标范围重叠则行为未定义
```cpp
template< class InputIt, class OutputIt, class UnaryPredicate >
OutputIt copy_if( InputIt first, InputIt last,
                  OutputIt d_first,
                  UnaryPredicate pred );
```

**只复制身高大于 175的数值！**
```cpp
std::vector<double> heights{ 173.5, 177.5,185.2,169.15,175.2,180.2 }; 
std::vector<double> more(
    std::count_if(heights.begin(), heights.end(), [](auto v){ return v > 175.0; })
); 

std::copy_if(heights.begin(), heights.end(), more.begin(), [](auto v){ return v > 175.0; });

for (auto &i : more)
{
    std::cout << i << " ";
}
//177.5 185.2 175.2 180.2
printf("\n");
```

#### [5.3 replace](#)
以 `new_value` 替换范围 `[first, last)` 中所有满足特定判别标准的元素。替换所有等于 `old_value` 的元素。

```cpp
template< class ForwardIt, class T >
void replace( ForwardIt first, ForwardIt last,
              const T& old_value, const T& new_value );
```

```cpp
std::array<int, 10> s{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
 
std::replace(s.begin(), s.end(), 8, 88);
for (int a : s) {
    std::cout << a << " ";
}
std::cout << '\n';
//5 7 4 2 88 6 1 9 0 3
```

#### [5.4 replace_if](#)
以 `new_value` 替换范围 `[first, last)` 中所有满足特定判别标准的元素。替换所有谓词 `p` 对其返回 `true` 的元素。

```cpp
template< class ForwardIt, class UnaryPredicate, class T >
void replace_if( ForwardIt first, ForwardIt last,
                 UnaryPredicate p, const T& new_value );
```

```cpp
std::array<int, 10> s{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
std::replace_if(s.begin(), s.end(), [](auto v){ return v >= 5; }, 10);

for (auto &&i : s)
{
    printf("%d ", i);
}
printf("\n"); //10 10 4 2 10 10 1 10 0 3
```


#### [5.5 swap](#)
交换两个值！有几乎无数种特化重载！

* `std::swap(std::list)`
* `std::swap(std::vector)`
* `std::swap(std::stack)`
* `std::swap(std::valarray)`
* `std::swap(std::map)`
* `.....`

```cpp
int a = 10, b = 12;

std::swap(a, b);

printf("a = %d  b =%d \n",a,b);
//a = 12, b = 10

std::array<int, 10> s1{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
std::array<int, 10> s2{15, 17, 14, 12, 18, 16, 11, 19, 10, 13};

std::swap(s1, s2);

for (auto &&i : s1)
{
    printf("%d ", i);
}
printf("\n"); //15 17 14 12 18 16 11 19 10 13 

```

### [6. 删除算法](#) 
**std::remove, std::remove_if， unique**


#### [6.1 remove](#)
从范围 [first, last) 移除所有满足特定判别标准的元素，并返回范围新结尾的尾后迭代器。

```cpp
template< class ForwardIt, class T >
ForwardIt remove( ForwardIt first, ForwardIt last, const T& value );
```

```cpp
std::array<int, 10> s1{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

std::remove(s1.begin(),s1.end(), 8);

for (auto &&i : s1)
{
    printf("%d ", i);
}
printf("\n"); //5 7 4 2 6 1 9 0 3 3 
```

#### [6.2 remove_if](#)


#### [6.3 unique](#)
从来自范围 [first, last) 的相继等价元素组消除首元素外的元素，并返回范围的新逻辑结尾的尾后迭代器。通过用覆写要被擦除的元素的方式迁移范围中的元素进行移除。
**最好先对内容进行排序，然后再回收无用的内存**
```cpp
template< class ForwardIt >
constexpr ForwardIt unique( ForwardIt first, ForwardIt last );
```

**谨慎使用！**
```cpp
std::vector<int> v{1, 2, 1, 1, 3, 3, 3, 4, 5, 4};
std::sort(v.begin(), v.end());
auto last = std::unique(v.begin(), v.end());
// v 现在保有 {1 2 3 4 5 x x x x x } ，其中 x 不确定
v.erase(last, v.end()); //将后面的无用内存擦除掉

std::for_each(v.begin(), v.end(), [](auto v){std::cout << v << "\n"; });
```

### [7. 集合算法](#)
集合运算符为通常的集合运算并、交、差。

#### [7.1 includes](#)
若一个序列是另一个的子列则返回 true

```cpp
std::set<int> voters_b1 = { 12,15,98,56,47,87,89,45 };
std::set<int> voters_b2 = { 98,56,47,87 };

bool isHave = std::includes(voters_b1.begin(), voters_b1.end(), voters_b2.begin(), voters_b2.end());
std::cout << isHave << " \n"; //1
```

#### [7.2 set_union](#)
计算两个集合的并集

```cpp
#include <iostream>
#include <string>
#include<algorithm>
#include<set>
#include<math.h>

int main(int argc, char const* argv[])
{   
   std::set<int> voters_b1 = {12,15,98,56,47,87,89,45};
   std::set<int> voters_b2 = {22,15,98,56,49,87,89,41};
   std::set<int> voters;
   std::set_union(voters_b1.begin(),voters_b1.end(),
                      voters_b2.begin(),voters_b2.end(), 
                      std::insert_iterator<std::set<int>>(voters, voters.begin()));

   std::cout <<"total:"<< voters.size() <<"\n";
   std::for_each(voters.begin(), voters.end(), [](int t) {
      std::cout<< t<<"\n";
   });
   return 0;
}
```

#### [7.3 set_intersection](#)
计算两个集合的交集

#### [7.4 set_difference](#)
计算两个集合的差集

#### [7.5 set_symmetric_difference](#)
计算两个集合的对称差



### [8. 数值算法](#)
**max、min、equal、accumulate**

#### [8.1 accumulate](#)
计算给定值 init 与给定范围 [first, last) 中元素的和。需要元素重载了运算符 **operator+**
```cpp
template< class InputIt, class T >
constexpr T accumulate( InputIt first, InputIt last, T init );
```

```cpp
std::list<int> l(10);
std::iota(l.begin(), l.end(), 1);

int sum = std::accumulate(l.begin(), l.end(), 0);
std::cout << "sum: " << sum << "\n"; //sum: 55
```

#### [8.2 max](#)
多重重载，用于获得两个及以上的元素集合中的最大值！

```cpp
template< class T >
const T& max( const T& a, const T& b );
template< class T, class Compare >
constexpr const T& max( const T& a, const T& b, Compare comp );
template< class T >
constexpr T max( std::initializer_list<T> ilist );
template< class T, class Compare >
T max( std::initializer_list<T> ilist, Compare comp );
```

```cpp
std::cout <<"max: " << std::max({010, 10, 0X10, 0B10}) << " \n"; //16
```

### [9. 配套算法](#)
配合迭代器使用的方法！

#### [9.1 begin 和 end](#)
begin和end是容器的两个成员函数，后来,数组也支持标准库函数begin和end， 定义在iterator头文件中

* **begin**： 返回指向数组首元素的指针
* **end**： 返回指向数组尾元素下一个位置的指针
 
```cpp
int a[] = {1, 2, 3, 4 ,5};//a是一个含有5个整型的数组
int *fir = begin(a);//指向a首元素的指针
int *las = end(a);//指向a尾元素下一个位置的指针
```

**注意尾后指针不能执行解引用和递增操作。**

#### [9.2 equal](#)
判断两个列表是否相等！

```cpp
template< class InputIt1, class InputIt2 >
constexpr bool equal( InputIt1 first1, InputIt1 last1,
                      InputIt2 first2 );

template< class InputIt1, class InputIt2, class BinaryPredicate >
bool equal( InputIt1 first1, InputIt1 last1,
            InputIt2 first2, BinaryPredicate p );

template< class InputIt1, class InputIt2 >
constexpr bool equal( InputIt1 first1, InputIt1 last1,
                      InputIt2 first2, InputIt2 last2 );

template< class InputIt1, class InputIt2, class BinaryPredicate >
bool equal( InputIt1 first1, InputIt1 last1,
            InputIt2 first2, InputIt2 last2,
            BinaryPredicate p );
```

* 如果范围 [first1, last1) 的长度不等于范围 [first2, ....) 的长度则返回 false 。
* 如果范围 [first1, last1) 的长度不等于范围 [first2, last2) 的长度则返回 false 。

```cpp
std::set<int> voters_b1 = { 12,15,98,56,47,87,89,45 };
std::set<int> voters_b2 = { 12,15,98,56,47,87,89,45 };

bool isEqual = std::equal(voters_b1.begin(), voters_b1.end(), voters_b2.begin());
std::cout <<"isEqual: " << isEqual << " \n"; //1
```

#### [9.3 bind](#)
是用来绑定函数调用的某些参数，可以将bind函数看作一个通用的函数适配器，它接受一个可调用对象，生成新的可调用对象来适应原对象的参数列表。
调用bind的一般形式：

```cpp
auto newCallable = bind(callable,arg_list);
```
其中，newCallable本身是一个可调用对象，arg_list是一个逗号分隔的参数列表，对应给定的callable的参数。即，当我们调用newCallable时，newCallable会调用callable,并传给它arg_list中的参数。

arg_list中的参数可能包含形如_n的名字，其中n是一个整数，这些参数是“占位符”，表示newCallable的参数，它们占据了传递给newCallable的参数的“位置”。数值n表示生成的可调用对象中参数的位置：_1为newCallable的第一个参数，_2为第二个参数，以此类推。

**1. 绑定普通函数**

```cpp
//先占位，再给参数
std::function<int(int, int)> f = std::bind(plus, std::placeholders::_1, std::placeholders::_2);

cout << f(25, 35) << endl; //3

//直接给参数
std::function<int()> fm = std::bind(multi, 10, 15, 10);
cout << fm() << endl; //1500
```
**2. 绑定类的成员函数**

bind绑定类成员函数时，第一个参数表示对象成员函数的指针，第二个参数表示对象的地址。

必须显示的指定`&Plus::plus`，因为编译器不会将对象的成员函数隐式转换成函数指针，所以必须在`Plus::plus`前面添加`&`。

使用对象成员函数的指针时，必须知道该指针属于哪个对象，因此第二个参数为对象的地址`&plus`。
```cpp
#include<iostream>
#include<functional>
using namespace std;
class Plus
{
public:
    int plus(int a, int b)
    {
        return a + b;
    }
};

int main(int argc, char* argv[]) {
    Plus p;
    //指针形式调用成员函数
    std::function<int(int, int)> func1 = std::bind(&Plus::plus, &p, std::placeholders::_1, std::placeholders::_2);
    //对象形式调用成员函数
    std::function<int(int, int)> func2 = std::bind(&Plus::plus, p, std::placeholders::_1, std::placeholders::_2);
    cout << func1(10, 2) << endl; //12
    cout << func2(10, 12) << endl; //22
    return 0;
}
```
**3.绑定类静态成员函数**

```cpp
#include<iostream>
#include<functional>
using namespace std;
class Plus
{
	public:
		static int plus(int a,int b)
		{
		    return a+b;
		}
}
int main()
{
   function<int<int,int>> func1 = std::bind(&Plus::plus, placeholders::_1, placeholders::_2);
   cout<<func1(1,2)<<endl; //3
   retunrn 0;
}
```

-----
`时间`: `[]` 