#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

volatile long int in = 0;
unsigned long int count_max;


void *run_thread(void *arg) {
	int thread_number = *((unsigned long int*) arg);

	for (int i = 0; i < count_max; i++) {
		int next_free_slot = in;
		next_free_slot += 1;
		in = next_free_slot;
	}

	printf("EXIT THREAD %d\n", thread_number);
}

int main(int argc, char *argv[]) {
	if (argc != 2) exit(EXIT_FAILURE);

	count_max = atoi(argv[1]);

	pthread_t tid[2];
	int thread_number[2] = {0,1};

	pthread_create(&tid[0], NULL, &run_thread, &thread_number[0]);
	pthread_create(&tid[1], NULL, &run_thread, &thread_number[1]);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	printf("EXIT: in=%ld\n", in);

	return 0;
}