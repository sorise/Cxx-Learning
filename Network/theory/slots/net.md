###  [网络数据流向](#)

----

首先是，网卡接受数据，转换为可以识别的数字信号。

> 网卡位于 `OIS` 中数据链路层，或者说是位于数据链路层和物理层的中间！在 **`TCP/IP`** 协议中位于物理层，**负责将物理层的光电信号转换为数据链路层中可以识别的数字信号。**

>  在 `linux`系统中使用 `eth0`，`eth1`，`eth2`……代表网卡一，网卡二，网卡三……

网卡收到数据后传输给网络协议栈。