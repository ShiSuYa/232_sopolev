#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_SIGNALS 64

volatile sig_atomic_t quit_received = 0;
int quit_signal = 0;

const char *signal_to_str(int signum) {
    const char *str = strsignal(signum);
    return str ? str : "Unknown";
}

void signal_handler(int signum) {
    printf("[Caught: %s]\n", signal_to_str(signum));
    fflush(stdout);

    if (signum == quit_signal) {
        quit_received = 1;
    }
}

int get_signal_number(const char *name) {
    int i = 1;
    while (i < NSIG) {
        if (strcasestr(sys_siglist[i], name)) {
            return i;
        }
        i++;
    }

    if (strcasecmp(name, "INT") == 0) return SIGINT;
    if (strcasecmp(name, "QUIT") == 0) return SIGQUIT;
    if (strcasecmp(name, "TERM") == 0) return SIGTERM;
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

    for (int i = 3; i < argc; i++) {
        int sig = get_signal_number(argv[i]);
        if (sig == -1) {
            fprintf(stderr, "Invalid signal: %s\n", argv[i]);
            return 1;
        }

        struct sigaction sa = {0};
        sa.sa_handler = signal_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        if (sigaction(sig, &sa, NULL) == -1) {
            perror("sigaction");
            return 1;
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        while (1) {
            pause();
        }
    } else {
        int counter = 0;
        while (!quit_received) {
            printf("Parent heartbeat: %d\n", counter++);
            fflush(stdout);
            sleep(timeout);
        }

        printf("Exiting gracefully on signal %s\n", argv[2]);
        fflush(stdout);

        kill(pid, SIGTERM);
        waitpid(pid, NULL, 0);
        return 0;
    }
}
