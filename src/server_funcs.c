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
            if (write(1, buffer, strlen(buffer)) < 0) {
                perror("write");
                return -1;
            }
        }
    }

    return 0;
}

int terminate_task(int task)
{
    if (kill(tasks.list[task].pid, SIGKILL) < 0) {
        perror("kill");
        return -1;
    }

    tasks.list[task].status = terminated;

    return 0;
}
