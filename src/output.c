#include "output.h"

int save_index_output(int pt, size_t tam){
    int fd;

    if((fd = open("out/log.idx", O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1){
        perror("error");
        return -1;
    }

    Output output = {
        .tam = tam,
        .pointer = pt,
    };

    write(fd,&output,sizeof(Output));

    close(fd);

    return 0;
}


int show_output(int index){
    int fd_idx,fd;

    if((fd_idx = open("out/log.idx",O_RDONLY)) == -1){
        perror("error");
        return -1;
    }

    if(lseek(fd_idx, index * sizeof(Output), SEEK_SET) == -1){
        perror("error");
        return -1;
    }

    Output aux;

    if(read(fd_idx,&aux,sizeof(Output)) <= 0){
        perror("error");
        return -1;
    }

    if((fd = open("out/log.bin",O_RDONLY)) == -1){
        perror("error");
        return -1;
    }

    if(lseek(fd,aux.pointer,SEEK_SET) == -1){
        perror("error");
        return -1;
    }

    char* output = calloc(aux.tam + 1, sizeof(char));

    if(read(fd,output,aux.tam) > 0){
        printf("%s\n",output);
        close(fd);
    }

    close(fd_idx);

    return 0;
}
