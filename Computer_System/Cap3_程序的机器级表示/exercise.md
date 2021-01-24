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
