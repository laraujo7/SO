#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "constants.h"

typedef struct parsed_line {
    int fd;
    ssize_t mem_size;
    char *line;
} ParsedLine;


typedef struct coisas {
    char opt;       // i - tempo-inactividade ; m - tempo-execucao ; e - executar ; l - listar ; t - terminar ;
    char args[2];   // r - historico ; h - ajuda ; o - output ;
} Cenas;


void initPL(int fd, ParsedLine *pl, size_t init_size);
void resizePL(ParsedLine *pl);
ssize_t readln(ParsedLine *pl);
ssize_t readlnToPL(ParsedLine *pl);

