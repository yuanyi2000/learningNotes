# 第二章 信息的表示和处理

*无符号* (unsigned) 基于传统的二进制表示法，表示大于或等于0的数字
*补码* (two's-complement) 表示有符号整数最常见的方式
*浮点数* (floating-point) 表示是熟的科学技术法的以2为基数的版本

## 2.1 信息储存

*字节* ：8位的块

机器级程序将内存视为一个非常大的字节数组，成为**虚拟内存**(virtual memory)。
内存中的每个字节都有一个唯一的数组来标识，成为他的*地址(address)*
所有可能地址的集合就称作*虚拟地址空间(virtual address space)*

### 2.1.1 16进制表示法

> 把一个10进制数 x=2^(i+4j) (0<= i <= 3) 表示为16进制为 ： 0x(x^i)000...000 (j个0)
>
>> 因为2^n的16进制就是1后面跟n个0
>
>
>>> 例如 x=2048=2^11 , n=11=3+4\*2 , 所以16进制为 0x800 (8=2^3 后面跟2个0)


十进制和十六进制之间的转换 
	
	把10进制x转换为16进制
	反复用16除以x
	x = 16q + r


>例如计算 314156
>	314156 = 19634 × 16 + 12 (C)
>	 19634 =  1227 × 16 + 2  (2)
>	  1227 =    76 × 16 + 11 (B)
>	    76 =     4 × 16 + 12 (C)
>		 4 =     0 × 16 + 4  (4)
>	
>	314156 = 0x4CB2C


### 2.1.2 字数据大小

每台计算机都有一个*字长(word size)*, 指明指针数据的标称大小(normal size)

> 32位字长限制虚拟地址空间为4GB(4千兆字节)，而64位字长使得虚拟地址空间为16EB，大约1.84 × 10^19字节

<escape>
<table>
<thead>
  <tr>
    <th colspan="2">C声明</th>
    <th colspan="2">字节数</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>有符号</td>
    <td>无符号</td>
    <td>32位</td>
    <td>64位</td>
  </tr>
  <tr>
    <td>[signed] char</td>
    <td>unsigned char</td>
    <td>1</td>
    <td>1</td>
  </tr>
  <tr>
    <td>short</td>
    <td>unsigned short </td>
    <td>2</td>
    <td>2</td>
  </tr>
  <tr>
    <td>int</td>
    <td>unsigned int </td>
    <td>4</td>
    <td>4</td>
  </tr>
  <tr>
    <td>long</td>
    <td>unsigned long</td>
    <td>4</td>
    <td>8</td>
  </tr>
  <tr>
    <td>int32_t</td>
    <td>uint32_t</td>
    <td>4</td>
    <td>4</td>
  </tr>
  <tr>
    <td>int63_t</td>
    <td>uint64_t</td>
    <td>8</td>
    <td>8</td>
  </tr>
  <tr>
    <td>char *</td>
    <td></td>
    <td>4</td>
    <td>8</td>
  </tr>
  <tr>
    <td>float </td>
    <td></td>
    <td>4</td>
    <td>4</td>
  </tr>
  <tr>
    <td>double </td>
    <td></td>
    <td>8</td>
    <td>8</td>
  </tr>
</tbody>
</table>
</escape>

### 2.1.3 寻址和字节顺序

一个w位的整数 [x(w-1), x(w-2), ... , x(1), x(0)]
*小端法(little endian)* 最低有效位在最前面
*大端法(big endian)*    最高有效位在最前面

字节顺序成为问题的情况
+ 在不同类型的机器之间通过网络传输二进制数据时
+ 阅读表示整数数据的字节序列时
	43 0B 20 00 -> 0x200B43
+ 编写规避正常的类型系统的程序时
