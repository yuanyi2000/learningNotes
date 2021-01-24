# 第三章 程序的机器级表示

> 精通细节是理解更深和更基本概念的先决条件

## 3.1 历史观点

Intel 处理器系列俗称 x86，经历了一个长期的不断发展的过程。

## 3.2 程序编码

`linux $ gcc -Og -o p p1.c p2.c `

过程：

1. 首先 C*预处理器*扩展源代码，插入所有#include 命令指定的文件，并扩展所有 #define 声明指定的宏

2. 其次，*编译器*产生两个源文件的汇编代码，名字分别为 p1.s p2.s

3. 接下来，*汇编器*会将汇编代码转换为二进制*目标代码*文件 p1.o p2.o ，目标代码是机器代码的一种形式，它包含所有指令的二进制表示，但还没有填入全局地址。

4. 最后，*链接器*将两个目标代码文件与实现库函数(例如 printf())的代码合并，并产生最终的可执行代码 p

### 3.2.1 机器级代码

对于机器级编程来说，有两种抽象尤其重要

1. 指令集体系结构或指令集架构(Instruction Set Architecture, ISA)\*来定义机器级程序的格式和行为
2. 第二种是，机器级程序使用的内存地址是虚拟地址

x86-64 的机器代码中，一些对 C 程序员隐藏的处理器状态都是可见的

- _程序计数器(PC)_ 在 x86-64 中用 `%rip` 表示，给出将要执行的下一条指令在内存中的地址

- _整数寄存器文件_ 包含 16 个命名的位置，分别储存 64 位的值

- _条件码寄存器_ 保存着最近执行的算数或逻辑指令的状态信息

- 一组向量寄存器可以存放一个或多个整数或浮点数值

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

会得到 mstore.s 文件，其中包括下面几行

```assembly
multstore:
	pushq	%rbx
	movq	%rdx, %rbx
	call	mult2@PLT
	movq	%rax, (%rbx)
	popq	%rbx
	ret
```

如果使用 gcc 的`-c`选项，就会产生目标代码文件 mstore.o

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

1. ATT 与 Intel 汇编代码格式

gcc objdump 等工具默认使用 AT&T 格式的汇编代码，但可以用以下命令生成 Intel 格式的汇编代码

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

- Intel 代码省略了指示大小的后缀，例如 `push mov` 而不是 `pushq movq`
- Intel 代码省略了寄存器名字前面的 `%` , 例如 rbx 而不是 `%rbx`
- Intel 代码用不同的方式描述内存中的位置，例如`QWORD PTR [rbx]`而不是`(%rbx)`
- 在带有多个操作数的的指令情况下，列出的操作数的顺序相反

2. 把 C 程序和汇编代码结合起来

在 C 程序中插入汇编代码有两种方式

- 我们可以编写完整的函数，放进一个独立的汇编代码文件中，让汇编器和链接器把它和 C 程序文件合并起来

- 我们可以使用 gcc 的内联汇编特性，用`asm`伪指令可以在 C 程序中包含简短的汇编指令

