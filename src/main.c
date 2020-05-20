#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "readln.h"

#define BUFSIZE 4096
#define MAX_BUFFER 4096
#define false 0
#define true !false


int fd_tmp;


int main(int argc, char* argv[]){
    char buff[BUFSIZE];
    char *token;
    char *args[2];
    int flag = true;
    char tmp_file[BUFSIZE];
    sprintf(tmp_file, "out/tmp_%d", getpid());
    fd_tmp = open(tmp_file, O_CREAT | O_TRUNC | O_RDWR, 0666); //TRUNK apaga tudo dentro do file

    Buffer buffer;
    create_buffer(0, &buffer, MAX_BUFFER);

    while (readln(&buffer, buff, BUFSIZ) > 0){
        token = strtok(buff, " ");
        if (token == NULL) flag = false;
        for(int i = 0 ; token != NULL && i < 2 ; i++) {
            args[i] = strdup(token);
            token = strtok(NULL, " ");
        }
        if (!flag) printf("Sem nada\n");
            else printf("%s %s\n",args[0],args[1]);

        flag = 1;
    }

    close(fd_tmp);
    unlink(tmp_file);

    return 0;
}