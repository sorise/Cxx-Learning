### [C++ spdlog 核心组件](#)

**介绍**：Logger是Spdlog日志库的核心组件，它负责记录和管理日志。

----

- [1. Logger 概述](#1-logger-概述)
- [2. Sink 概述](#2-sink-概述)
- [3. formatter 概述](#3-formatter-概述)



-----

### [1. Logger 概述](#)

Logger的定义如下：

```cpp
class logger {
public:
    explicit logger(std::string logger_name, sinks_init_list sinks);

    template<typename T>
    void log(level::level_enum lvl, const T &msg);
    
	template<typename... Args>
    void log(source_loc loc, level::level_enum lvl, format_string_t<Args...> fmt, Args &&... args);
    
    template<typename... Args>
    void log(level::level_enum lvl, format_string_t<Args...> fmt, Args &&... args);
   
    // ...
    template<typename... Args>
    void trace(format_string_t<Args...> fmt, Args &&... args);
    
    template<typename... Args>
    void debug(format_string_t<Args...> fmt, Args &&... args);

    template<typename... Args>
    void info(format_string_t<Args...> fmt, Args &&... args);
    // ....
private:
    std::string name_;
    std::vector<sink_ptr> sinks_;
    // ...
};
```

Logger主要包含 **一个名称** 和 **一组Sink**，它还提供了log()方法用于记录日志。当调用log()方法时，Logger会将日志消息传递给每个Sink，并由Sink将日志输出到目标。Logger还提供了其他一些方法，比如设置日志级别、添加和删除Sink等。

Logger 是个日志包装器，包含了日志名称和一组Sink，它提供了输出不同级别日志的方法，通过不同Sink的组合可以输出到一个或多个不同输出路径（文件，控制台，网络等）。

[**logger 日志名全局唯一**](#)

每个 `logger` 都有一个名称，并且是全局唯一的，通过上一篇提到的 `register` 组件注册到全局的 map里，代码如下， `registry` 的 `loggers_` 字段通过名字记录了所有的 `logger` 实例。

```cpp
class registry
{
public:
    // ..... 
    // 注册日志
	  void register_logger(std::shared_ptr<logger> new_logger);
 private:
  // ....
	std::unordered_map<std::string, std::shared_ptr<logger>> loggers_;
}

//通过spdlog::get(std::string)获取指定名称的logger进行使用
```

`registry` 提供 `register_logger` 接口注册日志。这里值得注意点是：注册时候如果发现已经存在则会抛异常， `throw_if_exists_` 会检查是否已经存在同名日志实例，存在则通过 `throw_spdlog_ex` 抛出异常。

```cpp
SPDLOG_INLINE void registry::throw_if_exists_(const std::string &logger_name)
{
    if (loggers_.find(logger_name) != loggers_.end())
    {
        throw_spdlog_ex("logger with name '" + logger_name + "' already exists");
    }
}

SPDLOG_INLINE void registry::register_logger_(std::shared_ptr<logger> new_logger)
{
    auto logger_name = new_logger->name();
    throw_if_exists_(logger_name);
    loggers_[logger_name] = std::move(new_logger);
}
```

#### 1.1 日志输出控制

[**提供不同级别日志的输出接口**](#)

在 `logger` 类中，Spdlog 提供了不同级别日志的输出接口，包括 `trace()`、`debug()`、`info()`、`warn()`、`error()` 和 `critical()` 等。下面是 `logger` 类中提供的不同级别日志输出接口的代码示例：

[**日志输出级别控制**](#)

`logger` 提供了 `set_level` 接口来设置日志级别，这个级别可以是枚举类型 `level` 中的任何一个，比如 `spdlog::set_level(spdlog::level::trace);` 将设置日志级别为 `trace`，这样所有级别的日志都会被记录下来。如果想要只记录 `info` 级别及以上的日志，则可以使用 `spdlog::set_level(spdlog::level::info);`。

如果想要在运行时动态地设置日志级别，可以使用 `set_level()` 方法，例如 `logger->set_level(spdlog::level::trace);` 将设置当前 `logger` 的日志级别为 `trace`。

**注意**：如果想要关闭日志，则可以将日志级别设置为 `off`，例如 `spdlog::set_level(spdlog::level::off);`。

[**日志刷新控制**](#)

`logger`提供了一些控制日志刷新的方法，最重要的方法是flush()。当调用flush()方法时，Logger会将所有挂起的日志消息刷新到Sink中。Logger还提供了set_pattern()方法，用于设置日志格式化模式。这个方法可以用于修改日志的输出格式，例如添加时间戳、线程ID等信息。

```cpp
void flush();
```

遇到指定级别的日志会立马将缓存输出到文件中，如果不立刻写入，当程序发生崩溃或产生异常而退出时，有些重要log可能还没等写入到文件中。

```cpp
daily_logger->flush_on(spd::level::err);
daily_logger->info(123.44);
daily_logger->error("Error happended! ");
```

[**日志格式设置**](#)

`logger` 提供了设置日志格式的方法 `set_pattern` 通过此方法可以设置包含的 skin的日志格式。

```cpp
void set_pattern(std::string pattern, pattern_time_type time_type = pattern_time_type::local);
```

通过格式字符串 pattern，会生成 `formatter` 实例，调用 skin的`set_formatter` 接口设置日志格式。



#### [1.2 使用spdlog自带的logger](#)

```cpp
auto logger1 = spdlog::stdout_color_st("LoggerName1"); //创建名称为LoggerName1、内容输出到控制台的单线程版本日志记录器

//<2.创建名称为LoggerName2、内容输出到Logs/BasicFileLog.txt的多线程版本日志记录器
auto logger2 = spdlog::basic_logger_mt("LoggerName2", "Logs/BasicFileLog.txt");

//<3.创建名称为LoggerName3、内容输出到Logs/RotatingFileLog.txt的多线程版本日志记录器
//参数1024*1024*5设置了文件最大容量为5mb，参数3设置了文件最大数量为3
//当日志文件存储超过5mb时，将重命名日志文件并且新增另外一个日志文件
//当日志文件数量超过3时，将删除第一个日志文件
auto logger3 = spdlog::rotating_logger_mt("LoggerName3", "Logs/RotatingFileLog.txt", 1024 * 1024 * 5, 3);

//<4.创建名称为LoggerName4、内容输出到Logs/DailyFileLog.txt的多线程版本日志记录器
//参数2和30指定每天生成日志文件的时间为凌晨2点30分
auto logger4 = spdlog::daily_logger_mt("LoggerName4", "Logs/DailyFileLog.txt", 2, 30);
```

#### [1.3 输出文件](#)

```cpp
// include 相关头文件
// include "spdlog/sinks/basic_file_sink.h"
// 开启并创建本地日志
auto my_logger = spdlog::basic_logger_mt("file_logger", "logs/basic-log.txt");

// 设置该日志的显示级别
my_logger->set_level(spdlog::level::warn);

// 向该日志中写入信息
my_logger->info("Hello, {}!", "World");

// 实时刷新日志
my_logger->flush_on(spdlog::level::trace);
```



### [2. sink 概述](#)

Sink负责将日志输出到不同的目标。Spdlog提供了多种Sink，包括stdout_sink（输出到控制台）、basic_file_sink（输出到文件）、syslog_sink（输出到syslog）等。

Sink组件的实现采用继承抽象类的方法，提供了灵活的扩展能力。最基础的sink基类定义如下：

```cpp
class sink
{
public:
    virtual ~sink() = default;
    virtual void log(const details::log_msg &msg) = 0;
    virtual void flush() = 0;
    virtual void set_pattern(const std::string &pattern) = 0;
    virtual void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) = 0;

    void set_level(level::level_enum log_level);
    level::level_enum level() const;
    bool should_log(level::level_enum msg_level) const;

protected:
    // sink log level - default is all
    level_t level_{level::trace};
};
```

`skin` 是最基础的抽象类，提供了 `level_` 日志等级设置。并且提供了，下面四个抽象方法：

- `log`：用于接收 `spdlog::details::log_msg` 类型的日志消息，实现将消息输出到指定目标的功能。
- `flush`：用于将缓冲区中的日志消息输出到目标。
- `set_pattern`：用于设置日志格式。该方法接收一个字符串参数，包含格式说明符和文本内容。
- `set_formatter`：用于设置日志的格式化器。该方法接收一个 `std::unique_ptr<spdlog::formatter>` 类型的参数，表示要使用的格式化器的指针。

#### [2.1 base_sink](#)

base_sink 继承自 sink，在次基础上提供了锁和格式化器，其中 `mutex_` 用于保证多线程环境下的数据安全，`formatter_` 表示该 sink 所使用的 formatter。

```cpp
class base_sink : public sink
{
public:
    base_sink();
    explicit base_sink(std::unique_ptr<spdlog::formatter> formatter);
    ~base_sink() override = default;

    base_sink(const base_sink &) = delete;
    base_sink(base_sink &&) = delete;

    base_sink &operator=(const base_sink &) = delete;
    base_sink &operator=(base_sink &&) = delete;

    void log(const details::log_msg &msg) final;
    void flush() final;
    void set_pattern(const std::string &pattern) final;
    void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) final;

protected:
    // sink formatter
    std::unique_ptr<spdlog::formatter> formatter_;
    Mutex mutex_;

    virtual void sink_it_(const details::log_msg &msg) = 0;
    virtual void flush_() = 0;
    virtual void set_pattern_(const std::string &pattern);
    virtual void set_formatter_(std::unique_ptr<spdlog::formatter> sink_formatter);
};
```

`sink_it_()` 方法用于实现将日志输出到目标的具体逻辑，`flush_()` 用于实现将缓冲区中的日志消息输出到目标，`set_pattern_()` 用于设置日志格式，`set_formatter_()` 用于设置日志的格式化器。由于 `base_sink` 是一个抽象类，所以这些方法都是纯虚函数，需要在子类中实现。

#### [2.2 自定义锁](#)

`mutex_` 的类型通过模板传递，也就是说我们可以自定义锁。那么自定义锁需要满足哪些条件呢？在 `log` 写日志函数里可以找到调用：

```cpp
template<typename Mutex>
void SPDLOG_INLINE spdlog::sinks::base_sink<Mutex>::log(const details::log_msg &msg)
{
    std::lock_guard<Mutex> lock(mutex_);
    sink_it_(msg);
}
```

std::lock_guard 使用了自定义的锁，查看 lock_guard的实现，是个模板函数，会调用到 Mutex的 `lock` 和 `unlock` 方法。

```cpp
template<typename _Mutex>
    class lock_guard
    {
    public:
      typedef _Mutex mutex_type;

      explicit lock_guard(mutex_type& __m) : _M_device(__m)
      { _M_device.lock(); }

      lock_guard(mutex_type& __m, adopt_lock_t) noexcept : _M_device(__m)
      { } // calling thread owns mutex

      ~lock_guard()
      { _M_device.unlock(); }

      lock_guard(const lock_guard&) = delete;
      lock_guard& operator=(const lock_guard&) = delete;

    private:
      mutex_type&  _M_device;
    };
```

也就是说我们自定义的 Mutex 只要实现 `lock` 与 `unlock` 方法就可以了。spdlog 也给我们提供了默认的无锁Mutex，来提供单线程不加锁的日志实现。

```cpp
struct null_mutex
{
    void lock() const {}
    void unlock() const {}
};
```

#### [2.3 实现自定义日志输出](#)

继承 `base_sink` 后我们可以定义自己的输出实现，只需要实现下面两个接口：

```cpp
virtual void sink_it_(const details::log_msg &msg) = 0;
virtual void flush_() = 0;
```

- sink_it_：用于写日志
- flush_：用于刷新缓冲区到输出

目前 spdlog 已经提供了多种 Sink 的实现，包括：

- stdout_sink（输出到控制台）
- basic_file_sink（输出到文件）
- daily_file_sink（每天输出到不同的日志文件）
- rotating_file_sink（按文件大小或时间自动切分日志文件）
- null_sink（无输出）
- syslog_sink（输出到系统日志）
- udp_sink（输出到udp)
- tcp_sink（输出到tcp）
- kafka_sink（输出到kafka）
- …

如果需要自定义输出实现，只需要继承 `base_sink` 并实现 `sink_it_` 和 `flush_` 两个接口即可。同时，也可以自定义锁，只需要实现 `lock` 和 `unlock` 两个方法，并在继承 `base_sink` 时将锁作为模板参数传入即可。



### [3. formatter 概述](#)

Formatter负责将日志格式化为字符串。Spdlog提供了多种Formatter，比如pattern_formatter（按指定的格式输出日志）、json_formatter（以JSON格式输出日志）等。

Formatter的定义如下：

```cpp
class formatter
{
public:
    virtual ~formatter() = default;
    virtual void format(const details::log_msg &msg, memory_buf_t &dest) = 0;
    virtual std::unique_ptr<formatter> clone() const = 0;
};
```

`formatter` 是一个抽象类，它包含一个format()方法用于将日志消息格式化为字符串。不同的Formatter实现会按照不同的格式输出日志。

实际在使用的是 `pattern_formatter` （继承自formatter），在base_sink的默认构造函数里为我们初始化好了。

```cpp
template<typename Mutex>
SPDLOG_INLINE spdlog::sinks::base_sink<Mutex>::base_sink()
    : formatter_{details::make_unique<spdlog::pattern_formatter>()}
{}
```





