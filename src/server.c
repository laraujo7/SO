#include "server.h"

int sfifo_fd;
int cfifo_fd;
TASKLIST tasks;

void sigchld_handler(int signum)
{
    char index[16];

    int signal_pipe_fd = open("signal_pipe", O_RDONLY, 0640);
    read(signal_pipe_fd, index, 16);

    printf("I WAS HERE MFS\n");

    int task_idx = 0;
    task_idx = atoi(index);
    printf("I WAS HERE MFS %d\n", task_idx);

    tasks.list[task_idx - 1].status = concluded;
}

void sigint_handler(int signum)
{
    if (unlink(SERVER) == -1) {
        exit(-1);
    }

    int fd;

    if ((fd = open("log", O_TRUNC)) == -1) {
        exit(-1);
    }
    close(fd);

    if ((fd = open("log.idx", O_TRUNC)) == -1) {
        exit(-1);
    }
    close(fd);

    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);

    signal(SIGCHLD, sigchld_handler);

    tasks.used = 0; //substituir por init xomxing

    if (mkfifo(SERVER, 0666) == -1) {
        return -1;
    }

    while (1) {
        if ((sfifo_fd = open(SERVER, O_RDONLY)) == -1) {
            return -1;
        }

        if ((cfifo_fd = open(CLIENT, O_WRONLY)) == -1) {
            return -1;
        }

        ParsedLine request;

        while (read(sfifo_fd, &request, sizeof(ParsedLine)) > 0) {
            process(request);
        }

        if (close(cfifo_fd) == -1) {
            return -1;
        }

        if (close(sfifo_fd) == -1) {
            return -1;
        }
    }

    if (unlink(SERVER) == -1) {
        return -1;
    }

    return -1;
}
