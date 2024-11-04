#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
    for (int i = 0; i < atoi(argv[argc - 1]); i++) {
        printf("%d\n", getpid());
    }
    abort();
}