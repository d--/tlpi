#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    off_t off;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "usage: %s pathname offset\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((fd = open(argv[1], O_RDWR | O_CREAT, 0644)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    off = atoll(argv[2]);
    if (lseek(fd, off, SEEK_SET) == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    if (write(fd, "test", 4) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
