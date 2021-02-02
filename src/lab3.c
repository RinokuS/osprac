#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main() {
    printf(" pid = %d, my ppid = %d\n", 
        getpid(), getppid());
    return 0;
}