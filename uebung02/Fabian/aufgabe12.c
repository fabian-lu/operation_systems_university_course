#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Anzahl der Threads 10
#define NUMTHRDS 10
// Feldgroesse pro Thread
#define CHUNKSIZE 10000

//globals
int *arr;
long sum=0;
pthread_mutex_t lock;

/*************************************************************/
//calc the sum and save it in sum variable
void *gausswasbetteratthis(void *threadid)
{
  //printf("%ld\n",(long)threadid);
  long start = (long)threadid*CHUNKSIZE;
  long end = start+CHUNKSIZE-1;

  pthread_mutex_lock(&lock);
  printf("Start: %ld \t End: %ld \n", start, end);

  for (long i=start; i<=end ; i++){
    sum += arr[i];
  }
  pthread_mutex_unlock(&lock);

  pthread_exit((void*) 0);
}

/*************************************************************/
int main()
{
  long i;
  void* status;
  pthread_t threads[NUMTHRDS];

  //fill array with numbers 1 - 100.000
  arr = (int*) malloc (CHUNKSIZE*NUMTHRDS*sizeof(int));

  for (i=0; i<CHUNKSIZE*NUMTHRDS; i++)
    arr[i] = i+1;

  //Create 10 Threads for summing up numbers.
  //last param is for the gausswasbetteratthis function
  pthread_mutex_init(&lock, NULL);
  for(i=0; i<NUMTHRDS; i++)
    pthread_create(&threads[i], NULL, gausswasbetteratthis, (void *)i);

  // wait for the 10 threads to finish
  for(i=0; i<NUMTHRDS; i++)
    pthread_join(threads[i], &status);

  pthread_mutex_destroy(&lock);

  printf ("Summe der ersten %d Zahlen ist %li\n", NUMTHRDS*CHUNKSIZE, sum);
  free (arr);
  pthread_exit(NULL);
}
/*************************************************************/
