#include "server.h"

int sfifo_fd;
int cfifo_fd;

void ctrl_c_handler(int signum)
{
    if (unlink("server_fifo") == -1) {
        perror("unlink");
        exit(-1);
    }

    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, ctrl_c_handler);

    if (mkfifo("server_fifo", 0666) == -1) {
        perror("mkfifo");
        return -1;
    }

    while (1) {
        if ((sfifo_fd = open("server_fifo", O_RDONLY)) == -1) {
            perror("open");
            return -1;
        }

        if ((cfifo_fd = open("client_fifo", O_WRONLY)) == -1) {
            perror("open");
            return -1;
        }

        ParsedLine request;

        while (read(sfifo_fd, &request, sizeof(ParsedLine)) > 0) {
            printf("Received request:\n\topt: %c\n\targ: %s\n", request.opt, request.arg);
            interpret(request);
        }

        if (close(cfifo_fd) == -1) {
            printf("HEYOU0\n");
            perror("close");
            return -1;
        }

        if (close(sfifo_fd) == -1) {
            printf("HEYOU0\n");
            perror("close");
            return -1;
        }
    }

    if (unlink("server_fifo") == -1) {
        perror("unlink");
        return -1;
    }

    return -1;
}
