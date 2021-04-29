#include <dirent.h>
#include <string.h>

#include "header.h"

void create_full_path(char *env[], int env_num, char *argv[]) {
  for (int i = 0; i < env_num; i++) {
    DIR *dir_pointa;
    struct dirent *dp;
    dir_pointa = opendir(env[i]);
    if (dir_pointa == NULL) {
      printf("%s\n", env[i]);
      perror("opendir");
      continue;
    }
    while ((dp = readdir(dir_pointa)) != NULL) {
      if (strncmp(argv[0], dp->d_name, sizeof(char) * 256) == 0) {
        printf("SUCCESS\n");
        char *path_tmp;
        strncpy(path_tmp, env[i], 128);
        strncat(path_tmp, "/", 2);
        strncat(path_tmp, argv[0], sizeof(argv[0]));
        printf("%s\n", path_tmp);
        // strncpy(argv[0], path_tmp, 64);
        argv[0] = &path_tmp[0];
        closedir(dir_pointa);
        printf("%s\n", argv[0]);
        return;
      }
    }
    closedir(dir_pointa);
  }
  printf("FAIL\n");
  return;
}