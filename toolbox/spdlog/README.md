## [C++ Spdlog日志库 ](https://github.com/gabime/spdlog)
**介绍**：spdlog（Super Fast C++ Logging Library）是一个快速、异步、线程安全、高性能的C++ (header-only) 日志库，它提供了简单易用的API和丰富的功能，能够满足各种日志记录需求。


-----

#### Spdlog 结构

Spdlog 的结构Spdlog 由五个主要组件构成：Loggers、Sinks、Formatters、Async Logger 和 Registry。每个组件都扮演着不同的角色，共同协作记录并输出日志消息。

* **Loggers** ：是 Spdlog 最基本的组件，负责**记录日志消息**。在 Spdlog 中，一个 Logger 对象代表着一个日志记录器，应用程序可以使用 Logger 对象记录不同级别的日志消息。
* **Sinks** ：**决定了日志消息的输出位置**。在 Spdlog 中，一个 Sink 对象代表着一个输出位置，例如控制台、文件、网络等。应用程序可以将不同的日志消息发送到不同的 Sink 中。
* **Formatters** ：负责将日志消息转换为**特定格式**。在 Spdlog 中，一个 Formatter 对象代表着一个消息格式器，应用程序可以使用不同的 Formatter 对象将日志消息转换为不同的格式。
* **Async Logger** ：是 Spdlog 的异步记录器，它负责将日志消息异步地写入到目标 Sink 中。当应用程序调用 Logger 对象记录一个日志消息时，该消息会被加入到一个队列中，然后异步地写入目标 Sink 中。这样可以避免多个线程同时访问 Sink，从而确保线程安全性。
* **Registry** ：用于管理 Spdlog 的所有组件。在 Spdlog 中，所有的 Loggers、Sinks、Formatters 和 Async Logger 都在一个全局注册表中注册，Registry 用于管理这些组件。




















<br/>
<br/>
<br/>
<br/>


**参考:**
> https://www.zhihu.com/column/c_1622996866337132544
> 
> [Github:https://github.com/gabime/spdlog](https://github.com/gabime/spdlog)