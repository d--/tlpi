#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    int option;
    while ((option = getopt(argc, argv, "b")) != -1) {
        switch (option) {
        case 'b':
            printf("butt\n");
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
