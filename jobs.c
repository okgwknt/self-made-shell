#include "func.h"

void jobs(char *argv) {
  printf("PID\n");
  int count = 1;
  child_t *target = head;
  while (target != NULL) {
    printf("%d: pid = %d\n", count, target->pid);
    target = target->next;
    count++;
  }
}