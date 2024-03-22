### [1. PEM格式](#)

**PEM（Privacy-Enhanced Mail）** 是一种基于文本的文件格式，用于存储和传输加密的数据、证书和私钥等信息。它最初设计用于在电子邮件系统中安全地传输加密数据，但现在已广泛应用于各种领域，特别是在公钥基础设施（PKI）和加密通信中。

-----

**PEM** 格式的文件通常使用扩展名 **.pem**，其内容由开始标记（如 **-----BEGIN CERTIFICATE-----**）和结束标记（如 **-----END CERTIFICATE-----**）包围的 Base64 编码的数据组成。这些标记用于标识文件中的不同类型的数据，例如证书、私钥、CA 根证书等。

PEM 格式可以用于存储多种类型的数据，包括：

- **证书**：如 X.509 数字证书，存储公钥和相关信息。
- **私钥**：如 RSA、DSA、ECDSA 等算法生成的私钥。
- **密钥对**：包含公钥和私钥。
- **CSR**（Certificate Signing Request）：用于请求签发数字证书的文件。

PEM 格式的文件可以使用文本编辑器进行查看和编辑，并且可以通过简单的复制粘贴进行数据的导入和导出。因此，PEM 格式非常方便用于在不同系统或应用程序之间交换加密数据和密钥材料。

在 OpenSSL 中，常用的密钥和证书文件格式（如 `.pem`、`.crt`、`.key`）通常都是 PEM 格式的文件。



私钥标记：

````
-----BEGIN RSA PRIVATE KEY----- and -----END RSA PRIVATEKEY-----
````

### [1. EVP_PKEY](#)

**EVP_PKEY** 是 OpenSSL 中用于表示和操作密钥的结构体类型。它是一个抽象的数据结构，可以用来存储各种类型的密钥，包括公钥、私钥、对称密钥等。

在 OpenSSL 中，`EVP_PKEY` 结构体是通过 `EVP_PKEY_new` 函数创建的，通过 `EVP_PKEY_free` 函数释放。它可以用于进行各种密钥相关操作，如生成密钥对、导入导出密钥、签名验证、加密解密等。

通过 `EVP_PKEY` 结构体，可以获取密钥的相关信息，例如算法类型、密钥位数等。它还提供了一组函数，用于执行与密钥相关的操作，如使用私钥进行签名、使用公钥进行验证、使用对称密钥进行加密解密等。



#### [1.1 EVP_PKEY_CTX_new_id](#)

`EVP_PKEY_CTX_new_id` 是 OpenSSL 中用于创建和初始化密钥上下文对象（`EVP_PKEY_CTX`）的函数之一。它接受一个算法标识符参数，用于指定要使用的密钥算法类型。

```cpp
EVP_PKEY_CTX *EVP_PKEY_CTX_new_id(int id, ENGINE *e);
```

参数说明：

- `id`：算法标识符，用于指定要使用的密钥算法类型。可以使用 OpenSSL 提供的宏定义来指定具体的算法，例如
  - EVP_PKEY_RSA  : RSA的密钥对！
  - EVP_PKEY_EC 等。
- `e`：可选参数，表示要使用的加密引擎。通常情况下，可以将其设置为 `NULL`，使用默认的加密引擎。

```cpp
EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

EVP_PKEY_CTX_new_id(EVP_PKEY_EC, nullptr);
```

该函数返回创建的密钥上下文对象（`EVP_PKEY_CTX`），如果创建失败则返回 `NULL`。



### 2. 生成RSA 密钥对

然后存放到文件中！

