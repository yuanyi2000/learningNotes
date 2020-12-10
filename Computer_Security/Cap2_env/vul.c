/* 容易被攻击的程序 */

#include <stdlib.h>
int main(int argc, char const *argv[])
{
	system("cal"); // 运行日历命令，但是没有提供完整路径
	return 0;
}