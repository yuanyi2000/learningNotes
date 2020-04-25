	这是在学习廖雪峰老师python教程时记录的学习笔记

- - -

# python #

	中文 免费 零起点 完整示例 基于最新的python3版本 

python能干什么？？

+ 可以做日常任务
+ 可以做网站
+ 可以做网络游戏的后台

python不能做什么？？

+ 不能做操作系统
+ 不能写手机应用
+ 不能写3D游戏

总之，来学python吧！


## python简介 ##

	Python是著名的“龟叔”Guido van Rossum在1989年圣诞节期间，为了打法无聊的圣诞节而编写的一个编程语言

# 1 安装python #

## python解释器 ##

# 2 第一个python程序 #

## 使用文本编辑器 ##

## python代码运行助手 ##

## 输入和输出 ##

```python
	print()  # 输出函数
	input()  # 输入函数
```

# 3 python基础 #

## 3.1 数据类型和变量 ##

+ 整数
+ 浮点数
+ 字符串
+ 布尔值
	* True
	* False
	* 布尔值运算可以用`and or not`运算符
+ 空值`None`


* 变量
* 常量

## 3.2 字符串和编码 ##

	Unicode应运而生，这样就不会有乱码问题了

* python的字符串

	在最新的python3中，字符串是Unicode编码的。

	对于单个字符的编码，Python提供了`ord()`函数获得字符的整数表示，而`chr()`可以得到编码的对应字符


> 由于Unicode一个字符对应若干字节，不方便网络传输或者保存在磁盘上，因此需要把`str`变成字节类型的`bytes`


	python的`bytes`类型用带前缀b的字符表示，例如

	`x = b'ABC'`


	Unicode表示的str通过`encode()`方法可以编码为对应的bytes

	而将bytes变为str需要的是 `decode()`方法

	如果str包含一小部分无效字节，可以传入`errors='ignore'`忽略错误字节

```python
>>>b'\xe4\xb8\xad\xff'.decode('utf-8',errors='ignore')
‘中’
```

	`len()`函数可以计算str的字符数，也可计算bytes的字节数


```python
# !/usr/bin/env python3
# -*- coding: utf-8 -*-
```

	申明了utf-8编码并不代表`.py`文件就是utf-8编码的，需要确保编辑器正在使用utf-8 without BOM编码


## 3.3 使用list和tuple ##

1. list
	
	list 列表

2. tuple

	tuple 元组 （不可更改）



	序列类型（string，list，tuple）小结


|操作符，内建函数或方法|说明|字符串string|列表list|元组tuple|
|:--|:--|:--:|:--:|:--:|
|[]|创建列表					| |.| |
|()|创建元组          		| | |.|
|""|创建字符串				|.| | |
|append()|在列表中添加新元素| |.| |
|capitalize()|字符串首字母大写|.| | |
|center()|使字符串居中并用指定字符填充|.| | |
|chr()|返回整数对应的ASCII字符|.| | |
|ord()|返回字符对应的ASCII码|.| | |
|cmp()|比较大小|.|.|.|
|count()|统计某个元素出现的次数|.|.| |
|decode()|解码|.| | |
|encode()|编码|.| | |
|endswith()|判断字符串是否以指定后缀结尾|.| | |
|expandtabs()|将字符串中的tab转换为空格|.| | |
|extend()|扩展列表，批量写入| |.| |
|find()|在字符串中查找字符|.| | |
|index()|查找元素，返回索引|.|.| |
|insert()|在指定位置插入元素| |.| |
|isdecimal()|检查字符串是否**只**包含十进制字符|.| | |
|isdigit()|检查字符串是否只由数字组成|.| | |
|islower()|检查字符串是否由小写字母组成|.| | |
|isnumeric()|检查字符串是否只由数字组成|.| | |
|isspace()|检查字符串是否只包含空格|.| | |
|istitle()|检查字符串是否只有首字母大写|.| | |
|isupper()|检查字符串是否包含大写字母|.| | |
|join()|将序列中的字符以指定字符连接起来|.| | |
|len()|返回序列长度|.|.|.|
|list()|列表生成器|.|.|.|
|ljust()|返回一个原字符串左对齐，并用空格填充至指定长度的新字符串|.| | |
|rjust()|返回一个原字符串右对齐，并用空格填充至指定长度的新字符串|.| | |
|lower()|转换小写|.| | |
|lstrip()|去除左边的字符|.| | |
|strip()|去除首尾字符|.| | |
|rstrip()|去除右边字符|.| | |
|max()\min()|返回最大\最小元素|.|.|.|
|oct()|转换一个整数对象为8进制字符串|.| | |
|hex()|转换一个整数对象为16进制字符串|.| | |
|pop()|栈出的方式取出元素| |.| |
|raw_input()|输入字符串|.| | |
|remove()|删除元素| |.| |
|replace()|替换指定字符为|.| | |
|split()|分割字符串，返回列表|.| | |



## 3.4 条件判断 ##


	 if..elif..else


## 3.5 循环 ##

1. for x in ...
2. while... 
	* break 令循环结束
	* continue 进行下一次循环


## 3.6 dict和set ##

+ dict 字典
	
	key： value

+ set

	只储存key


