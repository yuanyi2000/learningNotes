# 缓冲区溢出攻击

## 知识

1. 程序的内存布局

由低端地址到高段地址依次为

- 代码段(text segment)
  存放程序的可执行代码
- 数据段(data segment)
  存放由程序员初始化的静态/全局变量
- BSS 段(BSS segment)
  存放未初始化的静态/全局变量
- 堆(Heap)
  用于动态内存分配
- 栈(stack)
  存放函数内定义的局部变量，或是和函数有关的数据，如返回地址和参数等

2. 栈的内存布局

栈的增长方向是由高端地址向低端地址增长，依次为

- 参数 b 的值
- 参数 a 的值
- 返回地址
  当函数结束并执行返回指令时，他需要知道返回地址。
  在调用一个函数之前，计算机把下一条指令的地址压入栈顶，这就是栈帧的返回地址区域
- 前帧指针
  下一个被程序压入栈帧中的数据是上一个栈帧的指针。
- 局部变量 x 的值
- 局部变量 y 的值

> 参数 ab 和变量 xy 只是为了说明顺序

![](https://upload.wikimedia.org/wikipedia/commons/d/d3/Call_stack_layout.svg)

3. 帧指针

   在一个函数中，需要访问参数和局部变量。访问他们的唯一方式是通过他们的内存地址
   然而这些地址在编译时还不能确定，因为编译器无法预测栈的运行时状态，也就无法得知栈帧的位置。
   为此，CPU 引入了一个专门的寄存器，叫做帧指针。(%ebp)
   这个寄存器指向栈帧中的一个固定地址。

_前帧指针和函数调用链_

    由于CPU中只存在一个帧指针寄存器，因此在一个函数内调用另一个函数时，
    需要在被调用函数中保存上一个函数的帧指针，然后将帧指针寄存器指向当前函数的帧指针

## 环境准备

1. 关闭*地址空间随机化*(address space layout randomization,ASLR)

```sh
$sudo sysctl -w kernel.randomize_va_space=0
```

2. 有漏洞的程序

```sh
$ gcc -o stack.out -z execstack -fno-stack-protector stack.c
$ sudo chown root stack.out
$ sudo chmod 4775 stack.out
```

- -z execstack 关闭不可执行栈(non-executable stack)保护机制
  return-to-libc 可以成功破解这个保护机制

- -fno-stack-protector 关闭 StackGuard 保护机制

```sh
yuan@ubuntu:~/Cap5$ python -c 'print "a"*20' > badfile
yuan@ubuntu:~/Cap5$ ./stack.out
Return Properly
yuan@ubuntu:~/Cap5$ python -c 'print "a"*120' > badfile
yuan@ubuntu:~/Cap5$ ./stack.out
段错误 (核心已转储)
yuan@ubuntu:~/Cap5$
```

## 寻找注入代码的内存地址

1. 验证栈的起始地址是否总是相同

```c
#include <stdio.h>
void func(int *a1)
{
	printf("::a1's address is 0x%x \n", (unsigned int) &a1);
}

int main(int argc, char const *argv[])
{
	int x = 3;
	func(&x);
	return 0;
}
```

```sh
yuan@ubuntu:~/Cap5$ sudo sysctl -w kernel.randomize_va_space=0
[sudo] yuan 的密码：
kernel.randomize_va_space = 0
yuan@ubuntu:~/Cap5$ gcc -o searchAddr.out searchAddr.c
yuan@ubuntu:~/Cap5$ ./searchAddr.out
::a1's address is 0xffffdff8
yuan@ubuntu:~/Cap5$ ./searchAddr.out
::a1's address is 0xffffdff8
yuan@ubuntu:~/Cap5$ ./searchAddr.out
::a1's address is 0xffffdff8
yuan@ubuntu:~/Cap5$ ./searchAddr.out
::a1's address is 0xffffdff8
```

2. 提高猜测成功率

   在新的返回地址和恶意代码之间填充`NOP`指令

3. 通过调试程序找到地址

> 攻击者和目标程序在同一计算机中
