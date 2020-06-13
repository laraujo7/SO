#include "server.h"

int sfifo_fd;
int cfifo_fd;
TASKLIST tasks;

void sigint_handler(int signum)
{
    printf("\n\"Unlinking\" server fifo...\n");
    if (unlink(SERVER) == -1) {
        perror("unlink");
        exit(-1);
    }
    printf("...server fifo \"unlinked\".\n\n");

    printf("Server shutting down.\n");

    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);

    tasks.used = 0; //substituir por init xomxing

    if (mkfifo(SERVER, 0666) == -1) {
        perror("mkfifo");
        return -1;
    }

    while (1) {
        if ((sfifo_fd = open(SERVER, O_RDONLY)) == -1) {
            perror("open");
            return -1;
        }

        if ((cfifo_fd = open(CLIENT, O_WRONLY)) == -1) {
            perror("open");
            return -1;
        }

        ParsedLine request;

        while (read(sfifo_fd, &request, sizeof(ParsedLine)) > 0) {
            process(request);
            }

        if (close(cfifo_fd) == -1) {
            perror("close");
            return -1;
        }
        if (close(sfifo_fd) == -1) {
            perror("close");
            return -1;
        }

    }

    if (unlink(SERVER) == -1) {
        perror("unlink");
        return -1;
    }

    return -1;
}
