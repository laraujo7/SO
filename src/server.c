  
#include "server.h"

int sfifo_fd;
int cfifo_fd;
int task_terminated;
TASKLIST tasks;


void sigchld_handler(int signum){

    //saber quem é q esta a acabar
    printf("tarefa concluida -> %d\n",tasks.used -1);
    tasks.list[tasks.used - 1].status = concluded;
    
}

void sigint_handler(int signum)
{
    if (unlink(SERVER) == -1) {
        perror("unlink");
        exit(-1);
    }

    exit(0);
}

int main(int argc, char *argv[])
{
    int fd;

    signal(SIGINT, sigint_handler);

    signal(SIGCHLD, sigchld_handler);

    tasks.used = 0; //substituir por init xomxing

    if (mkfifo(SERVER, 0666) == -1) {
        unlink(SERVER);
        perror("mkfifo");
        return -1;
    }

    if((fd = open("log", O_CREAT | O_TRUNC, 0666)) == -1){
        perror("log");
        return -1;
    }
    close(fd);

    if((fd = open("log.idx", O_CREAT, O_TRUNC, 0666)) == -1){
        perror("log.idx");
        return -1;
    }
    close(fd);


    while (1) {
        
        if ((sfifo_fd = open(SERVER, O_RDONLY)) == -1) {
            perror("open");
            return -1;
        }

        if ((cfifo_fd = open(CLIENT, O_WRONLY)) == -1) {
            perror("open");
            return -1;
        }

        ParsedLine request;

        while (read(sfifo_fd, &request, sizeof(ParsedLine)) > 0) {
            process(request);
        }

        if (close(cfifo_fd) == -1) {
            perror("close");
            return -1;
        }
        
        if (close(sfifo_fd) == -1) {
            perror("close");
            return -1;
        }
    }

    if (unlink(SERVER) == -1) {
        perror("unlink");
        return -1;
    }

    return -1;
}