#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd, res; 
    size_t size; 
    char resstring[14]; 
    char name[]="my.fifo";

    (void)umask(0);
    
    printf("Writing data to FIFO\n");
    
    if((fd = open(name, O_WRONLY)) < 0){
        printf("Can\'t open FIFO for writing!\n");
        exit(-1);
    }
    size = write(fd, "Hello, world!", 14);

    if(size != 14) {
        printf("Can\'t write all string to the FIFO!\n");
        exit(-1);
    }
    close(fd);
    printf("Writer is done\n");

    return 0;
}
