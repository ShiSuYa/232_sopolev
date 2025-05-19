#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s command infile outfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *command = argv[1];
    char **cmd_args = &argv[1];

    int in_fd = open(argv[2], O_RDONLY);
    if (in_fd < 0) {
        perror("open infile");
        exit(EXIT_FAILURE);
    }

    int out_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0) {
        perror("open outfile");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        if (dup2(in_fd, STDIN_FILENO) < 0 || dup2(out_fd, STDOUT_FILENO) < 0) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(in_fd);
        close(out_fd);
        execvp(command, cmd_args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        close(in_fd);
        close(out_fd);
        int wstatus;
        waitpid(pid, &wstatus, 0);
        if (WIFEXITED(wstatus))
            printf("WEXITSTATUS %d received\n", WEXITSTATUS(wstatus));
    }

    return 0;
}
