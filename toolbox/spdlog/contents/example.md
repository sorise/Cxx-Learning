

### [logger 创建例子](#)

创建一个异步多线程安全的logger,配合两个 sink，输出信息到日志文件和控制台中。

```cpp
try
{
    auto tp = std::make_shared<spdlog::details::thread_pool>(128, 1);
    std::vector<spdlog::sink_ptr> sinks;

    auto rotating = make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "threadPoolRunning.log", 1024 * 1024 * 500, 3, false
    );

    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    sinks.push_back(rotating);

    sinks.push_back(consoleSink);

    auto threadsLogger = 
        std::make_shared<spdlog::async_logger>(
      	  "threadsLogger", 
           begin(sinks), end(sinks), 
       	   tp, 
           spdlog::async_overflow_policy::block
    	);

    threadsLogger->flush_on(spdlog::level::warn);

    for (size_t i = 10; i < 10000; i++)
    {
        threadsLogger->info("this is a log {}", i);
    }
    threadsLogger->flush();

}
catch (const spdlog::spdlog_ex& ex)
{
    std::cout << "Log initialization failed: " << ex.what() << std::endl;
}
```

