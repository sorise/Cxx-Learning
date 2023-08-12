### [OpenSSL BIO 接口](#)

**介绍**： openssl 抽象 IO(I/O abstraction，即BIO)是openssl对于io类型的抽象封装，包括：内存、文件、日志、标准输入输出、socket（TCP/UDP）、加/解密、摘要和ssl通道等。Openssl BIO通过回调函数为用户隐藏了底层实现细节，所有类型的bio的调用大体上是类似的。Bio中的数据能从一个BIO传送到另外一个BIO或者是应用程序。

------



### [1. OpenSSL 概述](#)

**OpenSSL 中的 BIO（Bi-directional I/O）接口** 是一个抽象的 I/O 抽象层，用于在 OpenSSL 应用程序中进行数据的输入和输出。它提供了一种统一的方式来处理不同类型的 I/O，包括文件、套接字、内存缓冲区等。包含了很多种接口，用通用的函数接口，主要控制在BIO_METHOD中的不同实现函数控制，包括 **6种filter型和8种source/sink型**。

**source/sink类型的BIO是数据源**，例如，sokect BIO和文件BIO。 而filter BIO就是把数据从一个BIO转换到另外一个BIO或应用接口，在转换过程中，这些数据可以不修改（如信息摘要BIO)，也可以进行转换。例如在加密BIO中，如果写操作，数据就会被加密，如果是读操作，数据就会被解密。



