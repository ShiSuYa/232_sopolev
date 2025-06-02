#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <queue_name> <message>\n", argv[0]);
        return 1;
    }

    const char *queue_name = argv[1];
    const char *message = argv[2];

    mqd_t mq = mq_open(queue_name, O_WRONLY);
    if (mq == (mqd_t) -1) {
        perror("mq_open");
        return 1;
    }

    if (mq_send(mq, message, strlen(message), 1) == -1) {
        perror("mq_send");
        mq_close(mq);
        return 1;
    }

    if (mq_close(mq) == -1) {
        perror("mq_close");
        return 1;
    }

    return 0;
}
