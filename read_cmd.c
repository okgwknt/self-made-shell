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

  argv[argc] = '\0';

  return argc;
}

int divide_space(char **argv, char *buf)
{

  int size = sizeof(*argv) / sizeof(char);
  for (int i = 0; i < size; i++)
    argv[i] = NULL;

  // 引数に分割
  char *word;       /* 分割済み文字列 */
  int num_word = 0; /* wordの個数 */
  word = strtok(buf, " ");
  while (word != 0)
  {
    argv[num_word] = word;
    word = strtok(NULL, " ");
    num_word += 1;
  }

  return num_word;
}