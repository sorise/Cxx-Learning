### [C++ 细节之 成员函数指针](#)
**介绍**: 对我们写库、造轮子很有帮助！

```cpp
//
// Created by remix on 23-7-26.
//
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

class Normal{
public:
    Normal(int _value, const std::string& _name)
    :value(_value), name( std::move(_name)){}

    std::string setValueAndGetName(int _new_value){
        this->value = _new_value;
        return this->name;
    }

    int getValue(){
        return this->value;
    }

private:
    int value;
    std::string name;
};

typedef std::string (Normal::* call)(int val);

//添加到树上
template<typename F,typename R,  typename ...Args>
void setTimeout(long long milliseconds, F&& f, R&& r,  Args&&... args){
    auto callBack = std::bind(std::forward<F>(f) , std::forward<R>(r) ,std::forward<Args>(args)...);
    callBack();
}


//将任务封装成一个 token！
template<typename R, typename F, typename ...Args>
auto make_executor_(F&& f, R&& r, Args&&...args) -> decltype((r.*f)(args...)){
    using ReturnType = decltype((r.*f)(args...));
    auto func = std::bind(std::forward<F>(f), std::forward<R>(r),std::forward<Args>(args)...);
    return func();
}

int main(int argc, char *argv[]){
    Normal normal(25,"remix");
//    auto f = std::bind(&Normal::setValueAndGetName, normal, 56);
//    f();

//    make_executor_(&Normal::setValueAndGetName, normal, 56);

    call c = &Normal::setValueAndGetName;

    (normal.*c)(56);

    std::cout << normal.getValue();
}
```