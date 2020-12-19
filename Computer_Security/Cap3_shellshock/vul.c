#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	setuid(geteuid()); // geteuid 有效ID
	system("/bin/ls -l");
	return 0;
}
