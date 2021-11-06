#include "header.h"

// global 変数
child_t *head = NULL;
child_t *tail = NULL;
int sum_child = 0; // tail number and running process
//　foregroundになっているプロセスの情報を保存
pid_t foreground_pid;
bool foreground_check = false;

int main(void)
{
  // jobs
  int status;

  // Internal command
  char *internal_command[] = {"quit", "q"};

  if (signal(SIGINT, signal_sigint) == SIG_ERR)
  {
    exit(EXIT_FAILURE);
  }
  if (signal(SIGCHLD, signal_fin) == SIG_ERR)
  {
    exit(EXIT_FAILURE);
  }

  while (1)
  {

    // char **argv = (char **)calloc(10, sizeof(char *));
    char **argv = (char **)malloc(10 * sizeof(char *));

    printf("> ");

    int argc = read_cmd(argv);

    printf("s");

    if (argc < 1)
    {
      continue;
    }

    // & check and & delete
    bool background_flag = false;
    if (argc > 1)
    {
      if (strncmp(argv[argc - 1], "&", 2) == 0)
      {
        argv[argc - 1] = NULL;
        background_flag = true;
      }
    }

    // shell終了
    if (strncmp(argv[0], internal_command[0], 5) == 0 ||
        strncmp(argv[0], internal_command[1], 2) == 0)
    {
      break;
    }

    if (strncmp(argv[0], "fg", 3) == 0)
    {
      if (sum_child > 0)
      {
        fg(argv[1]);
      }
      continue;
    }

    if (strncmp(argv[0], "jobs", 5) == 0)
    {
      jobs();
      continue;
    }

    if (strncmp(argv[0], "/", 1) != 0)
    {
      if (create_full_path(argv) == -1)
      {
        exit(EXIT_FAILURE);
      }
    }

    printf("%s\n", argv[0]);
    printf("%s\n", argv[1]);

    // generate child process
    pid_t pid = fork();

    // cannot generate child process
    if (pid < 0)
    {
      exit(EXIT_FAILURE);
    }

    // child process
    if (pid == 0)
    {
      setpgid(0, 0);
      execv(argv[0], argv);
      exit(EXIT_FAILURE);
    }

    // parent process
    if (pid > 0)
    {
      printf("start %s (pid: %d)\n", argv[0], pid);

      add_process(pid, background_flag);

      // wait child process Foreground
      if (!background_flag)
      {
        foreground_check = true;
        foreground_pid = pid;
        // Foreground do
        pid = waitpid(pid, &status, 0);
        foreground_check = false;
      }
    }
    // for (int i = 0; i < 10; i++)
    // {
    //   free(argv[i]);
    // }
    // free(argv);
  }
  return 1;
}
