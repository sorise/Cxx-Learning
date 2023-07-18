### [Sink](#)

**介绍**：接收器是实际将日志写入其目标的对象。每个接收器应该只负责单个目标（例如文件、控制台、数据库），并且每个接收器都有自己的格式化程序对象的私有实例。

-------

每个Logger都有一个 **std::vector\<std::shared_ptr\<sinks::sink\>\> sinks_; **在每次日志调用中(如果日志级别正确)，记录器将在每个日志调用上调用，sink(log_msg)”函数。

spdlog 实现的 sink: [源代码文件](https://github.com/gabime/spdlog/tree/v1.x/include/spdlog/sinks/) 、 [文档](https://github.com/gabime/spdlog/wiki/4.-Sinks)



####  1. rotating_file_sink

当达到最大文件大小时，关闭文件，重命名文件，然后创建一个新文件。最大文件大小和最大文件数都可以在构造函数中配置。

```cpp
rotating_logger_mt(const std::string& logger_name, const filename_t& filename, size_t max_file_size, size_t max_files)
```

参数max_files 规定了滚动文件的个数。

* 当logger_name存满时，将其名称更改为logger_name.1，再新建一个logger_name文件来存储新的日志。
* 再次存满时，把logger_name.1改名为logger_name.2，logger_name改名为logger_name.1，新建一个logger_name来存放新的日志。
* max_files 数量为几，就可以有几个logger_name文件用来滚动。

```cpp
size_t max_size = 1024 * 10;
std::string basename = "testLog.log";

std::shared_ptr<spdlog::logger> file_logger;  //日志的文件指针

file_logger = spdlog::rotating_logger_mt("2232", basename, max_size, 0);
```

日志保存在工作目录下的**testLog.log**里面

**或者手动创建接收器并将其传递给记录器：**

```cpp
#include "spdlog/sinks/rotating_file_sink.h"

auto rotating = make_shared<spdlog::sinks::rotating_file_sink_mt> ("log_filename", 1024*1024, 5, false);

auto file_logger = make_shared<spdlog::logger>("my_logger", rotating);
```



#### 2. daily_file_sink 

每天在指定的时间创建一个新的日志文件，并在文件名后附加一个时间戳。

```cpp
#include "spdlog/sinks/daily_file_sink.h"

auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily", 14, 55);
```

将创建一个线程安全接收器，该接收器将在每天14:55创建一个新的日志文件。

注意：从1.5.0版本开始，如果需要，spdlog将创建包含日志文件的文件夹。在此之前，必须手动创建文件夹。



#### [3. simple_file_sink](#)

一个简单的文件接收器，只写入给定的日志文件，没有任何限制。

```cpp
#include "spdlog/sinks/basic_file_sink.h"

auto logger = spdlog::basic_logger_mt("mylogger", "log.txt");
```

注意：从1.5.0版本开始，如果需要，spdlog将创建包含日志文件的文件夹。在此之前，必须手动创建文件夹。



#### [4. stdout_sink/stderr_sink](#)



```cpp
#include "spdlog/sinks/stdout_sinks.h"

auto console = spdlog::stdout_logger_mt("console");
auto err_console = spdlog::stderr_logger_st("console");
```



#### [5. stdout_sink/stderr_sink with colors](#)



```cpp
#include "spdlog/sinks/stdout_color_sinks.h"

auto console = spdlog::stdout_color_mt("console");
auto err_console = spdlog::stderr_color_st("console");

auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
```
