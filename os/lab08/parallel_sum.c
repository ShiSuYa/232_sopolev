#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int *array;
int sum = 0;
pthread_mutex_t sum_mutex;

typedef struct {
    int start;
    int end;
} ThreadData;

void* sum_part(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int local_sum = 0;

    for (int i = data->start; i < data->end; i++) {
        local_sum += array[i];
    }

    pthread_mutex_lock(&sum_mutex);
    sum += local_sum;
    pthread_mutex_unlock(&sum_mutex);

    return NULL;
}

int main() {
    int N;
    printf("Enter the number of elements (N >= 16): ");
    scanf("%d", &N);

    if (N < 16) {
        printf("Error: N must be at least 16.\n");
        return 1;
    }

    array = (int*)malloc(N * sizeof(int));
    if (array == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        array[i] = rand() % 100;
    }

    pthread_t threads[4];
    ThreadData thread_data[4];
    int part_size = N / 4;

    pthread_mutex_init(&sum_mutex, NULL);

    for (int i = 0; i < 4; i++) {
        thread_data[i].start = i * part_size;
        thread_data[i].end = (i == 3) ? N : (i + 1) * part_size;
        pthread_create(&threads[i], NULL, sum_part, &thread_data[i]);
    }

    printf("Array elements:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&sum_mutex);
    free(array);

    printf("Total sum = %d\n", sum);

    return 0;
}
