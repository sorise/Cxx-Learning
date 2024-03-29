### [OpenSSL EVP](#)

**介绍**： [Openssl](https://so.csdn.net/so/search?q=Openssl&spm=1001.2101.3001.7020) EVP(high-level cryptographic functions[1])提供了丰富的密码学中的各种函数。Openssl 中实现了各种对称算法、摘要算法以及签名/验签算法。EVP 函数将这些具体的算法进行了封装。



### 1. 概述

EVP系列的函数的声明包含在 **”evp.h”** 里面，这是一系列封装了 openssl 加密库里面所有算法的函数。通过这样的统一的封装，使得只需要在初始化参数的时候做很少的改变，就可以使用相同的代码但采用不同的加密算法进行数据的加密和解密。
EVP系列函数主要封装了 **加密**、**摘要**、**编码**三大类型的算法，使用算法前需要调用 **OpenSSL_add_all_algorithms** 函数。
其中以加密算法与摘要算法为基本，公开密钥算法是对数据加密采用了对称加密算法，对密钥采用非对称加密（公钥加密，私钥解密）。数字签名是非对称算法（私钥签名，公钥认证）。
EVP 主要封装了如下功能函数：

* 实现了base64 编解码BIO；
* 实现了加解密BIO；
* 实现了摘要BIO；
* 实现了reliable BIO；
* 封装了摘要算法；
* 封装了对称加解密算法；
* 封装了非对称密钥的加密(公钥)、解密(私钥)、签名与验证以及辅助函数；
* 基于口令的加密(PBE)；
* 对称密钥处理；
* 数字信封：数字信封用对方的公钥加密对称密钥，数据则用此对称密钥加密。发送给对方时，同时发送对称密钥密文和数据密文。接收方首先用自己的私钥解
* 密密钥密文，得到对称密钥，然后用它解密数据。
* 其他辅助函数。
  

evp 源码位于crypto/evp 目录，可以分为如下几类：