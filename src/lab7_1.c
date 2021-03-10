#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main() {
    int shmid_for_size, shmid;
    char pathname[] = "lab7_1.c";
    char pathname_for_size[] = "lab7_2.c";
    key_t key_for_size, key;
    
    int file_size = 0;
    int *shared_mem_ptr_size;
    char *shared_mem_ptr;
    FILE *file;
    file = fopen(pathname, "r");
    
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file); // получаем размер файла
    rewind(file); // перемещаем указатель обратно на начало потока, ибо подсчет размера его двигает

    // генерируем ключ разделяемой памяти для переменной размера файла
    if ((key_for_size = ftok(pathname_for_size, 0)) < 0) { 
        printf("Can\'t generate key\n"); 
        exit(-1);
    }
    // генерируем ключ разделяемой памяти (позволяет нам не пересекать деятельность независимых процессов)
    if ((key = ftok(pathname, 0)) < 0) { 
        printf("Can\'t generate key\n"); 
        exit(-1);
    }

    // получаем по ключу дескриптор размером в один инт
    if ((shmid_for_size = shmget(key_for_size, sizeof(int), 0666|IPC_CREAT)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }
    // получаем по ключу дескриптор нужного размера
    if ((shmid = shmget(key, file_size * sizeof(char), 0666|IPC_CREAT)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }

    // получаем адрес нашей разделяемой памяти для работы с ней
    if ((shared_mem_ptr_size = (int *)shmat(shmid_for_size, 
                                        NULL, // желаемый адрес, куда мы хотим записать нашу разделяемую память, в нашем случае нам все равно
                                        0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    // получаем адрес нашей разделяемой памяти для работы с ней
    if ((shared_mem_ptr = (char *)shmat(shmid, 
                                        NULL, // желаемый адрес, куда мы хотим записать нашу разделяемую память, в нашем случае нам все равно
                                        0)) == (char *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    // записываем размер файла в первый блок разделяемой памяти
    *shared_mem_ptr_size = file_size;
    // записываем наш сурс код в память
    for (int i = 0; i < file_size; ++i)
        shared_mem_ptr[i] = fgetc(file); // считываем с помощью fgetc, но можно читать и read`ом "блоками" по sizeof(char) байт
        
    fclose(file);
    // детатчим разделяемую память для данного процесса
    if (shmdt(shared_mem_ptr_size) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    // детатчим разделяемую память для данного процесса
    if (shmdt(shared_mem_ptr) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    return 0;
}
