#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define N 4
volatile long int in;
//mutex
sem_t semaphore[N];

void *threadFunc(int *args)
{
  int i = 0;
  while (i < args[1]) {
    sem_wait(&semaphore[args[0] - 1]);
    printf("Criticical Section of Thread: %d\n", args[0]);
    long int next_free_slot = in;


    next_free_slot++;
    i++;
    in = next_free_slot;
    sem_post(&semaphore[args[0] % N]);
  }

  pthread_exit((void*) 0);
}

int main(int argc, char *argv[])
{
  //main thread
  in = 0;
  unsigned long int count_max = atoi(argv[1]);

  pthread_t th1, th2, th3, th4;

  // init semaphores
  sem_init(&semaphore[0], 0, 1); // me. count = 1, start with first thread
  sem_init(&semaphore[1], 0, 0); // me. count = 0
  sem_init(&semaphore[2], 0, 0); // me. count = 0
  sem_init(&semaphore[3], 0, 0); // me. count = 0

  int argsP1[2] = {1, count_max};
  int argsP2[2] = {2, count_max};
  int argsP3[2] = {3, count_max};
  int argsP4[2] = {4, count_max};
  pthread_create(&th1, NULL, (void *) threadFunc, argsP1);
  pthread_create(&th2, NULL, (void *) threadFunc, argsP2);
  pthread_create(&th3, NULL, (void *) threadFunc, argsP3);
  pthread_create(&th4, NULL, (void *) threadFunc, argsP4);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);
  pthread_join(th4, NULL);

  // destroy
  sem_destroy(&semaphore[0]);
  sem_destroy(&semaphore[1]);
  sem_destroy(&semaphore[2]);
  sem_destroy(&semaphore[3]);

  printf("%ld\n", in);
  return 0;
}
