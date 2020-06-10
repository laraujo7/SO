#include "execute.h"

extern TASKLIST tasks;

int execute(char *argv[256][256], int n)
{
    int beforePipe = 0;
    int afterPipe[2];
    int log_fd;
    pid_t pid;

    switch (pid = fork()) {
        case -1:
            perror("fork");
            return -1;
        case 0:
            if ((log_fd = open("log", O_CREAT | O_APPEND | O_WRONLY, 0666)) == -1) {
                perror("open");
                return -1;
            }
            dup2(log_fd, 1);
            close(log_fd);

            for (int i = 0; i < n; i++) {
                if (i < n - 1)
                    pipe(afterPipe);

                switch (fork()) {
                    case -1:
                        perror("fork");
                        return -1;
                    case 0:
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

                        break;
                        /*
                    default:
                        int status;
                        wait(&status);
                        */

                        //alarm_inac(10);
                }

                if (i < n - 1)
                    close(afterPipe[1]);

                if (i > 0)
                    close(beforePipe);

                beforePipe = afterPipe[0];
            }

            _exit(0);
        default:
            break;
            //waitpid(-1, NULL, WNOHANG);
    }

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
