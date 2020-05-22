#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 1024 


typedef struct parsed_line {
    int fd;          
    int size;                   
    ssize_t mem_size;
    char opt;     
    char *line;                                                   
} ParsedLine;

void initPL(int fd, ParsedLine *pl, size_t init_size);
void resizePL(ParsedLine *pl);
ssize_t readln(ParsedLine *pl);
ssize_t readlnToPL(ParsedLine *pl);

