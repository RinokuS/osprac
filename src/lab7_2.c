#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main() {
    int *array;
    int shmid;
    char pathname[] = "lab7_1.c";
    key_t key;
    char *source_code;
    
    int file_size = 0;
    int *shared_mem_ptr;
    // генерируем ключ разделяемой памяти (позволяет нам не пересекать деятельность независимых процессов)
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    // получаем по ключу дескриптор нужного размера, в этот раз в флаги передаем 0, ибо память уже создана
    if ((shmid = shmget(key, sizeof(int) + file_size * sizeof(char), 0)) < 0) {
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
    // нагло крадем с первого слота разделяемой памяти размер файла для чтения (ибо мы также нагло его туда записали)
    file_size = *shared_mem_ptr;
    // двигаем наш указатель на один инт, ибо там хранится размер файла, и приводим к чару для верной дальнейшей итерации
    source_code = (char*)(shared_mem_ptr + 1);
    // теперь итерируемся по оставшейся памяти и считываем сам файл 
    for (int i = 0; i < file_size; i++)
        printf("%c", source_code[i]);
    // детатчим разделяемую память для данного процесса
    if (shmdt(shared_mem_ptr) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    // удаляем разделяемую память
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        printf("Can't delete shared memory\n");
        exit(-1);
    }

    return 0;
}
