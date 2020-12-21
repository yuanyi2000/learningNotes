# Linux 下C语言开发环境 #
## 1.1 Linux操作系统简介 ##
### 1.1.1 linux操作系统简介 ###

	作者是Linus Torvalds 

### 1.1.2 GNU/linux 简介 ###

### 1.1.3 相关术语 ###

1. POSIX及其重要地位

	*POSIX表示可移植操作系统接口( Portable Operating System Interface)*

2. GNU和linux的关系

	Linux中使用了许多的GNU工具

3. 通用公共许可证(GPL ,Generial Public License)

	GPL的文本通常保存在COPYING文件里

4. LGPL GNU程序库通用公共许可证

## Linux开发初步 ##

### 1.2.1 Linux下C程序标准库 ###

	在Linux操作系统下进行C程序开发的标准主要有两个：ANSI C标准 和 POSIX标准

1. ANSIC 的目的是为各种操作系统上的C程序提供可移植性保证

	ISO标准库的头文件如下表所示

|头文件|说明|头文件|说明|
|:-|:-|:-|:-|
|<assert.h>|验证程序断言|<signal.h>|信号|
|<complex.h>|支持复数运算|<stdarg.h>|可变参数表|
|<ctype.h>|字符类型|<stdbool.h>|布尔类型和值|
|<error.h>|出错码|<stddef.h>|标准定义|
|<fenv.h>|浮点环境|<stdint.h>|整型|
|<float.h>|浮点常量|<stdio.h>|标准io库|
|<inttypes.h>|整型格式转换|<stdlib.h>|实用程序库函数|
|<iso646.h>|替代关系操作符宏|<string.h>|字符串操作|
|<limits.h>|实现常量|<tgmath.h>|通用类型数学宏|
|<local.h>|局部类别|<time.h>|时间和日期|
|<math.h>|数学常量|<wchar.h>|扩展的多字节和宽字符支持|
|<setjmp.h>|非局部goto|<wctype.h>|宽字符分类和映射字符|



