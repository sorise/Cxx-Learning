### [C++11 多线程高性能库ConcurrentQueue 的使用](#)

**介绍**: ConcurrentQueue是一个工业级(header only)的多生产者、多消费者无锁队列。

---

Github地址：https://github.com/cameron314/concurrentqueue

但生产者单消费者队列：https://github.com/cameron314/readerwriterqueue



#### 概述说明

队列不是**顺序一致的**



#### 头文件引入

将以下2个文件复制到项目目录即可使用

* **concurrentqueue.h**
* **blockingconcurrentqueue.h**

```cpp
#include "concurrentqueue.h"
#include "blockingconcurrentqueue.h"

using namespace std;
using namespace moodycamel;
```



#### 全部Api，伪代码

```cpp
# Allocates more memory if necessary
enqueue(item) : bool
enqueue(prod_token, item) : bool
enqueue_bulk(item_first, count) : bool
enqueue_bulk(prod_token, item_first, count) : bool

# Fails if not enough memory to enqueue
try_enqueue(item) : bool
try_enqueue(prod_token, item) : bool
try_enqueue_bulk(item_first, count) : bool
try_enqueue_bulk(prod_token, item_first, count) : bool

# Attempts to dequeue from the queue (never allocates)
try_dequeue(item&) : bool
try_dequeue(cons_token, item&) : bool
try_dequeue_bulk(item_first, max) : size_t
try_dequeue_bulk(cons_token, item_first, max) : size_t

# If you happen to know which producer you want to dequeue from
try_dequeue_from_producer(prod_token, item&) : bool
try_dequeue_bulk_from_producer(prod_token, item_first, max) : size_t

# A not-necessarily-accurate count of the total number of elements
size_approx() : size_t
```

