### [对称加密技术](#)

**介绍**：**对称加密是一种加密方法，使用相同的密钥（也称为对称密钥）进行加密和解密**。在对称加密中，原始数据被称为明文，通过使用密钥和特定的算法，将明文转换为加密后的密文。而接收方可以使用相同的密钥和算法对密文进行解密，还原为原始的明文。



### 1. 概述

对称加密的特点如下：

1. 使用相同的密钥：加密和解密过程使用相同的密钥，这要求密钥在发送和接收之间得到安全地共享。
2. 快速高效：对称加密算法通常运算速度快，适合处理大量的数据。
3. 加密强度依赖于密钥长度：对称加密的安全性依赖于密钥的保密性和长度。密钥越长，破解难度越大。

常见的对称加密算法包括：

- DES（Data Encryption Standard）：数据加密标准，使用56位密钥，已经不再被广泛使用，因为其密钥长度较短，安全性较弱。
- AES（Advanced Encryption Standard）：高级加密标准，使用128、192 或 256 位密钥，是目前广泛应用的对称加密算法之一。AES 是一种块加密算法，将明文分割为一定大小的块，并对每个块进行独立的加密操作。
- **SM4**：国密，可使用软件实现
- 3DES（Triple Data Encryption Standard）：三重数据加密标准，是 DES 的改进版本，通过多次应用 DES 算法提高安全性。

对称加密广泛应用于文件加密、网络通信、数据传输等场景。然而，相对于非对称加密算法，对称加密在密钥分发和管理上存在一些挑战，因此在某些情况下，非对称加密算法会更加适合。



#### 1.1 实现方式

主要有两种实现方式，分组密码和流密码。



**流密码（Stream Cipher）是一种对称加密算法**，它以连续的流（或者称为比特流）作为输入，并产生相应的加密流作为输出。流密码与分组密码不同，**它逐个比特地加密数据**，而不是将数据分割为固定大小的块。

在流密码中，加密和解密使用相同的密钥和算法。加密过程中，明文中的每个比特都与密钥流中的对应比特进行异或运算，生成相应的密文比特。解密时，密文比特再次与相同的密钥流进行异或运算得到原始明文比特。

流密码具有以下特点：

1. 速度高效：由于逐个比特地处理数据，流密码通常可以提供较快的加密和解密速度。
2. 适用于实时通信：流密码对于需要即时加密和解密的场景非常适用，如实时音视频通信、流媒体传输等。
3. 密钥流生成方法多样：流密码可以使用伪随机数生成器（PRNG）生成密钥流，也可以使用硬件设备（如随机数发生器）生成真随机密钥流。

常见的流密码算法包括：

1. RC4（Rivest Cipher 4）：RC4 是一种广泛使用的流密码算法，但由于其已经被发现存在一些安全性漏洞，在实际应用中不再推荐使用。
2. Salsa20：Salsa20 是一种流密码算法，具有良好的安全性和高效的性能，被广泛应用于加密通信和数据保护领域。

值得注意的是，流密码相对于分组密码来说，在一些特定的情况下可能会更容易受到攻击。因此，在选择加密算法时，需要根据具体的需求和安全要求进行综合考虑。



**分组加密（Block Cipher）是一种对称加密算法**，在对数据进行加密和解密时将数据划分为固定大小的数据块进行处理。每个数据块的大小通常为 64 比特（如 DES 算法）或 128 比特（如 AES 算法）。有一个初始化向量。

在分组加密中，加密和解密使用相同的密钥和算法。加密时，明文数据块通过加密算法与密钥进行多轮的运算，生成对应的密文数据块。解密过程中，密文数据块与相同的密钥进行逆向运算，得到原始的明文数据块。

分组加密具有以下特点：

1. 可逆性：分组加密是可逆的，可以通过解密算法将密文还原为原始的明文。
2. 块独立性：每个数据块是独立加密的，即不同的数据块之间没有直接关联。
3. 密钥长度固定：分组加密算法所使用的密钥长度是固定的，通常为 128 比特、192 比特或 256 比特。

常见的分组加密算法包括：

