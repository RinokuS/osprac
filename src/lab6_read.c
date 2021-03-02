#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd, res;
    size_t size;
    char resstring[14];
    char name[] = "my.fifo";

    umask(0);

    if (mknod(name, S_IFIFO | 0666, 0) < 0) {
        printf("Can\'t create FIFO!\n");
        exit(-1);
    }
    printf("FIFO is successfully created\n");
    printf("Reading data from FIFO\n");

    if ((fd = open(name, O_RDONLY)) < 0) {
        printf("Can\'t open FIFO for reading!\n");
        exit(-1);
    }
    size = read(fd, resstring, 14);

    if (size < 0) {
        printf("Can\'t read string!\n");
        exit(-1);
    }
    printf("FIFO`s data: %s\n", resstring);
    printf("Reader is done\n");
    close(fd);

    return 0;
}
