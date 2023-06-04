## [模板递归](#)
模板可以被递归调用，在模板递归的过程中，可以执行前面我们提到的两种编译期计算：数值计算和类型计算，**退出递归是通过特化来实现的**。


### [模板计算](#)

```cpp
template<int N>
struct Factorial
{
    enum { Value = N * Factorial<N - 1>::Value };
};

template<>
struct Factorial<1>
{
    enum { Value = 1 };
};
```


### [利用模板递归挨个访问元组的值](#)


```cpp
template<unsigned int N>
struct TupleWriter {
    template<typename... Args>
    static void write(const std::tuple<Args...>& t, BinarySerializer& serializer){
        serializer.input(std::get<N>(t)); //访问元组的值
        TupleWriter<N-1>::write(t, serializer); //递归模板
    }
};

template<>
struct BinarySerializer::TupleWriter<0> {
    template<typename... Args>
    static void write(const std::tuple<Args...>& t, BinarySerializer& serializer){
        serializer.input(std::get<0>(t));
    }
};
```