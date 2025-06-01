#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <timeout>\n", argv[0]);
        return 1;
    }

    char *endptr;
    long timeout = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || timeout <= 0) {
        fprintf(stderr, "Invalid timeout: %s\n", argv[1]);
        return 1;
    }

    int counter = 0;
    pid_t pid = getpid();

    while (1) {
        printf("%d: %d\n", pid, counter++);
        fflush(stdout);
        sleep(timeout);
    }

    return 0;
}
