#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
    id_t pid;
    int a = 0; 
    pid = fork();

    if (pid == 0) {
        execle("robot.out", "robot.out", 0, envp);
    } else if (pid == -1) {
        printf("Error!");
    } else {
        a = 2;
        printf("I`m main process! My pid = %d, my ppid = %d, result = %d\n", 
        getpid(), getppid(), a);
    }
    
    return 0;
}