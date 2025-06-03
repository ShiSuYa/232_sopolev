#define _GNU_SOURCE
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define QUEUE_NAME "/mqserver"

int main() {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;

    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0600, &attr);
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

    while (1) {
        ssize_t bytes = mq_receive(mq, buf, attr.mq_msgsize, NULL);
        if (bytes == -1) {
            perror("mq_receive");
            free(buf);
            mq_close(mq);
            mq_unlink(QUEUE_NAME);
            return 1;
        }

        if (bytes >= attr.mq_msgsize)
            buf[attr.mq_msgsize - 1] = '\0';
        else
            buf[bytes] = '\0';

        if (strcmp(buf, "QUIT") == 0) {
            break;
        }

        printf("Received: %s\n", buf);
        fflush(stdout);
    }

    free(buf);
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    return 0;
}
