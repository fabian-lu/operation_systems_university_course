#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// number of philosophers / chopsticks
#define N 5

//stick [n] <0 stick is on the table stick [n]=m philosopher m has stick n
int stick[N];

void P(int i) {
  int philosopher = i;
  int left = (i +1)% N;
  int right = i;

  while(1) {
    //printf("%d\n", stick[left]);
    while(stick[left] >= 0) {
      //printf("Philosopher %d waits for LEFT chopstick!\n", philosopher);
    };
    stick[left] = i;
    //printf("%d\n", stick[right]);
    while(stick[right] >= 0) {
      //printf("Philosopher %d waits for RIGHT chopstick!\n", philosopher);
    };
    stick[right] = i;

    printf("Philosopher %d EATS now!\n",philosopher);

    //printf("Philosopher %d PUTS chopsticks BACK on table!\n",philosopher);
    stick[right] = -1;
    stick[left] = -1;
  }
}

int main(int argc, char *argv[]) {
  printf("START MAIN\n");
  //initialize chopstick array
  for(int i = 0; i < N; i++) {
    stick[i] = -1;
  }

  pthread_t threads[N];
  for(int i = 0; i < N; i++) {
    pthread_create(&threads[i], NULL, (void *) P, i);
  }

  for(int i = 0; i < N; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("END MAIN\n");
}
