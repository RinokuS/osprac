#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main() {
    int *array;
    int shmid_for_size, shmid;
    char pathname[] = "lab7_1.c";
    char pathname_for_size[] = "lab7_2.c";
    key_t key_for_size, key;
    
    int file_size = 0;
    int *shared_mem_ptr_size;
    char *shared_mem_ptr;

    // генерируем ключ разделяемой памяти (позволяет нам не пересекать деятельность независимых процессов)
    if ((key = ftok(pathname, 0)) < 0 || (key_for_size = ftok(pathname_for_size, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    // получаем по ключу дескриптор размера в один инт
    if ((shmid_for_size = shmget(key_for_size, sizeof(int), 0)) < 0) {
        printf("Can\'t create shared memory!\n");
        exit(-1);
    }
    // получаем адрес нашей разделяемой памяти для работы с ней
    if ((shared_mem_ptr_size = (int *)shmat(shmid_for_size, 
                                        NULL, // желаемый адрес, куда мы хотим записать нашу разделяемую память, в нашем случае нам все равно
                                        0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    // нагло крадем из разделяемой памяти размер файла для чтения (ибо мы также нагло его туда записали)
    file_size = *shared_mem_ptr_size;

    // получаем по ключу дескриптор нужного размера, в этот раз в флаги передаем 0, ибо память уже создана
    if ((shmid = shmget(key, file_size * sizeof(char), 0)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }
    // получаем адрес нашей разделяемой памяти для работы с ней
    if ((shared_mem_ptr = (char *)shmat(shmid, 
                                        NULL, // желаемый адрес, куда мы хотим записать нашу разделяемую память, в нашем случае нам все равно
                                        0)) == (char *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    // теперь итерируемся по памяти и считываем сам файл 
    for (int i = 0; i < file_size; i++)
        printf("%c", shared_mem_ptr[i]);
    // детатчим разделяемую память для данного процесса
    if (shmdt(shared_mem_ptr) < 0 || shmdt(shared_mem_ptr_size) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    // удаляем разделяемую память
    if (shmctl(shmid, IPC_RMID, NULL) < 0 || shmctl(shmid_for_size, IPC_RMID, NULL) < 0) {
        printf("Can't delete shared memory\n");
        exit(-1);
    }

    return 0;
}
