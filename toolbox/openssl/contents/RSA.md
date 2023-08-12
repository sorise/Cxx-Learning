### [RAS 非对称加密算法](#)

**RSA（Rivest-Shamir-Adleman）**是一种非对称加密算法，广泛应用于信息安全领域。它是由 Ron Rivest、Adi Shamir 和 Leonard Adleman 三位密码学家在1977年共同提出的。

-----



### 1. 概述

RSA 算法依赖于大数因子分解的困难性，其基本原理如下：

1. 密钥生成：
   - 随机选择两个大质数 p 和 q。
   - 计算 n = p * q，这个值将作为 RSA 的模数。
   - 计算 φ(n) = (p - 1) * (q - 1)，φ(n) 被称为欧拉函数。
   - 选择一个整数 e，满足 1 < e < φ(n)，且 e 与 φ(n) 互质。e 将作为公钥中的指数。
   - 计算 d，使得 (d * e) % φ(n) = 1。d 将作为私钥中的指数。
2. 加密：
   - 将明文消息转换成数字，使其小于模数 n。
   - 使用公钥中的指数 e 对明文进行加密，计算密文 C = M^e mod n，其中 M 是明文，C 是密文。
3. 解密：
   - 使用私钥中的指数 d 对密文进行解密，计算明文 M = C^d mod n。

RSA 算法的安全性依赖于大数因子分解的困难性，即在已知 n 的情况下，无法有效地计算出 p 和 q。只有掌握了私钥中的指数 d 才能够解密密文并获取明文。

RSA 算法的主要优点是：

- 提供了可靠的安全性，依赖于大数因子分解的数学问题。
- 可以用于加密或签名，支持信息机密性和完整性的保护。
- 适用于各种数据长度的加密和解密操作。

然而，RSA 算法也有一些限制和注意事项：

- 需要进行大数运算，对于非常大的数可能会比较耗时。
- 密钥管理是重要的一环，合理地生成、保管和更新密钥对至关重要。
- 对于特别长的消息，可以使用混合加密方案，即使用 RSA 加密对称加密算法的密钥，然后使用对称加密算法加密消息，以提高性能。

总之，RSA 算法是一种广泛使用的非对称加密算法，被广泛应用于保护敏感信息和实现安全通信。



#### 1.1 常见的 RSA 数据填充机制

**RSA也是采用分组加密，所以如果数据不够分组大小也需要填充，但是受制于算法设计约束，填充策略有所约束。**

**PKCS#1 v1.5 填充**：

- PKCS#1 v1.5 是 RSA 实验室发布的填充标准，广泛应用于 RSA 加密和签名中。
- 在加密时，明文数据会被填充为一个特定长度的消息块，以满足 RSA 算法对输入长度要求。填充内容包括随机字节和特定的固定字节序列。
- PKCS#1 v1.5 填充方案可以提供较好的安全性和兼容性。

**OAEP（Optimal Asymmetric Encryption Padding）填充**：

- OAEP 是一种更现代的填充方案，设计用于解决 PKCS#1 v1.5 填充方案中的一些安全性问题。
- OAEP 填充方案包括将明文进行扩展和混淆的步骤，以增加对抗密码分析的能力。
- OAEP 填充是一种保守的填充方案，并通过加入随机数来提供一定程度的安全性。



需要注意的是，填充机制只应用于 RSA 加密和签名操作中，对于 RSA 的解密和验签操作，需要在解密或验签后对填充进行验证和去除，以获取原始的明文数据。



### 2. 秘钥生成

RSA 算法使用一对公钥和私钥来进行加密和解密操作。公钥可用于加密数据，私钥用于解密数据。以下是公钥和私钥的基本信息：

1. 公钥：
   - 公钥由两个参数组成：(e, n)
   - e 是公钥指数（public exponent），通常选取一个较小的素数，常见的选择是 65537（0x10001）。
   - n 是模数（modulus），n 是两个大质数 p 和 q 的乘积。
2. 私钥：
   - 私钥由两个参数组成：(d, n)
   - d 是私钥指数（private exponent），它满足 (d * e) mod φ(n) = 1，其中 φ(n) = (p-1) * (q-1) 是欧拉函数。
   - n 是模数（modulus），同样是两个大质数 p 和 q 的乘积。

```cpp
//生成RSA 密钥对
RSA* CreateRSAKey() {
	// 存放rsa密钥对
	RSA* r = RSA_new();

	//公钥指数是一个大质数 (E,N)
	BIGNUM* e = BN_new();
	BN_set_word(e, RSA_3); //RSA_F4 是 0x10001L 是公钥质数 ,RSA_3 是 质数 3
	//第二个参数也可以提供一个随机数 但必须是质数

	//生成私钥 指数D 和 N = p*q, p、q是随机大质数
	RSA_generate_key_ex(
		r,
		512, //生成多少位的秘钥，此地是 64字节
		e, //公钥指数
		nullptr //秘钥生成回调
	);
	return r;
}
```

