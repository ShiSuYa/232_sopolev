#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

struct sigmap {
    const char *name;
    int number;
};

static struct sigmap signals[] = {
    {"HUP", SIGHUP},     {"INT", SIGINT},     {"QUIT", SIGQUIT},
    {"ILL", SIGILL},     {"ABRT", SIGABRT},   {"FPE", SIGFPE},
    {"KILL", SIGKILL},   {"SEGV", SIGSEGV},   {"PIPE", SIGPIPE},
    {"ALRM", SIGALRM},   {"TERM", SIGTERM},   {"USR1", SIGUSR1},
    {"USR2", SIGUSR2},   {"CHLD", SIGCHLD},   {"CONT", SIGCONT},
    {"STOP", SIGSTOP},   {"TSTP", SIGTSTP},   {"TTIN", SIGTTIN},
    {"TTOU", SIGTTOU},   {"BUS", SIGBUS},     {"POLL", SIGPOLL},
    {"PROF", SIGPROF},   {"SYS", SIGSYS},     {"TRAP", SIGTRAP},
    {"URG", SIGURG},     {"VTALRM", SIGVTALRM},{"XCPU", SIGXCPU},
    {"XFSZ", SIGXFSZ},   {"WINCH", SIGWINCH}, {NULL, 0}
};

void handler(int signum) {
    fprintf(stdout, "[Caught: %s]", strsignal(signum));
    fflush(stdout);
}

int signal_number(const char *name) {
    for (int i = 0; signals[i].name != NULL; i++) {
        if (strcmp(signals[i].name, name) == 0) {
            return signals[i].number;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <interval> <SIGNAL_NAME>...\n", argv[0]);
        return 1;
    }

    int interval = atoi(argv[1]);
    if (interval <= 0) {
        fprintf(stderr, "Invalid interval: %s\n", argv[1]);
        return 1;
    }

    for (int i = 2; i < argc; i++) {
        int sig = signal_number(argv[i]);
        if (sig == -1) {
            fprintf(stderr, "No such signal: %s\n", argv[i]);
            return 1;
        }
        if (signal(sig, handler) == SIG_ERR) {
            perror("signal");
            return 1;
        }
    }

    pid_t pid = getpid();
    int counter = 0;

    while (1) {
        printf("%d: %d\n", pid, counter++);
        fflush(stdout);
        sleep(interval);
    }

    return 0;
}
