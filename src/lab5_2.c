#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int parent[2], child[2], result;
    size_t size;
    char resstring[14];

    // Создаём Pipe родителя и ребёнка:
    if (pipe(parent) < 0) {
        printf("Can\'t create parent\'s pipe\n");
        exit(-1);
    }
    if (pipe(child) < 0) {
        printf("Can\'t create child\'s pipe\n");
        exit(-1);
    }

    result = fork();
    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {
        // PARENT
        // Закрываем на чтение родителя
        close(parent[0]);
        // Закрываем на запись ребёнка
        close(child[1]);
        // Пишем HelloWorld в родителя
        size = write(parent[1], "Hello, world!", 14);
    
        if (size != 14) {
            printf("Can\'t write all string\n");
            exit(-1);
        }
        // Закрываем запись родителя
        close(parent[1]);

        // Читаем ребёнка
        size = read(child[0], resstring, 14);
        if (size != 14) {
            printf("Can\'t read from child\n");
            exit(-1);
        }

        close(child[0]);
        printf("Parent read from child: %s\nParent is done.\n", resstring);
    } else {
        // CHILD
        // Закрывем на запись родителя
        close(parent[1]);
        // Закрываем на чтение ребёнка
        close(child[0]);
        // Читаем родителя
        size = read(parent[0], resstring, 14);
        if (size != 14) {
            printf("Can\'t read from parent\n");
            exit(-1);
        }

        // Печатаем, что прочитали:
        printf("Child read from parent: %s\n", resstring);
        // Пишем, что ребенок думает о мире:
        size = write(child[1], "World is hell", 14);
        
        close(child[1]);
        close(parent[0]);
        printf("Child is done.\n");
    }
    return 0;
}