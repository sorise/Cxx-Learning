### [对称加密技术](#)

**介绍**：**3DES**（Triple Data Encryption Standard）是一种对称加密算法，它基于DES算法进行了三次迭代加密。3DES提供了更高的安全性，同时保留了DES的兼容性。

### 1. 概述

3DES是DES向AES过渡的加密算法，它使用2个或者3个56位的密钥对数据进行三次加密。相比DES，3DES因密钥长度变长，安全性有所提高，但其处理速度不高。因此又出现了AES加密算法，AES较于3DES速度更快、安全性更高。

* **3DES 密钥长度为128位/192位**，即16或24个字节组成的字符串；



处理填充部分的方法：

* 固定填充即可，对于30字节的数据，填充2个字节，最后一个块只有六个字节的数据，填充补0，然后返回块，只提取其中6个字节数据。



#### 1.1 加密、解密方法

```cpp
#pragma warning(disable : 4996)
#include <iostream>
#include <openssl/des.h>
#include<openssl/evp.h>

using namespace std;


int main()
{
	const unsigned char data[] = { "012345670123456701234567sdasd" };
	unsigned char out[1024] = { 0 }; //输出

	unsigned char key[128] = "12345467890ABCDEF12345678";
	unsigned char iv[128] = { '\0' }; //初始化向量


	//加解密算法
	auto cipher = EVP_des_ede3_cbc(); 

	//获取算法的分组大小
	int block_size = EVP_CIPHER_block_size(cipher); //字节数
	int key_size = EVP_CIPHER_key_length(cipher);   //秘钥长度
	int iv_size = EVP_CIPHER_iv_length(cipher);     //向量长度

	std::cout << block_size << std::endl;  // 8  字节
	std::cout << key_size << std::endl;	   // 24 字节
	std::cout << iv_size << std::endl;	   // 8 字节

	//加解密上下文
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	//加密算法初始化
	int re = EVP_CipherInit(ctx, cipher, key, iv, 1); //加密
	if (!re) {
		std::cerr << "error" << std::endl;
		return 0;
	};

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

