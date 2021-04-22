#include "../header.h"

void jobs() {
  printf("PID\n");
  int count = 1;
  child_t *target = head;
  while (target != NULL) {
    printf("%d: pid = %d\n", count, target->pid);
    target = target->next;
    count += 1;
  }
}