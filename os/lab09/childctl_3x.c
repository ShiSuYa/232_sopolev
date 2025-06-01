#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

volatile sig_atomic_t quit_count = 0;
int quit_signal = 0;

const char *signal_to_str(int signum) {
    const char *desc = strsignal(signum);
    return desc ? desc : "Unknown";
}

void signal_handler(int signum) {
    printf("[Caught: %s]\n", signal_to_str(signum));
    fflush(stdout);

    if (signum == quit_signal) {
        quit_count++;
    }
}

int get_signal_number(const char *name) {
    if (strcasecmp(name, "INT") == 0) return SIGINT;
    if (strcasecmp(name, "QUIT") == 0) return SIGQUIT;
    if (strcasecmp(name, "TERM") == 0) return SIGTERM;
    for (int i = 1; i < NSIG; ++i) {
        if (strcasestr(sys_siglist[i], name)) return i;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <timeout> <quit_signal> <signal1> [signal2...]\n", argv[0]);
        return 1;
    }

    int timeout = atoi(argv[1]);
    if (timeout <= 0) {
        fprintf(stderr, "Invalid timeout: %s\n", argv[1]);
        return 1;
    }

    quit_signal = get_signal_number(argv[2]);
    if (quit_signal == -1) {
        fprintf(stderr, "Invalid quit signal: %s\n", argv[2]);
        return 1;
    }

    for (int i = 3; i < argc; ++i) {
        int signum = get_signal_number(argv[i]);
        if (signum == -1) {
            fprintf(stderr, "Invalid signal: %s\n", argv[i]);
            return 1;
        }

        struct sigaction sa;
        sa.sa_handler = signal_handler;
        sa.sa_flags = SA_RESTART;
        if (sigemptyset(&sa.sa_mask) == -1) {
            perror("sigemptyset");
            return 1;
        }

        if (sigaction(signum, &sa, NULL) == -1) {
            perror("sigaction");
            return 1;
        }
    }

    int counter = 0;
    while (quit_count < 3) {
        printf("Parent heartbeat: %d\n", counter++);
        fflush(stdout);
        if (sleep(timeout) != 0 && errno != 0) {
            perror("sleep");
        }
    }

    printf("Exiting after 3 %s signals\n", argv[2]);
    fflush(stdout);
    return 0;
}
