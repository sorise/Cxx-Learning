### [AES 算法](#)

**介绍** : AES（Advanced Encryption Standard）是一种对称加密算法，也被称为 Rijndael 算法。它是目前应用最广泛的加密算法之一，被用于保护敏感数据的机密性。

-----

AES 算法使用相同的密钥进行加密和解密操作，密钥长度可以是 **128** 比特、**192** 比特或 **256** 比特。它将明文块分成固定长度的字节块，并对其中的每个字节进行一系列的变换和替换操作，通过多轮迭代来实现加密和解密过程。

属于分组秘钥：

* AES-128
* AES-192
* AES-256



```cpp
const EVP_CIPHER *EVP_aes_192_ecb(void);
const EVP_CIPHER *EVP_aes_192_cbc(void);
const EVP_CIPHER *EVP_aes_256_ecb(void);
const EVP_CIPHER *EVP_aes_256_cbc(void);
const EVP_CIPHER *EVP_aes_128_ecb(void);
const EVP_CIPHER *EVP_aes_128_cbc(void);
```





#### 1. EVP_aes_192_cbc

需要初始化向量模式

```cpp
#pragma warning(disable : 4996)
#include <iostream>
#include "base.h"
#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include<openssl/evp.h>

using namespace std;

void PrintHex(const string& str) {
	for (auto c : str) {
		std::cout << hex << (int)(unsigned char)c;
	}
	cout << endl;
}


int main()
{
	const unsigned char data[] = { "012345670123456701234567sdasd" };
	unsigned char out[1024] = { 0 }; //输出

	auto cipher = EVP_aes_192_cbc();;
	if (!cipher) return -1;

	unsigned char key[128] = "12345467890ABCDEF12345678";

	//获取算法的分组大小
	int block_size = EVP_CIPHER_block_size(cipher); //字节数
	int key_size = EVP_CIPHER_key_length(cipher);   //秘钥长度
	int iv_size = EVP_CIPHER_iv_length(cipher);     //向量长度

	std::cout << block_size << std::endl;  // 16 字节
	std::cout << key_size << std::endl;	   // 24 字节
	std::cout << iv_size << std::endl;	   // 16 字节

	//初始化向量
	unsigned char iv[128] = { 0 };
	
	//加解密上下文
	auto ctx = EVP_CIPHER_CTX_new();

	//初始化上下文 1 是加密的意思
	int re = EVP_CipherInit(ctx, cipher, key, iv, 1); 
	if (!re) {
		std::cerr << "error" << std::endl;
		return 0;
	};

	EVP_CIPHER_CTX_set_padding(ctx, 1);

	int out_size = 0; //输出数据大小
	
	//开启自动填充
	EVP_CIPHER_CTX_set_padding(ctx, 1);
	//获得结果,默认 如果 33个字节 只加密 32个 字节, 7个字节直接不加密

	EVP_CipherUpdate(ctx, out, &out_size, data, strlen((char*)data));
	cout << "EVP_CipherUpdate size :" << out_size << std::endl;
	
	//最后一个分组数据，因为可能发生填充
	int pad = 0;
	EVP_CipherFinal(ctx, out + out_size, &pad);
	out_size += pad;
	cout << "padding size :" << pad << std::endl;

	std::cout << out << std::endl;

	//解密
	unsigned char iv_decode[128] = { '\0' }; //初始化向量

	int re_again = EVP_CipherInit(ctx, cipher, key, iv, 0); //解密

	unsigned char out2[1024] = { 0 }; //输出

	int out_size2 = 0;

	if (!re_again) {
		std::cerr << "error again" << std::endl;
		return 0;
	};

	EVP_CipherUpdate(ctx, out2, &out_size2, out, out_size);
	cout << "out_size2 size :" << out_size2 << std::endl;

	EVP_CipherFinal(ctx, out2 + out_size2, &pad);


	std::cout << out2 << std::endl;
	std::cout << pad << std::endl; //填充部分数据大小


	//释放掉
	EVP_CIPHER_CTX_free(ctx);
}

```



#### 2. EVP_aes_192_ecb

和cbc差别不大，只是不需要传递初始化向量。

```cpp
#pragma warning(disable : 4996)
#include <iostream>
#include "base.h"
#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include<openssl/evp.h>

using namespace std;

void PrintHex(const string& str) {
	for (auto c : str) {
		std::cout << hex << (int)(unsigned char)c;
	}
	cout << endl;
}


int main()
{
	const unsigned char data[] = { "012345670123456701234567sdasd" };
	unsigned char out[1024] = { 0 }; //输出

	auto cipher = EVP_aes_192_ecb();;
	if (!cipher) return -1;

	unsigned char key[128] = "12345467890ABCDEF12345678";

	//获取算法的分组大小
	int block_size = EVP_CIPHER_block_size(cipher); //字节数
	int key_size = EVP_CIPHER_key_length(cipher);   //秘钥长度
	int iv_size = EVP_CIPHER_iv_length(cipher);     //向量长度

	std::cout << block_size << std::endl;  // 16 字节
	std::cout << key_size << std::endl;	   // 24 字节
	std::cout << iv_size << std::endl;	   // 16 字节

	//初始化向量
	unsigned char iv[128] = { 16 };
	
	//加解密上下文
	auto ctx = EVP_CIPHER_CTX_new();

	

	//初始化上下文 1 是加密的意思
	int re = EVP_CipherInit(ctx, cipher, key, nullptr, 1);
	if (!re) {
		std::cerr << "error" << std::endl;
		return 0;
	};

	EVP_CIPHER_CTX_set_padding(ctx, 1);

	int out_size = 0; //输出数据大小
	
	//开启自动填充
	EVP_CIPHER_CTX_set_padding(ctx, 1);
	//获得结果,默认 如果 33个字节 只加密 32个 字节, 7个字节直接不加密

	EVP_CipherUpdate(ctx, out, &out_size, data, strlen((char*)data));
	cout << "EVP_CipherUpdate size :" << out_size << std::endl;
	
	//最后一个分组数据，因为可能发生填充
	int pad = 0;
	EVP_CipherFinal(ctx, out + out_size, &pad);
	out_size += pad;
	cout << "padding size :" << pad << std::endl;

	std::cout << out << std::endl;

	//解密
	unsigned char iv_decode[128] = { '\0' }; //初始化向量

	int re_again = EVP_CipherInit(ctx, cipher, key, nullptr, 0); //解密

	unsigned char out2[1024] = { 0 }; //输出

	int out_size2 = 0;

	if (!re_again) {
		std::cerr << "error again" << std::endl;
		return 0;
	};

	EVP_CipherUpdate(ctx, out2, &out_size2, out, out_size);
	cout << "out_size2 size :" << out_size2 << std::endl;

	EVP_CipherFinal(ctx, out2 + out_size2, &pad);


	std::cout << out2 << std::endl;
	std::cout << pad << std::endl; //填充部分数据大小


	//释放掉
	EVP_CIPHER_CTX_free(ctx);
}

```

