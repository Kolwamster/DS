#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

void exec(char**, int, char*);

int main() {
    int redir = 0;
    pid_t main_pid = getpid();
    size_t len = 0;
    char *buf;
    char *args[32];
    char *filename = NULL;
    ssize_t bufsize = 0;
    while((bufsize = getline(&buf, &len, stdin)) != 0){
        redir = 0;
        buf[strlen(buf) - 1] = '\0';
        if(strcmp(buf, "exit") == 0){
            kill(main_pid, 2);
        }
        for(int i = 0; i < 32; i++){
            args[i] = strsep(&buf, " ");
            if(args[i] == NULL)
                break;
            if(strlen(args[i]) == 0)
                i--;
        }   
        free(buf);
        exec(args, redir, filename);
    }
    
    return 0;
}
