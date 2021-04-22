#include "header.h"

int read_cmd(char *argv[], int argc) {
  // command enter
  char command[256];
  for (int i = 0; i < 256; i++) {
    command[i] = '\0';
  }
  if (fgets(command, sizeof(command), stdin) == NULL) {
    fputs("Read Error\n", stderr);
    exit(EXIT_FAILURE);
  }
  sscanf(command, "%[^\n]", command);

  // char *argv = malloc(10 * sizeof(char *));

  return divide_space(argv, command);
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