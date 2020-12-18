#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

void exec(char** args, int redir_out, int redir_in, const char* filename_out, const char* filename_in){
    pid_t pid;
    int fd_in, fd_out;
    pid = fork();
    if(pid == 0){
        if(redir_out == 1){
            fd_out = open(filename_out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
            dup2(fd_out, 1);
        }
        if(redir_out == 2){
            fd_out = open(filename_out, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
            dup2(fd_out, 1);
        }
        if(redir_in == 1){
            fd_in = open(filename_in, O_RDONLY);
            dup2(fd_in, 0);
        }
        if(execvp(args[0], args) < 0){
            printf("Can't start this process\n");
	        exit(0);
	    }
        close(fd_in);
        close(fd_out);
    }
    else{
        wait(NULL);
    }
}
