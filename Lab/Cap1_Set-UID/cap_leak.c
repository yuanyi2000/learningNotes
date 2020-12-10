#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void main(){
    int fd;
    char * v[2];

    fd = open("./zzz", O_RDWR | O_APPEND);
    if(fd == -1){
        printf("can't open file /etc/zzz");
        exit(0);
    }

    printf("fd is %d \n", fd);

    setuid(getuid());

    v[0] = "/bin/bash"; v[1] = 0;
    execve(v[0], v, 0);
}
