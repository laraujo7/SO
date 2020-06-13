#include "processor.h"

extern TASKLIST tasks;
extern int cfifo_fd;

int process(ParsedLine request)
{
    switch (request.opt) { // e i l m o t r
        case 'i': // tempo inatividade
            return process_time_inactivity(request.arg);
        case 'm': // tempo execucao
            return process_time_execution(request.arg);
        case 'e': // executar tarefa
            return process_execute(request.arg);
        case 'o': // output
            return process_output(request.arg);
        case 'l': case 'r': // listar tarefas em execucao
            return list_tasks(request.opt);
        case 't': // terminar tarefa
            return process_terminate(request.arg);
        case 'h':
            return help();
    }

    return -1;
}

int process_time_inactivity(char *sec_inact)
{
    char *endptr;
    int sec;

    sec = (int)strtol(sec_inact, &endptr, 10);
    
    if (sec < 1 || *endptr != '\0') {
        char *invalid = "Invalid inactivity time. Type \"ajuda\" (w/o quotes) for help\n";
        write(cfifo_fd, invalid, strlen(invalid));
        return -1;
    }

    time_inactivity(sec);

    return 0;
}

int process_time_execution(char *sec_exec)
{
    char *endptr;
    int sec;

    sec = (int)strtol(sec_exec, &endptr, 10);

    if (sec < 1 || *endptr != '\0') {
        char *invalid = "Invalid execution time. Type \"ajuda\" (w/o quotes) for help\n";
        write(cfifo_fd, invalid, strlen(invalid));
        return -1;
    }

    time_execution(sec);

    return 0;
}

int process_execute(char *task)
{
    int n;
    char *argv[256][256];

    char *executing = "Task received\n";
    write(cfifo_fd, executing, strlen(executing));

    task = strtok(task, "'");
    task_add(task);
    idx_add();
    n = parse(task, argv);
    execute(argv, n);

    return 0;
}

int process_output(char *task_idx)
{
    char *endptr;
    int index;

    index = (int)strtol(task_idx, &endptr, 10) - 1;

    if (index < 0 || *endptr != '\0') {
        char *invalid = "Invalid task index. Type \"ajuda\" (w/o quotes) for help\n";
        write(cfifo_fd, invalid, strlen(invalid));
        return -1;
    }

    return output(index);
}

int process_terminate(char *task_idx)
{
    char *endptr;
    int index;

    index = (int)strtol(task_idx, &endptr, 10) - 1;

    if (index < 0 || *endptr != '\0') {
        char *invalid = "Invalid task index. Type \"ajuda\" (w/o quotes) for help\n";
        write(cfifo_fd, invalid, strlen(invalid));
        return -1;
    }

    //return terminate(index);
    return 0;
}
