#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

volatile long int in;

void *threadFunc(unsigned long int *count_max)
{
  long int next_free_slot = in;
  int i = 0;
  while (i < *count_max) {
    next_free_slot++;
    i++;
  }
  in = next_free_slot;

  pthread_exit((void*) 0);
}


int main(int argc, char *argv[])
{
  //main thread
  in = 0;
  unsigned long int count_max = atoi(argv[1]);

  pthread_t th1, th2;
  pthread_create(&th1, NULL, (void *) threadFunc, &count_max);
  pthread_create(&th2, NULL, (void *) threadFunc, &count_max);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);

  printf("%ld\n", in);
  return 0;
}
