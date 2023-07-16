### [C++ SPDLOG Logger 使用方法](#)
**介绍**: spdlog是一个开源的、快速的、仅有头文件的C++日志库。它提供了向stream、标准输出、文件、系统日志、调试器等目标输出日志的能力，支持的平台包括Windows、Linux、Mac、Android。

-----
它使用[fmt](https://github.com/fmtlib/fmt)库提供了类似于python的格式化API（请参阅参考资料）：

spdlog 的头文件可以按需引用，而不是全部使用它！

例如：

* 如果您只需要旋转记录器，则需要包含头文件 **spdlog/ssink/rotating_file_sink.h**

* 如果你需要获得异步日志记录功能则只需要包含头文件 **spdlog/async.h**。

```cpp
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging
#include "spdlog/sinks/rotating_file_sink.h" // support for rotating file logging

int main(int, char* [])
{
    try 
    {
        //  创建基本的文件日志记录器
        auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
        
        //  创建基本的旋转记录器 5MB大小 3 个旋转文件 
        auto file_logger = spdlog::rotating_logger_mt("file_logger", "myfilename", 1024 * 1024 * 5, 3);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
```

使用工厂方法创建异步记录器：

```cpp
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/async.h" //support for async logging.
#include "spdlog/sinks/basic_file_sink.h"

int main(int, char* [])
{
    try
    {        
        auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");
        for (int i = 1; i < 101; ++i)
        {
            async_file->info("Async message #{}", i);
        }
        // Under VisualStudio, this must be called before main finishes to workaround a known VS issue
        spdlog::drop_all(); 
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
```







spdlog中各对象都分为**多线程和单线程版本**：

* \*_st：单线程版本，不用加锁，效率更高；
* \*_mt：多线程版本，用于多线程程序下是线程安全的；

spdlog **整体结构**可分为三层，自下而上分别是:
* **sink** 执行具体的动作，如写入日志文件或者输出到命令行；
* **logger**，用户调用logger的方法，执行相关动作，logger会调用自身的所有sink进行输出；
* **registry**, 管理所有的logger，用户创建的logger会注册到registry统一管理。可以通过spdlog::get()访问已创建的logger；


#### [日志记录槽sink](#)
spdlog定义了几种sink，用于不同场景下日志输出，具体包括：

* 生成日志文件；
* 控制台日志输出（支持颜色）；
* 支持设置日志文件占用的最大空间，以及最多包含多少个；
* 支持每天生成日志文件；

#### [日志级别](#)
spdlog默认支持七种日志级别，spdlog以枚举的方式提供了七个等级：

* **trace**
* **debug**
* **info**
* **warn**
* **err**
* **critical**
* **off**

```cpp
enum level_enum {
	trace = SPDLOG_LEVEL_TRACE 0
	debug = SPDLOG_LEVEL_DEBUG 1
	info  = SPDLOG_LEVEL_INFO  2(默认输出等级)
	warn = SPDLOG_LEVEL_WARN 3
	err = SPDLOG_LEVEL_ERROR   4
	critical = SPDLOG_LEVEL_CRITICAL 5
	off  = SPDLOG_LEVEL_OFF 6
}
```

等级按照大小进行排序，设定等级A后，小于A的信息将不再输出.



#### [输出到终端上面](#)

将日志消息输出到终端：

```cpp
spdlog::info("Initialise Thread Pool argc - cores:{0} type:{1} maxQueueSize:{2} maxthread {3}", 4, "Flexible", 2048, 16);
//错误信息
spdlog::error("create core thread failed {0} {1}", __FILE__, __LINE__);
//二进制显示
spdlog::warn("Easy padding in numbers like {:08d}", 12);
//各种进制支持
spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
//精度
spdlog::info("Support for floats {:03.2f}", 1.23456);
//左对齐
spdlog::info("{:<30}", "left aligned");
```

