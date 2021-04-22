#include <string.h>

#include "header.h"

int read_cmd(char *argv[]) {
  // command enter
  char command[256];

  if (fgets(command, sizeof(command), stdin) == NULL) {
    fputs("Read Error\n", stderr);
    exit(EXIT_FAILURE);
  }

  command[strlen(command) - 1] = '\0';

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