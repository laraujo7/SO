#include "client.h"

int sfifo_fd;
int cfifo_fd;

void crtl_c_handler(int signum)
{
    if (unlink("client_fifo") == -1) {
        perror("unlink");
        exit(-1);
    }

    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, crtl_c_handler);

    if (mkfifo("client_fifo", 0666) == -1) {
        perror("mkfifo");
        return(-1);
    } 

    if ((sfifo_fd = open("server_fifo", O_WRONLY)) == -1) {
        perror("open");
        return -1;
    }

    if ((cfifo_fd = open("client_fifo", O_RDONLY)) == -1) {
        perror("open");
        return -1;
    }

    ReadlnBuffer *rb = (ReadlnBuffer *)calloc(1, sizeof(ReadlnBuffer));
    initRB(0, rb, MAX_BUFFER);

    //ParsedLine *pl = calloc(1, sizeof(ParsedLine));
    ParsedLine pl;

    if (argc == 1) {
        while (readlnToPL(rb, &pl) > 0) {
            write(sfifo_fd, &pl, sizeof(ParsedLine));
        }
    } else {
        pl.opt = argv[1][1];
        //pl->arg = argv[2];
        strcpy(pl.arg, argv[2]);

        if (validate(argv[1], &pl) == -1) {
            printf("Invalid comand use \"ajuda\" (option -h) for help\n");
        } else {
            printf("pl2: %s\n", pl.arg);
            write(sfifo_fd, &pl, sizeof(ParsedLine));
        }
    }

    if (close(cfifo_fd) == -1) {
        perror("close");
        return -1;
    }

    if (close(sfifo_fd) == -1) {
        perror("close");
        return -1;
    }

    if (unlink("client_fifo") == -1) {
        perror("unlink");
        return -1;
    }

    return 0;
}

void help_func()
{
    char *help[7] = {
        "tempo-inactividade segs\n",
        "tempo-execucao segs\n",
        "executar p1 | p2 ... | pn\n",
        "terminar segs\n",
        "historico\n",
        "ajudar\n",
        "output segs\n",
    };

    for (int i = 0; i < 7; i++)
        write(0, help[i], sizeof(strlen(help[i]) * sizeof(char)));
}
