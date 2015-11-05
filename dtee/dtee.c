#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    // get options
    int option;
    while ((option = getopt(argc, argv, "b:")) != -1) {
        switch (option) {
        case 'b':
            printf("butt\n");
            printf("%s\n", optarg);
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    // create a buffer that is the appropriate size 
    struct stat bufstat;
    if (fstat(STDIN_FILENO, &bufstat) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    blksize_t bsize = bufstat.st_blksize;
    static char *buffer;
    if ((buffer = malloc(bsize)) == NULL) {
        fprintf(stderr, "error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    memset(buffer, 0, bsize);

    // write directly from stdin to stdout
    int st;
    while ((st = read(STDIN_FILENO, buffer, bsize)) > 0) {
        if (write(STDOUT_FILENO, buffer, bsize) == -1) {
           perror("write");
           exit(EXIT_FAILURE);
        }
        memset(buffer, 0, bsize);
    }
    if (st == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    free(buffer);
    exit(EXIT_SUCCESS);
}