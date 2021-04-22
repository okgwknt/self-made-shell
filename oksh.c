#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "header.h"

// global 変数
child_t *head = NULL;
child_t *tail = NULL;
int sum_child = 0; // tail number and running process
int status;
//　foregroundになっているプロセスの情報を保存
pid_t foreground_pid;
bool foreground_check = false;

int main(void) {
  // jobs
  pid_t pid;

  // Internal command
  char *internal_command[] = {"quit", "exit", "q"};

  // ^C 取得
  if (signal(SIGINT, signal_sigint) == SIG_ERR) {
    exit(EXIT_FAILURE);
  }
  // バックグラウンドプロセス終了取得
  if (signal(SIGCHLD, signal_background) == SIG_ERR) {
    exit(EXIT_FAILURE);
  }

  //　環境変数取得
  char *env[512];
  int env_num = get_path(env);

  while (1) {
    char *argv[256];
    int argc = 0;

    printf("> ");

    argc = read_cmd(argv);

    if (argc < 1) {
      continue;
    }

    // & check and & delete
    bool background_flag = false;
    if (argc > 1) {
      if (strncmp(argv[argc - 1], "&", 2) == 0) {
        argv[argc - 1] = NULL;
        background_flag = true;
      }
    }

    // shell終了
    if (strncmp(argv[0], internal_command[0], 5) == 0 ||
        strncmp(argv[0], internal_command[1], 5) == 0 ||
        strncmp(argv[0], internal_command[2], 5) == 0) {
      break;
    }
    if (strncmp(argv[0], "fg", 3) == 0) {
      fg(argv[1]);
      continue;
    }
    if (strncmp(argv[0], "jobs", 5) == 0) {
      jobs();
      continue;
    }
    if (strncmp(argv[0], "/", 1) != 0) {
      create_full_path(env, env_num, argv[0]);
    }

    // generate child process
    pid = fork();

    // cannot generate child process
    if (pid == -1) {
      exit(EXIT_FAILURE);
    }
    // which process judge
    if (pid == 0) {
      setpgid(0, 0);
      // child shell do
      execv(argv[0], argv);
      exit(EXIT_FAILURE);
    }
    if (pid > 0) {
      printf("start %s (pid: %d)\n", argv[0], pid);
      add_process(pid, background_flag);

      // wait child process Foreground
      if (background_flag == false) {
        foreground_check = true;
        foreground_pid = pid;
        // Foreground do
        pid = waitpid(pid, &status, 0);
      }
    }
    foreground_check = false;
  }
  return EXIT_SUCCESS;
}
