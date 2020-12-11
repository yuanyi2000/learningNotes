#include <stdlib.h>
#include <stdio.h>

extern char ** environ;

int main(int argc, char const *argv[])
{
	int i = 0;
	while(environ[i] != NULL){
		printf("%s\n", environ[i]);
		i++;
	}
	return 0;
}