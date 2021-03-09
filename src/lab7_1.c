#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main() {
    int shmid;
    char pathname[] = "lab7_1.c";
    key_t key;
    char *source_code;
    
    int file_size = 0;
    int *shared_mem_ptr;
    FILE *file;
    file = fopen(pathname, "r");
    
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file); // получаем размер файла
    rewind(file); // перемещаем указатель обратно на начало потока, ибо подсчет размера его двигает
    // генерируем ключ разделяемой памяти (позволяет нам не пересекать деятельность независимых процессов)
    if ((key = ftok(pathname, 0)) < 0) { 
        printf("Can\'t generate key\n"); 
        exit(-1);
    }
    // получаем по ключу дескриптор нужного размера
    if ((shmid = shmget(key, sizeof(int) + file_size * sizeof(char), 0666|IPC_CREAT)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }
    // получаем адрес нашей разделяемой памяти для работы с ней
    if ((shared_mem_ptr = (int *)shmat(shmid, 
                                        NULL, // желаемый адрес, куда мы хотим записать нашу разделяемую память, в нашем случае нам все равно
                                        0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    // в первый слот памяти записываем размер файла, для того мы и выделили дополнительные sizeof(int) байт
    *shared_mem_ptr = file_size;
    // сначала двигаем наш указатель на один инт, ибо там хранится размер файла, и приводим к чару для верной дальнейшей итерации
    source_code = (char*)(shared_mem_ptr + 1); 
    // записываем наш сурс код в память
    for (int i = 0; i < file_size; ++i)
        source_code[i] = fgetc(file); // считываем с помощью fgetc, но можно читать и read`ом "блоками" по sizeof(char) байт
        
    fclose(file);
    // детатчим разделяемую память для данного процесса
    if (shmdt(shared_mem_ptr) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    return 0;
}
