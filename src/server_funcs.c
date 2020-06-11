#include "server_funcs.h"

int time_inact;
int time_exec;
extern TASKLIST tasks;
extern int cfifo_fd;

int process(ParsedLine request)
{
    int n;
    int input;
    char *token;
    char *endptr;
    char *argv[256][256];

    switch (request.opt) { // e i l m o t r
        case 'i': // tempo inatividade
            input = (int)strtol(request.arg, &endptr, 10);
            if (*endptr)
                return -1;
            time_inactivity(input);
            return 0;
        case 'm': // tempo execucao
            input = (int)strtol(request.arg, &endptr, 10);
            if (*endptr)
                return -1;
            time_execution(input);
            return 0;
        case 'e': // executar tarefa
            token = strtok(request.arg, "'");
            add_task(token);
            add_idx();
            n = parse(token, argv);
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
        case 'h':
            return help();
    }

    return -1;
}

void time_inactivity(int sec)
{
    time_inact = sec;
}

void time_execution(int sec)
{
    time_exec = sec;
}

int output(int task)
{
    int idx_fd, log_fd;
    LOGIDX idx;
    char *response;

    if ((idx_fd = open("log.idx", O_RDONLY)) == -1) {
        perror("open");
        return -1;
    }
    if (lseek(idx_fd, task * sizeof(LOGIDX), SEEK_SET) == -1) {
        perror("lseek");
        return -1;
    }
    if (read(idx_fd, &idx, sizeof(LOGIDX)) == -1) {
        perror("read");
        return -1;
    }

    if ((log_fd = open("log", O_RDONLY)) == -1) {
        perror("open");
        return -1;
    }
    if (lseek(log_fd, idx.offset, SEEK_SET) == -1) {
        perror("lseek");
        return -1;
    }
    if (read(log_fd, &response, idx.size) == -1) {
        perror("read");
        return -1;
    }

    if (write(cfifo_fd, response, strlen(response)) == -1) {
        perror("write");
        return -1;
    }

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

    tasks.list[2].task = "pacman -Q | grep alsa";
    tasks.list[2].status = running;
    tasks.used = 3;

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
            char response[4096];
            sprintf(response, "#%d%s%s\n", i + 1, status[task.status], task.task);
            if (write(cfifo_fd, response, strlen(response)) == -1) {
                perror("write");
                return -1;
            }
        }
    }

    return 0;
}

int terminate_task(int task)
{
    if (kill(tasks.list[task].pid, SIGKILL) == -1) {
        perror("kill");
        return -1;
    }

    tasks.list[task].status = terminated;

    char *response = "Task %d terminated successfully\n";
    write(cfifo_fd, response, strlen(response));

    return 0;
}


int help()
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

    for (int i = 0; i < 7; i++) {
        if (write(cfifo_fd, help[i], strlen(help[i])) == -1) {
            perror("write");
            return -1;
        }
    }

    return 0;
}
