#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s command output_filename [args...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *command = argv[1];
    char *outfile = argv[2];
    char **cmd_args = &argv[1];

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {

        int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }

        close(fd);

        execvp(command, cmd_args);

        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }

    return 0;
}
