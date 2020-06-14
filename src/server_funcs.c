#include "server_funcs.h"

int time_inact;
int time_exec;
extern TASKLIST tasks;
extern int cfifo_fd;

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
    if (task + 1 > tasks.used) {
        char invalid[34];
        sprintf(invalid, "There's only %d task(s)\n", tasks.used);
        write(cfifo_fd, invalid, strlen(invalid));
        return -1;
    }

    int idx_fd, log_fd;
    LOGIDX idx;

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

    char response[idx.size];

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

    if (write(cfifo_fd, response, idx.size) == -1) {
        perror("write");
        return -1;
    }

    return 0;
}

int list_tasks(char type)
{
    char *status[5] = {
        ": ",
        ", concluida: ",
        ", max inatividade: ",
        ", max execução: ",
        ", terminada: "
    };

    char response[4096] = "";
    char line[4096];

    for (int i = 0; i < tasks.used; i++) {
        TASK task = tasks.list[i];
        if (!!(type - 'l') == !!task.status) {
            sprintf(line, "#%d%s%s\n", i + 1, status[task.status], task.task);
            strcat(response, line);
            line[0] = '\0';
        }
    }

    if (*response == '\0')
        sprintf(response, "No tasks %s\n", type - 'l' ? "done" : "running");

    if (write(cfifo_fd, response, strlen(response)) == -1) {
        perror("write");
        return -1;
    }

    return 0;
}

int terminate(int task_idx)
{
    TASK task = tasks.list[task_idx];

    for (int i = 0; i < task.ncmd; i++) {
        if (kill(task.pid[i], SIGKILL) == -1) {
            perror("kill");
            return -1;
        }
    }

    tasks.list[task_idx].status = terminated;

    char response[29];
    sprintf(response, "Task %d terminated\n", task_idx + 1);
    write(cfifo_fd, response, strlen(response));

    return 0;
}

int help()
{
    char *help = "tempo-inactividade segs\ntempo-execucao segs\nexecutar p1 | p2 ... | pn\nlistar\nterminar n\nhistorico\noutput n\najuda\nsegs > 0 && n > 0\n";

    if (write(cfifo_fd, help, strlen(help)) == -1) {
        perror("write");
        return -1;
    }

    return 0;
}
