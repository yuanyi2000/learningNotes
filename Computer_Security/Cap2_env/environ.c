#include <stdio.h>

extern char ** environ;	// extern提示编译器遇到这个函数或者变量时，在其他模块寻找定义

int main(int argc, char const *argv[], char *envp[])
{
	int i = 0;
	while(environ[i] != NULL){
		printf("%s\n", environ[i++]);
	}
	return 0;
}