#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
    pid_t pid;
    int a = 0; 
    pid = fork();

    if (pid == 0) {
        a = 1;
        printf("I`m child! My pid = %d, my ppid = %d, result = %d\n", 
            getpid(), getppid(), a);
    } else if (pid == -1) {
        printf("Error!");
    } else {
        a = 2;
        printf("I`m main process! My pid = %d, my ppid = %d, result = %d\n", 
        getpid(), getppid(), a);
    }
    
    return 0;
}