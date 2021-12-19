#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>

#define N 4

volatile long int in = 0;
unsigned long int count_max;


void *run_thread(void *arg) {
	int thread_number = *((unsigned long int*) arg);

	for (int i = 0; i < count_max; i++) {
		

		// Critical section start
		int next_free_slot = in;
		next_free_slot += 1;
		in = next_free_slot;
		// Critical section end

		
	}

	printf("EXIT THREAD %d\n", thread_number);
}

int main(int argc, char *argv[]) {
	if (argc != 2) exit(EXIT_FAILURE);

	count_max = atoi(argv[1]);

	pthread_t tid[N];
	int thread_number[N];

	for (int i = 0; i < N; i++) {
		thread_number[i] = i;
		pthread_create(&tid[i], NULL, &run_thread, &thread_number[i]);
	}

	for (int i = 0; i < N; i++) {
		pthread_join(tid[i], NULL);
	}
	
	printf("EXIT: in=%ld\n", in);

	return 0;
}