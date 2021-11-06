#include "header.h"

void delete_process(pid_t pid)
{
  child_t *target = head;
  // 終了するpidの探索
  while (1)
  {
    if (target == NULL)
    {
      printf("Not found");
      exit(EXIT_FAILURE);
    }
    if (target->pid == pid)
    {
      break;
    }
    target = target->next;
  }
  // delete process
  printf("pid %d delete process\n", target->pid);
  if (target->pid == head->pid)
  {
    child_t *new = head->next;
    free(head);
    head = new;
  }
  else
  {
    child_t *new = head;
    while (new->next != target)
    {
      new = new->next;
    }
    new->next = target->next;
    free(target);
    target = new;
  }
  sum_child -= 1;
  return;
}

void add_process(pid_t pid, bool background_flag)
{
  child_t *target = malloc(1 * sizeof(child_t));
  // set pid ID
  target->pid = pid;
  // FG or BG
  if (background_flag == true)
  {
    target->fg_status = false;
  }
  else
  {
    target->fg_status = true;
  }

  target->next = NULL;
  if (sum_child == 0)
  {
    head = target;
    tail = target;
  }
  else
  {
    tail->next = target;
    tail = target;
  }
  sum_child += 1;
}
