#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

volatile long int in = 0;
unsigned long int count_max;

volatile bool flag[2] = {false, false};
volatile int turn;


void *run_thread(void *arg) {
	int thread_number = *((unsigned long int*) arg);

	for (int i = 0; i < count_max; i++) {
		flag[thread_number] = true;
		asm volatile("mfence" ::: "memory");
		turn = !thread_number;
		asm volatile("mfence" ::: "memory");
		while (flag[!thread_number] && turn == !thread_number);
		asm volatile("mfence" ::: "memory");

		// Critical section start
		int next_free_slot = in;
		next_free_slot += 1;
		in = next_free_slot;
		// Critical section end

		flag[thread_number] = false;
		asm volatile("mfence" ::: "memory");
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