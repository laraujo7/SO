#include "server.h"

int main(int argc, char *argv[])
{
    if ((mkfifo("fifo", 0666)) < 0) {
        perror("mkfifo");
        return -1;
    }

    int log;

    if ((log = open("log", O_CREAT | TRUNC | O_WRONLY, 0644)) < 0) {
        perror("open");
        return -1;
    }

    int fifo_fd;

    while ((fifo_fd = open("fifo", O_RDONLY)) > 0) {
        char buf[BUFSIZ];
        int bytes_read;

        while ((bytes_read = read(fifo_fd, buf, BUFSIZ)) > 0) {
            if (bytes_read < 0) {
                perror("read");
                return -1;
            }

            ARGS *argvp = parse(buf);

            switch (fork()) {
                case -1:
                    perror("fork");
                    return -1;
                case 0:
                    //executar(argvp);
                    exit(0);
                    break;
                default:
                    wait(NULL);
            }
        }

        if (close(fifo_fd) < 0) {
            perror("close");
            return -1;
        }
    }

    if (fifo_fd < 0) {
        perror("open");
        return -1;
    }

    if (close(log) < 0) {
        perror("close");
        return -1;
    }

    return 0;
}

ARGS* parse(char *buf)
{
    char *token;
    char *pipe_ptr, *space_ptr;
    ARGS *args;

    token = strtok_r(buf, "|", &pipe_ptr);

    for (int i = 0; token; i++) {
        args = (ARGS *)realloc(args, (i + 1) * sizeof(ARGS));
        token = strtok_r(token, " ", &space_ptr);

        for (int j = 0; token; j++) {
            args[i].argv = (char **)realloc(args[i].argv, (j + 1) * sizeof(char **));
            args[i].argv[j] = token;

            token = strtok_r(NULL, " ", &space_ptr);
        }

        token = strtok_r(NULL, "|", &pipe_ptr);
    }

    return args;
}
