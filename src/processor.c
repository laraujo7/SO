#include "processor.h"

extern TASKLIST tasks;
extern int cfifo_fd;

int process(ParsedLine request)
{
    int input;
    char *endptr;

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
            return process_execute(request.arg);
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

int process_execute(char *task)
{
    int n;
    char *argv[256][256];

    task = strtok(task, "'");
    task_add(task);
    idx_add();
    n = parse(task, argv);
    execute(argv, n);

    return 0;
}
