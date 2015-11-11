#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t create_read_buffer(char **buffer);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: dumb_seek file_name\n");
        exit(EXIT_FAILURE);
    }

    // open a file with the append flag
    int fd;
    int flags = O_CREAT | O_APPEND | O_WRONLY;
    if ((fd = open(argv[1], flags, 0644)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char *buffer;
    size_t bsize = create_read_buffer(&buffer);

    int read_len;
    while ((read_len = read(STDIN_FILENO, buffer, bsize)) > 0) {
        off_t cursor;
        if ((cursor = lseek(fd, 0, SEEK_SET)) == -1) {
            perror("lseek");
            exit(EXIT_FAILURE);
        }
        printf("write cursor set to %ld\n", cursor);

        int write_len;
        if ((write_len = write(fd, buffer, read_len)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);    
        }
        if (write_len != read_len) {
            fprintf(stderr, "partial write\n");
            exit(EXIT_FAILURE);
        }
    }
    if (read_len == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
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
