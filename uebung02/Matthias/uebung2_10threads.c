#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>

#define NUM_THREADS 10

// Schreibe die Thread-ID und beende
void* print_hello(void* threadid) {
    pthread_t thread_id = pthread_self();
    printf("Hallo von Thread %lu\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    // Erstelle die threads
    for (int i = 0; i < NUM_THREADS; i++) {
        errno = pthread_create(&threads[i], NULL, print_hello, NULL);
        if (errno != 0) {
            perror("Error creating threads");
            return -1;
        }
    }
    // Joine alle threads
    for (int i = 0; i < NUM_THREADS; i++) {
        errno = pthread_join(threads[i], NULL);
        if (errno != 0) {
            perror("Join failed");
            return -1;
        }
    }
}