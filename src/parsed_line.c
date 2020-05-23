#include <stdio.h>
#include "parsed_line.h"

ssize_t validate(ParsedLine* pl){

    if(!strcmp(pl->args[0],"tempo-inactividade") || !strcmp(pl->args[0],"-i")) pl->opt = 'i';
            else if(!strcmp(pl->args[0],"tempo-execucao") || !strcmp(pl->args[0],"-m")) pl->opt = 'm';
            else if(!strcmp(pl->args[0],"executar") || !strcmp(pl->args[0],"-e")) pl->opt = 'e';
            else if(!strcmp(pl->args[0],"listar") || !strcmp(pl->args[0],"-l")) pl->opt = 'l';
            else if(!strcmp(pl->args[0],"terminar") || !strcmp(pl->args[0],"-t")) pl->opt = 't';
            else if(!strcmp(pl->args[0],"historico") || !strcmp(pl->args[0],"-r")) pl->opt = 'r';
            else if(!strcmp(pl->args[0],"ajuda") || !strcmp(pl->args[0],"-h")) pl->opt = 'h';
            else if(!strcmp(pl->args[0],"output") || !strcmp(pl->args[0],"-o")) pl->opt = 'o';
            else {perror("error"); return -1;}

    return 0;
}


void initRB(int fd, ReadlnBuffer *pl, size_t init_size){
    pl->fd = fd;
    pl->mem_size = init_size;
    pl->line = malloc(sizeof(char)*init_size);
}

void resizeRB(ReadlnBuffer *rb){       
    rb->mem_size += 2 * rb->mem_size;          
    rb->line = realloc(rb->line, rb->mem_size);
}


ssize_t readlnToPL(ReadlnBuffer* rb, ParsedLine* pl){
    ssize_t bytes_read;
    char *token;
    int flag = true;

    if(bytes_read = readln(rb)) {

        token = strtok(rb->line, " ");

        if (token == NULL) flag = false;

        for(int i = 0 ; token != NULL && i < 2 ; i++) {
            pl->args[i] = strdup(token);
            token = strtok(NULL, "'");
        }

        if (flag) {
            if(validate(pl) == -1)
                printf("Invalid comand use \"ajuda\" (option -h) for help\n");
        }
    }

    return bytes_read;
}



ssize_t readln(ReadlnBuffer* rb){
    int read_bytes = 0, found = false, size=0;
    char buffer[BUFFSIZE];

    while(!found && (read_bytes = read(rb->fd, buffer, BUFFSIZE)) > 0){ 
        
        if(size > rb->mem_size)
            resizeRB(rb);

        for(int i=0 ; i < read_bytes && !found ; size++, i++){
            
            if(buffer[i] == '\n'){
                buffer[i] = '\0';
                found = true;
            }

            rb->line[size] = buffer[i];

        }
    }

    return size;
}
