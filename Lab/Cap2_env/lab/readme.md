# SEED Labs  - Enviroment Variable and *Set-UID* Program Lab

## 1 Overview 

## 2 Lab Tasks

#### 2.1 Task1 Manipulating Enviroment Variables

+ 使用`printenv`或`env`访问环境变量

```sh
yuan@ubuntu:~/learningNotes/Lab/Cap2_env/lab$ printenv PWD
/home/yuan/learningNotes/Lab/Cap2_env/lab
yuan@ubuntu:~/learningNotes/Lab/Cap2_env/lab$ env | grep PWD
OLDPWD=/home/yuan
PWD=/home/yuan/learningNotes/Lab/Cap2_env/lab
```
+ 使用`export`和`unset`来添加或删除一个环境变量

#### 2.2 Task2 向子进程传递环境变量

编译并运行task2.c 第一次编译时将Line22注释掉，输出子进程的所有环境变量； 第二次编译时将Line19注释掉，输出父进程的环境变量。

用`diff`对比二者

```sh
yuan@ubuntu:~/learningNotes/Lab/Cap2_env/lab$ gcc -o a.out task2.c 
yuan@ubuntu:~/learningNotes/Lab/Cap2_env/lab$ ./a.out > child 
yuan@ubuntu:~/learningNotes/Lab/Cap2_env/lab$ gcc -o a.out task2.c 
yuan@ubuntu:~/learningNotes/Lab/Cap2_env/lab$ ./a.out > another 
yuan@ubuntu:~/learningNotes/Lab/Cap2_env/lab$ diff -s child another 
檔案 child 和 another 相同
```

发现二者没有差别。

	结论：使用fork()函数，父进程将所有环境变量传递给了子进程