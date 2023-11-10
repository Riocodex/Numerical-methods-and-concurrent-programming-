#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_NUM 10000

// Shared data between threads
int currentNumber = 2; // Starting from 2, the first prime number
int primeCount = 0;    // Counter for the number of prime numbers found
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to check if a number is prime
int isPrime(int num) {
    if (num < 2) {
        return 0; // Not prime
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0; // Not prime
        }
    }
    return 1; // Prime
}

// Thread function to print prime numbers and cancel threads after 5 primes
void *printPrimes(void *threadID) {
    long tid = (long)threadID;

    while (1) {
        pthread_mutex_lock(&mutex);
        int num = currentNumber++;
        pthread_mutex_unlock(&mutex);

        if (num > MAX_NUM) {
            break; // All numbers processed
        }

        if (isPrime(num)) {
            primeCount++;
            printf("Thread %ld: %d is prime. Total primes: %d\n", tid, num, primeCount);

            if (primeCount >= 5) {
                // Cancel all threads after the 5th prime is found
                printf("Cancelling all threads after the 5th prime.\n");
                pthread_cancel(pthread_self());
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    int numThreads;

    // Prompt the user for the number of threads
    printf("Enter the number of threads: ");
    scanf("%d", &numThreads);

    // Validate the number of threads
    if (numThreads <= 0 || numThreads > 100) {
        printf("Please enter a valid number of threads (1-100).\n");
        return 1;
    }

    pthread_t threads[numThreads];
    int rc;
    long t;

    // Create user-specified number of threads
    for (t = 0; t < numThreads; t++) {
        rc = pthread_create(&threads[t], NULL, printPrimes, (void *)t);
        if (rc) {
            printf("Error creating thread; return code from pthread_create() is %d\n", rc);
            return 1;
        }
    }

    // Join threads
    for (t = 0; t < numThreads; t++) {
        rc = pthread_join(threads[t], NULL);
        if (rc && rc != 3) {
            printf("Error joining thread; return code from pthread_join() is %d\n", rc);
            return 1;
        }
    }

    printf("All threads joined.\n");

    pthread_exit(NULL);
}
