#include "header.h"

void signal_fin(int sig) {
  pid_t fin_pid;
  int fin_status;
  while ((fin_pid = waitpid(-1, &fin_status, WNOHANG)) > 0) {
    if (WIFEXITED(fin_status) != 0) {
      printf("pid %d finished\n", fin_pid);
      delete_process(fin_pid);
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