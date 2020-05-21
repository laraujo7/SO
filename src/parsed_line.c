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


/*
ssize_t readlnToPL(ParsedLine *pl){
    char *token;
    int flag = true;
    char* args[2];
    ssize_t r = readln(pl);



    token = strtok(pl->line, " ");

            if (token == NULL) flag = false;

            for(int i = 0 ; token != NULL && i < 2 ; i++) {
                args[i] = strdup(token);
                token = strtok(NULL, " ");
            }

            if (flag) printf("%s\n",pl->line); //parse(args[0],args[1]);

            flag = true;

    return r;
}
*/

ssize_t readln(ParsedLine *pl){
    int read_bytes = 0, found = false, size=0;
    char buffer[BUFFSIZE];

    while(!found && (read_bytes = read(pl->fd, buffer, BUFFSIZE)) > 0){ 
        
        if(size > pl->mem_size)
            resizePL(pl);

        for(int i=0 ; i < read_bytes && !found ; size++, i++){
            
            if(buffer[i] == '\n'){
                buffer[i] = '\0';
                found = true;
            }

            pl->line[size] = buffer[i];

        }
    }

    return size;
}

/*
int main(){
    ParsedLine *p = (ParsedLine *) calloc(1,sizeof(ParsedLine));
    initPL(0,p,20);
    readln(p);
    printf("%s\n",p->line);
}

*/