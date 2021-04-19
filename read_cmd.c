#include "func.h"

int read_cmd(char *argv, int argc) {
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
  for (int i = 0; i < 256; i++) {
    argv[i] = (char *)calloc(10, sizeof(char));
  }

  int argc = space_judge(argv, command);

  return argc;
}

int divide_space(char *argv[], char *buf) {
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
