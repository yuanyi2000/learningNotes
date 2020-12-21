# 缓冲区溢出攻击


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

+ -z execstack 关闭不可执行栈(non-executable stack)保护机制
	return-to-libc可以成功破解这个保护机制

+ -fno-stack-protector 关闭StackGuard保护机制

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




