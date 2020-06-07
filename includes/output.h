#ifndef OUTPUT_H
#define OUTPUT_H

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"
#include "parsed_line.h"

typedef struct output {
    size_t tam;
    int pointer;
} Output;

int save_output(char *output);
int save_index_output(int pt, size_t tam);
int show_output(int index);

#endif //OUTPUT_H
