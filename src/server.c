#include "server.h"

int sfifo_fd;
int cfifo_fd;
TASKLIST tasks;

void sigchld_handler(int signum){

    tasks.list[tasks.used - 1].status = concluded;
    
}

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
    int fd;

    signal(SIGINT, sigint_handler);

    signal(SIGCHLD, sigchld_handler);

    tasks.used = 0; //substituir por init xomxing

    printf("Making server fifo...\n");
    if (mkfifo(SERVER, 0666) == -1) {
        unlink(SERVER);
        perror("mkfifo");
        return -1;
    }
    printf("...server fifo done.\n\n");

    if((fd = open("log", O_TRUNC)) == -1){
        perror("log");
        return -1;
    }
    close(fd);

    if((fd = open("log.idx", O_TRUNC)) == -1){
        perror("log.idx");
        return -1;
    }
    close(fd);

    while (1) {
        printf("Waiting for client to open server fifo...\n");
        if ((sfifo_fd = open(SERVER, O_RDONLY)) == -1) {
            perror("open");
            return -1;
        }
        printf("...server fifo opened.\n\n");

        printf("Waiting for client to open client fifo...\n");
        if ((cfifo_fd = open(CLIENT, O_WRONLY)) == -1) {
            perror("open");
            return -1;
        }
        printf("...client fifo opened.\n\n");

        ParsedLine request;

        printf("Waiting for client request...\n\n");
        while (read(sfifo_fd, &request, sizeof(ParsedLine)) > 0) {
            printf("Request received:\n\topt: %c\n\targ: %s\n\n", request.opt, request.arg);
            printf("Processing request...\n");
            process(request);
            printf("...request processed.\n\n");
            printf("Waiting for client request...\n\n");
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

    if (unlink(SERVER) == -1) {
        perror("unlink");
        return -1;
    }

    return -1;
}
