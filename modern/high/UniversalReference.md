### [C++ 万能引用与引用折叠](#)
**介绍**:

----




### 问题篇

#### X.1 函数指针能赋值给万能引用吗？ 

```cpp
class Registry {
private:
    //方法存储中心，使用hash 列表 存储, 参数是 BinarySerializer
    std::unordered_map<std::string, std::function<void(BinarySerializer*)>> dictionary;
public:
    Registry(uint16_t _port);
private:

public:
    template<typename F>
    void Bind(const std::string& name, F&& func){
        dictionary[name] = std::bind(&Registry::Proxy<F>, this, std::forward<F>(func), std::placeholders::_1);
    }
};
```

如下操作 gcc 报错： `In template: no viable overloaded '='` 再 Bind 方法中的 ` dictionary[name] = ` 语句部分!
```cpp
int test_fun1(int value){
    int i  = 10;
    return i + value;
}

int main() {
    Registry proxy(15500);
    proxy.Bind("run", test_fun1);
}
```

改造一下,它又行了
```cpp
proxy.Bind<decltype(test_fun1)>("run", test_fun1);
```

再换个改造方式，它也行了
```cpp
template<typename F>
void Bind(const std::string& name, F func){
    dictionary[name] = std::bind(&Registry::Proxy<F>, this, std::forward<F>(func), std::placeholders::_1);
}
```
下面也不报错了，模板也能自动推断了。
```cpp
Registry proxy(15500);

proxy.Bind("run", test_fun1);
```