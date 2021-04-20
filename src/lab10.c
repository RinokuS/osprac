#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/sem.h>


void semop_function(int sem_id, int sem_op) {
    struct sembuf temp_buf;
    temp_buf.sem_num = 0;
    temp_buf.sem_flg = 0;
    temp_buf.sem_op  = sem_op;

    if (semop(sem_id, &temp_buf, 1) < 0) {
        printf("Can\'t wait for condition\n");
        exit(-1);
    }
}
// Incrementer
void A(int sem_id, int value) {
    semop_function(sem_id, value);
}
// Decrementer 
void D(int sem_id, int value) {
    semop_function(sem_id, -value);
}
// Zero checker
void Z(int sem_id) {
    semop_function(sem_id, 0);
}

int main() {
    int fd[2], result, N = 0, sem_id;
    size_t size = 0;
    key_t key;
    char pathname[] = "lab10.c";
    char resstring[14];

    // trying to scan N value
    do {
        printf("Enter N (N >= 2): \n");
        scanf("%d", &N);
    } while(N < 2);

    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((sem_id = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Can\'t create semaphore set\n");
            exit(-1);
        } else if ((sem_id = semget(key, 1, 0)) < 0) {
            printf("Can\'t find semaphore\n");
            exit(-1);
        } else {
            A(sem_id, 2);
        }
    } else {
        A(sem_id, 2);
    }

    /**
     * Idea: Entering and exiting a function for parent will cost 1, so while semaphore is above zero
     * we can either enter or exit the function. We will init semaphore with 2, so parent can do both operations.
     * Child will enter his function only if semaphore is 0, this will mean that parent is already done.
     */
    result = fork();
    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {
        /* Parent process */
        for (int i = 0; i <= N; i++) {
            D(sem_id, 1);
            /* Reading */
            if (i > 0) { // can't read anything on first step
                size = read(fd[0], resstring, 14);

                if (size < 0) {
                    printf("Can\'t read string from pipe\n");
                    exit(-1);
                }
                printf("Parent read message №%d: %s\n", i, resstring);
            }

            /* Writing */
            if (i < N) { // shouldn't write anything on last step
                size = write(fd[1], "Hello, world!", 14);
                
                if (size != 14) {
                    printf("Can\'t write all string to pipe\n");
                    exit(-1);
                }
            }
            D(sem_id, 1);
        }
        
        if (close(fd[0]) < 0) {
            printf("parent: Can\'t close reading side of pipe\n"); 
            exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("parent: Can\'t close writing side of pipe\n"); 
            exit(-1);
        }
        printf("Parent exit\n");
    } else {
        /* Child process */
        for (int i = 0; i < N; i++) {
            Z(sem_id);
            /* Reading */
            size = read(fd[0], resstring, 14);

            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }
            printf("Child read message №%d: %s\n", i + 1, resstring);

            /* Writing */
            size = write(fd[1], "Hello, parent", 14);
            
            if (size != 14) {
                printf("Can\'t write all string to pipe\n");
                exit(-1);
            }
            A(sem_id, 2);
        }

        if (close(fd[1]) < 0) {
            printf("child: Can\'t close writing side of pipe\n"); 
            exit(-1);
        }
        if (close(fd[0]) < 0) {
            printf("child: Can\'t close reading side of pipe\n"); 
            exit(-1);
        }
        printf("Child exit\n");
    }

    return 0;
}