1. DES（Data Encryption Standard）：DES 是一种早期的分组加密算法，使用 64 比特的数据块和 56 比特的密钥长度。由于密钥长度较短，DES 的安全性逐渐被 AES 算法所取代。
2. AES（Advanced Encryption Standard）：AES 是一种广泛使用的分组加密算法，使用 128 比特的数据块和密钥长度可为 128、192 或 256 比特。AES 具有较高的安全性和性能，在许多安全通信和数据保护场景中得到广泛应用。

需要注意的是，分组加密只是对数据进行块独立的加密处理，而不提供完整的消息完整性与机密性保护。在实际应用中，通常需要将分组加密算法与其他密码学技术（如消息认证码、数字签名等）结合使用，以提供更全面的数据保护方案。



#### 1.2 分组密码的 ECB 和 CBC模式

分组密码的 ECB（Electronic Codebook）模式和 CBC（Cipher Block Chaining）模式是两种常见的加密模式，用于对数据进行分组加密处理。

* **ECB 模式**：ECB 模式是最简单的分组加密模式之一。在 ECB 模式中，明文数据被划分为固定大小的数据块，每个数据块都独立地使用相同的密钥进行加密，生成对应的密文数据块。这意味着同样的明文块会被加密成相同的密文块。

ECB 模式的特点是简单和高效，适用于某些特定的场景。然而，ECB 模式也存在一些安全性问题。由于相同的明文块加密后会得到相同的密文块，因此可能存在信息泄露的问题。此外，ECB 模式对于明文数据的模式敏感，因此在某些情况下，可能会导致一些隐私问题。

* **CBC 模式**：CBC 模式在进行分组加密时引入了初始化向量（Initialization Vector，IV）的概念。在 CBC 模式中，每个明文数据块与前一个密文数据块进行异或运算，然后再使用密钥进行加密。第一个块的前一个密文块由初始化向量来代替。

CBC 模式的特点是具有一定的安全性。由于每个数据块都与前一个密文块有关，因此即使相同的明文块在不同的位置出现，加密后得到的密文块也是不同的。这样可以提高信息的保密性。

然而，CBC 模式也有一些问题。首先，由于各个数据块之间有依赖关系，导致无法并行处理。其次，在传输过程中，如果数据块丢失或者被篡改，可能会导致后续数据块的解密出错。

需要注意的是，为了进一步增强安全性和保护数据完整性，通常会在分组加密的基础上结合使用消息认证码（MAC）或者添加额外的认证步骤，如使用 HMAC 或者 GCM 等模式来提供更全面的数据保护方案。





### 2. DES

DES（Data Encryption Standard）是一种对称密钥分组加密算法，由美国国家标准局（NIST）在上世纪70年代初期开发并发布。DES 算法使用一个 56 位的密钥对数据进行加密和解密，将明文数据分为 64 位的数据块，并对每个数据块进行加密处理。

DES 算法的主要步骤如下：

1. 初始置换（Initial Permutation，IP）：将输入的 64 位数据块按照固定的规则进行置换。
2. 密钥置换（Key Schedule）：根据输入的 56 位密钥生成 16 个子密钥，用于后续加密过程中的轮函数运算。
3. 分组加密：将输入的 64 位数据块拆分为左右两部分，经过 16 轮的迭代运算，每轮包括以下步骤：
   - 扩展置换（Expansion Permutation，E-Box）：将右半部分的 32 位数据扩展为 48 位。
   - 轮函数运算（Round Function）：对扩展后的数据与子密钥进行异或运算、替代置换（Substitution Boxes，S-Boxes）和置换运算。
   - 左右交换：交换左半部分和右半部分的位置。
4. 逆初始置换（Inverse Initial Permutation，IP^-1）：将最后一轮迭代后的数据块按照逆向的置换规则进行处理。

**DES 算法的安全性在现代密码学中已经不再足够，因为其密钥长度相对较短**。然而，DES 算法为其他更强大的分组密码算法的发展提供了基础，如 AES（Advanced Encryption Standard）。AES 取代了 DES，并成为当前广泛使用的对称密钥分组加密算法。

值得注意的是，由于 DES 算法的历史原因和制约性，其使用受到一些限制。在实际应用中，通常建议使用更安全和强健的加密算法，如 AES，以保护数据的安全性。

