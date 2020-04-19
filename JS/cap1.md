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