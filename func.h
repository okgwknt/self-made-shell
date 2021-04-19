#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef HEADER_H // フラグが立っていなければ（ if HEADER_H is not defined ）
#define HEADER_H
typedef struct _child {
  pid_t pid;
  bool fg_status;
  struct _child *next;
} child_t;
#endif

// global var
extern child_t *head = NULL;
extern child_t *tail = NULL;
extern int child_number = 0; // tail number or running process
extern int status;

extern pid_t foreground_pid;
extern bool foreground_check = false;

void add_process(pid_t pid, bool background_flag);
void delete_process(pid_t pid);

void signal_background(int sig);
void signal_sigint(int sig);
