### [C++ 线程管控](#)
**介绍**: 学会如何启动线程，指定线程指定运行代码，等待线程晚上，分离线程，识别唯一线程！

-----

- [x] [1. 线程管控概述](#1-线程管控概述)
- [x] [2. 向线程函数传递参数](#2-向线程函数传递参数)
- [x] [3. 移交线程归属权](#3-移交线程归属权)
- [x] [4. 线程集合](#4-线程集合)
- [x] [5. 管理当前线程的函数](#5-管理当前线程的函数)
- [x] [6. 线程状态说明](#6-线程状态说明)

-----

### [1. 线程管控概述](#)
需要学会如何启动一个线程，挂起一个线程，向线程传递参数，查验线程是否已经结束，检测运行状态！转移线程的归属权！识别某个真在运行的线程，等待线程执行完毕，获取线程返回值！

**等待线程完成的重要性**: 不等待线程结束，新线程中的函数可能还在运行，如果线程还在运行，有可能访问已经销毁的变量！

**每个C++程序都至少含有一个线程，即运行main函数的线程！它在程序开始运行时由系统启动！随后，程序可以发起更多的线程，它们以别的函数作为入口。这些新线程和起始线程一
起并发运行。当mian函数运行完return时，程序就会退出；同样当入口函数返回时！对应的线程就会随之终结！**


启动一个线程来完成一个加和输出操作！入口函数传递一个整数操作，指定生成多少个整数！
```cpp
#include<iostream>
#include<list>
#include<thread>
#include<algorithm>
#include<ctime>

//伪随机生成10个数字，然后加和输出
void calculate(int count) {
	int sum_count = 0; //存储 整数和
	std::list<int> vals(count); //给随机数准备一个容器
	srand((unsigned int)time(NULL)); //设置随机数种子
    //生成随机数 存储在list中
	std::generate(vals.begin(), vals.end(), [](){ return std::rand() % 1000; });
	for (auto& i : vals) sum_count += i; //求和
	printf("hello concurrency! the sum is %d \n", sum_count); //打印输出
}

int main(int args, char* argv[]) {
	std::thread t(calculate, 10); //10 就是传递给 函数的参数

	t.join(); //等待 t 执行，不然mian可能提前结束，t还没有启动！
	return 0;
}
```


#### [1.1 发起线程](#)
需要引入头文件 **#include\<thread\>** ，线程通过 **std::thread** 对象启动，创建线程对象，需要指定线程的入口函数，还有传递给入口函数的参数！也可以传递无参数的简单函数   **复杂点，还可以传递一个函数对象**

**thread 构造函数如下所示:**
```cpp
thread( thread&& other ) noexcept;

template< class Function, class... Args >
explicit thread( Function&& f, Args&&... args );
//执行于新线程的可调用 (Callable)
//args...	-	传递给新函数的参数
```
**第一个参数：可以传递的**: 函数指针、函数对象、lambda 表达式。

给线程传递一个函数对象！ 如何这个函数对象没有参数，最好使用列表初始化方法，不然会解析错误！
```cpp
class AddBasicPoint
{
private:
  int bp;
public:
  AddBasicPoint(int _bp):bp(_bp){};

  int operator()(int val){
    int all = bp * val;
    printf("%d\n", all);
    return all;
  }
};

int main(int args, char* argv[]) {
	std::thread t {AddBasicPoint(20), 20};

	t.join(); //等待 t 执行，不然mian可能提前结束，t还没有启动！
	return 0;
}
```

传入的可调类型可以是lambda表达式
```cpp
std::thread my_thread([](){
    do_something();
    do_something_else();
});
```

**函数对象的问题:** AddBasic类 重载()的函数是一个无参数函数，那么以下语句将把t解释成一个函数声明，也就是
接受一个参数，返回值是std::thread，函数名为t； 而不是启动一个线程！
```cpp
std::thread t{AddBasic()}; 
```
解决方法: 使用列表初始化或者加括号。
```cpp
std::thread t{AddBasic()}; 
std::thread t((AddBasic())); 
```
**传递类的函数指针方法:** 将对象方法传递给线程，传递静态方法就很简单了！
```cpp
class user {
public:
    user(const std::string &name):
    _name(name){};
    user(const user &other):
    _name(other._name){};
    std::string getName();
    int calculate(const int& a, const int &b);
    ~user(){
        printf("user [%s] is over\n", this->_name.c_str());
    };
private:
    std::string _name;
};

user remix(std::string("remix"));
std::thread task(&user::calculate, &remix, 10, 20);
```


#### [1.2 分离和汇合](#)
[一旦启动了线程，我们就需明确是要等待它结束（与之汇合），还是任由它独自运行（与之分离）。假如等到 std::thread对象销毁之际还没决定好，那std::thread对象的析构函数会调用 std::terminate()终止整个程序！](#)
* **汇合线程**, 实质是父线程等待子线程结束并获取子线程返回的结果的操作。
	* 同步子线程，等待子线程结束。
	* 获取线程过程函数的返回值。
	* 清理并释放子线程的残余资源。
* **分离线程**，如果分离时新线程还未结束运行，那么它将继续运行，甚至在std::thread对象销毁很久之后依然运行，它只有最终从线程函数返回时才会结束运行。
	* 不关心线程过程函数返回值的场景，使用分离线程是最好的方式。
	* 需要处理数据访问正确性问题, 需要高度重视指针和引用数据。
	* 最好使得线程的数据是自含(self-contained)的，尽可能少的引用外部数据、共享变量。

如下所示，当主线程执行到 `f()` 末尾时，按构建的逆序，所有局部对象都会被销毁，但是由于 **my_thread**已经分离，它将继续执行，执行 **do_something(i)** 函数的时候，
**i**引用的 **some_local_state** 作为局部变量已经被销毁了，这将会导致可能的错误发生！
```cpp
struct func {
    int& i;
    func(int& i_) :i(i_) {}
    void operator()() {
        for (unsigned j = 0; j < 1000000; ++j)
            do_something(i); // i是引用，需要注意可能导致悬空引用
    }
};

void f() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach(); //分离线程
}
```
所以较好的方式是：令分离线程的函数持有的数据完全是**自含的**，将数据复制到其内部，而不是共享！
#### [1.3 std::thread](#)
类**thread**表示单个执行线程。线程允许多个函数同时执行。线程在构造关联的线程对象时立即开始执行（等待任何OS调度延迟），从提供给作为构造函数参数的顶层函数开始。

顶层函数的返回值将被忽略，而且若它以抛异常终止，则调用 **std::terminate**。 顶层函数可以通过 **std::promise** 或通过**修改共享变量**（可能需要同步，见 `std::mutex` 与 `std::atomic` ）将其返回值或异常传递给调用方。


| 方法 | 返回值 | 参数个数 | 说明 |
| :----- | :------- | :--------- | :----- |
| join    | void      | void join();        | 等待线程完成其执行, 阻塞当前线程直至 *this 所标识的线程结束其执行!|
| detach    | void      | void detach();        | 容许线程从线程句柄独立开来执行, 从 thread 对象分离执行线程，允许执行独立地持续。|
| swap    | void      | void swap( std::thread& other ) noexcept;        | 交换二个 thread 对象    |
|get_id|std`::`thread`::`id|std`::`thread`::`id get_id() const noexcept|返回标识与 *this 关联的线程的 std`::`thread`::`id 。|
|[静态方法] hardware_concurrency|unsigned int|static unsigned int hardware_concurrency() noexcept;|返回实现支持的并发线程数。应该只把该值当做提示。|
|joinable()|bool|bool joinable() const noexcept;|检查线程是否可合并，即潜在地运行于平行环境中！调用join以后就返回false!|

[**超级重点知识:**](#) **std::thread 对象一经创建，立马开始执行。** **而不是等到你调用 join()或者detach()方法决定好线程是汇合还是分离以后才开始运行！** 
[也就是说很有可能在你调用 join或者 detach方法之前，线程就已经运行完毕了！](#)



**线程ID转换为字符串的方法：**

线程ID有专门的类型，但是这个类型不能直接转换为某个整型或者字符串。它唯一支持序列化的操作是 << 和 hash函数

```cpp
std::thread::id id = std::this_thread::get_id();
std::stringstream strstream;
strstream << id;
std::string mystring = strstream.str();
```



#### [1.4 join 等待线程完成](#)
如果在一个函数中需要等待里面启动的一个线程执行完毕以后，再结束当前函数，可以通过调用成员函数 **join()**实现，将**1.2**代码里面的 **my_thread.detach()** 修改为 **my_thread.join()**, **f** 函数就会等到线程 my_thread
执行完毕以后再销毁局部变量，退出结束。 只要调用 **join** 隶属于该线程的任何存储空间就会立即清除。

对于某个给定的线程，**join()** 只能调用一次，只要 **std::thread** 对象曾经调用过 **join()**，线程就不再可汇合，成员函数 **joinable()** 将返回 **false**。

```cpp
#include<iostream>
#include<thread>

class AddBasicPoint
{
private:
  int bp;
public:
  AddBasicPoint(int _bp):bp(_bp){};

  int operator()(int val){
    for (int i = 0; i < 10000; i++){};
    int all = bp * val;
    printf("%d\n", all);
    return all;
  }
};

void startThread(){
  printf("i will start a thread\n");
  std::thread t { AddBasicPoint(20), 20};
  std::cout << t.joinable() << "\n"; //1
  printf("i will wait thread end\n");
	t.join(); //等待 t 执行，不然mian可能提前结束，t还没有启动！
  std::cout << t.joinable() << "\n"; //0
  printf("the thread end\n");
  return;
}

int main(int args, char* argv[]) {
  startThread();
	return 0;
}
/*
i will start a thread
1
i will wait thread end
400
0
the thread end
*/
```

#### [1.5 创建异常情况](#)
由于系统资源紧张，那么创建线程就可能会失败，然后程序可能会崩溃，若不能开始线程则抛出 std::system_error 。
异常可能表示错误条件 std::errc::resource_unavailable_try_again 或另一实现限定的错误条件。

#### [1.6 join调用前的异常情况](#)
如果启动线程后抛出异常(*并不是说在你传递给线程的函数中抛出异常！是新线程外部抛出异常！*)，但是此时 **join()** 尚未开始指向。那么改 **join** 调用就会略过！不起作用！ 为了防止因异常而导致应用程序终止，
我们需要处理这种情况。虽然 **join** 调用仅仅为了处理没有异常的情况，但是万一发生异常，还是需要调用 **join** 函数，以防止共享变量访问/引用/指针错误的问题。

```cpp
void f() {
    int some_local_state = 0;
    func my_func(some_local_state);//func 是 函数对象，上面 1.2 代码所示
    std::thread my_thread(my_func);
    try
    {
      //在当前线程可能会出错的一些操作
    }
    catch(...)
    {
       my_thread.join(); //发生错误也调用 join
       throw;
    }
	my_thread.join(); //正常调用
}
```

当然我们可以使用 **RAII**(Resource Acquisition is Initialization) 意为“资源获取即初始化”，来实现这个效果！
```cpp
class thread_guard
{
private:
  std::thread& t; 
public:
  thread_guard(std::thread &_t):t(_t){

  };
  ~thread_guard(){
    if (t.joinable()) //还没有汇合的 就汇合以下
    {
      t.join();
    }
  }
  thread_guard(thread_guard const &) = delete;// =delete  禁止默认复制构造函数被调用，限制编译器自动生成新的代码
  thread_guard operator=(thread_guard const&) = delete;
};

void f() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    thread_guard guard(my_thread);
    //在当前线程可能会出错的一些操作
}
```
当函数 `f()` 正常指向到末尾的时候，按构建的逆序，全体局部对象都会销毁。因为类型 **thread_guard**的对象**g**首先被销毁，在其析构函数中，新线程指向汇合函数。
即使是 `f()` 函数中接下来的代码抛出异常，以上行为仍然发生。


#### [1.6 detach 后台运行线程](#)
调用线程的 **detach()** 方法会让线程后台运行，遂无法与之直接通信、也无法等待它结束，也不可能获得
与它相关联的 **`std::thread`** 对象。**分离后的线程仍在后台运行，其归属权和控制权都转移给C++ 运行时库 runtime library**。
**由此保证，一旦线程退出，与之关联的资源都会被正确回收**。分离出的线程常常被称为**守护线程 (daemon thread)!** 只有 joinable() 返回ture的时候采用调用 detach。

> **守护线程**:  这种线程往往长时间运行，几乎在应用程序的整个生存期，他们都一直运行，以执行后台任务：如文件系统监控、从对象缓存中清除无用数据项目、优化数据结构等！


```cpp
void calculate(int count) {
	int sum_count = 0;
	std::list<int> vals(count);
	srand((unsigned int)time(NULL));
	std::generate(vals.begin(), vals.end(), [](){ return std::rand() % 1000; });
	for (auto& i : vals) sum_count += i;
	printf("hello concurrency! the sum is %d \n", sum_count);
}

int main(int args, char* argv[]) {
	std::thread t(calculate, 20);
	t.detach();
	assert(!t.joinable());

  // 等等 calculate
	char str[256];
	printf("请输入一个内容: \n");
	std::cin.getline(str, 200);
	return 0;
}
```
**线程一旦分离、就不能再汇合！**


### [2. 向线程函数传递参数](#) 
向线程传递参数看起来非常简单，只需要将参数传递给 ***std::thread** 的构造函数就可以了(如下所示), 其实不然，里面还有些弯弯绕绕。

[**关键：线程具有内部存储空间**](#)，**参数会按照默认方式先复制到存储空间，新创建的执行线程才能直接访问它们，然后这些副本被当做临时变量，以** [**右值**](#) **的形式传递给新线程上的可调用对象**
**即使函数的相关参数按照设想应该是引用**

```cpp
std::thread t(calculate, 20);
std::thread t(f, 2, 20, "hello");
std::thread t(task, remix, count, true);
```

**引用传递的问题** 如下调用不会通过运行，因为calculate要求的是一个引用参数，应该是左值，但是传递的确实右值。

```cpp
//参数是引用
void calculate(int &count) {
  /* 函数内容 */
}

int main(int args, char* argv[]) {
  int val = 20;
  std::thread t(calculate, val);
  t.detach();

  char str[256];
  std::cin.getline(str, 200);
}
```

#### [2.1 正确传递引用](#)
[**对于传递给线程的参数，默认情况下执行的是拷贝，即使你传递的是引用，还是会复制一份对象，而不是拷贝引用！ 想要传递真正的引用，需要 std::ref()函数，进行包装！**](#)
**但是请记住，如果线程是分离的，那么传递引用需要充分考虑安全问题，如果函数调用结束引用对象回收，那么可怕的事情可能发生！** 所以最好对join的线程使用引用传递参数！

```cpp
//参数是引用
void calculate(int &count) {
  /* 函数内容 */
}

int main(int args, char* argv[]) {
  int val = 20;
  std::thread t(calculate, std::ref(val));
  t.detach();

  char str[256];
  std::cin.getline(str, 200);
}
```

#### [2.2 传递指针](#)
如果你的指针的指向的是栈地址，那么你需要考虑指针指向对象的消亡时机，是否会和线程的执行有所影响。
如果你的指针的指向的堆地址，那么你需要考虑，如何防止反复释放，在什么时机释放，如果实现原子操作。

如果你使用**智能指针**，那么是极好的，但是也需要知道，智能指针有 **unique_ptr**, **share_ptr** 还有
一个**std::move**的操作。 

**智能指针unique_ptr不能直接传递，需要使用 std::move 配合**
```cpp
void calculate(std::unique_ptr<animal> _uptr_animal) {
	printf("%p \n", _uptr_animal);
	std::cout << *_uptr_animal << "\n";
}

int main(int args, char* argv[]) {
	std::unique_ptr<animal> ptrAnimal(new animal("mike", 2, "Kitty"));
	printf("%p \n", ptrAnimal);
	std::thread t(calculate, ptrAnimal);//没有 move配合 无法运行
	t.join();
	return 0;
}
```
**正确使用方式：**
```cpp
std::thread t(calculate, std::move(ptrAnimal)); // move配合
```

#### [2.3 传递一个类](#)
**如果你需要的参数是类，那么请你的函数参数类型为： const ClassName& 或者 ClassName&&**。 不然你会发现函数
找不到参数。

```cpp
#include<iostream>
#include<list>
#include<thread>
#include<algorithm>
#include<functional>
#include<ctime>
#include<assert.h>

class animal
{
friend std::ostream& operator<<(std::ostream& os, const animal& _animal) noexcept {
	os << _animal.name << " " << _animal.age << " ";
	return os;
}
private:
	std::string typeName;
	int age;
	std::string name;
public:
	animal(std::string _name, int _age, std::string _typeName) {
		this->name = _name;
		this->age = _age;
		this->typeName = _typeName;
	}
	animal(const char* _name, const int& _age, const char* _typeName) 
	:name(_name),age(_age), typeName(_typeName){}
	~animal() = default;
	animal(const animal&& other) = delete;
	//复制构造函数
	animal(const animal& other) :name(other.name), age(other.age), typeName(other.typeName) {
		printf("call 复制构造函数\n");
	}
	//赋值运算符
	animal& operator=(const animal& other) {
		printf("call 赋值运算符\n");
		if (&other != this){
			this->name = other.name;
			this->age = other.age;
			this->typeName = other.typeName;
		}
		return *this;
	};
};

//参数
void calculate(animal &&mini) {
	printf("%p \n", &mini);
	std::cout << mini << "\n";
}

int main(int args, char* argv[]) {
	animal kitty("mike", 2, "Kitty");
	printf("%p \n", &kitty);
	std::thread t(calculate, kitty);
	t.join();
	return 0;
}
/*
000000B575B1F8F0
call 复制构造函数
000001C8BAC39DD0
mike 2
*/
```


### [3. 移交线程归属权](#) 
首先要明白 std::thread 和 std::unique_ptr、std::ifstream 一样都是拥有资源的类型，并不能直接复制！，在以下两种情况，需要线程归属权转移的功能。
1. **编写一个函数，用于创建线程，置于后台运行，但该函数本身并不等待线程完结，而是将其归属权上交给函数的调用者。**
2. **创建一个线程，将归属权传入某个函数，由它复制等待该线程结束。**
3. [**std::thread 支持移动操作**](#)

**线程的归属权可以在多个std::thread中多次转移，但是需要std::move函数、移动构造函数来完成转移！** [CMake不支持移动赋值运算符](#)。
以下程序有两个线程，四个std::thread对象。 t1将线程归属权转移给t2. t3将线程归属权转移给 t4。
```cpp
void calculate(int count) {
	int sum_count = 0;
	std::list<int> vals(count);
	srand((unsigned int)time(NULL));
	std::generate(vals.begin(), vals.end(), []() { return std::rand() % 1000; });
	for (auto& i : vals) sum_count += i;
	printf("hello concurrency! the sum is %d \n", sum_count);
}

int main(int args, char* argv[]) {
	using std::thread;
	thread t1([](int number){
		printf("%d input a number: \n", number);
		int rig = scanf_s("%d", &number);
		if (rig == 1)
		{
			printf("the new number is: %d", number);
		}
	}, 10);
	//将线程的归属权从t1 转移给 t2
	thread t2 = std::move(t1);
	thread t3(calculate, 20);
	thread t4;
	t4 = std::move(t3); //把t3 转移给 t4

	t2.join();
	t4.join();
	return 0;
}
```
**注意不要将一个线程归属权转移给一个已经关联了线程了的std::thread对象**。[**std::thread对象正在管控着一个线程，就不能简单地向它赋新值，否则该线程就会因此被遗弃！**](#)
```cpp
thread task1(calculate, 20);
thread task2(calculate, 10);
task1 = std::move(task2); //会导致程序奔溃导致 std::terminate() 被调用！
```

#### [3.1 返回线程](#)
std::thread 支持移动操作,函数可以便捷地向外部转移线程的归属权，
```cpp
std::thread createCalculateThread(int times) {
	void(*f)(int) = calculate;
	return std::thread(f, times);
}

int main(int args, char* argv[]) {
	using std::thread;
	thread t1 = createCalculateThread(10);
	t1.join();
	return 0;
}
```

#### [3.2 传入线程给函数](#)
直接传递std::thread 给函数是不行的，还是需要 std::move 来配合使用。
```cpp
void insertThread(std::thread task) {
	std::cout << task.get_id() << "\n";
	task.join();
}

int main(int args, char* argv[]) {
	using std::thread;
	thread t1(calculate, 10);
  insertThread(std::move(t1)); 
  // 不适应std::move(t1) 直接传递 t1会报如下的错误
  //“std::thread::thread(const std::thread &)”: 尝试引用已删除的函数
	return 0;
}
```

#### [3.3 joining_thread](#)
一种在调用析构函数会自动汇合的线程，这种线程只有一个选择，**汇合**， 包装后称为一种汇合安全的线程，不会已经异常导致线程未汇合而导致错误！！
C++ 20 提供了 **std::jthread** 作为专门汇合线程。
```cpp
class joining_thread
{
    std::thread t;

public:
	// 默认无参构造
    joining_thread() noexcept = default;
	// 可变参数模板构造线程
    template <typename Callable, typename... Args>
    explicit joining_thread(Callable &&func, Args &&...args):
    t(std::forward<Callable>(func), std::forward<Args>(args)...)
    {
    }
    
    // 移动构造
    explicit joining_thread(std::thread t_) noexcept : t(std::move(t_))
    {
    }
    // 右值引用构造
    joining_thread(joining_thread &&other) noexcept : t(std::move(other.t))
    {
    }
    
    // 等待此线程结束后进行赋值操作
    joining_thread &operator=(joining_thread &&other) noexcept
    {
        if(joinable())
        {
            join();
        }
        t = std::move(other.t);
        return *this;
    }
    joining_thread &operator=(std::thread other) noexcept
    {
        if (joinable())
            join();
        t = std::move(other);
        return *this;
    }
	// 析构
    ~joining_thread() noexcept
    {
        if (joinable())
            join();
    }
    void swap(joining_thread &other) noexcept
    {
        t.swap(other.t);
    }
    std::thread::id get_id() const noexcept
    {
        return t.get_id();
    }
    bool joinable() const noexcept
    {
        return t.joinable();
    }
    void join()
    {
        t.join();
    }
    void detach()
    {
        t.detach();
    }
    // get方法
    std::thread &as_thread() noexcept
    {
        return t;
    }
    const std::thread &as_thread() const noexcept
    {
        return t;
    }
};
```

### [4. 线程集合](#) 
生成多个线程，并等待他们完成运行，我们通常会使用 vector容器来完成！

```cpp
std::thread createCalculateThread(int times) {
	void(*f)(int) = calculate;
	std::thread task(f, times);
	std::cout <<"Thread ID" << task.get_id() << "\n";
	return task;
}

int main(int args, char* argv[]) {
	using std::thread;
	std::vector<thread> threads;
	for (int i = 0; i < 10; i++) threads.push_back(createCalculateThread(i + 5));

	for (auto& th : threads)
	{
		th.join();
	}
	return 0;
}
```

#### [4.1 hardware_concurrency](#)
获取硬件支持的并发线程数,返回值是一个指标，表示程序在各次运行中可真正并发的线程数量。在多核系统中，返回值可以是CPU核心的数量，返回值也仅仅是一个提示，当系统无法获取时，函数返回0

```cpp 
//amd r9-4900h 
std::cout << thread::hardware_concurrency << "\n";
//00007FF6EDD21136
```


### [5. 管理当前线程的函数](#) 
在命名空间 this_thread 定义了数个函数用于管理当前线程。

|命令|解释|
|:----|:----|
|yield|建议实现重新调度各执行线程|
|get_id|返回当前线程的线程 id|
|sleep_for|使当前线程的执行停止指定的时间段|
|sleep_until|使当前线程的执行停止直到指定的时间点|

#### [5.1 让当前线程放弃争夺cpu资源](#)
提供提示给实现，以重调度线程的执行，允许其他线程运行。

此函数的准确行为依赖于实现，特别是取决于使用中的 OS 调度器机制和系统状态。例如，先进先出实时调度器（ Linux 的 SCHED_FIFO ）将悬挂当前线程并将它放到准备运行的同优先级线程的队列尾（而若无其他线程在同优先级，则 yield 无效果）。

```cpp
void yield() noexcept;
```
该函数的功能是让当前线程放弃争夺cpu资源，让操作系统调度进行其他操作。如果在线程运行中，需要不断地判断成立
条件，会严重消耗cpu的资源，为了防止循环判断影响cpu的资源，可以判断一次操作是否完成，如果没有完成就调
用yield交出时间片，过一会儿再来判断是否完成，这样这个线程占用CPU时间会大大减少。

```cpp
while(!isDone()); // Bad
while(!isDone()) yield(); // Good
```

**std::this_thread::yield()** 的目的是避免一个线程频繁与其他线程争抢CPU时间片, 从而导致多线程处理性能下降.

std::this_thread::yield(); 是将当前线程所抢到的CPU”时间片A”让渡给其他线程(其他线程会争抢”时间片A”,注意: 此时”当前线程”不参与争抢).
等到其他线程使用完”时间片A”后, 再由操作系统调度, 当前线程再和其他线程一起开始抢CPU时间片。

#### [5.2 返回当前线程的线程 id](#)
线程的标识类型是 `std::thread::id` 这是一个类，可以通过两种方式进行检索。其对象作为线程ID，可以进行复制操作和比较运算，如果两个线程的 `std::thread::id`相等！
则它们表示相同的线程。

1. 可以通过 `std::thread` 对象的成员函数get_id()进行直接获取!, 如果 `std::thread` 对象没有关联任何执行线程，会返回一个按照默认构造函数生成的 `std::thread::id`， **表示线程不存在**。
2. 使用静态方法 `std::thread::get_id()` 获得 
```cpp
class thread::id { // thread id
public:
    //默认构造函数
    id() noxcept : _Id(0) {} // id for no thread
}
```

判断两个线程是否一致！
```cpp
std::thread createCalculateThread(int times) {
	void(*f)(int) = calculate;
	std::thread task(f, times);
	std::cout <<"Thread ID: " << task.get_id() << "\n";
	return task;
}

int main(int args, char* argv[]) {
	using std::thread;
	thread task1 = createCalculateThread(10);
	thread task2 = createCalculateThread(20);
	
	if (task1.get_id() == task2.get_id())
		printf("same thread \n");
	else
		printf("different thread\n");

	task1.join();
	task2.join();
	return 0;
}
```

### [6. 线程状态说明](#)
线程和进程一样都是有状态的，明白线程运行时处于哪个状态对于调试线程非常重要,  <span style="color:#2c73ff;font-weight:600;font-family:宋体" >线程有5种状态：新建（new Thread）、就绪（runnable），运行（running）、阻塞（blocked）、结束（dead）</span>。 `[八股范畴]`

* **初始化(Init/new)**：该线程正在被创建，一旦创建完毕就进入就绪状态，放到就绪队列里面。
* **就绪(Ready)**：该线程在就绪列表中，等待 CPU 调度。
* **运行(Running)**：该线程正在运行。
* **阻塞(Blocked)**：该线程被阻塞挂起。Blocked 状态包括：pend(锁、 事件、信号量等阻塞)、suspend（主动 pend）、delay(延时阻塞)、 pendtime(因为锁、事件、信号量时间等超时等待)。
* **退出(Exit/dead)**：该线程运行结束，**等待父线程回收其控制块资源**。



<img src="./assets/image-20230705194342380.png" alt="image-20230705194342380" width="700px" />



| 方法                     | 状态说明                                                     |
| ------------------------ | ------------------------------------------------------------ |
| task.join()              | join方法，阻塞当前线程thread，直到任务线程task执行完成；     |
| task.detach()            | 使得当前线程脱离主线程的控制，该线程的资源由后台进程回收资源； |
| sleep_for                | 使当前线程的执行停止指定的时间段                             |
| sleep_until              | 使当前线程的执行停止直到指定的时间点                         |
| yield                    | 该函数的功能是让当前线程放弃争夺cpu资源，让操作系统调度进行其他操作。 |
| lock、try_lock_for/until | 阻塞等待获得锁                                               |
















-----