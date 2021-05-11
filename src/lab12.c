#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void) {
  int msqid;      // IPC descriptor for the message queue
  char pathname[]="lab12.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len;      // Cycle counter and the length of the informative part of the message

  pid_t my_id = getpid();
  struct mymsgbuf // Custom structure for the message
  {
    long mtype;
    struct {
        double value;
        pid_t proc_id;
    } info;
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  /* Send information */
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 1;
    double temp = 0;
    printf("Client: Type some double value: ");
    scanf("%lf", &temp);
    mybuf.info.value = temp;
    mybuf.info.proc_id = my_id;
    len = sizeof(mybuf.info);
    //
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
    printf("Client: message sent to server\n");
  
  len = sizeof(mybuf.info);
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, my_id, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("Client: squared value recieved. Message: value = %lf\n", mybuf.info.value);

  printf("Client: done.\n");

  return 0;
}