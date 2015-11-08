#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int fd;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    if ((fd = open("holy_file", flags, 0644)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char *start = "mind the gap";
    if (write(fd, start, 12) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    off_t cursor;
    if ((cursor = lseek(fd, 500000, SEEK_END)) == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    } 

    char *end = "what a gap!";
    if (write(fd, end, 11) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
