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
    int n_bytes = 0, read_bytes = 0, found = 0, i=0;
    char buffer[BUFFSIZE];

    while((read_bytes = read(pl->fd, buffer, BUFFSIZE)) && !found){ 
        if(read_bytes > pl->mem_size)
            resizePL(pl);

        for(int j=0; i < read_bytes && !found; i++, j++){
            if(buffer[j] == '\n'){
                buffer[j] = '\0';
                found=1;
            }
            pl->line[i] = buffer[j];
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

