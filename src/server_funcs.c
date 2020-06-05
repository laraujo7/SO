#include "../includes/server_funcs.h"

int time_inactivity(int sec)
{
    return 0;    
}

int time_execution(int sec)
{
    return 0;
}

int parse(char *buf, char *args[256][256])
{
    char *token;
    char *pipe_ptr, *space_ptr;
    int i, j;

    token = strtok_r(buf, "|\n", &pipe_ptr);

    for (i = 0; token; i++) {
        token = strtok_r(token, " ", &space_ptr);

        for (j = 0; token; j++) {
            args[i][j] = token;
            token = strtok_r(NULL, " ", &space_ptr);
        }

        args[i][j] = NULL;

        token = strtok_r(NULL, "|\n", &pipe_ptr);
    }

    return i;
}

int execution(int n, char *argv[256][256])
{
    int beforePipe = 0;
    int afterPipe[2];

    for (int i = 0; i < n; i++) {
        if (i < n - 1)
            pipe(afterPipe);

        if (!fork()) {
            //dup2(log_fd, 1);
            //close(log_fd);

            if (i > 0) {
                dup2(beforePipe, 0);
                close(beforePipe);
            }

            if (i < n - 1) {
                dup2(afterPipe[1], 1);
                close(afterPipe[0]);
                close(afterPipe[1]);
            }

            if (execvp(argv[i][0], argv[i]) < 0) {
                perror("execvp");
                return -1;
            }
        }

        if (i < n - 1)
            close(afterPipe[1]);

        if (i > 0)
            close(beforePipe);

        beforePipe = afterPipe[0];
    }
 
    return 0;
}

int output(int task)
{
    return 0;
}

int list_tasks(char type)
{
    return 0;
}

int terminate_task(int task)
{
    return 0;
}
