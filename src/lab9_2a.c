#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


int main() {
  int     *array;
  int     shmid;
  int     new = 1;
  key_t   key;
  long    i;
  
  int   semid;
  char pathname[] = "lab9_2a.c";
  struct sembuf WAIT, SIGNAL; // using 2 bufs to make critical sections

  // Defining WAIT
  WAIT.sem_num = 0;
  WAIT.sem_op = -1;
  WAIT.sem_flg = 0;

  // Defining SIGNAL
  SIGNAL.sem_num = 0;
  SIGNAL.sem_op = 1;
  SIGNAL.sem_flg = 0;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  // Trying not to create semaphore twice
  if ((semid = semget(key, 1, 0666)) < 0) {
      printf("Semaphore doesnt exist. Trying create...\n");
      if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
          printf("Cant create new semaphore");
          exit(-1);
      }
      printf("Semaphore was created!\n");

      if (semop(semid, &SIGNAL, 1) < 0) {
          printf("Something went wrong\n");
          exit(-1);
      }
  }

  if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }
  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  if (semop(semid, &WAIT, 1) < 0) { // Block 
    printf("Something went wrong\n");
    exit(-1);
  }
  // Critical section
  if (new) {
    array[0] =  1;
    array[1] =  0;
    array[2] =  1;
  } else {
    array[0] += 1;
    for(i=0; i<2000000000L; i++);
    array[2] += 1;
  }
  printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
    array[0], array[1], array[2]);
  
  // Critical section
  if (semop(semid, &SIGNAL, 1) < 0) { // Unblock
 		printf("Something went wrong\n");
   	exit(-1);
  }

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }
  return 0;
}