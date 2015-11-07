#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Please specify a source file.\n");
        exit(EXIT_FAILURE);
    }
    int src_fd, src_flags;
    src_flags = O_RDONLY;
    if ((src_fd = open(argv[1], src_flags)) == -1) {
        perror("source file");
        exit(EXIT_FAILURE);
    }

    if (argc < 3) {
        fprintf(stderr, "Please specify a destination file.\n");
        exit(EXIT_FAILURE);
    }
    int dest_fd, dest_flags;
    dest_flags = O_WRONLY | O_CREAT | O_TRUNC;
    if ((dest_fd = open(argv[2], dest_flags, 0644)) == -1) {
        perror("destination file");
        exit(EXIT_FAILURE);
    }

    return 0;
}
