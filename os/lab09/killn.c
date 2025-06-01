#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

typedef struct {
    const char *name;
    int number;
} SignalEntry;

SignalEntry signals[] = {
    {"HUP", SIGHUP}, {"INT", SIGINT}, {"QUIT", SIGQUIT}, {"ILL", SIGILL},
    {"ABRT", SIGABRT}, {"FPE", SIGFPE}, {"KILL", SIGKILL}, {"SEGV", SIGSEGV},
    {"PIPE", SIGPIPE}, {"ALRM", SIGALRM}, {"TERM", SIGTERM}, {"USR1", SIGUSR1},
    {"USR2", SIGUSR2}, {"CHLD", SIGCHLD}, {"CONT", SIGCONT}, {"STOP", SIGSTOP},
    {"TSTP", SIGTSTP}, {"TTIN", SIGTTIN}, {"TTOU", SIGTTOU}
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s PID SIGNAL_NAME\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    const char *signal_name = argv[2];

    int found = 0;
    int sig = 0;

    for (size_t i = 0; i < sizeof(signals) / sizeof(SignalEntry); ++i) {
        if (strcmp(signals[i].name, signal_name) == 0) {
            sig = signals[i].number;
            found = 1;
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "No such signal\n");
        return 1;
    }

    if (kill(pid, sig) == -1) {
        perror("Failed to send signal");
        return 1;
    }

    return 0;
}
