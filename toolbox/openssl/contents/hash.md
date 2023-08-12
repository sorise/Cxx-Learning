### [单向散列函数](#)

**介绍**：散列函数（Hash Function）是一种用于将数据映射为固定长度散列值（哈希值）的函数。散列函数的输入可以是任意长度的数据，而输出通常是一个固定长度的二进制字符串，常见的散列函数包括 MD5、SHA-1、SHA-256 等。



应用场景：

* 文件完整性证明
* 口令加密(口令/密码+随机数salt) 同密码不同hash。
* 消息认证，确保消息不被篡改
  * SSL 安全套接字通信
* 配合非对称加密实现数字签名
* 比特币工作量证明



### 1. SHA家族

SHA（Secure Hash Algorithm）是一系列密码学安全散列算法的缩写，由美国国家安全局（NSA）设计，目前已经发展出三代算法，SHA1、SHA-2、SHA-3。

以下是一些常见的 SHA 散列算法：

* SHA-1：SHA-1 是最早发布的 SHA 算法之一，生成 160 位（20 字节）的散列值。然而，由于存在碰撞攻击的安全漏洞，SHA-1 不再被推荐用于安全性要求较高的场景。

* SHA-2：SHA-2 算法家族包括 SHA-224、SHA-256、SHA-384 和 SHA-512，分别生成不同长度的散列值。SHA-256 和 SHA-512 是最常用的两种算法。

  - SHA-256：生成 256 位（32 字节）的散列值，安全性较高，广泛应用于数字签名、数据完整性校验等领域。

  - SHA-512：生成 512 位（64 字节）的散列值，提供更高的安全性和更大的输出空间。

* SHA-3：SHA-3 是最新的 SHA 算法，也称作 Keccak 算法。与 SHA-2 不同，SHA-3 提供了不同长度的散列值选项，包括 SHA-3-224、SHA-3-256、SHA-3-384 和 SHA-3-512。

这些 SHA 散列算法的安全性随着时间的推移而发展，一般认为 SHA-2 算法目前仍然是安全的选择。在实际应用中，应根据具体需求和安全性要求选择适合的散列算法。此外，还需要注意使用适当的盐（salt）和迭代次数来加强散列函数的安全性，以防止恶意攻击。



#### 1.1 算法列表

```c
const EVP_MD *EVP_sha1(void);
const EVP_MD *EVP_sha224(void);
const EVP_MD *EVP_sha256(void);
const EVP_MD *EVP_sha384(void);
const EVP_MD *EVP_sha512(void);
const EVP_MD *EVP_sha512_224(void);
const EVP_MD *EVP_sha512_256(void);
const EVP_MD *EVP_sha3_224(void);
const EVP_MD *EVP_sha3_256(void);
const EVP_MD *EVP_sha3_384(void);
const EVP_MD *EVP_sha3_512(void);
const EVP_MD *EVP_shake128(void);
const EVP_MD *EVP_shake256(void);
```

测试：

```cpp
void PrintHex(const string& str) {
	for (auto c : str) {
		std::cout << hex << (int)(unsigned char)c;
	}
	cout << endl;
}
 
void TestEVP(){
	unsigned char data[] = "测试数据：asduasdkashdiasndiaj54a5s4d8asda325s1d";
	auto ctx = EVP_MD_CTX_new();
    //算法使用
	auto evp_md = EVP_sha3_256();

	//hash初始化
	EVP_DigestInit_ex(ctx, evp_md, nullptr);

	//计算hash值
	EVP_DigestUpdate(ctx, data, strlen((char*)data));

	unsigned char out[32] = { 0 };
	unsigned int  out_size = 0;

	EVP_DigestFinal_ex(ctx, out, &out_size);

	PrintHex(string(out, out + 32));

	EVP_MD_CTX_free(ctx);
}
```



### 2. SM3 国密散列算法

SM3算法适用于商用密码应用中的数字签名和验证，是在SHA-256基础上改进实现的一种算法。SM3算法采用Merkle-Damgard结构，消息分组长度为512位，**摘要值长度为256位**。
SM3算法的压缩函数与SHA-256的压缩函数具有相似的结构,但是SM3算法的设计更加复杂,比如压缩函数的每一轮都使用2个消息字。

```c
const EVP_MD *EVP_sm3(void);
```

使用：

```cpp
void TestSM3() {
	unsigned char data[] = "测试数据：asduasdkashdiasndiaj54a5s4d8asda325s1d";
	auto ctx = EVP_MD_CTX_new();
	unsigned char out[32] = { 0 };
	unsigned int  out_size = 0;

	EVP_Digest(data, strlen((char*)data), out, &out_size, EVP_sm3(), NULL);

	cout << "国密SM3：";
	cout << "(" << out_size << ")";
	PrintHex(string(out, out + out_size));
}
```



### 3. MD算法

MD5是输入不定长度信息，输出固定长度128-bits的算法。经过程序流程，生成四个32位数据，最后联合起来成为一个128-bits[散列](https://zh.wikipedia.org/wiki/散列)。

返回值是16*8 = 128 位，16字节的数据。

强抗碰撞性已经被攻破，使用非常广泛！

```cpp
const EVP_MD *EVP_md5(void);
const EVP_MD *EVP_md5_sha1(void);
```

使用：

```cpp
void TestMD5() {
	unsigned char data[] = "测试数据：asduasdkashdiasndiaj54a5s4d8asda325s1d";
	auto ctx = EVP_MD_CTX_new();
	unsigned char out[32] = { 0 };
	unsigned int  out_size = 0;

	EVP_Digest(data, strlen((char*)data), out, &out_size, EVP_md5(), NULL);

	cout << "md5：";
	cout << "(" << out_size << ")";
	PrintHex(string(out, out + out_size));
}
```

3.0 之前接口,3.1版本已经不再支持。

```cpp
unsigned char data[] = "测试数据：asduasdkashdiasndiaj54a5s4d8asda325s1d";
unsigned char out[1024]{ 0 };

out[0] = 123;

int len = sizeof(data);

MD5_CTX c;
MD5_Init(&c);
MD5_Update(&c, data, len);

MD5_Final(out, &c);

std::string str(out, out + 16);

PrintHex(str);
```

