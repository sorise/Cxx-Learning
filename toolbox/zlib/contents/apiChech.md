## [zlib 函数API](#)

zlib官方的API中压缩使用 **deflateInit** 或者 **deflateInit2**，解压使用 **inflateInit** 或者 **inflateInit2**，推荐使用后者，后续我们也是根据后者来对zlib进行一个封装

### [deflateInit](#)
deflateInit函数是zlib库提供的用于初始化压缩流的函数。

```cpp
int deflateInit(z_stream *strm, int level);
```
参数说明：
* **strm**：指向z_stream结构体的指针，用于保存压缩流的相关信息。
* **level**：压缩级别，取值范围为0到9，其中0表示无压缩，1表示速度最快，9表示压缩比最高，默认值为 Z_DEFAULT_COMPRESSION。

返回值：
* 如果初始化成功，则返回 **Z_OK** 。
* 如果发生错误，则返回一个错误代码，例如Z_MEM_ERROR表示内存分配失败。

在使用deflateInit函数时，首先需要声明一个z_stream结构体变量，并进行相关字段的初始化。然后调用deflateInit函数进行压缩流的初始化，可以设置压缩级别（可选），并检查返回值以确保初始化成功。此后可以对数据进行压缩处理。最后，务必调用deflateEnd函数释放资源。

```cpp
#include <iostream>
#include <zlib.h>

int main() {
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    int ret = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        std::cout << "deflateInit failed: " << ret << std::endl;
        return ret;
    }

    // 压缩处理...

    deflateEnd(&stream);

    return 0;
}
```

### [deflateInit2](#)
相比于，参数更多

```cpp
int deflateInit2(z_stream *strm, int level, int method, int windowBits, int memLevel, int strategy);
```
* **strm**：指向z_stream结构体的指针，用于保存压缩流的相关信息。
* **level**：压缩级别，取值范围为0到9，其中0表示无压缩，1表示速度最快，9表示压缩比最高，默认值为Z_DEFAULT_COMPRESSION。
    * 值为Z_NO_COMPRESSION表示不进行压缩，数据仅通过DEFLATE算法进行处理。
    * 值为Z_BEST_SPEED表示以最快的速度进行压缩，但压缩比较低。
    * 值为Z_BEST_COMPRESSION表示以最高的压缩比进行压缩，但速度较慢。
    * 默认值为Z_DEFAULT_COMPRESSION，根据zlib库的实现有所不同，通常设置为一个平衡速度和压缩比的中间值。
* **method**：压缩算法的方法，一般使用Z_DEFLATED即可。
* **windowBits**：窗口大小，用于控制压缩算法的内部缓冲区大小，默认值为15（即32K），取值范围为8到15。
    * 值为8至15表示滑动窗口的大小为2的窗口大小指数（从256字节到32K字节）。
    * 值为Z_DEFAULT_WINDOW_BITS表示使用默认的窗口大小，默认为15。
* **memLevel**：内存使用级别，取值范围为1到9，其中1表示内存使用最少，9表示内存使用最大，默认值为8。
* **strategy**：压缩策略，可选择Z_DEFAULT_STRATEGY、Z_FILTERED、Z_HUFFMAN_ONLY、Z_RLE或Z_FIXED，默认值为Z_DEFAULT_STRATEGY。
    * 值为Z_FILTERED表示使用基于过滤器的压缩策略，将尝试识别和压缩重复模式。
    * 值为Z_HUFFMAN_ONLY表示只使用霍夫曼编码，不使用LZ77算法。
    * 值为Z_RLE表示使用重复长度编码（Run-Length Encoding）进行压缩。
    * 值为Z_FIXED表示强制禁止压缩，即使有重复的模式。
        

这些选项可以通过调用deflateInit2函数时提供给level、windowBits和strategy参数来设置。

返回值：

* 如果初始化成功，则返回 **Z_OK**。
* 如果发生错误，则返回一个错误代码，例如 **Z_MEM_ERROR** 表示内存分配失败。

