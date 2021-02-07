# 第三章 程序的机器级表示

> 精通细节是理解更深和更基本概念的先决条件

## 3.1 历史观点

Intel 处理器系列俗称 x86，经历了一个长期的不断发展的过程。

> X86 是由 Intel 推出的一种复杂指令集,用于控制芯片的运行的程序

[x86 架构 百度百科](https://baike.baidu.com/item/X86%E6%9E%B6%E6%9E%84/7470217?fr=aladdin)

## 3.2 程序编码

`linux $ gcc -Og -o p p1.c p2.c `

过程：

1. 首先 C*预处理器*扩展源代码，插入所有#include 命令指定的文件，并扩展所有 #define 声明指定的宏

2. 其次，*编译器*产生两个源文件的汇编代码，名字分别为 p1.s p2.s

3. 接下来，*汇编器*会将汇编代码转换为二进制*目标代码*文件 p1.o p2.o ，目标代码是机器代码的一种形式，它包含所有指令的二进制表示，但还没有填入全局地址。

4. 最后，*链接器*将两个目标代码文件与实现库函数(例如 printf())的代码合并，并产生最终的可执行代码 p

### 3.2.1 机器级代码

对于机器级编程来说，有两种抽象尤其重要

1.  指令集体系结构或指令集架构(Instruction Set Architecture, ISA)来定义机器级程序的格式和行为

        ISA定义了一台计算机可以执行的所有指令的集合，每条指令规定了计算机执行什么操作，所处理的操作数存放的地址空间以及操作数类型。ISA规定的内容包括数据类型及格式，指令格式，寻址方式和可访问地址空间的大小，程序可访问的寄存器个数、位数和编号，控制寄存器的定义，I/O空间的编制方式，中断结构，机器工作状态的定义和切换，输入输出结构和数据传送方式，存储保护方式等。因此，可以看出，指令集体系结构是指软件能够感知到的部分，也称软件可见部分。

2.  第二种是，机器级程序使用的内存地址是虚拟地址

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

    	pushq %rbx 的作用是保存%rbx的值，因为接下来mulstore函数要用到%rbx，但其中可能有main使用到的值

    	movq %rdx, %rbx 将dest的值放进%rbx保存起来，因为调用mult2之后还要用到dest的值，调用之前应该保存到被调用者保存寄存器中

    	也就是说对于一个过程，在调用其他过程时需要用到被调用者保存寄存器时，首先要将其寄存器的值保存起来才能用

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

> 一个字节是两个 16 进制数，也就是八个 2 进制数

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
3. *内存*引用， 根据计算出来的地址(_有效地址_)访问某个内存位置。
   `M[Addr]`表示对存储在内存中从 Addr 开始的字节引用

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
movabsq $0x0011223344556677, %rax			%rax = 0011223344556677
movb	$-1, %al							%rax = 00112233445566FF
movw	$-1, %ax							%rax = 001122334455FFFF
movl	$-1, %eax							%rax = 00000000FFFFFFFF
movq    $-1, %rax							%rax = FFFFFFFFFFFFFFFF

movabsq $0x0011223344556677, %rax			%rax = 0011223344556677
movb 	$0xAA, %dl							%dl  = AA
movb 	%dl, %al							%rax = 00112233445566AA
movsbq  %dl, %rax							%rax = FFFFFFFFFFFFFFAA
movzbq  %dl, %rax							%rax = 00000000000000AA
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

栈指针`%rsp`保存着栈顶元素的地址

- `pushq S`

  R[%rsp] <- R[%rsp]-8;
  M[ R[%rsp] ] <- S
  将四字压入栈

  (R[r]表示寄存器的值，M[Addr]表示对储存在 Addr 处的一段字节值的引用)

- `popq D`

  D <- M[ R[%rsp] ];
  R[%rsp] <- R[%rsp]+8

一般来说，栈向下增长(增长方向是地址减小的方向)，具体可以 Google 一下

## 3.5 算数和逻辑操作

算数和逻辑操作被分成四组，分别为

- 加载有效地址

|   指令    |  操作   |     描述     |
| :-------: | :-----: | :----------: |
| leaq S, D | D <- &S | 加载有效地址 |

- 一元操作

| 指令  |  操作   | 描述 |
| :---: | :-----: | :--: |
| INC D |   D++   |  +1  |
| DEC D |   D--   |  -1  |
| NEG D | D <- -D | 取负 |
| NOT D | D <- ~D | 取补 |

- 二元操作

|   指令    |    操作     | 描述 |
| :-------: | :---------: | :--: |
| ADD S, D  | D <- D + S  |  加  |
| SUB S, D  | D <- D - S  |  减  |
| IMUL S, D | D <- D \* S |  乘  |
| XOR S, D  | D <- D ^ S  | 异或 |
|  OR S, D  | D <- D \| S |  或  |
| AND S, D  | D <- D & S  |  与  |

- 移位

|   指令   |      操作      |   描述   |
| :------: | :------------: | :------: |
| SAL k, D |  D <- D << k   |   左移   |
| SHL k, D |  D <- D << k   |   左移   |
| SAR k, D | D -> D >>(A) k | 算数右移 |
| SHR k, D | D -> D >>(L) k | 逻辑右移 |

### 3.5.1 加载有效地址

- 编译器经常发现 leaq 有一些与有效地址计算根本无关的灵活用法 (其目的操作数必须为寄存器)

```C
long scale(long x, long y, long z)
{
	long t = x + 4 * y + 12 * z;
	return t;
}
```

```assembly
long scale(long x, long y, long z)
x in %rdi, y in %rsi, z in %rdx

scale:
	leaq (%rdi, %rsi, 4), %rax		x + 4*y
	leaq (%rdx, %rdx, 2), %rdx		z + 2*z = 3*z
	leaq (%rax, %rdx, 4), %rax		(x+4*y) + 4*(3*z) = x + 4*y + 12*z
	ret
```

### 3.5.2 一元和二元操作

- 一元操作

  - 该操作数既可以是内存位置也可以是寄存器
  - 类似 C 语言中的 ++ -- 运算符

- 二元操作

  - 源操作数是第一个，目的操作数是第二个
  - 源操作数可以是内存、立即数或寄存器， 目的操作数可以是内存位置或寄存器

> 当第二个操作数为内存地址时，处理器必须从内存中读出值，执行操作，再把值写回

### 3.5.3 移位操作

    	算数移位填充符号位
    	而逻辑移位填充0

### 3.5.4 讨论

### 3.5.5 特殊的算术操作

| 指令    | 效果                                                             | 描述         |
| :------ | :--------------------------------------------------------------- | :----------- |
| imulq S | R[%rdx]: R[%rax] <- S \* R[%rax]                                 | 有符号全乘法 |
| mulq S  | R[%rdx]: R[%rax] <- S \* R[%rax]                                 | 无符号全乘法 |
|         |                                                                  |              |
| clto    | R[%rdx]: R[%rax] <- 符号扩展(R[%rax])                            | 转换为八字   |
|         |                                                                  |              |
| idivq S | R[%rdx] <- R[%rdx]:R[%rax] mod S, R[%rdx] <- R[%rdx]:R[%rax] / S | 有符号除法   |
| divq S  | R[%rdx] <- R[%rdx]:R[%rax] mod S, R[%rdx] <- R[%rdx]:R[%rax] / S | 无符号除法   |
|         |                                                                  |              |
| cqto    | 将%rdx 的位设置为全 0(无符号运算)或%rax 的符号位(有符号运算)     | 置零         |

- 乘法运算的 C 代码和汇编代码示例

```C
#include <inttypes.h>

typedef unsigned __int128 uint128_t;

void store_uprod(uint128_t *dest, uint64_t x, uint64_t y)
{
	*dest = x * (uint128_t) y;
}
```

```assembly
void store_uprod(uint128_t *dest, uint64_t x, uint64_t y)
dest in %rdi, x in %rsi, y in %rdx
store_uprod:
	movq %rsi, %rax		Copy x to multiplicand
	mulq %rdx			Multiply by y
	movq %rax, (%rdi)	Store lower 8 bytes at dest
	movq %rdx, 8(%rdi)	Store upper 8 bytes at dest + 8
	ret
```

- 除法运算的 C 代码和汇编代码示例

```C
void remdiv(long x, long y, long *qp, long *rp)
{
	long q = x / y;
	long r = x % y;
	*qp = q;
	*rp = r;
}
```

```assembly
void remdiv(long x, long y, long *qp, long *rp)
x in %rdi, y in %rsi, qp in %rdx, rp in %rcx
remdiv:
	movq %rdx, %r8		Copy qp
	movq %rdi, %rax		Move x to lower 8 bytes of dividend
	cqto				Sign-extend to upper 8 bytes of dividend
	idivq %rsi			Divided by y
	movq %rax, (%r8)	Store quotient at qp
	movq %rdx, (%rcx)	Store remainder at rp
	ret
```

## 3.6 控制

### 3.6.1 条件码

*条件码寄存器(condition code register)*描述了最近操作的算术或逻辑操作的属性

- CF 进位标志 最近的操作使高位产生了进位，用来检查无符号操作的溢出
- ZF 零标志 最近的操作结果为 0
- SF 符号标志 最近的操作结果为负数
- OF 溢出标志 最近的操作导致一个补码溢出--正溢出或负溢出

      leaq不改变任何条件码
      对于逻辑操作，CF和OF会设置为0
      INC和DEC会设值OF和ZF，但不会改变CF

比较和测试指令，这些指令只改变条件码

| 指令        |  基于   | 描述 |
| :---------- | :-----: | :--- |
| CMP S1, S2  | S2 - S1 | 比较 |
| TEST S1, S2 | S1 & S2 | 测试 |

    	CMP  指令与 SUB 的行为是一样的
    	TEST 指令与 AND 的行为是一样的
    	但是他们都不会改变除了条件码以外的寄存器的值

### 3.6.2 访问条件码

条件码不会直接读取，常用的访问方式有三种

1. 可以根据条件码的某种组合，将一个字节设置为 0 或 1
2. 可以条件跳转到程序的某个部分
3. 可以有条件的传送数据

- SET 指令

      一条SET指令的目的操作数是低位单字节寄存器元素之一 ，
      或者是一个字节的内存位置，指令会将其设置为0 或 1

| 指令    | 效果              | 设置条件 |
| :------ | :---------------- | :------- |
| sete D  | D <- ZF           | ==       |
| setne   | D <- ~ZF          | !=       |
|         |                   |          |
| sets D  | D <- SF           | <0       |
| setns D |                   |          |
|         |                   |          |
| setg D  | D <- ~(SF^OF)&~ZF | > 有符号 |
| setge D |                   |
| setl D  |                   |
| setle D |                   |
|         |                   |          |
| seta D  |                   | > 无符号 |
| setae D |                   |
| setb D  |                   |
| setbe D |                   |

> 详情见 《深入理解计算机系统》 P138

### 3.6.3 跳转指令

跳转指令分为以下

- jmp

  jmp Label
  jmp \*Operand

- je jne

  是否相等

- js jns

  是否为负数

- jg jge jl jle

  有符号数

- ja jae jb jbe

  无符号数

### 3.6.4 跳转指令的编码

    	理解跳转指令的编码问题，对研究链接非常重要

跳转指令的编码方式

1.  PC-relative

        将目标指令的地址与紧跟在跳转指令后面那条指令的地址之间的差作为编码

2.  绝对地址编码

        用四个字节直接指定目标

> `rep; ret`的组合可以使代码再 AMD 上运行的更快，因为它避免了 ret 指令成为条件跳转指令的目标

理解 PC 相对寻址：

假设有一段汇编代码

```assembly
ADDR1	eb XX 		jmp xxxxx
ADDR2   48 d1 f8	xxxxxxxxx
```

    	对于ADDR1处的jmp指令，想确定他要跳转到的指令，需要先找到下一条指令的地址
    	也就是ADDR2，之后，要跳转的目标指令的地址即是
    	ADDR2 + XX
    	（XX可以是补码负数）

### 3.6.5 | 3.6.6 实现条件分支

1. 条件控制

2. 条件传送

用条件赋值实现计算两个数差的绝对值

```C
long cmovdiff(long x, long y)
{
	long rval = x - y;
	long eval = y - x;
	long ntest = x >= y;

	if(ntest) rval = eval;
	return eval;
}
```

```assembly
x in %rdi, y in %rsi
absdiff:
	movq %rsi, %rax
	subq %rdi, %rax		rval = y - x
	movq %rdi, %rdx
	subq %rsi, %rdx		eval = x - y
	cmpq %rsi, %rdi
	cmovge %rdx, %rax
	ret
```

    	处理器通过使用流水线来获得高性能
    	在流水线中，一条指令的处理要经过多阶段，例如
    	从内存取指令，确定指令的类型，从内存读取数据，执行算数运算，向内存写数据，更新程序计数器
    	这种方法通过重叠连续指令的步骤来获得高性能
    	例如，在取下一条指令的同时执行上一条指令的算数运算。

    	当遇到条件跳转时，只有当分支条件求值完成后，才会决定分支往那边走
    	处理器采用非常精密的分支预测逻辑来猜测每条跳转指令是否会执行
    	而一旦预测错误，会导致很严重的惩罚，导致程序性能下降

条件传送指令：当条件满足时，将源值 S 复制到目的 R

`CMOVX S, R`

    	X 是e(相等)， ne(不等)， s(负数) ...

### 3.6.7 循环

1. do-while

```
loop:
	body-statement
	t = test-expr;
	if (t)
		goto loop;
```

## 3.7 过程

    	过程的形式多样，函数、方法、子例程、处理函数等，但是他们有一些共有的特性

- 传递控制

- 传递数据

- 分配和释放内存

### 3.7.1 运行时栈

当 x86-64 过程需要的存储空间超过了寄存器能够存放的大小的时候，就会在栈上分配空间。这个部分称为过程的*栈帧*。

[![yi7v0U.png](https://s3.ax1x.com/2021/01/29/yi7v0U.png)](https://imgchr.com/i/yi7v0U)

    	当前正在执行的过程的帧总是在栈顶

    	大多数过程的帧都是定长的，在过程开始前就已经分配好了

### 3.7.2 转移控制

    	在由过程P进入过程Q的时候，程序计数器必须设置为Q的代码的起始位置，然后在返回时，
    	要把程序计数器设置为P中调用Q后面那条指令

例如有以下反汇编代码

```assembly
1	0000000000400540 <multstore>:
2	  400540:  53					push %rbx
3	  400541:  48 89 d3				mov  %rdx, %rbx
4	  40054d:  c3					retq

5	  400563:  e8 d8 ff ff ff 		callq 400540 <multstore>
6	  400568:  48 8b 54 24 08		mov  0x8(%rsp), %rdx
```

运行时栈寄存器和程序计数器的状态：

1. 执行 call

```
%rip	0x400563
%rsp	0x7fffffffe840
```

2. call 执行之后

```
%rip	0x400540
%rsp	0x7fffffffe838
```

3. ret 执行之后

```
%rip	0x400568
%rsp	0x7fffffffe840
```

### 3.7.3 数据传送

    	可以通过寄存器最多传递 6 个整型参数
    	寄存器的使用顺序为：%rdi %rsi %rdx %rcx %r8 %r9
    	当传递的位数是 32 位时使用%edi %esi....16 位时是%di %si....8 位则为%dil %sil...

    	超出6个的部分要用栈来传递，第七个参数位于栈顶
    	参数到位之后，才调用call指令传递控制

    	当被调用的过程友调用了新的超过6个参数的函数时
    	他也需要在自己的栈帧中为超出6个的部分分配空间
    	即为栈帧中的参数构造区

来看一个示例

```C
void proc(long a1, long *a1p,
		  int a2, int *a2p,
		  short a3, short *a3p,
		  char a4, char *a4p)
{
	*a1p += a1;
	*a2p += a2;
	*a3p += a3;
	*a4p += a4;
}
```

```assembly
void proc(a1, a1p, a2, a2p, a3, a3p, a4, a4p)
a1	in %rdi		(64bit)
a1p in %rsi		(64bit)
a2 	in %edx		(32bit)
a2p in %rcx		(64bit)
a3	in %r8w		(16bit)
a3p	in %r9		(64bit)
a4	in %rsp+8	( 8bit)
a4p in %rsp+16	(64bit)
proc:
	movq	16(%rsp), %rax		Fetch a4p
	addq	%rdi, %rsi			*a1p += a1
	addl 	%edx, (%rcx)		*a2p += a2
	addw	%r8w, (%r9)			*a3p += a3
	movl	8(%rsp), %edx		Fetch a4
	addb	%dl, (%rax)			*a4p += a4
	ret
```

    	a4 a4p 的值不能直接使用，而是应该放到寄存器里使用

### 3.7.4 栈上的局部储存

有些时候，局部数据必须放在内存中：

- 寄存器不足够存放所有的本地数据
- 对一个局部变量使用取地址运算符&， 因此必须能够为他产生一个地址
- 某些局部变量是数组或结构， 因此必须能够通过数组或结构引用被访问到

示例：

```C
long swap_add(long *xp, long *yp)
{
	long x = *xp;
	long y = *yp;
	*xp = y;
	*yp = x;
	return x + y;
}
long caller()
{
	long arg1 = 534;
	long arg2 = 1057;
	long sum = swap_add(&arg1, &arg2);
	long diff = arg1 - arg2;
	return sum * diff;
}
```

```assembly
long caller()
caller:
	subq 	$16, %rsp		Allocate 16 bytes for stack frame
	movq 	$534, (%rsp)
	movq	$1057, 8(%rsp)
	leaq	8(%rsp), %rsi	Compute &arg2 as second argument
	movq	%rsp, %rdi		Compute &arg1 as first  argument
	call 	swap_add
	movq	(%rsp), %rdx
	subq	8(%rsp), %rdx
	imulq	%rdx, %rax
	addq 	$16, %rsp		Deallocate stack frame
	ret
```

---

示例 2， 调用#3.7.3 中 proc 的代码示例，该代码创建了一个栈帧

```C
long call_proc()
{
	long x1 = 1; int x2 = 2;
	short x3 = 3; char x4 = 4;
	proc(x1, &x1, x2, &x2, x3, &x3, x4, &x4);
	return (x1+x2)*(x3+x4);
}
```

```assembly
long call_proc()
call_proc:
	set up argument to proc
	subq	$32, %rsp				Allocate 32 bytes stack frame
	movq 	$1, 24(%rsp)			store 1 in &x1
	movl	$2, 20(%rsp)			store 2 in &x2
	movw	$3, 18(%rsp)			store 3 in &x3
	movb	$4, 17(%rsp)			store 4 in &x4
	leaq	17(%rsp), %rax			create &x4
	movq	%rax, 8(%rsp)			store &x4 as arg8
	movl	$4, (%rsp)				store 4 as arg7
	leaq	18(%rsp), %r9			pass &x3 as arg6
	movl	$3, %r8d
	leaq	20(%rsp), %rcx
	movl	$2, %edx
	leaq	24(%rsp), %rsi
	movl	$1, %edi

	call	proc

	movslq	20(%rsp), %rdx
	addq	24(%rsp), %rdx
	movswl	18(%rsp), %eax
	movsbl	17(%rsp), %ecx
	subl	%ecx, %eax
	cltq
	imulq	%rdx, %rax
	addq	$32, %rsp
	ret
```

### 3.7.5 寄存器的局部存储空间

    	寄存器是唯一被所有过程共享的资源

    	x86-64采用了一组统一的寄存器使用惯例来保证被调用者不会覆盖到调用者稍后会使用的寄存器

    	根据惯例，寄存器 %rbx,%rbp,%r12~%r15被划分为被调用者保存寄存器
    	当过程P调用过程Q时，Q必须保存这些寄存器的值，保证他们的值在Q返回P时和P调用Q时是一样的
    	Q保存一个寄存器的值不变，要么不去改变它，要么把原始值压入栈中，在返回前再弹出旧值

    	所有其他寄存器，除了%rsp，都分类为调用者保存寄存器，这意味着任何函数都能修改他们
    	如果过程P在这类寄存器中有局部数据，那么调用过程Q之前保存好他是P的责任

示例：

```C
long P(long x, long y)
{
	long u = Q(y);
	long v = Q(x);
	return u + x;
}
```

```assembly
long P(x, y)
x in %rdi, y in %rsi
P:
	pushq 	%rbp
	pushq 	%rbx
	subq	$8, %rsp		Allign stack frame
	movq	%rdi, %rbp		Save x
	movq	%rsi, %rdi		Move y to first argument
	call 	Q
	movq	%rax, %rbx		Save resault
	movq	%rbp, %rdi		Move x to first argument
	call 	Q
	addq	%rbp, %rax
	addq	$8, %rsp		Deallocate last part of stack
	popq	%rbx
	popq	%rbp
	ret
```

> 第一次调用中，必须保存 x 的值，第二次要保存 Q(y)的值
> P 要使用 %rbp 和 %rbx， 但其中有可能有 mian 函数的数据因此必须先将他们的值压入栈中才能使用
> P 的最后还要使用 pop 指令将他们恢复

### 3.6.7 递归过程

    	寄存器和栈的惯例使得x86-64过程可以递归的调用他们自身
    	每个过程调用在栈中有他们独有的空间，因此多个未完成的调用的局部变量不会相互影响
    	此外栈适当的策略是过程调用时分配局部存储，返回时释放存储

阶乘递归的代码

```C
int rfact(long n)
{
	long result;
	if(n <= 1)
		result = 1;
	else
		result = n * rfact(n-1);
	return result;
}
```

```assembly
long rfact(long)
n in %rdi
rfact:
	pushq	%rbx
	movq	%rdi, %rbx
	movl	$1, %eax
	cmpq	$1,	%rdi
	jle	.L35
	leaq	-1(%rdi), %rdi
	call	rfact
	imulq	%rbx, %rax
.L35:
	popq	%rbx
	ret
```

## 3.8 数组的分配和访问

对于数据类型 T 和整型常数 N，声明如下：

`T A[N]`

起始位置表示为 xA

    	这个声明有两个效果
    	首先，在内存中分配了 sizeof(T)*N 字节的连续区域
    	其次，引入了标识符A，可以用A来作为指向数组开头的指针(xA)

|    声明     | 元素大小 | 总的大小 | 起始地址 | 元素 i |
| :---------: | :------: | :------: | :------: | :----: |
| char A[12]  |    1     |    12    |    xA    |  xA+i  |
| char \*B[8] |    8     |    64    |    xB    |  xB+i  |

### 3.8.2 指针运算
