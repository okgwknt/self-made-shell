#include "header.h"
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void signal_int(int sig) { printf("*"); }

int main(void) {
  int status;
  if (signal(SIGCHLD, signal_int) == SIG_ERR) {
  }
  while (1) {
    char *argv[16] = {"/bin/ls", "."};

    pid_t pid;
    pid = fork();
    printf("%s\n", argv[0]);
    if (pid == 0) {
      execv(argv[0], argv);
    }
    if (pid > 0) {
      printf("parent %s\n", argv[0]);
      pid = waitpid(pid, &status, 0);
    }
    char buf[15];
    scanf("%s", buf);
  }
}