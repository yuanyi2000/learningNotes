/* 这个程序有缓冲区攻击漏洞 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int foo(char * str)
{
	char buffer[100];
	printf("buff addr  0x%x\n", (unsigned int)&buffer);

	/* 下面的语句有缓冲区攻击问题 */
	strcpy(buffer, str);
	return 1;
}

int main(int argc, char const *argv[])
{
	char str[400];
	FILE *badfile;

	badfile = fopen("badfile", "r");
	fread(str, sizeof(char), 300, badfile);
	foo(str);

	printf("Return Properly\n");
	return 0;
}