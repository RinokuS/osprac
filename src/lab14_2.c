
#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {
    if (nsig == 2) // CTRL-C command ID
        printf("Receive signal %d, CTRL-C pressed\n", nsig);
    else if (nsig == 3) // CTRL-4 (CTRL-\) command ID
        printf("Receive signal %d, CTRL-4 pressed\n", nsig);
}

int main(void) {
  //
  // Set the process response to the SIGINT signal
  //
  (void)signal(SIGINT, my_handler);
  //
  // Set the process response to the SIGQUIT (Ctrl + 4) signal
  //
  (void)signal(SIGQUIT, my_handler);
  //
  // From this point, the process will print SIGINT message.
  //
  while(1);
  return 0;
}