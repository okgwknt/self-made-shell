#include "header.h"

int get_path(char *env[])
{
  char *strenv;
  if ((strenv = getenv("PATH")) == NULL)
  {
    printf("error enviroment variable fail\n");
  }

  return divide_colon(env, strenv);
}
int divide_colon(char *env[], char *strenv)
{
  int i;
  for (i = 0; *strenv != '\0'; i++)
  {
    while (*strenv == ':')
    {
      *strenv = '\0';
      strenv++;
    }

    if (*strenv == '\0')
    {
      break;
    }
    env[i] = strenv;
    while (*strenv != '\0' && *strenv != ':')
    {
      strenv++;
    }
  }
  env[i] = NULL;
  return i;
}