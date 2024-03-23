## [std::tuple](https://zh.cppreference.com/w/cpp/utility/tuple)

**介绍**：类模板 `std::tuple` 是固定大小的异类值汇集。它是 [std::pair](https://zh.cppreference.com/w/cpp/utility/pair) 的泛用形式， 源代码定义如下所示：

```cpp
template< class... Types > //可变参数模板
class tuple;
```
使用案例：
```cpp
std::tuple<std::string, float, int> user(std::string("king zong"), 3.1415, 108);
std::cout << std::get<0>(user) << std::endl;//king zong
std::cout << std::get<1>(user) << std::endl;//3.1415
std::cout << std::get<2>(user) << std::endl;//108
```

#### [定义和初始化tuple](#)
可以直接创建模板类，或者使用[make_tuple](https://zh.cppreference.com/w/cpp/utility/tuple/make_tuple)方法创建一个 `tuple` 对象，其类型根据各实参类型定义。

```cpp
std::tuple<int, int, float> numbers{ 52,35,35.12 };


std::tuple<std::string, std::array<float, 5>> grade{ 
    std::string("remix"), 
    {89.5,68.5,95.5,98.5,99.0} 
};
//使用 make_tuple
auto tpl = std::make_tuple<std::string, std::array<float, 5>>(
    std::string("remix"), 
    { 89.5,68.5,95.5,98.5,99.0 }
);

std::cout << std::get<1>(tpl)[2] << std::endl; //95.5
std::cout << std::get<1>(grade)[2] << std::endl; //95.5
```

#### [std::get 访问tuple的成员](#)
使用API [std::get(std::tuple)](https://zh.cppreference.com/w/cpp/utility/tuple/get)返回t的第i个数据成员的**引用**：如果t是一个左值，结果是一个左值引用；否则，结果是一个右值引用。此外tuple的所有成员都是pulic的。

```cpp
get<i>(t)
```
通过引用修改元祖内容：
```cpp
auto tpl = std::make_tuple<std::string, std::array<float, 5>>(
    std::string("remix"), 
    { 89.5,68.5,95.5,98.5,99.0 }
);

std::get<1>(tpl)[1] = 100; //返回的是引用，可以修改
std::cout << std::get<1>(tpl)[1] << std::endl; //100
```

#### std::tie
它允许将多个变量绑定到一个 tuple 或者 pair 中，以便对这些变量进行同时赋值或者比较操作。使用 std::tie 可以方便地创建一个 tuple 或者 pair，用来存储多个变量的值， 这在函数返回多个值时非常有用。

原始定义：
```cpp
template <typename... Args>
constexpr // C++14 起
std::tuple<Args&...> tie(Args&... args) noexcept {
    return {args...};
}
```

使用例子：
```cpp
std::tuple<int, double, std::string> getValues() {
    return std::make_tuple(42, 3.14, "hello");
}

int main() {
    int intValue;
    double doubleValue;
    std::string stringValue;

    std::tie(intValue, doubleValue, stringValue) = getValues();
    std::cout << "Int value: " << intValue << std::endl;
    std::cout << "Double value: " << doubleValue << std::endl;
    std::cout << "String value: " << stringValue << std::endl;

    return 0;
}
```

#### [结构化绑定](#)
C++17 引入了结构化绑定（Structured Binding）的特性，它允许我们将一个 tuple 或者 pair 中的元素解包到多个变量中，从而方便地访问和操作其中的值，
结构化绑定可以用于任何支持结构化绑定的类型（如 tuple、pair、数组等），使得代码更加简洁和易读。
```cpp
auto [age, name , sex] = std::make_tuple(20, "John", 'M');

fmt::print("age: {}, name: {}, sex: {}\n", age, name, sex);
```
#### [判断元祖是否相等](#)
对元祖的比较操作C++20以后就支持判断两个元祖是否相等或者 不相等了！也就是说只支持 **==** 和 **!=** 运算符，而且还要求必须两个元祖的类型是一致的！

#### [获得tuple成员的类型信息](#)
首先我们获取一个元祖的大小, 使用 **tuple_size** 赋值：

```cpp
//获取元祖的大小
std::cout << std::tuple_size<decltype(tpl)>::value << std::endl; //2
```
然后获得元祖某位置的类型， 使用 **tuple_element**赋值:
```cpp
//获得类型
std::tuple_element<1, decltype(tpl)>::type number = std::get<1>(tpl);
//number 的类型就是 std::array<float, 5>
std::for_each(number.begin(), number.end(), [](const float& v) {
    std::cout << v << std::endl;
});
```

### 注

1. 反对C++函数返回用std::tuple, 可读性可变性太差。
