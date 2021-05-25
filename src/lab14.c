#include <signal.h>

int main(void) {
  //
  // Set the process response to the SIGINT signal to ignore
  //
  (void)signal(SIGINT, SIG_IGN);
  //
  // Set the process response to the SIGQUIT (Ctrl + 4) signal to ignore
  //
  (void)signal(SIGQUIT, SIG_IGN);
  //
  // From this point, the process will ignore the occurrence of the SIGINT signal.
  //
  while(1);
  return 0;
}