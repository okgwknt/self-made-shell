#include <dirent.h>
#include <string.h>

#include "header.h"

int create_full_path(char **argv) {

  char command[32];
  strncpy(command, argv[0], strlen(argv[0]));

  //　環境変数取得
  char *env[512];
  int env_num = get_path(env);

  for (int i = 0; i < env_num; i++) {
    DIR *dir_pointa;
    dir_pointa = opendir(env[i]);
    if (dir_pointa == NULL) {
      // printf("%s\n", env[i]);
      // perror("opendir");
      continue;
    }

    struct dirent *dp;
    while (dp = readdir(dir_pointa)) {
      if (strncmp(command, dp->d_name, 32) == 0) {
        // printf("SUCCESS\n");
        char path_tmp[256];
        strncpy(path_tmp, env[i], 128);
        strncat(path_tmp, "/", 2);
        strncat(path_tmp, command, sizeof(command));
        argv[0] = &path_tmp[0];
        // strncmp(argv[0], path_tmp, 256);

        closedir(dir_pointa);
        return 0;
      }
    }
    closedir(dir_pointa);
  }
  printf("fail\n");
  return -1;
}