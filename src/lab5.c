#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd;
    size_t size;

    umask(0);

    // Открываем с O_RDONLY
    if ((fd = open("myfile", O_RDONLY)) < 0) {
        printf("Can\'t open your file!\n");
        exit(-1);
    }

    // 14 - размер записанной ранее строки.
    char string[14];
    size = read(fd, string, 14);

    if (size != 14) {
        printf("Something wrong with string!\n");
        exit(-1);
    }

    printf("%s\n", string);

    // Закрываем поток:
    if (close(fd) < 0) 
        printf("Can\'t close your file!\n");

    return 0;
}