### [C++ 模板类的类型限制](#)
 **介绍**： 有时候我们写的模板可能并不适用于所有的类型，或者需要参数具有某些特性，比如一定要有默认构造函数、一定需要const修饰，这个时候我们需要给模板参数增加一些限制。

-----

### [1. 静态断言语法](#) 
使用静态断言 `static_assert` 的来限制模板的类型户或者特性：

```cpp
//确保模板类的入参类型为integer类
template <typename T>
T add_2(T t) {
  static_assert(std::is_integral_v<T>, "Type T should be integer");
  return t+1;
}
```