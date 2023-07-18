### [spdlog rotating log](#)

**介绍**：滚动日志，当日志文件超出规定大小时，会删除当前日志文件中所有内容，重新开始写入。

-----



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