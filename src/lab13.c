#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define MAX_SIZE 100 

char alp[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                't', 'u', 'v', 'w', 'x', 'y', 'z'};

int main(void) {
    const char first_file_name[] = "a";
    char pathname[MAX_SIZE] = "", pathname2[MAX_SIZE] = ""; // вспомогательные строки для хранения названий файлов-связей
    strcat(pathname, first_file_name);
    strcat(pathname2, first_file_name);
    int counter = 0, counter2 = 1, rec_depth = 0;
    FILE *f = fopen(first_file_name, "w+"); // создаем файл 'a' если его нет
    fclose (f);

    while (1) {
        if (counter2 < strlen(alp)) { // костыль для изменения названий последующий файлов
            pathname2[counter] = alp[counter2++];
        } else {
            pathname2[++counter] = alp[0];
            counter2 = 1;
        }

        symlink(pathname, pathname2); // создаем связь

        for(int i=0; pathname2[i]!='\0'; i++) {
            pathname[i] = pathname2[i]; // копируем вторую строку в первую для продолжения цикла создания связей
        }

        FILE *f;
        if ((f=fopen(pathname, "w+")) == NULL) { // пытаемся открыть файл
            fclose (f);
            printf("Recursion depth: %i\n", rec_depth); // выводим получившуюся глубину
            exit(-1);
        }
        fclose (f);
        rec_depth++; // если файл открылся успешно, увеличиваем допустимую глубину
    }

    return 0;
}