#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
using namespace std;

// 修改过的IntArray类 基类
class IntArray
	{
	public:
		// 构造函数

		explicit IntArray(int sz = DefaultArraySize); // 省缺构造函数，不需要用户提供参数
		IntArray(int * array, int array_size)		  // 
		IntArray(const IntArray &rhs)			      // 拷贝构造函数

		// 虚拟析构函数
		virtual ~IntArray(){delete [] ia;}

		// 相等于不相等操作
		bool operator == (const IntArray&) const;
		bool operator != (const IntArray&) const;
		IntArray& operator = (const IntArray&);
		int size() const {return _size};

		virtual int& operator[](int index) { return ia[index]; }
		virtual void sort();
		virtual int min() const;
		virtual int max() const;
		virtual int find(int val) const;

	protected:// 保护级别的数据和成员函数只提供给派生类
		static const int DefaultArraySize = 12;
		void init(int sz, int * array);
		int _size;
		int * ia;
		
	};

	// 派生类，包括边界检查
	/*
		派生类对象： 每个基类是一个类的子对象subibject，它在新定义的派生类中有独立的一部分
		初始化： 首先自动调用每个基类的构造函数，然后再执行派生类的构造函数
		 	从设计的角度看，派生类的构造函数应该只初始化那些在派生类中被定义的数据成员
	*/
/*
	1. 它提供自己的下表操作符实例，以支持边界检查
	2. 它提供实际的操作来做边界检查工作（声明为private）
	3. 它有自己的构造函数
*/
class IntArrayRC:public IntArray 
{
public:
	IntArrayRC(int sz = DefaultArraySize);
	IntArrayRC(int * array, int array_size);
	// IntArrayRC(const IntArrayRC &rhs); 不要提供拷贝构造函数
	virtual int& operator[](int);
	~IntArrayRC();
private:
	void check_range(int);
};

inline void // inline关键字 显式的要求一个函数是内联函数
IntArrayRC::check_range(int index)
{
	assert(index >= 0 && index < size);
}

inline IntArrayRC::IntArrayRC(int sz)
: IntArray(sz){}				// 初始化列表（函数体为空）
inline IntArrayRC::IntArrayRC(const int *iar, int sz)
: IntArray(iar, sz){}

