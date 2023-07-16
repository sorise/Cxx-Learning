## [zlib 数据类型](#)


#### [z_stream](#)
z_stream是C语言中zlib库提供的结构体，用于压缩和解压缩数据。

在C++中使用zlib库时，可以通过定义一个z_stream类型的变量来操作数据的压缩和解压缩。具体的z_stream结构体定义如下：
```cpp
typedef struct z_stream_s {
    Bytef *next_in;     // 输入数据的起始地址
    uInt   avail_in;    // 输入数据的长度
    uLong  total_in;    // 输入数据已处理的字节数

    Bytef *next_out;    // 输出数据的起始地址
    uInt   avail_out;   // 输出数据的长度
    uLong  total_out;   // 输出数据已处理的字节数

    char   *msg;        // 错误信息
    void   *state;      // 内部状态，不需关注

    alloc_func zalloc;  // 内存分配函数，一般为NULL或者自定义
    free_func  zfree;   // 内存释放函数，一般为NULL或者自定义
    voidpf     opaque;  // 透明指针，一般为NULL或者自定义

    int    data_type;   // 数据类型，不需关注
    uLong  adler;       // Adler-32值，不需关注
    uLong  reserved;    // 保留字段，不需关注
} z_stream;
```

在使用zlib库进行数据压缩和解压缩时，首先需要初始化z_stream结构体，然后设置输入和输出数据的相关参数，使用zlib库提供
的函数进行数据处理，最后释放资源。下面是一个简单的示例代码：

```cpp
#include <iostream>
#include <zlib.h>

int main()
{
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    int ret = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
    {
        std::cout << "deflateInit failed: " << ret << std::endl;
        return ret;
    }

    // 设置输入和输出数据
    Bytef in[1024] = {0};   // 输入数据
    Bytef out[1024] = {0};  // 输出数据

    stream.next_in = in;
    stream.avail_in = sizeof(in);

    stream.next_out = out;
    stream.avail_out = sizeof(out);

    // 压缩数据
    ret = deflate(&stream, Z_FINISH);
    if (ret != Z_STREAM_END)
    {
        std::cout << "deflate failed: " << ret << std::endl;
        deflateEnd(&stream);
        return ret;
    }

    // 检查压缩后的数据长度
    size_t compressedSize = sizeof(out) - stream.avail_out;
    std::cout << "Compressed size: " << compressedSize << std::endl;

    // 解压数据
    ret = inflate(&stream, Z_FINISH);
    if (ret != Z_STREAM_END)
    {
        std::cout << "inflate failed: " << ret << std::endl;
        deflateEnd(&stream);
        return ret;
    }

    // 检查解压后的数据长度
    size_t uncompressedSize = sizeof(in) - stream.avail_in;
    std::cout << "Uncompressed size: " << uncompressedSize << std::endl;

    deflateEnd(&stream);

    return 0;
}
```