```cpp
#include<openssl/evp.h>
#include<openssl/pem.h>
#include<openssl/rsa.h>

// 返回值由调用者 EVP_PKEY_free(EVP_PKEY*)  释放 
EVP_PKEY* EvpRsaKey(uint16_t bits) {
	// 1. 创建 RSA 公钥加密上下文
	EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
	if (!ctx) {
		ERR_print_errors_fp(stderr);
		return nullptr;
	}
	// 2. 初始化密钥对生成上下文
	if (EVP_PKEY_keygen_init(ctx) <= 0) {
		ERR_print_errors_fp(stderr);
		EVP_PKEY_CTX_free(ctx);
		return nullptr;
	}

	// 3. 设置参数 RSA 秘钥位数
	if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
		ERR_print_errors_fp(stderr);
		EVP_PKEY_CTX_free(ctx);
		return nullptr;
	}
	// 4. 秘钥生成
	// 内部会需要申请空间
	EVP_PKEY* pkey = nullptr;
	if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
		ERR_print_errors_fp(stderr);
		EVP_PKEY_CTX_free(ctx);
		return nullptr;
	}

	// 5. 释放上下文
	EVP_PKEY_CTX_free(ctx);
	return pkey;
}

int main()
{
	const unsigned char data[] = {
"012345670123456701234567s984das324d98w132daasdasdasdasdddddasd556646489sd23a4s98dqwd46a561a2f1ghtgh4tasdasdahgh5+56584as5d18wf3a1sd89wd3w489dasd"
	};

	unsigned char out[1024] = { 0 }; //加密后
	unsigned char out2[1024] = { 0 }; //解密后


	EVP_PKEY* kp = EvpRsaKey(512);

	const OSSL_PARAM* tb = EVP_PKEY_gettable_params(kp);
	/*
	struct ossl_param_st {
		const char *key;             // the name of the parameter 
		unsigned int data_type;      // declare what kind of content is in buffer
		void* data;                  // value being passed in or out 
		size_t data_size;            // data size 
		size_t return_size;         // returned content size 
	}*/

	while (tb)
	{
		if (!tb->key) break;
		cout << tb->key << endl;
		tb++;
	}
	/*
	bits
	security-bits
	max-size
	default-digest
	n
	e
	d
	rsa-factor1
	rsa-factor2
	rsa-factor3
	rsa-factor4
	rsa-factor5
	rsa-factor6
	rsa-factor7
	rsa-factor8
	rsa-factor9
	rsa-factor10
	rsa-exponent1
	...
	*/
	BIGNUM* d = nullptr;
	EVP_PKEY_get_bn_param(kp, "d", &d);
	PrintBn(d);
	BN_free(d);

	//输出公钥 pem 文件
	FILE* pubf = fopen("pubkey.pem", "w");
	if (!pubf) {
		std::cerr <<"open failed";
	}
	PEM_write_RSAPublicKey(pubf, EVP_PKEY_get0_RSA(kp));

	//输出私钥 pem 文件
	FILE* prif = fopen("prikey.pem", "w");
	if (!pubf) {
		std::cerr << "open failed";
	}
	PEM_write_RSAPrivateKey(
		prif, 
		EVP_PKEY_get0_RSA(kp),
		nullptr, //需要对私钥加密码？需要传递加密算法
		nullptr, //秘钥
		0,		 //秘钥长度
		nullptr,	 // 加密回调函数
		nullptr 	 // 用户数据回调使用
	);

	fclose(pubf);
	fclose(prif);

	BN_free(d);

	EVP_PKEY_free(kp);
}
```



### 3. EVP_PKEY 接口

相关接口：

```cpp
int EVP_PKEY_encrypt_init(EVP_PKEY_CTX *ctx);

int EVP_PKEY_encrypt(EVP_PKEY_CTX *ctx, unsigned char *out, size_t *outlen,
                     const unsigned char *in, size_t inlen);

int EVP_PKEY_decrypt_init(EVP_PKEY_CTX *ctx);

int EVP_PKEY_decrypt(EVP_PKEY_CTX *ctx,
                     unsigned char *out, size_t *outlen,
                     const unsigned char *in, size_t inlen);
```



#### 3.1 读取密钥文件

直接使用接口即可：

```cpp
RSA* rPub = nullptr;
RSA* rPri = nullptr;

FILE* pubf = fopen("pubkey.pem", "r");
FILE* prif = fopen("prikey.pem", "r");

PEM_read_RSAPublicKey(pubf, &rPub, NULL, NULL);
PEM_read_RSAPrivateKey(prif, &rPri, NULL, NULL);
```



