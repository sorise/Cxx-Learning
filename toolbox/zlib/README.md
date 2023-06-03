### [C++ Zlib 库](#)

**Zlib** 是一个通用的数据压缩库，所有代码都是线程安全。最早是由 `Jean-loup Gailly` 与 `Mark Adler` 所开发。今天，zlib是一种事实上的业界标准，以至于在标准文档中，`zlib` 和 `DEFLATE` 常常互换使用。
数以万计的应用程序直接或间接依靠zlib压缩函式库，包括： `Linux核心`、`libpng`、`Apache`、`nginx`等等。 

* `official documentation`: [http://www.zlib.net/](http://www.zlib.net/)  
* `manual`: [http://www.zlib.net/manual.html](http://www.zlib.net/manual.html)  

------

预定于的 **版本宏** Version:
```cpp
#define ZLIB_VERSION "1.2.13"
#define ZLIB_VERNUM 0x12d0
#define ZLIB_VER_MAJOR 1
#define ZLIB_VER_MINOR 2
#define ZLIB_VER_REVISION 13
#define ZLIB_VER_SUBREVISION 0
```

### [1. Zlib 常用API](#)
Zlib提供的内存中的压缩和解压功能，包括未压缩数据的完整性检查。zlib官方的API中压缩使用deflateInit或者deflateInit2，解压使用inflateInit或者inflateInit2。压缩的函数位于inflate.c 中，解压的函数位于 deflate.c 文件中，
压缩数据流通过结构体z_stream_s来进行交互。

* 该库未安装任何信号处理程序。解码器检查压缩数据的一致性，因此即使在输入损坏的情况下，库也绝不应该崩溃。
* 在zlib的格式被设计为紧凑和快速的在内存和通信信道的使用。

#### [1.1 使用小例子](#)
这里，zlib只是把一段内存压缩，压缩后放的另一段内存上，这离压缩文件甚至文件夹的目标还很远。
确切的说，zlib可能并不是一个针对ZIP文件的库，它只是一个针对gzip以及deflate算法的库。它提供了一个叫做minizip (contrib\minizip) 例子来给出操作 ZIP 文件的方法。

```cpp
int main(int argc, char *argv[]) {

    unsigned char strSrc[] = "hhhhhhhhhCCCCCChhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhaaaaa中文";

    unsigned char buf[1024] = {0};
    unsigned char strDst[1024] = {0};
    
    unsigned long srcLen = sizeof(strSrc); //源字节
    unsigned long bufLen = sizeof(buf);
    unsigned long dstLen = sizeof(strDst);
    
    //打印一下 源字节信息
    printf("源字符:%s\n长度:%ld\n", strSrc, srcLen);
    
    /* 压缩 */
    compress(buf, &bufLen, strSrc, srcLen);
    printf("压缩后长度:%ld\n", bufLen);
    for (int i = 0; i <bufLen ; ++i) {
        std::cout << (short )buf[i] << " ";
    }
    std::cout << "\n";
    
    /* 解压缩 */
    uncompress(strDst, &dstLen, buf, bufLen);
    printf("解压后长度:%ld\n",dstLen);
    printf("压缩后字符串:%s\n",strDst);
    return 0;
}

/*
源字符:hhhhhhhhhCCCCCChhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhaaaaa中文
长度:75
压缩后长度:27
120 156 203 200 128 2 103 48 200 32 17 36 130 192 147 29 107 159 77 107 103 0 0 86 221 30 236 
解压后长度:75
压缩后字符串:hhhhhhhhhCCCCCChhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhaaaaa中文
*/
```

#### [1.2 z_stream](#)
如果缓冲区足够大（例如，如果对输入文件进行了map'ed操作），则压缩可以在单个步骤中完成，也可以通过重复调用压缩函数来完成。
在后一种情况下，应用程序必须在每次调用之前提供更多的输入和/或使用输出（提供更多的输出空间）。

z_stream 结构体定义如下:

```cpp
typedef voidpf (*alloc_func) OF((voidpf opaque, uInt items, uInt size));
typedef void   (*free_func)  OF((voidpf opaque, voidpf address));

struct internal_state;

typedef struct z_stream_s {
    z_const Bytef *next_in;  /* 下一个字节 */
    uInt     avail_in;  /* 下一个可用字节数字 */
    uLong    total_in;  /* 到目前为止读取的输入字节总数 */

    Bytef    *next_out; /* 下一个输出字节将转到此处 */
    uInt     avail_out; /* next_out 的 剩余可用空间 */
    uLong    total_out; /* 到目前为止输出的总字节数 */

    z_const char *msg;  /* 最后一条错误消息，如果没有错误，则为 NULL */
    struct internal_state FAR *state; /* 应用程序不可见 */

    alloc_func zalloc;  /* 用于分配内部状态 */
    free_func  zfree;   /* 用于释放内部状态 */
    voidpf     opaque;  /* 传递给 Zalloc 和 Zfree 的私有数据对象 */

    int     data_type;  /* 关于数据类型的最佳猜测：二进制或文本
                            用于放气，或用于充气的解码状态 */
    uLong   adler;      /* Adler-32 or CRC-32 value of the uncompressed data */
    uLong   reserved;   /* reserved for future use */
} z_stream;

typedef z_stream FAR *z_streamp;
```

传入和传出 zlib 例程的 gzip 标头信息。有关这些字段含义的更多详细信息，请参阅 RFC 1952。
```cpp
typedef struct gz_header_s {
    int     text;       /* true if compressed data believed to be text */
    uLong   time;       /* modification time */
    int     xflags;     /* extra flags (not used when writing a gzip file) */
    int     os;         /* operating system */
    Bytef   *extra;     /* pointer to extra field or Z_NULL if none */
    uInt    extra_len;  /* extra field length (valid if extra != Z_NULL) */
    uInt    extra_max;  /* space at extra (only when reading header) */
    Bytef   *name;      /* pointer to zero-terminated file name or Z_NULL */
    uInt    name_max;   /* space at name (only when reading header) */
    Bytef   *comment;   /* pointer to zero-terminated comment or Z_NULL */
    uInt    comm_max;   /* space at comment (only when reading header) */
    int     hcrc;       /* true if there was or will be a header crc */
    int     done;       /* true when done reading gzip header (not used
                           when writing a gzip file) */
} gz_header;

typedef gz_header FAR *gz_headerp;
```

**例子：**
```cpp
auto pStreamS = new z_stream();

deflateInit(pStreamS, Z_DEFAULT_COMPRESSION); //默认压缩算法

std::ifstream source_file("../temp.data",std::ios_base::in | std::ios_base::binary);
std::ofstream zip_file("../my_data.zip",std::ios_base::out | std::ios_base::binary | std::ios_base::trunc );

if (source_file.fail()){
    throw std::runtime_error("file error!");
}
const int block_size = 1024;
const int block_out_size = 2048;

char buffer[block_size] = {0};         //输入空间
char out_buffer[block_out_size] = {0}; //输出空间

auto read_len = source_file.readsome(buffer, block_size);
while(read_len > 0){

    auto total_out = pStreamS->total_out;
    std::cout<< "total in:" << read_len << std::endl;

    pStreamS->avail_in = read_len;      //读取的数量
    pStreamS->next_in = (Byte *)buffer; //unsigned char


    pStreamS->avail_out =block_size;      //压缩后数据存放空间大小
    pStreamS->next_out = (Byte*)out_buffer; //压缩后数据存放空间地址

    //执行压缩
    int result = deflate(pStreamS, Z_SYNC_FLUSH);
    if (result != Z_OK){
        std::cerr << "error in zlib deflate" << std::endl;
    }

    auto zip_data_len =  pStreamS->total_out - total_out;
    std::cout<< "total out:" << zip_data_len << std::endl;
    zip_file.write(out_buffer, (long)zip_data_len);

    read_len = source_file.readsome(buffer, block_size);
}
```

#### [1.3 数据头(header)](#)
zlib能使用一个gzip数据头，zlib数据头或者不使用数据头压缩数据。通常情况下，数据压缩使用zlib数据头，因为这提供错误数据检测。
当数据不使用数据头写入时，结果是没有任何错误检测的原始DEFLATE数据，那么解压缩软件的调用者知道压缩数据在什么地方结束。

gzip数据头比zlib数据头要大，因为它保存了文件名和其他文件系统信息，事实上这是广泛使用的gzip文件的数据头格式。
注意zlib函式库本身不能创建一个gzip文件，但是它相当轻松的通过把压缩数据写入到一个有gzip文件头的文件中。

#### [1.4 算法](#)
目前zlib仅支持一个LZ77的变种算法，DEFLATE的算法。这个算法使用很少的系统资源，对各种数据提供很好的压缩效果。
这也是在ZIP档案中无一例外的使用这个算法。（尽管zip文件格式也支持几种其他的算法）。

看起来zlib格式将不会被扩展使用任何其他算法，尽管数据头可以有这种可能性。