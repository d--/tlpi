#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

size_t create_read_buffer(char **buffer);
int open_file(char *fname, int append);

int main(int argc, char *argv[])
{
    int append = 0;
    int opt;
    while ((opt = getopt(argc, argv, "a")) != -1) {
        switch (opt) {
        case 'a':
            append = 1;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    int output_to_file = (optind < argc);
    char *fname;
    if (output_to_file) {
        fname = argv[optind];
    } else if (append) {
        fprintf(stderr, "-a: Specify a filename.\n");
        exit(EXIT_FAILURE);
    }
    
    static char *buffer;
    size_t bsize = create_read_buffer(&buffer);

    int fd;
    if (output_to_file)
        fd = open_file(fname, append);

    int readlen;
    while ((readlen = read(STDIN_FILENO, buffer, bsize)) > 0) {
        if (write(STDOUT_FILENO, buffer, readlen) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        if (output_to_file && write(fd, buffer, readlen) == -1) {
            perror("file write");
            exit(EXIT_FAILURE);
        }
    }
    if (readlen == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    free(buffer);
    exit(EXIT_SUCCESS);
}

size_t create_read_buffer(char **buffer)
{
    struct stat stdin_stat;
    if (fstat(STDIN_FILENO, &stdin_stat) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    blksize_t bsize = stdin_stat.st_blksize;
    if ((*buffer = calloc(bsize, 1)) == NULL) {
        fprintf(stderr, "error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    return bsize;
}

int open_file(char *fname, int append)
{
    int fd;
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    if (append) {
        flags |= O_APPEND;
        flags ^= O_TRUNC;
    }
    fd = open(fname, flags, 0644); 
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}
