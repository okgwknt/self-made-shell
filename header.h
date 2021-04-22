#ifndef HEADER_H // フラグが立っていなければ（ if HEADER_H is not defined ）
#define HEADER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _child {
  pid_t pid;
  bool fg_status;
  struct _child *next;
} child_t;

// global 変数
child_t *head = NULL;
child_t *tail = NULL;
int sum_child = 0; // tail number and running process
int status;
//　foregroundになっているプロセスの情報を保存
pid_t foreground_pid;
bool foreground_check = false;

// child_processのヘッダ
void add_process(pid_t pid, bool background_flag);
void delete_process(pid_t pid);

// read_cmdのヘッダ
int read_cmd(char *argv[], int argc);
int divide_space(char *argv[], char *buf);

// get_pathのヘッダ
int get_path(char *env[]);
int divide_colon(char *env[], char *strenv);

// signalfilenoのヘッダ
void signal_background(int sig);
void signal_sigint(int sig);

// jobsとfgのヘッダ
extern void jobs();
extern void fg(char *arg1);
extern void change_fg(child_t *fg_target);

#endif
