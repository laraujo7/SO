#include <stdio.h>
#include "parsed_line.h"


void initPL(int fd, ParsedLine *pl, size_t init_size){
    pl->fd = fd;
    pl->mem_size = init_size;
    pl->line = malloc(sizeof(char)*init_size);
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
    int size = 0, read_bytes = 0, found = 0;
    char buffer[BUFFSIZE];

    while(!found && (read_bytes = read(pl->fd, buffer, BUFFSIZE))){ 
        if(size > pl->mem_size)
            resizePL(pl);

        for(int i=0; i < read_bytes && !found; size++, i++){
            if(buffer[i] == '\n'){
                buffer[i] = '\0';
                found=1;
            }
            pl->line[size] = buffer[i];
        }
    }
    return read_bytes;
}


int main(){
    ParsedLine *p = (ParsedLine *) calloc(1,sizeof(ParsedLine));
    initPL(0,p,20);
    readln(p);
    printf("%s\n",p->line);
}

