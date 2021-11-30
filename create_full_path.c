#include "header.h"

int create_full_path(char **argv)
{

  char command[64];
  strncpy(command, argv[0], strlen(command));

  for (int i = 0; i < env_num; i++)
  {
    DIR *dir_pointa;
    dir_pointa = opendir(env[i]);
    if (dir_pointa == NULL)
    {
      // perror("opendir");
      continue;
    }

    struct dirent *dp;
    while (dp = readdir(dir_pointa))
    {
      if (strncmp(command, dp->d_name, strlen(command)) == 0)
      {
        char *path = malloc(sizeof(char) * 1024);
        strncpy(path, env[i], sizeof(env[i]));
        strncat(path, "/", 1);
        strncat(path, command, sizeof(command));
        strncpy(argv[0], path, sizeof(path));
        free(path);
        // argv[0][sizeof(path)] = '\0';
        closedir(dir_pointa);
        return 0;
      }
    }
    closedir(dir_pointa);
  }
  return -1;
}