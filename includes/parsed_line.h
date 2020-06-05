#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "constants.h"

typedef struct readln_buffer {
    int fd;
    ssize_t mem_size;
    char *line;
} ReadlnBuffer;

typedef struct parsed_line {
    char opt;           // i - tempo-inactividade ; m - tempo-execucao ; e - executar ; l - listar ; t - terminar ;
    char arg[4096];  // r - historico ; h - ajuda ; o - output ;
} ParsedLine;

ssize_t validate(char *token, ParsedLine *pl);
void initRB(int fd, ReadlnBuffer *rb, size_t init_size);
void resizeRB(ReadlnBuffer *rb);
ssize_t readln(ReadlnBuffer *rb);
ssize_t readlnToPL(ReadlnBuffer *rb, ParsedLine *pl);
