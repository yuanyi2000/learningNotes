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

