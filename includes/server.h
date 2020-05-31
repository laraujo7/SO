#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct args {
    char *pathname[PATH_MAX]; // kinda desnecessário
    char **argv;
} ARGS;

ARGS* parse(char *buf);
