#include <dirent.h>
#include <string.h>

#include "header.h"

void create_full_path(char *env[], int env_num, char *arg0) {
  // パス解析とパス結合
  for (int i = 0; i < env_num; i++) {
    DIR *dir_pointa;
    struct dirent *dp;
    dir_pointa = opendir(env[i]);
    if (dir_pointa == NULL) {
      perror("opendir");
      exit(EXIT_FAILURE);
    }
    while ((dp = readdir(dir_pointa)) != NULL) {
      if (strncmp(arg0, dp->d_name, sizeof(256)) == 0) {
        char env_tmp[256];
        for (int j = 0; j < 256; j++) {
          env_tmp[j] = '\0';
        }
        strncpy(env_tmp, env[i], 255);
        strncat(env_tmp, "/", 2);
        strncat(env_tmp, arg0, 255);
        arg0 = &env_tmp[0];
        closedir(dir_pointa);
        return;
      }
    }
    closedir(dir_pointa);
  }
}