#### 3.2 EVP_PKEY_encrypt

`EVP_PKEY_encrypt` 是 OpenSSL 中用于使用公钥进行加密的函数之一。它可以将输入的明文数据使用指定的公钥进行加密，得到密文数据。

```cpp
int EVP_PKEY_encrypt(EVP_PKEY_CTX *ctx, unsigned char *out, size_t *outlen,
                     const unsigned char *in, size_t inlen);
```

参数说明：

- `ctx`：密钥上下文对象（`EVP_PKEY_CTX`），其中包含了进行加密操作所需的上下文信息和密钥信息。
- `out`：输出缓冲区，用于存储加密后的密文数据。
- `outlen`：输出缓冲区的长度，同时也是密文数据的长度。
- `in`：输入缓冲区，待加密的明文数据。
- `inlen`：输入缓冲区的长度，即待加密的明文数据长度。

函数返回值为整数类型，表示加密操作的结果：

- 如果返回值大于等于 0，表示加密成功，返回值为实际加密的密文数据的长度。
- 如果返回值为 -1，表示加密失败。

**例子**：

```cpp
const unsigned char data[] = {
    "012345670123456701234567s984das324d98w132daasdasdasdasdddddasd556646489sd23a4s98dqwd46a561a2f1ghtgh4tasdasdahgh5+56584as5d18wf3a1sd89wd3w489dasd"
};

unsigned char out[1024] = { 0 }; //加密后
unsigned char out2[1024] = { 0 }; //解密后

RSA* rPub = nullptr;
RSA* rPri = nullptr;

FILE* pubf = fopen("pubkey.pem", "r");
FILE* prif = fopen("prikey.pem", "r");

PEM_read_RSAPublicKey(pubf, &rPub, NULL, NULL);
PEM_read_RSAPrivateKey(prif, &rPri, NULL, NULL);

if (rPub == nullptr) {
    ERR_print_errors_fp(stderr);
}

//通过
EVP_PKEY* pkey = EVP_PKEY_new();
EVP_PKEY_set1_RSA(pkey, rPub);

//生成上下文
auto ctx = EVP_PKEY_CTX_new(pkey, NULL);

//加密
EVP_PKEY_encrypt_init(ctx);
int key_size   = RSA_size(rPub);
int block_size = key_size - RSA_PKCS1_PADDING_SIZE;

// coding_size 已经加密的数据大小
int coding_size = 0;
// 已经获得的密文大小
int cipher_size = 0;

int data_size = strlen((char*)data);

while (coding_size < data_size) {
    //真实的数据大小，最后一块的数据可能小于  block_size
    auto real_block = ((data_size - coding_size) > block_size) ? block_size : (data_size - coding_size);

    size_t out_size = key_size;
    EVP_PKEY_encrypt(ctx,
        out + cipher_size,
        &out_size,
        data + coding_size,
        real_block
    );

    cipher_size += out_size;
    coding_size += block_size;
}

std::cout << cipher_size << std::endl;


RSA_free(rPub);
RSA_free(rPri);


EVP_PKEY_CTX_free(ctx);
EVP_PKEY_free(pkey);
```



#### 3.3 EVP_PKEY_decrypt

**EVP_PKEY_decrypt** 是 OpenSSL 中用于使用私钥进行解密的函数之一。它可以将输入的密文数据使用指定的私钥进行解密，得到明文数据。

```cpp
int EVP_PKEY_decrypt(EVP_PKEY_CTX *ctx, unsigned char *out, size_t *outlen,
                     const unsigned char *in, size_t inlen);
```

参数说明：

- `ctx`：密钥上下文对象（`EVP_PKEY_CTX`），其中包含了进行解密操作所需的上下文信息和密钥信息。
- `out`：输出缓冲区，用于存储解密后的明文数据。
- `outlen`：输出缓冲区的长度，同时也是明文数据的长度。
- `in`：输入缓冲区，待解密的密文数据。
- `inlen`：输入缓冲区的长度，即待解密的密文数据长度。

函数返回值为整数类型，表示解密操作的结果：

