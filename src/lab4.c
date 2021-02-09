#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    for (int i = 0; i < argc; ++i) 
        printf("Command Line Argument №%d = %s.\n", i, argv[i]);
    
    int iter = 0;
    while (envp[iter] != NULL) {
        printf("Environment Argument №%d = %s.\n", iter, envp[iter]);
        iter++;
    }

    return 0;
}