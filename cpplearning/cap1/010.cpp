#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
using namespace std;

// 标准数组——向量
const int size = 8;
const int value = 1024;
// 创建vector对象的各种方法
vector<int> veco;
std::vector<int> v1(size);
std::vector<int> v2(size, value);

int ia = {0,1,2,3};
std::vector<int> v3(ia, ia+4);

std::vector<int> v4(v2);

// 遍历向量
for (int i = 0; i < size; ++i)
{
	v1[i] = i;
}

// 迭代器
void mumble()
{
	int size = getSize();
	std::vector<int> v5(size);

	vector<int>::interator iter = vec.begin();

	for (int i = 0; iter != vec.end(); ++i, ++iter)
	{
		*iter = i;
	}
}
