#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "processor.h"
#include "parsed_line.h"
#include "execute.h"

typedef enum {running, concluded, max_inactivity, max_execution, terminated} STATUS;

typedef struct task {
    int ncmd;
    pid_t pid[64];
    STATUS status;
    char *task;
} TASK;

typedef struct tasklist {
    int used;
    TASK list[4096];
} TASKLIST;

#endif //SERVER_H
