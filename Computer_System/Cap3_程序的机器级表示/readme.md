# 第三章 程序的机器级表示

> 精通细节是理解更深和更基本概念的先决条件

## 3.1 历史观点

Intel处理器系列俗称x86，经历了一个长期的不断发展的过程。


## 3.2 程序编码


`linux $ gcc -Og -o p p1.c p2.c `


过程： 

1. 首先C*预处理器*扩展源代码，插入所有#include命令指定的文件，并扩展所有 #define 声明指定的宏

2. 其次，*编译器*产生两个源文件的汇编代码，名字分别为p1.s p2.s

3. 接下来，*汇编器*会将汇编代码转换为二进制*目标代码*文件 p1.o p2.o ，目标代码是机器代码的一种形式，它包含所有指令的二进制表示，但还没有填入全局地址。

4. 最后，*链接器*将两个目标代码文件与实现库函数(例如printf())的代码合并，并产生最终的可执行代码p


### 3.2.1 机器级代码

对于机器级编程来说，有两种抽象尤其重要


1. 指令集体系结构或指令集架构(Instruction Set Architecture, ISA)*来定义机器级程序的格式和行为
2. 第二种是，机器级程序使用的内存地址是虚拟地址



x86-64的机器代码中，一些对C程序员隐藏的处理器状态都是可见的

+ *程序计数器(PC)* 在x86-64中用 `%rip` 表示，给出将要执行的下一条指令在内存中的地址

+ *整数寄存器文件* 包含16个命名的位置，分别储存64位的值

+ *条件码寄存器* 保存着最近执行的算数或逻辑指令的状态信息

+ 一组向量寄存器可以存放一个或多个整数或浮点数值


### 3.2.2 代码示例

例如我们写一段代码

```c
long mult2(long, long);

void multstore(long x,long y, long *dest){
    long t = mult2(x, y);
    *dest = t;
}
```

```sh
linux$ gcc -Og -S mstore.c 
```

会得到mstore.s文件，其中包括下面几行

```assembly
multstore:
	pushq	%rbx
	movq	%rdx, %rbx
	call	mult2@PLT
	movq	%rax, (%rbx)
	popq	%rbx
	ret
```

如果使用gcc的`-c`选项，就会产生目标代码文件mstore.o

```sh
linux$ gcc -Og -c mstore.c 
```

> 要查看机器代码文件的内容。就要用到*反汇编器*

```sh
linux$ objdump -d mstore.o 
```

会得到下面的输出
```assembly
mstore.o：     文件格式 elf64-x86-64


Disassembly of section .text:

0000000000000000 <multstore>:
   0:	53                   	push   %rbx
   1:	48 89 d3             	mov    %rdx,%rbx
   4:	e8 00 00 00 00       	callq  9 <multstore+0x9>
   9:	48 89 03             	mov    %rax,(%rbx)
   c:	5b                   	pop    %rbx
   d:	c3                   	retq   
```

### 3.2.3 关于格式的注解

```assemble
multstore:
.LFB0:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdx, %rbx
	call	mult2@PLT
	movq	%rax, (%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
```

其中以`.`开头的行都是指导汇编器和链接器工作的伪指令


旁注：
1. ATT与Intel汇编代码格式

gcc objdump等工具默认使用AT&T格式的汇编代码，但可以用以下命令生成Intel格式的汇编代码

```sh
linux$ gcc -Og -S -masm=intel mstore.c
```

```intel 
multstore:
	push rbx
	mov  rbx, rdx
	call mult2 
	mov  QWORD PTR [rbx], rax
	pop  rbx
	ret 
```

+ Intel代码省略了指示大小的后缀，例如 `push mov` 而不是 `pushq movq` 
+ Intel代码省略了寄存器名字前面的 `%` , 例如 rbx 而不是 `%rbx` 
+ Intel代码用不同的方式描述内存中的位置，例如`QWORD PTR [rbx]`而不是`(%rbx)`
+ 在带有多个操作数的的指令情况下，列出的操作数的顺序相反

2. 把C程序和汇编代码结合起来

