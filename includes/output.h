#ifndef SAVE_OUTPUT_H
#define SAVE_OUTPUT_H

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

typedef struct output{
    size_t tam;
    int pointer;
}Output;


int save_output(char* output);

int save_index_output(int pt, size_t tam);

int show_output(int index);


#endif