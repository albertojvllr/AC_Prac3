#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>

#define N 100000000
#define INI 1500

int p[N/10];
int num_threads;
int pp;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int start;
    int end;
} thread_data_t;

void* find_primes(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    struct timespec start_c, end_c;
    int start = data->start;
    int end = data->end;
    int i, num;
               
    clock_gettime(CLOCK_MONOTONIC, &start_c);
    for (num = start; num < end; num += 2) {
        int div = 0;
        for (i = 1; p[i] * p[i] <= num && !div; i++) {
            div = div || !(num % p[i]);
        }
        if (!div) {
            pthread_mutex_lock(&mutex);
            p[pp++] = num;
            pthread_mutex_unlock(&mutex);
            //printf("Thread %d: Found prime %d\n", start, num);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end_c);
    double elapsed = (end_c.tv_sec - start_c.tv_sec) + (end_c.tv_nsec - start_c.tv_nsec) / 1e9;
    printf("Thread: Time elapsed: %f\n", elapsed);
    pthread_exit(NULL);
}

int main(int na, char* arg[]) {
    int i, nn, num;
    pthread_t* threads;
    thread_data_t* thread_data;

    assert(na == 3); // nombre d'arguments
    nn = atoi(arg[1]);
    num_threads = atoi(arg[2]);
    assert(nn <= N);

    printf("Tots els primers fins a %d\n", nn);

    p[0] = 2;
    p[1] = 3;
    pp = 2;
    num = 5;

    // Inicialización secuencial hasta 1500
    while (pp < INI) {
        for (i = 1; p[i] * p[i] <= num; i++)
            if (num % p[i] == 0) break;
        if (p[i] * p[i] > num) p[pp++] = num;
        num += 2;
    }

    threads = malloc(num_threads * sizeof(pthread_t));
    thread_data = malloc(num_threads * sizeof(thread_data_t));

    int range = (nn - (INI + 1)) / num_threads;
    for (i = 0; i < num_threads; i++) {
        thread_data[i].start = (INI + 1) + i * range;
        thread_data[i].end = (i == num_threads - 1) ? nn : (INI + 1) + (i + 1) * range;
        printf("Thread %d: start=%d end=%d\n", i, thread_data[i].start, thread_data[i].end);
        pthread_create(&threads[i], NULL, find_primes, (void*)&thread_data[i]);
    }

    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Hi ha %d primers\n", pp - 1);
    printf("Darrer primer trobat %d\n", p[pp - 1]);

    free(threads);
    free(thread_data);

    return 0;
}