## std::share_ptr\<T\> 循环引用问题

**介绍**:  如果使用不好，发生致循环引用，这将导致资源永远得不到回收！

```cpp
class B;

class A{
public:
    A(){ std::cout << "A created" <<std::endl; }
    ~A(){ std::cout << "A disappear" <<std::endl; }

    std::shared_ptr<B> b1;
};


class B{
public:
    B(){ std::cout << "B created" <<std::endl; }
    ~B(){ std::cout << "B disappear" <<std::endl; }

    std::shared_ptr<A> a1;
};

int main() {
    {
        auto a = std::make_shared<A>();
        auto b = std::make_shared<B>();

        a->b1 = b;

        b->a1 = a;

        std::cout << "a.use_count(): " << a.use_count()<<std::endl; //2
        std::cout << "b.use_count(): " << b.use_count()<<std::endl; //2
    }
    return 0;
}
```

最后析构函数将不会被调用！



### [weak_ptr 解决循环引用](#)

使用 **weak_ptr**，**weak_ptr指针创建一个weak_ptr指针**，有以下3种方式：

* 可以创建一个空的weak_ptr指针，例如：
```cpp
std::weak_ptr<int> wp1;
```
* 凭借已有的weak_ptr指针，可以创建一个新的weak_ptr指针，例如：
```cpp
std::weak_ptr<int> wp2(wp1);
```
若wp1为空指针，则wp2也为空指针；反之，如果wp1指向某一shared_ptr指针拥有的堆内存，则wp2也指向该块存储空间（可以访问，但无所有权）。

* weak_ptr指针更常用于指向某一shared_ptr指针拥有的堆内存，因为在构建weak_ptr指针对象时，可以利用已有的shared_ptr指针为其初始化。例如：
```cpp
std::shared_ptr<int> sp(new int);
std::weak_ptr<int> wp3(sp);
```
由此，wp3指针和sp指针有相同的指针。再次强调，**weak_ptr类型指针不会导致堆内存空间的引用计数增加或减少**。

#### [weak_ptr模板类提供的成员方法](#)

和shared_ptr\<T\>以及unique_ptr\<T\>相比，weak_ptr\<T\>模板类提供的成员方法不多，下表罗列了常用的成员方法及各自的功能。

| `成员方法`  | `功能`                                                       |
| :---------- | :----------------------------------------------------------- |
| operator=() | 重载=赋值运算符，weak_ptr指针可以直接被weak_ptr或者shared_ptr类型指针赋值。 |
| swap(x)     | 其中x表示一个同类型的weak_ptr类型指针，该函数可以互换2个共同类型weak_ptr指针的内容。 |
| reset()     | 将当前weak_ptr指针置为空指针。                               |
| use_count() | 查看指向和当前weak_ptr指针相同的shared_ptr指针的数量。       |
| expired()   | 判断当前weak_ptr指针是否过期（指针为空，或者指向的堆内存已经被释放）。 |
| lock()      | 如果当前weak_ptr已经过期，则该函数会返回一个空的shared_ptr指针；反之该函数返回一个和当前weak_ptr指针指向相同的shared_ptr指针， **但是出了作用域，会让引用计数减少1**。 |

再次强调，weak_ptr<T>模板类没有重载*和->运算符，因此weak_ptr类型指针只能访问某一shared_ptr指针指向的堆内存空间，无法对其进行修改。

```cpp
class B;

class A{
public:
    A(){ std::cout << "A created" <<std::endl; }
    ~A(){
        std::cout << "A disappear" <<std::endl;
    }

    std::weak_ptr<B> b1;
};


class B{
public:
    B(){ std::cout << "B created" <<std::endl; }
    ~B(){
        std::cout << "B disappear" <<std::endl;
    }

    std::weak_ptr<A> a1;
};
```



