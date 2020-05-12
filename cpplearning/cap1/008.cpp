#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
using namespace std;

// 基于异常的设计

// 异常的抛出由throw表达式执行
void openFileFail()
{
	if(! infile)
	{
		string errMsg("unable to open file: ");
		errMsg += fileName;
		throw errMsg;
	}
}

// 异常的处理由catch语句来执行
void catchErr(){
	catch(string exceptionErr){
		log_message(exception);
		return false;
	}
}

// 使用try块
int *
stats(const int *ia, int size)
{
	int * pstasts = new int[4];

	try{
		pstasts[0] = sum_it(ia, size);
		pstasts[1] = min_val(ia, size);
		pstasts[2] = max_val(ia,size);
	}
	catch(string exceptionMsg)
	{/*异常处理*/}
	catch(const statsException &statsExcp)
	{/*异常处理*/}
	do_something(pstasts);
	return pstasts;
}
