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

typedef struct _child {
  pid_t pid;
  bool fg_status;
  struct _child *next;
} child_t;

// global var
child_t *head = NULL;
child_t *tail = NULL;
int child_number = 0; // tail number or running process

int space_judge(char *argv[], char *buf);
int colon_judge(char *env[], char *strenv);
void add_process(pid_t pid, bool background_flag);
void delete_process(pid_t pid);

void signal_background(int sig) {
  pid_t all_pid;
  int status;
  while ((all_pid = waitpid(-1, &status, WNOHANG)) > 0) {
    if (WIFEXITED(status) != 0) {
      printf("pid %d finished\n", all_pid);
      delete_process(all_pid);
      child_number--;
    }
  }
}
pid_t foreground_pid;
bool foreground_check = false;
void signal_sigint(int sig) {
  if (foreground_check == true) {
    if (kill(foreground_pid, sig) == -1) {
      exit(EXIT_FAILURE);
    }
    printf("kill pid %d\n", foreground_pid);
    foreground_check = false;
  }
}

int main() {
  // jobs
  pid_t pid;
  int status;
  // Internal command
  char *internal_command[] = {"quit", "exit", "q", "jobs", "fg"};
  // signal
  if (signal(SIGINT, signal_sigint) == SIG_ERR) {
    exit(EXIT_FAILURE);
  }
  // background finished signal
  if (signal(SIGCHLD, signal_background) == SIG_ERR) {
    exit(EXIT_FAILURE);
  }

  // environment variable
  char *strenv;
  if ((strenv = getenv("PATH")) == NULL) {
    printf("error enviroment variable fail\n");
  }
  // : separation
  char *env[512];
  int env_num;
  env_num = colon_judge(env, strenv);

  while (1) {
    // sleep(1);
    printf("> ");
    // command enter
    char line[256], command[256];
    for (int i = 0; i < 256; i++) {
      command[i] = '\0';
    }
    if (fgets(line, sizeof(line), stdin) == NULL) {
      fputs("Read Error\n", stderr);
      exit(EXIT_FAILURE);
    }
    sscanf(line, "%[^\n]", command);
    // argv
    char *argv[256];
    for (int i = 0; i < 256; i++) {
      argv[i] = (char *)calloc(10, sizeof(char));
    }

    int argc = space_judge(argv, command);

    // enter continue
    if (argv[0] == NULL) {
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

    // internal command run
    // finish shell
    if (strncmp(argv[0], internal_command[0], 10) == 0 ||
        strncmp(argv[0], internal_command[1], 10) == 0 ||
        strncmp(argv[0], internal_command[2], 10) == 0) {
      break;
    }
    // jobs command
    child_t *target = head;
    if (strncmp(argv[0], internal_command[3], 4) == 0) {
      // BG pid display
      printf("PID\n");
      int count = 1;
      while (target != NULL) {
        printf("%d: pid = %d\n", count, target->pid);
        target = target->next;
        count++;
      }
      continue;
    }
    // fg command
    child_t *fg_target;
    if (strncmp(argv[0], internal_command[4], 3) == 0) {
      foreground_check = true;
      if (argv[1] == NULL) {
        // running tail
        fg_target = tail;
        // foreground do
        printf("change foreground");
        foreground_pid = fg_target->pid;
        waitpid(fg_target->pid, &status, 0);
        printf("pid %d finished\n", fg_target->pid);
        delete_process(fg_target->pid);
      } else {
        // list running true number
        int count = (int)strtol(argv[1], NULL, 10);
        fg_target = head;
        for (int i = 1; i < count; i++) {
          fg_target = fg_target->next;
        }
        // foreground do
        printf("change foreground");
        foreground_pid = fg_target->pid;
        waitpid(fg_target->pid, &status, 0);
        printf("pid %d finished\n", fg_target->pid);
        delete_process(fg_target->pid);
      }
      continue;
    }

    // path check
    if (strncmp(argv[0], "/", 1) != 0) {
      for (int i = 0; i < env_num; i++) {
        DIR *dir_pointa;
        struct dirent *dp;
        if ((dir_pointa = opendir(env[i])) == NULL) {
          perror("opendir");
          exit(EXIT_FAILURE);
        }
        while ((dp = readdir(dir_pointa)) != NULL) {
          if (strncmp(argv[0], dp->d_name, sizeof(256)) == 0) {
            char env_tmp[256];
            for (int j = 0; j < 256; j++) {
              env_tmp[j] = '\0';
            }
            strncpy(env_tmp, env[i], 255);
            strncat(env_tmp, "/", 2);
            strncat(env_tmp, argv[0], 255);
            argv[0] = &env_tmp[0];
            closedir(dir_pointa);
            goto loop;
          }
        }
        closedir(dir_pointa);
      }
    }

  loop:

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
      printf("start %s (pid: %d)\n", command, pid);
      add_process(pid, background_flag);
      child_number++;

      // wait child process Foreground
      if (background_flag == false) {
        foreground_check = true;
        foreground_pid = pid;
        // Foreground do
        pid = waitpid(pid, &status, 0);
      }
    }
    // free(argv);
    foreground_check = false;
  }
  return EXIT_SUCCESS;
}

void delete_process(pid_t pid) {
  child_t *target = head;
  while (1) {
    if (target == NULL) {
      printf("Not found");
      exit(EXIT_FAILURE);
    }
    if (target->pid == pid) {
      break;
    }
    target = target->next;
  }
  // delete process
  printf("pid %d delete process\n", target->pid);
  if (target->pid == head->pid) {
    child_t *new = head->next;
    free(head);
    head = new;
  } else {
    child_t *new = head;
    while (new->next != target) {
      new = new->next;
    }
    new->next = target->next;
    free(target);
    target = new;
  }
  return;
}
void add_process(pid_t pid, bool background_flag) {
  child_t *x = malloc(1 * sizeof(child_t));
  // set pid ID
  x->pid = pid;
  // FG or BG
  if (background_flag == true) {
    x->fg_status = false;
  } else {
    x->fg_status = true;
  }
  // next
  x->next = NULL;
  // linear list
  if (head == NULL) {
    head = x;
    tail = x;
  } else {
    tail->next = x;
    tail = x;
  }
}
int space_judge(char *argv[], char *buf) {
  int i;

  for (i = 0; *buf != '\0'; i++) {
    while (*buf == ' ') {
      *buf = '\0';
      buf++;
    }

    if (*buf == '\0') {
      break;
    }
    argv[i] = buf;
    while (*buf != '\0' && *buf != ' ') {
      buf++;
    }
  }

  argv[i] = NULL;
  return i;
}
int colon_judge(char *env[], char *strenv) {
  int i;
  for (i = 0; *strenv != '\0'; i++) {
    while (*strenv == ':') {
      *strenv = '\0';
      strenv++;
    }

    if (*strenv == '\0') {
      break;
    }
    env[i] = strenv;
    while (*strenv != '\0' && *strenv != ':') {
      strenv++;
    }
  }
  env[i] = NULL;
  return i;
}