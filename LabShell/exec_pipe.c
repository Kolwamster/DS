#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

void exec_pipe(char** args, char** piped_args, int redir_out, int redir_in, const char* filename_out, const char* filename_in){
    pid_t pid, pid2;
    int fd_in, fd_out, pfd[2];
    pipe(pfd);
    pid = fork();
    if(pid == 0){
        if(redir_in == 1){
            fd_in = open(filename_in, O_RDONLY);
            dup2(fd_in, 0);
            close(fd_in);
        }
        close(pfd[0]);
        dup2(pfd[1], 1);
        close(pfd[1]);
        if(execvp(args[0], args) < 0){
            printf("Can't start first process\n");
	        exit(0);
	    }
    }
    else{
        pid2 = fork();
        if(pid2 == 0){
            if(redir_out == 1){
                fd_out = open(filename_out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                dup2(fd_out, 1);
                close(fd_out);
            }
            if(redir_out == 2){
                fd_out = open(filename_out, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                dup2(fd_out, 1);
                close(fd_out);
            }
            close(pfd[1]);
            dup2(pfd[0], 0);
            close(pfd[0]);
            if(execvp(piped_args[0], piped_args) < 0){
                printf("Can't start second process\n");
	            exit(0);
	        }
        }
        else{
            close(pfd[1]);
            close(pfd[0]);
            wait(NULL);
            wait(NULL);
        }
    }
}
