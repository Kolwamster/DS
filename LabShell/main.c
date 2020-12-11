#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

#include "swapper.h"

void exec(char**, int, int, char*, char*);

int main() {
    int redir_in = 0, redir_out = 0;
    int j = 0;
    int inner = 0;
    int size = 0;
    pid_t main_pid = getpid();
    size_t len = 0;
    char *buf;
    char filename_out[32];
    char filename_in[32];
    ssize_t bufsize = 0;
    while((bufsize = getline(&buf, &len, stdin)) != 0){
        inner = 0;
        char *args[32];
        char *arg[32];
        buf[strlen(buf) - 1] = '\0';
        if(strcmp(buf, "exit") == 0){
            kill(main_pid, 2);
        }
        for(int i = 0; i < 32; i++){
            args[i] = strsep(&buf, " ");
            if(args[i] == NULL)
                break;
            size++;
            if(strlen(args[i]) == 0)
                i--;
        }
        for(int i = 0; i < size; i++){
            if(args[i] != NULL){
                if(strcmp(args[i], ">") == 0){
                    redir_out = 1;
                    i++;
                    strcpy(filename_out, args[i]);
                    continue;
                }
                if(strcmp(args[i], "<") == 0){
                    redir_in = 1;
                    i++;
                    strcpy(filename_in, args[i]);
                    continue;
                }
                if(strcmp(args[0], "swap") == 0 && args[2] != NULL){
                    swap_files(args[1], args[2]);
                    inner = 1;
                    break;
                }
                arg[j] = args[i];
                j++;
            }
            else{
                break;
            }
        }
        free(buf);
        if(inner == 0){
            exec(arg, redir_out, redir_in, filename_out, filename_in);
        }
        size = 0;
        j = 0;
        bzero(filename_out, 32);
        bzero(filename_in, 32);
        for(int i = 0; i < 32; i++){
            args[i] = NULL;
            arg[i] = NULL;
        }
    }
    
    return 0;
}
