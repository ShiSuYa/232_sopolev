#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s infile outfile command [args...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int infd = open(argv[1], O_RDONLY);
    if (infd < 0) {
        perror("open infile");
        exit(EXIT_FAILURE);
    }

    int outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfd < 0) {
        perror("open outfile");
        close(infd);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(infd);
        close(outfd);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        if (dup2(infd, STDIN_FILENO) < 0) {
            perror("dup2 infile");
            exit(EXIT_FAILURE);
        }

        if (dup2(outfd, STDOUT_FILENO) < 0) {
            perror("dup2 outfile");
            exit(EXIT_FAILURE);
        }

        close(infd);
        close(outfd);

        execvp(argv[3], &argv[3]);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    close(infd);
    close(outfd);

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
        printf("WEXITSTATUS: %d\n", WEXITSTATUS(status));
    }

    return 0;
}
