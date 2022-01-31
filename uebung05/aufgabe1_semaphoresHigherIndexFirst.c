#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

// number of philosophers / chopsticks
#define N 5

//stick [n] <0 stick is on the table stick [n]=m philosopher m has stick n
int stick[N];

//mutex semaphores
sem_t semaphore[N];
//for tablePrinter
unsigned long int eatCount[N];

void tablePrinter(int philosopher);
void P(int *index);

void P(int *index) {
  int i = *index;
  int philosopher = i;
  int left = (i +1)% N;
  int right = i;

  int first, second;
  first = left; //stick mit dem größren Index wird in first
  second = right; //gespeichert, der andere in second
  if(left < right){
    first = right;
    second = left;
  }

  while(1) {
    //printf("%d\n", stick[left]);
    sem_wait(&semaphore[first]);
    stick[first] = i;
    //printf("%d\n", stick[right]);
    sem_wait(&semaphore[second]);
    stick[second] = i;

    tablePrinter(philosopher);

    //printf("Philosopher %d PUTS chopsticks BACK on table!\n",philosopher);
    stick[second] = -1;
    sem_post(&semaphore[second]);
    stick[first] = -1;
    sem_post(&semaphore[first]);
  }
}

void tablePrinter(int philosopher) {
  eatCount[philosopher]++;
  for(int i = 0; i < N; i++) {
      printf("Philosopher %d ate %ld times\n", i, eatCount[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  printf("START MAIN\n");
  //initialize chopstick array
  for(int i = 0; i < N; i++) {
    stick[i] = -1;
  }

  //initialize eatingCount array
  for(int i = 0; i < N; i++) {
    eatCount[i] = 0;
  }

  //initialize semaphores
  for(int i = 0; i < N; i++) {
    sem_init(&semaphore[i], 0, 1); //1 = available
  }

  pthread_t threads[N];
  for(int i = 0; i < N; i++) {
    pthread_create(&threads[i], NULL, (void *) P, &i);
  }

  for(int i = 0; i < N; i++) {
    pthread_join(threads[i], NULL);
  }

  //destroy semaphores
  for(int i = 0; i < N; i++) {
    sem_destroy(&semaphore[i]);
  }

  printf("END MAIN\n");
}