[IBM Linux 中 x86 的内联汇编](https://www.ibm.com/developerworks/cn/linux/sdk/assemble/inline/index.html)

## 3.3 数据格式

    非常重要的表格

|         |                |              |            |          |
| :-----: | :------------: | :----------: | :--------: | :------: |
| C 声明  | Intel 数据类型 | 汇编代码后缀 | 大小(字节) | 大小(位) |
|  char   |      字节      |      b       |     1      |    8     |
|  short  |       字       |      w       |     2      |    16    |
|   int   |      双字      |      l       |     4      |    32    |
|  long   |      四字      |      q       |     8      |    64    |
| char \* |      四字      |      q       |     8      |    64    |
|  float  |     单精度     |      s       |     4      |    32    |
| double  |     双精度     |      l       |     8      |    64    |

> 后缀`l`既可以表示双字，也可以表示双精度浮点，这不会产生歧义，因为浮点数用的完全是另一组不同的指令和寄存器

## 3.4 访问信息

一个 x86-64 的中央处理单元(CPU)包含 16 个储存 64 位值的*通用目的寄存器*

|                  |            |        |       |              |
| :--------------- | :--------- | :----- | :---- | :----------: |
| 63............32 | 31......16 | 15...8 | 7...0 |     用途     |
| %rax             | %eax       | %ax    | %al   |    返回值    |
| %rbx             | %ebx       | %bx    | %bl   | 被调用者保存 |
| %rcx             | %ecx       | %cx    | %cl   | 第 4 个参数  |
| %rdx             | %edx       | %dx    | %dl   | 第 3 个参数  |
| %rsi             | %esi       | %si    | %sil  | 第 2 个参数  |
| %rdi             | %edi       | %di    | %dil  | 第 1 个参数  |
| %rbp             | %ebp       | %bp    | %bpl  | 被调用者保存 |
| %rsp             | %esp       | %sp    | %spl  |    栈指针    |
| %r8              | %r8d       | %r8w   | %r8b  | 第 5 个参数  |
| %r9              | %r9d       | %r9w   | %r9b  | 第 6 个参数  |
| %r10             | %r10d      | %r10w  | %r10b |  调用者保存  |
| %r11             | %r11d      | %r11w  | %r11b |  调用者保存  |
| %r12             | %r12d      | %r12w  | %r12b | 被调用者保存 |
| %r13             | %r13d      | %r13w  | %r13b | 被调用者保存 |
| %r14             | %r14d      | %r14w  | %r14b | 被调用者保存 |
| %r15             | %r15d      | %r15w  | %r15b | 被调用者保存 |

其中最特别的是栈指针`%rsp`, 用来指明运行时栈的结束位置

### 3.4.1 操作数指示符

大多数指令都有一个或多个*操作数*，操作数有三种存在的可能性：

1. _立即数(Imm, immediate)_， 用来表示常数值， 例如 `$0x1F $-517`
2. _寄存器_， 表示某个寄存器的内容， 例如 `R[r]`
   r 表示寄存器，R[r]表示寄存器的值，这是将寄存器看做一个数组 R，用寄存器标识符作为索引
3. *内存*引用， 根据计算出来的地址(_有效地址_)访问某个内存位置。 `M[Addr]`
   表示对存储在内存中从 Addr 开始的字节引用

_寻址模式_ `Imm(rb, ri, s)` 表示 `M[Imm + R[rb] + R[ri]×s]`, 这个引用有四个部分，分别为

- 立即数偏移 Imm
- 基址寄存器 rb
- 变址寄存器 ri
- 比例因子 s

> 操作数格式表

|  类型  | 格式         | 操作数值                 |     名称     |
| :----: | :----------- | :----------------------- | :----------: |
| 立即数 | $Imm         | Imm                      |  立即数寻址  |
| 寄存器 | ra           | R[ra]                    |  寄存器寻址  |
| 存储器 | Imm          | M[Imm]                   |   绝对寻址   |
| 存储器 | (ra)         | M[R[ra]]                 |   间接寻址   |
| 存储器 | Imm(rb,ri,s) | M[Imm + R[rb] + R[ri]×s] | 比例变址寻址 |

### 3.4.2 数据传送指令

1.  `MOV S, D`

        效果 S -> D ,传送

- 传送指令的两个操作数不能都指向内存位置。
- 另外，movl 指令以寄存器为目的时，他会把寄存器的高 4 位字节设置为 0

2.  `MOVZ S, R`

        较小源复制到较大目的时的*零扩展*

3.  `MOVS S, R`

        较小源复制到较大目的时的*符号扩展(最高位进行复制)*

    指令`cltq` `%eax`符号扩展到`%rax`

_理解数据传送如何改变目标寄存器_

```assembly
movabsq $0x0011223344556677, %rax		%rax = 0011223344556677
movb	$-1, %al						%rax = 00112233445566FF
movw	$-1, %ax						%rax = 001122334455FFFF
movl	$-1, %eax						%rax = 00000000FFFFFFFF
movq    $-1, %rax						%rax = FFFFFFFFFFFFFFFF

movabsq $0x0011223344556677, %rax		%rax = 0011223344556677
movb 	$0xAA, %dl						%dl  = AA
movb 	%dl, %al						%rax = 00112233445566AA
movsbq  %dl, %rax						%rax = FFFFFFFFFFFFFFAA
movzbq  %dl, %rax						%rax = 00000000000000AA
```

### 3.4.3 数据传送示例

```C
long exchange(long *xp, long y){
	long x = *xp;
	*xp = y;
	return x;
}
```

```assembly
long exchange(long *xp, long y)
xp in %rdi, y in %rsi
exchange:
	movq (%rdi), %rax	Get x at xp, Set as return value
	movq %rsi, (%rdi)	Store y at xp
	ret
```

---

关于上面代码的说明：

1. [指针](https://github.com/yuanyi2000/learningNotes/tree/master/Intermedia_C_Programming/Part1_%E8%AE%A1%E7%AE%97%E6%9C%BA%E5%82%A8%E5%AD%98%EF%BC%9A%E5%86%85%E5%AD%98%E5%92%8C%E6%96%87%E4%BB%B6#%E6%8C%87%E9%92%88)
   - `*iptr`在赋值号的右边
     1. 把 iptr 的值看作是一个地址
     2. 访问这个地址
     3. 读取这个地址处的值
     4. 把这个值赋值给左边
   - `*iptr`在赋值号左边
     1. 把 iptr 的值看作一个地址
     2. 访问这个地址处的值
     3. 把这个值修改为赋值号右边的值
2. 汇编代码

```assembly
	movq (%rdi), %rax
```

    	左边是一个取地址运算，相当于把%rdi看作一个地址，访问这个地址处的值并赋值给%rax
    	因此这相当于指针中*iptr在赋值号右边的情况，对应的也是下面的C代码

```C
	long x = *xp;
```

    	而对于

```assembly
	movq %rsi, (%rdi)
```

    	相当于把%rsi的值赋值给了把%rdi的值看作的一个地址处的值，因此相当于指针中
    	*iptr在赋值号左边的情况

```C
	*xp = y;
```

> 综上，对于汇编代码中的数据传送对应的 C 代码的问题，只要联系指针的概念即可写出相应的代码

> 形如`mov %rax, (%rdi)`的汇编代码对应 C 中的`*x = y` \
> 形如`mov (%rax), %rdi`的汇编代码对应 C 中的`x = *y`

---

这段代码有两点值得注意：

- C 语言中所谓的指针其实就是地址， 间接引用指针其实就是将该指针放在一个寄存器里， 然后内存引用使用这个寄存器
- 像 x 这样的局部变量通常保存在寄存器而不是内存中，因为访问寄存器比访问内存快得多

### 3.4.4 压入和弹出栈数据
