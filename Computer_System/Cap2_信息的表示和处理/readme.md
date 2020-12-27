# 第二章 信息的表示和处理

*无符号* (unsigned) 基于传统的二进制表示法，表示大于或等于0的数字
*补码* (two's-complement) 表示有符号整数最常见的方式
*浮点数* (floating-point) 表示是熟的科学技术法的以2为基数的版本

## 2.1 信息储存

*字节* ：8位的块

机器级程序将内存视为一个非常大的字节数组，成为**虚拟内存**(virtual memory)。
内存中的每个字节都有一个唯一的数组来标识，成为他的*地址(address)*
所有可能地址的集合就称作*虚拟地址空间(virtual address space)*

*程序对象* 指的是程序数据、指令和控制信息

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


> 例如计算 314156
>
> 314156 = 19634 × 16 + 12 (C)  
>    
>	 19634 =  1227 × 16 + 2  (2)
>
>	  1227 =    76 × 16 + 11 (B)
>
>	    76 =     4 × 16 + 12 (C)
>
>		   4 =     0 × 16 + 4  (4)
>
>	
> 314156 = 0x4CB2C


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

  使用强制类型转换或联合来允许以一种数据类型引用一个对象，而这种数据类型和创建这个对象时定义的数据类型不同。
  大多数应用编程都不推荐这种编码技巧，但对系统级程序非常有用
```C
#include <stdio.h>
typedef unsigned char * byte_pointer; // 将数据类型byte_pointer定义为一个指向unsigned char的指针

void show_bytes(byte_pointer start, size_t len){  // 输入一个字节序列的地址，打印出每个16进制表示的字节
  size_t t;
  for (int i = 0; i < len; ++i)
  {
    printf(" %.2x\n", start[i]);
  }
  printf("\n");
}

void show_int(int x){
  show_bytes((byte_pointer) &x, sizeof(int));
}
void show_float(float x){
  show_bytes((byte_pointer) &x, sizeof(float));
}
void show_pointer(void *x){
  show_bytes((byte_pointer) &x, sizeof(void *));
}
```
可以观察到`show_int` `show_float` `show_pointer`仅仅传递给`show_byte`一个指向他们参数x的指针`&x`，这个指针被强制类型转换为`unsigned char *`。这个强制类型转换告诉编译器，程序应该把这个指针看做指向一个字节序列，而不是指向原始数据类型的对象。

---
练习题

1. 
ex2.5
```C
int val = 0x87654321;
byte_pointer valp = (byte_pointer) &val;
show_bytes(valp, 1); // A
show_bytes(valp, 2); // B
show_bytes(valp, 3); // C
```


|:-:|:-:|:-:|
| |小端机器|大端机器|
|:-:|:-:|:-:|
|A:|21|21|
|B:|21 43|21 43|
|C:|21 43 65|21 43 65|


2.
ex2.6

3510593的十进制和十六进制分别是 0x00359141 0x4A564504

A: 0x00359141
0000 0000 0011 0101 1001 0001 0100 0001

B: 0x4A564504
0100 1010 0101 0110 0100 0101 0000 0100

### 2.1.4 表示字符串

C语言中字符串被编码为一个以null(其值为0)字符结尾的字符数组。每个字符都由某个标准编码来表示。

    这个规则与字节顺序和字大小规则无关，文本数据比二进制数据具有更强的平台独立性

### 2.1.5 表示代码

不同的机器类型使用不同的且不兼容的指令方式和编码方式

### 2.1.6 布尔代数简介

| | |
|:-:|:-:|
|~(NOT)||
|0  |1  |
|1  |0  |


| | | |
|:-:|:-:|:-:|
|&(AND)|0|1|
|0|0|1|
|1|0|1|



| | | |
|:-:|:-:|:-:|
| \|(OR) |0|1|
|0|0|1|
|1|1|1|



| | | |
|:-:|:-:|:-:|
|^(EXCLUSIVE-OR)|0|1|
|0|0|1|
|1|1|0|


### 2.1.7 C语言中的位级运算



C语言中支持按位布尔运算。

`(a^b)^a = b`

--- 
练习
1. ex2.10
```c
void inplace_swap(int *x, int *y){
  *y = *x ^ *y; // step 1
  *x = *x ^ *y; // step 2
  *y = *x ^ *y; // step 3
}
```
`*x`和`*y*`的初始值分别为a b，试写出每一步中的这两个位置的值




||||
|:-:|:-:|:-:|
|steps|*x|*y|
| |a|b|
|s1|a|a^b|
|s2|a^(a^b)=b|a^b|
|s3|b|b^(a^b)=a|




2. ex2.11
只适用于偶数个元素数组的首位对调函数
```c
void reverse_array(int a[], int cnt){
  int first, last;
  for(first = 0, last = cnt - 1;
      first <= last;
      first++, last--){
    inplace_swap(&a[first], &a[last]);
  }
}
```
改动一下代码
```c
void reverse_array(int a[], int cnt){
  int first, last;
  for(first = 0, last = cnt - 1;
      first < last; // 去掉等号条件
      first++, last--){
    inplace_swap(&a[first], &a[last])
  }
}
```



>   位级运算的一个常见用法就是实现*掩码*运算， 这里的掩码是一个位模式，表示从一个字中选出的位集合

3. ex2.12 
对于下面的值，写出变量x的C语言表达式。 你的代码应该对任何字长w>=8都能工作。 
例如x = 0x87654321
 + x的最低有效字节，其他位均置0 [0x00000021]

    `x = x&0xFF`

 + 除了x的最低有效字节外，其他位置均取补，最低有效字节不变[0x789ABC21] 

```c
void fun1(int x){
  int m = ~(x^x) - x;
  int n = ~(x^x) - 0xFF;
  m = m&n;
  n = x&0xFF;
  m += n;
  show_int(m);
  return;
}
```


 + x的最低有效字节设置成全1，其他字节保持不变[0x876543FF]

```c
void fun2(int x){
  int m = ~(x^x) - 0xFF;
  int y = (x&m) + 0xff;
  show_int(y);
  return;
}
```


4. ex2_13

bis (位设置) 指令，提供数据字x和掩码字m，生成一个结果z，根据掩码m的位来修改x的位来得到z
bic (位清楚)

```c
int bis(int x, int m);
int bic(int x, int m);

/* Compute x|y only use calls to func bis and bic */
int bool_or(int x, int y){
  int result = bis(x, y); // 将y为1对应的x的位全部设置为1即可
  return result;
}

/* Compute x^y only use calls to func bis and bic */
int bool_or(int x, int y){
int bool_xor(int x, int y){
  int result = ;
  return result;
}
```

### 2.1.8 C语言中的逻辑运算

C语言还提供了一组逻辑运算符。(逻辑运算认为所有的非零参数都表示TRUE， 而参数0表示FALSE)

|||
|:-:|:-:|
|运算符|命题逻辑|
|\|\||OR|
|&&|AND|
|!|NOT|

> 要注意区分逻辑运算和位运算


--- 

1. ex2_14


2. ex2_15


### 2.1.9 C语言中的移位运算


`x << k`  x向左移动k位并丢弃最高的k位, 在右端补k个0

`x >> k`  **逻辑右移** x向右移动k位并丢弃最低的k位, 在左端补k个0 

`x >> k`  **算数右移** x向右移动k位并丢弃最低的k位, 在左端补k个最高有效位的值


    几乎所有的编译器/机器组合都对有符号数使用算数右移
    对于无符号数，右移必须是逻辑右移


> 在许多机器上，移位指令通常只考虑 `k mod ω` ，也就是位移量的低 `log₂ω` 位



## 2.2 整数表示

### 2.2.1 整数数据类型



### 2.2.2 无符号数的编码


![](https://pic.downk.cc/item/5fe7e67b3ffa7d37b399d0c7.gif)



?