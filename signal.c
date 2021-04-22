#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "header.h"

void signal_background(int sig) {
  pid_t all_pid;
  int status;
  while ((all_pid = waitpid(-1, &status, WNOHANG)) > 0) {
    if (WIFEXITED(status) != 0) {
      printf("pid %d finished\n", all_pid);
      delete_process(all_pid);
    }
  }
}
void signal_sigint(int sig) {
  if (foreground_check == true) {
    if (kill(foreground_pid, sig) == -1) {
      exit(EXIT_FAILURE);
    }
    printf("kill pid %d\n", foreground_pid);
    foreground_check = false;
  }
}