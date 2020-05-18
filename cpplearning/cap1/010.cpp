#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
using namespace std;

// 标准数组——向量
<<<<<<< HEAD

// 创建vector对象

std::vector<int> veco; // 空

const int size = 8;
const int value = 1024;

std::vector<int> vec1(size); // size=8 value=0

std::vector<int > vec2(size, value); // size=8 value=1024

int ia[4] = { 0, 1, 1, 2 };
std::vector<int> vec3(ia,ia+4); // 初始化为ia的值

std::vector<int> vec4(vec2); // 拷贝

// 标准 vector 类模板也支持下标操作符

extern int getSize();

void mumber()
{
	int size = getSize();
	vecot<int> vec(size);

	for (int i = 0; i < size; ++i)
	{
		vec[i] = i;
	}
}

// 另一种遍历方法是使用迭代器对(iterator pair)来标记向量的其实处和结束处
/*
	vector类对象提供一对操作
	begin() end()
	分别指向“向量开始处“和”结束后一个“的迭代器
*/

void mumber2()
{
	int size = getSize();
	std::vector<int> vec(size);

	vector<int>::interator iter = vec.begin();

	for (int i = 0; iter != vec.end(); ++i, ++iter)
	{
		*iter = i;
	}
}

