#include "client.h"

int sfifo_fd;
int cfifo_fd;

void sigint_handler(int signum)
{
    printf("\n\"Unlinking\" client fifo...\n");
    if (unlink(CLIENT) == -1) {
        perror("unlink");
        exit(-1);
    }
    printf("...client fifo \"unlinked\".\n\n");

    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);

    printf("Making client fifo...\n");
    if (mkfifo(CLIENT, 0666) == -1) {
        unlink(CLIENT);
        perror("mkfifo");
        return -1;
    }
    printf("...cliente fifo done.\n\n");

    printf("Waiting for server to open server fifo...\n");
    if ((sfifo_fd = open(SERVER, O_WRONLY)) == -1) {
        perror("open");
        return -1;
    }
    printf("...server fifo opened.\n\n");

    printf("Waiting for server to open client fifo...\n");
    if ((cfifo_fd = open(CLIENT, O_RDONLY)) == -1) {
        perror("open");
        return -1;
    }
    printf("...client fifo opened.\n\n");

    ReadlnBuffer *rb = (ReadlnBuffer *)calloc(1, sizeof(ReadlnBuffer));
    initRB(0, rb, MAX_BUFFER);

    //ParsedLine *pl = calloc(1, sizeof(ParsedLine));
    ParsedLine pl;

    if (argc == 1) {
        ssize_t bytes_read = 1;
        while (bytes_read) {
            char *prompt = "argus$ ";
            if (write(STDIN_FILENO, prompt, strlen(prompt)) == -1) {
                perror("write");
                return -1;
            }
            if ((bytes_read = readlnToPL(rb, &pl)) > 1) {
                if (write(sfifo_fd, &pl, sizeof(ParsedLine)) == -1) {
                    perror("write");
                    return -1;
                }

                char response[4096];
                ssize_t bytes_output;

                do {
                    if ((bytes_output = read(cfifo_fd, &response, 4096)) == -1) {
                        perror("read");
                        return -1;
                    }
                    if (write(STDOUT_FILENO, response, bytes_output) == -1) {
                        perror("write");
                        return -1;
                    }
                } while (bytes_output == 4096);

                if (bytes_output == -1) {
                    perror("read");
                    return -1;
                }
            }
        }
    } else {
        pl.opt = argv[1][1];
        //pl->arg = argv[2];
        strcpy(pl.arg, argv[2]);

        if (validate(argv[1], &pl) == -1) {
            char *invalid = "Invalid comand use \"ajuda\" (option -h) for help\n";
            write(1, invalid, strlen(invalid));
        } else {
            write(sfifo_fd, &pl, sizeof(ParsedLine));
        }
    }

    printf("Closing client fifo...\n");
    if (close(cfifo_fd) == -1) {
        perror("close");
        return -1;
    }
    printf("...client fifo closed.\n\n");

    printf("Closing server fifo...\n");
    if (close(sfifo_fd) == -1) {
        perror("close");
        return -1;
    }
    printf("...server fifo closed.\n\n");

    if (unlink(CLIENT) == -1) {
        perror("unlink");
        return -1;
    }

    return 0;
}
