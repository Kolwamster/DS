#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <dlfcn.h>

void exec(char**, int, int, const char*, const char*);
void exec_pipe(char**, char**, int, int, const char*, const char*);

int main() {
    int redir_in = 0, redir_out = 0;
    int j = 0;
    int k = 0;
    int inner = 0, piped = 0;
    int size = 0;
    pid_t main_pid = getpid();
    size_t len = 0;
    char *buf;
    char filename_out[32];
    char filename_in[32];
    ssize_t bufsize = 0;
    void *handle_swapper;
    int (*swap_files)(const char*, const char*);
    char *error;

    while((bufsize = getline(&buf, &len, stdin)) != 0){
        char *args[32];
        char *arg[32];
        char *arg2[32];
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
                if(strcmp(args[i], ">>") == 0){
                    redir_out = 2;
                    i++;
                    strcpy(filename_out, args[i]);
                    continue;
                }
                if(strcmp(args[i], "|") == 0){
                    piped = 1;
                    continue;
                }
                if(strcmp(args[0], "swap") == 0 && args[2] != NULL){
                    handle_swapper = dlopen("./libswapper.so", RTLD_LAZY);
                    if(!handle_swapper) {
                        fputs(dlerror(), stderr);
                        exit(1);
                    }
                    swap_files = dlsym(handle_swapper, "swap_files");
                    if ((error = dlerror()) != NULL){
                        fputs(error, stderr);
                        exit(1);
                    }
                    (*swap_files)(args[1], args[2]);
                    inner = 1;
                    dlclose(handle_swapper);
                    break;
                }
                if(piped == 0){
                    arg[j] = args[i];
                    j++;
                }
                else{
                    arg2[k] = args[i];
                    k++;
                }
            }
            else{
                break;
            }
        }
        free(buf);
        if(inner == 0){
            if(piped == 0)
                exec(arg, redir_out, redir_in, filename_out, filename_in);
            else
                exec_pipe(arg, arg2, redir_out, redir_in, filename_out, filename_in);
        }
        size = 0;
        j = 0;
        k = 0;
        redir_in = 0;
        redir_out = 0;
        piped = 0;
        inner = 0;
        bzero(filename_out, 32);
        bzero(filename_in, 32);
        for(int i = 0; i < 32; i++){
            args[i] = NULL;
            arg[i] = NULL;
            arg2[i] = NULL;
        }
    }
    return 0;
}
