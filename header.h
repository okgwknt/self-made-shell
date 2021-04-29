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
extern child_t *head;
extern child_t *tail;
extern int sum_child; // tail number and running process
//　foregroundになっているプロセスの情報を保存
extern pid_t foreground_pid;
extern bool foreground_check;

// child_processのヘッダ
void add_process(pid_t pid, bool background_flag);
void delete_process(pid_t pid);

// read_cmdのヘッダ
int read_cmd(char *argv[]);
int divide_space(char *argv[], char *buf);

// get_pathのヘッダ
int get_path(char *env[]);
int divide_colon(char *env[], char *strenv);

// create_full_pathのヘッダ
void create_full_path(char *env[], int env_num, char *argv[]);

// signalfilenoのヘッダ
void signal_fin(int sig);
void signal_sigint(int sig);

// jobsとfgのヘッダ
void jobs();
void fg(char *arg1);
void change_fg(child_t *fg_target);

#endif
