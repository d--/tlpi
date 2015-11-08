#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t create_read_buffer(char **buffer);

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

    static char *buffer;
    size_t bsize = create_read_buffer(&buffer);

    // simple cp
    int read_len;
    while ((read_len = read(src_fd, buffer, bsize)) > 0) {
        if (write(dest_fd, buffer, read_len) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    if (read_len == -1) {
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
