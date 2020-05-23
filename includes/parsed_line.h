#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"

typedef struct readln_buffer {
    int fd;
    ssize_t mem_size;
    char *line;
} ReadlnBuffer;


typedef struct pased_line {
    char opt;       // i - tempo-inactividade ; m - tempo-execucao ; e - executar ; l - listar ; t - terminar ;
    char* args[2];   // r - historico ; h - ajuda ; o - output ;
} ParsedLine;


void initPL(int fd, ReadlnBuffer *rb, size_t init_size);
void resizeRB(ReadlnBuffer *rb);
ssize_t readln(ReadlnBuffer *rb);
ssize_t readlnToPL(ReadlnBuffer *rb, ParsedLine* pl);

