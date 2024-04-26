## [C++ 成员指针](#)

**介绍**：介绍`int(X::*)const = &X::function;` 和 `X::*ptr = &X::v`是什么东西；



```cpp
#include <mutex>
#include <shared_mutex>
#include <iostream>

struct Person: public{
private:
    mutable std::shared_mutex mtx;
    int age{0};
public:

    explicit Person(const int& _age): age(_age){

    }

    [[nodiscard]] int get_age() const{
        std::shared_lock<std::shared_mutex> lock(this->mtx);
        return age;
    }

    void set_age(const int& _age){
        std::lock_guard<std::shared_mutex> lck(mtx);
        this->age = _age;
    }

    virtual ~Person(){
        fmt::print("Person destructor!\n");
    }

    Person(): mtx(){
        fmt::print("Person create!\n");
    }
};
```

成员函数

```cpp
// int(Person::*)const
auto k = &Person::get_age;
Person* p = new Person(12);

((*p).*k)();

delete p;
```