在C程序中插入汇编代码有两种方式

 + 我们可以编写完整的函数，放进一个独立的汇编代码文件中，让汇编器和链接器把它和C程序文件合并起来

 + 我们可以使用gcc的内联汇编特性，用`asm`伪指令可以在C程序中包含简短的汇编指令

[IBM Linux中x86的内联汇编](https://www.ibm.com/developerworks/cn/linux/sdk/assemble/inline/index.html)



## 3.3 数据格式



	非常重要的表格


||||||
|:-:|:-:|:-:|:-:|:-:|
|C声明|Intel数据类型|汇编代码后缀|大小(字节)|大小(位)|
|char|字节|b|1|8|
|short|字|w|2|16|
|int|双字|l|4|32|
|long|四字|q|8|64|
|char *|四字|q|8|64|
|float|单精度|s|4|32|
|double|双精度|l|8|64|


> 后缀`l`既可以表示双字，也可以表示双精度浮点，这不会产生歧义，因为浮点数用的完全是另一组不同的指令和寄存器


## 3.4 访问信息

一个x86-64的中央处理单元(CPU)包含16个储存64位值的*通用目的寄存器*



||||||
|:-|:-|:-|:-|:--:|
|63............32|31......16|15...8|7...0|用途|
|%rax|%eax|%ax|%al|返回值|
|%rbx|%ebx|%bx|%bl|被调用者保存|
|%rcx|%ecx|%cx|%cl|第4个参数|
|%rdx|%edx|%dx|%dl|第3个参数|
|%rsi|%esi|%si|%sil|第2个参数|
|%rdi|%edi|%di|%dil|第1个参数|
|%rbp|%ebp|%bp|%bpl|被调用者保存|
|%rsp|%esp|%sp|%spl|栈指针|
|%r8|%r8d|%r8w|%r8b|第5个参数|
|%r9|%r9d|%r9w|%r9b|第6个参数|
|%r10|%r10d|%r10w|%r10b|调用者保存|
|%r11|%r11d|%r11w|%r11b|调用者保存|
|%r12|%r12d|%r12w|%r12b|被调用者保存|
|%r13|%r13d|%r13w|%r13b|被调用者保存|
|%r14|%r14d|%r14w|%r14b|被调用者保存|
|%r15|%r15d|%r15w|%r15b|被调用者保存|

其中最特别的时栈指针`%rsp`, 用来指明运行时栈的结束位置


### 3.4.1 操作数指示符

大多数指令都有一个或多个*操作数*，操作数有三种存在的可能性：

1. *立即数(Imm, immediate)*， 用来表示常数值， 例如 `$0x1F $-517`
2. *寄存器*， 表示某个寄存器的内容， 例如 `R[r]`
	r表示寄存器，R[r]表示寄存器的值，这是将寄存器看做一个数组R，用寄存器标识符作为索引
3. *内存*引用， 根据计算出来的地址(*有效地址*)访问某个内存位置。 `M[Addr]` 
	表示对存储在内存中从Addr开始的字节引用



*寻址模式* 	`Imm(rb, ri, s)` 表示 `M[Imm + R[rb] + R[ri]×s]`, 这个引用有四个部分，分别为 
+ 立即数偏移 Imm 
+ 基址寄存器 rb 
+ 变址寄存器 ri 
+ 比例因子   s




>  操作数格式表

||||
|:-:|:-:|:-:|:-:|
|类型|格式|操作数值|名称|
|:-:||:--|:--|:-:|
|立即数|$Imm|Imm|立即数寻址|
|寄存器|ra|R[ra]|寄存器寻址|
|存储器|Imm|M[Imm]|绝对寻址|
|存储器|(ra)|M[R[ra]]|间接寻址|
|存储器|Imm(rb,ri,s)|M[Imm + R[rb] + R[ri]×s]|比例变址寻址|


### 3.4.2 数据传送指令

1. MOV S, D 
	
		效果 S -> D ,传送

传送指令的两个操作数不能都指向内存位置。
另外，movl指令以寄存器为目的时，他会把寄存器的高4位字节设置为0