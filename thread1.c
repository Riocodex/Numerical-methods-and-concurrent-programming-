#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MIN_THREADS 1
#define MAX_THREADS 100
#define ITERATIONS 1000

void *threadFunction(void *threadID){
    long tid;
    tid = (long)threadID;

    for(int i = 0; i < ITERATIONS; i++){
        printf("Thread %ld: i=%d\n", tid, i);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return -1;
    }

    int numThreads = atoi(argv[1]);
    
    if (numThreads < MIN_THREADS || numThreads > MAX_THREADS) {
        printf("Number of threads must be between %d and %d\n", MIN_THREADS, MAX_THREADS);
        return -1;
    }

    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    if (threads == NULL) {
        perror("Error allocating memory for threads");
        return -1;
    }

    int rc;
    long t;
    struct timeval start, end;

    gettimeofday(&start, NULL);

    for(t = 0; t < numThreads; t++){
        rc = pthread_create(&threads[t], NULL, threadFunction, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(t = 0; t < numThreads; t++){
        pthread_join(threads[t], NULL);
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Time taken: %f seconds\n", elapsed);

    free(threads);
    pthread_exit(NULL);
}
