#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <dlfcn.h>
#include <errno.h>

#define BUF_SIZE 128

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
    char *cwd;
    void *handle_plugin;
    char *error;
    char *plug_name;
    void (*execute)(char**, int);

    cwd = (char*)malloc(BUF_SIZE);
    getcwd(cwd, BUF_SIZE);   
    printf("%s>", cwd);

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
                if(strcmp(args[0], "cd") == 0){
                    inner = 1;
                    if(args[1] != NULL){
                        chdir(args[1]);
                    }
                    else{
                        chdir("/");
                    }
                    break;
                }
                if(strcmp(args[0], "chroot") == 0 && args[1] != NULL){
                    inner = 1;
                    chroot(args[1]);
                    printf("%s\n", strerror(errno));
                    break;
                }
                if(strcmp(args[0], "plugin") == 0 && args[1] != NULL){
                    inner = 1;
                    handle_plugin = dlopen(args[1], RTLD_LAZY);
                    if(!handle_plugin) {
                        fputs(dlerror(), stderr);
                        exit(1);
                    }
                    char* (*get_plug_name)();
                    execute = dlsym(handle_plugin, "execute");
                    get_plug_name = dlsym(handle_plugin, "get_plug_name");
                    plug_name = get_plug_name();
                    printf("plugged %s\n", plug_name);
                    if ((error = dlerror()) != NULL){
                        fputs(error, stderr);
                        exit(1);
                    }
                    break;
                }
                if(plug_name != NULL && strcmp(args[0], plug_name) == 0){

                    inner = 1;
                    for(int q = 1; q < 32; q++){
                        if(args[q] == NULL){
                            break;
                        }
                        arg[j] = args[q];
                        j++;
                    }

                    execute(arg, j);
                    break;
                }
                /*if(strcmp(args[0], "unplug") == 0 && args[1] != NULL){
                    inner = 1;
                    if(strcmp(args[1], plug_name) == 0){
                        dlclose(handle_plugin);
                        printf("unplugged %s\n", plug_name);
                        plug_name = NULL;
                        execute = NULL;
                    }
                    break;
                }*/
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
        free(cwd);
        cwd = (char*)malloc(BUF_SIZE);
        getcwd(cwd, BUF_SIZE);   
        printf("%s>", cwd);
    }

    free(cwd);

    return 0;
}