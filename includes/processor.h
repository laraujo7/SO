#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "server_funcs.h"
#include "execute.h"

int process(ParsedLine request);
int process_execute(char *task);

#endif //PROCESSOR_H
