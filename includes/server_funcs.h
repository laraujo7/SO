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

int interpret(ParsedLine request);
int parse(char *buf, char *args[256][256]);
int time_inactivity(int sec);
int time_execution(int sec);
int execute(char *argv[256][256], int n);
int output(int task);
int list_tasks(char type);
int terminate_task(int task);

#endif //SERVER_FUNCS_H
