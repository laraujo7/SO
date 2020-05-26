#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct output {
    size_t tam;
    size_t pointer;
} Output;

int save_output(size_t pt, size_t tam)
{
    int fd;

    if ((fd = open("out/log.idx", O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1) {
        perror("error");
        return -1;
    }

    Output output = {
        .tam = tam,
        .pointer = pt,
    };

    write(fd, &output, sizeof(Output));

    close(fd);

    return 0;
}
