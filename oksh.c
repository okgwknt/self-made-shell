#include "func.h"
#include "get_path.c"
#include "read_cmd.c"

#include "fg.c"
#include "jobs.c"

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

  /*


  */
  while (1) {
    char **argv;
    int argc;

    printf("> ");

    argc = read_cmd(&argc, argc);

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
      fg(&argv);
      continue;
    }
    if (strncmp(argv[0], "jobs", 4) == 0) {
      jobs(&argv);
      continue;
    }

    // パス解析とパス結合
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
      printf("start %s (pid: %d)\n", argv[0], pid);
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
