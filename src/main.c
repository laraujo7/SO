#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "readln.h"
#include "constants.h"

int fd_tmp;

/**
 *
 * ./so
 * ./so -i 20 
 * 
 */


int parse(char* cod, char* snd){

    if(!strcmp(cod,"tempo-inactividade") || !strcmp(cod,"-i")){
        printf("%s\n",snd);
    }

    else if(!strcmp(cod,"tempo-execucao") || !strcmp(cod,"-m")){
        printf("%s\n",snd);
    }

    else if(!strcmp(cod,"executar") || !strcmp(cod,"-e")){
        printf("%s\n",snd);
    }

    else if(!strcmp(cod,"listar") || !strcmp(cod,"-l")){
        printf("%s\n",snd);
    }

    else if(!strcmp(cod,"terminar") || !strcmp(cod,"-t")){
        printf("%s\n",snd);
    }

    else if(!strcmp(cod,"historico") || !strcmp(cod,"-r")){
        printf("%s\n",snd);
    }

    else if(!strcmp(cod,"ajuda") || !strcmp(cod,"-h")){
        printf("%s\n",snd);
    }
    else if(!strcmp(cod,"output") || !strcmp(cod,"-o")){
        printf("%s\n",snd);
    }    
    else {
        perror("error");
        return -1;
    }

    return 0;
}




int main(int argc, char* argv[]){
    char buff[BUFSIZE];
    char *token;
    char *args[2];
    int flag = true;
    char tmp_file[BUFSIZE];
    sprintf(tmp_file, "out/tmp_%d", getpid());
    fd_tmp = open(tmp_file, O_CREAT | O_TRUNC | O_RDWR, 0666); //TRUNK apaga tudo dentro do file

    //o arraylist do nelson era o historico

    Buffer buffer;
    create_buffer(0, &buffer, MAX_BUFFER);

    if(argc == 1) {

        while (readln(&buffer, buff, BUFSIZ) > 0){
            token = strtok(buff, " ");

            if (token == NULL) flag = false;

            for(int i = 0 ; token != NULL && i < 2 ; i++) {
                args[i] = strdup(token);
                token = strtok(NULL, "'");
            }

            if (flag) parse(args[0],args[1]);

            flag = true;
        }
    }

    else parse(argv[1],argv[2]);

    close(fd_tmp);
    unlink(tmp_file);

    return 0;
}
