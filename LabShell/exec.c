#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

void exec(char** args){
    pid_t pid;
    pid = fork();
    int fd;
    if(pid == 0){
        if(execvp(args[0], args) < 0){
            printf("Can't start this process\n");
	        exit(0);
	    }
    }
    else{
        wait(NULL);
    }
}
