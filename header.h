#ifndef HEADER_H // フラグが立っていなければ（ if HEADER_H is not defined ）
#define HEADER_H

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

#define FLAG 0

typedef struct _child
{
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

extern char *env[512];
extern int env_num;

// child_processのヘッダ
void add_process(pid_t pid, bool background_flag);
void delete_process(pid_t pid);

// read_cmdのヘッダ
int read_cmd(char **argv);
int divide_space(char **argv, char *buf);

// get_pathのヘッダ
int get_path(char *env[]);
int divide_colon(char *env[], char *strenv);

// create_full_pathのヘッダ
int create_full_path(char **argv);

// signalfilenoのヘッダ
void signal_fin(int sig);
void signal_sigint(int sig);

// jobsとfgのヘッダ
void jobs();
void fg(char *arg1);
void change_fg(child_t *fg_target);

pid_t execute(char **argv);

#endif
