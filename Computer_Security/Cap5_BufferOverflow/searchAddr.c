#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void func(int *a1)
{
	printf("::a1's address is 0x%x \n", (unsigned int) &a1);
}

void foo(char * str)
{
	char buffer[100];
	printf("buffer addr   0x%x\n", buffer);
	strcpy(buffer, str);
}

int main(int argc, char const *argv[])
{
	int a = 3;
	func(&a);

	char x[12];
	char * y = malloc(sizeof(char)*12);

	printf("Addr buffer x 0x%x\n", x);
	printf("Addr heap   y 0x%x\n", y);
	free(y);

	char * str = "okokok";
	foo(str);


	return 0;
}