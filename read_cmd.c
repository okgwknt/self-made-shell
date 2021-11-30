#include "header.h"

int read_cmd(char **argv)
{
  // command enter
  char *input = malloc(sizeof(char) * 1024);

  if (fgets(input, 1024, stdin) == NULL)
  {
    fputs("Read Error\n", stderr);
    exit(EXIT_FAILURE);
  }
  input[strlen(input) - 1] = '\0';

  int argc = divide_space(argv, input);

  argv[argc] = NULL;

  return argc;
}

int divide_space(char **argv, char *buf)
{

  int size = sizeof(*argv) / sizeof(char);
  for (int i = 0; i < size; i++)
    argv[i] = NULL;

  char *word;
  int num_word = 0;
  word = strtok(buf, " ");
  while (word != 0)
  {
    argv[num_word] = word;
    word = strtok(NULL, " ");
    num_word += 1;
  }

  return num_word;
}