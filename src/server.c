#include "../includes/server.h"

int sfifo_fd;
int cfifo_fd;

void ctrl_c_handler(int signum)
{
    unlink("server_fifo");
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, ctrl_c_handler);

    if (mkfifo("server_fifo", 0666) == -1) {
        perror("mkfifo");
        return -1;
    }

    while (1) {
        sfifo_fd = open("server_fifo", O_RDONLY);
        cfifo_fd = open("client_fifo", O_WRONLY);

        //char buf[4096];
        ParsedLine request;// = (ParsedLine *)malloc(sizeof(ParsedLine));
        //pl->opt = malloc(sizeof(char));
        //pl.arg = malloc(4096);

        ssize_t bytes_read = 0;

        while ((bytes_read = read(sfifo_fd, &request, sizeof(ParsedLine))) > 0) {
            printf("opt: %c\narg: %s\n", request.opt, request.arg);
            interpreter(request);

            /*
            char *args[256][256];
            int i = parse(buf, args);
            executar(args, i, log_fd);

            for (int line = 0; line < i; line++) {
                for (int column = 0; args[line][column]; column++) {
                    memset(args[line][column], 0, sizeof(char **));
                }
            }
            */
        }

        close(cfifo_fd);
        close(sfifo_fd);
    }
}

int interpreter(ParsedLine request)
{
    int n;
    int input;
    char *endptr;
    char *argv[256][256];

    switch (request.opt) { // e i l m o t r 
        case 'i': // tempo inatividade
            input = (int)strtol(request.arg, &endptr, 10);
            if (*endptr)
                return -1;
            return time_inactivity(input);
        case 'm': // tempo execucao
            input = (int)strtol(request.arg, &endptr, 10);
            if (*endptr)
                return -1;
            return time_execution(0);
        case 'e': // executar tarefa
            n = parse(request.arg, argv);
            return execution(n, argv);
        case 'o': // output
            input = (int)strtol(request.arg, &endptr, 10);
            if (*endptr)
                return -1;
            return output(0);
        case 'l': case 'r': // listar tarefas em execucao
            return list_tasks(request.opt);
        case 't': // terminar tarefa
            input = (int)strtol(request.arg, &endptr, 10);
            if (*endptr)
                return -1;
            return terminate_task(0);
    }

    return -1;
}
