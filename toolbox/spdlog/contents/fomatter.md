### [3. Custom formatting](https://github.com/gabime/spdlog/wiki/3.-Custom-formatting)

**介绍**:每个记录器的接收器都有一个格式化程序，用于将消息格式化到其目的地，并且支持用户自定义格式化。

默认日志格式：`[2014-10-31 23:46:59.678] [my_loggername] [info] Some message`

有两种方法可以自定义记录器的格式：

- 设置模式字符串(**推荐**):

```
set_pattern(pattern_string);
```

- 或者实现实现格式化程序接口和调用的自定义格式化程序

```
set_formatter(std::make_unique<my_custom_formatter>());
```



#### set_pattern(..)的使用

格式可以全局应用于所有注册的日志记录器 logger：

```cpp
spdlog::set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
```

特定的日志记录器对象：

```cpp
some_logger->set_pattern(">>>>>>>>> %H:%M:%S %z %v <<<<<<<<<");
```

或特定sink对象：

```cpp
some_logger->sinks()[0]->set_pattern(">>>>>>>>> %H:%M:%S %z %v <<<<<<<<<");
some_logger->sinks()[1]->set_pattern("..");
```



#### 模板参数 Pattern flags

模式标志的形式为%flag，类似于 C语言日期格式化 **strftime** 函数：

```cpp
std::time_t t = std::time(nullptr);
char mbstr[100];
if (std::strftime(mbstr, sizeof(mbstr), "%Y/%m/%d %H:%M:%S", std::localtime(&t))) {
    std::cout << mbstr << '\n';
}//2023/06/27 20:47:10
```

链接： [https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags](#)

| 标识 | 意义             | 例如                                                       |
| ---- | ---------------- | ---------------------------------------------------------- |
| `%v` | 真实文本         | "some user text"                                           |
| `%t` | 线程ID           | "1232"                                                     |
| `%P` | 进程ID           | "3456"                                                     |
| `%n` | 日志对象名称     | "some logger name"                                         |
| `%l` | 日志级别         | "debug", "info", etc                                       |
| `%L` | 日志级别的短写法 | "D", "I", etc                                              |
| `%+` | 默认格式         | "[2014-10-31 23:46:59.678] [mylogger] [info] Some message" |

```cpp
 // change log pattern
spdlog::set_pattern("%+ %v [thread:%t] [source: %s]");

 SPDLOG_INFO("Support for floats {:03.2f}", 1.23456);
// [2023-07-12 21:18:26.834] [info] [main.cpp:18] 
// Support for floats 1.23 Support for floats 1.23 [thread:3160] [source: main.cpp]
```

