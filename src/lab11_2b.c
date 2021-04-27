#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void) {
  int msqid;
  char pathname[]="lab11_2a.c";
  key_t  key;
  int len, maxlen;
  struct mymsgbuf {
    long mtype;
    struct {
        short sinfo;
        float finfo;
    } info;
  } mybuf;

  double e_number[] = {2.71, 2.718, 2.7182, 2.71828, 2.718281};

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  printf("Second program: start recieving messages.\n");
  for (size_t i = 1; i <= 5; i++) {
    len = sizeof(mybuf.info);
    
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 1, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("Second program: message of type %ld recieved. Message: sinfo = %i, finfo = %f\n", mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
  }

  printf("Second programm: all messages recieved. Start sending messages.\n");
  for (int i = 1; i <= 5; i++) {
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 2;
    mybuf.info.sinfo = 2012;
    mybuf.info.finfo = e_number[i-1];
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
    printf("Second programm: message %i sent\n", i);
  }

  printf("Second program: done.\n");
  
  return 0;
}