#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define ASSERT(cond, err) { \
    if(cond) { \
        fprintf(stderr, err); \
        exit(EXIT_FAILURE); \
    } \
}

int main(int argc, char *argv[])
{
    // get options
    int option;
    int append = 0;
    while ((option = getopt(argc, argv, "a")) != -1) {
        switch (option) {
        case 'a':
            append = 1;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    // get other args
    int have_more_args = (optind < argc);
    int fout = 0;
    char *fname = NULL;
    if (have_more_args) {
        fout = 1;
        fname = argv[optind];
    }
    ASSERT(fname == NULL && append, "-a: Specify a filename.\n");

    // create a buffer that is the appropriate size 
    struct stat bufstat;
    if (fstat(STDIN_FILENO, &bufstat) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    blksize_t bsize = bufstat.st_blksize;
    static char *buffer;
    ASSERT((buffer = malloc(bsize)) == NULL, "error allocating memory\n");
    memset(buffer, 0, bsize);

    // open file
    int fd;
    if (fout) {
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
    }

    // write to stdout and file
    int readlen;
    while ((readlen = read(STDIN_FILENO, buffer, bsize)) > 0) {
        if (write(STDOUT_FILENO, buffer, readlen) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        if (fout && write(fd, buffer, readlen) == -1) {
            perror("file write");
            exit(EXIT_FAILURE);
        }
        memset(buffer, 0, bsize);
    }
    if (readlen == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    free(buffer);
    exit(EXIT_SUCCESS);
}
