#ifndef READLN_H
#define READLN_H

//Mover para .h
typedef struct buffer {
    int fd;
    int size;
    int pt;
    size_t capacity;
    char *body;
} Buffer;


ssize_t read_block(Buffer *buffer, size_t size);

void create_buffer(int fd, Buffer *buffer, size_t initial_capacity);

void destroy_buffer(Buffer *buffer);

ssize_t readln(Buffer *buffer, void *buf, size_t nbyte);

int contaEspacos(char *string);

#endif