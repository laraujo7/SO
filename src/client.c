#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "parsed_line.h"
#include "constants.h"

void help_func();

void crtl_c_handler(int signum)
{
    unlink("client_fifo");
    exit(0);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, crtl_c_handler);

    int sfifo_fd;
    char *sfifo = "server_fifo";
    ssize_t bytes_read;


    if(mkfifo(sfifo, 0666) == -1){
        perror("sfifo file creation");
    } 

    if((sfifo_fd = open(sfifo, O_WRONLY)) == -1) {
        perror("sfifo open");
        return -1;
    }

    ReadlnBuffer *rb = (ReadlnBuffer *)calloc(1, sizeof(ReadlnBuffer));
    initRB(0, rb, MAX_BUFFER);

    ParsedLine *pl = calloc(1, sizeof(ParsedLine));

    if (argc == 1) {
        while ((bytes_read = readlnToPL(rb, pl)) > 0) {
        if ((sfifo_fd = open(sfifo, O_WRONLY)) == -1) return -1;
            write(sfifo_fd, &pl, sizeof(ParsedLine));
            close(sfifo_fd);
        }
    } else {
        pl->arg = argv[2];

        if (validate(argv[1], pl) == -1)
            printf("Invalid comand use \"ajuda\" (option -h) for help\n");
        else
            write(sfifo_fd, pl, sizeof(struct parsed_line));
    }

    return 0;
}

void help_func()
{
    for (int i = 0 ; i < 7 ; i++) {
        write(0, help[i], sizeof(strlen(help[i]) * sizeof(char)));
    }
}
