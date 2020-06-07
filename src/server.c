#include "server.h"

int sfifo_fd;
int cfifo_fd;

void ctrl_c_handler(int signum)
{
    printf("\n\"Unlinking\" server fifo...\n");
    if (unlink("server_fifo") == -1) {
        perror("unlink");
        exit(-1);
    }
    printf("...server fifo \"unlinked\".\n\n");

    printf("Server shutting down.\n");

    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, ctrl_c_handler);

    printf("Making server fifo...\n");
    if (mkfifo("server_fifo", 0666) == -1) {
        perror("mkfifo");
        return -1;
    }
    printf("...server fifo done.\n\n");

    while (1) {
        printf("Waiting for client to open server fifo...\n");
        if ((sfifo_fd = open("server_fifo", O_RDONLY)) == -1) {
            perror("open");
            return -1;
        }
        printf("...server fifo opened.\n\n");

        printf("Waiting for client to open client fifo...\n");
        if ((cfifo_fd = open("client_fifo", O_WRONLY)) == -1) {
            perror("open");
            return -1;
        }
        printf("...client fifo opened.\n\n");

        ParsedLine request;

        printf("Waiting for client input...\n\n");
        while (read(sfifo_fd, &request, sizeof(ParsedLine)) > 0) {
            printf("Request received:\n\topt: %c\n\targ: %s\n\n", request.opt, request.arg);
            interpret(request);
            printf("Request processed.\n");
            printf("Waiting for client input...\n\n");
        }
        printf("Client exited.\n\n");

        printf("Closing client fifo...\n");
        if (close(cfifo_fd) == -1) {
            perror("close");
            return -1;
        }
        printf("...client fifo closed.\n\n");

        printf("Closing server fifo...\n");
        if (close(sfifo_fd) == -1) {
            perror("close");
            return -1;
        }
        printf("...server fifo closed.\n\n");
    }

    if (unlink("server_fifo") == -1) {
        perror("unlink");
        return -1;
    }

    return -1;
}
