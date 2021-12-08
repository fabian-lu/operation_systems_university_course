#include <stdio.h>
#include <pthread.h>

#define N 10

void *print_hello(void *threadid) {
	pthread_t thread_id = pthread_self();
	printf("Hallo von Thread %ld\n", thread_id);
}


int main() {
	pthread_t threads[N];

	for (int i=0; i<N; i++) {
		pthread_create(&threads[i], NULL, print_hello, NULL);
	}

	for (int i=0; i<N; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}