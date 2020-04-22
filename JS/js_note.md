# JS #

## 第一章 JS简介 ##

---

### 1.1 js简史 ###
没什么好说的

### 1.2 js实现 ###
一个完整的JavaScript实现应该由下列三个不同的部分组成：

+ 核心（ECMAScript）
+ 文档对象模型（DOM)

> 文档对象模型是针对XML但经过扩展用于HTML的应用程序编程接口

+ 浏览器对向模型（BOM）

---

## 第二章 在HTML中使用JS ##

### 2.1 <script>元素 ###
HTML 4.01为`<script>`定义了下列六个属性：

+ async 可选 表示应立即下载脚本
+ charset 可选 表示通过src属性指定的代码的字符集（不常用）
+ defer 可选 表示脚本可以延迟到文档完全被解析之后再执行（只对外部脚本有效）
+ language 已废弃
+ src 可选 表示包含要执行代码的外部文件
+ type 可选 表示编写代码使用的脚本语言类型


在使用`<script>`元素嵌入js代码时，只需指定type属性

```javascript
	<script type="text/javascript">
		function sayHi(){
			alert("Hi");
		} 
	</script>
```

> 在嵌入的js代码内要避免出现`</script>`字符，如果有需要应该使用转义字符\


如果要通过`<script>`包含外部文件，则src属性就是必须的，其值为指向该文件的链接

`<script type="text/javascript" src="example.js"></script>`


> 当使用的src属性后，标签之间不能再出现额外的JS代码


#### 2.1.1 标签的位置 ####


```HTML
<!DOCTYPE html>
<html>
	<head>
		<title>Example HTML Page</title>
	</head>
	<body>
		<!--这里放内容--！>
		<script type="text/javascript" src="example.js"></script>
	</body>
</html>
```

#### 2.1.2 延迟脚本 ####

defer属性设置后，脚本会延迟到整个页面都解析完毕后再执行

>defer属性只适用于外部脚本文件

在使用defer属性时，只需设置defer="defer"即可

`<script type="text/javascript" defer="defer" src="example.js"></script>`

#### 2.1.3 异步脚本 ####

async属性

目的是不让页面等待脚本的下载和执行，从而异步加载页面的其他内容。

>建议异步脚本不要在加载期间修改DOM

#### 2.1.4 在XHTML中的用法 ####


#### 2.1.5 不推荐使用的用法 ####



### 2.2 嵌入代码与外部文件 ###


### 2.3 文档模式 ###


### 2.4 <noscript>元素 ###


### 2.5 小结 ###

+ 在包含外部js文件时，*必须将src属性设置为指向文件的URL*

+ 所有的`<script>`元素都会按照他们在页面中出现的先后顺序被依次解析（不使用defer和async属性的情况下）

+ 一般应该把`<script>`元素放在页面主要内容后，`</body>`标签前

+ 使用defer属性可以让脚本在文档完全呈现之后再执行。延迟脚本总是按照他们的顺序执行

+ 使用async属性表示当前脚本不必等待其他脚本。不能保证异步脚本按顺序执行



## 第三章 基本概念 ##


### 3.1 语法 ###

ECMAScript的语法大量借鉴了C及其他类C语言（如Java和Perl）


#### 3.1.1 区分大小写 ####

ECMAScript中的一切都区分大小写（变量、函数和操作符）

#### 3.1.2 标识符 ####

所谓*标识符*，就是指变量、函数、属性的名字，或者函数的参数

+ 第一个字符必须是一个字母、下划线(\_)或美元符号($)

+ 其他字符可以是字母、下划线、美元符号或数字

按照惯例，ECMAScript*采用驼峰大小写格式*，例如

```
firstSecond
myCar
doSomeThingImportant
```


#### 3.1.3 注释 ####

ECMASript采用C风格注释


#### 3.1.4 严格模式 ####

在顶部添加如下代码以启用严格模式

`'use strict'`

#### 3.1.5 语句 ####

分号结尾表示一条语句 

>即使没有分号也是一条有效的语句，但不推荐这样做

### 3.2 关键字和保留字 ###


### 3.3 变量 ###

ECMAScript的变量是松散类型的，可以保存任何类型的数据

定义变量时要使用*var*操作符

```javascript
	var message = 'hi';
```

当var被省略时创建的是一个全局变量

```javascript
	function test()
	{
		message = "hi"; // 这是一个全局变量
	}
	test();
	alert(message); // "hi"
```

### 3.4 数据类型 ###

ECMAScript有5种简单数据类型和一种复杂数据类型：

+ Undefined
+ Null
+ Boolean
+ Number
+ String
+ Object

>由于ECMAScript数据类型具有动态性，因此没有再定义其他数据类型的必要了



#### 3.4.1 typeof操作符 ####

typeof操作符用于检测给定变量的数据类型

* "undefined"——如果这个值未定义
* "boolean"——如果这个值是布尔值
* "string"——字符串
* "number"——数值
* "object"——如果这个值是对象或null
* "function"——如果这个值是函数

#### 3.4.2 Undefined ####

当var声明了一个变量而*未对他初始化*，那么他的值就是undefined


#### 3.4.3 Null ####

null表示一个空对象指针


#### 3.4.4 boolean ####

该类型只有两个字面值：true false


要将一个值转换为其对应的boolean值，可以调用转型函数
```javascript
	Boolean()
```

下表给出各种数据类型及其对应的转换规则

|数据类型|转换为true的值|转换为false的值|
|:-:|:-:|:-:|
|Boolean|true|false|
|String|任何非空字符串|""(空字符串)|
|Number|任何非0数字值(包括无穷大)|0和NaN|
|Object|任何对象|null|
|Undefined|n/a|undefined|

>n/a(或N/A),是not applicable的缩写，意思是"不适用"

#### 3.4.5 Number类型 ####

Number类型可以表示整数和浮点数，在数字前加0可表示八进制数，0x表示十六进制数

1. 浮点数值

2. 数值范围
	+ 无穷 Infinity

3. NaN(not a number)
	+ NaN与任何值都不相等，包括NaN本身

4. 数值转换

有三个数值可以把非数值转换为数值

```javascript
	Number()     // 可用于任何数据类型
	parseInt()   // 把字符串转换为整数
	parseFloat() // 把字符串转换为浮点数
```

`Number()`函数转换规则如下

+ 如果是boolean值，true和false会分别转换为1和0
+ 如果是数字值，则只是简单的传入和返回
+ 如果是null值，则返回0
+ 如果是undefine，则返回NaN
+ 如果是字符串，则有如下规则
	* 如果只包含数字，会忽略前导0
	* 有小数点则转换为浮点数
	* 如果包含有效的十六进制，则返回同样大小的*十进制整数值*
	* undefined返回NaN
	* 空字符串返回0
	* 除上述情况外，返回NaN
+ 如果是对象，则调用对象的`valueOf()`方法 若转换的结果是NaN，则调用对象的`toString()`方法

- - -


`parseInt()`函数转换规则如下
+ 忽略字符串前面的空格，直到找到第一个非空格字符
+ 如果第一个非空格字符不是数字或负号，则返回NaN
+ 解析完后忽略后面的非数字字符

> parseInt()可以识别十六或八进制字符

可以为`parseInt()`函数提供第二个参数，其值为转换的进制

- - -
`parseFloat()`函数
+ 会忽略所有前导0
+ 只有第一个小数点有效
+ 只解析十进制的值


#### 3.4.6 String ####


