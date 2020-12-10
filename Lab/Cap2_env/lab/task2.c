#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

extern char ** environ;

void printenv(){
	int i = 0;
	while(environ[i] != NULL){
		printf("%s\n", environ[i]);
		i++;
	}
}
int main(int argc, char const *argv[])
{
	pid_t childPid;
	switch(childPid = fork()){
		case 0:
		printenv();	// child
			exit(0);
		default:
		// printenv();		// another
		exit(0);
	}
	return 0;
}