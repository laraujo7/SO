#include "parsed_line.h"


void initPL(int fd, ParsedLine *pl, size_t init_size){
    pl->fd = fd;
    pl->mem_size = init_size;
}

void resizePL(ParsedLine *pl){       
    pl->mem_size += 2 * pl->mem_size;          
    pl->line = realloc(pl->line, pl->mem_size);
}

ssize_t readlnToPL(ParsedLine *pl){
    ssize_t r = readln(pl);

//parsing aqui;

    return r;
}

ssize_t readln(ParsedLine *pl){
    int n_bytes = 0, read_bytes = 0, found = 0;
    char buffer[BUFFSIZE];

    read_bytes = read(pl->fd, buffer, BUFFSIZE);

    if(read_bytes<pl->mem_size)
        resizePL(pl);
        

    for(int i = 0; i < read_bytes && !found; i++){
        if(buffer[i] == '\n'){
            buffer[i] = '\0';
            found=1;
        }
        pl->line[i] = buffer[i];
    }
    return read_bytes;
}
