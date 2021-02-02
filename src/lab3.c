#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main() {
    printf("Proc pid = %d, proc ppid = %d\n", 
        getpid(), getppid());
    return 0;
}