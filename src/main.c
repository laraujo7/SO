#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "parsed_line.h"
#include "constants.h"

int fd_tmp;


int main(int argc, char* argv[]){
    int sfifo_fd;
    char *sfifo = "server_fifo";
    ssize_t bytes_read;
    char tmp_file[BUFFSIZE];
    sprintf(tmp_file, "out/tmp_%d", getpid());
    fd_tmp = open(tmp_file, O_CREAT | O_TRUNC | O_RDWR, 0666); //TRUNK apaga tudo dentro do file

    //o arraylist do nelson era o historico

    if(mkfifo(sfifo, 0666) == -1){
        perror("sfifo file creation");
    } 

    if((sfifo_fd = open(sfifo, O_WRONLY)) == -1) {
        perror("sfifo open");
        return -1;
    }

    ReadlnBuffer *rb = (ReadlnBuffer *) calloc(1,sizeof(ReadlnBuffer));
    initRB(0,rb,MAX_BUFFER);

    ParsedLine *pl = calloc(1,sizeof(ParsedLine));

    if(argc == 1) {
            while((bytes_read = readlnToPL(rb,pl)) > 0){
                printf("%c %s\n",pl->opt,pl->arg);
        }
    }
    else {
        pl->arg = argv[2];

        if(validate(argv[1],pl) == -1)
                printf("Invalid comand use \"ajuda\" (option -h) for help\n");
        else
            write(sfifo, pl, sizeof(struct parsed_line));
    }

    close(fd_tmp);
    unlink(tmp_file);

    return 0;
}
