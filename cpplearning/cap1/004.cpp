#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[])
{
	// 声明一个整数常量
	int ival = 1024;

	// 声明一个单/双精度浮点数据
	double dval = 3.14159;
	float fval = 3.14159;

	// 加减乘除
	// 等于 不等于 大于 小于

	// 数组
	int fibon[9] = { 0,1,1,2,3,5,8,13,21 }
	int first_elem = fibon[0]

	// 遍历
	for( int index = 0;index < 9; ++index)
		cout << fibon[index];

	// 指针 和 动态内存分配
	int * pint = &ival;

	// 动态内存分配和释放
	int *ptr  = new int(1024); // 申请单个对象的内存
	int *ptr2 = new int[4];    // 多个对象

	delete ptr;
	delete [] ptr2;

	return 0;
}