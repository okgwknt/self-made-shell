#include "func.h"

void fg(char *argv) {
  // fg command
  child_t *fg_target;
  foreground_check = true;
  if (argv[1] == NULL) {
    // running tail
    fg_target = tail;
    // foreground do
    printf("change foreground");
    foreground_pid = fg_target->pid;
    waitpid(fg_target->pid, &status, 0);
    printf("pid %d finished\n", fg_target->pid);
    delete_process(fg_target->pid);
  } else {
    // list running true number
    int count = (int)strtol(argv[1], NULL, 10);
    fg_target = head;
    for (int i = 1; i < count; i++) {
      fg_target = fg_target->next;
    }
    // foreground do
    printf("change foreground");
    foreground_pid = fg_target->pid;
    waitpid(fg_target->pid, &status, 0);
    printf("pid %d finished\n", fg_target->pid);
    delete_process(fg_target->pid);
  }
}