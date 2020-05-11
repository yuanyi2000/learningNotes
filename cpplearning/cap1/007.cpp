#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
using namespace std;

// 泛型设计

// C++的模板设施提供了一种机制，能将类函数内部的类型和值参数化

// 将Array类转换成Array模板
template <class eleType>
class Array
{
public:
	// 把元素类型参数化
	explicit Array(int size = DefaultArraySize);
	Array(eleType * array, int array_size);
	Array(const Array &rhs);

	virtual ~Array(){delete [] ia;}
	bool operator==(const Array&) const;
	bool operator!=(const Array&) const;
	Array& operator=(const Array&);
	int size() const {return _size;}

	virtual eleType& operator[](int index){return ia[index];}
	virtual void sort();
	virtual eleType min() const;
	virtual eleType max() const;
	virtual int find(const eleType &val) const;
protected:
	static const int DefaultArraySize = 12;
	int _size;
	eleType *ia;
};

// 使用Array类模板

int main(int argc, char const *argv[])
{
	const int array_size = 4;

	// eleType -> int
	Array<int> ia(array_size);

	// eleType -> double
	Array<double> da(array_size);

	// eleType -> char
	Array<char> ca(array_size);

	for (int i = 0; i < array_size; ++i)
	{
		ia[i] = i;
		da[i] = i * 1.75;
		ca[i] = i + 'a';
	}

	for (int i = 0; i < array_size; ++i)
	{
		cout << "[" << i << "] ia:" << ia[i] << '\nca:' << ca[i] << '\nda:' << da[i] << endl; 
	}
	return 0;
}