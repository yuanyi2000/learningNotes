#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
using namespace std;


int main(int argc, char const *argv[])
{
	

	//  基于对象的设计
	/*
		使用C++的类机制来设计一个数组抽象。
		1.数组类的实现中有内置的自我意识。首先，它知道自己的大小
		2.支持赋值、比较等操作
		3.查询，极值，索引
		4.自排序
		5.指定长度，以此来创建新数组
		6.能够用一组值初始化数组
		7.通过索引访问值
		8.截获并指出错误的索引
	*/


	// 基于对象设计的类一般形式：
	class classname
	{
		// 公有操作集合
	public:
		classname();
		~classname();
		// 私有实现代码
	private:
	};



	class IntArray;
	class IntArray
	{
	public:
		// 构造函数

		explicit IntArray(int sz = DefaultArraySize); // 省缺构造函数，不需要用户提供参数
		IntArray(int * array, int array_size)		  // 
		IntArray(const IntArray &rhs)			      // 拷贝构造函数

		// 析构函数
		~IntArray(){delete [] ia;}

		// 相等于不相等操作
		bool operator == (const IntArray&) const;
		bool operator != (const IntArray&) const;

		// 赋值操作符
		IntArray& operator = (const IntArray&);

		int size() const;
		void sort();

		int min() const;
		int max() const;

		int find(int value) const;
	private:
		// 私有实现代码
		/** 信息隐藏 **/

		// 内部数据
		int _size;  // 用公有访问函数size()访问私有数据_size
		int * ia;

		// 内联函数机制
		// 重载机制

		static const int DefaultArraySize = 12;
	};


	IntArray::  // 类域操作符
	IntArray(int sz)
	{
		// 设置数据成员
		size = sz;
		ia = new int[_size]

		// 初始化内存
		for(int ix=0; ix<_size; ++ix)
			ia[ix] = 0;
	}

	IntArray::
	IntArray(int * array, int sz)
	{
		size = sz;
		ia = new int[_size];

		for(int ix=0; ix<_size; ++ix)
			ia[ix] = array[ix];
	}

	IntArray::
	IntArray(const IntArray &rhs) // 引用，是一种没有指针语法的指针
	{
		// 拷贝构造函数
		_size = rhs._size;
		ia = new int[_size];
		for(int ix = 0; ix < _size; ix++)
			ia[ix] = rhs.ia[ix];
	}


	// 支持索引
	int&
	IntArray::
	operator[](int index)
	{
		assert(index >= 0 && index < size); // 为了后面程序可以正常执行的断言
		return ia[index];
	}


	// 单个IntArray对象
	IntArray myArray;
	IntArray * ptrArray = new IntArray;

	// 调用方法
	int min_val = myArray.min();
	int max_val = ptrArray->max();

	// 独立的共享函数
	class IntArray2
	{
	public:
		explicit IntArray(int sz = DefaultArraySize); // 省缺构造函数，不需要用户提供参数
		IntArray(int * array, int array_size)		  // 
		IntArray(const IntArray &rhs)			      // 拷贝构造函数
	private:
		void init(int sz, int * array);
		
		int _size;
		int * ia;
	}
	void
	IntArray2::
	init(int sz, int * array)
	{
		_size = sz;
		ia = new int[_size];
		for (int i = 0; i < _size; ++i)
		{
			if( ! array )
				ia[ix] = 0;
			else
				ia[ix] = array[ix];
		}
	}
	// 三个构造函数可重写为：
	IntArray2::IntArray2(int sz){init(sz, 0);}
	IntArray2::IntArray2(int *array, int sz)
	{init(sz, array);}
	IntArray2::IntArray2(const IntArray2 &rhs)
	{init(rhs.size, rhs.ia)}





	// 练习2.5

	/* 
		C++的关键特征就是接口与实现的分离
		接口：用户可以应用到类对象上的操作。包括：
			1.操作的名字
			2.返回值
			3.参数表
		（一般的，这就是用户所要知道的全部内容）
		
	*/



	return 0;
}