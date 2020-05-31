/*
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv){
    int fd;
    char* sfifo = "server_fifo";
    void* buffer;
    size_t bytes_read = 0;

    if((fd=open(sfifo,O_RDONLY)) == -1){
        perror("open");
    }
    buffer = malloc(sizeof(char) * 64);
    while( (bytes_read = read(fd,buffer,1)) );

    printf("%s",(char*)buffer);
  
    return 0;
}
*/