打印测试：

```cpp
void PrintBn(const BIGNUM* n) {
	unsigned char out[256] = { 0 };
	BN_bn2bin(n, out);
	int byte_size = BN_num_bytes(n);
	for (size_t i = 0; i < byte_size; i++)
	{
		printf("%02x", out[i]);
	}
	printf("\n");
}

int main()
{
	auto r = CreateRSAKey();
	//模数N
	auto n = RSA_get0_n(r);
	auto e = RSA_get0_e(r); //公钥质数
	auto d = RSA_get0_d(r); //私钥质数

	PrintBn(n);
	PrintBn(e);
	PrintBn(d);

}
```

### 3. 加解密函数

一般来说私钥的持有者给别人发生数据，使用自己的私钥加密。

| 函数                | 功能                                   |
| ------------------- | -------------------------------------- |
| RSA_public_encrypt  | 使用公钥加密，需要私钥来解密           |
| RSA_public_decrypt  | 使用公钥解密，数据是使用私钥加密了的   |
| RSA_private_decrypt | 使用私钥解密，数据使用公钥加密了       |
| RSA_private_encrypt | 使用私钥加密，数据需要使用公钥解密了。 |

#### 3. 1 RSA_public_encrypt

```cpp
int RSA_public_encrypt(int flen, const unsigned char *from, unsigned char *to,
                       RSA *rsa, int padding);
```

参数说明如下：

- `flen`： 明文数据长度字节数，若padding参数使用RSA_PKCS1_PADDING方式，则该值最大为所使用密钥的位数 / 8 - 11。
- `from`：指向待加密的数据缓冲区的指针。
- `to`：指向加密后的数据缓冲区的指针，必须预先分配足够的空间来保存加密后的数据，大小应为 RSA_size(rsa)。
- `rsa`：指向 RSA 结构体的指针，包含了公钥的信息。
- `padding`：指定填充方案，常见的取值有：
  - `RSA_PKCS1_PADDING`：使用 `PKCS#1 v1.5` 填充方案。
  - `RSA_NO_PADDING`：不使用填充方案，明文数据长度必须等于 RSA 模长。

`RSA_public_encrypt` 函数的返回值为加密后的数据长度（如果加密失败，则返回 -1）。

```cpp
#ifdef _WIN32
#include<openssl/applink.c>
#endif // _WIN32

int RSA_PublicKey_Encode(RSA* r, const unsigned char *data, int data_size, unsigned char* out, int out_size) {
	// 输出数据的大小
	int key_size = RSA_size(r);
	// 数据切割大小 传入数据的大小
	int block_size = RSA_size(r) - RSA_PKCS1_PADDING_SIZE;
	// coding_size 已经加密的数据大小
	int coding_size = 0;
	// 已经获得的密文大小
	int cipher_size = 0;
	// 需求大小
	int need_out_size = ((data_size / block_size) + ((data_size % block_size) == 0) ? 0 : 1) * key_size;
	if (out_size < 0) return -1;

	while (coding_size < data_size) {
		//真实的数据大小，最后一块的数据可能小于  block_size
		auto real_block = ((data_size - coding_size) > block_size) ? block_size : (data_size - coding_size);

		int re = RSA_public_encrypt(
			block_size,
			data + coding_size,
			out + cipher_size,
			r,
			RSA_PKCS1_PADDING	//填充方式
		);

		if (re < 0) {
			ERR_print_errors_fp(stderr);
			return -1;
		}

		cipher_size += re;
		coding_size += block_size;
	}
	return cipher_size;
}

```

#### 3.2 RSA_private_decrypt

```cpp
int RSA_private_decrypt(int flen, const unsigned char *from,
                        unsigned char *to, RSA *rsa, int padding);
```

* `flen` :密文数据长度，一般固定为秘钥位数 / 8
* `from` : 密文数据
* `to` ：存放解密后的明文数据，该空间大小应该为秘钥位数 / 8，保证可以存放的下
* `rsa`	私钥
* `padding`	填充方式

```cpp
//私钥解密 需要同时具有 e,n,d
int RSA_PrivateKey_Decode(RSA* r, const unsigned char* cipher, int cipher_size, unsigned char* out, int out_size) {
	int key_size = RSA_size(r);
	if (cipher_size <= 0 || cipher_size % key_size != 0) return -1;

	int write_index = 0;
	for (int j = 0; j < cipher_size; j += key_size) {
		int rte = RSA_private_decrypt(
			key_size,
			cipher + j,
			out + write_index,
			r,
			RSA_PKCS1_PADDING	//填充方式
		);

		if (rte < 0) {
			ERR_print_errors_fp(stderr);
			return -1;
		}
		write_index += rte;
	}
}

```

