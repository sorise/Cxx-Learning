### [C++ 模板类的类型限制](#)
 **介绍**： 有时候我们写的模板可能并不适用于所有的类型，或者需要参数具有某些特性，比如一定要有默认构造函数、一定需要const修饰，这个时候我们需要给模板参数增加一些限制。

-----

#### [1. 静态断言语法](#) 

使用静态断言 `static_assert` 的来限制模板的类型户或者特性：

```cpp
//确保模板类的入参类型为integer类
template <typename T>
T add_2(T t) {
  static_assert(std::is_integral_v<T>, "Type T should be integer");
  return t+1;
}
```

**std::enable**

`std::enable_if` 顾名思义，满足条件时类型有效。作为选择类型的小工具，其广泛的应用在 C++ 的模板元编程中。它的定义也非常的简单：

```cpp
// STRUCT TEMPLATE enable_if
template <bool _Test, class _Ty = void>
struct enable_if {}; // no member "type" when !_Test

template <class _Ty>
struct enable_if<true, _Ty> { // type is _Ty for _Test
    using type = _Ty;
};
```

由此可知，只有当第一个模板参数`_Test`为 `true` 时，`type` 才有定义（`type`即第二个模板参数`_Ty`）；否则使用 `type` 会产生编译错误，且默认模板参数可以让你不必指定类型。



**enable_if_t** 是一个别名，其定义如下：

```cpp
template <bool _Test, class _Ty = void>
using enable_if_t = typename enable_if<_Test, _Ty>::type;
```





#### 2. 类型偏特化

在使用模板编程时，经常会用到根据模板参数的某些特性进行不同类型的选择，或者在编译时校验模板参数的某些特性。

**控制函数返回类型**, 返回值必须是整数。

```cpp
template <typename T>
typename std::enable_if_t<std::is_integral_v<T>, bool>
is_odd(T t) {
  return bool(t%2);
}
```

**校验函数模板参数类型**

有时定义的模板函数，只希望特定的类型可以调用

```cpp
template <typename T, typename = typename std::enable_if_t<std::is_integral_v<T>>>
bool is_even(T t) {
  return t%2 == 0;
}
```