- 如果返回值大于等于 0，表示解密成功，返回值为实际解密的明文数据的长度。
- 如果返回值为 -1，表示解密失败。



#### 3.4 加密封装

公钥加密封装

```cpp
int EVP_RSA_PublicKey_Encode(RSA* r, const unsigned char* data, int data_size, unsigned char* out, int out_size) {
	//通过
	EVP_PKEY* pkey = EVP_PKEY_new();
	EVP_PKEY_set1_RSA(pkey, r);

	//生成上下文
	auto ctx = EVP_PKEY_CTX_new(pkey, NULL);

	//加密
	EVP_PKEY_encrypt_init(ctx);
	int key_size = RSA_size(r);
	int block_size = key_size - RSA_PKCS1_PADDING_SIZE;

	// coding_size 已经加密的数据大小
	int coding_size = 0;
	// 已经获得的密文大小
	int cipher_size = 0;


	while (coding_size < data_size) {
		//真实的数据大小，最后一块的数据可能小于  block_size
		auto real_block = ((data_size - coding_size) > block_size) ? block_size : (data_size - coding_size);

		size_t out_size = key_size;
		int rte = EVP_PKEY_encrypt(ctx,
			out + cipher_size,
			&out_size,
			data + coding_size,
			real_block
		);
		if (rte < 0) {
			ERR_print_errors_fp(stderr);
			EVP_PKEY_CTX_free(ctx);
			EVP_PKEY_free(pkey);
			return -1;
		}
		cipher_size += out_size;
		coding_size += block_size;
	}

	EVP_PKEY_CTX_free(ctx);
	EVP_PKEY_free(pkey);
	return cipher_size;
}
```

#### 3.5 解密封装

私钥解密封装

```cpp
int EVP_RSA_PrivateKey_Decode(RSA* r, const unsigned char* cipher, int cipher_size, unsigned char* out, int out_size) {
	auto key_size = RSA_size(r); // 秘钥大小

	if (cipher_size <= 0 || cipher_size % key_size != 0) return -1;
	//通过
	EVP_PKEY* pkey = EVP_PKEY_new();
	EVP_PKEY_set1_RSA(pkey, r);

	//生成上下文
	auto ctx = EVP_PKEY_CTX_new(pkey, NULL);

	//解密
	EVP_PKEY_decrypt_init(ctx);
	
	//数据大小
	int block_size = key_size - RSA_PKCS1_PADDING_SIZE;

	size_t write_index = 0;
	
	for (int j = 0; j < cipher_size; j += key_size) {
		size_t outSize = key_size;
		int rte = EVP_PKEY_decrypt(ctx,
			out + write_index,
			&outSize,
			cipher + j,
			key_size
		);

		if (rte < 0) {
			ERR_print_errors_fp(stderr);
			EVP_PKEY_CTX_free(ctx);
			EVP_PKEY_free(pkey);
			return -1;
		}
		write_index += outSize;
	}
	EVP_PKEY_CTX_free(ctx);
	EVP_PKEY_free(pkey);
	return write_index;
}
```



加密解密测试：

```cpp
const unsigned char data[] = { "012345670123456701234567s984das324d98w132daasdasdasdasdddddasd556646489sd23a4s98dqwd46a561a2f1ghtgh4tasdasdahgh5+56584as5d18wf3a1sd89wd3w489dasd"
};

unsigned char out[1024] = { 0 }; //加密后
unsigned char out2[1024] = { 0 }; //解密后

RSA* rPub = nullptr;
RSA* rPri = nullptr;

FILE* pubf = fopen("pubkey.pem", "r");
FILE* prif = fopen("prikey.pem", "r");

PEM_read_RSAPublicKey(pubf, &rPub, NULL, NULL);
PEM_read_RSAPrivateKey(prif, &rPri, NULL, NULL);

int re = EVP_RSA_PublicKey_Encode(rPub, data, strlen((char*)data), out, 1024);

std::cout << "len: " << re << std::endl;

EVP_RSA_PrivateKey_Decode(rPri, out, re, out2, 1024);

std::cout  << out2 << std::endl;
```
