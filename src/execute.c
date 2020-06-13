#include "execute.h"

extern int cfifo_fd;
extern int time_inact;
extern int time_exec;
extern TASKLIST tasks;

int execute(char *argv[256][256], int n)
{
    int log_fd;
    int status;
    int beforePipe = 0;
    int afterPipe[2];

    //switch (fork()) {
    //case -1:
    //    perror("fork");
    //    return -1;
    //case 0:
        if ((log_fd = open("log", O_CREAT | O_APPEND | O_WRONLY, 0666)) == -1) {
            perror("open");
            return -1;
        }

        for (int i = 0; i < n; i++) {
            int offsetA, offsetB;

            if (i == n - 1)
                offsetA = lseek(log_fd, 0, SEEK_END);

            if (i < n - 1)
                pipe(afterPipe);

            switch (tasks.list[tasks.used - 1].pid[i] = fork()) {
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
                if (i == n - 1) {
                    dup2(log_fd, STDOUT_FILENO);
                    close(log_fd);
                }
                if (execvp(argv[i][0], argv[i])) {
                    perror("execvp");
                    return -1;
                }
                break;
            default:
                if (i < n - 1)
                    close(afterPipe[1]);
                if (i > 0)
                    close(beforePipe);
                beforePipe = afterPipe[0];
            }

            if (i == n - 1) {
                wait(&status);
                offsetB = lseek(log_fd, 0, SEEK_END);
                idx_set(tasks.used - 1, offsetA, offsetB - offsetA);
                tasks.list[tasks.used - 1].status = concluded; //SINALLL PORQUE NAO PODE ESTAR DENTRO DO FORK OU NAO AFETA O RESTO
            }

/*
            int interpipe[2];
            pipe(interpipe);

            switch (fork()) {
            case -1:
                perror("fork");
                return -1;
            case 0:
                if (time_inact > 0) {
                    //signal(SIGALRM, sendsig);
                    alarm(time_inact);
                }

                int n;
                char str[4096];

                while ((n = read(afterPipe[0], str, 4096))) {
                    write(interpipe[1], str, n);
                    if (time_inact > 0)
                        alarm(time_inact);
                }

                exit(0);
            }
*/
        }

        close(log_fd);
    //    exit(0);
    //}

    return 0;
}

int idx_add()
{
    int idx_fd;
    LOGIDX idx;

    idx_fd = open("log.idx", O_CREAT | O_APPEND | O_WRONLY, 0640);
    write(idx_fd, &idx, sizeof(LOGIDX));
    close(idx_fd);

    return 0;
}

int idx_set(int index, int offset, int size)
{
    int idx_fd;

    idx_fd = open("log.idx", O_RDWR);
    lseek(idx_fd, index * sizeof(LOGIDX), SEEK_SET);

    LOGIDX idx = {
        .offset = offset,
        .size = size,
    };

    write(idx_fd, &idx, sizeof(idx));

    close(idx_fd);

    return 0;
}

void task_add(char *task, int ncmd)
{
    tasks.list[tasks.used].task = task;
    tasks.list[tasks.used].status = running;
    tasks.list[tasks.used].ncmd = ncmd;
    tasks.used++;
}

int parse(char *buf, char *args[256][256])
{
    char *token;
    char *pipe_ptr, *space_ptr;
    int i, j;

    token = strtok_r(buf, "|", &pipe_ptr);

    for (i = 0; token; i++) {
        token = strtok_r(token, " ", &space_ptr);

        for (j = 0; token; j++) {
            args[i][j] = token;
            token = strtok_r(NULL, " ", &space_ptr);
        }

        args[i][j] = NULL;

        token = strtok_r(NULL, "|", &pipe_ptr);
    }

    return i;
}

/*
int execute(char *argv[256][256], int n)
{
    int beforePipe = 0;
    int afterPipe[2];
    int log_fd;
    int status;
    //pid_t pid;

//    switch (pid = fork()) {
//        case -1:
//            perror("fork");
//            return -1;
//        case 0:
            if ((log_fd = open("log", O_CREAT | O_APPEND | O_WRONLY, 0666)) == -1) {
                perror("open");
                return -1;
            }
            //dup2(log_fd, 1);
            //close(log_fd);

            int idx_fd;
            idx_fd = open("log.idx", O_CREAT | O_WRONLY, 0640);

            int offsetA, offsetB;

            for (int i = 0; i < n; i++) {
                if (i < n - 1)
                    pipe(afterPipe);

                switch (fork()) {
                    case -1:
                        perror("fork");
                        return -1;
                    case 0:
                        dup2(log_fd, 1);
                        close(log_fd);

                        if (i > 0) {
                            dup2(beforePipe, 0);
                            close(beforePipe);
                        }

                        if (i < n - 1) {
                            dup2(afterPipe[1], 1);
                            close(afterPipe[0]);
                            close(afterPipe[1]);
                        }

                        if (execvp(argv[i][0], argv[i])) {
                            perror("execvp");
                            return -1;
                        }

                        break;
                    default:
                        if (i == n -1) {
                            offsetA = lseek(idx_fd, 0, SEEK_END);
                            wait(&status);
                            offsetB = lseek(idx_fd, 0, SEEK_END);
                            idx_set_size(tasks.used, offsetA, offsetB - offsetA);
                            close(idx_fd);
                        }
                }

                if (i < n - 1)
                    close(afterPipe[1]);

                if (i > 0)
                    close(beforePipe);

                beforePipe = afterPipe[0];
            }

//            _exit(0);
//        default:
//            break;
//            waitpid(-1, NULL, WNOHANG);
//    }

    write(cfifo_fd, "feito I guess\n", 14);

    return 0;
}
*/