[**BIO是封装了许多类型I/O接口细节的一种应用接口**](#)，可以和**SSL连接**、非加密的**网络连接**以及**文件IO**进行透明的连接。

BIO可以连接在一起成为一个BIO链（单个的BIO就是一个环节的BIO链的特例），如下是BIO的结构定义，可以看到它**有上下环节**。

[一个BIO链通常包括一个source BIO和一个或多个filter BIO，数据从第一个BIO读出或写入,然后经过一系列BIO变化到输出（通常是一个source/sink BIO）。](#)

* bio.h：主定义的头文件，包括了很多通用的宏的定义。
* bio_lib.c主要的BIO操作定义文件，是比较上层的函数了。
* bss_*系列：是soruce/sink型BIO具体的操作实现文件_
* bf\_*系列：是filter型BIO具体的操作实现文件
* bio_err.c:是错误信息处理文件
* bio_cb.c：是callback函数的相关文件
* b_print.c: 是信息输出的处理函数文件
* b_socket.c: 是Socket连接的一些相关信息处理文件
* b_dump.c: 是对内存内容的存储操作处理
  

BIO 接口的主要目的是为了将 OpenSSL 库与特定的 I/O 设备解耦，使得应用程序可以方便地切换不同的 I/O 设备，而无需修改底层代码。下面是一些常见的 BIO 函数：

1. 创建和销毁 BIO 对象：

   - `BIO_new()`：创建一个新的 BIO 对象。
   - `BIO_free()`：销毁一个 BIO 对象。

2. 连接和断开 BIO 对象：

   - `BIO_push()`：将一个 BIO 对象连接到另一个 BIO 对象。
   - `BIO_pop()`：断开 BIO 对象之间的连接关系。

3. 设置和获取 BIO 的属性：

   - `BIO_ctrl()`：设置或获取 BIO 的属性。
   - `BIO_gets()` 和 `BIO_puts()`：读取和写入字符串数据。

4. 从 BIO 中读取和写入数据：

   - `BIO_read()` 和 `BIO_write()`：从 BIO 中读取或写入二进制数据。
   - `BIO_read_ex()` 和 `BIO_write_ex()`：带有错误处理的 BIO 读取和写入。

5. 使用特定类型的 BIO：

   - `BIO_new_file()`：创建一个与文件关联的 BIO 对象。
   - `BIO_new_socket()`：创建一个与套接字关联的 BIO 对象。
   - `BIO_s_mem()`：创建一个与内存缓冲区关联的 BIO 对象。

这只是 BIO 接口的一小部分功能，它提供了许多其他操作和选项，以适应各种 I/O 场景。建议在使用 BIO 接口时参考 OpenSSL 的官方文档或相关的示例代码，以了解更详细的用法和配置选项。



#### 1.1 BIO 结构体

在BIO的所用成员中，**method**可以说是最关键的一个成员，它决定了BIO的类型，可以看到，在定义一个新的BIO结构时，总是使用下面的函数：

```c
BIO* BIO_new(BIO_METHOD *type);
```

BIO定义如下所示：

```c
typedef struct bio_st BIO;

struct bio_st
{
     BIO_METHOD *method;    //BIO方法结构，是决定BIO类型和行为的重要参数，各种BIO的不同之处主要也正在于此项。

     long (*callback)(struct bio_st *,int,const char *,int, long,long);     //BIO回调函数
     char *cb_arg;                                                          //回调函数的第一个参量

     int init;           //初始化标志，初始化了为1，否则为0。比如文件BIO 中，通过BIO_set_fp
                         //关联一个文件指针时，该标记则置1。
     int shutdown;       //BIO开关标志，如果为BIO_CLOSE，则释放BIO时自动释放持有的资源，否则不自动释放持有资源
     int flags;          //有些BIO 实现需要它来控制各个函数的行为。比如文件BIO 默认该值为BIO_FLAGS_UPLINK，
                         //这时文件读操作调用UP_fread 函数而不是调用fread 函数。
     int retry_reason;  //重试原因，主要用在socket 和ssl BIO 的异步阻塞。比如socketbio 中，遇到 
                        //WSAEWOULDBLOCK 错误时，openssl 告诉用户的操作需要重试
     int num;           //该值因具体BIO 而异，比如socket BIO 中num 用来存放链接字。
     void *ptr;         //ptr：指针，具体bio 有不同含义。比如文件BIO中它用来存放文件句柄；mem bio 中它用来存放
                        //内存地址；connect bio 中它用来存放BIO_CONNECT 数据，acceptbio 中它
                        //用来存放BIO_ACCEPT数据。

     struct bio_st *next_bio;    //BIO链中下一个BIO 地址，BIO 数据可以从一个BIO 传送到另一个BIO。
     struct bio_st *prev_bio;    //BIO链中上一个BIO 地址，

     int references;                     //引用计数
     unsigned long num_read;             //已读出的数据长度
     unsigned long num_write;            //已写入的数据长度

     CRYPTO_EX_DATA ex_data;             //额外数据
};
```

在源代码可以看出，**BIO_new** 函数除了给一些初始变量赋值外，**主要就是把type中的各个变量赋值给BIO结构中的method成员**。 一般来说，上述type参数是以一个返回值为BIO_METHOD类型的函数提供的，如生成一个mem型的BIO结构，就使用下面的语句：

```c
BIO *mem = BIO_new(BIO_s_mem());
```

这样的函数有以下一些：

```c
//【source/sink型 BIO】
BIO_METHOD* BIO_s_accept()   //一个封装了类似TCP/IP socket Accept规则的接口，并且使TCP/IP操作对于BIO接口透明。
BIO_METHOD* BIO_s_connect()  //一个封装了类似TCP/IP socket Connect规则的接口，并且使TCP/IP操作对于BIO接口透明
BIO_METHOD* BIO_s_socket()   //封装了socket接口的BIO类型

BIO_METHOD* BIO_s_bio()      //封装了一个BIO对，数据从其中一个BIO写入，从另外一个BIO读出

BIO_METHOD* BIO_s_fd()       //是一个封装了文件描述符的BIO接口，提供类似文件读写操作的功能
BIO_METHOD* BIO_s_file()     //封装了标准的文件接口的BIO，包括标准的输入输出设备如stdin等
BIO_METHOD* BIO_s_mem()      //封装了内存操作的BIO接口，包括了对内存的读写操作
BIO_METHOD* BIO_s_null()     //返回空的sink型BIO接口，写入这种接口的所有数据读被丢弃，读的时候总是返回EOF

//【filter型 BIO】
BIO_METHOD* BIO_f_base64()   //封装了base64编码方法的BIO,写的时候进行编码，读的时候解码                                                         
BIO_METHOD* BIO_f_cipher()   //封装了加解密方法的BIO，写的时候加密，读的时候解密
BIO_METHOD* BIO_f_md()       //封装了信息摘要方法的BIO，通过该接口读写的数据都是已经经过摘要的。
BIO_METHOD* BIO_f_ssl()      //封装了openssl 的SSL协议的BIO类型，也就是为SSL协议增加了一些BIO操作方法。

BIO_METHOD* BIO_f_null()     //一个不作任何事情的BIO，对它的操作都简单传到下一个BIO去了，相当于不存在。

BIO_METHOD* BIO_f_buffer()   //封装了缓冲区操作的BIO，写入该接口的数据一般是准备传入下一个BIO接口的，从该——
                             //接口读出的数据一般也是从另一个BIO传过来的。
```

上述各种类型的函数正是构成BIO强大功能的基本单元，所以，**要了解BIO的各种结构和功能，也就应该了解这些函数类型相关的操作函数**。

所有这些源文件，都基本上包含于 `/crypto/bio/` 目录下的同名.c文件（大部分是同名的）中。

#### 1.2 BIO_METHOD

在BIO_METHOD里面，定义了一组行为函数，上述不同类型的 **BIO_METHOD** 行为函数的定义是不同的，其结构如下（以非16位系统为例）：

```c
typedef struct bio_method_st
{
     int type;                                        //具体BIO 类型
     const char *name;                                //具体BIO 的名字；

     int (*bwrite)(BIO *, const char *, int);         //具体BIO 二进制写操作回调函数
     int (*bread)(BIO *, char *, int);                //具体BIO 二进制写操作回调函数            
     int (*bputs)(BIO *, const char *);               //具体BIO 中文本写回调函数；
     int (*bgets)(BIO *, char *, int);                //具体BIO 中文本读回调函数；

     long (*ctrl)(BIO *, int, long, void *);          //具体BIO 的控制回调函数；  

     int (*create)(BIO *);                            //生成具体BIO 回调函数；
     int (*destroy)(BIO *);                           //销毁具体BIO 回调函数；  

     long (*callback_ctrl)(BIO *, int, bio_info_cb *);//具体BIO 控制回调函数，与ctrl 回调函数不一样，
                                                      //该函数可由调用者（而不是实现者）来实现，然后
                                                      //通过BIO_set_callback 等函数来设置。
} BIO_METHOD;

//type 的取值
#define BIO_TYPE_NONE 0
#define BIO_TYPE_MEM (1|0x0400)

#define BIO_TYPE_FILE (2|0x0400)

#define BIO_TYPE_FD (4|0x0400|0x0100)
#define BIO_TYPE_SOCKET (5|0x0400|0x0100)
#define BIO_TYPE_NULL (6|0x0400)
#define BIO_TYPE_SSL (7|0x0200)
#define BIO_TYPE_MD (8|0x0200)
#define BIO_TYPE_BUFFER (9|0x0200)
#define BIO_TYPE_CIPHER (10|0x0200)
#define BIO_TYPE_BASE64 (11|0x0200)
#define BIO_TYPE_CONNECT (12|0x0400|0x0100)
#define BIO_TYPE_ACCEPT (13|0x0400|0x0100)
#define BIO_TYPE_PROXY_CLIENT (14|0x0200)
#define BIO_TYPE_PROXY_SERVER (15|0x0200)
#define BIO_TYPE_NBIO_TEST (16|0x0200)
#define BIO_TYPE_NULL_FILTER (17|0x0200)
#define BIO_TYPE_BER (18|0x0200)
#define BIO_TYPE_BIO (19|0x0400)

#define BIO_TYPE_DESCRIPTOR 0x0100
#define BIO_TYPE_FILTER 0x0200
#define BIO_TYPE_SOURCE_SINK 0x0400
```

在BIO的成员中，**callback** 也是比较重要的，它能够用于程序调试用或者自定义改变BIO的行为。详细会在以后相关的部分介绍。
BIO的很多操作，都是 **BIO_ctrl** 系列函数根据不通参数组成的宏定义来完成的。所以要了解BIO的行为，了解 **BIO_ctrl** 系列函数以及其各个参数的意义也是很重要的。

### [2. BIO的声明和释放等基本操作](#)

在BIO的基本操作系列函数中，用来BIO分配和释放操作，，其声明形式如下：

```c
 #include<openssl/bio.h>

 BIO*  BIO_new(BIO_METHOD *type);
 int   BIO_set(BIO *a,BIO_METHOD *type);
 int   BIO_free(BIO *a);
 void  BIO_vfree(BIO *a);
 void  BIO_free_all(BIO *a);
```

#### 2.1 BIO_new

这个函数**创建并返回一个相应的新的BIO**，并根据给定的BIO_METHOD类型调用BIO_set()函数给BIO结构的method成员赋值，如果创建或给method赋值**失败，则返回NULL**。

创建一个Memory类型的BIO例子如下：

```c
BIO* mem = BIO_new(BIO_s_mem());
```

有些类型的BIO使用BIO_new()函数之后就可以直接使用了，如memory类型的BIO; 而有些BIO创建之后还需要一些初始化工作，如文件BIO,一般来说，也提供了这样的一些函数来创建和初始化这种类型的BIO。

```c
BIO* in = NULL;
in = BIO_new(BIO_s_file());
BIO_read_filename(in,"rsa512.pem");
```



#### 2.2 BIO_set

该函数功能比较简单，**就是对一个已经存在的BIO 设置新的BIO_METHOD类型**。

其实就是简单的对BIO的各个成员进行初始化，并将参数type赋值给该BIO。

其实，BIO_new函数在使用OPENSSL_malloc给BIO分配了内存之后，就简单调用了BIO_set函数进行初始化工作。所以一般来说，除非你要重新设置你已经存在的BIO，否则是不需要直接调用这个函数的。

**成功操作返回1，否则返回0**。

```c
int   BIO_set(BIO *a,BIO_METHOD *type);
```



#### 2.3 BIO_free

该函数释放单个BIO的内存和资源，成功操作返回1，失败返回0。

**BIO的操作不仅仅是释放BIO结构所占用的资源，也会释放其下层的I/O资源，比如关闭释放相关的文件符等，这对不同类型的BIO是不一样的，详细的请参看各种类型BIO本身的说明文件和源文件**。

**需要注意的是**，BIO_free只释放当前的一个BIO，如果用来释放一个BIO链，就可能会导致内存泄漏，这种情况应该使用下述的BIO_free_all函数。



#### 2.4 BIO_vfree

该**函数功能与BIO_free完全相同，只是没有返回值。**事实上，它简单调用了BIO_free函数，但不返回该函数的返回值，所以它的函数实现代码只有一个语句。



#### 2.5 BIO_free_all

**该函数释放这个BIO链，并且即使在这个过程中，如果释放其中一个BIO出错，释放过程也不会停止，会继续释放下面的BIO，这保证了尽量避免内存泄漏的出现**。

如果你非要调用这个函数释放单个的BIO，那么效果跟BIO_free是一样的。事实上，该函数只是简单的遍历整个BIO链，并调用BIO_free释放各个环节的BIO。


### 3. BIO的通用控制函数

其实，在这些函数中，除了BIO_ctrl, BIO_callback_ctrl, BIO_ptr_ctrl, BIO_int_ctrl, BIO_ctrl_pending, BIO_ctrl_wpending是真正的函数外，其它都是宏定义，而且，在这些函数中，除了BIO_ctrl,BIO_callback_ctrl，其它基本上都是简单的BIO_ctrl输入不同的参数的调用。


```c
#include <openssl/bio.h>
//最底层BIO控制函数，理解原理
long   BIO_ctrl(BIO *bp,int cmd,long larg,void *parg); 

//上层BIO控制函数
char*  BIO_ptr_ctrl(BIO *bp,int cmd,long larg);
long   BIO_int_ctrl(BIO *bp,int cmd,long larg,int iarg);

//最上层BIO控制函数,需要掌握！！！
int    BIO_reset(BIO *b);
int    BIO_seek(BIO *b, int ofs);
int    BIO_tell(BIO *b);
int    BIO_flush(BIO *b);
int    BIO_eof(BIO *b);
int    BIO_set_close(BIO *b,long flag);
int    BIO_get_close(BIO *b);
int    BIO_pending(BIO *b);
int    BIO_wpending(BIO *b);
size_t BIO_ctrl_pending(BIO *b);
size_t BIO_ctrl_wpending(BIO *b);  

//最上层BIO控制函数的宏定义方式
#define BIO_reset(b)       (int)BIO_ctrl(b,BIO_CTRL_RESET,0,NULL)
#define BIO_seek(b,ofs)    (int)BIO_ctrl(b,BIO_C_FILE_SEEK,ofs,NULL)
#define BIO_tell(b)        (int)BIO_ctrl(b,BIO_C_FILE_TELL,0,NULL)
#define BIO_flush(b)       (int)BIO_ctrl(b,BIO_CTRL_FLUSH,0,NULL)
#define BIO_eof(b)         (int)BIO_ctrl(b,BIO_CTRL_EOF,0,NULL)
#define BIO_set_close(b,c) (int)BIO_ctrl(b,BIO_CTRL_SET_CLOSE,(c),NULL)
#define BIO_get_close(b)   (int)BIO_ctrl(b,BIO_CTRL_GET_CLOSE,0,NULL)
#define BIO_pending(b)     (int)BIO_ctrl(b,BIO_CTRL_PENDING,0,NULL)
#define BIO_wpending(b)    (int)BIO_ctrl(b,BIO_CTRL_WPENDING,0,NULL)

//最底层BIO回调函数控制函数，理解原理
long   BIO_callback_ctrl(BIO *b, int cmd, 
                               void (*fp)(struct bio_st *, int, const char *, int, long, long));
//上层BIO回调函数控制函数，需要掌握！！！
#define BIO_set_callback(b,cb)      ((b)->callback=(cb))
#define BIO_get_callback(b)         ((b)->callback)
#define BIO_set_callback_arg(b,arg) ((b)->cb_arg=(char *)(arg))
#define BIO_get_callback_arg(b)     ((b)->cb_arg)

//其中，callback函数本身的声明在BIO结构体中
typedef long callback(BIO *b, int oper, const char *argp,int argi, long argl, long retvalue);

//此外，还有一个用于调试目的的函数，其实声明如下：
long BIO_debug_callback(BIO *bio,int cmd,const char *argp,int argi,long argl,long ret);
```



#### [3.1 BIO_ctrl](#)

从上面的叙述可以知道，**BIO_ctrl是整个控制函数中最基本的函数**，它支持不同的命令输入，从而产生不同的功能，由此，它也就衍生了许多其它函数，作为一个比较底层的控制函数，**一般来说用户并不需要直接调用它**，因为在它之上已经使用宏定义和函数调用的形式建造了许多直接面向用户的函数。

**filter型的BIO没有定义BIO_ctrl功能**，如果对他们调用这个函数，他们就简单的把命令传到BIO链中的下一个BIO。

也就是说，通常可以不用直接调用一个BIO的BIO_ctrl函数，只需要在它所在的BIO链上调用该函数，那么BIO链就会自动将该调用函数传到相应的BIO上去。这样可能就会导致一些意想不到的结果，比如，在目前的filter型BIO中没有实现BIO_seek()函数（大家待会就会明白BIO_seek就是BIO_ctrl的简单宏定义），但如果在这个BIO链上的末尾是一个文件或文件描述符型BIO，那么这个调用也会返回成功的结果。

对于source/sink型BIO来说，如果他们不认得BIO_ctrl所定义的操作，那么就返回08。



#### [3.2 BIO_callback_ctrl](#)

**这个函数是这组控制函数中唯一一个不是通过调用BIO_ctrl建立起来的，它有自己的实现函数，而且跟BIO_ctrl毫不相干**。

跟BIO_ctrl一样，它也是比较底层的控制函数，在它上面也定义了一些直接面向用户的控制函数，一般来说，用户不需要直接调用该函数。

需要说明的是，该函数和BIO_ctrl函数为了实现不同类型BIO具有不同的BIO_ctrl控制功能，他们的操作基本上都是由各个BIO的callback函数来定义的。这是不同的BIO能灵活实现不同功能的根本所在。


#### [3.3 BIO_ptr_ctrl、BIO_int_ctrl](#)

这两个函数都是简单的调用了BIO_ctrl函数，不同的是，后者是输入了四个参数并传入到BIO_ctrl函数中，简单**返回了调用BIO_ctrl返回的返回值**；而前者只输入了三个参数，最后一个**BIO_ctrl参数是作为输出参数并作为返回值**的。



#### 3.4 BIO_reset

宏定义函数，BIO_reset函数只是简单的**将BIO的状态设回到初始化的时候的状态**，比如文件BIO，调用该函数就是将文件指针指向文件开始位置。

一般来说，调用**成功**的时候该函数**返回1**，**失败**的时候**返回0或－1**；但是**文件BIO**是一个例外，**成功**调用的时候**返回0**，**失败**的时候**返回－1**。