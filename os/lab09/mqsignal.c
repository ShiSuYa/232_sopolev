#define _GNU_SOURCE
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define QUEUE_NAME "/mqsignal"

volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    (void)sig;
    stop = 1;
}

int main() {
    struct mq_attr attr;
    mqd_t mq;

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0600, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
        return 1;
    }

    char *buf = malloc(attr.mq_msgsize);
    if (!buf) {
        perror("malloc");
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
        return 1;
    }

    while (!stop) {
        ssize_t bytes = mq_receive(mq, buf, attr.mq_msgsize, NULL);
        if (bytes >= 0) {
            if (bytes >= attr.mq_msgsize)
                buf[attr.mq_msgsize - 1] = '\0';
            else
                buf[bytes] = '\0';

            printf("Received: %s\n", buf);
            fflush(stdout);

            if (strcmp(buf, "QUIT") == 0) {
                break;
            }
        } else {
            if (errno == EINTR) {
                break;
            } else {
                perror("mq_receive");
                free(buf);
                mq_close(mq);
                mq_unlink(QUEUE_NAME);
                return 1;
            }
        }
    }

    free(buf);
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    printf("Server stopped\n");
    return 0;
}
