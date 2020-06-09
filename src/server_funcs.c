#include "server_funcs.h"

extern TASKLIST tasks;
extern int cfifo_fd;

int process(ParsedLine request)
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
            return time_execution(input);
        case 'e': // executar tarefa
            tasks.list[tasks.used].task = request.arg;
            tasks.list[tasks.used].status = running;
            tasks.used++;
            n = parse(request.arg, argv);
            return execute(argv, n);
        case 'o': // output
            input = (int)strtol(request.arg, &endptr, 10);
            if (*endptr)
                return -1;
            return output(input);
        case 'l': case 'r': // listar tarefas em execucao
            return list_tasks(request.opt);
        case 't': // terminar tarefa
            input = (int)strtol(request.arg, &endptr, 10);
            if (*endptr)
                return -1;
            return terminate_task(input);
    }

    return -1;
}

int time_inactivity(int sec)
{
    return 0;
}

int time_execution(int sec)
{
    return 0;
}

int parse(char *buf, char *args[256][256])
{
    char *token;
    char *pipe_ptr, *space_ptr;
    int i, j;

    token = strtok_r(buf, "|\n", &pipe_ptr);

    for (i = 0; token; i++) {
        token = strtok_r(token, " ", &space_ptr);

        for (j = 0; token; j++) {
            args[i][j] = token;
            token = strtok_r(NULL, " ", &space_ptr);
        }

        args[i][j] = NULL;

        token = strtok_r(NULL, "|\n", &pipe_ptr);
    }

    return i;
}

int execute(char *argv[256][256], int n)
{
    int beforePipe = 0;
    int afterPipe[2];

    for (int i = 0; i < n; i++) {
        if (i < n - 1)
            pipe(afterPipe);

        if (!fork()) {
            //dup2(log_fd, 1);
            //close(log_fd);

            if (i > 0) {
                dup2(beforePipe, 0);
                close(beforePipe);
            }

            if (i < n - 1) {
                dup2(afterPipe[1], 1);
                close(afterPipe[0]);
                close(afterPipe[1]);
            }

            if (execvp(argv[i][0], argv[i]) < 0) {
                perror("execvp");
                return -1;
            }
        }

        if (i < n - 1)
            close(afterPipe[1]);

        if (i > 0)
            close(beforePipe);

        beforePipe = afterPipe[0];
    }

    return 0;
}

int output(int task)
{
    return 0;
}

int list_tasks(char type)
{
    tasks.list[0].task = "ls -l | wc -l";
    tasks.list[0].status = running;
    tasks.used = 1;

    tasks.list[1].task = "pacman -Q | grep alsa | wc -l";
    tasks.list[1].status = max_execution;
    tasks.used = 2;

    char *status[5] = {
        ": ",
        ", concluida: ",
        ", max inatividade: ",
        ", max execução: ",
        ", terminada: "
    };

    for (int i = 0; i < tasks.used; i++) {
        TASK task = tasks.list[i];
        if (!!(type - 'l') == !!task.status) {
            char buffer[4096];
            sprintf(buffer, "#%d%s%s\n", i + 1, status[task.status], task.task);
            write(1, buffer, strlen(buffer));
        }
    }

    return 0;
}

int terminate_task(int task)
{
    return 0;
}