```cpp
#include <iostream>
#include <zlib.h>

int main() {
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    int ret = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        std::cout << "deflateInit2 failed: " << ret << std::endl;
        return ret;
    }

    // 压缩处理...

    deflateEnd(&stream);

    return 0;
}
```
在使用deflateInit2函数时，与deflateInit相似，需要首先声明一个z_stream结构体变量，并进行相关字段的初始化。然后调用deflateInit2函数进行压缩流的初始化，可以根据需求提供更多的选项进行配置，如压缩级别、压缩算法方法、窗口大小等。最后，务必调用deflateEnd函数释放资源。

需要注意的是，在使用deflateInit2函数时，需要确保调用了zlib库的相关初始化函数（例如 gzopen 、gzwrit e等），以便正确加载zlib库。

### [deflate](#)

`deflate`函数将输入数据进行压缩，并将结果输出到`strm`结构中配置的输出缓冲区。

```cpp
int deflate(z_stream *strm, int flush);
```

**deflate**函数接受两个参数：

1. **strm**：指向 `z_stream` 结构的指针，该结构包含了压缩的状态和配置信息。
2. **flush**：指定压缩时的刷新选项。可以使用以下值：
   - **Z_NO_FLUSH**：不进行刷新操作。
   - **Z_SYNC_FLUSH**：将输出缓冲区中的数据刷新到输出流，以便其他程序读取。
   - **Z_FINISH**：完成整个压缩，输出所有剩余的数据。

**返回值**：成功返回 **Z_OK**，表示压缩操作成功进行；返回 **Z_STREAM_END**，表示输入数据已压缩完成。



```cpp
z_stream strm;
memset(&strm, 0, sizeof(strm));
int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
if(ret != Z_OK) {
    // 处理初始化失败的情况
    printf("deflateInit failed");
}else{

    char dataBuffer[512] 
    {"Generator expresions are allowehe context of many target "
    "properties, such as LINK_LIBRARIES, INCLUDE_DIRECTORIES, COMPILE_DEFINITIONS and others. "
    "They may also be used when using commands to pooperties, suchet_link_libraries(),"
    " target_include_directories(), target_compile_definitions() and others. They enable conditional linking,"
    " conditional definitions used when compiling, conditional include directories, and more. The conditions "
    "may be based on the build configuration, or any other queryable"
    " information."};
    char outBuffer[512];

    strm.next_in = (Bytef*)dataBuffer; // 待压缩的输入数据
    strm.avail_in = 512; // 输入数据的大小
    strm.next_out = (Bytef*)outBuffer; // 输出缓冲区
    strm.avail_out = 512; // 输出缓冲区的大小

    ret = deflate(&strm, Z_FINISH);
    if(ret != Z_STREAM_END) {
        printf("deflate failed");
    }else{
        printf("len :%ld\n", strm.avail_out);
        deflateEnd(&strm);
    }
}
```

### inflateInit
用于初始化解压缩流。

```cpp
int inflateInit(z_stream *strm);
```

### inflateInit2
用于初始化解压缩流，并提供更多的选项进行配置。

```cpp
int inflateInit2(z_stream *strm, int windowBits);
```

### gzopen
用于打开一个gzip格式的文件，并返回一个gzFile指针。
```cpp
gzFile gzopen(const char *path, const char *mode);
```

### gzwrite
用于将数据写入到gzip文件中。

```cpp
int gzwrite(gzFile file, const void *buf, unsigned len);
```

### gzread
用于从gzip文件中读取数据。

```cpp
int gzread(gzFile file, void *buf, unsigned len);
```
### gzclose
用于关闭打开的gzip文件。

```cpp
int gzclose(gzFile file);
```

这些函数都需要传入一个指向z_stream结构体的指针，该结构体用于保存压缩/解压缩流的相关信息。在调用初始化函数之前，需要对该结构体的相关字段进行适当地初始化。

需要注意的是，以上只是一些常用的zlib库的初始化函数，还有其他更多的函数可供使用，具体使用时请参考zlib库的官方文档或相关资料进行了解。