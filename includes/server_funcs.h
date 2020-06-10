#ifndef SERVER_FUNCS_H
#define SERVER_FUNCS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "parsed_line.h"
#include "execute.h"
#include "server.h"

typedef struct logidx {
    int size;
    int offset;
} LOGIDX;

int process(ParsedLine request);
void time_inactivity(int sec);
void time_execution(int sec);
int output(int task);
int list_tasks(char type);
int terminate_task(int task);
int help();

#endif //SERVER_FUNCS_H