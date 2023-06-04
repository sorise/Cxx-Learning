### [C++ 模板元编程介绍](#)
**介绍**： 模板元编程产生的源程序是在编译期间执行的程序，可以生成在运行期间执行的程序，可以在编译期执行两种计算：数值计算和类型计算。

----

### 1. 编译器能够操作的变量和类型
元编程无法控制运行时的代码，它不能操作普通的变量，不能使用运行时关键字，可以采用的语法元素只能是如下集中：
* enum、static、const 用来定义编译器整数常量
* typedef, using 定义元数据
* T、Args... 声明元数据类型
* template 定义元函数
* :: 域运算符，解析作用域。


#### 1.1 编译器 IF 例子

```cpp
template<bool FLAG>
struct IF{};

template<>
struct IF<true>{
    static void Func() { std::cout << "true carry-out!\n"; };
};

template<>
struct IF<false>{
    static void Func() { std::cout << "false carry-out!\n"; };
};


int main() {
    
    IF<true>::Func(); //true carry-out!
    IF<false>::Func(); //false carry-out!

    return 0;
}
```

#### 1.2 编译器 SWITCH 例子

```cpp
template<int Case>
struct SWITCH{
    static void Func() { std::cout << "default value: 0\n"; };
};

template<>
struct SWITCH<10>{
    static void Func() { std::cout << "value: 10\n"; };
};

template<>
struct SWITCH<20>{
    static void Func() { std::cout << "value: 20\n"; };
};

template<>
struct SWITCH<30>{
    static void Func() { std::cout << "value: 30\n"; };
};

int main() {
    SWITCH<10>::Func(); //value: 10
    SWITCH<20>::Func(); //value: 20
    SWITCH<30>::Func(); //value: 30
    SWITCH<110>::Func(); //value: 0

    return 0;
}
```

#### 1.3 编译器 for 循环
for循环本质是利用模板递归：

```cpp
template <typename T,int N>
struct FOR{
    static inline void each(T t){
        std::cout << t[N] << std::endl;
        FOR<T, N -1>::each(t);
    }
};

template<typename T>
struct FOR<T,0>{
    static inline void each(T t){
        std::cout << t[0] << std::endl;
    }
};

//完成对 names的循环
int main() {
    std::vector<std::string> names{"remix", "gege", "kun", "2nb", "tome"};
    FOR<std::vector<std::string>, 4>::each(names);
    return 0;
}
```


### 2. traits 技术 
特性萃取技术, C++ 的 traits 技术，是一种约定俗称的技术方案，用来为同一类数据（包括自定义数据类型和内置数据类型）提供统一的操作函数，例如 advance(), swap(), encode()/decode() 等。
[官方API](https://zh.cppreference.com/w/cpp/meta)

C++ 提供元编程设施，诸如类型特性、编译时有理数算术，以及编译时整数序列。

除非另外有指定，可以用不完整类型实例化定义于 \<type_traits\> 头文件的模板，尽管通常禁止以不完整类型实例化标准库模板。
在标头 \<type_traits\> 定义

```cpp
int * a = new int(25);

std::cout << std::boolalpha;
std::cout << std::is_integral_v<int> << std::endl;
std::cout << std::is_void_v<int> << std::endl; //false
std::cout << std::is_void<typeof(a)>::value << std::endl; //false

delete a;
a = nullptr;
```




































