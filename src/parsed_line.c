#include <stdio.h>
#include "parsed_line.h"


void initPL(int fd, ParsedLine *pl, size_t init_size){
    pl->fd = fd;
    pl->mem_size = init_size;
    pl->line = calloc(pl->mem_size,sizeof(char));
}

void resizePL(ParsedLine *pl, size_t init_size){       
    pl->mem_size += init_size;          
    pl->line = realloc(pl->line, pl->mem_size);
}

ssize_t readlnToPL(ParsedLine *pl){
    ssize_t r = readln(pl);

//parsing aqui;

    return r;
}

ssize_t readln(ParsedLine *pl){
    int read_bytes = 0, found = false , size = 0;
    char buffer[BUFFSIZE];

    while(!found && (read_bytes = read(pl->fd, buffer, BUFFSIZE)) > 0){

        for(int i = 0; i < read_bytes && !found ; i++){
        
            if(buffer[i] == '\n'){
                buffer[i] = '\0';
                found = true;
            }

        if(read_bytes + i > pl->mem_size)
            resizePL(pl,read_bytes);

        pl->line[i] = buffer[i];
        }
    }

    return read_bytes;
}



