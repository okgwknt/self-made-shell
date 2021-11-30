#include "header.h"

pid_t execute(char *argv[])
{
    pid_t pid = fork();

    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        return pid;
    }

    // setpgid(0, 0);

#if FLAG
    execv(argv[0], argv);
#else
    execvp(argv[0], argv);
#endif

    perror(argv[0]);
    exit(EXIT_FAILURE);
}