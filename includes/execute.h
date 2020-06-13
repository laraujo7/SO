#ifndef EXECUTE_H
#define EXECUTE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "server_funcs.h"
#include "server.h"

int idx_add();
int idx_set(int index, int offset, int size);
void task_add(char *buff, int ncmd);
int parse(char *buff, char *args[256][256]);
int execute(char *argv[256][256], int n);

#endif //EXECUTE_H
