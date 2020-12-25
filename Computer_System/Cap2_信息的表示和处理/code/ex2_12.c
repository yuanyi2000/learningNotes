#include "show_bytes.h"
#include <stdlib.h>

void fun0(int x){
	int y = x&0xFF;
	show_int(y);
}
void fun1(int x){
	int m = ~(x^x) - x;
	int n = ~(x^x) - 0xFF;
	m = m&n;
	n = x&0xFF;
	m += n;
	show_int(m);
	return;
}
void fun2(int x){
	int m = ~(x^x) - 0xFF;
	int y = (x&m) + 0xff;
	show_int(y);
	return;
}

int main(int argc, char const *argv[])
{
	int x = 0x87654321;

	printf("%d\n", x);
	show_int(x);
	fun0(x);
	fun1(x);
	fun2(x);
	return 0;
}