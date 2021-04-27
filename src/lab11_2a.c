#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void) {
  int msqid;      // IPC descriptor for the message queue
  char pathname[]="lab11_2a.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len;      // Cycle counter and the length of the informative part of the message
  struct mymsgbuf // Custom structure for the message
  {
    long mtype;
    struct {
        short sinfo;
        float finfo;
    } info;
  } mybuf;

  double pi_number[] = {3.14, 3.141, 3.1415, 3.14159, 3.141592};

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

  printf("First program: start sending messages.\n");
  /* Send information */
  for (i = 1; i <= 5; i++) {
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 1;
    mybuf.info.sinfo = 2021;
    mybuf.info.finfo = pi_number[i-1];
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
    printf("First programm: message %i sent\n", i);
  }

  printf("First programm: all messages sent. Start recieving messages.\n");
  
  for (size_t i = 1; i <= 5; i++) {
    len = sizeof(mybuf.info);
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 2, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("First program: message of type %ld recieved. Message: sinfo = %i, finfo = %f\n", mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
  }

  msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

  printf("First program: done.\n");

  return 0;
}