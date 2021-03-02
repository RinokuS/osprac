#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

int res; // глобальные переменные можно не инициализировать

int main() {
    int fd[2];
    char* resstring = malloc(1 * sizeof(char));

    if (pipe(fd) < 0) { // Создаем пайп
        printf("Can\'t create the pipe!\n");
        exit(-1);
    }
    fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);
    
    size_t size = write(fd[1], resstring, 1); // первый вызов позволит нам зайти в цикл, чтобы начать увеличивать размерность
    while (size == 1) {
        size = write(fd[1], resstring, 1); // далее продолжаем вызывы, пока цикл не оборвется
        res++;
    }
    printf("Size: %d bytes\n", res);

    return 0;
}
