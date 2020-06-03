#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "parsed_line.h"
#include "constants.h"
#include "output.h"



int main(int argc, char* argv[])
{
    int sfifo_fd;
    char *sfifo = "server_fifo";
    ssize_t bytes_read;

    ReadlnBuffer *rb = (ReadlnBuffer *)calloc(1, sizeof(ReadlnBuffer));
    initRB(0, rb, MAX_BUFFER);
    ParsedLine *pl = calloc(1, sizeof(ParsedLine));

    if(argc == 1) {
        while(bytes_read = readlnToPL(rb,pl)){

            if((sfifo_fd = open(sfifo,O_WRONLY)) == -1) return -1;

            write(sfifo_fd,&pl,sizeof(ParsedLine));
            close(sfifo_fd);
            if(pl->opt == 'o') show_output(atoi(pl->arg) - 1);
                else save_output(pl->arg);
        }
    } 

    else {
 
        pl->arg = argv[2];
        validate(argv[1],pl);

        
        if(pl->opt == 'o') show_output(atoi(pl->arg) - 1);
            else save_output(pl->arg);
        
    }

    return 0;
}