#### 3.3 测试

```cpp
const unsigned char data[] = { "012345670123456701234567s984das324d98w132daasdasdasdasddddddddddddddddddddddsadasd\
556646489sd23a4s98dqwd46a561a2f1ghtgh4tasdasdahgh5+56584as5d18wf3a1sd89wd3w489dasd" };

unsigned char out[1024] = { 0 }; //加密后
unsigned char out2[1024] = { 0 }; //解密后

auto r = CreateRSAKey();

auto data_size = strlen((char*)data);

int encode_size = RSA_PublicKey_Encode(r, data, data_size, out, 1024);


/* 秘钥复制 */
RSA* rd = RSA_new();
auto n = BN_new();
auto d = BN_new();
auto e = BN_new();

BN_copy(n, RSA_get0_n(r));
BN_copy(d, RSA_get0_d(r));
BN_copy(e, RSA_get0_e(r));

RSA_set0_key(rd, n, e, d);


int key_size = RSA_size(rd);

int decode_size = RSA_PrivateKey_Decode(rd, out, encode_size, out2, 1024);

std::cout << out2 << std::endl;

RSA_free(r);
```



#### 3.4 RSA_private_encrypt

使用私钥加密！

```cpp
int RSA_PrivateKey_Encode(RSA* r, const unsigned char* data, int data_size, unsigned char* out, int out_size) {
	// 输出数据的大小
	int key_size = RSA_size(r);
	// 数据切割大小 传入数据的大小
	int block_size = RSA_size(r) - RSA_PKCS1_PADDING_SIZE;
	// coding_size 已经加密的数据大小
	int coding_size = 0;
	// 已经获得的密文大小
	int cipher_size = 0;
	// 需求大小
	int need_out_size = ((data_size / block_size) + ((data_size % block_size) == 0) ? 0 : 1) * key_size;
	if (out_size < 0) return -1;

	while (coding_size < data_size) {
		//真实的数据大小，最后一块的数据可能小于  block_size
		auto real_block = ((data_size - coding_size) > block_size) ? block_size : (data_size - coding_size);

		int re = RSA_private_encrypt(
			block_size,
			data + coding_size,
			out + cipher_size,
			r,
			RSA_PKCS1_PADDING	//填充方式
		);

		if (re < 0) {
			ERR_print_errors_fp(stderr);
			return -1;
		}

		cipher_size += re;
		coding_size += block_size;
	}
	return cipher_size;
}

```



#### 3.5 RSA_public_decrypt

使用公钥解密

```cpp
int RSA_PublicKey_Decode(RSA* r, const unsigned char* cipher, int cipher_size, unsigned char* out, int out_size) {
	int key_size = RSA_size(r);
	if (cipher_size <= 0 || cipher_size % key_size != 0) return -1;

	int write_index = 0;
	for (int j = 0; j < cipher_size; j += key_size) {
		int rte = RSA_public_decrypt(
			key_size,
			cipher + j,
			out + write_index,
			r,
			RSA_PKCS1_PADDING	//填充方式
		);

		if (rte < 0) {
			ERR_print_errors_fp(stderr);
			return -1;
		}
		write_index += rte;
	}
	return write_index;
}
```

#### 3.6 测试

测试

```cpp
const unsigned char data[] = { "012345670123456701234567s984das324d98w132daasdasdasdasddddddddddddddddddddddsadasd\
556646489sd23a4s98dqwd46a561a2f1ghtgh4tasdasdahgh5+56584as5d18wf3a1sd89wd3w489dasd" };

unsigned char out[1024] = { 0 }; //加密后
unsigned char out2[1024] = { 0 }; //解密后

auto r = CreateRSAKey();

auto data_size = strlen((char*)data);

int encode_size = RSA_PrivateKey_Encode(r, data, data_size, out, 1024);

std::cout << "encode size:" << encode_size << endl;

cout << out << endl;

/* 秘钥复制 */
RSA* rd = RSA_new();
auto n = BN_new();
auto e = BN_new();

BN_copy(n, RSA_get0_n(r));
BN_copy(e, RSA_get0_e(r));

RSA_set0_key(rd, n, e, 0);


int key_size = RSA_size(rd);

int decode_size = RSA_PublicKey_Decode(rd, out, encode_size, out2, 1024);

std::cout << decode_size << std::endl;

std::cout << out2 << std::endl;


RSA_free(r);
```



