# 练习题

- ex3.5

写出对应的 C 代码：

```assembly
    void decode1(long *xp, long *yp, long *zp)
    xp in %rdi, yp in %rsi, zp in %rdx
decode1:
    movq (%rdi), %r8
    movq (%rsi), %rcx
    movq (%rdi), %rax
    movq %r8, (%rsi)
    movq %rcx, (%rdx)
    movq %rax, (%rdi)
    ret
```

见 `./code/decode1.c`

- ex 3.8

|  地址  |  值   |
| :----: | :---: |
| 0x100  | 0xFF  |
| 0x108  | 0xAB  |
| 0x110  | 0x13  |
| 0x118  | 0x11  |
|        |       |
| 寄存器 |  值   |
|  %rax  | 0x100 |
|  %rcx  |  0x1  |
|  %rdx  |  0x3  |

| 指令                     | 目的                               | 值            |
| :----------------------- | :--------------------------------- | :------------ |
| addq %rcx, (%rax)        | `%rax指向的地址处的值加上%rcx的值` | 0x100 <- 0x00 |
| subq %rdx, 8(%rax)       | `将%rax+8处的值减去%rdx的值`       | 0x108 <- 0xA8 |
| imulq $16, (%rax,%rdx,8) | `将%rax+8*%rdx处的值×16`           | 0x118 <- 0x10 |
| incq 16(%rax)            | `%rax+16地址处的值自加`            | 0x110 <- 0x14 |
| decq %rcx                | `%rcx自减`                         | %rcx <- 0x0   |
| subq %rdx, %rax          | `%rax存储%rax减去%rdx的值`         | %rax <- 0x0FC |

- ex 3.9 根据 C 代码写汇编

```C
long shift_left4_rightn(long x, long n)
{
    x <<= 4;
    x >>= n;
    return x;
}
```

```assembly
x in %rdi, n in %rsi
shift_left4_rightn:
    movq %rdi, %rax         Get x
    salq 4, %rax            x <<= 4
    movl %esi, %ecx         Get n
    sarq %ecx, %rax         x >>= n
    ret
```

- ex 3.20

```C
# define OP _____ /* Unkown operator */

long arith(long x)
{
    return x OP 8;
}
```

对应的汇编代码如下

```assembly
long arith(long x)
x in %rdi
arith:
    leaq 7(%rax), %rax
    testq %rdi, %rdi
    cmovns %rdi, %rax
    sarq $3, %rax
    ret
```

(A) OP 是什么操作？
(B) 解释代码每一步是怎么工作的

- ex 3.22

A 用一个 32 位 int 表示 n!, 最大的 n 是多少?

B 64 位呢？
