#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
using namespace std;

// 用其他名字来命名数组

// 封装命名空间
namespace Cplusplus_Prime_3E{
	template <class eleType>
	class Array{/*...*/}
}
// 使用 限定修饰名字符 来访问命名空间的声明
namespace_identifier::entity_name;
// 例如
Cplusplus_Prime_3E::Array<string> text;


// 命名空间别名
namespace CPE = Cplusplus_Prime_3E;
CPE::Array<string> text2;

// 使用using
using namespace Cplusplus_Prime_3E;
int main(int argc, char const *argv[])
{
	Array<string> text3; // ok
	return 0;
}

// 只让命名空间的某一部分可见
using namespace Cplusplus_Prime_3E::Array;