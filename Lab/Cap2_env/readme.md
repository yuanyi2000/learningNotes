# 第二章实验

## 实验1 介绍环境变量

environ.c 程序可以打印进程中所有的环境变量


## 通过动态连接进行的攻击

mytest.c 是一个调用了 sleep()函数的程序，但可通过将sleep.c编译成动态链接库，并export到LD_PRELOAD从而使mytest程序不能正常运行


	使mytest成为Set-UID程序之后，会发现在保护机制的保护下仍可正常运行


