#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if ((mkfifo("fifo", 0666)) < 0) {
        perror("mkfifo");
        return -1;
    }

    int log;

    if ((log = open("log", O_CREAT | TRUNC | O_WRONLY, 0644)) < 0) {
        perror("open");
        return -1;
    }

    int fifo_fd;

    while ((fifo_fd = open("fifo", O_RDONLY)) > 0) {
        char buf[BUFSIZ];
        int bytes_read;

        while ((bytes_read = read(fifo_fd, buf, BUFSIZ)) > 0) {
            if (bytes_read < 0) {
                perror("read");
                return -1;
            }

            switch (fork()) {
                case -1:
                    perror("fork");
                    return -1;
                case 0:
                    //executar(buf);
                    exit(0);
                    break;
                default:
                    wait(NULL);
            }

            if (close(fifo_fd) < 0) {
                perror("close");
                return -1;
            }
        }
    }

    if (fifo_fd < 0) {
        perror("open");
        return -1;
    }

    if (close(log) < 0) {
        perror("close");
        return -1;
    }

    return 0;
}
