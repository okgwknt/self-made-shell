#include <string.h>

#include "../header.h"

// fg command
void fg(char *arg1) {
  child_t *fg_target; // fgにするargvを探す
  foreground_check = true;
  if (arg1 == NULL) { // 引数が指定されていない時，tailを指定する
    fg_target = tail;
    change_fg(fg_target);
  } else {
    int num = (int)strtol(arg1, NULL, 10);
    fg_target = head;
    for (int i = 1; i < num; i++) {
      fg_target = fg_target->next;
    }
    change_fg(fg_target);
  }
}
void change_fg(child_t *fg_target) {
  int wstatus;
  printf("change foreground");
  foreground_pid = fg_target->pid;
  waitpid(fg_target->pid, &wstatus, 0);
  printf("pid %d finished\n", fg_target->pid);
}