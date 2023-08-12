### [国密SM4 对称分组加密算法](#)

**介绍**：SM4是一种分组密码算法，其**分组长度为128位**（即**16字节**，**4字**），**密钥长度也为128位**（即**16字节**，**4字**）。其加解密过程采用了**32轮**迭代机制（与DES、AES类似），每一轮需要一个轮密钥（与DES、AES类似）。

----

* **分组长度为128位**（即**16字节**，**4字**）
* **密钥长度也为128位**（即**16字节**，**4字**）



#### 具有 ecb 和 cbc 两种模式

结构和des、aes类似啊。

```cpp
	const unsigned char data[] = { "012345670123456701234567sdasd" };
	unsigned char out[1024] = { 0 }; //输出

	auto cipher = EVP_sm4_cbc();;
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
```

