#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1 = fork();

    if (pid1 < 0) {
        perror("fork 1 failed");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        printf("Hello from First Child (PID %d, Parent PID %d)\n", getpid(), getppid());

        pid_t pid2 = fork();
        if (pid2 < 0) {
            perror("fork 2 failed");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            printf("Hello from Second Child (PID %d, Parent PID %d)\n", getpid(), getppid());
        } else {
            wait(NULL);
        }

    } else {
        wait(NULL);
        printf("Hello from Parent (PID %d)\n", getpid());
    }

    return 0;
}
