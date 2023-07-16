### [reinterpret_cast 详解](#)
**介绍**: reinterpret中文意为 “重新解释; 重新诠释” ,reinterpret_cast 运算符并不会改变括号中运算对象的值，而是对该对象从位模式上进行重新解释。

-----

```cpp
reinpreter_cast<type-id> (exp)
```
其中， reinterpret_cast后的尖括号中的type-id类型必须是一个指针、引用、算术类型、函数指针或者成员指针。它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针。


* reinterpret_cast 几乎可以转换为所有的类型， **一般就只是用于指针类型之间的转换**。
* reinterpret_cast运算符允许将任意指针转换到其他指针类型，也允许做任意整数类型和任意指针类型之间的转换。转换时，执行的是逐个比特复制的操作。
```cpp
int val = 20;

float* ptr = reinterpret_cast<float *>(&val);

printf("value: %e", *ptr); // 2.802597e-44

int *val_ptr = reinterpret_cast<int *>(ptr);

printf("value_ptr: %d\n", *val_ptr); //20
```

**实现泛型指针向非泛型指针的转换:**

```cpp
void get_result_executor(const std::shared_ptr<Executor>& token){
    if (token != nullptr){
        auto result = reinterpret_cast<ExecutorToken<void> *>(token.get());
        return result->get();
    }else{
        throw std::runtime_error("The parameter passed in is nullptr");
    }
}
```