```cpp
typedef struct DES_ks {
    union {
        DES_cblock cblock;
        /*
         * make sure things are correct size on machines with 8 byte longs
         */
        DES_LONG deslong[2];
    } ks[16];
} DES_key_schedule;

int DES_set_key(const_DES_cblock *key, DES_key_schedule *schedule);

void DES_ecb_encrypt(
    const_DES_cblock *input,  //每次只能处理八个字节
    DES_cblock *output,                
    DES_key_schedule *ks, 
    int enc
);
//enc 的取值
// DES_ENCRYPT  加密
// DES_DECRYPT  解密

void DES_cbc_encrypt(
	 const unsigned char *input, 
     unsigned char *output, 
     long length, 
     DES_key_schedule *schedule, 
     DES_cblock *ivec, 
     int enc
);
```

**DES_ecb_encrypt** 函数是用于在 ECB 模式下进行 DES 分组加密的函数。它的参数包括：

1. `const unsigned char *input`：指向要加密的数据块的输入缓冲区的指针。输入数据块的长度通常为 64 位（8 字节）。
2. `unsigned char *output`：指向输出缓冲区的指针，用于存储加密后的数据块。输出缓冲区的长度应足够容纳一个完整的数据块。
3. `const_des_key_schedule *ks`：指向 DES 密钥调度表（key schedule）的指针。密钥调度表是根据加密密钥生成的一种数据结构，用于加密过程中的轮函数运算。
4. `const int enc`：指定加密或解密操作。设置为 1 表示加密操作，设置为 0 表示解密操作。

`DES_cbc_encrypt()` 是 OpenSSL 中用于进行 DES 加密的函数，它的参数如下：

```
cCopy Codevoid DES_cbc_encrypt(const unsigned char *input, unsigned char *output, long length,
                     const DES_key_schedule *schedule, unsigned char *ivec, int enc);
```

**DES_cbc_encrypt** 参数说明如下：

1. `input`：指向要加密的输入数据的指针。
2. `output`：指向加密后的输出数据的指针。
3. `length`：要加密的数据长度（以字节为单位）。
4. `schedule`：DES 密钥排程（key schedule），即通过 `DES_set_key()` 函数生成的密钥排程。
5. `ivec`：初始向量（Initialization Vector，IV），用于 CBC（Cipher Block Chaining）模式的加密操作。需要在加密之前将其设置为一个 8 字节（64 位）的初始值，并在每次加密操作后更新该值，以便用于下一个数据块的加密。
6. `enc`：加密方向的标志位。如果为非零值，则进行加密操作；如果为零值，则进行解密操作。

`DES_cbc_encrypt()` 函数将根据指定的密钥排程和初始向量对输入数据进行 CBC 模式的 DES 加密（或解密）操作，并将结果存储在输出缓冲区中。加密过程是按照指定的密钥排程和初始向量逐个数据块（8 字节）进行加密，直到达到指定的数据长度（length）。解密过程同样逐个数据块进行操作，输出结果为解密后的数据。

需要注意的是，在使用 `DES_cbc_encrypt()` 函数之前，需要通过 `DES_set_key()` 函数生成密钥排程，并将初始向量（IV）设置为一个合适的值。初始向量在 CBC 模式中起到重要的作用，它必须与加密时使用的初始向量一致，否则可能导致解密结果错误。

此外，为了确保加密操作的安全和正确性，建议使用更安全的加密算法，如 AES，而不是 DES。DES 已经被广泛认为不再具备足够的安全性。



```cpp
uint8_t data[1024] = "ceshi des data544ld8a6a56ds4a95sd498as4d5as4d98as4d98as4d654as98d4sa5";
int data_size = strlen((char*)data);

unsigned char out[1024] = { 0 };
unsigned char out2[1024] = { 0 };

const_DES_cblock key {'0','1','2','3','4','5','6','7' };//8个字符
DES_key_schedule key_sch;
//设置秘钥
DES_set_key(&key, &key_sch);
//ecb
DES_ecb_encrypt((const_DES_cblock*)data, (DES_cblock*)out, &key_sch, DES_ENCRYPT);

cout << out << endl;

//数据解密
DES_ecb_encrypt((const_DES_cblock*)out, (DES_cblock*)out2, &key_sch, DES_DECRYPT);
cout << out2 << endl;
```



