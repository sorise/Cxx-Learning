## [C++成员指针](#)
**介绍**: 不是指 `this` 指针，而是指向对象内部成员的指针，内部成员有数据成员、函数成员、虚函数成员。

----

### [特点](#)
* **成员指针也没办法脱离类的实例对象单独使用**，不管是非静态数据成员指针还是非静态成员函数指针(非要强制转换成员函数指针也是可以的)。
* **静态数据成员和静态成员函数不与类关联**，也就不参与这个成员指针的讨论了。
* **从语法上来，就是加了一个作用于解析运算符**

### [1. 数据成员指针](#)
数据成员指针或**虚函数成员指针**并没有真的指向一个内存，**它只是表示表示在当前的类，那个字段的位置而已**，比如&X::value表示的只是这个数据成员value在X类中的位置。
数据成员指针和虚成员函数指针是一个类似**偏移量**的东西。

```cpp
class A{
public:
    int age = 18;
    void func()  { std::cout << "call A func" << std::endl;}
};

int A::* A_age = &A::age;
//A_age 是一个成员指针类型， 利用 .* 运算符

A a;
a.age = 68;

std::cout << a.age << '\n';//68
int &ref_age = (a.*A_age);

ref_age = 100;

std::cout << a.age << '\n';//100
return 0;
```
和绑定成员函数指针没什么区别，甚至更加方便，我们直接得到了x对象的数据成员x的引用。

**数据成员指针存什么？** **偏移量**, 和数据成员差不多，虚成员函数指针也是偏移量, 这个编译量是相对虚表而言的，当然，虚表的编译器的实现，标准没有规定这些东西，只规定了行为。
```cpp
struct X {
	int a,b;
    double d;
};
int main() {
	auto p = &X::a;
	auto p2 = &X::b;
	auto p3 = &X::d;
    std::cout<<*reinterpret_cast<int*>(&p)<<'\n';//0
    std::cout<<*reinterpret_cast<int*>(&p2)<<'\n';//4
    std::cout<<*reinterpret_cast<int*>(&p3)<<'\n';//8
} //也就印证了我们之前说的存的偏移量
```


### [2. 成员函数指针](#)
证明普通的成员函数指针是存了明确的地址的，这也是必然的，所有函数都有明确地址，不然无法jmp过去。
```cpp
class A{
public:
    void func() { std::cout << "call func" << std::endl;}
};

void(A::*p)() = &A::func;
A a;
(a.*p)();  //call func
```

**应用方向**：
```cpp
class A{
public:
    void func()  { std::cout << "call A func" << std::endl;}
    static void Func() { std::cout << "call Func" << std::endl;}
};

class B{
public:
    void func()  { std::cout << "call B func" << std::endl;}
    static void Func() { std::cout << "call Func" << std::endl;}
};

template<typename T>
void call_func(T a){
    void(T::*p)() = &T::func;
    (a.*p)();  //call func
}

template<typename X>
void f2(void(X::* p)(), X& x) {
    (x.*p)();
}

int main() {
    A a;
    B b;
    
    call_func(a);  //call A func
    call_func(b);  //call B func

    f2(&A::func, a); //call A func
    return 0;
}
```

这属于c++的基本语法，我们常用的std::thread，std::bind，std::invoke都是这样的。

这些一般我们都不会去直接使用，都是间接的，在c++的标准库中有很多好用的包装函数，我们介绍一下c++17的invoke

```cpp
class Test {
public:
	uint16_t num = 0;
	void f() {
		std::cout << "66\n";
	}
};

int main() {
	Test c;
	uint16_t& i = std::invoke(&Test::num, &c);//绑定数据成员返回引用
	i = 9999;
	std::cout << c.num << '\n';
	std::invoke(&Test::f, &c);//直接调用，无返回值
}
```


#### [2.1 解决成员函数重载问题](#)
那么问题又来了，如果我们要传递一个非静态成员函数是有重载的，该怎么办呢?

```cpp
struct X {
	void f() { std::cout << "6\n"; }
	void f(int) { std::cout << "int\n"; }
};
```
如果是这样的话，我们还能直接f2(&X::f, &x,参数);吗？其实是可以的

```cpp
struct X {
	void f() { std::cout << "6\n"; }
	void f(int) { std::cout << "int\n"; }
};

void f2(void(X::* p)(), X* x,int n) {
	(x->*p)();
}

int main() {
	X x;
	f2(&X::f, &x,5);//6
}
```
这种传参还是足以区分重载的，没什么问题

但是我们要考虑另一个问题，比如我们使用std::bind那些库的都是用的模板，如何确定绑定哪个成员函数呢?

这个时候就必须使用static_cast指定类型的成员函数到成员函数指针类型转换来消解函数重载的歧义

```cpp
struct Test_bind {
	void t(int n) {
		for (; n; n--)
			std::cout << "t\n";
	}
	void t() {
		std::cout << "重载的t\n";
	}
};

int main() {
	Test_bind t_b;
	auto n = std::bind(static_cast<void(Test_bind::*)(int)>(&Test_bind::t), &t_b, 3);
	n();
}
```
除了成员函数这样解决，其他的普通函数重载都可以使用这种方式解决，这并没有什么难度

注意，`operator.*`不可以重载，但是 `operator->*`可以重载，如下:
```cpp
struct X {
	void f() { std::cout << "6\n"; }
	template<typename Ty>
	auto operator ->*(Ty v) {
		return (this->*v)();
	}
};

int main(){
	X x;
	x->*& X::f;
}
```
讲完了成员函数指针和那几个运算符，那数据成员指针也是非常的简单了


### [3 成员访问运算符](#)


<table data-draft-node="block" data-draft-type="table" data-size="normal" data-row-style="normal">
	<tbody>
		<tr>
			<th>运算符名</th>
			<th>语法</th>
			<th>可重载</th>
			<th>原型示例（对于 class T ）类定义内</th>
			<th>原型示例（对于 class T ）类定义外</th>
		</tr>
		<tr>
			<td>下标</td>
			<td>a[b]</td>
			<td>是</td>
			<td>R&amp; T::operator[](S b);
				<br>
				<br>
				<br>
				<br>
				<br> R&amp; T::operator[](S1 s1, ...);
				<br>
				<br> (C++23 起)</td>
			<td>N/A</td>
		</tr>
		<tr>
			<td>间接寻址</td>
			<td>*a</td>
			<td>是</td>
			<td>R&amp; T::operator*();</td>
			<td>R&amp; operator*(T a);</td>
		</tr>
		<tr>
			<td>取地址</td>
			<td>&amp;a</td>
			<td>是</td>
			<td>R* T::operator&amp;();</td>
			<td>R* operator&amp;(T a);</td>
		</tr>
		<tr>
			<td>对象的成员</td>
			<td>a.b</td>
			<td>否</td>
			<td>N/A</td>
			<td>N/A</td>
		</tr>
		<tr>
			<td>指针的成员</td>
			<td>a-&gt;b</td>
			<td>是</td>
			<td>R* T::operator-&gt;()</td>
			<td>N/A</td>
		</tr>
		<tr>
			<td>指向对象的成员的指针</td>
			<td>a.*b</td>
			<td>否</td>
			<td>N/A</td>
			<td>N/A</td>
		</tr>
		<tr>
			<td>指向指针的成员的指针</td>
			<td>a-&gt;*b</td>
			<td>是</td>
			<td>R&amp; T::operator-&gt;*(S b);</td>
			<td>R&amp; operator-&gt;*(T a, S b);</td>
		</tr>
	</tbody>
</table>
