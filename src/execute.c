#include "execute.h"

extern int cfifo_fd;
extern int time_inact;
extern int time_exec;
extern TASKLIST tasks;

int add_idx()
{
    int idx_fd;
    idx_fd = open("log.idx", O_CREAT | O_WRONLY, 0640);

    int offset;
    offset = lseek(idx_fd, 0, SEEK_END);

    LOGIDX idx;
    idx.offset = offset;
    write(idx_fd, &idx, sizeof(idx));

    close(idx_fd);

    return 0;
}

int idx_set_size(int index, int size)
{
    int idx_fd;
    idx_fd = open("log.idx", O_CREAT | O_WRONLY, 0640);
    lseek(idx_fd, index * sizeof(LOGIDX), SEEK_SET);

    LOGIDX idx;
    idx.size = size;
    write(idx_fd, &idx, sizeof(idx));

    close(idx_fd);

    return 0;
}

void add_task(char *buf)
{
    tasks.list[tasks.used].task = buf;
    tasks.list[tasks.used].status = running;
    tasks.used++;
}

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
                        alarm(sec);
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
        //default:
            //waitpid(-1, NULL, WNOHANG);
    }

    write(cfifo_fd, "feito I guess\n", 14);

    int idx_fd;
    idx_fd = open("log.idx", O_CREAT | O_WRONLY, 0640);

    int offset;
    offset = lseek(idx_fd, 0, SEEK_END);

    close(idx_fd);

    idx_set_size(tasks.used, offset);

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