#### ECB 封装

如果补0了 解密后无法知道实际大小，需要用户存储原数据大小。

```cpp
std::string DES_ECB_Encode(uint8_t* data, int len, const_DES_cblock* key) {
	//8个字节一组加密
	DES_key_schedule key_sch;
	size_t result_size = (len % 8 == 0) ? len : (len + (8 - len % 8));//9 = 16
	std::string result(result_size,'\0');
	//设置秘钥
	DES_set_key(key, &key_sch);
	//循环加密然后输出
	int i = 0;
	while (i < len) {
		const_DES_cblock _data{ 0 };
		auto data_block_size = (len - i) >= 8 ? 8 : len - i;
		std::memcpy(_data, data + i,data_block_size);

		DES_ecb_encrypt(&_data, (DES_cblock*)&result[i], &key_sch, DES_ENCRYPT);
		i += 8;
	}
	return result;
}

//如果补0了 解密后无法知道实际大小，需要用户存储原数据大小
std::string DES_ECB_Decode(const uint8_t* data, size_t data_size , int len, const_DES_cblock* key) {
	DES_key_schedule key_sch;
	if (data_size % 8 != 0) return std::string();

	std::string result(len, '\0');
	
	//设置秘钥
	DES_set_key(key, &key_sch);
	//循环加密然后输出
	int i = 0;
	while (i < len) {
		DES_ecb_encrypt((const_DES_cblock*)(data + i), (DES_cblock*)&result[i], &key_sch, DES_DECRYPT);
		i += 8;
	}
	//删除填充部分
	result.erase(result.begin() + len, result.end());
	return result;
}
```

使用方法：

```cpp
const_DES_cblock key{ '0','1','2','3','4','5','6','7' };//8个字符

uint8_t data[1024] = "ceshdasdasadsiasdasddeasddata544ldasdas8a6a56ds4a95sd498as4d5as4d98as4d98as4d654as98d4sa5";

auto str = DES_ECB_Encode(data, strlen((char*)data), &key);

std::cout << str << std::endl;

auto de_str = DES_ECB_Decode((const unsigned char*)str.c_str(), str.size(), strlen((char*)data), &key);

std::cout << de_str << std::endl;
```



#### CBC 封装

如果补0了 解密后无法知道实际大小，需要用户存储原数据大小。

```cpp
std::string DES_CBC_Encode(uint8_t* data, int len, const_DES_cblock* key) {
	//8个字节一组加密
	DES_key_schedule key_sch;
	size_t result_size = (len % 8 == 0) ? len : (len + (8 - len % 8));//9 = 16
	//设置秘钥
	DES_set_key(key, &key_sch);
	DES_cblock iv = { 0 }; //初始化向量
	std::string result(result_size, '\0');
	//如果数据不是8的倍数  会自动补0
	DES_cbc_encrypt(data, (unsigned char*)&result[0], len, &key_sch, &iv, DES_ENCRYPT);
	return result;
}
```

解密

```cpp
//len 是原数据大小
std::string DES_CBC_Decode(const uint8_t* data, size_t data_size, int len, const_DES_cblock* key) {
	//8个字节一组加密
	DES_key_schedule key_sch;
	//设置秘钥
	DES_set_key(key, &key_sch);
	DES_cblock iv = { 0 }; //初始化向量
	std::string result(data_size, '\0');
	//如果数据不是8的倍数  会自动补0
	DES_cbc_encrypt(data, (unsigned char*)&result[0], len, &key_sch, &iv, DES_DECRYPT);
	result.erase(result.begin() + len, result.end()); //需不要删除冗余数据
	return result;
}
```

测试使用：

```cpp
const_DES_cblock key{ '0','1','2','3','4','5','6','7' };//8个字符

uint8_t data[1024] = "ceshdasdasadsiasdasddeasddata544ldasdas8a6a56ds4a95sd498as4d5as4d98as4d98as4d654as98d4sa5";

auto str = DES_CBC_Encode(data, strlen((char*)data), &key);

std::cout << str << std::endl;

auto de_str = DES_CBC_Decode((const unsigned char*)str.c_str(), str.size(), strlen((char*)data), &key);
```

