#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "../includes/parsed_line.h"
#include "../includes/constants.h"

void help_func();

void crtl_c_handler(int signum)
{
    unlink("client_fifo");
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, crtl_c_handler);

    int cfifo_fd, sfifo_fd;
    ssize_t bytes_read;

    if (mkfifo("client_fifo", 0666) == -1) {
        perror("mkfifo");
        return(-1);
    } 

    if ((sfifo_fd = open("server_fifo", O_WRONLY)) == -1) {
        perror("open");
        return -1;
    }

    if ((cfifo_fd = open("client_fifo", O_RDONLY)) == -1) {
        perror("open");
        return -1;
    }

    ReadlnBuffer *rb = (ReadlnBuffer *)calloc(1, sizeof(ReadlnBuffer));
    initRB(0, rb, MAX_BUFFER);

    //ParsedLine *pl = calloc(1, sizeof(ParsedLine));
    ParsedLine pl;

    if (argc == 1) {
        while ((bytes_read = readlnToPL(rb, &pl)) > 0) {
            write(sfifo_fd, &pl, sizeof(ParsedLine));
        }
    } else {
        pl.opt = argv[1][1];
        //pl->arg = argv[2];
        strcpy(pl.arg, argv[2]);
        printf("argv: %s\n", argv[2]);
        printf("pl1: %s\n", pl.arg);

        //if (validate(argv[1], &pl) == -1)
        //    printf("Invalid comand use \"ajuda\" (option -h) for help\n");
        //else {
            printf("pl2: %s\n", pl.arg);
            write(sfifo_fd, &pl, sizeof(ParsedLine));
        //}
    }

    if (close(cfifo_fd) == -1) {
        perror("close");
        return -1;
    }
    if (unlink("client_fifo") == -1) {
        perror("unlink");
        return -1;
    }

    return 0;
}

void help_func()
{
    for (int i = 0; i < 7; i++) {
        write(0, help[i], sizeof(strlen(help[i]) * sizeof(char)));
    }
}
