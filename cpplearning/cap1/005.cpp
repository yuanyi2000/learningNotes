#include <iostream>
#include <string>
#include <fstream>
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

		explicit IntArray(int sz = DefaultArraySize);
		IntArray(int * array, int array_size)
		IntArray(const IntArray &rhs)

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

	// 单个IntArray对象
	IntArray myArray;
	IntArray * ptrArray = new IntArray;

	// 调用方法
	int min_val = myArray.min();
	int max_val = ptrArray->max();


	return 0;
}