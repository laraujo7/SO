#ifndef PARSED_LINE_H
#define PARSED_LINE_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdio.h>

#include "constants.h"

typedef struct readln_buffer {
    int fd;
    ssize_t mem_size;
    char *line;
} ReadlnBuffer;

typedef struct parsed_line {
    char opt;
    char arg[4096];
} ParsedLine;

ssize_t validate(char *token, ParsedLine *pl);
void initRB(int fd, ReadlnBuffer *rb, size_t init_size);
void resizeRB(ReadlnBuffer *rb);
ssize_t readln(ReadlnBuffer *rb);
ssize_t readlnToPL(ReadlnBuffer *rb, ParsedLine *pl);

#endif //PARSED_LINE_H
