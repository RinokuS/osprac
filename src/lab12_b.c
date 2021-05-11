#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void) {
  int msqid;
  char pathname[]="lab12.c";
  key_t  key;
  int len, maxlen;
  struct mymsgbuf {
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
  if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0) { // флаг для первого пункта доп. задания
    printf("Can\'t get msqid\n");
    exit(-1);
  }


  while(1) {
    printf("Server: start recieving messages.\n");
    len = sizeof(mybuf.info);
    
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 1, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("Server: message of type %ld recieved. Message: value = %lf\n", mybuf.mtype, mybuf.info.value);

    mybuf.mtype = mybuf.info.proc_id;
    mybuf.info.value *= mybuf.info.value;

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
    printf("Server: message sent back to client.\n");

  }

    printf("Server is done");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
  return 0;
}